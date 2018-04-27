using System.Collections;
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
using TheLastOne.SendFunction;
using TheLastOne.GameClass;
using UnityEngine.UI;   // DebugText를 쓰기 위하여
                        //---------------------------------------------------------------

public class NetworkMessage
{
    public int LimitReceivebyte = 4000;                     // Receive Data Length. (byte)
    public byte[] Receivebyte = new byte[4000];    // Receive data by this array to save.
    public byte[] Sendbyte = new byte[4000];
    public int now_packet_size = 0;
    public int prev_packet_size = 0;
    public StringBuilder sb = new StringBuilder();

    public void set_prev(int value)
    {
        this.prev_packet_size = value;
    }
};

public class PacketData
{
    public int p_size = 0;
    public int type_Pos = 0;
    public PacketData(int size, int pos)
    {
        this.p_size = size;
        this.type_Pos = pos;
    }
};

namespace TheLastOne.Game.Network
{
    public class NetworkCtrl : MonoBehaviour
    {
        public static Socket m_Socket;
        public GameObject Player;
        private PlayerCtrl Player_Script;
        public GameObject PrefabPlayer;
        public GameObject Zombie;
        //------------------------------------------
        public GameObject OtherPlayerCollection;
        public GameObject ItemCollection;
        public GameObject ZombieCollection;
        //------------------------------------------
        // 게임 아이템 Object
        public GameObject item_AK47;
        public GameObject item_M16;
        public GameObject item_556;
        public GameObject item_762;
        public GameObject item_AidKit;
        // 게임 차량 Object
        public GameObject Car_UAZ;
        //------------------------------------------
        public Text DebugText;

        Vector3 Player_Position;
        Vector3 Player_Rotation;
        Vector3 Car_Rotation;

        public string iPAdress = "127.0.0.1";
        public const int kPort = 9000;
        private byte[] Sendbyte = new byte[4000];
        private static ManualResetEvent connectDone = new ManualResetEvent(false);

        public static Dictionary<int, Game_ClientClass> client_data = new Dictionary<int, Game_ClientClass>();
        // 클라이언트 데이터 저장할 컨테이너
        public static Dictionary<int, Game_ZombieClass> zombie_data = new Dictionary<int, Game_ZombieClass>();
        // 좀비 데이터 저장할 컨테이너
        public static Dictionary<int, Game_ItemClass> item_Collection = new Dictionary<int, Game_ItemClass>();
        // 클라이언트 아이템 저장할 컨테이너

        Game_ProtocolClass recv_protocol = new Game_ProtocolClass();
        Socket_SendFunction sF = new Socket_SendFunction();
        DangerLineCtrl DangerLineCtrl;


        private static int Client_imei = -1;         // 자신의 클라이언트 아이디
        public int get_imei() { return Client_imei; }
        private string debugString;        // Debug 출력을 위한 string
        private static bool serverConnect = false;  // 서버 연결을 했는지 체크


        IEnumerator SocketCheck()
        {
            if (m_Socket.Connected == true)
            {
                // 서버가 정상적으로 연결 되었을경우
                serverConnect = true;
                StartCoroutine(startPrefab());
                StartCoroutine(playerLocation_Packet());
                StartCoroutine(DrawDebugText());
                StartCoroutine(drawItems());
            }
            yield return null;
        }

