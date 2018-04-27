﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using UnityEngine;
using FlatBuffers;
using Game.TheLastOne; // Client, Vec3 을 불러오기 위해
using System.Collections.Generic;
using TheLastOne.GameClass;
//using TheLastOne.Game.Network;



namespace TheLastOne.SendFunction
{
    class Socket_SendFunction : Game_ProtocolClass
    {
        FlatBufferBuilder fbb = new FlatBufferBuilder(1);

        public Byte[] makeClient_PacketInfo(Vector3 Player, int Player_Animator, float horizontal, float vertical, Vector3 PlayerRotation, int Player_Weapone, int inCar, Vector3 CarRotation)
        {
            //var offset = fbb.CreateString("WindowsHyun"); // String 문자열이 있을경우 미리 생성해라.
            fbb.Clear(); // 클리어를 안해주고 시작하면 계속 누적해서 데이터가 들어간다.
            Client_info.StartClient_info(fbb);
            //Client.AddName(fbb, offset); // string 사용
            Client_info.AddAnimator(fbb, Player_Animator);
            Client_info.AddHorizontal(fbb, horizontal);
            Client_info.AddVertical(fbb, vertical);
            Client_info.AddInCar(fbb, inCar);
            Client_info.AddCarrotation(fbb, Vec3.CreateVec3(fbb, CarRotation.x, CarRotation.y, CarRotation.z));
            Client_info.AddPosition(fbb, Vec3.CreateVec3(fbb, Player.x, Player.y, Player.z));
            Client_info.AddRotation(fbb, Vec3.CreateVec3(fbb, PlayerRotation.x, PlayerRotation.y, PlayerRotation.z));
            Client_info.AddNowWeapon(fbb, Player_Weapone);
            var endOffset = Client_info.EndClient_info(fbb);
            fbb.Finish(endOffset.Value);


            byte[] packet = fbb.SizedByteArray();   // flatbuffers 실제 패킷 데이터
            byte[] packet_len = BitConverter.GetBytes(packet.Length);   // flatbuffers의 패킷 크기
            byte[] packet_type = BitConverter.GetBytes(CS_Info);
            byte[] real_packet = new byte[packet_len.Length + packet.Length];

            System.Buffer.BlockCopy(packet_len, 0, real_packet, 0, packet_len.Length);
            System.Buffer.BlockCopy(packet_type, 0, real_packet, 1, packet_type.Length);
            System.Buffer.BlockCopy(packet, 0, real_packet, 4, packet.Length);
            return real_packet;
        }

        public Byte[] makeZombie_PacketInfo(Dictionary<int, Game_ZombieClass> zombie_data, int client_imei)
        {
            fbb.Clear();
            //var target_zombie = new Offset<Zombie_info>[10];
            List<Offset<Zombie_info>> target_zombie = new List<Offset<Zombie_info>>();

            int num = 0;
            foreach (var key in zombie_data.Keys.ToList())
            {
                if (zombie_data[key].get_target() == client_imei)
                {
                    // 좀비 Target과 Client_Imei가 같은경우에만 Vector에 넣는다.
                    Zombie_info.StartZombie_info(fbb);
                    Zombie_info.AddId(fbb, zombie_data[key].get_id());
                    Zombie_info.AddHp(fbb, zombie_data[key].get_hp());
                    Zombie_info.AddAnimator(fbb, zombie_data[key].get_animator());
                    Zombie_info.AddTargetPlayer(fbb, zombie_data[key].get_target());
                    Zombie_info.AddPosition(fbb, Vec3.CreateVec3(fbb, zombie_data[key].get_pos().x, zombie_data[key].get_pos().y, zombie_data[key].get_pos().z));
                    Zombie_info.AddRotation(fbb, Vec3.CreateVec3(fbb, zombie_data[key].get_rot().x, zombie_data[key].get_rot().y, zombie_data[key].get_rot().z));
                    target_zombie.Add(Zombie_info.EndZombie_info(fbb));
                    //target_zombie[num] = Zombie_info.EndZombie_info(fbb)
                    ++num;
                }
            }

            var send_zombie = new Offset<Zombie_info>[target_zombie.Count()];
            num = 0;
            foreach (var data in target_zombie)
            {
                send_zombie[num] = data;
                ++num;
            }

            var zombie_vector = Zombie_Collection.CreateDataVector(fbb, send_zombie);
            Zombie_Collection.StartZombie_Collection(fbb);
            Zombie_Collection.AddData(fbb, zombie_vector);
            var endOffset = Zombie_Collection.EndZombie_Collection(fbb);
            fbb.Finish(endOffset.Value);

            byte[] packet = fbb.SizedByteArray();   // flatbuffers 실제 패킷 데이터
            byte[] packet_len = BitConverter.GetBytes(20);   // flatbuffers의 패킷 크기
            byte[] packet_type = BitConverter.GetBytes(CS_Zombie_info);
            byte[] real_packet = new byte[packet_len.Length + packet.Length];

            System.Buffer.BlockCopy(packet_len, 0, real_packet, 0, packet_len.Length);
            System.Buffer.BlockCopy(packet_type, 0, real_packet, 1, packet_type.Length);
            System.Buffer.BlockCopy(packet, 0, real_packet, 4, packet.Length);

            Debug.Log(real_packet.Length);

            if (num != 0)
                return real_packet;
            else
                return null;

        }

