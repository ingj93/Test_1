#include "Global.h"
#include "MyWin.h"

int APIENTRY WinMain(
	HINSTANCE	_hInst,			//	���μ��� �ڵ�
	HINSTANCE	_hPrevInst,		//	���� ���μ��� �ڵ�( ���� �Ⱦ� )
	LPSTR		_CmdParam,		//	����� �μ�( char* )
	int			_CmdShow)		//	�⺻â ����
{
	MyWin	main;
	
	//	������ �����ϸ� ���� ����
	if (main.NewWindow(_hInst, WIN_WIDTH, WIN_HEIGHT, WIN_NAME) == FALSE)
	{
		return 0;
	}

	return main.MessageLoop();
}