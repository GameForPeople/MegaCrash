// Main Server.cpp : ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include "PhysX\MeshCooking.h"
#include "Server\MainServer.h"

int main()
{
	//ExportFBXAnimation("CookingFBX/spear charge2.FBX");
	//ExportFBXMesh("CookingFBX/OutPostBreakable.FBX");
	//ReadPositionFile("./ServerAssets/Ground.txt");

	MainServer* server;
	server = new MainServer();
	server->Initialize();
	server->Run();
	server->Release();

	return 0;
}