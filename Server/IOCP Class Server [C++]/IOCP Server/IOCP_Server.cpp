﻿#include "IOCP_Server.h"
#include "Timer.h"

IOCP_Server::IOCP_Server()
{
	std::wcout.imbue(std::locale("korean"));
	initServer();
	makeThread();
}

IOCP_Server::~IOCP_Server()
{
	//Shutdown_Server();
}

void IOCP_Server::initServer()
{
	serverTimer = high_resolution_clock::now();
	WSADATA	wsadata;
	WSAStartup(MAKEWORD(2, 2), &wsadata);
	g_hiocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, NULL, 0);
	g_socket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);

	SOCKADDR_IN ServerAddr;
	ZeroMemory(&ServerAddr, sizeof(SOCKADDR_IN));
	ServerAddr.sin_family = AF_INET;
	ServerAddr.sin_port = htons(SERVERPORT);
	ServerAddr.sin_addr.s_addr = INADDR_ANY;

	bind(g_socket, reinterpret_cast<sockaddr *>(&ServerAddr), sizeof(ServerAddr));
	listen(g_socket, 5);

	int option = TRUE;
	setsockopt(g_socket, IPPROTO_TCP, TCP_NODELAY, (const char*)&option, sizeof(option));	// 네이글 알고리즘 OFF

	for (int i = 0; i < Total_Max_Client; ++i) {
		// 클라이언트 고유번호 넣어 주기.
		remove_client_id.push(i);
	}

	// 게임 대기방 생성
	GameRoom.reserve(GameRoomLimit);
	create_GameRoom(-1);
	//for (int i = 0; i < 1; ++i) {
	//	GameRoom.emplace_back(Room_Manager(i, -1));	// 대기방은 MapType 이 -1
	//}
	std::cout << "init Complete..!" << std::endl;
}

void IOCP_Server::err_quit(char * msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	printf("Error : %s\n", msg);
	LocalFree(lpMsgBuf);
	exit(1);
}

void IOCP_Server::err_display(char * msg, int err_no)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, err_no,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	std::cout << "[" << msg << "]" << (char *)lpMsgBuf << std::endl;
	LocalFree(lpMsgBuf);
}

void IOCP_Server::makeThread()
{
	std::vector <std::thread * > worker_threads;
	for (int i = 0; i < 6; ++i) {
		worker_threads.emplace_back(new std::thread{ &IOCP_Server::Worker_Thread, this });
	}

	std::thread accept_tread{ &IOCP_Server::Accept_Thread, this };

	Timer.initTimer(g_hiocp);		// 타이머 스레드를 만들어 준다.

	accept_tread.join();
	for (auto pth : worker_threads) {
		pth->join();
		delete pth;
	}

	worker_threads.clear();
}

