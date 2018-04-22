#ifndef __GAMEZOMBIE_H__
#define __GAMEZOMBIE_H__

#include "IOCP_Server.h"

class Game_Zombie {
private:
	int client_id = -1;
	xyz position;
	xyz rotation;
	int hp = -1;
	int animator = 0;

public:
	int get_client_id() { return this->client_id; }																// Ŭ���̾�Ʈ ���̵� ����
	int get_hp() { return this->hp; };																			// Ŭ���̾�Ʈ ü�� ����
	int get_animator() { return this->animator; };															// Ŭ���̾�Ʈ �ִϸ��̼� ����
	Vec3 get_position() { return Vec3(this->position.x, this->position.y, this->position.z); };		// Ŭ���̾�Ʈ ������ ����
	Vec3 get_rotation() { return Vec3(this->rotation.x, this->rotation.y, this->rotation.z); };		// Ŭ���̾�Ʈ �����̼� ����


	void set_client_position(const xyz position) { this->position = position; };			// Ŭ���̾�Ʈ ������ ����
	void set_client_rotation(const xyz rotation) { this->rotation = rotation; };			// Ŭ���̾�Ʈ �����̼� ����
	void set_client_animator(const int value) { this->animator = value; };					// Ŭ���̾�Ʈ �ִϸ��̼� ����

	Game_Zombie(const int client_id);
	~Game_Zombie();
};

void init_Zombie(int count, std::unordered_map<int, Game_Zombie>* zombie);
#endif