        IEnumerator startPrefab()
        {
            do
            {
                // 플레이어 관련한 프리팹
                foreach (var key in client_data.Keys.ToList())
                {
                    if (client_data[key].get_id() == Client_imei)
                        continue;
                    if (client_data[key].get_connect() == true && client_data[key].get_prefab() == false)
                    {
                        // 플레이어가 연결된 상태에서 프리팹 생성이 안되 었을 경우.
                        client_data[key].Player = Instantiate(PrefabPlayer, client_data[key].get_pos(), Quaternion.identity);
                        client_data[key].Player.transform.SetParent(OtherPlayerCollection.transform);

                        client_data[key].script = client_data[key].Player.GetComponent<OtherPlayerCtrl>();
                        client_data[key].set_prefab(true);

                        // 처음 위치를 넣어 줘야 한다. 그러지 않을경우 다른 클라이언트 에서는 0,0 에서부터 천천히 올라오게 보인다
                        client_data[key].Player.transform.position = client_data[key].get_pos();
                    }
                    else if (client_data[key].get_prefab() == true)
                    {
                        // 플레이어 프리팹이 정상적으로 생성 되었을 경우.
                        if (client_data[key].Player.activeSelf == false && client_data[key].get_activePlayer() == true)
                        {
                            // setActive가 꺼져 있을 경우, 코루틴이랑 같이 활성화 시킨다.
                            client_data[key].Player.SetActive(true);
                            client_data[key].script.StartCoroutine(client_data[key].script.createPrefab());
                            client_data[key].set_activePlayer(false);
                        }

                        if (client_data[key].get_inCar() != -1)
                        {
                            // 차량의 탑승 할 경우 캐릭터 콜라이더 비활성화 및 움직임 보간이 아닌 바로바로 이동
                            client_data[key].script.collider_script.enabled = false;
                            client_data[key].script.transform.position = client_data[key].get_pos();
                        }
                        else
                        {
                            // 실제로 캐릭터를 움직이는 것은 코루틴 여기서 움직임을 진행 한다.
                            client_data[key].script.MovePos(client_data[key].get_pos());
                            client_data[key].script.collider_script.enabled = true;

                            Vector3 rotation = client_data[key].get_rot();
                            client_data[key].Player.transform.rotation = Quaternion.Euler(rotation.x, rotation.y, rotation.z);
                        }
                    }
                    if (client_data[key].get_removeClient() == true)
                    {
                        // 플레이어 삭제를 할 경우 SetActive를 꺼준다.
                        client_data[key].set_activePlayer(false);
                        client_data[key].Player.SetActive(false);
                        client_data[key].set_removeClient(false);
                    }
                }

                // 좀비 관련한 프리팹
                foreach (var key in zombie_data.Keys.ToList())
                {
                    if (zombie_data[key].get_prefab() == false)
                    {
                        // 좀비 프리팹을 생성해준다.
                        zombie_data[key].Zombie = Instantiate(Zombie, zombie_data[key].get_pos(), Quaternion.identity);
                        zombie_data[key].Zombie.transform.SetParent(ZombieCollection.transform);
                        zombie_data[key].script = zombie_data[key].Zombie.GetComponent<ZombieCtrl>();
                        zombie_data[key].script.zombieNum = key;
                        zombie_data[key].set_prefab(true);

                        // 처음 위치를 넣어 줘야 한다. 그러지 않을경우 다른 클라이언트 에서는 0,0 에서부터 천천히 올라오게 보인다
                        zombie_data[key].Zombie.transform.position = zombie_data[key].get_pos();
                    }
                    else if (zombie_data[key].get_prefab() == true)
                    {
                        // 좀비 프리팹이 정상적으로 생성 되었을 경우.
                        if (zombie_data[key].Zombie.activeSelf == false && zombie_data[key].get_activeZombie() == true)
                        {
                            // setActive가 꺼져 있을 경우, 코루틴이랑 같이 활성화 시킨다.
                            zombie_data[key].Zombie.SetActive(true);
                            zombie_data[key].script.StartCoroutine(zombie_data[key].script.CheckZombieNav());
                            zombie_data[key].script.StartCoroutine(zombie_data[key].script.CheckZombieState());
                            zombie_data[key].script.StartCoroutine(zombie_data[key].script.ZombieAction());
                            zombie_data[key].set_activeZombie(false);
                        }

                        if (zombie_data[key].script.targetPlayer == -1 || zombie_data[key].Zombie.transform.position.x == 0)
                        {
                            // 포지션이 서버위치와 다를경우 초기화를 해준다.
                            zombie_data[key].Zombie.transform.position = zombie_data[key].get_pos();
                            zombie_data[key].script.zombieNum = key;
                        }
                        else if (zombie_data[key].script.targetPlayer != Client_imei)
                        {
                            // 좀비 Target과 내 IMEI가 다른경우 다른 클라이언트의 데이터를 동기화 해준다.
                            zombie_data[key].script.MovePos(zombie_data[key].get_pos());
                            Vector3 rotation = zombie_data[key].get_rot();
                            zombie_data[key].Zombie.transform.rotation = Quaternion.Euler(rotation.x, rotation.y, rotation.z);
                            zombie_data[key].script.animator_value = zombie_data[key].get_animator();
                        }
                        zombie_data[key].script.targetPlayer = zombie_data[key].get_target();
                    }
                    if (zombie_data[key].get_removeZombie() == true)
                    {
                        // 플레이어 삭제를 할 경우 SetActive를 꺼준다.
                        zombie_data[key].set_activeZombie(false);
                        zombie_data[key].Zombie.SetActive(false);
                        zombie_data[key].set_removeZombie(false);
                    }

                }
                yield return null;
            } while (true);
            //yield return null;
        }

