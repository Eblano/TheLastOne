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
			} // ť�� ��������� ������ �ȵǴϱ�
			Timer_Event t = timer_queue.top(); // ���� �̺�Ʈ �߿� ����ð��� ���� �ֱ��� �̺�Ʈ�� �����ؾ� �ϹǷ� �켱���� ť�� ����
			if (t.exec_time > high_resolution_clock::now()) {
				tq_lock.unlock();
				break; // ����ð����� ũ�ٸ�, ��ٷ���
			}
			timer_queue.pop();
			tq_lock.unlock();
			OverlappedEx *over = new OverlappedEx;
			if (E_initTime == t.event) {
				over->event_type = OP_InitTime;
			}
			else if (E_Remove_Client == t.event) {
				over->event_type = OP_RemoveClient;
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

	Timer_Event t = { 600, high_resolution_clock::now() + 1s, E_initTime };
	setTimerEvent(t);

	t = { 1, high_resolution_clock::now() + 1s, E_Remove_Client };
	setTimerEvent(t);

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
