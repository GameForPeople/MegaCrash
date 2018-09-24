#pragma once
//-----------------------------------------------------------------------------
// File: Timer.h
//-----------------------------------------------------------------------------
#define MAX_FPS 0.016f

const ULONG MAX_SAMPLE_COUNT = 50; // Maximum frame time sample count

class CTimer
{
public:
	CTimer();
	virtual ~CTimer();

	bool Tick();
	void Start();
	void Stop();
	void Reset();

    unsigned long GetFrameRate(LPTSTR lpszString = NULL, int nCharacters=0);
    float GetTimeElapsed();
	float GetTotalTime();

private:
	double							m_fTimeScale;						
	float							m_fTimeElapsed;		

	__int64							m_nBasePerformanceCounter;
	__int64							m_nPausedPerformanceCounter;
	__int64							m_nStopPerformanceCounter;
	__int64							m_nCurrentPerformanceCounter;
    __int64							m_nLastPerformanceCounter;

	__int64							m_nPerformanceFrequencyPerSec;				

    float							m_fFrameTime[MAX_SAMPLE_COUNT];
    ULONG							m_nSampleCount;

    unsigned long					m_nCurrentFrameRate;				
	unsigned long					m_nFramesPerSecond;					
	float							m_fFPSTimeElapsed;		

	bool							m_bStopped;
};

class CDebugTimer
{
public:
	CDebugTimer();
	virtual ~CDebugTimer();

	void Start();
	void End();
	void PrintSecondInterval(const char* target_name);
	void PrintMilliSecInterval(const char* target_name);
	void PrintMicroSecInterval(const char* target_name);

private:
	double							m_fTimeScalePerSec;
	double							m_fTimeScalePerMillisec;
	double							m_fTimeScalePerMicrosec;
	__int64							m_nPerformanceFrequencyPerSec;

	__int64							m_nCurrentPerformanceCounter;
	__int64							m_nLastPerformanceCounter;
	__int64							m_nIntervalPerformanceCounter;

};