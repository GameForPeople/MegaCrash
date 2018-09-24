#include "stdafx.h"
#include "Server.h"

Server::Server()
{
}
Server::~Server()
{
}

void Server::Initialize()
{
	srand((unsigned int)time(NULL));

	int retval;

	if (WSAStartup(MAKEWORD(2, 2), &m_WSA) != 0) 
	{
		cout << "WinSock Error" << endl;

		// Assert
		return;
	}
	cout << "WSAStartUP" << endl;

	m_hIOCP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);

	m_ListenSock = WSASocketW(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (m_ListenSock == INVALID_SOCKET)
	{
		cout << "Listen Socket Error" << endl;

		// Assert
		return;
	}
	cout << "Listen Socket Initialized" << endl;
	
	ZeroMemory(&m_ServerAddr, sizeof(m_ServerAddr));
	m_ServerAddr.sin_family = AF_INET;
	m_ServerAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	m_ServerAddr.sin_port = htons(m_ServerPort);
	retval = ::bind(m_ListenSock, (SOCKADDR *)&m_ServerAddr, sizeof(m_ServerAddr));
	if (retval == SOCKET_ERROR)
	{
		cout << "Bind Error" << endl;
		// Assert
		return;
	}
	cout << "Local Address Bind Initialized" << endl;

	retval = listen(m_ListenSock, SOMAXCONN);
	if (retval == SOCKET_ERROR)
	{
		cout << "Listening Failed" << endl;
		// Assert
		return;
	}
	cout << "Start Server Listening" << endl;
}

void Server::Release()
{
	closesocket(m_ListenSock);
	cout << "close ListenSock" << endl;
	WSACleanup();
	cout << "WSACleanup" << endl;
}
