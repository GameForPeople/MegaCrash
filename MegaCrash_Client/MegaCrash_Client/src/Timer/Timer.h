#pragma once
//-----------------------------------------------------------------------------
// File: Timer.h
//-----------------------------------------------------------------------------

constexpr ULONG MAX_SAMPLE_COUNT = 50; // Maximum frame time sample count

class CTimer
{
public:
	CTimer();
	virtual ~CTimer();

	bool Tick();
	void Start();
	void Stop();
	void Reset();

    unsigned long GetFrameRate();
    float GetTimeElapsed();
	float GetTotalTime();

private:
	float							m_fTimeElapsed;		

    float							m_fFrameTime[MAX_SAMPLE_COUNT];
    ULONG							m_nSampleCount;

    unsigned long					m_nCurrentFrameRate;				
	unsigned long					m_nFramesPerSecond;					
	float							m_fFPSTimeElapsed;		

	bool							m_bStopped;

	time_point<steady_clock>		m_tpStartTime;
	time_point<steady_clock>		m_tpStopTime;
	time_point<steady_clock>		m_tpPauseTime;
	time_point<steady_clock>		m_tpCurrentTime;
	time_point<steady_clock>		m_tpLastTime;
	duration<float>					m_DurationTime;
};