void IOCP_Server::Worker_Thread()
{
	while (true) {
		DWORD io_size;
		unsigned long long ci;
		OverlappedEx *over;
		BOOL ret = GetQueuedCompletionStatus(g_hiocp, &io_size, &ci, reinterpret_cast<LPWSAOVERLAPPED *>(&over), INFINITE);

		if (FALSE == ret) {
			int err_no = WSAGetLastError();
			if (err_no == 64)
				DisconnectClient(over->room_id, (int)ci);
			else
				err_display((char *)"QOCS : ", WSAGetLastError());
		}

		if (0 == io_size) {
			DisconnectClient(over->room_id, (int)ci);
			continue;
		}

		if (OP_RECV == over->event_type) {
#if (DebugMod == TRUE )
			//std::cout << "RECV from Client : " << ci << std::endl;
			//std::cout << "IO_SIZE : " << io_size << std::endl;
#endif

			auto client = GameRoom[over->room_id].get_room().get_client_iter((int)ci);
			unsigned char *buf = client->second.recv_over.IOCP_buf;

			unsigned psize = client->second.get_curr_packet();
			unsigned pr_size = client->second.get_prev_packet();
			int packet_i = 0;
			char packet_data[8]{ 0 };
			while (io_size != 0) {
				if (0 == psize) {
					for ( packet_i = 0; packet_i < 8; ++packet_i) {
						if (buf[packet_i] != 124)
							packet_data[packet_i] = buf[packet_i];
						else
							break;
					}
					psize = atoi(packet_data) + 8;
				}
				// 0일 경우[바로전 패킷이 처리가 끝나고 새피킷으로 시작해도 된다. / 처음 받는다] 강제로 정해준다.
				if (io_size + pr_size >= psize) {
					// 지금 패킷 완성이 가능하다.
					char packet[MAX_PACKET_SIZE];
					memcpy(packet, client->second.packet_buf, pr_size);
					memcpy(packet + pr_size, buf, psize - pr_size);

					if (psize > 19) {
						ProcessPacket(over->room_id, static_cast<int>(ci), psize, packet_i, packet);
					}
					io_size -= psize - pr_size;
					buf += psize - pr_size;
					psize = 0; pr_size = 0;
				}
				else {
					memcpy(client->second.packet_buf + pr_size, buf, io_size);
					pr_size += io_size;
					io_size = 0;
				}
			}
			client->second.set_curr_packet(psize);
			client->second.set_prev_packet(pr_size);
			DWORD recv_flag = 0;

			if (over->room_change == true || over->target_id >= MAX_Client || over->target_id < 0) {
				GameRoom[0].get_room().get_client().erase((int)ci);
			}
			else {
				WSARecv(client->second.get_Socket(), &client->second.recv_over.wsabuf, 1, NULL, &recv_flag, &client->second.recv_over.over, NULL);
			}
		}
		else if (OP_SEND == over->event_type) {

			auto client = GameRoom[over->room_id].get_room().get_client_iter((int)ci);

			if (io_size != over->wsabuf.len) { // 같아야 되는데 다르므로 
#if (DebugMod == TRUE )
				std::cout << "Send Incomplete Error!" << std::endl;
#endif
				closesocket(client->second.get_Socket());
				client->second.init();
				exit(-1);
			}
			delete over;
		}
		else if (OP_DangerLine == over->event_type) {
			Send_All_Time(over->room_id, SC_Server_Time, T_DangerLine, (int)ci, -1, true);
			if (ci <= 0) {
				// 초기 대기시간이 만료 되었을 경우
				mtx.lock();
				GameRoom[over->room_id].get_room().get_dangerLine().set_level(GameRoom[over->room_id].get_room().get_dangerLine().get_level() - 1);
				mtx.unlock();
				Send_DangerLine_info(over->room_id, GameRoom[over->room_id].get_room().get_dangerLine().get_demage(), GameRoom[over->room_id].get_room().get_dangerLine().get_pos(), GameRoom[over->room_id].get_room().get_dangerLine().get_scale());
				if (GameRoom[over->room_id].get_room().get_dangerLine().get_level() != -1) {
					Timer_Event t = { over->room_id, (int)GameRoom[over->room_id].get_room().get_dangerLine().get_scale_x() , high_resolution_clock::now() + 100ms, E_MoveDangerLine };
					if (GameRoom[over->room_id].get_status() == inGameStatus)
						Timer.setTimerEvent(t);
				}

			}
			else {
				// ci-1 을 한 이유는 1초씩 내리기 위하여.
				Send_DangerLine_info(over->room_id, GameRoom[over->room_id].get_room().get_dangerLine().get_demage(), GameRoom[over->room_id].get_room().get_dangerLine().get_pos(), GameRoom[over->room_id].get_room().get_dangerLine().get_scale());
				Timer_Event t = { over->room_id, (int)ci - 1, high_resolution_clock::now() + 1s, E_DangerLine };
				if (GameRoom[over->room_id].get_status() == inGameStatus)
					Timer.setTimerEvent(t);
			}
		}
		else if (OP_MoveDangerLine == over->event_type) {
			mtx.lock();
			GameRoom[over->room_id].get_room().get_dangerLine().set_scale(10);
			mtx.unlock();
			Send_DangerLine_info(over->room_id, GameRoom[over->room_id].get_room().get_dangerLine().get_demage(), GameRoom[over->room_id].get_room().get_dangerLine().get_pos(), GameRoom[over->room_id].get_room().get_dangerLine().get_scale());

			if (GameRoom[over->room_id].get_room().get_dangerLine().get_now_scale_x() <= ci) {
				Timer_Event t = { over->room_id, GameRoom[over->room_id].get_room().get_dangerLine().get_time() , high_resolution_clock::now() + 1s, E_DangerLine };	// 자기장 시작 전 대기시간
				if (GameRoom[over->room_id].get_status() == inGameStatus)
					Timer.setTimerEvent(t);
			}
			else {
				Timer_Event t = { over->room_id, (int)ci , high_resolution_clock::now() + 100ms, E_MoveDangerLine };
				if (GameRoom[over->room_id].get_status() == inGameStatus)
					Timer.setTimerEvent(t);
			}

		}
		else if (OP_RemoveClient == over->event_type) {
			// 1초마다 한번씩 종료된 클라이언트를 지워 준다.
			Remove_Client(over->room_id);
		}
		else if (OP_DangerLineDamage == over->event_type) {
			// 자기장 데미지는 1초마다 한번씩 데미지를 준다.
			Attack_DangerLine_Damge(over->room_id);
			Timer_Event t = { over->room_id, 1, high_resolution_clock::now() + 1s, E_DangerLineDamage };
			Timer.setTimerEvent(t);
		}
		else if (OP_LobbyWait == over->event_type) {
			// 룸에 대기인원이 몇명인지 확인을 한다.
			if (GameRoom[over->room_id].get_room().check_ReadyClients() >= Minimum_Players && GameRoom[over->room_id].get_status() == LobbyStatus) {
				// 레디한 인원이 최소 인원 보다 많아졌을 경우.
				mtx.lock();
				GameRoom[over->room_id].set_status(ReadyStatus);
				mtx.unlock();
				// 게임 레디 단계로 넘어간다.
				Timer_Event t = { over->room_id, GamePlayWait, high_resolution_clock::now() + 1s, E_LobbyReday };
				Timer.setTimerEvent(t);
				t = { over->room_id, 1, high_resolution_clock::now() + 1s, E_LobbyWait };
				Timer.setTimerEvent(t);
			}
			else if (GameRoom[over->room_id].get_room().get_client().size() <= 0 && GameRoom[over->room_id].get_room().get_playGame() == true) {
				// 룸 안에 클라이언트가 한명도 없을경우 방을 다시 대기 모드로 변경한다.
				std::cout << "Room (" << over->room_id << ") init..!" << std::endl;
				mtx.lock();
				GameRoom[over->room_id].get_room().room_init();
				GameRoom[over->room_id].set_status(LobbyStatus);
				mtx.unlock();
				Timer_Event t = { over->room_id, 1, high_resolution_clock::now() + 1s, E_LobbyWait };
				Timer.setTimerEvent(t);
			}
			else {
				Timer_Event t = { over->room_id, 1, high_resolution_clock::now() + 1s, E_LobbyWait };
				Timer.setTimerEvent(t);
			}
		}
		else if (OP_LobbyReday == over->event_type) {
			// 로비에서 게임 진행까지 카운트 다운.
			Send_All_Time(over->room_id, SC_Lobby_Time, T_LobbyReday, (int)ci, noPlayer, true);
			if (ci <= 0) {
				// 카운트 다운이 모두 끝난 경우.
				mtx.lock();
				GameRoom[over->room_id].get_room().set_playGame(true);
				GameRoom[over->room_id].set_status(inGameStatus);
				mtx.unlock();
				std::cout << "Room : " << over->room_id << " Game Start..!" << std::endl;
				// 자기장 밖의 경우 데미지를 잃게 타이머 시작을 한다.
				Timer_Event t = { over->room_id, 1, high_resolution_clock::now() + 1s, E_DangerLineDamage };
				Timer.setTimerEvent(t);
				// 모든 플레이어들이 인게임에 들어와 있는지 타이머를 돌려 확인한다.
				t = { over->room_id, 1, high_resolution_clock::now() + 100ms, E_StartCarWait };
				Timer.setTimerEvent(t);
			}
			else {
				Timer_Event t = { over->room_id, (int)ci - 1, high_resolution_clock::now() + 1s, E_LobbyReday };
				Timer.setTimerEvent(t);
			}
		}
		else if (OP_StartCarWait == over->event_type) {
			// 모든 플레이어가 인게임에 들어왔는지 확인을 한다.
			Check_InGamePlayer(over->room_id);
		}
		else {
#if (DebugMod == TRUE )
			std::cout << "Unknown GQCS event!\n" << "Type : " << over->event_type << std::endl;
#endif
			//exit(-1);
		}

	}
}

