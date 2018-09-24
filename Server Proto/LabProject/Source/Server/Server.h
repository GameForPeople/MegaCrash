#pragma once

#include <WinSock2.h>
#pragma comment(lib, "ws2_32")

#include <iostream>
#include <chrono>
#include <list>
using namespace std;

#include "Tmsg.h"

class Server
{
protected:
	WSADATA			m_WSA;
	SOCKET			m_ListenSock;
	SOCKADDR_IN		m_ServerAddr;
	u_short			m_ServerPort = 9000;
	HANDLE			m_hIOCP;

public:
	Server();
	virtual ~Server();

	void Initialize();
	void Release();
	virtual void Run() = 0;
};

