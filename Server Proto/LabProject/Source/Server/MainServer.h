#pragma once
#include "Server.h"
#include "Framework\GameFramework.h"

struct TMSG;
class MainServer;

#define		PVD_STANDBY		1

enum enumOperation {
	op_Send,
	op_Recv,
	op_UpdateScene,
	op_UpdateSend,
	op_Regen,
	op_TerminateRoom
};

struct OverlappedEx {
	WSAOVERLAPPED	wsaOver;
	WSABUF			wsabuf;
	char			io_buf[MAX_BUFF_SIZE];
	enumOperation	op;
};

struct sEvent
{
	long long		startTime = 0;
	enumOperation	operation;
	UINT			id;
};

struct cmp {
	bool operator()(sEvent t, sEvent u) {
		return t.startTime > u.startTime;
	}
};


struct ClientInfo {
	OverlappedEx			OverlappedEx;
	UINT					ID;
	UINT					IOCPKey;
	UINT					RoomID = -1;
	SOCKET					sock;
	SOCKADDR_IN				addr;

	int						packetsize;
	int						prev_packetsize;
	char					prev_packet[MAX_PACKET_SIZE];

	PlayerType				PlayerType = PlayerType::Ruby;

	ClientInfo()
		: sock(NULL)
	{}

	ClientInfo(int _id)
		: sock(NULL)
	{
		ZeroMemory(&OverlappedEx.wsaOver, sizeof(WSAOVERLAPPED));
		ID = _id;
		IOCPKey = _id;
		OverlappedEx.op = enumOperation::op_Recv;
		OverlappedEx.wsabuf.buf = OverlappedEx.io_buf;
		OverlappedEx.wsabuf.len = sizeof(OverlappedEx.io_buf);
		packetsize = 0;
		prev_packetsize = 0;
	}

	~ClientInfo() {
		if (sock)		closesocket(sock);
	}
};

enum RoomState {
	Invalid = -1
	, Waiting
	, Running
	, Finished
	, Terminated
};

struct RoomInfo {
	UINT					RoomID = INVALID_VALUE;
	RoomState				Status = RoomState::Invalid;

	unordered_map<UINT, ClientInfo*>	ClientIDMatcher;
	UINT					RoomOwnerID = INVALID_OBJECT_ID;

	CGameFramework			GameFramework;
	MainServer				*pMainServer;
	mutex					RoomMutex;

	PxFoundation			*m_pFoundation = NULL;

	int						AliveClientCounter = 0;
	int						MaxClientCount = 0;

	RoomInfo() {}
	~RoomInfo() { Release(); };

	void		BuildRoom();
	void		StartRoom();
	void		AcceptClient(ClientInfo* client);
	void		LeaveRoom(ClientInfo* client);
	void		SendCloseRoom();
	void		SendPacket(void* packet);

	void		SendPacket(const int id, void* packet);

	void		RegisterPxFoundation(PxFoundation* PxF) { m_pFoundation = PxF; }

	void Release()
	{
		ClientIDMatcher.clear();
	}
};

class MainServer : public Server
{
private:
	vector<thread*>				m_WorkerThread;
	thread						m_TimerThread;

	unordered_map<UINT, ClientInfo*>		m_ClientFinder;
	unordered_map<UINT, RoomInfo*>			m_RoomFinder;

	UCHAR						m_iRoomCounter = 0;

	PxDefaultAllocator			m_Allocator;
	PxDefaultErrorCallback		m_ErrorCallback;
	PxFoundation				*m_pFoundation = NULL;

	std::priority_queue<sEvent, vector<sEvent>, cmp>	EventQueue;
	std::mutex					EventMutex;

public:
	MainServer();
	virtual ~MainServer();
	virtual void Run();

	PxFoundation*			GetPxFoundation() { return m_pFoundation; }

	UINT			CreateRoom();
	void			DeleteRoom();
	void			RemoveClientFromServer(unsigned long long key);
	void			SendRoomlist(unsigned long long clientkey, UINT roomid);

	void			ProcessMSG(UINT key, const MSGWRAPPER& msg);
	void			SendPacket(UINT key, void * packet);

	static void		WorkerThreadfunc(MainServer* server);
	static void		TimerThreadfunc(MainServer* server);
	void			AddEvent(UINT id, enumOperation op, long long time);
};

static void err_quit(char *msg);
static void err_display(char *msg);