        IEnumerator drawItems()
        {
            do
            {
                if (Client_imei != -1)
                {
                    foreach (KeyValuePair<int, Game_ItemClass> iter in item_Collection)
                    {
                        if (iter.Value.get_draw() == false && iter.Value.get_name() != "")
                        {
                            // 그려져 있지 않을경우 그려준다.
                            if (iter.Value.get_name() == "AK47")
                            {
                                iter.Value.item = (GameObject)Instantiate(item_AK47, iter.Value.get_pos(), Quaternion.identity);
                                iter.Value.item.transform.SetParent(ItemCollection.transform);
                                iter.Value.set_draw(true);
                            }
                            else if (iter.Value.get_name() == "M16")
                            {
                                iter.Value.item = Instantiate(item_M16, iter.Value.get_pos(), Quaternion.identity);
                                iter.Value.item.transform.SetParent(ItemCollection.transform);
                                iter.Value.set_draw(true);
                            }
                            else if (iter.Value.get_name() == "556")
                            {
                                iter.Value.item = Instantiate(item_556, iter.Value.get_pos(), Quaternion.Euler(-90, 0, 0));
                                iter.Value.item.transform.SetParent(ItemCollection.transform);
                                iter.Value.set_draw(true);
                            }
                            else if (iter.Value.get_name() == "762")
                            {
                                iter.Value.item = Instantiate(item_762, iter.Value.get_pos(), Quaternion.Euler(-90, 0, 0));
                                iter.Value.item.transform.SetParent(ItemCollection.transform);
                                iter.Value.set_draw(true);
                            }
                            else if (iter.Value.get_name() == "AidKit")
                            {
                                iter.Value.item = Instantiate(item_AidKit, iter.Value.get_pos(), Quaternion.Euler(-90, 0, 0));
                                iter.Value.item.transform.SetParent(ItemCollection.transform);
                                iter.Value.set_draw(true);
                            }
                            else if (iter.Value.get_name() == "UAZ")
                            {
                                iter.Value.item = Instantiate(Car_UAZ, iter.Value.get_pos(), Quaternion.identity);
                                iter.Value.item.GetComponent<VehicleCtrl>().carNum = iter.Key;  // 해당 차량이 몇번째 차량인지 알려주자.
                                iter.Value.item.transform.SetParent(ItemCollection.transform);
                                iter.Value.set_draw(true);
                            }
                        }
                        else if (iter.Value.get_draw() == true && iter.Value.item.activeInHierarchy == false && iter.Value.get_sendPacket() == false && iter.Value.get_kind() != recv_protocol.Kind_Car)
                        {
                            // 이미 그려진 상태에서 아이템이 먹어졌을 경우, 서버로 아이템을 먹었다고 보내야 한다.
                            iter.Value.set_sendPacket(true);
                            Sendbyte = sF.makeEatItem_PacketInfo(iter.Value.get_id());
                            Send_Packet(Sendbyte);
                        }
                        else if (iter.Value.get_eat() == true && iter.Value.item != null && iter.Value.item.activeInHierarchy == true)
                        {
                            // Item이 정상적으로 나왔다가 다른 사람이 먹었을 경우에 대한 처리
                            iter.Value.item.SetActive(false);
                        }
                        if (iter.Value.get_kind() == recv_protocol.Kind_Car && Player_Script.CarNum != iter.Key)
                        {
                            // 차량의 경우 지속적으로 위치를 갱신 해준다.
                            iter.Value.item.transform.position = Vector3.MoveTowards(iter.Value.item.transform.position, iter.Value.get_pos(), Time.deltaTime * 4000.0f);

                            iter.Value.item.transform.rotation = Quaternion.Euler(iter.Value.get_rotation().x, iter.Value.get_rotation().y, iter.Value.get_rotation().z);
                        }
                    }
                }
                yield return new WaitForSeconds(0.1f);
            } while (true);
            //yield return null;
        }

        IEnumerator playerLocation_Packet()
        {
            do
            {
                if (Client_imei != -1)
                {
                    Player_Position.x = Player.transform.position.x;
                    Player_Position.y = Player.transform.position.y;
                    Player_Position.z = Player.transform.position.z;
                    Player_Rotation.x = Player.transform.eulerAngles.x;
                    Player_Rotation.y = Player.transform.eulerAngles.y;
                    Player_Rotation.z = Player.transform.eulerAngles.z;
                    if (Player_Script.CarNum != -1)
                    {
                        Car_Rotation.x = Player_Script.ridingCar.vehicle_tr.eulerAngles.x;
                        Car_Rotation.y = Player_Script.ridingCar.vehicle_tr.eulerAngles.y;
                        Car_Rotation.z = Player_Script.ridingCar.vehicle_tr.eulerAngles.z;
                    }
                    Enum get_int_enum = Player_Script.playerState;

                    // 플레이어 데이터 보내주기
                    Sendbyte = sF.makeClient_PacketInfo(Player_Position, Convert.ToInt32(get_int_enum), Player_Script.h, Player_Script.v, Player_Rotation, Player_Script.now_Weapon, Player_Script.CarNum, Car_Rotation);
                    Send_Packet(Sendbyte);

                    //좀비 데이터 보내주기
                    if (zombie_data.Count != 0)
                    {
                        Sendbyte = sF.makeZombie_PacketInfo(zombie_data, Client_imei);
                        if (Sendbyte != null)
                            Send_Packet(Sendbyte);
                    }

                    yield return new WaitForSeconds(0.04f);
                    // 초당25번 패킷 전송으로 제한을 한다.
                }
            } while (true);
            //yield return null;
        }

