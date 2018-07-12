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
	//	1. ������Ŭ���� �����Է� �� ���
	VOID	RegiWinClass();
	//	2. ������ ����
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
	//	�޽��� ���ν���
	virtual LRESULT	WndProc(HWND _hWnd, UINT _message, WPARAM _wParam, LPARAM _lParam);
public:
	virtual ~WinBase() = default;
	
	BOOL	NewWindow(HINSTANCE _hInst, int _width, int _height, LPCSTR _winName);
	//	3. �޽��� ����
	int		MessageLoop();
};