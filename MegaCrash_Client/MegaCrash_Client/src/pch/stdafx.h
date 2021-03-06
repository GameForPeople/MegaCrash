// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 또는 프로젝트 관련 포함 파일이
// 들어 있는 포함 파일입니다.
//

#pragma once
#include "targetver.h"
#define _ENABLE_EXTENDED_ALIGNED_STORAGE
// Windows 헤더 파일:
#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
#include <windows.h>

// C 런타임 헤더 파일:
#include <tchar.h>

// C++ 런타임 헤더 파일:
#include <iostream>
#include <fstream>
#include <chrono>
#include <memory>
#include <string>
#include <vector>
#include <list>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <mutex>
#include <thread>
using namespace std;
using namespace std::chrono;

// Window Socket
//#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32")

// ComPtr
#include <wrl.h>
using Microsoft::WRL::ComPtr;

// DXGI
#include <dxgi1_6.h>
#ifdef _DEBUG
#include <initguid.h>
#include <dxgidebug.h>
#endif
#pragma comment(lib, "dxgi.lib")

// DirectX 12
#include <D3D12.h>
#include <D3Dcompiler.h>
#include <DirectXColors.h>
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "d3dcompiler.lib")
//#pragma comment(lib, "DirectXTex.lib")

// FBX SDK
#include <fbxsdk.h>
#pragma comment(lib,"libfbxsdk-md.lib")
//#pragma comment(lib,"libfbxsdk.lib")
//#pragma comment(lib,"libfbxsdk-mt.lib")

// 사용자 정의
#include "UserDefined\SystemDefine.h"
#include "UserDefined\inlineFuncs.h"
#include "UserDefined\XMathHelper.h"
#include "External\FBXExporter.h"
#include "Client\Tmsg.h"
#include "Client\Client.h"
#include "Manager\SoundMgr\SoundMgr.h"
#include "Manager\ResourseMgr\ResMgr.h"

// IID_PPV_ARGS() - __uuidof(), (void**) 매크로

