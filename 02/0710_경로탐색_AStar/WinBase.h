#pragma once

#include <Windows.h>
#include <string>

class WinBase
{
protected:
	std::string		m_WinName;
	std::string		m_ClassName;
	SIZE			m_WinSize;

	HINSTANCE		m_hInst;
	HWND			m_hWnd;

private:
	//	1. 윈도우클래스 정보입력 및 등록
	VOID	RegiWinClass();
	//	2. 윈도우 생성
	BOOL	MakingWindow();
	//	friend
	friend	LRESULT	CALLBACK MyWndProc(HWND, UINT, WPARAM, LPARAM);
protected:
	//	Init
	virtual VOID	Init(HWND _hWnd);
	//	Release
	virtual VOID	Release();
	//	Update
	virtual VOID	Update(float _elapseTime);
	//	Render
	virtual VOID	Render(HDC _hdc);
	//	메시지 프로시저
	virtual LRESULT	WndProc(HWND _hWnd, UINT _message, WPARAM _wParam, LPARAM _lParam);
public:
	virtual ~WinBase() = default;
	
	BOOL	NewWindow(HINSTANCE _hInst, int _width, int _height, LPCSTR _winName);
	//	3. 메시지 루프
	int		MessageLoop();
};