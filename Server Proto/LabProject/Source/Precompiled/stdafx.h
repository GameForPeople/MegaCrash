// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 및 프로젝트 관련 포함 파일이
// 들어 있는 포함 파일입니다.
//

#pragma once

#define WIN32_LEAN_AND_MEAN		// 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.

// Windows 헤더 파일
#include <windows.h>

// C의 런타임 헤더 파일
#include <tchar.h>
#include <conio.h>


// C++의 런타임 헤더 파일
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <iomanip>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <list>
#include <chrono>
#include <thread>
#include <map>
#include <mutex>
#include <queue>

using std::list;

// FBX_SDK 헤더 파일
#include <fbxsdk.h>

#pragma comment(lib,"libfbxsdk-md.lib")
//#pragma comment(lib,"libfbxsdk.lib")
//#pragma comment(lib,"libfbxsdk-mt.lib")


// Direct 헤더 파일
#include "XMathHelper.h"

using namespace DirectX;
using namespace DirectX::PackedVector;

// PhysX engine
#include <PxPhysicsAPI.h>

#ifdef _DEBUG
#pragma comment(lib, "PhysX3DEBUG_x64.lib")
#pragma comment(lib, "PxPvdSDKDEBUG_x64.lib")
#pragma comment(lib, "PxFoundationDEBUG_x64.lib")
#pragma comment(lib, "PhysX3ExtensionsDEBUG.lib")
#pragma comment(lib, "PhysX3CommonDEBUG_x64.lib")
#pragma comment(lib, "PhysX3CookingDEBUG_x64.lib")
#pragma comment(lib, "PhysX3CharacterKinematicDEBUG_x64.lib")
//#pragma comment(lib, "PhysX3VehicleDEBUG.lib")
//#pragma comment(lib, "PsFastXmlDEBUG_x64.lib")
//#pragma comment(lib, "PxTaskDEBUG_x64.lib")
//#pragma comment(lib, "SceneQueryDEBUG.lib")
//#pragma comment(lib, "SimulationControllerDEBUG.lib")
#else
#pragma comment(lib, "PhysX3_x64.lib")
#pragma comment(lib, "PxPvdSDK_x64.lib")
#pragma comment(lib, "PxFoundation_x64.lib")
#pragma comment(lib, "PhysX3Extensions.lib")
#pragma comment(lib, "PhysX3Common_x64.lib")
#pragma comment(lib, "PhysX3Cooking_x64.lib")
#pragma comment(lib, "PhysX3CharacterKinematic_x64.lib")
//#pragma comment(lib, "PhysX3Vehicle.lib")
//#pragma comment(lib, "PsFastXml_x64.lib")
//#pragma comment(lib, "PxTask_x64.lib")
//#pragma comment(lib, "SceneQuery.lib")
//#pragma comment(lib, "SimulationController.lib")
#endif
using namespace physx;

// 사용자 정의
#include "./Server/Tmsg.h"
#include "FBX\FBXExporter.h"
#include "PhysX\MeshCooking.h"
#include "resource.h"

#define RANDOM_NUM(MIN, MAX)	((rand()%((MAX) - (MIN) + 1)) + (MIN))

#define MESSAGE_PROCESSING_DELAY	0.01f
#define MESSAGE_PROCESSING_TIME		0.1f

#define __GET_CURRENT_TIME__		_Get_Current_Time_()

#define UPDATETIME_ms				16 // 1/60s -> 16ms
#define UPDATETIME_s				0.016f // 1/60s -> 0.016s
#define BROADCASTTIME_ms			50
#define REGENTIEM_s					4.f
#define TERMINATETIME_ms			1000

typedef enum {
	Ruby, Legion, Epsilon, Gravis
} PlayerType;

typedef enum {
	Normal, Dash, Jump, Stop
} MoveType;

typedef enum {
	InvalidAtkType = -1, Normal1, Normal2, Skill, Skill_Shoot
} AtkType;

typedef enum {
	AtkSkill, HighJump, TypeCount
} SkillType;

typedef enum {
	None,
	GroundBoard,
	Block,
	Players,
	Weapon,
	Weapon_Skill,
	Bullet,
	Count
} ObjectTag;

inline void Assign_float3(XMFLOAT3 & xmf3, PxVec3& p) {
	xmf3.x = p.x;
	xmf3.y = p.y;
	xmf3.z = p.z;
}

inline void Assign_float4(XMFLOAT4 & xmf4, PxQuat& q) {
	xmf4.x = q.x;
	xmf4.y = q.y;
	xmf4.z = q.z;
	xmf4.w = q.w;
}

inline PxVec3 toPxVec3(const XMFLOAT3& p) {
	return PxVec3{ p.x, p.y, p.z };
}

inline PxQuat toPxQuat(const XMFLOAT4& q) {
	return PxQuat{ q.x, q.y, q.z, q.w };
}

inline XMFLOAT3 toXmfloat3(const PxVec3& p) {
	return XMFLOAT3{ p.x, p.y, p.z };
}

inline XMFLOAT3 toXmfloat3(const PxExtendedVec3& p) {
	return XMFLOAT3{ (float)p.x, (float)p.y, (float)p.z };
}

inline XMFLOAT4 toXmfloat4(const PxQuat& q) {
	return XMFLOAT4{ q.x, q.y, q.z, q.w };
}

inline long long _Get_Current_Time_() {
	const long long _Freq = _Query_perf_frequency();	// doesn't change after system boot
	const long long _Ctr = _Query_perf_counter();
	const long long _Whole = (_Ctr / _Freq) * 1000;
	const long long _Part = (_Ctr % _Freq) * 1000 / _Freq;
	return _Whole + _Part;
}

inline std::string str_objtag(ObjectTag tag) {
	std::string str;
	switch (tag) {
	case ObjectTag::GroundBoard:	str = "Ground ";		break;
	case ObjectTag::Block:			str = "Block ";			break;
	case ObjectTag::Bullet:			str = "Bullet ";		break;
	case ObjectTag::Weapon:			str = "Weapon ";		break;
	case ObjectTag::Weapon_Skill:	str = "Weapon_Skill ";	break;
	case ObjectTag::Players:		str = "Players ";		break;
	default:	str = "none "; break;
	}
	return str;
}