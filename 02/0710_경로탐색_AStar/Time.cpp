#include "Time.h"

#include <Windows.h>
#pragma comment(lib, "winmm.lib")

Time::Time()
{
	Initialize();
}
Time::~Time()
{
	Release();
}

void Time::Initialize()
{
	LARGE_INTEGER	periodRrequency = {};
	m_isHighTimer = QueryPerformanceFrequency(&periodRrequency);

	if (m_isHighTimer == true)
	{
		QueryPerformanceCounter((LARGE_INTEGER*)&m_lastTime);
		m_timeResolusion = 1.0f / periodRrequency.QuadPart;
	}
	else
	{
		//	해상도 설정
		timeBeginPeriod(1);

		m_lastTime = timeGetTime();
		m_timeResolusion = 0.001f;
	}
}

void Time::Release()
{
	timeEndPeriod(1);
}

float Time::Tick()
{
	do
	{
		//	현재 시간
		if( m_isHighTimer )
			QueryPerformanceCounter((LARGE_INTEGER*)&m_nowTime);
		else
			m_nowTime		= timeGetTime();

		//	경과 시간 계산
		m_timeElapsed	= (m_nowTime - m_lastTime) * m_timeResolusion;

	} while (m_timeElapsed <= m_lockTime);

	//	마지막 시간 저장
	m_lastTime			= m_nowTime;
	//	FPSCount 증가
	m_FPSCount++;
	//	FPS 경과시간 증가
	m_FPSTimeElapsed	+= m_timeElapsed;
	//	플레이 시간 증가
	m_playTime			+= m_timeElapsed;

	//	1초 경과에 대한 값 설정
	if (m_FPSTimeElapsed > 1.0f)
	{
		m_FPS				= m_FPSCount;
		m_FPSCount			= 0;
		m_FPSTimeElapsed	-= 1.0f;
	}

	return m_timeElapsed;
}