        IEnumerator DrawDebugText()
        {
            do
            {

                DebugText.text = debugString.ToString();
                yield return null;
            } while (true);
            //yield return null;
        }

        void ProcessPacket(int size, int type, byte[] recvPacket)
        {
            if (type == recv_protocol.SC_ID)
            {
                // 클라이언트 아이디를 가져온다.
                byte[] t_buf = new byte[size + 1];
                System.Buffer.BlockCopy(recvPacket, 8, t_buf, 0, size); // 사이즈를 제외한 실제 패킷값을 복사한다.
                ByteBuffer revc_buf = new ByteBuffer(t_buf); // ByteBuffer로 byte[]로 복사한다.
                var Get_ServerData = Client_id.GetRootAsClient_id(revc_buf);
                Client_imei = Int32.Parse(Get_ServerData.Id.ToString());
                debugString = "Client ID :" + Client_imei;
                //----------------------------------------------------------------
                // 클라이언트 아이디가 정상적으로 받은건지 확인을 한다.
                // 버그로 인하여 일단 임시로 나둔다.
                //Sendbyte = sF.check_ClientIMEI(Int32.Parse(Get_ServerData.Id.ToString()));
                //Send_Packet(Sendbyte);
                //----------------------------------------------------------------
                Debug.Log("클라이언트 아이디 : " + Client_imei);
            }
            else if (type == recv_protocol.SC_PUT_PLAYER)
            {
                // 클라이언트 하나에 대한 데이터가 들어온다.
                byte[] t_buf = new byte[size + 1];
                System.Buffer.BlockCopy(recvPacket, 8, t_buf, 0, size); // 사이즈를 제외한 실제 패킷값을 복사한다.
                ByteBuffer revc_buf = new ByteBuffer(t_buf); // ByteBuffer로 byte[]로 복사한다.

                var Get_ServerData = Client_info.GetRootAsClient_info(revc_buf);

                // 클라이언트 데이터에 서버에서 받은 데이터를 넣어준다.
                if (client_data.ContainsKey(Get_ServerData.Id))
                {
                    // 이미 값이 들어가 있는 상태라면
                    Game_ClientClass iter = client_data[Get_ServerData.Id];
                    iter.set_pos(new Vector3(Get_ServerData.Position.Value.X, Get_ServerData.Position.Value.Y, Get_ServerData.Position.Value.Z));
                    iter.set_rot(new Vector3(Get_ServerData.Rotation.Value.X, Get_ServerData.Rotation.Value.Y, Get_ServerData.Rotation.Value.Z));
                }
                else
                {
                    // 클라이언트가 자기 자신이 아닐경우에만 추가해준다.
                    client_data.Add(Get_ServerData.Id, new Game_ClientClass(Get_ServerData.Id, Get_ServerData.Name.ToString(), new Vector3(Get_ServerData.Position.Value.X, Get_ServerData.Position.Value.Y, Get_ServerData.Position.Value.Z), new Vector3(Get_ServerData.Rotation.Value.X, Get_ServerData.Rotation.Value.Y, Get_ServerData.Rotation.Value.Z)));
                }
            }
            else if (type == recv_protocol.SC_Client_Data)
            {
                // 클라이언트 모든 데이터가 들어온다.
                byte[] t_buf = new byte[size + 1];
                System.Buffer.BlockCopy(recvPacket, 8, t_buf, 0, size); // 사이즈를 제외한 실제 패킷값을 복사한다.
                ByteBuffer revc_buf = new ByteBuffer(t_buf); // ByteBuffer로 byte[]로 복사한다.
                var Get_ServerData = Client_Collection.GetRootAsClient_Collection(revc_buf);

                // 서버에서 받은 데이터 묶음을 확인하여 묶음 수 만큼 추가해준다.
                for (int i = 0; i < Get_ServerData.DataLength; i++)
                {
                    if (client_data.ContainsKey(Get_ServerData.Data(i).Value.Id))
                    {
                        // 이미 값이 들어가 있는 상태라면
                        Game_ClientClass iter = client_data[Get_ServerData.Data(i).Value.Id];
                        iter.set_pos(new Vector3(Get_ServerData.Data(i).Value.Position.Value.X, Get_ServerData.Data(i).Value.Position.Value.Y, Get_ServerData.Data(i).Value.Position.Value.Z));
                        iter.set_rot(new Vector3(Get_ServerData.Data(i).Value.Rotation.Value.X, Get_ServerData.Data(i).Value.Rotation.Value.Y, Get_ServerData.Data(i).Value.Rotation.Value.Z));
                        iter.set_weapon(Get_ServerData.Data(i).Value.NowWeapon);
                        iter.set_horizontal(Get_ServerData.Data(i).Value.Horizontal);
                        iter.set_vertical(Get_ServerData.Data(i).Value.Vertical);
                        iter.set_inCar(Get_ServerData.Data(i).Value.InCar);
                        iter.set_car_rot(new Vector3(Get_ServerData.Data(i).Value.Carrotation.Value.X, Get_ServerData.Data(i).Value.Carrotation.Value.Y, Get_ServerData.Data(i).Value.Carrotation.Value.Z));
                        iter.set_activePlayer(true);
                        if (iter.get_prefab() == true)
                        {
                            // 프리팹이 만들어진 이후 부터 script를 사용할 수 있기 때문에 그 이후 애니메이션 동기화를 시작한다.
                            iter.script.get_Animator(Get_ServerData.Data(i).Value.Animator);
                            iter.script.get_Weapon(Get_ServerData.Data(i).Value.NowWeapon);
                            iter.script.Vertical = Get_ServerData.Data(i).Value.Vertical;
                            iter.script.Horizontal = Get_ServerData.Data(i).Value.Horizontal;
                        }
                    }
                    else
                    {
                        // 클라이언트가 자기 자신이 아닐경우에만 추가해준다.
                        client_data.Add(Get_ServerData.Data(i).Value.Id, new Game_ClientClass(Get_ServerData.Data(i).Value.Id, Get_ServerData.Data(i).Value.Name.ToString(), new Vector3(Get_ServerData.Data(i).Value.Position.Value.X, Get_ServerData.Data(i).Value.Position.Value.Y, Get_ServerData.Data(i).Value.Position.Value.Z), new Vector3(Get_ServerData.Data(i).Value.Rotation.Value.X, Get_ServerData.Data(i).Value.Rotation.Value.Y, Get_ServerData.Data(i).Value.Rotation.Value.Z)));
                    }
                }
            }
            else if (type == recv_protocol.SC_REMOVE_PLAYER)
            {
                // 서버에서 내보낸 클라이언트를 가져 온다.
                byte[] t_buf = new byte[size + 1];
                System.Buffer.BlockCopy(recvPacket, 8, t_buf, 0, size); // 사이즈를 제외한 실제 패킷값을 복사한다.
                ByteBuffer revc_buf = new ByteBuffer(t_buf); // ByteBuffer로 byte[]로 복사한다.
                var Get_ServerData = Client_id.GetRootAsClient_id(revc_buf);

                Game_ClientClass iter = client_data[Get_ServerData.Id];
                // 해당 클라이언트의 SetActive를 꺼준다.
                iter.set_removeClient(true);
            }
            else if (type == recv_protocol.SC_Server_Time)
            {
                byte[] t_buf = new byte[size + 1];
                System.Buffer.BlockCopy(recvPacket, 8, t_buf, 0, size); // 사이즈를 제외한 실제 패킷값을 복사한다.
                ByteBuffer revc_buf = new ByteBuffer(t_buf); // ByteBuffer로 byte[]로 복사한다.
                var Get_ServerData = Game_Timer.GetRootAsGame_Timer(revc_buf);
                //Debug.Log("Time : " + Get_ServerData.Time);
                debugString = "Time : " + (Get_ServerData.Time / 60) + "m " + (Get_ServerData.Time % 60) + "s";
            }
            else if (type == recv_protocol.SC_Server_Item)
            {
                // 서버 아이템 관련...
                byte[] t_buf = new byte[size + 1];
                System.Buffer.BlockCopy(recvPacket, 8, t_buf, 0, size); // 사이즈를 제외한 실제 패킷값을 복사한다.
                ByteBuffer revc_buf = new ByteBuffer(t_buf); // ByteBuffer로 byte[]로 복사한다.

                var Get_ServerData = Game_Items.GetRootAsGame_Items(revc_buf);

                for (int i = 0; i < Get_ServerData.DataLength; i++)
                {

                    Vector3 pos;
                    pos.x = Get_ServerData.Data(i).Value.Position.Value.X;
                    pos.y = 29.99451f;
                    pos.z = Get_ServerData.Data(i).Value.Position.Value.Z;
                    Vector3 rotation;
                    rotation.x = Get_ServerData.Data(i).Value.Rotation.Value.X;
                    rotation.y = Get_ServerData.Data(i).Value.Rotation.Value.Y;
                    rotation.z = Get_ServerData.Data(i).Value.Rotation.Value.Z;

                    if (item_Collection.ContainsKey(Get_ServerData.Data(i).Value.Id))
                    {
                        // 이미 값이 들어가 있는 상태라면
                        Game_ItemClass iter = item_Collection[Get_ServerData.Data(i).Value.Id];
                        iter.set_pos(pos);
                        iter.set_rotation(rotation);
                        iter.set_eat(Get_ServerData.Data(i).Value.Eat);
                    }
                    else
                    {
                        item_Collection.Add(Get_ServerData.Data(i).Value.Id, new Game_ItemClass(Get_ServerData.Data(i).Value.Id, Get_ServerData.Data(i).Value.Name.ToString(), pos, rotation, Get_ServerData.Data(i).Value.Eat, Get_ServerData.Data(i).Value.Kind));
                    }
                }

            }
            else if (type == recv_protocol.SC_Shot_Client)
            {
                // 클라이언트 Shot 을 가져온다.
                byte[] t_buf = new byte[size + 1];
                System.Buffer.BlockCopy(recvPacket, 8, t_buf, 0, size); // 사이즈를 제외한 실제 패킷값을 복사한다.
                ByteBuffer revc_buf = new ByteBuffer(t_buf); // ByteBuffer로 byte[]로 복사한다.
                var Get_ServerData = Client_id.GetRootAsClient_id(revc_buf);

                if (Get_ServerData.Id != Client_imei)
                {
                    Game_ClientClass iter = client_data[Get_ServerData.Id];
                    Game_ClientClass iter2 = client_data[Client_imei];
                    iter.script.Fire(iter2.get_pos());      // 자신의 위치를 보내는 것은 총 소리를 판별 하기 위하여.
                }
            }
            else if (type == recv_protocol.SC_DangerLine)
            {
                // 클라이언트 DangerLine 정보를 가져온다.
                byte[] t_buf = new byte[size + 1];
                System.Buffer.BlockCopy(recvPacket, 8, t_buf, 0, size); // 사이즈를 제외한 실제 패킷값을 복사한다.
                ByteBuffer revc_buf = new ByteBuffer(t_buf); // ByteBuffer로 byte[]로 복사한다.
                var Get_ServerData = GameDangerLine.GetRootAsGameDangerLine(revc_buf);

                DangerLineCtrl.set_demage(Get_ServerData.Demage);
                DangerLineCtrl.set_pos(new Vector3(Get_ServerData.Position.Value.X, Get_ServerData.Position.Value.Y, Get_ServerData.Position.Value.Z));
                DangerLineCtrl.set_scale(new Vector3(Get_ServerData.Scale.Value.X, Get_ServerData.Scale.Value.Y, Get_ServerData.Scale.Value.Z));
                DangerLineCtrl.set_start(true);
            }
            else if (type == recv_protocol.SC_Zombie_Info)
            {
                // 좀비 관련한 데이터를 받았다.
                byte[] t_buf = new byte[size + 1];
                System.Buffer.BlockCopy(recvPacket, 8, t_buf, 0, size); // 사이즈를 제외한 실제 패킷값을 복사한다.
                ByteBuffer revc_buf = new ByteBuffer(t_buf); // ByteBuffer로 byte[]로 복사한다.
                var Get_ServerData = Zombie_Collection.GetRootAsZombie_Collection(revc_buf);

                // 서버에서 받은 데이터 묶음을 확인하여 묶음 수 만큼 추가해준다.
                for (int i = 0; i < Get_ServerData.DataLength; i++)
                {
                    if (zombie_data.ContainsKey(Get_ServerData.Data(i).Value.Id))
                    {
                        // 이미 값이 들어가 있는 상태라면
                        Game_ZombieClass iter = zombie_data[Get_ServerData.Data(i).Value.Id];
                        if (Get_ServerData.Data(i).Value.TargetPlayer != Client_imei)
                        {
                            // Target과 플레이어가 다를 경우에만 위치를 동기화 해준다.
                            iter.set_pos(new Vector3(Get_ServerData.Data(i).Value.Position.Value.X, Get_ServerData.Data(i).Value.Position.Value.Y, Get_ServerData.Data(i).Value.Position.Value.Z));
                            iter.set_rot(new Vector3(Get_ServerData.Data(i).Value.Rotation.Value.X, Get_ServerData.Data(i).Value.Rotation.Value.Y, Get_ServerData.Data(i).Value.Rotation.Value.Z));
                            iter.set_hp(Get_ServerData.Data(i).Value.Hp);
                            iter.set_animator(Get_ServerData.Data(i).Value.Animator);
                        }
                        else if (iter.get_pos().x == 0 && iter.get_pos().y == 0 && iter.get_pos().z == 0)
                        {
                            iter.set_pos(new Vector3(Get_ServerData.Data(i).Value.Position.Value.X, Get_ServerData.Data(i).Value.Position.Value.Y, Get_ServerData.Data(i).Value.Position.Value.Z));
                            iter.set_rot(new Vector3(Get_ServerData.Data(i).Value.Rotation.Value.X, Get_ServerData.Data(i).Value.Rotation.Value.Y, Get_ServerData.Data(i).Value.Rotation.Value.Z));
                            iter.set_hp(Get_ServerData.Data(i).Value.Hp);
                            iter.set_animator(Get_ServerData.Data(i).Value.Animator);
                        }
                        iter.set_target(Get_ServerData.Data(i).Value.TargetPlayer);
                        iter.set_activeZombie(true);
                    }
                    else
                    {
                        // 좀비를 추가해준다.
                        zombie_data.Add(Get_ServerData.Data(i).Value.Id, new Game_ZombieClass(Get_ServerData.Data(i).Value.Id, Get_ServerData.Data(i).Value.TargetPlayer, new Vector3(Get_ServerData.Data(i).Value.Position.Value.X, Get_ServerData.Data(i).Value.Position.Value.Y, Get_ServerData.Data(i).Value.Position.Value.Z), new Vector3(Get_ServerData.Data(i).Value.Rotation.Value.X, Get_ServerData.Data(i).Value.Rotation.Value.Y, Get_ServerData.Data(i).Value.Rotation.Value.Z)));
                    }
                }
            }
            else if (type == recv_protocol.SC_Remove_Zombie)
            {
                // 서버에서 내보낸 클라이언트를 가져 온다.
                byte[] t_buf = new byte[size + 1];
                System.Buffer.BlockCopy(recvPacket, 8, t_buf, 0, size); // 사이즈를 제외한 실제 패킷값을 복사한다.
                ByteBuffer revc_buf = new ByteBuffer(t_buf); // ByteBuffer로 byte[]로 복사한다.
                var Get_ServerData = Client_id.GetRootAsClient_id(revc_buf);

                Game_ZombieClass iter = zombie_data[Get_ServerData.Id];
                // 해당 좀비의 SetActive를 꺼준다.
                iter.set_removeZombie(true);
            }

        }