void IOCP_Server::Accept_Thread()
{
	SOCKADDR_IN ClientAddr;
	ZeroMemory(&ClientAddr, sizeof(SOCKADDR_IN));
	ClientAddr.sin_family = AF_INET;
	ClientAddr.sin_port = htons(SERVERPORT);
	ClientAddr.sin_addr.s_addr = INADDR_ANY;
	int addr_size = sizeof(ClientAddr);
	while (true) {
		SOCKET new_client = WSAAccept(g_socket, reinterpret_cast<sockaddr *>(&ClientAddr), &addr_size, NULL, NULL);

		if (INVALID_SOCKET == new_client) {
			int err_no = WSAGetLastError();
			err_display((char *)"WSAAccept : ", err_no);
		}
		//---------------------------------------------------------------------------------------------------------------------------------------------------
		int new_id;
		// 스택에서 클라이언트 고유번호를 가져온다.
		new_id = remove_client_id.front();
		remove_client_id.pop();
#if (DebugMod == TRUE )
		std::cout << "New Client : " << new_id << std::endl;
#endif
		// 처음 시작시 대기방으로 들어가는 것 이므로, 0번 방 으로 보낸다.
		int room_id = 0;	//GameRoomEnter(new_id, new_client);
		GameRoom[room_id].get_room().get_client().insert(std::pair< int, Game_Client>(new_id, { new_client, new_id, (char *)"TheLastOne", room_id }));
		Send_Client_ID(room_id, new_id, SC_ID, false);		// 클라이언트에게 자신의 아이디를 보내준다.
		//---------------------------------------------------------------------------------------------------------------------------------------------------
		// map에 들어간 클라이언트를 찾는다.
		auto client = GameRoom[room_id].get_room().get_client_iter(new_id);
		//---------------------------------------------------------------------------------------------------------------------------------------------------

		DWORD recv_flag = 0;
		CreateIoCompletionPort(reinterpret_cast<HANDLE>(new_client), g_hiocp, new_id, 0);

		int ret = WSARecv(new_client, &client->second.recv_over.wsabuf, 1, NULL, &recv_flag, &client->second.recv_over.over, NULL);

		if (0 != ret) {
			int error_no = WSAGetLastError();
			if (WSA_IO_PENDING != error_no) {
				err_display((char*)"RecvPacket:WSARecv", error_no);
				//while ( true );
			}
		}

	}	// while(true)
}

void IOCP_Server::Remove_Client(const int room_id)
{

	if (GameRoom[room_id].get_room().get_client().size() > 0 && GameRoom[room_id].get_room().get_client().size() != 0) {
		// 1명 이상 있을 경우
		for (auto client_iter = GameRoom[room_id].get_room().get_client().begin(); client_iter != GameRoom[room_id].get_room().get_client().end();) {
			// 해당 방에서 Client를 시작부터 끝까지 확인 한다.
			if (client_iter->second.get_Connect() == false && client_iter->second.get_Remove() == true) {
				// Disconnect로 설정된 클라이언트만 찾는다.
#if (DebugMod == TRUE )
				std::cout << "Disconnect Client : " << client_iter->second.get_client_id() << std::endl;
#endif
				Send_Client_ID(client_iter->second.get_room_id(), client_iter->second.get_client_id(), SC_REMOVE_PLAYER, true);
				if (client_iter->second.get_hp() >= -100 && client_iter->second.get_hp() <= 100)
					closesocket(client_iter->second.get_Socket());
				mtx.lock();
				remove_client_id.push(client_iter->second.get_client_id());	// 종료된 클라이언트 번호를 스택에 넣어준다.
				client_iter = GameRoom[room_id].get_room().get_client().erase(client_iter++);		// Map 에서 해당 클라이언트를 삭제한다.
				mtx.unlock();
			}
			else {
				++client_iter;
			}
		}

	}


	Timer_Event t = { room_id, 1, high_resolution_clock::now() + 1s, E_Remove_Client };
	Timer.setTimerEvent(t);
}

void IOCP_Server::Shutdown_Server()
{
	closesocket(g_socket);
	CloseHandle(g_hiocp);
	WSACleanup();
}

void IOCP_Server::DisconnectClient(const int room_id, const int ci)
{
	// Disconnect시 Remove_Client에서 지울 수 있게 값을 변경 해준다.
	mtx.lock();
	GameRoom[room_id].get_room().get_client().find(ci)->second.set_client_Connect(false);
	GameRoom[room_id].get_room().get_client().find(ci)->second.set_client_Remove(true);
	mtx.unlock();
}

