#include "MyWin.h"
#include <sstream>

VOID MyWin::Init(HWND _hWnd)
{
	m_BgSize.cx = WIN_WIDTH * 2 / 3;	//	���� ȭ���� 2/3
	m_BgSize.cy = WIN_HEIGHT * 4 / 5;	//	���� ȭ���� 3/4
	m_isImg		= false;	//	�̹��� ���
	m_isView	= true;		//	��ǥ ���

	int	size = 100;
	m_CellSize.cx = size;	//	���� �ʺ�
	m_CellSize.cy = size;	//	���� ����

	m_nx = 20;				//	Ÿ���� ���� ����
	m_ny = 20;				//	Ÿ���� ���� ����

	
	m_size = m_nx * m_ny;

	//  �����Ҵ�
	if (m_pMap == nullptr)
	{
		m_pMap = new int[m_size];
	}
	for (int i = 0; i < m_size; i++)
	{
		m_pMap[i] = 0;
	}

	IMG_MGR->SethWnd(_hWnd);
	//	�غ��� �̹��� ������ ũ�Ⱑ 500 x 100 �̸�
	//	0 = �⺻, 1 = �Ķ�(��ֹ�), 2 = �ʷ�(���), 3 = ����(����), 4 = ���
	//	���Ͽ� �̹��� 5���� �پ��־(m_CellSize.cx * 5, m_CellSize.cy)
	IMG_MGR->OpenImg("tile_01.bmp", m_CellSize.cx * 5, m_CellSize.cy);

	auto	img = IMG_MGR->GetImg("tile_01");

	if (img != nullptr)
	{
		img->SetTrans();
		img->SetFrame(5);
	}
}

VOID MyWin::Release()
{
	IMG_MGR->Destroy();

	if (m_pMap != nullptr)
	{
		delete[] m_pMap;
		m_pMap = nullptr;
	}
}

VOID MyWin::Update(float _elapseTime)
{
	//	���� �����츦 �ٷ�� �ִ°�?
	if (m_isFocus == false)
		return;

	//	�� ��ũ��
	int speed = 10;

	if (!m_isImg)
	{
		if (GetKeyState(VK_LEFT) & 0x8000)
		{
			m_CamPos.x -= speed;
		}
		if (GetKeyState(VK_RIGHT) & 0x8000)
		{
			m_CamPos.x += speed;
		}
		if (GetKeyState(VK_UP) & 0x8000)
		{
			m_CamPos.y -= speed;
		}
		if (GetKeyState(VK_DOWN) & 0x8000)
		{
			m_CamPos.y += speed;
		}
	}
	if (m_CamPos.x < 0)		m_CamPos.x = 0;
	if (m_CamPos.x + m_BgSize.cx > m_CellSize.cx * m_nx)
		m_CamPos.x = m_CellSize.cx * m_nx - m_BgSize.cx;
	if (m_CamPos.y < 0)		m_CamPos.y = 0;
	if (m_CamPos.y + m_BgSize.cy > m_CellSize.cy * m_ny)
		m_CamPos.y = m_CellSize.cy * m_ny - m_BgSize.cy;
}