        void Awake()
        {
            Application.runInBackground = true; // 백그라운드에서도 Network는 작동해야한다.
            DangerLineCtrl = GameObject.FindGameObjectWithTag("DangerLine").GetComponent<DangerLineCtrl>();
            //=======================================================
            // Socket create.
            m_Socket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
            m_Socket.SetSocketOption(SocketOptionLevel.Socket, SocketOptionName.ReuseAddress, 1);
            //m_Socket.SetSocketOption(SocketOptionLevel.Socket, SocketOptionName.SendTimeout, 10000);
            //m_Socket.SetSocketOption(SocketOptionLevel.Socket, SocketOptionName.ReceiveTimeout, 10000);
            m_Socket.NoDelay = true;
            //=======================================================
            // Socket connect.
            try
            {
                m_Socket.BeginConnect(iPAdress, kPort, new AsyncCallback(ConnectCallback), m_Socket);
                connectDone.WaitOne();
                Player_Script = GameObject.FindWithTag("Player").GetComponent<PlayerCtrl>();
                StartCoroutine(SocketCheck());
            }
            catch (SocketException SCE)
            {
                debugString = "Socket connect error! : " + SCE.ToString();
                return;
            }

            //=======================================================
        }

        void ConnectCallback(IAsyncResult ar)
        {
            try
            {
                // 서버가 정상적으로 연결이 되었을 경우.

                connectDone.Set();
                if (m_Socket.Connected == true)
                {
                    Debug.Log("Connected to server, start recieve data");
                    RecieveHeader();//start recieve header
                }
            }
            catch (Exception e)
            {
                connectDone.Set();
                Console.WriteLine(e.ToString());
            }
        }

