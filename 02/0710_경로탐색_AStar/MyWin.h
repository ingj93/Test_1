#pragma once

#include "Global.h"
#include "WinBase.h"
#include "TimeMgr.h"
#include "ImgMgr.h"
#include <list>
using namespace std;

struct	TileNode
{
	//	�θ� ���
	TileNode	*parentNode = nullptr;

	//	Ÿ�� ��ǥ
	POINT		pos = {};

	//	F,G,H
	int			F = 0;
	int			G = 0;
	int			H = 0;

	//	sort ����� ���� ������ ����
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
	

	POINT	m_TileToStart	= { 100, 100 };	//	ȭ�鿡 ǥ���� Ÿ�ϸ��� ��ǥ
	SIZE	m_CellSize		= {};			//	Ÿ�� ��ĭ�� ������
	int		m_nx			= 0;			//	��ġ�� Ÿ���� x����
	int		m_ny			= 0;			//	��ġ�� Ÿ���� y����

	int		*m_pMap			= nullptr;		//	������
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
	//	����
	VOID	Update(float _elapseTime);
	//	�׸���
	VOID	Render(HDC _hdc);
	//	�޽���
	LRESULT WndProc(HWND _hWnd, UINT _message, WPARAM _wParam, LPARAM _lParam);
public:
	MyWin();
	~MyWin();

	//	�� ����
	VOID	TransMap(int _index);
	VOID	GetStartEnd();
	bool	FindPath_AStar();
};
