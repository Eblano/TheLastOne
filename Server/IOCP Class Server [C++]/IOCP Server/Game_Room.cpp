#include "Game_Room.h"

std::unordered_map<int, Game_Client>::iterator Game_Room::get_client_iter(int ci)
{
	if (g_clients.find(ci) == g_clients.end()) {
		return g_clients.end();
	}
	else {
		return g_clients.find(ci);
	}
}

std::unordered_map<int, Game_Item>::iterator Game_Room::get_item_iter(int ci)
{
	return g_item.find(ci);
}

std::unordered_map<int, Game_Zombie>::iterator Game_Room::get_zombie_iter(int ci)
{
	return g_zombie.find(ci);
}

float DistanceToPoint(const xyz player, const xyz zombie)
{
	// �÷��̾�� ������ �Ÿ� ���ϱ�.
	return (float)sqrt(pow(player.x - zombie.x, 2) + pow(player.z - zombie.z, 2));
}


void Game_Room::player_To_Zombie()
{
	float dist = 0.0f;
	for (auto zombie : g_zombie) {

		if (zombie.second.get_target() != -1 && g_clients.find(zombie.second.get_target()) == g_clients.end()) {
			// �÷��̾ ���� ���� �� ��� ���� �ʱ�ȭ
			zombie.second.set_distance(Zombie_Dist);
			zombie.second.set_target(-1);
		}
		if (zombie.second.get_hp() <= 0 && zombie.second.get_live() != false) {
			// ������ ü���� 0�ϰ��
			g_zombie.find(zombie.first)->second.set_live(false);
			g_zombie.find(zombie.first)->second.set_target(-1);
		}

		for (auto player : g_clients) {
			dist = DistanceToPoint(player.second.get_position(), zombie.second.get_position());

			if (dist <= zombie.second.get_distance() && dist <= Zombie_Dist && player.second.get_hp() > 0) {
				// �ֱ� ������ �Ÿ��� �� �ְ�� ����� �Ÿ��� �������ش�.
				g_zombie.find(zombie.first)->second.set_distance(dist);
				g_zombie.find(zombie.first)->second.set_target(player.second.get_client_id());
			}
			if (dist >= Zombie_Dist) {
				// �÷��̾ ���������� �Ÿ��� �־��� ���� ��� ���� Target�� �ʱ�ȭ
				zombie.second.set_distance(Zombie_Dist);
				g_zombie.find(zombie.first)->second.set_target(-1);
			}
		}
		// �÷��̾ ��� Ȯ���� ���� zombie dist�� �ʱ�ȭ ���ش�.
		zombie.second.set_distance(Zombie_Dist);
	}
}

int Game_Room::check_ReadyClients()
{
	int readyClients = 0;
	for (auto iter : g_clients) {
		if (iter.second.get_playerStatus() == ReadyStatus) {
			++readyClients;
		}
	}
	return readyClients;
}

void Game_Room::room_init()
{
	g_item.clear();
	g_zombie.clear();
	g_clients.clear();
	dangerLine.init();

	this->playGame = false;

	// ���� ������ ���� g_item�� �־��ֱ�.
	load_item_txt("./Game_Item_Collection.txt", &g_item);

	// ���� ĳ���� �����ϱ�
	init_Zombie(Create_Zombie, &g_zombie);
}

Game_Room::Game_Room()
{
	// ���� ������ ���� g_item�� �־��ֱ�.
	load_item_txt("./Game_Item_Collection.txt", &g_item);

	// ���� ĳ���� �����ϱ�
	init_Zombie(Create_Zombie, &g_zombie);

	this->playGame = false;
	/*for (auto iter : g_zombie) {
		std::cout << iter.first << " : " << iter.second.get_pos().x << ", " << iter.second.get_pos().y << ", " << iter.second.get_pos().z << std::endl;
	}*/

}

Game_Room::~Game_Room()
{
}
