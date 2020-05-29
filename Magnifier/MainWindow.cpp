// MainWindow.cpp : implementation file
//
// Magnifier Program by Shen 2002/03/27
#include "stdafx.h"
#include "MyApp.h"
#include "MainWindow.h"
#include "AboutDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// //////////////////////////////
// include the mouse patch code
#include "MousePatch.cpp"
// //////////////////////////////

#define UPPER_BOUND 20
#define LOWER_BOUND 1

/////////////////////////////////////////////////////////////////////////////
// CMainWindow

CMainWindow::CMainWindow()
{
	//���o Desktop �� DC Handle
	m_hDesktopDC  = ::GetDC(0);

	m_Wnd_Width   = 139;
	m_Wnd_Height  = 126;
	m_Multiple    = 1;
	m_pColorArray = new COLORREF[m_Wnd_Width*m_Wnd_Height];
	memset(m_pColorArray, 0x00, m_Wnd_Width*m_Wnd_Height*sizeof(COLORREF));

	//Begin: Set The Logo
	CBitmap bitmap;
	bitmap.LoadBitmap(IDB_LOGO);
	int ret = bitmap.GetBitmapBits(m_Wnd_Width*m_Wnd_Height*sizeof(COLORREF), m_pColorArray);
	bitmap.Detach();
	//End:   Set The Logo

	//���ͩ�j�誺����
    Create(NULL, "���N��j��");

	//�}�l�B�z�ƹ����T��
	StartWindowsHook();

	//�e�X�����I�����v��
	//ShowWindowBackground();
}

CMainWindow::~CMainWindow()
{
	if(m_pColorArray != NULL)
	{
		delete m_pColorArray;
		m_pColorArray = NULL;
	}
}


BEGIN_MESSAGE_MAP(CMainWindow, CFrameWnd)
	//{{AFX_MSG_MAP(CMainWindow)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_NCLBUTTONDOWN()
	ON_WM_NCLBUTTONUP()
	ON_WM_MOUSEWHEEL()
	ON_WM_KEYUP()
	ON_WM_LBUTTONDBLCLK()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMainWindow message handlers

BOOL CMainWindow::PreCreateWindow(CREATESTRUCT& cs) 
{
	//�]�w������l���j�p�P��m
	//cs.style = WS_OVERLAPPED|WS_CAPTION|WS_SYSMENU|WS_MINIMIZEBOX;
	cs.cx = m_Wnd_Width;
	cs.cy = m_Wnd_Height;
	//cs.x  = 100;
	//cs.y  = 50;

	return CFrameWnd::PreCreateWindow(cs);
}

void CMainWindow::OnPaint() 
{
	// device context for painting
	CPaintDC dc(this); 
	
	//�Y�v���}�C���ŴN���@��ø
	if(m_pColorArray == NULL) return;

	//Begin: �N�I���v���e�X
	int i, j, h, k, x, y;
	int Height = m_Wnd_Height / m_Multiple;
	int Width  = m_Wnd_Width / m_Multiple;
	for(i=0; i<Height; i++)
	{
		for(j=0; j<Width; j++)
		{
			for(h=0; h<m_Multiple; h++)
			{
				for(k=0; k<m_Multiple; k++)
				{
					x = j*m_Multiple+h;
					y = i*m_Multiple+k;
					dc.SetPixel(x, y, m_pColorArray[i*m_Wnd_Width+j]);
				}
			}
		}
	}
	//End:   �N�I���v���e�X

	// Do not call CFrameWnd::OnPaint() for painting messages
}

void CMainWindow::OnDestroy() 
{
	CFrameWnd::OnDestroy();

	//����B�z�ƹ����T��
	StopWindowsHook();
}

/////////////////////////////////////////////////////////////////////////////
// ��L�P�ƹ����T���B�z�禡

void CMainWindow::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if(nChar == 13 || nChar == 116)
	{
		//���U [Enter] �� [F5] ����
		//Reflash Client Area of Magnifier
		ShowWindowBackground();
	}
	else
	{
		//���U [PageUp] �� [+] ������
		if((nChar == 33) || (nChar == 107))
			m_Multiple++;
		//���U [PageDown] �� [-] ������
		if((nChar == 34) || (nChar == 109))
			m_Multiple--;

		if(m_Multiple <= LOWER_BOUND)
			m_Multiple = LOWER_BOUND;

		if(m_Multiple >= UPPER_BOUND)
			m_Multiple = UPPER_BOUND;

		//��ø�X�v���}�C�������
		DrawBackground();
	}

	CFrameWnd::OnKeyUp(nChar, nRepCnt, nFlags);
}

