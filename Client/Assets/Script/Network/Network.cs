﻿using System.Collections;
using System.Collections.Generic;
using UnityEngine;
//---------------------------------------------------------------
// Network
using System.Text;
using System.Net;
using System.Net.Sockets;
using System.Threading; // ManualResetEvent
using System.Runtime.InteropServices; // sizeof

using System;
using System.ComponentModel;
using System.Linq;
using FlatBuffers;
using Game.TheLastOne; // Client, Vec3 을 불러오기 위해
                       //---------------------------------------------------------------

public struct Client_Data
{
    public int id;
    public Vector3 Client_xyz;  // 클라이언트 위치
    public Vector3 view;    // 클라이언트 보는 방향
    public bool connect;    // 클라이언트 접속
    public string name;     // 클라이언트 닉네임
    public GameObject Player;

    public Client_Data(Vector3 xyz, Vector3 view, bool connect, GameObject Player)
    {
        this.id = -1;
        this.Client_xyz = xyz;
        this.view = view;
        this.connect = connect;
        this.Player = null;
        this.name = "";
    }
}

public class Network : MonoBehaviour
{
    public static Socket m_Socket;
    public GameObject Player;
    public GameObject OtherPlayer;
    public GameObject Camera;

    Vector3 Player_Pos;
    Vector3 Camera_Pos;

    public string iPAdress = "127.0.0.1";
    public const int kPort = 9000;

    public bool Connect = false;
    private const int MaxClient = 50;    // 최대 동접자수
    public Client_Data[] client_data = new Client_Data[MaxClient];      // 클라이언트 데이터 저장할 구조체
    public int Client_imei = 0;         // 자신의 클라이언트 아이디


    private int ReceivedataLength;                     // Receive Data Length. (byte)
    private byte[] Receivebyte = new byte[2000];    // Receive data by this array to save.
    private string ReceiveString;                     // Receive bytes to Change string. 

    FlatBufferBuilder fbb = new FlatBufferBuilder(1);

    IEnumerator startServer(Socket m_Socket)
    {
        do
        {
            Array.Clear(Receivebyte, 0, Receivebyte.Length);
            m_Socket.Receive(Receivebyte);

            //Receive(m_Socket);

            //m_Socket.BeginReceive(Receivebyte, 0, Receivebyte.Length, SocketFlags.None, new AsyncCallback(ReaderThread), m_Socket);


            //소켓에 리시브가 들어왔을 경우
            int psize = Receivebyte[0]; // 패킷 사이즈
            int ptype = Receivebyte[1]; // 패킷 타입
            Debug.Log("총 사이즈 : " + psize + ", 패킷 타입 : " + ptype);
            ProcessPacket(psize, ptype, Receivebyte);



            yield return null;
        } while (true);


        yield return null;
    }


    void ProcessPacket(int size, int type, byte[] recvPacket)
    {
        if (type == 1)
        {
            // 클라이언트 아이디를 가져온다.
            byte[] t_buf = new byte[size + 1];
            System.Buffer.BlockCopy(recvPacket, 4, t_buf, 0, size); // 사이즈를 제외한 실제 패킷값을 복사한다.
            ByteBuffer revc_buf = new ByteBuffer(t_buf); // ByteBuffer로 byte[]로 복사한다.
            var Get_ServerData = Client_id.GetRootAsClient_id(revc_buf);
            Client_imei = Get_ServerData.Id;
            Debug.Log("클라이언트 아이디 : " + Client_imei);
        }
        else if (type == 2)
        {
            byte[] t_buf = new byte[size + 1];
            System.Buffer.BlockCopy(recvPacket, 4, t_buf, 0, size); // 사이즈를 제외한 실제 패킷값을 복사한다.

            ByteBuffer revc_buf = new ByteBuffer(t_buf); // ByteBuffer로 byte[]로 복사한다.
            var Get_ServerData = Client_info.GetRootAsClient_info(revc_buf);


            client_data[Get_ServerData.Id].Client_xyz = new Vector3(Get_ServerData.Xyz.Value.X, Get_ServerData.Xyz.Value.Y, Get_ServerData.Xyz.Value.Z);
            client_data[Get_ServerData.Id].name = Get_ServerData.Name;
            client_data[Get_ServerData.Id].Player = OtherPlayer;

            //Debug.Log("오브젝트 아이디 : " + Get_ServerData.Id);

            if (client_data[Get_ServerData.Id].connect != true)
            {
                // 클라이언트가 처음 들어와서 프리팹이 없을경우 
                Instantiate(client_data[Get_ServerData.Id].Player, client_data[Get_ServerData.Id].Client_xyz, Quaternion.identity);
                client_data[Get_ServerData.Id].connect = true;
            }
            else
            {
                // 이미 클라이언트가 들어와 있을경우 위치만 옮겨준다.
                client_data[Get_ServerData.Id].Player.transform.position = client_data[Get_ServerData.Id].Client_xyz;
            }

        }
    }