void IOCP_Server::ProcessPacket(const int room_id, const int ci, const int packet_size, const int packet_i, const char *packet)
{
	int errnum = 0;

	// 패킷 사이즈를 찾아주자.
	//char packet_data[8]{ 0 };
	//int packet_i = 0;
	//for (packet_i = 0; packet_i < 8; ++packet_i) {
	//	if (packet[packet_i] != 124)
	//		packet_data[packet_i] = packet[packet_i];
	//	else
	//		break;
	//}
	//int packet_size = atoi(packet_data) + 8;


	//char get_packet[MAX_PACKET_SIZE];
	char *get_packet = new char[packet_size] {0};
	bool all_Client_Packet = false;	// 모든 클라이언트에게 보낼 때는 True
	for (int i = 8; i < packet_size; ++i)
		get_packet[i - 8] = packet[i];
	/*
	Packet 사이즈를 확인한 후 받아야 한다.
	그렇지 않으면 Packet 사이즈 초과를 하여 확인을 할 수가 없다.
	*/

	auto client = GameRoom[room_id].get_room().get_client_iter(ci);

	switch (packet[packet_i + 1]) {
	case CS_Info:
	{
		auto client_View = GetClientView(get_packet);
		xyz packet_position{ client_View->position()->x() , client_View->position()->y() , client_View->position()->z() };
		xyz packet_rotation{ client_View->rotation()->x() , client_View->rotation()->y() , client_View->rotation()->z() };
		client->second.set_client_position(packet_position);
		client->second.set_client_rotation(packet_rotation);
		client->second.set_client_animator(client_View->animator());
		client->second.set_client_weapon(client_View->nowWeapon());
		client->second.set_horizontal(client_View->Horizontal());
		client->second.set_vertical(client_View->Vertical());
		client->second.set_inCar(client_View->inCar());
		client->second.nickName = client_View->name()->str();
		client->second.set_client_DangerLine(client_View->dangerLineIn());
		client->second.set_CostumNum(client_View->costumNum());
		//std::cout << "Costum Num : " << client->second.get_CostumNum() << std::endl;
		//std::cout << client->first << " | 자기장 : " << client_View->dangerLineIn() << std::endl;
		if (client_View->inCar() != -1) {
			// 차량을 실제로 탑승 하고 있을 경우.
			auto item = GameRoom[room_id].get_room().get_item_iter(client_View->inCar());
			item->second.set_Kmh(client_View->carkmh());	// 클라이언트에서 Carkmh를 받아오지만 클라이언트에 저장 안하고 아이템에 저장한다.
			item->second.set_pos(client_View->position()->x(), client_View->position()->y(), client_View->position()->z());
			xyz car_rotation{ client_View->carrotation()->x() , client_View->carrotation()->y() , client_View->carrotation()->z() };
			client->second.set_client_car_rotation(car_rotation);
			item->second.set_rotation(client_View->carrotation()->x(), client_View->carrotation()->y(), client_View->carrotation()->z());
		}
		//std::cout << client_View->position()->x() << ", " << client_View->position()->y() << ", " << client_View->position()->z() << std::endl;
	}
	break;
	case CS_Shot_info:
	{
		auto client_Shot_View = GetClient_packetView(get_packet);
		auto client = GameRoom[room_id].get_room().get_client_iter(client_Shot_View->id());
		Send_Client_Shot(room_id, client->first);
	}
	break;
	case CS_Check_info:
	{
		auto client_Check_info = GetClient_packetView(get_packet);
		if (ci != client_Check_info->id()) {
			/*
			클라이언트의 고유번호와 서버의 고유번호가 다를 경우
			값이 다르다 라는 결과를 표출하고 다시한번 클라이언트 아이디를 보내줘야 한다.
			현재 문제오류가 없어서 일단 임시 보류.
			*/
			std::cout << "클라이언트 의 값 : " << client_Check_info->id() << std::endl;
			std::cout << "실제 값 : " << ci << std::endl;
			Send_Client_ID(room_id, ci, SC_ID, false);
		}
	}
	break;
	case CS_Eat_Item:
	{
		auto client_Check_info = GetClient_packetView(get_packet);
		auto iter = GameRoom[room_id].get_room().get_item_iter(client_Check_info->id());
		mtx.lock();
		iter->second.set_eat(true);
		mtx.unlock();
	}
	break;
	case CS_Zombie_info:
	{
		auto client_Check_info = getZombie_CollectionView(get_packet);
		//std::cout << sizeof(get_packet) << std::endl;
		auto packet_zombie = client_Check_info->data();

		if (packet_zombie == NULL)
			break;

		for (unsigned int i = 0; i < packet_zombie->size(); ++i) {
			auto iter = GameRoom[room_id].get_room().get_zombie_iter(packet_zombie->Get(i)->id());
			iter->second.set_animator(packet_zombie->Get(i)->animator());
			if (packet_zombie->Get(i)->position()->x() <= 0.0f || packet_zombie->Get(i)->position()->y() <= 0.0f || packet_zombie->Get(i)->position()->z() <= 0.0f) {
				// 좀비 위치가 정상적이지 않을경우 동기화 하지 않는다.
				continue;
			}

			xyz packet_position{ packet_zombie->Get(i)->position()->x() , packet_zombie->Get(i)->position()->y() , packet_zombie->Get(i)->position()->z() };
			xyz packet_rotation{ packet_zombie->Get(i)->rotation()->x() , packet_zombie->Get(i)->rotation()->y() , packet_zombie->Get(i)->rotation()->z() };

			mtx.lock();
			iter->second.set_zombie_position(packet_position);
			iter->second.set_zombie_rotation(packet_rotation);
			mtx.unlock();

			if (iter->second.get_target() == -1 && packet_zombie->Get(i)->targetPlayer() != -1) {
				// 타겟이 없었는데 클라이언트에서 타겟을 넣어줬다.
				mtx.lock();
				iter->second.set_target(packet_zombie->Get(i)->targetPlayer());
				mtx.unlock();
				//std::cout << "타겟 설정 : " << iter->second.get_target() << std::endl;
			}
			else if (iter->second.get_target() != -1 && packet_zombie->Get(i)->targetPlayer() == -1) {
				// 타겟이 있었는데 클라이언트에서 타겟을 초기화 해줬다.
				mtx.lock();
				iter->second.set_target(packet_zombie->Get(i)->targetPlayer());
				mtx.unlock();
				//std::cout << "타겟 초기화 : " << iter->second.get_target() << std::endl;
			}


		}
	}
	break;
	case CS_Object_HP:
	{
		auto client_Check_info = getGame_HP_SetView(get_packet);
		if (client_Check_info->kind() == Kind_Player) {
			auto iter = GameRoom[room_id].get_room().get_client_iter(client_Check_info->id());
			mtx.lock();
			iter->second.set_hp(client_Check_info->hp());
			iter->second.set_armour(client_Check_info->armour());
			mtx.unlock();
			if (iter->second.get_hp() <= 0) {
				// 체력이 0일경우 죽을을 허용한다.
				Send_KillLog(room_id, client_Check_info->shotNick()->str(), iter->second.nickName);
				iter->second.set_clientDie(true);
			}
		}
		else if (client_Check_info->kind() == Kind_Zombie) {
			auto iter = GameRoom[room_id].get_room().get_zombie_iter(client_Check_info->id());
			mtx.lock();
			iter->second.set_hp(client_Check_info->hp());
			mtx.unlock();
		}
		else if (client_Check_info->kind() == Kind_Car) {
			auto iter = GameRoom[room_id].get_room().get_item_iter(client_Check_info->id());
			mtx.lock();
			iter->second.set_hp(client_Check_info->hp());
			mtx.unlock();
			if (iter->second.get_hp() <= 0) {
				// 차량 체력이 0일경우 폭파를 허용한다.
				iter->second.set_exp(true);
			}
		}
	}
	break;
	case CS_Car_Riding:
	{
		auto client_Check_info = GetClient_packetView(get_packet);
		auto iter = GameRoom[room_id].get_room().get_item_iter(client_Check_info->id());
		mtx.lock();
		iter->second.set_riding(true);
		mtx.unlock();
	}
	break;
	case CS_Car_Rode:
	{
		auto client_Check_info = GetClient_packetView(get_packet);
		auto iter = GameRoom[room_id].get_room().get_item_iter(client_Check_info->id());
		mtx.lock();
		iter->second.set_riding(false);
		mtx.unlock();
	}
	break;
	case CS_Player_Status:
	{
		auto client_Check_info = getClient_Status(get_packet);
		mtx.lock();
		client->second.set_playerStatus(client_Check_info->status());
		mtx.unlock();
		if (client_Check_info->mapType() != -1) {
			// mapType가 -1이 아닐경우 : 플레이어가 레디를 하여 방을 찾는 상황이다.
			GameRoomEnter(ci, client_Check_info->mapType(), client->second.get_Socket());
		}
	}
	break;
	}

	if (GameRoom[room_id].get_status() == inGameStatus) {
		// 인게임에 들어가기 전까지는 패킷 전송을 하지 않는다.
		Send_All_Player(room_id, ci);
		Send_Hide_Player(room_id, ci);
		if (GameRoom[room_id].get_room().get_dangerLine().get_level() <= 4) {
			Send_All_Zombie(room_id, ci);
			Send_Hide_Zombie(room_id, ci);

			GameRoom[room_id].get_room().player_To_Zombie();
		}
		Send_All_Item(room_id, ci);

		Send_SurvivalCount(room_id, ci);
	}
}

