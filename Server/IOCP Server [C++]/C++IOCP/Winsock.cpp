#include "Server.h"
#include "protocol.h"
#include "Flatbuffers_View.h"

using namespace Game::TheLastOne; // Flatbuffers�� �о����.

//CLIENT g_clients[MAX_NPC];

void err_quit(char *msg) {
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	printf("Error : %s\n", msg);
	LocalFree(lpMsgBuf);
	exit(1);
}

void err_display(char *msg, int err_no) {
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, err_no,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	printf("[%s] %s\n", msg, (char *)lpMsgBuf);
	LocalFree(lpMsgBuf);
}

//void SendPacket( int cl, void *packet ) {
//	if ( g_clients[cl].connect == true ) {
//		int psize = reinterpret_cast<unsigned char *>(packet)[0];
//		int ptype = reinterpret_cast<unsigned char *>(packet)[1];
//		OverlappedEx *over = new OverlappedEx;
//		ZeroMemory( &over->over, sizeof( over->over ) );
//		over->event_type = OP_SEND;
//		memcpy( over->IOCP_buf, packet, psize );
//		over->wsabuf.buf = reinterpret_cast<CHAR *>(over->IOCP_buf);
//		over->wsabuf.len = psize;
//		int res = WSASend( g_clients[cl].client_socket, &over->wsabuf, 1, NULL, 0, &over->over, NULL );
//		if ( 0 != res ) {
//			int error_no = WSAGetLastError();
//			if ( WSA_IO_PENDING != error_no ) {
//				err_display( "SendPacket:WSASend", error_no );
//				DisconnectClient( cl );
//			}
//		}
//	}
//}

void SendPacket(int type, int cl, void *packet, int psize) {
	if (g_clients[cl].connect == true) {
		//int ptype = reinterpret_cast<unsigned char *>(packet)[1];
		OverlappedEx *over = new OverlappedEx;
		ZeroMemory(&over->over, sizeof(over->over));
		over->event_type = OP_SEND;
		char p_size[MAX_PACKET_SIZE]{ int(psize), int(type) };
		// ��Ŷ ����� 252�� �Ѿ��� charũ�⸦ �ʰ��ؼ� ������ ����.
		// �ش� �κ��� ��� �����ؾ� �ұ�?



		// ��Ŷ ����� �̸� ���ļ� ��������Ѵ�.
		memcpy(over->IOCP_buf, packet, psize);

		for (int i = 4; i < psize + 4; ++i) {
			p_size[i] = over->IOCP_buf[i - 4];
		}

		//strcat( p_size, reinterpret_cast<CHAR *>(over->IOCP_buf) );
		//sprintf( buf, "%c%s", p_size, over->IOCP_buf );

		over->wsabuf.buf = reinterpret_cast<CHAR *>(p_size);
		over->wsabuf.len = psize + 4;
		int res = WSASend(g_clients[cl].client_socket, &over->wsabuf, 1, NULL, 0, &over->over, NULL);
		if (0 != res) {
			int error_no = WSAGetLastError();
			if (WSA_IO_PENDING != error_no) {
				err_display("SendPacket:WSASend", error_no);
				DisconnectClient(cl);
			}
		}
	}
}

void Send_Client_ID(int client_id) {
	flatbuffers::FlatBufferBuilder builder;
	auto Client_id = client_id;
	auto orc = CreateClient_id(builder, Client_id);
	builder.Finish(orc); // Serialize the root of the object.
	SendPacket(SC_ID, client_id, builder.GetBufferPointer(), builder.GetSize());
}


void Send_Position(int client, int object) {
	// client = �ڱ��ڽ�, object = ����
	flatbuffers::FlatBufferBuilder builder;
	auto id = object;
	auto name = builder.CreateString(g_clients[object].game_id);
	auto hp = g_clients[object].hp;
	auto xyz = Vec3(g_clients[object].position.x, g_clients[object].position.y, g_clients[object].position.z);
	auto rotation = Vec3(g_clients[object].rotation.x, g_clients[object].rotation.y, g_clients[object].rotation.z);
	auto orc = CreateClient_info(builder, id, hp, name, &xyz, &rotation);
	builder.Finish(orc); // Serialize the root of the object.
	SendPacket(SC_PUT_PLAYER, client, builder.GetBufferPointer(), builder.GetSize());
}

void Send_All_Data(int client) {
	flatbuffers::FlatBufferBuilder builder;
	
	std::vector<flatbuffers::Offset<Client_info>> Individual_client;		// ���� ������

	for (int i = 0; i < MAX_Client; ++i) {
		if (g_clients[i].connect != true)
			continue;
		auto id = i;
		auto name = builder.CreateString(g_clients[i].game_id);
		auto hp = g_clients[i].hp;
		auto xyz = Vec3(g_clients[i].position.x, g_clients[i].position.y, g_clients[i].position.z);
		auto rotation = Vec3(g_clients[i].rotation.x, g_clients[i].rotation.y, g_clients[i].rotation.z);
		auto client_data = CreateClient_info(builder, id, hp, name, &xyz, &rotation); 
		// client_data ��� ���̺� Ŭ���̾�Ʈ �����Ͱ� �� �ִ�.

		Individual_client.push_back(client_data);	// Vector�� �־���.
		// Individual_client ��� ��ü ���̺� client_data�� �־�����.
	}

	auto Full_client_data = builder.CreateVector(Individual_client);		// ���� Vector�� ��� ������ �����ͷ� ���������.

	auto orc = CreateAll_information(builder, Full_client_data);		// ������ ������ ���̺� ���� Client_Data
	builder.Finish(orc); // Serialize the root of the object.

	SendPacket(SC_Client_Data, client, builder.GetBufferPointer(), builder.GetSize());
}