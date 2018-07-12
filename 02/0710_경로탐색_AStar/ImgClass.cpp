#undef UNICODE
#include "ImgClass.h"

void	AniInfo::SetFrameTime(std::initializer_list<float> _list)
{
	for (auto &i : _list)
	{
		m_frameTime.push_back(i);
	}
}
void	AniInfo::Update(float _elapseTime)
{
	if (m_isAni == false) return;

	m_time += _elapseTime;

	if (m_frameTime[m_nowIndex] <= m_time)
	{
		m_time -= m_frameTime[m_nowIndex];

		m_nowIndex++;

		//	엔드 프레임
		if (m_nowIndex + 1 == m_frameTime.size())
		{
			if (m_isLoop == false)
			{
				m_isAni = false;
				m_time = 0;
			}
			else
			{
				m_nowIndex = 0;
			}
		}
	}
}
void	AniInfo::Start()
{
	m_isAni = true;
	m_nowIndex = 0;
}
void	AniInfo::Stop()
{
	m_isAni = false;
	m_nowIndex = 0;
}

/////////////////////////////////////////////////////////////////////////

#include <windowsx.h>

ImgClass::ImgClass(HWND _hWnd, int _width, int _height)
{
	CreateImg(_hWnd, _width, _height);
	Clear();
}

ImgClass::ImgClass(HDC _hdc, int _width, int _height)
{
	CreateImg(_hdc, _width, _height);
	Clear();
}

ImgClass::~ImgClass()
{
	DeleteObject(SelectBitmap(m_imgDC, m_oldBit));
	DeleteDC(m_imgDC);
}

bool ImgClass::CreateImg(HWND _hWnd, int _width, int _height)
{
	if (_hWnd	== nullptr	||
		_width	<= 0		||
		_height <= 0)
	{
		return false;
	}

	HDC hdc = GetDC(_hWnd);

	m_imgDC		= CreateCompatibleDC(hdc);
	m_bit		= CreateCompatibleBitmap(hdc, _width, _height);
	m_oldBit	= SelectBitmap(m_imgDC, m_bit);

	ReleaseDC(_hWnd, hdc);

	m_size.cx = _width;
	m_size.cy = _height;

	return true;
}

bool ImgClass::CreateImg(HDC _hdc, int _width, int _height)
{
	if (_hdc == nullptr ||
		_width <= 0		||
		_height <= 0)
	{
		return false;
	}

	m_imgDC = CreateCompatibleDC(_hdc);
	m_bit = CreateCompatibleBitmap(_hdc, _width, _height);
	m_oldBit = SelectBitmap(m_imgDC, m_bit);

	m_size.cx = _width;
	m_size.cy = _height;

	return true;
}

bool ImgClass::LoadImg(HWND _hWnd, LPCSTR _path, int _width, int _height)
{
	if (_hWnd	==	nullptr ||
		_width	< 0			||
		_height	< 0)
	{
		return false;
	}

	HDC hdc		= GetDC(_hWnd);

	m_imgDC		= CreateCompatibleDC(hdc);
	m_bit		= (HBITMAP)LoadImage(nullptr, _path, IMAGE_BITMAP, _width, _height, LR_LOADFROMFILE);
	m_oldBit	= SelectBitmap(m_imgDC, m_bit);

	ReleaseDC(_hWnd, hdc);

	//	크기를 0, 0 으로 넣었을 경우 생성된 정보에서 가로세로를 구한다.
	if (_width == 0 && _height == 0)
	{
		BITMAP	bm = {};
		GetObject(m_bit, sizeof(BITMAP), &bm);

		_width	= bm.bmWidth;
		_height = bm.bmHeight;
	}
	
	m_size.cx = _width;
	m_size.cy = _height;

	m_isTrans = FALSE;
	m_opacity = 255;

	return true;
}

void ImgClass::SetTrans(BOOL _isTrans, COLORREF _colorkey)
{
	m_isTrans	= _isTrans;
	m_colorkey	= _colorkey;
}

void ImgClass::SetOpacity(INT _opacity)
{
	m_opacity = _opacity;

	if (m_opacity > 255)	m_opacity = 255;
	if (m_opacity < 0)		m_opacity = 0;
}