void IOCP_Server::SendPacket(const int type, const int room_id, const int ci, const void *packet, const int psize)
{
	auto client = GameRoom[room_id].get_room().get_client_iter(ci);

	if (client != GameRoom[room_id].get_room().get_client().end()) {
		// client가 map 안에 정상적으로 있을 경우에만 작업을 한다.
		if (client->second.get_Socket() != NULL) {
			//int ptype = reinterpret_cast<unsigned char *>(packet)[1];
			OverlappedEx *over = new OverlappedEx;
			ZeroMemory(&over->over, sizeof(over->over));
			over->room_id = room_id;
			over->event_type = OP_SEND;
			char p_size[MAX_PACKET_SIZE]{ 0 };

			// 클라이언트에게 패킷 전송시 <패킷크기 | 패킷 타입> 으로 전송을 한다.
			itoa(psize + 8, p_size, 10);
			int buf_len = (int)strlen(p_size);
			p_size[buf_len] = '|';
			p_size[buf_len + 1] = int(type);

			// 패킷 사이즈를 미리 합쳐서 보내줘야한다.
			memcpy(over->IOCP_buf, packet, psize);

			for (int i = 8; i < psize + 8; ++i) {
				p_size[i] = over->IOCP_buf[i - 8];
			}

			over->wsabuf.buf = reinterpret_cast<CHAR *>(p_size);
			over->wsabuf.len = psize + 8;
			int res = WSASend(client->second.get_Socket(), &over->wsabuf, 1, NULL, 0, &over->over, NULL);
			if (0 != res) {
				int error_no = WSAGetLastError();
				if (WSA_IO_PENDING != error_no) {
					//err_display((char *)"SendPacket:WSASend", error_no);
					DisconnectClient(room_id, ci);
				}
			}
		}
	}
}

void IOCP_Server::Send_Client_ID(const int room_id, const int client_id, const int value, const bool allClient)
{
	try {
		// value = 아이디를 추가 or 삭제 할때 사용.
		flatbuffers::FlatBufferBuilder builder;
		auto Client_id = client_id;
		auto orc = CreateClient_Packet(builder, Client_id);
		builder.Finish(orc); // Serialize the root of the object.

		if (allClient == true) {
			// 모든 클라이언트 에게 나갔다는 것을 보내준다..!
			for (auto iter : GameRoom[room_id].get_room().get_client()) {
				if (iter.second.get_Connect() != true)
					continue;
				SendPacket(value, room_id, iter.second.get_client_id(), builder.GetBufferPointer(), builder.GetSize());
			}
		}
		else {
			SendPacket(value, room_id, client_id, builder.GetBufferPointer(), builder.GetSize());
		}
	}
	catch (int exceptionCode) {
		std::cout << "Error : " << exceptionCode << std::endl;
	}
}

