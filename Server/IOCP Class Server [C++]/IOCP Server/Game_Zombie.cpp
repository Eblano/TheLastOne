#include "Game_Zombie.h"

Game_Zombie::Game_Zombie(const int client_id)
{
	//< 1�ܰ�. �õ� ����
	std::random_device rn;
	std::mt19937_64 rnd(rn());
	std::uniform_int_distribution<int> xRange(250, 1920);
	std::uniform_int_distribution<int> zRange(250, 2500);

	this->client_id = client_id;
	this->hp = Zombie_HP;
	this->position.x = (float)xRange(rnd);
	this->position.y = 29.99451f;
	this->position.z = (float)zRange(rnd);
	this->rotation.x = 0;
	this->rotation.y = 0;
	this->rotation.z = 0;
	this->target_Player = -1;
	this->distance = Zombie_Dist;		// �ʱⰪ�� ���� ���� ������ �������ش�.
	this->live = true;
}

Game_Zombie::~Game_Zombie()
{
}

void init_Zombie(int count, std::unordered_map<int, Game_Zombie>* zombie)
{
	for (int i = 0; i < count; ++i) {
		zombie->insert(std::pair<int, Game_Zombie>(i, { i }));
	}
}