        public Byte[] makeShot_PacketInfo(int client)
        {
            //var offset = fbb.CreateString("WindowsHyun"); // String 문자열이 있을경우 미리 생성해라.
            fbb.Clear(); // 클리어를 안해주고 시작하면 계속 누적해서 데이터가 들어간다.
            Client_Shot_info.StartClient_Shot_info(fbb);
            Client_Shot_info.AddId(fbb, client);
            var endOffset = Client_Shot_info.EndClient_Shot_info(fbb);
            fbb.Finish(endOffset.Value);


            byte[] packet = fbb.SizedByteArray();   // flatbuffers 실제 패킷 데이터
            byte[] packet_len = BitConverter.GetBytes(packet.Length);   // flatbuffers의 패킷 크기
            byte[] packet_type = BitConverter.GetBytes(CS_Shot_info);
            byte[] real_packet = new byte[packet_len.Length + packet.Length];

            System.Buffer.BlockCopy(packet_len, 0, real_packet, 0, packet_len.Length);
            System.Buffer.BlockCopy(packet_type, 0, real_packet, 1, packet_type.Length);
            System.Buffer.BlockCopy(packet, 0, real_packet, 4, packet.Length);
            return real_packet;
        }

        public Byte[] check_ClientIMEI(int client)
        {
            //var offset = fbb.CreateString("WindowsHyun"); // String 문자열이 있을경우 미리 생성해라.
            fbb.Clear(); // 클리어를 안해주고 시작하면 계속 누적해서 데이터가 들어간다.
            Client_id.StartClient_id(fbb);
            Client_id.AddId(fbb, client);
            var endOffset = Client_id.EndClient_id(fbb);
            fbb.Finish(endOffset.Value);


            byte[] packet = fbb.SizedByteArray();   // flatbuffers 실제 패킷 데이터
            byte[] packet_len = BitConverter.GetBytes(packet.Length);   // flatbuffers의 패킷 크기
            byte[] packet_type = BitConverter.GetBytes(CS_Check_info);
            byte[] real_packet = new byte[packet_len.Length + packet.Length];

            System.Buffer.BlockCopy(packet_len, 0, real_packet, 0, packet_len.Length);
            System.Buffer.BlockCopy(packet_type, 0, real_packet, 1, packet_type.Length);
            System.Buffer.BlockCopy(packet, 0, real_packet, 4, packet.Length);
            return real_packet;
        }

        public Byte[] makeEatItem_PacketInfo(int item_num)
        {
            //var offset = fbb.CreateString("WindowsHyun"); // String 문자열이 있을경우 미리 생성해라.
            fbb.Clear(); // 클리어를 안해주고 시작하면 계속 누적해서 데이터가 들어간다.
            Client_id.StartClient_id(fbb);
            Client_id.AddId(fbb, item_num);
            var endOffset = Client_id.EndClient_id(fbb);
            fbb.Finish(endOffset.Value);


            byte[] packet = fbb.SizedByteArray();   // flatbuffers 실제 패킷 데이터
            byte[] packet_len = BitConverter.GetBytes(packet.Length);   // flatbuffers의 패킷 크기
            byte[] packet_type = BitConverter.GetBytes(CS_Eat_Item);
            byte[] real_packet = new byte[packet_len.Length + packet.Length];

            System.Buffer.BlockCopy(packet_len, 0, real_packet, 0, packet_len.Length);
            System.Buffer.BlockCopy(packet_type, 0, real_packet, 1, packet_type.Length);
            System.Buffer.BlockCopy(packet, 0, real_packet, 4, packet.Length);
            return real_packet;
        }

    }
}