void IOCP_Server::Send_All_Player(const int room_id, const int client)
{
	auto client_iter = GameRoom[room_id].get_room().get_client_iter(client);
	flatbuffers::FlatBufferBuilder builder;
	std::vector<flatbuffers::Offset<Client_info>> Individual_client;		// 개인 데이터
	for (auto iter : GameRoom[room_id].get_room().get_client()) {
		if (iter.second.get_Connect() != true || Distance(room_id, client_iter->second.get_client_id(), iter.second.get_client_id(), Player_Dist, Kind_Player) == false)
			// 클라이언트가 연결 안되어 있으면 제외 한다. 또는 체력이 없을경우 제외한다.
			continue;

		auto id = iter.second.get_client_id();
		auto name = builder.CreateString(iter.second.nickName);
		auto hp = iter.second.get_hp();
		auto armour = iter.second.get_armour();
		auto animator = iter.second.get_animator();
		float horizontal = iter.second.get_horizontal();
		float vertical = iter.second.get_vertical();
		auto position = Vec3(iter.second.get_position().x, iter.second.get_position().y, iter.second.get_position().z);
		auto rotation = Vec3(iter.second.get_rotation().x, iter.second.get_rotation().y, iter.second.get_rotation().z);
		auto weaponState = iter.second.get_weapon();
		auto inCar = iter.second.get_inCar();
		auto dangerLineIn = iter.second.get_DangerLine();
		auto car_rotation = Vec3(iter.second.get_car_rotation().x, iter.second.get_car_rotation().y, iter.second.get_car_rotation().z);
		auto playerDie = iter.second.get_clientdie();
		auto CostumNum = iter.second.get_CostumNum();
		//std::cout << CostumNum << std::endl;
		auto client_data = CreateClient_info(builder, id, hp, armour, animator, horizontal, vertical, inCar, name, &position, &rotation, &car_rotation, 0.0f, dangerLineIn, weaponState, playerDie, CostumNum);
		// client_data 라는 테이블에 클라이언트 데이터가 들어가 있다.

		Individual_client.push_back(client_data);	// Vector에 넣었다.
		// Individual_client 라는 전체 테이블에 client_data를 넣어주자.
	}

	auto Full_client_data = builder.CreateVector(Individual_client);		// 이제 Vector로 묶어서 전송할 데이터로 만들어주자.
	auto orc = CreateClient_Collection(builder, Full_client_data);		// 실제로 보내는 테이블 명은 Client_Data
	builder.Finish(orc); // Serialize the root of the object.

	SendPacket(SC_Client_Data, room_id, client, builder.GetBufferPointer(), builder.GetSize());
}

void IOCP_Server::Send_All_Zombie(const int room_id, const int client)
{
	auto client_iter = GameRoom[room_id].get_room().get_client_iter(client);

	flatbuffers::FlatBufferBuilder builder;
	std::vector<flatbuffers::Offset<Zombie_info>> Individual_zombie;		// 개인 데이터

	for (auto zombie : GameRoom[room_id].get_room().get_zombie()) {
		if (Distance(room_id, client_iter->second.get_client_id(), zombie.second.get_client_id(), Zombie_Dist, Kind_Zombie) == false ||
			zombie.second.get_hp() <= 0)
			// 클라이언트가 연결 안되어 있으면 제외 한다.
			continue;

		auto z_id = zombie.first;
		auto z_hp = zombie.second.get_hp();
		auto z_ani = zombie.second.get_animator();
		auto z_target = zombie.second.get_target();
		auto z_pos = Vec3(zombie.second.get_position().x, zombie.second.get_position().y, zombie.second.get_position().z);
		auto z_rot = Vec3(zombie.second.get_rotation().x, zombie.second.get_rotation().y, zombie.second.get_rotation().z);
		auto zombie_data = CreateZombie_info(builder, z_id, z_hp, z_ani, z_target, &z_pos, &z_rot);

		Individual_zombie.push_back(zombie_data);
	}

	auto All_zombie_data = builder.CreateVector(Individual_zombie);		// 이제 Vector로 묶어서 전송할 데이터로 만들어주자.
	auto orc = CreateZombie_Collection(builder, All_zombie_data);		// 실제로 보내는 테이블 명은 Client_Data
	builder.Finish(orc); // Serialize the root of the object.

	SendPacket(SC_Zombie_Info, room_id, client, builder.GetBufferPointer(), builder.GetSize());
}

void IOCP_Server::Send_All_Time(const int room_id, const int type, const int kind, const int time, const int client_id, const bool allClient)
{
	flatbuffers::FlatBufferBuilder builder;
	auto f_kind = kind;
	auto f_time = time;
	auto orc = CreateGame_Timer(builder, f_kind, f_time);
	builder.Finish(orc); // Serialize the root of the object.

	if (allClient == true) {
		// 모든 클라이언트 에게 나갔다는 것을 보내준다..!
		for (auto iter : GameRoom[room_id].get_room().get_client()) {
			if (iter.second.get_Connect() != true)
				continue;
			SendPacket(type, room_id, iter.second.get_client_id(), builder.GetBufferPointer(), builder.GetSize());
		}
	}
	else {
		SendPacket(type, room_id, client_id, builder.GetBufferPointer(), builder.GetSize());
	}
}

void IOCP_Server::Send_All_Item(const int room_id, const int ci)
{
	flatbuffers::FlatBufferBuilder builder;
	std::vector<flatbuffers::Offset<Gameitem>> Individual_client;		// 개인 데이터

	for (auto iter : GameRoom[room_id].get_room().get_item()) {
		//if (iter.second.get_eat() == true)	// 아이템을 이미 먹은경우 패스 한다.
		//	continue;
		if (Distance(room_id, ci, iter.first, Item_Dist, Kind_Item) == false &&
			(iter.second.get_name() != "UAZ" || iter.second.get_name() != "JEEP"))
			continue;

		auto id = iter.first;
		auto name = builder.CreateString(iter.second.get_name());
		Vec3 pos = { iter.second.get_position().x, iter.second.get_position().y,  iter.second.get_position().z };
		Vec3 rotation = { iter.second.get_rotation().x, iter.second.get_rotation().y,  iter.second.get_rotation().z };
		auto eat = iter.second.get_eat();
		auto riding = iter.second.get_riding();
		auto hp = iter.second.get_hp();
		auto kind = iter.second.get_kind();
		auto kmh = iter.second.get_Kmh();
		auto explosion = iter.second.get_exp();
		auto client_data = CreateGameitem(builder, id, name, &pos, &rotation, eat, riding, hp, kind, kmh, explosion);

		Individual_client.push_back(client_data);	// Vector에 넣었다.
	}

	auto Full_client_data = builder.CreateVector(Individual_client);		// 이제 Vector로 묶어서 전송할 데이터로 만들어주자.
	auto orc = CreateGame_Items(builder, Full_client_data);		// 실제로 보내는 테이블 명은 Client_Data
	builder.Finish(orc); // Serialize the root of the object.

	for (auto iter : GameRoom[room_id].get_room().get_client()) {
		if (iter.second.get_Connect() != true)
			continue;
		SendPacket(SC_Server_Item, room_id, iter.second.get_client_id(), builder.GetBufferPointer(), builder.GetSize());
	}

}