        void RecieveHeader()
        {
            try
            {
                NetworkMessage msg = new NetworkMessage();
                m_Socket.BeginReceive(msg.Receivebyte, 0, msg.LimitReceivebyte, SocketFlags.None, new AsyncCallback(RecieveHeaderCallback), msg);
            }
            catch (Exception e)
            {
                Debug.Log(e.Message);
            }
        }

        void RecieveHeaderCallback(IAsyncResult ar)
        {
            try
            {
                NetworkMessage msg = (NetworkMessage)ar.AsyncState;     // Recieve된 Packet을 받아온다.
                int bytesRead = m_Socket.EndReceive(ar);        // 소켓에서 받아온 사이즈를 확인한다.

                PacketData size_data = Get_packet_size(msg.Receivebyte);

                int psize = size_data.p_size;
                int ptype = msg.Receivebyte[size_data.type_Pos + 1]; // 패킷 타입

                if (psize == bytesRead)
                {
                    // 소켓에서 받은 데이터와 실제 패킷 사이즈가 같을 경우
                    ProcessPacket(psize, ptype, msg.Receivebyte);
                    // 패킷 처리가 완료 되었으니 다시 리시브 상태로 돌아간다.
                    NetworkMessage new_msg = new NetworkMessage();
                    m_Socket.BeginReceive(new_msg.Receivebyte, 0, new_msg.LimitReceivebyte, SocketFlags.None, new AsyncCallback(RecieveHeaderCallback), new_msg);
                }
                else
                {
                    // 소켓에서 받은 데이터와 실제 패킷 사이즈가 다를 경우
                    msg.sb.Append(Encoding.ASCII.GetString(msg.Receivebyte, 0, bytesRead));
                    msg.set_prev(bytesRead);
                    // 소켓에서 받은 데이터가 안맞는 경우 패킷이 뒤에 붙어서 오는거 같은 느낌이 든다...
                    size_data = Get_packet_size(msg.Receivebyte);
                    byte[] recv_byte = new byte[size_data.p_size + 9];

                    for (int i = 0; i < size_data.p_size; ++i)
                    {
                        recv_byte[i] = msg.Receivebyte[i];
                    }

                    ProcessPacket(psize, ptype, recv_byte);

                    m_Socket.BeginReceive(msg.Receivebyte, 0, msg.LimitReceivebyte, SocketFlags.None, new AsyncCallback(RecieveHeaderCallback), msg);
                }
            }
            catch (Exception e)
            {
                //Debug.Log(e.Message);
                NetworkMessage new_msg = new NetworkMessage();
                m_Socket.BeginReceive(new_msg.Receivebyte, 0, new_msg.LimitReceivebyte, SocketFlags.None, new AsyncCallback(RecieveHeaderCallback), new_msg);
            }
        }

