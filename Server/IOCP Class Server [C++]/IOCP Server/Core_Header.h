#ifndef __COREHEADER_H__
#define __COREHEADER_H__
#pragma comment(lib, "ws2_32")
#include <unordered_map>
#include <unordered_set> // ������ �� ��������. [������ ����������]
#include <string>
#include <winsock2.h>
#include <random>
#include <iostream>
#include <thread>
#include <mutex>

#include "xyz.h"
#include "Protocol.h"

struct OverlappedEx {
	WSAOVERLAPPED over;
	WSABUF wsabuf;
	unsigned char IOCP_buf[MAX_BUFF_SIZE];
	OPTYPE event_type;
	int target_id;
	int room_id;
};
#endif