// main.cpp: 응용 프로그램의 진입점을 정의합니다.
//

#include "stdafx.h"
#include "main.h"
#include "Framework\Framework.h"

// 전역 변수:
HINSTANCE						hInst;
WCHAR							szTitle[TITLE_MAX_LENGTH];
WCHAR							szWindowClass[TITLE_MAX_LENGTH];

CFramework						gGameFramework;

// 이 코드 모듈에 들어 있는 함수의 정방향 선언입니다.
ATOM MyRegisterClass(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE, int);

int APIENTRY wWinMain(
	_In_		HINSTANCE	hInstance,
	_In_opt_	HINSTANCE	hPrevInstance,
	_In_		LPWSTR		lpCmdLine,
	_In_		int			nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

#if USE_DEBUG_CONSOLE
	// 디버그 콘솔 생성
	if (!AllocConsole()) {
		MessageBox(NULL, L"The console window was not created", NULL, MB_ICONEXCLAMATION);
	}

	FILE *fp_in{ nullptr }, *fp_out{ nullptr }, *fp_err{ nullptr };
	freopen_s(&fp_in, "CONIN$", "r", stdin);
	freopen_s(&fp_out, "CONOUT$", "w", stdout);
	freopen_s(&fp_err, "CONOUT$", "w", stderr);

	HWND consoleWindow	{ GetConsoleWindow() };
	LONG style			{ GetWindowLong(consoleWindow, GWL_STYLE) };
	style |= WS_VSCROLL;
	::SetWindowLong(consoleWindow, GWL_STYLE, style);
	::SetWindowTextA(consoleWindow, "DEBUG CONSOLE");

	std::string setConsoleSize = 
		  "mode con:lines=" + std::to_string(DEBUG_CONSOLE_HEIGHT) 
		+ " cols=" + std::to_string(DEBUG_CONSOLE_WIDTH);
	system(setConsoleSize.c_str());
#endif

	// 전역 문자열을 초기화합니다.
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, TITLE_MAX_LENGTH);
	LoadStringW(hInstance, IDC_MEGACRASHCLIENT, szWindowClass, TITLE_MAX_LENGTH);
	MyRegisterClass(hInstance);

	// 응용 프로그램 초기화를 수행합니다.
	if (!InitInstance(hInstance, nCmdShow)) {
#if USE_DEBUG_CONSOLE
		fclose(fp_in);
		fclose(fp_out);
		fclose(fp_err);
		if (!FreeConsole()) {
			MessageBox(NULL, L"Failed to free the console!", NULL, MB_ICONEXCLAMATION);
		}
#endif
		return FALSE;
	}

	HACCEL hAccelTable{ LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MEGACRASHCLIENT)) };
	MSG msg;

	// 기본 메시지 루프입니다.
	for(;;) {
		if (::PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT) break;
			if (::TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) continue;

			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
		gGameFramework.FrameAdvance();
	}
	gGameFramework.OnDestroy();

#if USE_DEBUG_CONSOLE
	fclose(fp_in);
	fclose(fp_out);
	fclose(fp_err);
	if (!FreeConsole()) {
		MessageBox(NULL, L"Failed to free the console!", NULL, MB_ICONEXCLAMATION);
	}
#endif
	return (int)msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = CFramework::WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MEGACRASHCLIENT));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= NULL;
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return ::RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

	// 윈도우 스타일
	DWORD dwStyle = 0
		| WS_OVERLAPPED 	// 디폴트 윈도우. 타이틀 바와 크기 조절이 안되는 경계선을 가진다. 아무런 스타일도 주지 않으면 이 스타일이 적용된다.
		| WS_CAPTION 		// 타이틀 바를 가진 윈도우를 만들며 WS_BORDER 스타일을 포함한다.
		| WS_SYSMENU		// 시스템 메뉴를 가진 윈도우를 만든다.
		| WS_MINIMIZEBOX	// 최소화 버튼을 만든다.
		| WS_BORDER			// 단선으로 된 경계선을 만들며 크기 조정은 할 수 없다.
		;					// 추가로 필요한 윈도우 스타일은 http://www.soen.kr/lecture/win32api/reference/Function/CreateWindow.htm 참고.

	// 데스크톱 윈도우 사이즈
	RECT getWinSize;
	::GetWindowRect(::GetDesktopWindow(), &getWinSize);

	// 클라이언트 사이즈
	RECT rc;
	rc.left = rc.top = 0;
	rc.right = CLIENT_WIDTH;
	rc.bottom = CLIENT_HEIGHT;

	// 데스크톱의 중앙에 클라이언트가 위치하도록 설정
	CLIENT_LEFT_TOP_POS.x = (getWinSize.right - CLIENT_WIDTH) / 2;
	CLIENT_LEFT_TOP_POS.y = (getWinSize.bottom - CLIENT_HEIGHT) / 2;

	//	윈도우 사이즈에 실제로 추가되는(캡션, 외곽선 등) 크기를 보정.
	::AdjustWindowRect(&rc, dwStyle, FALSE);

	// 윈도우 생성
	HWND hWnd = CreateWindow(
		  szWindowClass				//	윈도우 클래스 명
		, szTitle					//	캡션 표시 문자열
		, dwStyle					//	윈도우 스타일
		, CLIENT_LEFT_TOP_POS.x		//	부모 윈도우 기반 윈도우 시작좌표 : x
		, CLIENT_LEFT_TOP_POS.y		//	부모 윈도우 기반 윈도우 시작좌표 : y
		, rc.right - rc.left		//	윈도우 사이즈 : width
		, rc.bottom - rc.top		//	윈도우 사이즈 : height
		, NULL						//	부모 윈도우.
		, NULL						//	메뉴 핸들
		, hInstance					//	인스턴스 핸들
		, NULL						//	추가 파라메터 : NULL
	);

	// 생성 실패시 프로그램 종료
	// 확인 : WndProc의 default msg handler가 DefWindowProc 함수를 반환하는가?
	if (!hWnd) {
		LPVOID lpMsgBuf;
		FormatMessage(
		 	  FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM
			, NULL
			, ::GetLastError()
			, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT)
			, reinterpret_cast<LPTSTR>(&lpMsgBuf)
			, 0
			, NULL
		);
		MessageBox(NULL, reinterpret_cast<LPCTSTR>(lpMsgBuf), L"Create Window Fail", MB_ICONERROR);
		::LocalFree(lpMsgBuf);
		return FALSE;
	}

	gGameFramework.OnCreate(hInstance, hWnd);

	::ShowWindow(hWnd, nCmdShow);
	::UpdateWindow(hWnd);

	return TRUE;
}