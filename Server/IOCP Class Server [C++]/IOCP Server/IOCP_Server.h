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
#include <unordered_map>
#include <queue>
#include <thread>
#include <random>
#include <windows.h>
#include <unordered_set> // ������ �� ��������. [������ ����������]
#include <mutex>
#include <string>
#include <cmath>

#include "Protocol.h"
#include "Flatbuffers_View.h"
#include "Game_Item.h"

using namespace std::chrono;
using namespace Game::TheLastOne; // Flatbuffers�� �о����.

struct xyz {
	float x;
	float y;
	float z;
};

class IOCP_Server {
private:
	SOCKET g_socket;
	std::chrono::high_resolution_clock::time_point serverTimer;
	HANDLE g_hiocp;

	void initServer();
	void err_quit(char *msg);							// Error ���� ��� ����
	void err_display(char *msg, int err_no);		// Error ǥ�� ���ֱ�
	void makeThread();								// ������ �����
	void Worker_Thread();							// ���� ���� ������
	void Accept_Thread();								// Ŭ���̾�Ʈ �޴� ������
	void Remove_Client();							// Ŭ���̾�Ʈ�� ����� ����� ������
	void Shutdown_Server();							// ���� ����
	void DisconnectClient(int ci);					// Ŭ���̾�Ʈ ����
	void ProcessPacket(int ci, char *packet);		// ��Ŷ ó��
	void SendPacket(int type, int cl, void *packet, int psize);		// ��Ŷ ������
	void Send_Client_ID(int client_id, int value, bool allClient);	// Ŭ���̾�Ʈ ���� ��Ŷ ���̵� ������
	void Send_All_Player(int client);					// Ŭ���̾�Ʈ���� ��� Ŭ���̾�Ʈ ��ġ ������
	void Send_All_Zombie(int client);							// Ŭ���̾�Ʈ���� ��� ���� ��ġ ������
	void Send_All_Time(int kind, int time, int client_id, bool allClient);					// Ŭ���̾�Ʈ���� �ð��� �����ش�.
	void Send_All_Item();		// Ŭ���̾�Ʈ���� �ð������� �����ش�.
	void Send_Client_Shot(int shot_client);		// Ŭ���̾�Ʈ�鿡�� Shot ������ �����ش�.
	void Send_DangerLine_info(int demage, xyz pos, xyz scale);
	void Send_Hide_Player(int client);		// Ŭ���̾�Ʈ ������ ��� ��� �����ش�.
	void Send_Hide_Zombie(int client);		// Ŭ���̾�Ʈ ������ ��� ��� �����ش�.

public:
	HANDLE getHandle() { return g_hiocp; }
	IOCP_Server();
	~IOCP_Server();
};


#endif