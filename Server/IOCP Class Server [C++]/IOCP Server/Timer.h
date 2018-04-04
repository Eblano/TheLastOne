#ifndef __GAMETIMER_H__
#define __GAMETIMER_H__

#include "IOCP_Server.h"
#include "Game_Client.h"

#include <chrono>
#include <queue>

struct Timer_Event {
	int object_id;	// Ư�� ��� Ȥ�� �ð��� �ִ´�.
	high_resolution_clock::time_point exec_time; // �� �̺�Ʈ�� ���� ����Ǿ� �ϴ°�
	Event_Type event; // ���� �̺�Ʈ�� �ִ�.
};

class comparison {
	bool reverse;
public:
	comparison() {}
	bool operator() (const Timer_Event first, const Timer_Event second) const {
		return first.exec_time > second.exec_time;
	}
};

class Server_Timer {
private:
	HANDLE g_hiocp;
	std::thread timer_tread;
	std::mutex tq_lock;
	std::chrono::high_resolution_clock::time_point serverTimer;
	std::priority_queue <Timer_Event, std::vector<Timer_Event>, comparison> timer_queue;
	void Timer_Thread();

public:
	void setTimerEvent(Timer_Event t);
	void initTimer(HANDLE g_hiocp);
	

	Server_Timer();
	~Server_Timer();
};


#endif