#pragma once

#include <Windows.h>

#include <vector>
#include <initializer_list>

class AniInfo
{
	float				m_time = 0;		//	현재 경과 시간
	std::vector<float>	m_frameTime;
public:
	
	bool				m_isAni		= false;	//	애니메이션 진행 여부
	int					m_nowIndex	= 0;		//	현재 프레임
	bool				m_isLoop	= false;	//	반복 여부

	void	SetFrameTime(std::initializer_list<float> _list);
	void	Update(float _elapseTime);
	void	Start();
	void	Stop();
};



class ImgClass
{
private:
	HDC			m_imgDC;
	HBITMAP		m_bit, m_oldBit;
	SIZE		m_size;

	BOOL		m_isTrans;
	COLORREF	m_colorkey;

	INT			m_opacity;

public:
	ImgClass() = default;
	ImgClass(HWND _hWnd, int _width, int _height);
	ImgClass(HDC _hdc, int _width, int _height);
	~ImgClass();

	//	생성
	bool	CreateImg(HWND _hWnd, int _width, int _height);
	bool	CreateImg(HDC _hdc, int _width, int _height);
	bool	LoadImg(HWND _hWnd, LPCSTR _path, int _width = 0, int _height = 0);

	//	설정
	void	SetTrans(BOOL _isTrans = TRUE, COLORREF _colorkey = RGB(255, 0, 255));
	void	SetOpacity(INT _opacity);
	INT		GetOpacity()
	{
		return m_opacity;
	}

	void	SetFrame(int _frame)
	{
		m_size.cx /= _frame;
	}

	//	랜더
	void	Render(HDC _destDC, int _posX = 0, int _posY = 0, int _width = 0, int _height = 0 );
	void	Render(HWND _hWnd);

	//	ani
	void	AniRender(HDC _destDC, int _frameX, int _posX = 0, int _posY = 0);

	//	Scroll
	void	ScrollRender(HDC _destDC, int _posX, int _posY, int _offset);

	//	수정
	void	Clear(COLORREF _color = RGB(255,255,255) );

	//	DC 얻기
	HDC		GetImgDC()
	{
		return m_imgDC;
	}
	SIZE	GetSize()
	{
		return m_size;
	}
};

