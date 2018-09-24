#pragma once

/* ========================================================== //
// ===================== Option setting ===================== //
// =================================== (0 : 비활성 | 1 : 활성) */
#define USE_LIMIT_MAX_FPS			1	// 최대 FPS 설정 여부
#define USE_DEBUG_CONSOLE			0	// 디버그 콘솔 사용 여부
#define USE_SERVER_CONNECT			1	// 클라 및 서버 연결 여부
#define USE_FEATURE_LEVEL_12		0	// Feature level 12 사용
#define USE_LOADING_CACHE_SHADER	1	// hlsl 캐시 파일 사옹

// 서버 연결처리 전역상수 설정
constexpr UINT				SERVERPORT			{ 9000 };
constexpr UINT				BUF_SIZE			{ 4096 };
constexpr UINT				MAX_PACKET_SIZE		{ 255 };
constexpr UINT				STR_SIZE			{ 64 };

// 클라이언트 크기 설정
//constexpr UINT				CLIENT_WIDTH		{ 1280 }; // 1920
//constexpr UINT				CLIENT_HEIGHT		{ 720 }; // 1080
constexpr UINT				CLIENT_WIDTH		{ 1600 }; // 1920
constexpr UINT				CLIENT_HEIGHT		{ 900 }; // 1080

// 클라이언트 타이틀 설정
constexpr UINT				TITLE_MAX_LENGTH	{ 64 };
constexpr wchar_t * const	TITLESTRING			{ L"MegaCrash" };

// 바탕화면에서의 클라이언트 절대좌표(left, top)
static POINT				CLIENT_LEFT_TOP_POS;

// 최대 FPS 설정
#if USE_LIMIT_MAX_FPS
constexpr float MAX_FPS{ 1.0f / 60.f };
#else
constexpr float MAX_FPS{ 0.0f };
#endif

// 서버 업데이트 FPS 설정
constexpr float		MAX_UPDATE_FPS			{ 1.0f / 15.f };

// 클라이언트 패킷전송 ms 설정
constexpr long long SEND_MOVE_PACKET_MS		{ 20 };
constexpr long long SEND_ATK_PACKET_MS		{ 150 };

// 디버그 콘솔창 크기 설정
constexpr size_t DEBUG_CONSOLE_WIDTH		{ 60 };
constexpr size_t DEBUG_CONSOLE_HEIGHT		{ 35 };

/* 컴파일러 최적화 끄기, 켜기 (Release 모드 디버그용)
#pragma optimize("gpsy",off)
{ 
	// 코드블럭
}
#pragma optimize("gpsy",on)
*/