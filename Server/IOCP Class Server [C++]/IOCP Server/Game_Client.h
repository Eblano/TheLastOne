#ifndef __GAMECLIENT_H__
#define __GAMECLIENT_H__

#include "IOCP_Server.h"

struct OverlappedEx {
	WSAOVERLAPPED over;
	WSABUF wsabuf;
	unsigned char IOCP_buf[MAX_BUFF_SIZE];
	OPTYPE event_type;
	int target_id;
};

struct xyz {
	float x;
	float y;
	float z;
};

class Game_Client {
private:
	int client_id = -1;
	xyz position;
	xyz rotation;
	int hp = -1;
	int animator = 0;
	bool connect;
	bool remove_client;
	bool shotting = false;

	SOCKET client_socket;
	int prev_packet_data; // ���� ó������ �ʴ� ��Ŷ�� �󸶳�
	int curr_packet_size; // ���� ó���ϰ� �ִ� ��Ŷ�� �󸶳�
	std::unordered_set<int> view_list; //�� set���� �ξ�������!
	std::mutex vl_lock;

public:
	char nickName[10];																							// Ŭ���̾�Ʈ �г���
	OverlappedEx recv_over;
	unsigned char packet_buf[MAX_PACKET_SIZE];

	void init();

	int get_client_id() { return this->client_id; }																// Ŭ���̾�Ʈ ���̵� ����
	int get_hp() { return this->hp; };																			// Ŭ���̾�Ʈ ü�� ����
	int get_animator() { return this->animator; };															// Ŭ���̾�Ʈ �ִϸ��̼� ����
	int get_shotting() { return this->shotting; };																// Ŭ���̾�Ʈ Shot ����
	bool get_Connect() { return this->connect; };															// Ŭ���̾�Ʈ ���� ���� ����
	bool get_Remove() { return this->remove_client; };													// Ŭ���̾�Ʈ ���� ���� ����
	int get_curr_packet() { return this->curr_packet_size; };												// Ŭ���̾�Ʈ ��Ŷ ������ ����
	int get_prev_packet() { return this->prev_packet_data; };												// Ŭ���̾�Ʈ ��Ŷ ������ ����
	Vec3 get_position() { return Vec3(this->position.x, this->position.y, this->position.z); };		// Ŭ���̾�Ʈ ������ ����
	Vec3 get_rotation() { return Vec3(this->rotation.x, this->rotation.y, this->rotation.z); };		// Ŭ���̾�Ʈ �����̼� ����
	SOCKET get_Socket() { return this->client_socket; };													// Ŭ���̾�Ʈ ���� ����
	OverlappedEx get_over() { return this->recv_over; };													// Overlapped ����ü ����




	void set_prev_packet(const int size) { this->prev_packet_data = size; };				// Ŭ���̾�Ʈ ��Ŷ ������ ����
	void set_curr_packet(const int size) { this->curr_packet_size = size; };				// Ŭ���̾�Ʈ ��Ŷ ������ ����
	void set_client_position(const xyz position) { this->position = position; };			// Ŭ���̾�Ʈ ������ ����
	void set_client_rotation(const xyz rotation) { this->rotation = rotation; };			// Ŭ���̾�Ʈ �����̼� ����
	void set_client_animator(const int value) { this->animator = value; };					// Ŭ���̾�Ʈ �ִϸ��̼� ����
	void set_client_shotting(const bool value) { this->shotting = value; };				// Ŭ���̾�Ʈ Shot ����
	void set_client_Connect(const bool value) { this->connect = value; };				// Ŭ���̾�Ʈ Connect ����
	void set_client_Remove(const bool value) { this->remove_client = value; };				// Ŭ���̾�Ʈ Remove ����

	Game_Client(const SOCKET sock, const int client_id, const char * game_id);
	Game_Client(const Game_Client& g_cl);
	~Game_Client();
};


#endif