VOID MyWin::Render(HDC _hdc)
{
	///////////////////////////////////////////////////////////////////////
	//	LineTo�� �̿��� Ÿ�ϸ� �׸���
	//	�Է��� Update���� �޴´�
	//	���� Ÿ�� ��ǥ
	int		startPosX = m_CamPos.x / m_CellSize.cx;
	int		startPosY = m_CamPos.y / m_CellSize.cy;

	//	�ܰ���
	Rectangle(_hdc,
		m_TileToStart.x,
		m_TileToStart.y,
		m_TileToStart.x + m_BgSize.cx,
		m_TileToStart.y + m_BgSize.cy);

	//	����
	if (!m_isImg)
	{
		for (int x = 0; x <= m_nx; x++)
		{
			int winPosX = m_CellSize.cx * (startPosX + x) - m_CamPos.x;

			if (winPosX < 0 || winPosX > m_BgSize.cx)	continue;

			winPosX += m_TileToStart.x;

			MoveToEx(_hdc, winPosX, m_TileToStart.y, nullptr);
			LineTo(_hdc, winPosX, m_TileToStart.y + m_BgSize.cy);
		}
		//	����
		for (int y = 0; y <= m_ny; y++)
		{
			int winPosY = m_CellSize.cy * (startPosY + y) - m_CamPos.y;

			if (winPosY < 0 || winPosY > m_BgSize.cy)	continue;

			winPosY += m_TileToStart.y;

			MoveToEx(_hdc, m_TileToStart.x, winPosY, nullptr);
			LineTo(_hdc, m_TileToStart.x + m_BgSize.cx, winPosY);
		}
	}
	///////////////////////////////////////////////////////////////////////


	//	�̹��� ������ �̿��Ͽ� Ÿ�� ���
	//	�Է��� KeyDown���� �޴´� (�Է� �ѹ��� �� ���� �̵��ϱ� ���� )
	//	�̹����� �������� â�ӿ� ����� ������ �̵��� CellSize������ŭ �̵��ؾ� �Ѵ�
	
	if (m_isImg)
	{
		auto img = IMG_MGR->GetImg("tile_01");
		for (int y = 0; y < m_ny; y++)
		{
			for (int x = 0; x < m_nx; x++)
			{
				int winPosX = (m_CellSize.cx * x) - m_CamPos.x;
				if (winPosX < 0 || winPosX >= m_BgSize.cx)	continue;

				int winPosY = (m_CellSize.cy * y) - m_CamPos.y;
				if (winPosY < 0 || winPosY >= m_BgSize.cy)	continue;

				winPosX += m_TileToStart.x;
				winPosY += m_TileToStart.y;

				switch (m_pMap[(m_nx * y) + x])
				{
				case 0: {	img->AniRender(_hdc, 0, winPosX, winPosY);	}	break;
				case 1: {	img->AniRender(_hdc, 1, winPosX, winPosY);	}	break;
				case 2: {	img->AniRender(_hdc, 2, winPosX, winPosY);	}	break;
				case 3: {	img->AniRender(_hdc, 3, winPosX, winPosY);	}	break;
				case 4: {	img->AniRender(_hdc, 4, winPosX, winPosY);	}	break;
				}
			}
		}
	}

	//	Ÿ�� �� ĭ�� �ش� ��ǥ ���
	if (m_isView)
	{
		for (int y = 0; y < m_ny; y++)
		{
			for (int x = 0; x < m_nx; x++)
			{
				int winPosX = m_CellSize.cx * (startPosX + x) - m_CamPos.x;
				if (winPosX < 0 || winPosX >= m_BgSize.cx)	continue;

				int winPosY = m_CellSize.cy * (startPosY + y) - m_CamPos.y;
				if (winPosY < 0 || winPosY >= m_BgSize.cy)	continue;

				winPosX += m_TileToStart.x;
				winPosY += m_TileToStart.y;

				std::stringstream buf;
				buf << "[" << startPosX + x << ", " << startPosY + y << "]";
				TextOut(_hdc, winPosX + 5, winPosY + 5, buf.str().c_str(), buf.str().length());
				
				if (!m_isImg)
				{
					buf.str("");
					switch (m_pMap[(m_nx * y) + x])
					{
					case 1:
					{
						buf << "��";
						TextOut(_hdc, winPosX + m_CellSize.cy / 2 - 10, winPosY + m_CellSize.cy / 2, buf.str().c_str(), buf.str().length());
					} break;
					case 2:
					{
						buf << "�����";
						TextOut(_hdc, winPosX + 5, winPosY + m_CellSize.cy / 2, buf.str().c_str(), buf.str().length());
					} break;
					case 3:
					{
						buf << "������";
						TextOut(_hdc, winPosX + 5, winPosY + m_CellSize.cy / 2, buf.str().c_str(), buf.str().length());
					} break;
					case 4:
					{
						buf << "��";
						TextOut(_hdc, winPosX + 5, winPosY + m_CellSize.cy / 2, buf.str().c_str(), buf.str().length());
					} break;
					}
				}
			}
		}
	}

	//	Text
	std::stringstream buf;
	buf << "<Mode>";
	TextOut(_hdc, 1400, 100, buf.str().c_str(), buf.str().length());

	buf.str("");
	buf << "SpaceBar         : ��ũ�Ѹ�� <�̹���,��>";
	TextOut(_hdc, 1400, 120, buf.str().c_str(), buf.str().length());
	
	buf.str("");
	buf << "Mouse Rbutton : ��ǥ <On,Off>";
	TextOut(_hdc, 1400, 140, buf.str().c_str(), buf.str().length());
	
	buf.str("");
	buf << "Clicked : <" << m_ClickPos.x << "," << m_ClickPos.y << ">";
	TextOut(_hdc, 1400, 200, buf.str().c_str(), buf.str().length());

	buf.str("");
	buf << "<Ŭ���� ��ǥ�� ��ġ>";
	TextOut(_hdc, 1400, 220, buf.str().c_str(), buf.str().length());
		
	buf.str("");
	buf << "1.��ֹ�";
	TextOut(_hdc, 1400, 240, buf.str().c_str(), buf.str().length());

	buf.str("");
	buf << "2.������ <�巡��o>";
	TextOut(_hdc, 1400, 260, buf.str().c_str(), buf.str().length());
	
	buf.str("");
	buf << "3.������ <�巡��o>";
	TextOut(_hdc, 1400, 280, buf.str().c_str(), buf.str().length());
	
	buf.str("");
	buf << "Enter : ���Ž�� A*";
	TextOut(_hdc, 1400, 300, buf.str().c_str(), buf.str().length());
}

