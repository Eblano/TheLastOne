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
#include <stack>
#include <thread>
#include <random>
#include <windows.h>
#include <unordered_set> // ������ �� ��������. [������ ����������]
#include <mutex>
#include <string.h>    // strchr �Լ��� ����� ��� ����

#include "Protocol.h"
#include "Flatbuffers_View.h"

using namespace std::chrono;
using namespace Game::TheLastOne; // Flatbuffers�� �о����.

//---------------------------------------------------------------------------------------------
// ���� ����
#define SERVERPORT 9000
#define BUFSIZE    1024
#define MAX_BUFF_SIZE   4000
#define MAX_PACKET_SIZE  4000
#define MAX_Client 50
//---------------------------------------------------------------------------------------------
// ���� ����
#define DebugMod TRUE
//---------------------------------------------------------------------------------------------

class IOCP_Server {

private:
	SOCKET g_socket;
	std::chrono::high_resolution_clock::time_point serverTimer;
	HANDLE g_hiocp;
	//-------------------------------------------------------------------------------------
	// ���ӵ� �ο��� �� ������� ������ �Ѵ�.
	std::mutex cp_lock;
	int connected_Person = 0;
	void set_Person(int value);
	int get_Person() { return connected_Person; }
	//-------------------------------------------------------------------------------------
	void initServer();
	void err_quit(char *msg);							// Error ���� ��� ����
	void err_display(char *msg, int err_no);		// Error ǥ�� ���ֱ�
	void makeThread();								// ������ �����
	void Worker_Thread();							// ���� ���� ������
	void Accept_Thread();								// Ŭ���̾�Ʈ �޴� ������
	void Shutdown_Server();							// ���� ����
	void DisconnectClient(int ci);					// Ŭ���̾�Ʈ ����
	void ProcessPacket(int ci, char *packet);		// ��Ŷ ó��
	void SendPacket(int type, int cl, void *packet, int psize);		// ��Ŷ ������
	void Send_Client_ID(int client_id, int value, bool allClient);	// Ŭ���̾�Ʈ ���� ��Ŷ ���̵� ������
	void Send_All_Data(int client, bool allClient);					// Ŭ���̾�Ʈ���� ��� Ŭ���̾�Ʈ ��ġ ������
	void Send_All_Time(int kind, int time, int client_id, bool allClient);					// Ŭ���̾�Ʈ���� �ð��� �����ش�.

public:
	HANDLE getHandle() { return g_hiocp; }
	IOCP_Server();
	~IOCP_Server();
};


#endif