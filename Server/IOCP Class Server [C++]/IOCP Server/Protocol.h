#ifndef __PROTOCOL_H__
#define __PROTOCOL_H__

enum TimerType { T_DangerLine };
enum OPTYPE { OP_SEND, OP_RECV, OP_DangerLine, OP_RemoveClient, OP_MoveDangerLine };
enum Event_Type { E_DangerLine, E_RoundTime, E_Remove_Client, E_MoveDangerLine };

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
#define SC_DangerLine	8				// Ŭ���̾�Ʈ DangerLine ���� ����
#define SC_Zombie_Info 9				// Ŭ���̾�Ʈ���� ���� ��ġ�� ������ �ش�.
#define SC_Remove_Zombie 10		// ���� ����

// Ŭ���̾�Ʈ�� �������� ������ ��Ŷ
#define CS_Info           1					// Ŭ���̾�Ʈ�� �������� �ڽ��� ��ġ������ �����ش�.
#define CS_Shot_info    2					// Ŭ���̾�Ʈ�� �������� Shot ������ �����ش�.
#define CS_Check_info  3					// Ŭ���̾�Ʈ�� �������� �ڽ��� ������ �´��� Ȯ���� �ش�.
#define CS_Eat_Item	   4					// Ŭ���̾�Ʈ�� �������� ���� ������ ������ �����ش�.
#define CS_Zombie_info 5					// Ŭ���̾�Ʈ�� �������� ���� �����͸� ������ �ش�.
#define CS_Object_HP 6						// Ŭ���̾�Ʈ�� �������� HP �����͸� ������ �ش�.
#define CS_Car_Riding 7						// Ŭ���̾�Ʈ�� �������� ������ ž���ߴٰ� ������ �ش�.
#define CS_Car_Rode 8						// Ŭ���̾�Ʈ�� �������� ������ �����ߴٰ� ������ �ش�.

// �ڱ��� �ð�
#define DangerLine_Level4 240
#define DangerLine_Level3 240
#define DangerLine_Level2 240
#define DangerLine_Level1 240
#define DangerLine_Level0 240

// �÷��̾� �� �� �ִ� �Ÿ�
#define Player_Dist 300

// ���� �� �� �ִ� �Ÿ�
#define Zombie_Dist 200
#define Limit_Zombie 1
#define Create_Zombie 1

// ������, �÷��̾� ����
#define Kind_Item 0
#define Kind_Car 1
#define Kind_Player 2
#define Kind_Zombie 3

// ü�� ����
#define Car_HP 200
#define Player_HP 100
#define Zombie_HP 100

#endif