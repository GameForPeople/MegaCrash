// Main Server.cpp : 응용 프로그램에 대한 진입점을 정의합니다.
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