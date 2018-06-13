#ifndef __GAMEROOM_H__
#define __GAMEROOM_H__
#include "Game_Room.h"


class Room_Manager {
private:
	int id;		// �� ID
	int mapType;
	int gameStatus;
	//HANDLE *g_hiocp;
	Game_Room inGame;		// ���� ���� ������.	

public:
	Game_Room & get_room() { return this->inGame; }		// ���� ���ӷ��� ����.
	int get_status() { return this->gameStatus; }			// �뿡 ���� ���¸� ����.
	int get_id() { return this->id; }		// �� ���̵� ����.
	int get_mapType() { return this->mapType; }

	void set_mapType(int value) { this->mapType = value; }
	void set_status(int value) { this->gameStatus = value; }		// �뿡 ���� ���¸� ����.
	Room_Manager(const int id, const int mapType);
	Room_Manager(const Room_Manager& g_r);
	~Room_Manager();
};
#endif