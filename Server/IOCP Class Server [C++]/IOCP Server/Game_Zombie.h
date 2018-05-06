#ifndef __GAMEZOMBIE_H__
#define __GAMEZOMBIE_H__

#include "Core_Header.h"

class Game_Zombie {
private:
	int client_id = -1;
	xyz position;
	xyz rotation;
	int hp = -1;
	int animator = 0;
	int target_Player = -1;
	bool live = false;
	float distance;

public:
	float get_distance() { return this->distance; }															// ����� �÷��̾� �Ÿ� ����
	bool get_live() { return this->live; }																		// ���� ���� ���� ����
	int get_client_id() { return this->client_id; }																// ���� ���̵� ����
	int get_hp() { return this->hp; };																			// ���� ü�� ����
	int get_animator() { return this->animator; };															// ���� �ִϸ��̼� ����
	int get_target() { return this->target_Player; };															// ���� Ÿ�� �÷��̾� ����
	//xyz get_pos() { return this->position; };
	xyz get_position() { return this->position; };		// ���� ������ ����
	xyz get_rotation() { return this->rotation; };		// ���� �����̼� ����


	void set_distance(const float value) { this->distance = value; }							// ����� �÷��̾� �Ÿ� ����
	void set_live(const bool value) { this->live = value; };										// ���� ���� ���� ����
	void set_target(const int value) { this->target_Player = value; };						// ���� Ÿ�� �÷��̾� ����
	void set_animator(const int value) { this->animator = value; };							// ���� �ִϸ��̼� ����
	void set_hp(const int value) { this->hp = value; };											// ���� ü�� ����
	void set_zombie_position(const xyz position) { this->position = position; };			// ���� ������ ����
	void set_zombie_rotation(const xyz rotation) { this->rotation = rotation; };			// ���� �����̼� ����
	void set_zombie_animator(const int value) { this->animator = value; };					// ���� �ִϸ��̼� ����

	Game_Zombie(const int client_id);
	~Game_Zombie();
};
void init_Zombie(int count, std::unordered_map<int, Game_Zombie>* zombie);
#endif