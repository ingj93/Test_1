#include "ImgMgr.h"
#include "ImgClass.h"

ImgMgr::~ImgMgr()
{
	for (auto &i : m_imgList)
	{
		delete i.second;
	}
	m_imgList.clear();
}

std::string ImgMgr::GetFileName(std::string _path)
{
	//	마지막 / or \\ 위치를 찾는다.
	int start = _path.find_last_of("/\\") + 1;
	int end = _path.find_last_of('.');

	return _path.substr(start, end - start);
}

bool ImgMgr::OpenImg(std::string _path, int _width, int _height)
{
	//	경로에서 파일 이름 얻기
	std::string name = GetFileName(_path);
	//	파일 이름을 찾기
	auto find = m_imgList.find(name);
	//	있으면 성공 반환
	if (find != m_imgList.end())return true;

	//	핸들 정보 존재 여부
	if (m_hWnd == nullptr)	return false;

	//	이미지 생성
	ImgClass *newImg = new ImgClass;
	bool isLoad = newImg->LoadImg(m_hWnd, _path.c_str(), _width, _height);
	if (isLoad == false)	return false;

	//	이미지 리스트에 넣기
	m_imgList.insert(std::make_pair(name, newImg));
	
	return true;
}

ImgClass*	ImgMgr::GetImg(std::string _name)
{
	auto find = m_imgList.find(_name);

	if(find == m_imgList.end() )return nullptr;

	return find->second;
}
