#include "Timer.h"

void Server_Timer::Timer_Thread()
{
	for (;;) {
		Sleep(10);
		for (;;) {
			tq_lock.lock();
			if (0 == timer_queue.size()) {
				tq_lock.unlock();
				break;
			} // 큐가 비어있으면 꺼내면 안되니까
			Timer_Event t = timer_queue.top(); // 여러 이벤트 중에 실행시간이 제일 최근인 이벤트를 실행해야 하므로 우선순위 큐를 만듬
			if (t.exec_time > high_resolution_clock::now()) {
				tq_lock.unlock();
				break; // 현재시간보다 크다면, 기다려줌
			}
			timer_queue.pop();
			tq_lock.unlock();
			OverlappedEx *over = new OverlappedEx;
			if (E_DangerLine == t.event) {
				over->room_id = t.room_id;
				over->event_type = OP_DangerLine;
			}
			else if (E_Remove_Client == t.event) {
				over->room_id = t.room_id;
				over->event_type = OP_RemoveClient;
			}
			else if (E_MoveDangerLine == t.event) {
				over->room_id = t.room_id;
				over->event_type = OP_MoveDangerLine;
			}
			else if (E_DangerLineDamage == t.event) {
				over->room_id = t.room_id;
				over->event_type = OP_DangerLineDamage;
			}
			else if (E_LobbyWait == t.event) {
				over->room_id = t.room_id;
				over->event_type = OP_LobbyWait;
			}
			else if (E_LobbyReday == t.event) {
				over->room_id = t.room_id;
				over->event_type = OP_LobbyReday;
			}
			else if (E_StartCarWait == t.event) {
				over->room_id = t.room_id;
				over->event_type = OP_StartCarWait;
			}
			
			PostQueuedCompletionStatus(g_hiocp, 1, t.object_id, &over->over);
		}
	}
}

void Server_Timer::setTimerEvent(Timer_Event t)
{
	tq_lock.lock();
	timer_queue.push(t);
	tq_lock.unlock();
}

void Server_Timer::initTimer(HANDLE handle)
{
	g_hiocp = handle;

	//t = { 1, high_resolution_clock::now() + 1s, E_Remove_Client };
	//setTimerEvent(t);



	
	timer_tread = std::thread(&Server_Timer::Timer_Thread, this);
	timer_tread.join();
}

Server_Timer::Server_Timer()
{
	serverTimer = high_resolution_clock::now();
}

Server_Timer::~Server_Timer()
{
	timer_tread.join();
}
