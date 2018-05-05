#ifndef __IOCPSERVER_H__
#define __IOCPSERVER_H__

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS						// scanf ���� ����
#define _CRT_NONSTDC_NO_DEPRECATE					// itoa ���� ����
/* flatbuffers ������ min, max ���� �ذ� ��� */
#define _WIN32_WINNT _WIN32_WINNT_XP
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
/* http://bspfp.pe.kr/archives/591 */

#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <iostream>
#include <vector>
#include <queue>
#include <random>
#include <windows.h>
#include <string>
#include <cmath>


#include "Protocol.h"
#include "Flatbuffers_View.h"
#include "Room_Manager.h"
#include "Timer.h"


using namespace Game::TheLastOne; // Flatbuffers�� �о����.

class IOCP_Server {
private:
	SOCKET g_socket;
	std::chrono::high_resolution_clock::time_point serverTimer;
	HANDLE g_hiocp;
	std::vector<Room_Manager> GameRoom;		// ���� ���� Vector�� ����
	std::queue<int> remove_client_id;					// Ŭ���̾�Ʈ ���̵�	Queue�� �ִ´�.
	//std::unordered_map< int, int> ci_room;			// Ŭ���̾�Ʈ ���̵�� �� ������ ������ �ִ´�. [���̵�, ������]
	Server_Timer Timer;									// ���� Ÿ�̸�

	void initServer();
	void err_quit(char *msg);							// Error ���� ��� ����
	void err_display(char *msg, int err_no);		// Error ǥ�� ���ֱ�
	void makeThread();								// ������ �����
	void Worker_Thread();							// ���� ���� ������
	void Accept_Thread();								// Ŭ���̾�Ʈ �޴� ������
	void Remove_Client();								// Ŭ���̾�Ʈ�� ����� ������ �����
	void Shutdown_Server();							// ���� ����
	void DisconnectClient(const int room_id, const int ci);					// Ŭ���̾�Ʈ ����
	void ProcessPacket(const int room_id, const int ci, const char *packet);		// ��Ŷ ó��
	void SendPacket(const int type, const int room_id, const int ci, const void *packet, const int psize);		// ��Ŷ ������
	void Send_Client_ID(const int room_id, const int client_id, const int value, const bool allClient);	// Ŭ���̾�Ʈ ���� ��Ŷ ���̵� ������
	void Send_All_Player(const int room_id, const int client);					// Ŭ���̾�Ʈ���� ��� Ŭ���̾�Ʈ ��ġ ������
	void Send_Hide_Player(const int room_id, const int client);		// Ŭ���̾�Ʈ ������ ��� ��� �����ش�.
	void Send_All_Zombie(const int room_id, const int client);							// Ŭ���̾�Ʈ���� ��� ���� ��ġ ������
	void Send_Hide_Zombie(const int room_id, const int client);		// Ŭ���̾�Ʈ ������ ��� ��� �����ش�.
	//void Send_All_Time(int kind, int time, int client_id, bool allClient);					// Ŭ���̾�Ʈ���� �ð��� �����ش�.
	void Send_All_Item(const int room_id, const int ci);		// Ŭ���̾�Ʈ���� �ð������� �����ش�.
	void Send_Client_Shot(const int room_id, const int shot_client);		// Ŭ���̾�Ʈ�鿡�� Shot ������ �����ش�.
	//void Send_DangerLine_info(int demage, xyz pos, xyz scale);
	//void Attack_DangerLine_Damge();			// �ڱ��� �������� �÷��̾�� �ش�.

	bool Distance(const int room_id, const int me, const int  you, const int Radius, const int kind);
public:
	HANDLE getHandle() { return g_hiocp; }
	IOCP_Server();
	~IOCP_Server();
};


#endif