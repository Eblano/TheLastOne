#ifndef __GAMEDANGERLINE_H__
#define __GAMEDANGERLINE_H__

#include "Core_Header.h"

class Game_DangerLine {
private:
	int level;			// �ڱ��� ����
	int demage;		// �ڱ��� ������
	xyz position;	// ������ ��ġ
	xyz scale;			// �ڱ��� ũ��
	xyz now_scale;	// ���� �ڱ��� ũ��
	int time;			// �ڱ��� ��� �ð�

public:
	void set_level(int value);
	void set_scale(int value);
	int get_level() { return this->level; }
	int get_time() { return this->time; }
	int get_demage() { return this->demage; }
	float get_scale_x() { return this->scale.x; }
	float get_now_scale_x() { return this->now_scale.x; }
	xyz get_pos() { return this->position; }
	xyz get_scale() { return this->now_scale; }

	void init();
	Game_DangerLine();
	~Game_DangerLine();
};


#endif