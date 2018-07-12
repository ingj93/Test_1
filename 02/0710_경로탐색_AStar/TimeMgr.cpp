#undef UNICODE
#include "TimeMgr.h"
#include "Time.h"
#include <sstream>

TimeMgr::TimeMgr()
{
	m_timer = new Time();
}

TimeMgr::~TimeMgr()
{
	delete m_timer;
	m_timer;
}

float TimeMgr::Tick()
{
	return m_timer->Tick();
}

unsigned long TimeMgr::GetFPS()
{
	return m_timer->GetFPS();
}

void TimeMgr::SetFPS(unsigned long _lockFPS)
{
	m_timer->SetFPS(_lockFPS);
}

float TimeMgr::GetElapsedTime()
{
	return m_timer->GetElapsedTime();
}

float TimeMgr::GetPlayTime()
{
	return m_timer->GetPlayTime();
}

void TimeMgr::Render(HDC _hdc, int _x, int _y)
{
	std::stringstream	text;

	text << "FPS : " << m_timer->GetFPS();
	TextOut(_hdc, _x, _y, text.str().c_str(), text.str().length());
	text.str("");

	text << "PlayTime : " << m_timer->GetPlayTime();
	TextOut(_hdc, _x, _y + 20, text.str().c_str(), text.str().length());
	text.str("");
}