void ImgClass::Render(HDC _destDC, int _posX, int _posY, int _width, int _height)
{
	if (_width == 0 || _height == 0)
	{
		_width	= m_size.cx;
		_height = m_size.cy;
	}

	if (m_isTrans == TRUE)
	{
		if (m_opacity == 255)
		{
			GdiTransparentBlt(_destDC, _posX, _posY, _width, _height, m_imgDC, 0, 0, m_size.cx, m_size.cy, m_colorkey);
		}
		else
		{
			//	1. 빈 종이 만들기
			ImgClass	empty(_destDC, _width, _height);
			//	2. 배경을 복사
			BitBlt(empty.GetImgDC(), 0, 0, _width, _height, _destDC, _posX, _posY, SRCCOPY);
			//	3. 빈 종이에 Transp....
			GdiTransparentBlt(empty.GetImgDC(), 0, 0, _width, _height, m_imgDC, 0, 0, m_size.cx, m_size.cy, m_colorkey);
			
			//	4. 배경에 빈종이를 Alpha
			BLENDFUNCTION	bf = {};
			bf.SourceConstantAlpha = m_opacity;

			GdiAlphaBlend(_destDC, _posX, _posY, _width, _height, empty.GetImgDC(), 0, 0, _width, _height, bf);
		}
	}
	else
	{
		if (m_opacity == 255)
		{
			StretchBlt(_destDC, _posX, _posY, _width, _height, m_imgDC, 0, 0, m_size.cx, m_size.cy, SRCCOPY);
		}
		else
		{
			BLENDFUNCTION	bf = {};
			bf.SourceConstantAlpha = m_opacity;

			GdiAlphaBlend(_destDC, _posX, _posY, _width, _height, m_imgDC, 0, 0, m_size.cx, m_size.cy, bf);
		}
	}
}

void ImgClass::Render(HWND _hWnd)
{
	HDC hdc = GetDC(_hWnd);

	BitBlt(hdc, 0, 0, m_size.cx, m_size.cy, m_imgDC, 0, 0, SRCCOPY);

	ReleaseDC(_hWnd, hdc);
}

void ImgClass::AniRender(HDC _destDC, int _frameX, int _posX, int _posY)
{
	if (m_isTrans == TRUE)
	{
		if (m_opacity == 255)
		{
			GdiTransparentBlt(_destDC, _posX, _posY, m_size.cx, m_size.cy, m_imgDC, m_size.cx * _frameX, 0, m_size.cx, m_size.cy, m_colorkey);
		}
		else
		{
			//	1. 빈 종이 만들기
			ImgClass	empty(_destDC, m_size.cx, m_size.cy);
			//	2. 배경을 복사
			BitBlt(empty.GetImgDC(), 0, 0, m_size.cx, m_size.cy, _destDC, _posX, _posY, SRCCOPY);
			//	3. 빈 종이에 Transp....
			GdiTransparentBlt(empty.GetImgDC(), 0, 0, m_size.cx, m_size.cy, m_imgDC, m_size.cx * _frameX, 0, m_size.cx, m_size.cy, m_colorkey);

			//	4. 배경에 빈종이를 Alpha
			BLENDFUNCTION	bf = {};
			bf.SourceConstantAlpha = m_opacity;

			GdiAlphaBlend(_destDC, _posX, _posY, m_size.cx, m_size.cy, empty.GetImgDC(), 0, 0, m_size.cx, m_size.cy, bf);
		}
	}
	else
	{
		if (m_opacity == 255)
		{
			StretchBlt(_destDC, _posX, _posY, m_size.cx, m_size.cy, m_imgDC, m_size.cx * _frameX, 0, m_size.cx, m_size.cy, SRCCOPY);
		}
		else
		{
			BLENDFUNCTION	bf = {};
			bf.SourceConstantAlpha = m_opacity;

			GdiAlphaBlend(_destDC, _posX, _posY, m_size.cx, m_size.cy, m_imgDC, m_size.cx * _frameX, 0, m_size.cx, m_size.cy, bf);
		}
	}
}

void ImgClass::ScrollRender(HDC _destDC, int _posX, int _posY, int _offset)
{
	static int offset = 0;

	offset += _offset;

	if (offset > m_size.cx)	offset -= m_size.cx;
	if (offset < 0)			offset += m_size.cx;

	BitBlt(_destDC, _posX			, _posY, offset				, m_size.cy, m_imgDC, m_size.cx - offset, 0, SRCCOPY);
	BitBlt(_destDC, _posX + offset	, _posY, m_size.cx - offset	, m_size.cy, m_imgDC, 0					, 0, SRCCOPY);
}

void ImgClass::Clear(COLORREF _color)
{
	HBRUSH brush	= CreateSolidBrush(_color);

	RECT rc = { 0, 0, m_size.cx, m_size.cy };
	FillRect(m_imgDC, &rc, brush);

	DeleteObject(brush);
}
