#undef UNICODE
#include "WinBase.h"
#include "TimeMgr.h"
#include "ImgMgr.h"

WinBase	*g_pMainWindow = nullptr;

//	콜백함수 : 윈도우가 대신 호출해준다.
LRESULT	CALLBACK MyWndProc(HWND _hWnd, UINT _message, WPARAM _wParam, LPARAM _lParam)
{
	return g_pMainWindow->WndProc(_hWnd, _message, _wParam, _lParam);
}


VOID WinBase::RegiWinClass()
{
	//	1. 윈도우 클래스 정의 및 등록
	WNDCLASS	wndClass = {};

	//	정보입력
	wndClass.lpszClassName = m_ClassName.c_str();
	wndClass.hInstance = m_hInst;
	wndClass.lpfnWndProc = MyWndProc;							//	함수포인터

	wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);	//	배경색
	wndClass.hCursor = LoadCursor(nullptr, IDC_ARROW);		//	커서모양
	wndClass.hIcon = LoadIcon(nullptr, IDI_APPLICATION);	//	아이콘

	RegisterClass(&wndClass);
}
BOOL WinBase::MakingWindow()
{
	g_pMainWindow = this;

	//	실제 윈도우 크기 구하기
	int		wsOption = WS_OVERLAPPEDWINDOW;
	RECT	rt = { 0, 0, m_WinSize.cx, m_WinSize.cy };

	//	윈도우 기본 옵션에 따른 실제 기본창 크기
	AdjustWindowRect(&rt, wsOption, FALSE);

	//	2. 윈도우 생성 및 화면 띄우기
	m_hWnd = CreateWindow(
		m_ClassName.c_str(),		//	위의 윈도우클래스 이름
		m_WinName.c_str(),			//	윈도우 타이틀
		wsOption,					//	윈도우 기본 옵션( 캡션바, 최대화버튼, 최소버튼. )
		0, 0,					//	윈도우 시작 위치
		rt.right - rt.left, rt.bottom - rt.top,	//	윈도우 크기
		nullptr,					//	부모윈도우핸들
		nullptr,					//	메뉴핸들
		m_hInst,					//	연결할 프로세스핸들
		nullptr);					//	무쓸모...

	if (m_hWnd == nullptr)
	{
		g_pMainWindow = nullptr;
		return FALSE;
	}

	//	초기화
	Init(m_hWnd);

	ShowWindow(m_hWnd, SW_SHOW);

	return TRUE;
}

LRESULT WinBase::WndProc(HWND _hWnd, UINT _message, WPARAM _wParam, LPARAM _lParam)
{
	switch (_message)
	{
	case WM_KEYDOWN:
	{
		switch (_wParam)
		{
		case VK_ESCAPE:
			PostQuitMessage(0);
			break;

		default:
			break;
		}

	}break;
	
	case WM_DESTROY:
	{
		PostQuitMessage(0);
	}break;

	}

	return DefWindowProc(_hWnd, _message, _wParam, _lParam);
}

VOID WinBase::Init(HWND _hWnd)
{
	return VOID();
}

VOID WinBase::Release()
{
	return VOID();
}

VOID WinBase::Update(float _elapseTime)
{
	return VOID();
}

VOID WinBase::Render(HDC _hdc)
{
	return VOID();
}


int WinBase::MessageLoop()
{

	MSG	msg = {};

	while (true)
	{
		if (PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
		{
			//	탈출조건
			if (msg.message == WM_QUIT) break;

			//	메시지가 발생한 경우
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			Update(TIME.Tick());

			ImgClass	back(m_hWnd, m_WinSize.cx, m_WinSize.cy);
			//	새로운 DC에 이미지를 그린다.
			Render(back.GetImgDC());
			//	화면 hdc 에 백버퍼 이미지를 그린다.
			back.Render(m_hWnd);
		}
	}

	//	Release
	Release();

	return 0;
}
BOOL WinBase::NewWindow(HINSTANCE _hInst, int _width, int _height, LPCSTR _winName)
{
	m_hInst = _hInst;

	m_WinSize.cx = _width;
	m_WinSize.cy = _height;

	m_ClassName = _winName;
	m_WinName = _winName;

	//	1. 윈도우 클래스 정보 입력 및 등록
	RegiWinClass();

	//	2. 윈도우 생성
	if (MakingWindow() == FALSE)
	{
		return FALSE;
	}

	return TRUE;
}