BOOL CMainWindow::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	if(zDelta > 0)
		m_Multiple++; //�W�[����
	else
		m_Multiple--; //��֭���

	if(m_Multiple <= LOWER_BOUND)
		m_Multiple = LOWER_BOUND;

	if(m_Multiple >= UPPER_BOUND)
		m_Multiple = UPPER_BOUND;

	//��ø�X�v���}�C�������
	DrawBackground();

	return CFrameWnd::OnMouseWheel(nFlags, zDelta, pt);
}

void CMainWindow::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	CAboutDlg dlg;
	dlg.DoModal();

	CFrameWnd::OnLButtonDblClk(nFlags, point);
}


void CMainWindow::OnNcLButtonDown(UINT nHitTest, CPoint point) 
{
	//�P�_�ƹ�����O�_�b��������ئ�m�Q���U
	if((nHitTest == HTBOTTOM) || (nHitTest == HTBOTTOMLEFT) || (nHitTest == HTBOTTOMRIGHT) || (nHitTest == HTLEFT)
							|| (nHitTest == HTRIGHT)|| (nHitTest == HTTOP) || (nHitTest == HTTOPLEFT) || (nHitTest == HTTOPRIGHT))
		ResizeColorArray(FALSE);

	CFrameWnd::OnNcLButtonDown(nHitTest, point);
} 

void CMainWindow::OnNcLButtonUp(UINT nHitTest, CPoint point) 
{
	//�P�_�ƹ�����O�_�b�������D��m�Q��}(���ʵ���)
	if(nHitTest == HTCAPTION)
		ShowWindowBackground();

	//�P�_�ƹ�����O�_�b��������ئ�m�Q��}(���ܵ����j�p)
	if(nHitTest == HTSIZE)
		ResizeColorArray(TRUE);

	CFrameWnd::OnNcLButtonUp(nHitTest, point);
}

/////////////////////////////////////////////////////////////////////////////
// ���O�������禡

void CMainWindow::ResizeColorArray(BOOL IsButtonUp)
{
	if( IsButtonUp )
	{
		//1-�̵����j�p���s�إ߼v���}�C
		if(m_pColorArray == NULL)
		{
			CRect rect;
			GetWindowRect( &rect );
			m_Wnd_Width  = rect.Width();
			m_Wnd_Height = rect.Height();

			//�t�m�@�ӻP�����j�p�ۦP���O����
			m_pColorArray = new COLORREF[ m_Wnd_Width*m_Wnd_Height ];
			memset(m_pColorArray, 0x00, m_Wnd_Width*m_Wnd_Height*sizeof(COLORREF));

			//�t�X�����j�p���s�e�X��I�����v��
			ShowWindowBackground();
		}
	}
	else
	{
		//2-�R���v���}�C
		if(m_pColorArray != NULL)
		{
			delete m_pColorArray;
			m_pColorArray = NULL;
		}
	}
}

void CMainWindow::ShowWindowBackground()
{
	ShowWindow( SW_HIDE );
	Sleep(100);

	//Begin: ���o�������I���v��
	GetClientRect( &m_ClientRect );
	ClientToScreen( &m_ClientRect );
	for(int i=0; i<m_Wnd_Height; i++)
	{
		for(int j=0; j<m_Wnd_Width; j++)
		{
			m_pColorArray[i*m_Wnd_Width+j] = ::GetPixel(m_hDesktopDC, m_ClientRect.left+j, m_ClientRect.top+i);
		}
	}
	//End:   ���o�������I���v��

	//Begin: �N�I���v�����s�e�X
	ShowWindow( SW_SHOW );
	DrawBackground();
	//End:   �N�I���v�����s�e�X
}

void CMainWindow::DrawBackground()
{
	//Begin: �N�I���v�����s�e�X
	CClientDC dc(this);

	int i, j, h, k, x, y;
	int Height = m_Wnd_Height / m_Multiple;
	int Width  = m_Wnd_Width / m_Multiple;
	for(i=0; i<Height; i++)
	{
		for(j=0; j<Width; j++)
		{
			//Begin: �̷ӭ��Ƶe�X��@�ӹ���
			for(h=0; h<m_Multiple; h++)
			{
				for(k=0; k<m_Multiple; k++)
				{
					x = j*m_Multiple+h;
					y = i*m_Multiple+k;
					dc.SetPixel(x, y, m_pColorArray[i*m_Wnd_Width+j]);
				}
			}
			//End:   �̷ӭ��Ƶe�X��@�ӹ���
		}
	}
	//End:   �N�I���v�����s�e�X
}

void CMainWindow::StartWindowsHook()
{
	if(hMHook != NULL)
		StopWindowsHook();

	// Install mouse message hook
	hMHook = SetWindowsHookEx(WH_MOUSE, MouseHookProc, AfxGetInstanceHandle(), AfxGetThread()->m_nThreadID);
}

void CMainWindow::StopWindowsHook()
{
	// Remove the hook
	if(hMHook != NULL)
		UnhookWindowsHookEx(hMHook);

	hMHook = NULL;
}