    void Awake()
    {
        Application.runInBackground = true; // 백그라운드에서도 Network는 작동해야한다.
        //=======================================================
        // Socket create.
        m_Socket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
        m_Socket.SetSocketOption(SocketOptionLevel.Socket, SocketOptionName.SendTimeout, 10000);
        m_Socket.SetSocketOption(SocketOptionLevel.Socket, SocketOptionName.ReceiveTimeout, 10000);

        //=======================================================
        // Socket connect.
        try
        {
            IPAddress ipAddr = System.Net.IPAddress.Parse(iPAdress);
            IPEndPoint ipEndPoint = new System.Net.IPEndPoint(ipAddr, kPort);
            m_Socket.Connect(ipEndPoint);
        }
        catch (SocketException SCE)
        {
            Debug.Log("Socket connect error! : " + SCE.ToString());
            return;
        }

        //=======================================================
    }

    void Send_Packet(byte[] packet)
    {
        try
        {
            m_Socket.Send(packet, packet.Length, 0);
        }
        catch (SocketException err)
        {
            Debug.Log("Socket send or receive error! : " + err.ToString());
        }
    }

    void Send_POS(Vector3 Player, Vector3 Camera)
    {
        //var offset = fbb.CreateString("WindowsHyun"); // String 문자열이 있을경우 미리 생성해라.
        fbb.Clear(); // 클리어를 안해주고 시작하면 계속 누적해서 데이터가 들어간다.
        Client_info.StartClient_info(fbb);
        //Client.AddName(fbb, offset); // string 사용
        Client_info.AddXyz(fbb, Vec3.CreateVec3(fbb, Player.x, Player.y, Player.z));
        Client_info.AddRotation(fbb, Vec3.CreateVec3(fbb, Camera.x, Camera.y, Camera.z));
        var endOffset = Client_info.EndClient_info(fbb);
        fbb.Finish(endOffset.Value);



        byte[] packet = fbb.SizedByteArray();
        //Debug.Log(packet.Length);
        byte[] packet_len = BitConverter.GetBytes(packet.Length);
        //Debug.Log(packet_len.Length);
        byte[] real_packet = new byte[packet_len.Length + packet.Length];

        System.Buffer.BlockCopy(packet_len, 0, real_packet, 0, packet_len.Length);
        System.Buffer.BlockCopy(packet, 0, real_packet, packet_len.Length, packet.Length);

        Send_Packet(real_packet);
        //Debug.Log(real_packet.Length);
        //Debug.Log("-----------------------");
        /*
        Array.Clear(packet, 0, packet.Length);
        Array.Clear(packet_len, 0, packet_len.Length);
        Array.Clear(real_packet, 0, real_packet.Length);
        */
    }

    void OnApplicationQuit()
    {
        m_Socket.Close();
        m_Socket = null;
    }

    void Update()
    {

        Player_Pos.x = Player.transform.position.x;
        Player_Pos.y = Player.transform.position.y;
        Player_Pos.z = Player.transform.position.z;
        Camera_Pos.x = Camera.transform.position.x;
        Camera_Pos.y = Camera.transform.position.y;
        Camera_Pos.z = Camera.transform.position.z;
        Send_POS(Player_Pos, Camera_Pos);

        if (Connect == false)
        {
            Connect = true;
            StartCoroutine(startServer(m_Socket));
        }
    }
}
