#pragma once

#include "TemplateSingletonA.h"
#include <Windows.h>

class Time;

class TimeMgr 
	: public TemplateSingletonA<TimeMgr>
{
	friend TemplateSingletonA;
private:
	Time	*m_timer = nullptr;

private:
	TimeMgr();
	~TimeMgr();

public:
	//	프레임 유지
	float Tick();
	//	FPS 설정
	unsigned long	GetFPS();
	void			SetFPS(unsigned long _lockFPS);

	//	경과시간 얻기
	float			GetElapsedTime();
	//	플레이 시간 얻기
	float			GetPlayTime();
	//	화면 FPS 출력
	void			Render(HDC _hdc, int _x, int _y);
};

#define		TIME	TimeMgr::GetInstance()