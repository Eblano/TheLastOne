#ifndef __GAMEDANGERLINE_H__
#define __GAMEDANGERLINE_H__

#include "IOCP_Server.h"

class Game_DangerLine {
private:
	int level;			// �ڱ��� ����
	int demage;		// �ڱ��� ������
	xyz position;	// ������ ��ġ
	xyz scale;			// �ڱ��� ũ��


public:
	
	Game_DangerLine();
	~Game_DangerLine();
};


#endif