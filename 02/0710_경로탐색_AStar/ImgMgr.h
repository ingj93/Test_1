#pragma once

#include <map>
#include <string>
#include <Windows.h>
#include "ImgClass.h"

#include "TemplateSingletonB.h"

class ImgMgr 
	: public TemplateSingletonB<ImgMgr>
{
	friend TemplateSingletonB;
private:
	HWND									m_hWnd;
	std::map< std::string, ImgClass* >		m_imgList;

private:
	ImgMgr() = default;
	~ImgMgr();

	std::string	GetFileName(std::string _path);

public:
	void	SethWnd(HWND _hWnd)
	{
		m_hWnd = _hWnd;
	}

	bool		OpenImg(std::string _path, int _width = 0, int _height = 0);
	ImgClass*	GetImg(std::string _name);
};

#define IMG_MGR		ImgMgr::GetInstance()

