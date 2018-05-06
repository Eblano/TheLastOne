#ifndef __GAMECLIENT_H__
#define __GAMECLIENT_H__

#include "Core_Header.h"

class Game_Client {
private:
	int client_id = -1;
	int room_id = -1;
	xyz position;
	xyz rotation;
	xyz car_rotation;
	int hp = -1;
	int armour = 0;
	int animator = 0;
	int weaponState = 0;
	int limit_Zombie = 0;
	int inCar = -1;
	int playerStatus = 0;
	float horizontal = 0.0f;
	float vertical = 0.0f;
	bool connect;
	bool remove_client;
	bool dangerLineIn;
	SOCKET client_socket;
	int prev_packet_data; // ���� ó������ �ʴ� ��Ŷ�� �󸶳�
	int curr_packet_size; // ���� ó���ϰ� �ִ� ��Ŷ�� �󸶳�
	

public:
	std::string nickName;																					// Ŭ���̾�Ʈ �г���
	OverlappedEx recv_over;
	unsigned char packet_buf[MAX_PACKET_SIZE];

	void init();
	int get_room_id() { return this->room_id; }						// Ŭ���̾�Ʈ �� ���� ����
	int get_inCar() { return this->inCar; }								// Ŭ���̾�Ʈ ���� ���� ž�� �� ����
	float get_vertical() { return (float)this->vertical; }				// Ŭ���̾�Ʈ �ִϸ��̼� �� ����
	float get_horizontal() { return (float)this->horizontal; }		// Ŭ���̾�Ʈ �ִϸ��̼� �� ����
	int get_limit_zombie() { return this->limit_Zombie; }			// Ŭ���̾�Ʈ ���� �ִ�ġ ����
	int get_client_id() { return this->client_id; }						// Ŭ���̾�Ʈ ���̵� ����
	int get_hp() { return this->hp; };									// Ŭ���̾�Ʈ ü�� ����
	int get_armour() { return this->armour; };						// Ŭ���̾�Ʈ �Ƹ� ����
	int get_animator() { return this->animator; };					// Ŭ���̾�Ʈ �ִϸ��̼� ����
	int get_weapon() { return this->weaponState; };				// Ŭ���̾�Ʈ WeaponState ����
	bool get_Connect() { return this->connect; };					// Ŭ���̾�Ʈ ���� ���� ����
	bool get_Remove() { return this->remove_client; };			// Ŭ���̾�Ʈ ���� ���� ����
	bool get_DangerLine() { return this->dangerLineIn; };			// Ŭ���̾�Ʈ �ڱ��� ����
	int get_curr_packet() { return this->curr_packet_size; };		// Ŭ���̾�Ʈ ��Ŷ ������ ����
	int get_prev_packet() { return this->prev_packet_data; };		// Ŭ���̾�Ʈ ��Ŷ ������ ����
	int get_playerStatus() { return this->playerStatus; };		// Ŭ���̾�Ʈ ���� ���� ����
	//xyz get_pos() { return this->position; };
	xyz get_position();													// Ŭ���̾�Ʈ ������ ����
	xyz get_rotation();													// Ŭ���̾�Ʈ �����̼� ����
	xyz get_car_rotation();												// Ŭ���̾�Ʈ �����̼� ����
	SOCKET get_Socket() { return this->client_socket; };			// Ŭ���̾�Ʈ ���� ����
	OverlappedEx get_over() { return this->recv_over; };			// Overlapped ����ü ����

	void set_prev_packet(const int size) { this->prev_packet_data = size; };				// Ŭ���̾�Ʈ ��Ŷ ������ ����
	void set_curr_packet(const int size) { this->curr_packet_size = size; };				// Ŭ���̾�Ʈ ��Ŷ ������ ����
	void set_client_position(const xyz position) { this->position = position; };			// Ŭ���̾�Ʈ ������ ����
	void set_client_rotation(const xyz rotation) { this->rotation = rotation; };			// Ŭ���̾�Ʈ �����̼� ����
	void set_client_car_rotation(const xyz rotation) { this->car_rotation = rotation; };			// Ŭ���̾�Ʈ ���� �����̼� ����
	void set_client_animator(const int value) { this->animator = value; };					// Ŭ���̾�Ʈ �ִϸ��̼� ����
	void set_client_weapon(const int value) { this->weaponState = value; };				// Ŭ���̾�Ʈ WeaponState ����
	void set_client_Connect(const bool value) { this->connect = value; };				// Ŭ���̾�Ʈ Connect ����
	void set_client_Remove(const bool value) { this->remove_client = value; };				// Ŭ���̾�Ʈ Remove ����
	void set_client_DangerLine(const bool value) { this->dangerLineIn = value; };				// Ŭ���̾�Ʈ �ڱ��� �� ����
	void set_limit_zombie(const int value) { this->limit_Zombie += value; }				// Ŭ���̾�Ʈ ���� �ִ�ġ ����
	void set_playerStatus(const int value) { this->playerStatus = value; }				// Ŭ���̾�Ʈ ���� ���� ����
	void set_vertical(float value) { this->vertical = (float)value; }
	void set_horizontal(float value) {  this->horizontal = (float)value; }
	void set_inCar(int value) { this->inCar = value; }												// Ŭ���̾�Ʈ ���� ���� ž�� �� ����
	void set_hp(int value) { this->hp = value; };														// Ŭ���̾�Ʈ ü�� ����
	void set_armour(int value) { this->armour = value; };											// Ŭ���̾�Ʈ �Ƹ� ����
	void set_room_id(int value) { this->room_id = value; }										// Ŭ���̾�Ʈ �� ���̵� ����

	Game_Client(const SOCKET sock, const int client_id, const char * game_id, const int room_id);
	Game_Client(const Game_Client& g_cl);
	~Game_Client();
};

#endif