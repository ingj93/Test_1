#pragma once

class Time
{
	bool			m_isHighTimer		= false;			//	고해상도 하드웨어 타이머 지원 여부
	float			m_timeResolusion	= 0.0f;				//	시간 해상도 ( timeGetTime == 1/1000 초 )
	float			m_timeElapsed		= 0.0f;				//	경과 시간
	__int64			m_lastTime			= 0;				//	지난 시간
	__int64			m_nowTime			= 0;				//	현재 시간

	unsigned long	m_FPS				= 0;				//	Frame Per Second
	unsigned long	m_FPSCount			= 0;				//	1초 동안의 카운팅용
	float			m_FPSTimeElapsed	= 0.0f;				//	FPS 체크용 1초 경과시간 누적용
	
	unsigned long	m_lockFPS			= 60;				//	기본 설정 FPS
	float			m_lockTime			= 1.0f / m_lockFPS;	//	1 프레임의 시간

	float			m_playTime			= 0.0f;

private:
	void	Initialize();
	void	Release();

public:
	Time();
	~Time();

	//	프레임 유지
	float Tick();

	//	FPS 얻기
	unsigned long	GetFPS()
	{
		return m_FPS;
	}
	//	FPS 설정
	void	SetFPS(unsigned long _lockFPS)
	{
		if (_lockFPS == 0 || _lockFPS == m_lockFPS)
			return;

		m_lockFPS	= _lockFPS;
		m_lockTime	= 1.0f / m_lockFPS;
	}

	//	경과시간 얻기
	float			GetElapsedTime()
	{
		return m_timeElapsed;
	}
	//	플레이 시간 얻기
	float			GetPlayTime()
	{
		return m_playTime;
	}
};

