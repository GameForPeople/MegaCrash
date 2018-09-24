//-----------------------------------------------------------------------------
// File: Timer.cpp
//-----------------------------------------------------------------------------

#include "stdafx.h"
#include "Timer.h"

CTimer::CTimer()
{
	::QueryPerformanceFrequency((LARGE_INTEGER *)&m_nPerformanceFrequencyPerSec);
	::QueryPerformanceCounter((LARGE_INTEGER *)&m_nLastPerformanceCounter); 
	m_fTimeScale = 1.0 / (double)m_nPerformanceFrequencyPerSec;

	m_nBasePerformanceCounter = m_nLastPerformanceCounter;
	m_nPausedPerformanceCounter = 0;
	m_nStopPerformanceCounter = 0;

	m_nSampleCount = 0;
	m_nCurrentFrameRate = 0;
	m_nFramesPerSecond = 0;
	m_fFPSTimeElapsed = 0.0f;
}

CTimer::~CTimer()
{
}

bool CTimer::Tick()
{
	if (m_bStopped)
	{
		m_fTimeElapsed = 0.0f;
		return false;
	}
	float fTimeElapsed;

	::QueryPerformanceCounter((LARGE_INTEGER *)&m_nCurrentPerformanceCounter);
	fTimeElapsed = float((m_nCurrentPerformanceCounter - m_nLastPerformanceCounter) * m_fTimeScale);

	if (fTimeElapsed < MAX_FPS) return false;

	m_nLastPerformanceCounter = m_nCurrentPerformanceCounter;

    if (fabsf(fTimeElapsed - m_fTimeElapsed) < 1.0f)
    {
        ::memmove(&m_fFrameTime[1], m_fFrameTime, (MAX_SAMPLE_COUNT - 1) * sizeof(float));
        m_fFrameTime[0] = fTimeElapsed;
        if (m_nSampleCount < MAX_SAMPLE_COUNT) m_nSampleCount++;
    }

	m_nFramesPerSecond++;
	m_fFPSTimeElapsed += fTimeElapsed;
	if (m_fFPSTimeElapsed > 1.0f) 
    {
		m_nCurrentFrameRate	= m_nFramesPerSecond;
		m_nFramesPerSecond = 0;
		m_fFPSTimeElapsed = 0.0f;
	} 

    m_fTimeElapsed = 0.0f;
    for (ULONG i = 0; i < m_nSampleCount; i++) m_fTimeElapsed += m_fFrameTime[i];
    if (m_nSampleCount > 0) m_fTimeElapsed /= m_nSampleCount;
	return true;
}

unsigned long CTimer::GetFrameRate(LPTSTR lpszString, int nCharacters) 
{
    if (lpszString)
    {
		*lpszString = L'(';
        _itow_s(m_nCurrentFrameRate, lpszString + 1, nCharacters, 10);
        wcscat_s(lpszString, nCharacters, _T(" FPS)"));
    } 

    return(m_nCurrentFrameRate);
}

float CTimer::GetTimeElapsed() 
{
    return(m_fTimeElapsed);
}

float CTimer::GetTotalTime()
{
	if (m_bStopped) return(float(((m_nStopPerformanceCounter - m_nPausedPerformanceCounter) - m_nBasePerformanceCounter) * m_fTimeScale));
	return(float(((m_nCurrentPerformanceCounter - m_nPausedPerformanceCounter) - m_nBasePerformanceCounter) * m_fTimeScale));
}

void CTimer::Reset()
{
	__int64 nPerformanceCounter;
	::QueryPerformanceCounter((LARGE_INTEGER*)&nPerformanceCounter);

	m_nBasePerformanceCounter = nPerformanceCounter;
	m_nLastPerformanceCounter = nPerformanceCounter;
	m_nStopPerformanceCounter = 0;
	m_bStopped = false;
}

void CTimer::Start()
{
	__int64 nPerformanceCounter;
	::QueryPerformanceCounter((LARGE_INTEGER *)&nPerformanceCounter);
	if (m_bStopped)
	{
		m_nPausedPerformanceCounter += (nPerformanceCounter - m_nStopPerformanceCounter);
		m_nLastPerformanceCounter = nPerformanceCounter;
		m_nStopPerformanceCounter = 0;
		m_bStopped = false;
	}
}

void CTimer::Stop()
{
	if (!m_bStopped)
	{
		::QueryPerformanceCounter((LARGE_INTEGER *)&m_nStopPerformanceCounter);
		m_bStopped = true;
	}
}

CDebugTimer::CDebugTimer()
{
	::QueryPerformanceFrequency((LARGE_INTEGER *)&m_nPerformanceFrequencyPerSec);
	::QueryPerformanceCounter((LARGE_INTEGER *)&m_nLastPerformanceCounter);
	m_fTimeScalePerSec = 1.0 / (double)m_nPerformanceFrequencyPerSec;
	m_fTimeScalePerMillisec = 1000.0 / (double)m_nPerformanceFrequencyPerSec;
	m_fTimeScalePerMicrosec = 1000000.0 / (double)m_nPerformanceFrequencyPerSec;
}

CDebugTimer::~CDebugTimer()
{
}

void CDebugTimer::Start()
{
	::QueryPerformanceCounter((LARGE_INTEGER *)&m_nLastPerformanceCounter);
}

void CDebugTimer::End()
{
	::QueryPerformanceCounter((LARGE_INTEGER *)&m_nCurrentPerformanceCounter);
	m_nIntervalPerformanceCounter = m_nCurrentPerformanceCounter - m_nLastPerformanceCounter;
	m_nLastPerformanceCounter = m_nCurrentPerformanceCounter;
}

void CDebugTimer::PrintSecondInterval(const char* target_name)
{
	printf("[%s]\n execution time: [%d sec] - [%d clock]\n\n"
		, target_name
		, (int)(m_nIntervalPerformanceCounter * m_fTimeScalePerSec)
		, (int)m_nIntervalPerformanceCounter
	);
}

void CDebugTimer::PrintMilliSecInterval(const char* target_name)
{
	printf("[%s]\n execution time: [%d millisec] - [%d clock]\n\n"
		, target_name
		, (int)(m_nIntervalPerformanceCounter * m_fTimeScalePerMillisec)
		, (int)m_nIntervalPerformanceCounter
	);
}

void CDebugTimer::PrintMicroSecInterval(const char* target_name)
{
	printf("[%s]\n execution time: [%d microsec] - [%d clock]\n\n"
		, target_name
		, (int)(m_nIntervalPerformanceCounter * m_fTimeScalePerMicrosec)
		, (int)m_nIntervalPerformanceCounter
	);
}