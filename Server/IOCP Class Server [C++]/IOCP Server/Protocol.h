#ifndef __PROTOCOL_H__
#define __PROTOCOL_H__

enum TimerType { T_InitTime };
enum OPTYPE { OP_SEND, OP_RECV, OP_InitTime, OP_RemoveClient };
enum Event_Type { E_initTime, E_RoundTime, E_Remove_Client };

// ���� ����
#define SERVERPORT 9000
#define BUFSIZE    1024
#define MAX_BUFF_SIZE   4000
#define MAX_PACKET_SIZE  4000
#define MAX_Client 50

// ���� ����
#define DebugMod TRUE

// �������� Ŭ���̾�Ʈ���� ������ ��Ŷ
#define SC_ID           1					// Ŭ���̾�Ʈ ���̵� ������.
#define SC_PUT_PLAYER    2			// Ŭ���̾�Ʈ �߰�
#define SC_REMOVE_PLAYER 3		// Ŭ���̾�Ʈ ����
#define SC_Client_Data	4				// Ŭ���̾�Ʈ ��� ������
#define SC_Server_Time	5				// ���� Ÿ�̸�
#define SC_Server_Item	6				// ���� ������
#define SC_Shot_Client	7				// Ŭ���̾�Ʈ Shot ����

// Ŭ���̾�Ʈ�� �������� ������ ��Ŷ
#define CS_Info           1					// Ŭ���̾�Ʈ�� �������� �ڽ��� ��ġ������ �����ش�.
#define CS_Shot_info    2					// Ŭ���̾�Ʈ�� �������� Shot ������ �����ش�.
#define CS_Check_info  3					// Ŭ���̾�Ʈ�� �������� �ڽ��� ������ �´��� Ȯ���� �ش�.
#define CS_Eat_Item	   4					// Ŭ���̾�Ʈ�� �������� ���� ������ ������ �����ش�.

#endif