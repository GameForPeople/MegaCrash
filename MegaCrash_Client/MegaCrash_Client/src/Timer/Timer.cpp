//-----------------------------------------------------------------------------
// File: Timer.cpp
//-----------------------------------------------------------------------------

#include "stdafx.h"
#include "Timer.h"

CTimer::CTimer()
	: m_tpStartTime{}
	, m_tpStopTime{}
	, m_tpPauseTime{}
	, m_tpCurrentTime{}
	, m_tpLastTime{}
	, m_bStopped{ false }
{
	m_tpStartTime = m_tpLastTime = high_resolution_clock::now();

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
	static float fTimeElapsed;
	if (m_bStopped)
	{
		m_fTimeElapsed = 0.0f;
		return false;
	}
	m_tpCurrentTime = high_resolution_clock::now();
	m_DurationTime = m_tpCurrentTime - m_tpLastTime;
	fTimeElapsed = m_DurationTime.count();
	if (fTimeElapsed < MAX_FPS) return false;

	m_tpLastTime = m_tpCurrentTime;

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

unsigned long CTimer::GetFrameRate() 
{
    return(m_nCurrentFrameRate);
}

float CTimer::GetTimeElapsed() 
{
    return(m_fTimeElapsed);
}

float CTimer::GetTotalTime()
{
	duration<float> TotalTime;
	if (m_bStopped)
		TotalTime = m_tpStopTime.time_since_epoch() - m_tpPauseTime.time_since_epoch() - m_tpStartTime.time_since_epoch();
	else 
		TotalTime = m_tpCurrentTime.time_since_epoch() - m_tpPauseTime.time_since_epoch() - m_tpStartTime.time_since_epoch();
	return TotalTime.count();
}

void CTimer::Reset()
{
	m_tpLastTime = m_tpStartTime = high_resolution_clock::now();
	m_tpStopTime = time_point<steady_clock>{};
	m_bStopped = false;
}

void CTimer::Start()
{
	if (m_bStopped)
	{
		m_tpLastTime = high_resolution_clock::now();
		m_tpPauseTime += (m_tpLastTime - m_tpStopTime);
		m_tpStopTime = time_point<steady_clock>{};
		m_bStopped = false;
	}
}

void CTimer::Stop()
{
	if (!m_bStopped)
	{
		m_tpStopTime = high_resolution_clock::now();
		m_bStopped = true;
	}
}