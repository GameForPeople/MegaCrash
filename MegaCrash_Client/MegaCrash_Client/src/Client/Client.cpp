#include "stdafx.h"
#include "Client.h"

void err_quit(char* msg)
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
void err_display(char* msg)
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

CClient::CClient()
	: m_ClientID{ 0 }
	, m_bConnected{ false }
{
	m_Send_wsabuf.buf = reinterpret_cast<char*>(m_SendBuf);
	m_Send_wsabuf.len = BUF_SIZE;
	m_Recv_wsabuf.buf = reinterpret_cast<char*>(m_RecvBuf);
	m_Recv_wsabuf.len = BUF_SIZE;

	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return;

	m_Sock = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, 0);
	if (m_Sock == INVALID_SOCKET)
		err_quit("socket()");
}

CClient * const CClient::Instance()
{
	static CClient* c{ nullptr };
	if (!c) c = new CClient();
	return c;
}

CClient::~CClient()
{
	Release();
}

void CClient::Release()
{
	WSACloseEvent(m_hWsaEvent);
	shutdown(m_Sock, SD_SEND);
	closesocket(m_Sock);
	WSACleanup();

	m_MsgQueueMutex.lock();
	for (auto& p : m_MsgQueue) delete[] p;
	m_MsgQueue.clear();
	m_MsgQueueMutex.unlock();

	m_thRecv.join();
}

bool CClient::ConnectServer()
{
	static bool bConnected{ false };

	if (!m_bConnected) {
		const int ip_buf_size		{ 16 };
		char ServerIP[ip_buf_size]	{ 0, };

		FILE* fp{ nullptr };
		fopen_s(&fp, "ServerIP.txt", "r");
		fread_s(ServerIP, sizeof(ServerIP), sizeof(char), ip_buf_size, fp);
		fclose(fp);

		m_SockAddr.sin_family = AF_INET;
		inet_pton(m_SockAddr.sin_family, ServerIP, &m_SockAddr.sin_addr);
		m_SockAddr.sin_port = htons(SERVERPORT);

		int retval = WSAConnect(
			  m_Sock
			, reinterpret_cast<SOCKADDR*>(&m_SockAddr)
			, sizeof(m_SockAddr)
			, NULL, NULL, NULL, NULL);

		if (retval == SOCKET_ERROR)
		{
			printf("Connect failed!\n");
			Sleep(1000);
			return bConnected;
		}

		m_hWsaEvent = WSACreateEvent();
		WSAEventSelect(m_Sock, m_hWsaEvent, FD_CLOSE | FD_READ);
		m_thRecv = thread{ [this]() { this->RecvMsg(); } };

		bConnected = true;
	}
	return bConnected;
}

void CClient::SendMsg(UINT send_packet_size)
{
	DWORD iobyte{ 0 };
	m_Send_wsabuf.len = send_packet_size;
	int ret = WSASend(m_Sock, &m_Send_wsabuf, 1, &iobyte, 0, NULL, NULL);
	if (ret) err_display("Send Error");
}

void CClient::RecvMsg()
{
	DWORD iobyte{ 0 }, ioflag{ 0 };
	int ret{ 0 };
	for (;;)
	{
		ret = WSAWaitForMultipleEvents(1, &m_hWsaEvent, FALSE, WSA_INFINITE, FALSE);
		if (ret == WSA_WAIT_FAILED || ret == WSA_WAIT_TIMEOUT) continue;
		WSAEnumNetworkEvents(m_Sock, m_hWsaEvent, &m_netEvents);
		if (m_netEvents.lNetworkEvents & FD_READ)
		{
			if (m_netEvents.iErrorCode[FD_READ_BIT] != 0) 
			{
				err_display("Read Error");
				continue;
			}

			ret = WSARecv(m_Sock, &m_Recv_wsabuf, 1, &iobyte, &ioflag, NULL, NULL);
			if (ret) err_display("Recv Error");
			BYTE *ptr = reinterpret_cast<BYTE *>(m_RecvBuf);

			while (0 != iobyte) {
				if (0 == m_InPacketSize) m_InPacketSize = ptr[0];
				if (iobyte + m_SavedPacketSize >= m_InPacketSize) {
					memcpy(m_PacketBuf + m_SavedPacketSize, ptr, m_InPacketSize - m_SavedPacketSize);

					//ProcessPacket(m_PacketBuf);
					BYTE* new_msg = new BYTE[m_InPacketSize];
					memcpy(new_msg, m_PacketBuf, m_InPacketSize);
					MsgEnqueue(new_msg);

					ptr += m_InPacketSize - m_SavedPacketSize;
					iobyte -= m_InPacketSize - m_SavedPacketSize;
					m_InPacketSize = 0;
					m_SavedPacketSize = 0;
				}
				else {
					memcpy(m_PacketBuf + m_SavedPacketSize, ptr, iobyte);
					m_SavedPacketSize += iobyte;
					iobyte = 0;
				}
			}
		}
		if (m_netEvents.lNetworkEvents & FD_CLOSE)
		{
			err_display("Connect close");
			break;
		}
	}
}

void CClient::MsgEnqueue(BYTE* msg)
{
	m_MsgQueueMutex.lock();
	m_MsgQueue.push_back(msg);
	m_MsgQueueMutex.unlock();
}

BYTE* CClient::MsgDequeue()
{
	BYTE* msg{ nullptr };
	m_MsgQueueMutex.lock();
	if (!m_MsgQueue.empty())
	{
		msg = m_MsgQueue.front();
		m_MsgQueue.pop_front();
	}
	m_MsgQueueMutex.unlock();
	return msg;
}