void IOCP_Server::Send_Client_Shot(const int room_id, const int shot_client)
{
	auto client_iter = GameRoom[room_id].get_room().get_client_iter(shot_client);
	flatbuffers::FlatBufferBuilder builder;
	auto id = shot_client;
	auto orc = CreateClient_Packet(builder, id);
	builder.Finish(orc); // Serialize the root of the object.

	for (auto iter : GameRoom[room_id].get_room().get_client()) {
		if (iter.second.get_Connect() != true || Distance(room_id, client_iter->second.get_client_id(), iter.second.get_client_id(), Player_Dist, Kind_Player) == false)
			continue;
		SendPacket(SC_Shot_Client, room_id, iter.second.get_client_id(), builder.GetBufferPointer(), builder.GetSize());
	}

}

void IOCP_Server::Send_DangerLine_info(const int room_id, const int demage, const xyz pos, const xyz scale)
{
	flatbuffers::FlatBufferBuilder builder;
	auto dem = demage;
	auto position = new Vec3(pos.x, pos.y, pos.z);
	auto sscale = new Vec3(scale.x, scale.y, scale.z);
	auto orc = CreateGameDangerLine(builder, dem, position, sscale);
	builder.Finish(orc); // Serialize the root of the object.

	if (GameRoom[room_id].get_status() == inGameStatus) {
		for (auto iter : GameRoom[room_id].get_room().get_client()) {
			if (iter.second.get_Connect() != true)
				continue;
			SendPacket(SC_DangerLine, room_id, iter.second.get_client_id(), builder.GetBufferPointer(), builder.GetSize());
		}
	}
}

void IOCP_Server::Send_Hide_Player(const int room_id, const int client)
{
	auto client_iter = GameRoom[room_id].get_room().get_client_iter(client);

	for (auto iter : GameRoom[room_id].get_room().get_client()) {
		if (iter.second.get_Connect() != true)
			continue;
		if (Distance(room_id, client_iter->second.get_client_id(), iter.second.get_client_id(), Player_Dist, Kind_Player) == true)
			continue;		// 범위내에 있을경우 넘긴다.

		flatbuffers::FlatBufferBuilder builder;
		auto Client_id = iter.second.get_client_id();
		auto orc = CreateClient_Packet(builder, Client_id);
		builder.Finish(orc); // Serialize the root of the object.

		SendPacket(SC_REMOVE_PLAYER, room_id, client_iter->second.get_client_id(), builder.GetBufferPointer(), builder.GetSize());
	}
}

void IOCP_Server::Send_Hide_Zombie(const int room_id, const int client)
{
	auto client_iter = GameRoom[room_id].get_room().get_client_iter(client);

	for (auto iter : GameRoom[room_id].get_room().get_zombie()) {

		//std::cout << Distance(room_id, client_iter->second.get_client_id(), iter.second.get_client_id(), Zombie_Dist, Kind_Zombie) << " ";
		if (Distance(room_id, client_iter->second.get_client_id(), iter.second.get_client_id(), Zombie_Dist, Kind_Zombie) == true)
			continue;		// 범위내에 있을경우 넘긴다.

		flatbuffers::FlatBufferBuilder builder;
		auto Client_id = iter.second.get_client_id();
		auto orc = CreateClient_Packet(builder, Client_id);
		builder.Finish(orc); // Serialize the root of the object.

		SendPacket(SC_Remove_Zombie, room_id, client_iter->second.get_client_id(), builder.GetBufferPointer(), builder.GetSize());
	}
}

bool IOCP_Server::Distance(const int room_id, const int me, const int you, const int Radius, const int kind)
{
	auto iter_me = GameRoom[room_id].get_room().get_client().find(me);
	auto iter_you = GameRoom[room_id].get_room().get_client().find(you);
	auto iter_zombie = GameRoom[room_id].get_room().get_zombie().find(you);
	auto iter_item = GameRoom[room_id].get_room().get_item().find(you);

	if (kind == Kind_Player)
		return (iter_me->second.get_position().x - iter_you->second.get_position().x) * (iter_me->second.get_position().x - iter_you->second.get_position().x) +
		(iter_me->second.get_position().z - iter_you->second.get_position().z) * (iter_me->second.get_position().z - iter_you->second.get_position().z) <= Radius * Radius;
	else if (kind == Kind_Zombie)
		return (iter_me->second.get_position().x - iter_zombie->second.get_position().x) * (iter_me->second.get_position().x - iter_zombie->second.get_position().x) +
		(iter_me->second.get_position().z - iter_zombie->second.get_position().z) * (iter_me->second.get_position().z - iter_zombie->second.get_position().z) <= Radius * Radius;
	else if (kind == Kind_Item)
		return (iter_me->second.get_position().x - iter_item->second.get_position().x) * (iter_me->second.get_position().x - iter_item->second.get_position().x) +
		(iter_me->second.get_position().z - iter_item->second.get_position().z) * (iter_me->second.get_position().z - iter_item->second.get_position().z) <= Radius * Radius;

	return false;
}

void IOCP_Server::Check_InGamePlayer(const int room_id)
{
	int ingamePlayer = 0;

	for (auto player : GameRoom[room_id].get_room().get_client()) {
		if (player.second.get_playerStatus() == playGameStatus) {
			ingamePlayer++;
		}
	}

	if (GameRoom[room_id].get_room().get_client().size() == ingamePlayer) {
		std::cout << "Room : " << room_id << " In Game Start..!" << std::endl;
		// 모두 인게임 상태일 경우 패킷을 전송해 준다.
		flatbuffers::FlatBufferBuilder builder;
		auto id = 616;		// 패킷 값만 보내면 되므로 아무런 숫자나 입력 해준다.
		auto orc = CreateClient_Packet(builder, id);
		builder.Finish(orc); // Serialize the root of the object.

		for (int i = 0; i < 3; ++i) {
			// 혹시 패킷 오류로 못받을 수 있으니 한번 더 보내준다.
			for (auto iter : GameRoom[room_id].get_room().get_client()) {
				if (iter.second.get_Connect() != true)
					continue;
				SendPacket(SC_StartCar_Play, room_id, iter.second.get_client_id(), builder.GetBufferPointer(), builder.GetSize());
			}
		}
		// 게임이 시작 되었으므로 자기장 타이머도 시작이 된다.
		Timer_Event t = { room_id, DangerLine_init, high_resolution_clock::now() + 1s, E_DangerLine };
		Timer.setTimerEvent(t);
	}
	else {
		Timer_Event t = { room_id, 1, high_resolution_clock::now() + 100ms, E_StartCarWait };
		Timer.setTimerEvent(t);
	}
}

