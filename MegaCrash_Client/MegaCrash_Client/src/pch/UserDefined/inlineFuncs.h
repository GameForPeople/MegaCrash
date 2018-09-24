#pragma once
#include <Windows.h>
#include <chrono>
#include <random>

/// <summary>
/// 현재 윈도우 프로시저와 연결된 클래스 인스턴스를 가져옵니다.
/// </summary>
///	<param name = "hWnd"> 윈도우 핸들입니다. 해당 윈도우 핸들과 연결된 클래스 인스턴스를 반환합니다. </param>
/// <returns> 연결된 클래스 인스턴스 포인터를 반환합니다. </returns>
template <typename FrameworkPtr>
FrameworkPtr GetUserDataPtr(HWND hWnd) noexcept
{
	return reinterpret_cast<FrameworkPtr>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
}

/// <summary>
/// 윈도우 프로시저와 클래스 인스턴스를 연결합니다.
/// </summary>
///	<param name = "hWnd"> 윈도우 핸들입니다. 이 핸들 값으로 데이터를 찾을 수 있게 됩니다. </param>
///	<param name = "ptr"> 윈도우 핸들과 연결될 데이터입니다. 대체로 클래스 인스턴스의 포인터를 사용합니다. </param>
/// <returns> LONG_PTR 형식으로 결과값을 반환합니다. </returns>
inline auto SetUserDataPtr(HWND hWnd, LPVOID ptr) noexcept
{
	return ::SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(ptr));
}

/// <summary>
/// HRESULT가 S_FALSE 일 경우 입력된 메시지를 입력한 메시지 박스를 출력하며 true를 반환합니다.
/// </summary>
///	<param name = "hr"> 평가할 HRSULT 객체입니다. S_FALSE이면 메시지 박스를 출력합니다. </param>
///	<param name = "msg"> 메시지입니다. 생략 가능합니다. </param>
///	<param name = "captionMsg"> 캡션 문장입니다. 생략 가능합니다. </param>
/// <returns> HRESULT 값이 S_FALSE일 경우 true를 반환합니다. </returns>
inline bool FailureMessage(HRESULT hr, LPCTSTR msg = TEXT(""), LPCTSTR captionMsg = TEXT("Error!"))
{
	if (FAILED(hr))
	{
		MessageBox(NULL, msg, captionMsg, MB_OK);
		return true;
	}
	return false;
}

/// <summary> 
/// 현재 시간(ms)을 반환합니다.
/// </summary>
inline long long GetCurrTime()
{
	static constexpr long long den{ 1000 };
	static const long long _Freq{ _Query_perf_frequency() };
	static long long _Ctr, _Whole, _Part;
	_Ctr = _Query_perf_counter();
	_Whole = (_Ctr / _Freq) * den;
	_Part = (_Ctr % _Freq) *  den / _Freq;
	return (_Whole + _Part);
}

static long long  gStopwatch = GetCurrTime();
/// <summary> 
/// 새로운 초 세기를 시작합니다. 
/// </summary>
inline void STOPWATCH_COUNT_START()
{
	gStopwatch = GetCurrTime();
}
/// <summary> 
/// 초 세기를 시작한 지점 또는 이전 함수호출 지점부터 현재까지 흐른 시간을 확인합니다.
/// </summary>
inline long long STOPWATCH_COUNT_CHECK()
{
	const long long retval{ GetCurrTime() - gStopwatch };
	gStopwatch = GetCurrTime();
	return retval;
}

static default_random_engine				gRE;
static uniform_int_distribution<int>		giDist;
static uniform_real_distribution<float>		gfDist;
/// <summary> 
/// 임의의 양의 정수를 반환합니다. 
/// </summary>
inline int rand_int() { return giDist(gRE); }
/// <summary> 
/// 임의의 양의 실수를 반환합니다. 
/// </summary>
inline float rand_float() { return gfDist(gRE); }

/// <summary>
/// 인자로 받은 두 값을 t 값만큼 선형 보간합니다.
/// </summary>
///	<param name = "start"> 시작 값입니다. </param>
///	<param name = "end"> 끝 값입니다. </param>
///	<param name = "t"> 보간 값입니다. 대체로 0부터 1 사이의 숫자입니다. </param>
template<typename Ty>
constexpr Ty Interpolation(Ty start, Ty end, float t)
{
	return static_cast<Ty>(start * (1.f - t) + end * t);
}

/// <summary>
/// 인자로 받은 정수 x 값을 n제곱 합니다.
/// </summary>
///	<param name = "x"> 거듭제곱에서의 밑 입니다. </param>
///	<param name = "n"> 거듭제곱에서의 지수 입니다. </param>
constexpr UINT iPow(UINT x, UINT n)
{
	UINT retval = 1;
	if (n < 0) return 0;
	for (UINT i = 0; i < n; ++i) retval *= x;
	return retval;
}


