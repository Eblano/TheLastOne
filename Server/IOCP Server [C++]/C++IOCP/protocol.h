#ifndef __PROTOCOL_H__
#define __PROTOCOL_H__

// �������� Ŭ���̾�Ʈ���� ������ ��Ŷ
#define SC_ID           1					// Ŭ���̾�Ʈ ���̵� ������.
#define SC_PUT_PLAYER    2			// Ŭ���̾�Ʈ �߰�
#define SC_REMOVE_PLAYER 3		// Ŭ���̾�Ʈ ����
#define SC_Client_Data	4				// Ŭ���̾�Ʈ ��� ������

// Ŭ���̾�Ʈ�� �������� ������ ��Ŷ
#define CS_Info           1					// Ŭ���̾�Ʈ�� �������� �ڽ��� ��ġ������ �����ش�.
#define CS_Shot_info    2					// Ŭ���̾�Ʈ�� �������� Shot ������ �����ش�.
#define CS_Check_info  3					// Ŭ���̾�Ʈ�� �������� �ڽ��� ������ �´��� Ȯ���� �ش�.

// BYTE�� 255������ �ν��Ͽ� int���� BOOL�� ����

struct sc_packet_put_player {
	BYTE size;
	BYTE type;
	WORD id;
	BOOL x;
	BOOL y;
	BOOL direction;
	BOOL movement;
};

struct sc_packet_pos {
	BYTE size;
	BYTE type;
	WORD id;
	BOOL x;
	BOOL y;
	BOOL direction;
	BOOL movement;
};

struct sc_packet_remove_player {
	BYTE size;
	BYTE type;
	WORD id;
};

struct cs_packet_chat {
	BYTE size;
	BYTE type;
	WCHAR message[MAX_STR_SIZE];
};

struct sc_packet_chat {
	BYTE size;
	BYTE type;
	WORD id;
	WCHAR message[MAX_STR_SIZE];
};

struct cs_packet_Move {
	BYTE size;
	BYTE type;
	BYTE x;
	BYTE y;
};

#endif