        public void Send_Packet(byte[] packet)
        {
            if (serverConnect == true)
            {
                try
                {
                    //Debug.Log(packet.Length);
                    m_Socket.Send(packet, packet.Length, 0);
                }
                catch (SocketException err)
                {
                    Debug.Log("Socket send or receive error! : " + err.ToString());
                }
            }
        }

        public void Player_Shot()
        {
            Sendbyte = sF.makeShot_PacketInfo(Client_imei);
            Send_Packet(Sendbyte);
        }

        public void Zombie_Pos(Vector3 pos, Vector3 rotation, int zombieNum, int hp, Enum animation)
        {
            // 좀비의 위치를 Zombie_Data에 넣어준다.
            Game_ZombieClass iter = zombie_data[zombieNum];
            iter.set_pos(pos);
            iter.set_rot(rotation);
            iter.set_hp(hp);
            iter.set_animator(Convert.ToInt32(animation));
        }

        void OnApplicationQuit()
        {
            m_Socket.Close();
            m_Socket = null;
        }

        public float DistanceToPoint(Vector3 a, Vector3 b)
        {
            // 캐릭터 간의 거리 구하기.
            return (float)Math.Sqrt(Math.Pow(a.x - b.x, 2) + Math.Pow(a.z - b.z, 2));
        }

