#undef UNICODE
#include "WinBase.h"
#include "TimeMgr.h"
#include "ImgMgr.h"

WinBase	*g_pMainWindow = nullptr;

//	�ݹ��Լ� : �����찡 ��� ȣ�����ش�.
LRESULT	CALLBACK MyWndProc(HWND _hWnd, UINT _message, WPARAM _wParam, LPARAM _lParam)
{
	return g_pMainWindow->WndProc(_hWnd, _message, _wParam, _lParam);
}


VOID WinBase::RegiWinClass()
{
	//	1. ������ Ŭ���� ���� �� ���
	WNDCLASS	wndClass = {};

	//	�����Է�
	wndClass.lpszClassName = m_ClassName.c_str();
	wndClass.hInstance = m_hInst;
	wndClass.lpfnWndProc = MyWndProc;							//	�Լ�������

	wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);	//	����
	wndClass.hCursor = LoadCursor(nullptr, IDC_ARROW);		//	Ŀ�����
	wndClass.hIcon = LoadIcon(nullptr, IDI_APPLICATION);	//	������

	RegisterClass(&wndClass);
}
BOOL WinBase::MakingWindow()
{
	g_pMainWindow = this;

	//	���� ������ ũ�� ���ϱ�
	int		wsOption = WS_OVERLAPPEDWINDOW;
	RECT	rt = { 0, 0, m_WinSize.cx, m_WinSize.cy };

	//	������ �⺻ �ɼǿ� ���� ���� �⺻â ũ��
	AdjustWindowRect(&rt, wsOption, FALSE);

	//	2. ������ ���� �� ȭ�� ����
	m_hWnd = CreateWindow(
		m_ClassName.c_str(),		//	���� ������Ŭ���� �̸�
		m_WinName.c_str(),			//	������ Ÿ��Ʋ
		wsOption,					//	������ �⺻ �ɼ�( ĸ�ǹ�, �ִ�ȭ��ư, �ּҹ�ư. )
		0, 0,					//	������ ���� ��ġ
		rt.right - rt.left, rt.bottom - rt.top,	//	������ ũ��
		nullptr,					//	�θ��������ڵ�
		nullptr,					//	�޴��ڵ�
		m_hInst,					//	������ ���μ����ڵ�
		nullptr);					//	������...

	if (m_hWnd == nullptr)
	{
		g_pMainWindow = nullptr;
		return FALSE;
	}

	//	�ʱ�ȭ
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
			//	Ż������
			if (msg.message == WM_QUIT) break;

			//	�޽����� �߻��� ���
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			Update(TIME.Tick());

			ImgClass	back(m_hWnd, m_WinSize.cx, m_WinSize.cy);
			//	���ο� DC�� �̹����� �׸���.
			Render(back.GetImgDC());
			//	ȭ�� hdc �� ����� �̹����� �׸���.
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

	//	1. ������ Ŭ���� ���� �Է� �� ���
	RegiWinClass();

	//	2. ������ ����
	if (MakingWindow() == FALSE)
	{
		return FALSE;
	}

	return TRUE;
}