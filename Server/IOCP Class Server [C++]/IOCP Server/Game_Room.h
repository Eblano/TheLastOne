#ifndef __INGAME_H__
#define __INGAME_H__

#include "Game_Client.h"
#include "Game_Item.h"
#include "Game_Zombie.h"
#include "Game_DangerLine.h"


class Game_Room {
private:
	std::unordered_map< int, Game_Client> g_clients;
	std::unordered_map< int, Game_Item> g_item;
	std::unordered_map< int, Game_Zombie> g_zombie;
	Game_DangerLine dangerLine;
	bool playGame;

public:
	bool get_playGame() { return this->playGame; }
	std::unordered_map< int, Game_Client> & get_client() { return this->g_clients; }		// Ŭ���̾�Ʈ ������ ����
	std::unordered_map< int, Game_Item> & get_item() { return this->g_item; }		// ������ ������ ����
	std::unordered_map< int, Game_Zombie> & get_zombie() { return this->g_zombie; }	// ���� ������ ����
	Game_DangerLine & get_dangerLine() { return this->dangerLine; }		// DangerLine ������ ����


	void set_playGame(bool value) { this->playGame = value; }
	std::unordered_map< int, Game_Client>::iterator get_client_iter(int ci);
	std::unordered_map< int, Game_Item>::iterator get_item_iter(int ci);
	std::unordered_map< int, Game_Zombie>::iterator get_zombie_iter(int ci);
	void player_To_Zombie();
	int check_ReadyClients();
	void room_init();

	Game_Room();
	~Game_Room();
};


#endif