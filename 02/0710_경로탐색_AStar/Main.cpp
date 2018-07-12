#include "Global.h"
#include "MyWin.h"

int APIENTRY WinMain(
	HINSTANCE	_hInst,			//	프로세스 핸들
	HINSTANCE	_hPrevInst,		//	이전 프로세스 핸들( 현재 안씀 )
	LPSTR		_CmdParam,		//	명령줄 인수( char* )
	int			_CmdShow)		//	기본창 상태
{
	MyWin	main;
	
	//	생성에 실패하면 리턴 종료
	if (main.NewWindow(_hInst, WIN_WIDTH, WIN_HEIGHT, WIN_NAME) == FALSE)
	{
		return 0;
	}

	return main.MessageLoop();
}