        public void get_object_collision()
        {
            string filePath = "./file.txt";
            string coordinates = "";
            BoxCollider[] boxObjects = UnityEngine.Object.FindObjectsOfType<BoxCollider>();

            foreach (BoxCollider trans in boxObjects)
            {
                if (trans.tag.ToString() != "CAMCHANGE")
                {
                    coordinates += "posx:" + trans.transform.position.x.ToString() + "|posy:" + trans.transform.position.y.ToString() + "|posz:" + trans.transform.position.z.ToString() + "|centerx:" + trans.center.x.ToString() + "|centery:" + trans.center.y.ToString() + "|centerz:" + trans.center.z.ToString() + "|sizex:" + trans.size.x.ToString() + "|sizey:" + trans.size.y.ToString() + "|sizez:" + trans.size.z.ToString() + "|scalex:" + trans.transform.localScale.x.ToString() + "|scaley:" + trans.transform.localScale.y.ToString() + "|scalez:" + trans.transform.localScale.z.ToString() + "|" + System.Environment.NewLine;
                }
            }

            //Write the coords to a file
            System.IO.File.WriteAllText(filePath, coordinates);
        }

        public PacketData Get_packet_size(byte[] Receivebyte)
        {
            //-------------------------------------------------------------------------------------
            /*
             C++ itoa를 통한 char로 넣은것을 for문을 통하여 컨버팅 하여 가져온다.
             124는 C++에서 '|'값 이다.
             str_size로 실제 패킷 값을 계산해서 넣는다.
             */
            string str_size = "";
            string tmp_int = "";
            byte[] temp = new byte[8];
            int type_Pos = 0;

            for (type_Pos = 0; type_Pos < 8; ++type_Pos)
            {
                if (Receivebyte[type_Pos] == 124)
                    break;
                temp[0] = Receivebyte[type_Pos];
                tmp_int = Encoding.Default.GetString(temp);
                str_size += Int32.Parse(tmp_int);
            }
            //-------------------------------------------------------------------------------------

            return new PacketData(Int32.Parse(str_size), type_Pos);
        }

    }
}