LRESULT MyWin::WndProc(HWND _hWnd, UINT _message, WPARAM _wParam, LPARAM _lParam)
{
	switch (_message)
	{
	case WM_KEYDOWN:
	{
		switch (_wParam)
		{
			//	�ƽ�Ű�ڵ� 49 == 1, 50 == 2, 51 == 3
		case 49:	{	TransMap(1); }	break;
		case 50:	{	TransMap(2); }	break;
		case 51:	{	TransMap(3); }	break;
		case VK_SPACE:		{	m_isImg = !m_isImg; }	break;
		case VK_RETURN:
		{
			if (FindPath_AStar())
			{
				for (auto i = m_pathList.begin(); i != m_pathList.end(); i++)
				{
					int x = i->x;
					int y = i->y;
					if(m_pMap[(m_nx * y) + x] == 0)
						m_pMap[(m_nx * y) + x] = 4;
				}
			}
		} break;
			if (m_isImg)
			{
		case VK_LEFT: {	m_CamPos.x -= m_CellSize.cx; }	break;
		case VK_RIGHT: {	m_CamPos.x += m_CellSize.cx; }	break;
		case VK_UP: {	m_CamPos.y -= m_CellSize.cy; }	break;
		case VK_DOWN: {	m_CamPos.y += m_CellSize.cy; }	break;
			}
		default:
			break;
		}

	}break;
	case WM_ACTIVATEAPP:
	{
		m_isFocus = (bool)_wParam;
	}break;

	case WM_MOUSEMOVE:
	{
		m_MousePos.x = GET_X_LPARAM(_lParam);
		m_MousePos.y = GET_Y_LPARAM(_lParam);
	}break;
	case WM_RBUTTONDOWN:
	{
		m_isView = !m_isView;
	} break;
	case WM_LBUTTONDOWN:
	{
		m_ClickPos.x = m_MousePos.x + m_CamPos.x - m_TileToStart.x;
		m_ClickPos.y = m_MousePos.y + m_CamPos.y - m_TileToStart.y;

		m_ClickPos.x /= m_CellSize.cx;
		m_ClickPos.y /= m_CellSize.cy;
		if (m_pMap[(m_nx * m_ClickPos.y) + m_ClickPos.x] == 2)
			m_drag = 2;
		else if(m_pMap[(m_nx * m_ClickPos.y) + m_ClickPos.x] == 3)
			m_drag = 3;
		else
			m_drag = 0;
	} break;

	case WM_LBUTTONUP:
	{
		m_ClickPos.x = m_MousePos.x + m_CamPos.x - m_TileToStart.x;
		m_ClickPos.y = m_MousePos.y + m_CamPos.y - m_TileToStart.y;

		m_ClickPos.x /= m_CellSize.cx;
		m_ClickPos.y /= m_CellSize.cy;
		if (m_drag == 2)
			TransMap(2);
		else if (m_drag == 3)
			TransMap(3);
	}

	}
	return WinBase::WndProc(_hWnd, _message, _wParam, _lParam);
}

MyWin::MyWin()
{
}

MyWin::~MyWin()
{
}

VOID MyWin::TransMap(int _index)
{
	//	1. ��ֹ�, 2. ������, 3. ������
	//	��ֹ� ĭ ���ý� �̹� ��ֹ��̿��ٸ� ����
	//	�������� �������� �Ѱ��� �����ؾ���
	//	Ŭ���� ��ǥ
	
	int mapIndex = (m_nx * m_ClickPos.y) + m_ClickPos.x;
	switch (_index)
	{
	//	��ֹ�
	case 1:
	{
		if (m_pMap[mapIndex] == _index)
			m_pMap[mapIndex] = 0;
		else
			m_pMap[mapIndex] = _index;
	}break;

	//	������
	case 2:
	{
		for (int i = 0; i < m_size; i++)
		{
			if (m_pMap[i] == 2)	m_pMap[i] = 0;
		}
		m_pMap[mapIndex] = _index;
		GetStartEnd();
	}break;

	//	������
	case 3:
	{
		for (int i = 0; i < m_size; i++)
		{
			if (m_pMap[i] == 3)	m_pMap[i] = 0;
		}
		m_pMap[mapIndex] = _index;
		GetStartEnd();
	}break;
	}
}

