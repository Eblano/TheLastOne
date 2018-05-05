#include "Game_Room.h"

std::unordered_map<int, Game_Client>::iterator Game_Room::get_client_iter(int ci)
{
	return g_clients.find(ci);
}

std::unordered_map<int, Game_Item>::iterator Game_Room::get_item_iter(int ci)
{
	return g_item.find(ci);
}

std::unordered_map<int, Game_Zombie>::iterator Game_Room::get_zombie_iter(int ci)
{
	return g_zombie.find(ci);
}

float DistanceToPoint(float player_x, float player_z, float zombie_x, float zombie_z)
{
	// ĳ���� ���� �Ÿ� ���ϱ�.
	return (float)sqrt(pow(player_x - zombie_x, 2) + pow(player_z - zombie_z, 2));
}


void Game_Room::player_To_Zombie()
{
	for (auto z : g_zombie) {
		// TargetPlayer�� ���� ������ �����ϴ��� Ȯ������.
		if (g_clients.find(z.second.get_target()) != g_clients.end()) {
			// �÷��̾ �����Ѵ�.
			auto iter = g_clients.find(z.second.get_target());
			float check_dist = DistanceToPoint(iter->second.get_pos().x, iter->second.get_pos().z, z.second.get_pos().x, z.second.get_pos().z);
			if (check_dist >= Zombie_Dist) {
				// �÷��̾ ���������� �Ÿ��� �־��� ���� ��� ���� Target�� �ʱ�ȭ
				iter->second.set_limit_zombie(-1);
				g_zombie.find(z.first)->second.set_target(-1);
			}
			if (iter->second.get_hp() <= 0) {
				// �÷��̾� ü���� 0 ������ ���
				iter->second.set_limit_zombie(-1);
				g_zombie.find(z.first)->second.set_target(-1);
			}
			if (z.second.get_hp() <= 0 && z.second.get_live() != false) {
				// ������ ü���� 0�ϰ��
				g_zombie.find(z.first)->second.set_live(false);
				iter->second.set_limit_zombie(-1);
				//std::cout << iter->first << ", " << iter->second.get_limit_zombie() << std::endl;
				g_zombie.find(z.first)->second.set_target(-1);
			}
			else {
				// ���� ��ó�� ��� �ѱ��.
				continue;
			}
		}
		else {
			int player_num = -1;
			float dist = Zombie_Dist;
			for (auto p : g_clients) {
				float check_dist = DistanceToPoint(p.second.get_pos().x, p.second.get_pos().z, z.second.get_pos().x, z.second.get_pos().z);
				if (dist >= check_dist) {
					dist = check_dist;
					player_num = p.first;
				}
			}
			if (player_num == -1)
				continue;
			if (g_clients.find(player_num)->second.get_limit_zombie() < Limit_Zombie) {
				g_clients.find(player_num)->second.set_limit_zombie(1);
				g_zombie.find(z.first)->second.set_target(player_num);
			}
		}
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
