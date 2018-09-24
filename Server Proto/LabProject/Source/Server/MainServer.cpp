#include "stdafx.h"
#include "MainServer.h"

using namespace std;

void err_quit(char *msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	MessageBoxA(NULL, (LPCSTR)lpMsgBuf, msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(1);
}

void err_display(char *msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	printf("[%s] %s", msg, (char *)lpMsgBuf);
	LocalFree(lpMsgBuf);
}

void RoomInfo::BuildRoom()
{
	MaxClientCount = ClientIDMatcher.size();
	GameFramework.RegisterPxFoundation(m_pFoundation);
	GameFramework.OnCreate();

	cout << "Complete Room " << RoomID << " Init\n" << endl;
}

void RoomInfo::StartRoom()
{
	if (Status == RoomState::Waiting) {
		BuildRoom();
		Status = RoomState::Running;

		PK_ROOM_START pk;
		pk.size = sizeof(PK_ROOM_START);
		pk.type = MSGTYPE::MSGROOM::STARTROOM;
		pk.RoomID = RoomID;
		SendPacket(&pk);

		pMainServer->AddEvent(RoomID, op_UpdateScene, UPDATETIME_ms);
		pMainServer->AddEvent(RoomID, op_UpdateSend, BROADCASTTIME_ms);
		cout << "Room " << RoomID << " Start\n";
	}
}

void RoomInfo::AcceptClient(ClientInfo * client)
{
	cout << "Client " << client->ID << " Enter Room " << RoomID << "\n";

	// New Client Info to All
	PK_ROOM_INFO p;
	p.size = sizeof(PK_ROOM_INFO);
	p.type = MSGTYPE::MSGROOM::ROOM_CLIENTINFO;
	p.RoomID = RoomID;
	p.ClientID = client->ID;
	p.ClientType = client->PlayerType;
	SendPacket(&p);

	// Order Create New Client objects (to all clients)
	PK_MSG_ADD_PLAYER packet;
	packet.size = sizeof(PK_MSG_ADD_PLAYER);
	packet.type = MSGTYPE::MSGUPDATE::CREATE_OBJECT;
	packet.id = client->ID;
	packet.HP = MAX_HP;
	packet.ObjType = client->PlayerType;
	SendPacket(&packet);


	// Accept Msg to New Client
	p.type = MSGTYPE::MSGROOM::ACCEPTROOM;
	pMainServer->SendPacket(client->ID, &p);

	// Room Info to New Client
	for (auto clientcontainer : ClientIDMatcher) {
		auto cl = clientcontainer.second;
		p.type = MSGTYPE::MSGROOM::ROOM_CLIENTINFO;
		p.ClientID = cl->ID;
		p.ClientType = cl->PlayerType;
		pMainServer->SendPacket(client->ID, &p);

		// Order Create New Client objects (to new client)
		packet.id = cl->ID;
		packet.ObjType = cl->PlayerType;
		packet.HP = MAX_HP;
		pMainServer->SendPacket(client->ID, &packet);
	}

	client->RoomID = RoomID;
	ClientIDMatcher.insert(pair<UINT, ClientInfo*>(
		client->ID, client
		));
	AliveClientCounter++;
	
	cout << "Room : " << RoomID << " Clientlist : ";
	for (auto cl : ClientIDMatcher) cout << cl.second->ID << " ";
	cout << "END\n";
}

void RoomInfo::LeaveRoom(ClientInfo * client)
{
	PK_ROOM_INFO p;
	p.size = sizeof(PK_ROOM_INFO);
	p.type = MSGTYPE::MSGROOM::DISCONNECTROOM;
	p.RoomID = RoomID;
	p.ClientID = client->ID;
	SendPacket(&p);

	AliveClientCounter--;
	client->RoomID = -1;

	cout << AliveClientCounter << endl;

	cout << "Client " << client->ID << " Leave Room " << RoomID << "\n";
}

void RoomInfo::SendCloseRoom()
{
	PK_ROOM_END p{};
	p.RoomID = RoomID;
	p.size = sizeof(PK_ROOM_END);
	p.type = MSGTYPE::MSGROOM::CLOSEROOM;

	SendPacket(&p);
}

void RoomInfo::SendPacket(void * packet)
{
	char* p = reinterpret_cast<char*>(packet);

	for (auto c : ClientIDMatcher) {
		OverlappedEx* o = new OverlappedEx();
		memcpy(o->io_buf, packet, p[0]);
		o->op = op_Send;
		o->wsabuf.buf = o->io_buf;
		o->wsabuf.len = p[0];
		ZeroMemory(&o->wsaOver, sizeof(WSAOVERLAPPED));

		WSASend(c.second->sock, &o->wsabuf, 1, NULL, 0, &o->wsaOver, NULL);
	}
}

void RoomInfo::SendPacket(const int id, void * packet)
{
	if (ClientIDMatcher.count(id) == 0) return;

	OverlappedEx* o = new OverlappedEx();
	char* p = reinterpret_cast<char*>(packet);
	memcpy(o->io_buf, packet, p[0]);
	o->op = op_Send;
	o->wsabuf.buf = o->io_buf;
	o->wsabuf.len = p[0];
	ZeroMemory(&o->wsaOver, sizeof(WSAOVERLAPPED));

	WSASend(ClientIDMatcher[id]->sock, &o->wsabuf, 1, NULL, 0, &o->wsaOver, NULL);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void MainServer::WorkerThreadfunc(MainServer* server)
{
	while (true)
	{
		unsigned long datasize;
		unsigned long long key;
		WSAOVERLAPPED* pOverlapped;

		BOOL isSuccess = GetQueuedCompletionStatus(server->m_hIOCP, &datasize, &key, &pOverlapped, INFINITE);
		OverlappedEx* o = reinterpret_cast<OverlappedEx*>(pOverlapped);

		if (0 == isSuccess)
		{
			server->RemoveClientFromServer(key);
			continue;
		}
		else if (0 == datasize && (o->op == op_Send || o->op == op_Recv))
		{
			server->RemoveClientFromServer(key);
			continue;
		}

		// Send, Recv
		if (o->op == op_Recv)
		{
			ClientInfo* client = server->m_ClientFinder[key];
			int Received_size = datasize;
			char* ptr = o->io_buf;

			while (0 < Received_size)
			{
				if (client->packetsize == 0)
					client->packetsize = ptr[0];

				int remainsize = client->packetsize - client->prev_packetsize;

				if (remainsize <= Received_size) {
					memcpy(client->prev_packet + client->prev_packetsize, ptr, remainsize);

					MSGWRAPPER msg = new char[client->packetsize];
					memcpy(msg, client->prev_packet, client->packetsize);
					server->ProcessMSG(key, msg);
					delete msg;

					Received_size -= remainsize;
					ptr += remainsize;
					client->packetsize = 0;
					client->prev_packetsize = 0;
					
					//if (remainsize == 0) break;
				}
				else {
					memcpy(client->prev_packet + client->prev_packetsize, ptr, Received_size);
					client->prev_packetsize += Received_size;
				}

			}
			unsigned long rFlag = 0;
			ZeroMemory(&o->wsaOver, sizeof(OVERLAPPED));
			WSARecv(client->sock, &o->wsabuf, 1, NULL, &rFlag, &o->wsaOver, NULL);
		}

		else if (o->op == op_UpdateScene)
		{
			// key == Roomid
			if (server->m_RoomFinder[(UINT)key]->Status == RoomState::Finished) 
			{
				cout << "Room " << (UINT)key << " Game Finished. End Game routine.\n" << "Terminte Room\n";
				server->AddEvent(key, op_TerminateRoom, TERMINATETIME_ms);
			}
			else if (server->m_RoomFinder[(UINT)key]->AliveClientCounter > 0) 
			{
				server->m_RoomFinder[(UINT)key]->GameFramework.GetScene()->AnimateObjects(UPDATETIME_s);
				server->AddEvent(key, op_UpdateScene, UPDATETIME_ms);
			}
			else
				server->AddEvent(key, op_TerminateRoom, TERMINATETIME_ms);

			delete o;
		}

		else if (o->op == op_UpdateSend)
		{
			if (server->m_RoomFinder.count((UINT)key) &&
				server->m_RoomFinder[(UINT)key]->AliveClientCounter > 0) 
			{
				server->m_RoomFinder[(UINT)key]->GameFramework.GetScene()->SendMsgs(MSGTYPE::MSGUPDATE::UPDATE_OBJECT);
				server->AddEvent(key, op_UpdateSend, BROADCASTTIME_ms);
			}
			else
				server->AddEvent(key, op_TerminateRoom, TERMINATETIME_ms);
			delete o;
		}

		else if (o->op == op_TerminateRoom) {
			if (server->m_RoomFinder.count((UINT)key) != 0) 
			{
				auto room = server->m_RoomFinder[(UINT)key];
				if (room->Status == RoomState::Running) 
				{
					room->Status = RoomState::Terminated;

					delete server->m_RoomFinder[(UINT)key];
					server->m_RoomFinder.erase((UINT)key);

					cout << "Room " << key << " Closed\n";
				}
				else if (room->Status == RoomState::Finished) 
				{
					room->SendCloseRoom();
					for (auto clientcontainer : room->ClientIDMatcher) 
					{
						auto client = clientcontainer.second;
						room->LeaveRoom(client);
						server->SendRoomlist(client->IOCPKey, room->RoomID);
					}

					room->ClientIDMatcher.clear();

					room->Status = RoomState::Terminated;
					delete server->m_RoomFinder[(UINT)key];
					server->m_RoomFinder.erase((UINT)key);

					cout << "Room " << key << " Closed\n";
				}
			}
			delete o;
		}
		else
		{
			delete o;
		}
	}
}

void MainServer::ProcessMSG(UINT key, const MSGWRAPPER & msg)
{
	switch (((char*)msg)[1])
	{
	case MSGTYPE::MSGACTION::ATTACK:
	case MSGTYPE::MSGACTION::MOVE:
	case MSGTYPE::MSGACTION::ANIMATE:
	case MSGTYPE::CHEAT::RESTART:
	{
		UINT roomid = m_ClientFinder[key]->RoomID;
		if (roomid != INVALID_VALUE && m_RoomFinder[roomid]->Status == RoomState::Running) {
			m_RoomFinder[roomid]->GameFramework.GetScene()->ProcessMsg(msg);
		}
		break;
	}

	case MSGTYPE::MSGROOM::CREATEROOM:
	{
		UINT roomid = CreateRoom();
		ClientInfo* client = m_ClientFinder[key];
		m_RoomFinder[roomid]->AcceptClient(client);
		m_RoomFinder[roomid]->RoomOwnerID = client->ID;
		break;
	}

	case MSGTYPE::MSGROOM::STARTROOM:
	{
		PK_ROOM_START* p = (PK_ROOM_START*)msg;
		if (m_RoomFinder.count(p->RoomID) != 0 &&
			m_RoomFinder[p->RoomID]->RoomOwnerID == p->ClientID &&
			m_RoomFinder[p->RoomID]->Status == RoomState::Waiting)
			m_RoomFinder[p->RoomID]->StartRoom();
		break;
	}

	case MSGTYPE::MSGROOM::ACCEPTROOM:
	{
		PK_ROOM_INFO* p = (PK_ROOM_INFO*)msg;
		ClientInfo* client = m_ClientFinder[key];
		if (m_RoomFinder.count(p->RoomID) != 0) {
			if (m_RoomFinder[p->RoomID]->Status == RoomState::Waiting) {
				RoomInfo* room = m_RoomFinder[p->RoomID];
				room->AcceptClient(client);
			}
		}
		break;
	}

	case MSGTYPE::MSGROOM::DISCONNECTROOM:
	{
		PK_ROOM_INFO* p = (PK_ROOM_INFO*)msg;
		ClientInfo* client = m_ClientFinder[key];
		if (client->RoomID != INVALID_VALUE) {
			auto room = m_RoomFinder[client->RoomID];
			room->LeaveRoom(client);
			room->ClientIDMatcher.erase(client->ID);
			if (room->AliveClientCounter <= 0) 
			{
				cout << "Room " << room->RoomID << " Client all out.\n";
				if (room->Status == RoomState::Waiting) {
					cout << "Room " << room->RoomID << " Closed.\n";
					m_RoomFinder.erase(room->RoomID);
					delete room;
				}
			}
			client->RoomID = INVALID_VALUE;
		}
		break;
	}

	case MSGTYPE::MSGROOM::REFRESHROOMINFO:
	{
		ClientInfo* client = m_ClientFinder[key];
		for (auto& room : m_RoomFinder) {
			if (room.second->Status == RoomState::Waiting) {
				PK_ROOM_INFO p;
				p.size = sizeof(PK_ROOM_INFO);
				p.type = MSGTYPE::MSGROOM::ROOMINFO;
				p.RoomID = room.second->RoomID;
				p.ClientID = client->ID;
				SendPacket(client->IOCPKey, &p);
			}
		}
		break;
	}

	case MSGTYPE::MSGROOM::CLIENT_TYPECHANGE:
	{
		PK_ROOM_INFO* p = (PK_ROOM_INFO*)msg;
		ClientInfo* client = m_ClientFinder[key];
		if (client->RoomID != INVALID_VALUE) {
			if (m_RoomFinder[client->RoomID]->Status == RoomState::Running) break;
			
			client->PlayerType = (PlayerType)p->ClientType;
			m_RoomFinder[client->RoomID]->SendPacket(msg);
			cout << "Client " << client->ID << " changed character to " << client->PlayerType << "\n";
		}
		break;
	}
	}
}

void MainServer::SendPacket(UINT key, void * packet)
{
	OverlappedEx* o = new OverlappedEx();
	char* p = reinterpret_cast<char*>(packet);
	memcpy(o->io_buf, packet, p[0]);
	o->op = op_Send;
	o->wsabuf.buf = o->io_buf;
	o->wsabuf.len = p[0];
	ZeroMemory(&o->wsaOver, sizeof(WSAOVERLAPPED));

	WSASend(m_ClientFinder[key]->sock, &o->wsabuf, 1, NULL, 0, &o->wsaOver, NULL);
}

void MainServer::TimerThreadfunc(MainServer * server)
{
	while (true) {
		Sleep(1);
		while (true) {
			server->EventMutex.lock();
			if (server->EventQueue.empty()) {
				server->EventMutex.unlock();
				break;
			}

			sEvent e = server->EventQueue.top();
			if (e.startTime > __GET_CURRENT_TIME__) {
				server->EventMutex.unlock();
				break;
			}

			server->EventQueue.pop();
			server->EventMutex.unlock();

			if (e.operation == enumOperation::op_UpdateScene ||
				e.operation == enumOperation::op_UpdateSend ||
				e.operation == enumOperation::op_TerminateRoom)
			{
				OverlappedEx* o = new OverlappedEx();
				o->op = e.operation;

				PostQueuedCompletionStatus(server->m_hIOCP, 0, e.id, reinterpret_cast<LPOVERLAPPED>(o));
			}
		}
	}
}

void MainServer::AddEvent(UINT id, enumOperation op, long long time)
{
	sEvent e;
	e.id = id;
	e.operation = op;
	e.startTime = __GET_CURRENT_TIME__ + time;

	EventMutex.lock();
	EventQueue.push(e);
	EventMutex.unlock();
}

void MainServer::Run()
{
	SOCKET client_sock;
	SOCKADDR_IN clientaddr;
	int addrlen;
	UINT clientCounter = 0;

	for (int i = 0; i<thread::hardware_concurrency(); ++i)
	{
		m_WorkerThread.push_back(new thread{ WorkerThreadfunc, this });
	}
	cout << "Worker Thread Initialized" << endl;

	m_TimerThread = thread{ TimerThreadfunc, this };

	while (1)
	{
		ZeroMemory(&clientaddr, sizeof(SOCKADDR_IN));
		clientaddr.sin_family = AF_INET;
		clientaddr.sin_port = htons(m_ServerPort);
		clientaddr.sin_addr.s_addr = INADDR_ANY;
		addrlen = sizeof(clientaddr);

		client_sock = WSAAccept(m_ListenSock, (SOCKADDR*)&clientaddr, &addrlen, NULL, NULL);
		if (client_sock == INVALID_SOCKET)
		{
			cout << "client socket error" << endl;
			break;
		}

		ClientInfo* newclient = new ClientInfo(clientCounter);
		newclient->addr = clientaddr;
		newclient->sock = client_sock;
		clientCounter++;

		CreateIoCompletionPort(
			reinterpret_cast<HANDLE*>(newclient->sock),
			m_hIOCP,
			newclient->IOCPKey,
			0
		);

		unsigned long flag = 0;

		WSARecv(
			newclient->sock,
			&newclient->OverlappedEx.wsabuf,
			1,
			NULL,
			&flag,
			&newclient->OverlappedEx.wsaOver,
			NULL
		);

		m_ClientFinder.insert(pair<UINT, ClientInfo*>(newclient->ID, newclient));

		cout << "Client " << newclient->ID << " Accepted\n";

		PK_MSG_CONNECT pa;
		pa.size = sizeof(PK_MSG_CONNECT);
		pa.type = MSGTYPE::MSGSTATE::CONNECT;
		pa.id = newclient->ID;
		SendPacket(newclient->IOCPKey, &pa);

	}
}

UINT MainServer::CreateRoom()
{
	RoomInfo* newroom = new RoomInfo();
	newroom->RoomID = m_iRoomCounter++;
	newroom->GameFramework.RegisterRoomInfo(newroom);
	newroom->RegisterPxFoundation(m_pFoundation);
	newroom->pMainServer = this;
	newroom->Status = RoomState::Waiting;

	m_RoomFinder.insert(pair<UINT, RoomInfo*>(newroom->RoomID, newroom));

	for (auto clientcontainer : m_ClientFinder) {
		auto client = clientcontainer.second;
		PK_ROOM_INFO p;
		p.size = sizeof(PK_ROOM_INFO);
		p.type = MSGTYPE::MSGROOM::ROOMINFO;
		p.RoomID = newroom->RoomID;
		p.ClientID = client->ID;
		SendPacket(client->IOCPKey, &p);
	}

	cout << "Create New Room " << newroom->RoomID << endl;
	return newroom->RoomID;
}

void MainServer::DeleteRoom()
{
	auto& iter = m_RoomFinder.begin();

	while (iter != m_RoomFinder.end()) {
		auto room = (*iter).second;
		if (room->ClientIDMatcher.size() <= 0 && room->Status != RoomState::Terminated) {
			AddEvent(room->RoomID, op_TerminateRoom, TERMINATETIME_ms);
		}
		else {
			++iter;
		}
	}
}

void MainServer::RemoveClientFromServer(unsigned long long key)
{
	PK_MSG_DELETE_PLAYER p;
	p.id = key;
	p.size = sizeof(PK_MSG_DELETE_PLAYER);
	p.type = MSGTYPE::MSGSTATE::DISCONNECT;

	ClientInfo* client = m_ClientFinder[key];
	if (client->RoomID != -1) {
		RoomInfo* clientroom = m_RoomFinder[client->RoomID];
		clientroom->RoomMutex.lock();
		clientroom->LeaveRoom(client);
		clientroom->RoomMutex.unlock();
		clientroom->SendPacket(&p);
		if (clientroom->Status != RoomState::Terminated && clientroom->AliveClientCounter <= 0) {
			AddEvent(clientroom->RoomID, op_TerminateRoom, TERMINATETIME_ms);
		}
		closesocket(client->sock);
	}

	delete client;
	m_ClientFinder.erase(key);
	cout << "Client " << key << " Disconnected\n";
}

void MainServer::SendRoomlist(unsigned long long clientkey, UINT current_roomid)
{
	if (!m_ClientFinder.count(clientkey)) return;
	
	ClientInfo* client = m_ClientFinder[clientkey];

	for (auto& room : m_RoomFinder) {
		if (room.first == current_roomid) continue;	// skip my room
		if (room.second->Status != RoomState::Waiting) continue;

		PK_ROOM_INFO p;
		p.size = sizeof(PK_ROOM_INFO);
		p.type = MSGTYPE::MSGROOM::ROOMINFO;
		p.RoomID = room.second->RoomID;
		p.ClientID = client->ID;
		SendPacket(client->IOCPKey, &p);
	}
}

MainServer::MainServer()
{
	m_pFoundation = PxCreateFoundation(PX_FOUNDATION_VERSION, m_Allocator, m_ErrorCallback);
}

MainServer::~MainServer()
{
	for (auto t : m_WorkerThread) { t->join(); delete t; }
	for (auto& c : m_ClientFinder) { delete c.second; }
	for (auto& r : m_RoomFinder) { delete r.second; }
	m_TimerThread.join();
	m_ClientFinder.clear();
	m_RoomFinder.clear();
}