VOID MyWin::GetStartEnd()
{
	for (int i = 0; i < m_size; i++)
	{
		if (m_pMap[i] == 0 || m_pMap[i] == 1 || m_pMap[i] == 4)	continue;
		if (m_pMap[i] == 2)
		{
			m_start.x = i % m_nx;
			m_start.y = i / m_nx;
		}
		if (m_pMap[i] == 3)
		{
			m_end.x = i % m_nx;
			m_end.y = i / m_nx;
		}
	}
}

bool	operator==(POINT _pos1, POINT _pos2)
{
	return (_pos1.x == _pos2.x && _pos1.y == _pos2.y);
}

bool MyWin::FindPath_AStar()
{
	//	�������, �������
	list<TileNode>	openList, closeList;

	//	����Ÿ���� ������Ͽ� �ִ´�.
	TileNode	info;
	info.pos = m_start;

	openList.push_back(info);

	//	���� ������ �ݺ�
	while (true)
	{
		//   a) ������Ͽ��� ���� ���� F ����� ã�� ����簢������ �����մϴ�.
		//   b) �̰��� ������Ͽ��� ���� ����������� �ֽ��ϴ�.
		closeList.push_back(openList.front());
		openList.pop_front();

		//	( ���������� ������Ͽ� �ִ� �簢���� ���õ� �簢���̴�. )
		TileNode	&selectNode = closeList.back();

		//	�����̾�?
		if (selectNode.pos == m_end)
		{
			for (auto i = m_pathList.begin(); i != m_pathList.end(); i++)
			{
				int x = i->x;
				int y = i->y;
				m_pMap[(m_nx * y) + x] = 0;
			}

			m_pathList.clear();

			//	��� ����
			TileNode	*node = &selectNode;

			while (node != nullptr)
			{
				m_pathList.push_front(node->pos);

				node = node->parentNode;
			}

			return true;
		}

		//	���õ� ����� ������ 8�� �˻�
		for (int y = -1; y <= 1; y++)
		{
			for (int x = -1; x <= 1; x++)
			{
				//	���õ� ���� ����
				if (x == 0 && y == 0) continue;

				//	��ǥ����
				POINT pos;
				pos.x = selectNode.pos.x + x;
				pos.y = selectNode.pos.y + y;

				//	�̵��Ұ�(���)
				if (pos.x < 0 ||
					pos.y < 0 ||
					pos.x >= m_nx ||
					pos.y >= m_ny)
					continue;

				//	�̵��Ұ�(��ֹ�)
				if (m_pMap[m_nx * pos.y + pos.x] == 1)
					continue;

				//	���� ��Ͽ� �ִ°�?
				bool	isCloseList = false;

				for (auto &i : closeList)
				{
					if (i.pos == pos)
					{
						isCloseList = true;
						break;
					}
				}
				if (isCloseList == true) continue;


				//	������Ͽ� �ִ°�?
				TileNode	*node = nullptr;

				for (auto &i : openList)
				{
					if (i.pos == pos)
					{
						node = &i;
						break;
					}
				}

				////////////////////////////////////////////

				int thisTimeG = selectNode.G + ((x*y == 0) ? 10 : 14);
				int thisTimeH = (abs(pos.x - m_end.x) + abs(pos.y - m_end.y)) * 10;
				int thisTimeF = thisTimeG + thisTimeH;

				//	������Ͽ� ���� ��
				if (node == nullptr)
				{
					TileNode newNode;

					//	�θ���
					newNode.parentNode = &selectNode;

					//	F,G,H
					newNode.F = thisTimeF;
					newNode.G = thisTimeG;
					newNode.H = thisTimeH;

					//	pos ����
					newNode.pos = pos;

					//	������Ͽ� �߰�
					openList.push_back(newNode);
				}

				//	������Ͽ� ���� ��
				else
				{
					//	�̹��� ����� ���� �� ȿ�����ΰ�?
					if (node->G > thisTimeG)
					{
						//	�θ� �̹� ���� ����
						node->parentNode = &selectNode;

						//	F,G,H �� ����
						node->F = thisTimeF;
						node->G = thisTimeG;
						node->H = thisTimeH;
					}
				}
			}
		}

		//	��������� ����ٸ�?
		if (openList.empty() == true)	break;

		//	����
		openList.sort();
	}

	return false;
}