int IOCP_Server::GameRoomEnter(const int client, const int mapType, const SOCKET sock)
{
	// 방중에서 현재 시작이 안된 방을 찾는다.
searchRoom:
	int room_id = -1;

	for (auto iter : GameRoom) {
		if (iter.get_id() == 0) continue;	// 0번은 대기방 이므로 넘어간다.
		if ((iter.get_status() == LobbyStatus || iter.get_status() == ReadyStatus) && iter.get_room().get_client().size() <= MAX_Client && iter.get_room().get_mapType() == mapType) {
			// room_id 부여
			room_id = iter.get_id();
#if (DebugMod == TRUE )
			std::cout << "Connect to Room : " << iter.get_id() << std::endl;
#endif
			mtx.lock();
			// 접속된 클라이언트 데이터를 넣어 준다.
			GameRoom[room_id].get_room().get_client().insert(std::pair< int, Game_Client>(client, { sock, client, (char *)"TheLastOne", room_id }));
			//Room을 옮겨준 이후 Ready 상태로 변경해 준다.
			GameRoom[room_id].get_room().get_client_iter(client)->second.set_playerStatus(ReadyStatus);
			mtx.unlock();

			//---------------------------------------------------------------------------------------
			// 새로 입장한 Room에 Recv를 설정 해준다.
			auto client_data = GameRoom[room_id].get_room().get_client_iter((int)client);
			mtx.lock();
			client_data->second.recv_over.room_id = room_id;
			client_data->second.recv_over.target_id = (int)client;
			mtx.unlock();

			DWORD recv_flag = 0;
			WSARecv(client_data->second.get_Socket(), &client_data->second.recv_over.wsabuf, 1, NULL, &recv_flag, &client_data->second.recv_over.over, NULL);
			//---------------------------------------------------------------------------------------
			// 기존 Overlapped 대기방이 변경이 되었다고 수정을 한다.
			mtx.lock();
			GameRoom[0].get_room().get_client().find(client)->second.set_room_id(room_id);
			GameRoom[0].get_room().get_client_iter((int)client)->second.recv_over.room_change = true;
			mtx.unlock();
			// 대기방 번호를 리턴한다.
			return room_id;
			break;
		}
	}
	if (room_id == -1) {
		// 빈 방이 없을 경우 게임방 + 맵을 설정하여 만들어 준다.
		create_GameRoom(mapType);
		goto searchRoom;
	}
	else {
		return room_id;
	}
}

void IOCP_Server::Send_SurvivalCount(const int room_id, const int client)
{
	int count = 0;
	for (auto iter : GameRoom[room_id].get_room().get_client()) {
		if (iter.second.get_hp() > 0)
			count++;
	}
	//count++;
	flatbuffers::FlatBufferBuilder builder;
	auto Client_id = count;	// 임시로 올려놓자.
	auto orc = CreateClient_Packet(builder, Client_id);
	builder.Finish(orc); // Serialize the root of the object.

	SendPacket(SC_Survival_Count, room_id, client, builder.GetBufferPointer(), builder.GetSize());
}

bool IOCP_Server::findRoomCi(const int room_id, const int ci)
{
	auto client = GameRoom[room_id].get_room().get_client_iter((int)ci);

	if (client == GameRoom[room_id].get_room().get_client().end()) {
		// 클라이언트가 존재 하지 않는다.
		return false;
	}
	else {
		// 클라이언트가 존재 한다.
		return true;
	}
}

void IOCP_Server::create_GameRoom(const int mapType)
{
	int createroom_id = (int)(GameRoom.size());
	if (createroom_id >= GameRoomLimit) {
		std::cout << "You can not make a room anymore.." << std::endl;
	}
	else {
		std::cout << "Create to Room : " << createroom_id << " (" << mapType << ")" << std::endl;
		mtx.lock();
		GameRoom.emplace_back(Room_Manager(createroom_id, mapType));
		mtx.unlock();
		// 대기방이 아닐 경우에만 타이머를 추가해준다.
		if (mapType != -1) {
			// 클라이언트 종료시 삭제 처리를 위한 각 방에대한 타이머 이벤트 추가.
			Timer_Event t = { createroom_id, 1, high_resolution_clock::now() + 1s, E_Remove_Client };
			Timer.setTimerEvent(t);
			// 클라이언트 로비 대기 타이머 이벤트 추가.
			t = { createroom_id, 1, high_resolution_clock::now() + 1s, E_LobbyWait };
			Timer.setTimerEvent(t);
		}
	}
}

void IOCP_Server::Send_KillLog(const int room_id, const std::string killNick, const std::string dieNick)
{
	flatbuffers::FlatBufferBuilder builder;
	auto leftNick = builder.CreateString(killNick);
	auto rightNick = builder.CreateString(dieNick);
	auto orc = CreateKillLog_info(builder, leftNick, rightNick);
	builder.Finish(orc); // Serialize the root of the object.

	for (auto iter : GameRoom[room_id].get_room().get_client()) {
		if (iter.second.get_Connect() != true)
			continue;
		SendPacket(SC_KillLog, room_id, iter.second.get_client_id(), builder.GetBufferPointer(), builder.GetSize());
	}
}

void IOCP_Server::Attack_DangerLine_Damge(const int room_id)
{
	for (auto iter : GameRoom[room_id].get_room().get_client()) {
		if (iter.second.get_Connect() != true)
			continue;
		if (iter.second.get_DangerLine() == false) {
			// 자기장 밖에 있을 경우.
			auto client_iter = GameRoom[room_id].get_room().get_client_iter(iter.first);
			int my_hp = client_iter->second.get_hp();
			mtx.lock();
			client_iter->second.set_hp(my_hp - GameRoom[room_id].get_room().get_dangerLine().get_demage());
			mtx.unlock();
			if (client_iter->second.get_hp() <= 0) {
				client_iter->second.set_hp(0);
			}
		}
	}

}