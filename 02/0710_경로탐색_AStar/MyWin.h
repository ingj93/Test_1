#pragma once

#include "Global.h"
#include "WinBase.h"
#include "TimeMgr.h"
#include "ImgMgr.h"
#include <list>
using namespace std;

struct	TileNode
{
	//	부모 노드
	TileNode	*parentNode = nullptr;

	//	타일 좌표
	POINT		pos = {};

	//	F,G,H
	int			F = 0;
	int			G = 0;
	int			H = 0;

	//	sort 사용을 위한 연산자 정의
	bool	operator < (TileNode &_right)
	{
		return F < _right.F;
	}
};

class MyWin :
	public WinBase
{
private:
	bool	m_isFocus		= true;
	bool	m_isView		= false;
	bool	m_isImg			= false;
	POINT	m_MousePos		= {};
	POINT	m_ClickPos		= {};
	POINT	m_CamPos		= {};
	SIZE	m_BgSize		= {};
	

	POINT	m_TileToStart	= { 100, 100 };	//	화면에 표시할 타일맵의 좌표
	SIZE	m_CellSize		= {};			//	타일 한칸의 사이즈
	int		m_nx			= 0;			//	배치할 타일의 x개수
	int		m_ny			= 0;			//	배치할 타일의 y개수

	int		*m_pMap			= nullptr;		//	맵정보
	int		m_size			= 0;
	int		m_drag			= 0;

///////////////////////////////////////////////////
	POINT		m_start		= {};		//	start
	POINT		m_end		= {};		//	end
	list<POINT>	m_pathList;

private:
	//	Init
	VOID	Init(HWND _hWnd);
	//	Release
	VOID	Release();
	//	갱신
	VOID	Update(float _elapseTime);
	//	그리기
	VOID	Render(HDC _hdc);
	//	메시지
	LRESULT WndProc(HWND _hWnd, UINT _message, WPARAM _wParam, LPARAM _lParam);
public:
	MyWin();
	~MyWin();

	//	맵 변경
	VOID	TransMap(int _index);
	VOID	GetStartEnd();
	bool	FindPath_AStar();
};
