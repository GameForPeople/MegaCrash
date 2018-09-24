#pragma once

class CClient
{
private:
	CClient();

public:
	static CClient* const Instance();
	~CClient();

	void Release();
	bool ConnectServer();
	void SendMsg(UINT send_packet_size);
	void RecvMsg();

	bool IsConnect() const { return m_bConnected; }
	bool IsEnteredRoom() const { return m_bEnterRoom; }

	void Disconnect() { m_bConnected = false; }
	void LeaveRoom() { m_bEnterRoom = false; }

	void MsgEnqueue(BYTE* msg);
	BYTE* MsgDequeue();
	BYTE* GetSendBuf() { return m_SendBuf; }

	void SetClientID(UINT id)		{ m_ClientID = id; m_bConnected = true; }
	void SetRoomID(UINT id)			{ m_RoomID = id; m_bEnterRoom = true; }
	void SetPlayerObjectID(UINT id) { m_ObjectID = id; }

	UINT GetClientID()			const { return m_ClientID; }
	UINT GetRoomID()			const { return m_RoomID; }
	UINT GetPlayerObjectID()	const { return m_ObjectID; }

private:
	UINT					m_RoomID;
	UINT 	         		m_ClientID;
	UINT					m_ObjectID;

	SOCKET					m_Sock;
	SOCKADDR_IN  			m_SockAddr;

	bool					m_bConnected;
	bool					m_bEnterRoom;

	list<BYTE*>				m_MsgQueue;
	mutex					m_MsgQueueMutex;

	WSAEVENT				m_hWsaEvent;
	WSANETWORKEVENTS		m_netEvents;

	thread					m_thRecv;

	WSABUF					m_Send_wsabuf;
	BYTE					m_SendBuf[MAX_PACKET_SIZE];

	WSABUF					m_Recv_wsabuf;
	BYTE					m_RecvBuf[BUF_SIZE];

	BYTE					m_PacketBuf[MAX_PACKET_SIZE];
	DWORD					m_InPacketSize;
	int						m_SavedPacketSize;
};