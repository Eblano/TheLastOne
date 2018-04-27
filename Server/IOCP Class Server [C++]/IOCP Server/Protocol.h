#ifndef __PROTOCOL_H__
#define __PROTOCOL_H__

enum TimerType { T_DangerLine };
enum OPTYPE { OP_SEND, OP_RECV, OP_DangerLine, OP_RemoveClient, OP_MoveDangerLine };
enum Event_Type { E_DangerLine, E_RoundTime, E_Remove_Client, E_MoveDangerLine };

// 소켓 설정
#define SERVERPORT 9000
#define BUFSIZE    1024
#define MAX_BUFF_SIZE   4000
#define MAX_PACKET_SIZE  4000
#define MAX_Client 50

// 게임 설정
#define DebugMod TRUE

// 서버에서 클라이언트에게 보내는 패킷
#define SC_ID           1					// 클라이언트 아이디를 보낸다.
#define SC_PUT_PLAYER    2			// 클라이언트 추가
#define SC_REMOVE_PLAYER 3		// 클라이언트 삭제
#define SC_Client_Data	4				// 클라이언트 모든 데이터
#define SC_Server_Time	5				// 서버 타이머
#define SC_Server_Item	6				// 서버 아이템
#define SC_Shot_Client	7				// 클라이언트 Shot 정보
#define SC_DangerLine	8				// 클라이언트 DangerLine 정보 전송
#define SC_Zombie_Info 9				// 클라이언트에게 좀비 위치를 전달해 준다.
#define SC_Remove_Zombie 10		// 좀비 삭제

// 클라이언트가 서버에게 보내는 패킷
#define CS_Info           1					// 클라이언트가 서버에게 자신의 위치정보를 보내준다.
#define CS_Shot_info    2					// 클라이언트가 서버에게 Shot 정보를 보내준다.
#define CS_Check_info  3					// 클라이언트가 서버에게 자신의 정보가 맞는지 확인해 준다.
#define CS_Eat_Item	   4					// 클라이언트가 서버에게 먹은 아이템 정보를 보내준다.
#define CS_Zombie_info 5					// 클라이언트가 서버에게 좀비 데이터를 전달해 준다.

// 자기장 시간
#define DangerLine_Level4 240
#define DangerLine_Level3 240
#define DangerLine_Level2 240
#define DangerLine_Level1 240
#define DangerLine_Level0 240

// 플레이어 볼 수 있는 거리
#define Player_Dist 300

// 좀비가 볼 수 있는 거리
#define Zombie_Dist 200
#define Limit_Zombie 10
#define Create_Zombie 50

// 아이템 종류
#define Kind_Item 0
#define Kind_Car 1

// 플레이어 종류
#define Kind_Player 0
#define Kind_Zombie 1

#endif