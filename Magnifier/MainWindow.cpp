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
	//取得 Desktop 的 DC Handle
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

	//產生放大鏡的視窗
    Create(NULL, "任意放大鏡");

	//開始處理滑鼠的訊息
	StartWindowsHook();

	//畫出視窗背景的影像
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
	//設定視窗初始的大小與位置
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
	
	//若影像陣列為空就不作重繪
	if(m_pColorArray == NULL) return;

	//Begin: 將背景影像畫出
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
	//End:   將背景影像畫出

	// Do not call CFrameWnd::OnPaint() for painting messages
}

void CMainWindow::OnDestroy() 
{
	CFrameWnd::OnDestroy();

	//停止處理滑鼠的訊息
	StopWindowsHook();
}

/////////////////////////////////////////////////////////////////////////////
// 鍵盤與滑鼠的訊息處理函式

void CMainWindow::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if(nChar == 13 || nChar == 116)
	{
		//按下 [Enter] 或 [F5] 按鍵
		//Reflash Client Area of Magnifier
		ShowWindowBackground();
	}
	else
	{
		//按下 [PageUp] 或 [+] 的按鍵
		if((nChar == 33) || (nChar == 107))
			m_Multiple++;
		//按下 [PageDown] 或 [-] 的按鍵
		if((nChar == 34) || (nChar == 109))
			m_Multiple--;

		if(m_Multiple <= LOWER_BOUND)
			m_Multiple = LOWER_BOUND;

		if(m_Multiple >= UPPER_BOUND)
			m_Multiple = UPPER_BOUND;

		//重繪出影像陣列內的資料
		DrawBackground();
	}

	CFrameWnd::OnKeyUp(nChar, nRepCnt, nFlags);
}

BOOL CMainWindow::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	if(zDelta > 0)
		m_Multiple++; //增加倍數
	else
		m_Multiple--; //減少倍數

	if(m_Multiple <= LOWER_BOUND)
		m_Multiple = LOWER_BOUND;

	if(m_Multiple >= UPPER_BOUND)
		m_Multiple = UPPER_BOUND;

	//重繪出影像陣列內的資料
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
	//判斷滑鼠左鍵是否在視窗的邊框位置被按下
	if((nHitTest == HTBOTTOM) || (nHitTest == HTBOTTOMLEFT) || (nHitTest == HTBOTTOMRIGHT) || (nHitTest == HTLEFT)
							|| (nHitTest == HTRIGHT)|| (nHitTest == HTTOP) || (nHitTest == HTTOPLEFT) || (nHitTest == HTTOPRIGHT))
		ResizeColorArray(FALSE);

	CFrameWnd::OnNcLButtonDown(nHitTest, point);
} 

void CMainWindow::OnNcLButtonUp(UINT nHitTest, CPoint point) 
{
	//判斷滑鼠左鍵是否在視窗標題位置被放開(移動視窗)
	if(nHitTest == HTCAPTION)
		ShowWindowBackground();

	//判斷滑鼠左鍵是否在視窗的邊框位置被放開(改變視窗大小)
	if(nHitTest == HTSIZE)
		ResizeColorArray(TRUE);

	CFrameWnd::OnNcLButtonUp(nHitTest, point);
}

/////////////////////////////////////////////////////////////////////////////
// 類別的成員函式

void CMainWindow::ResizeColorArray(BOOL IsButtonUp)
{
	if( IsButtonUp )
	{
		//1-依視窗大小重新建立影像陣列
		if(m_pColorArray == NULL)
		{
			CRect rect;
			GetWindowRect( &rect );
			m_Wnd_Width  = rect.Width();
			m_Wnd_Height = rect.Height();

			//配置一個與視窗大小相同的記憶體
			m_pColorArray = new COLORREF[ m_Wnd_Width*m_Wnd_Height ];
			memset(m_pColorArray, 0x00, m_Wnd_Width*m_Wnd_Height*sizeof(COLORREF));

			//配合視窗大小重新畫出其背景的影像
			ShowWindowBackground();
		}
	}
	else
	{
		//2-刪除影像陣列
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

	//Begin: 取得視窗的背景影像
	GetClientRect( &m_ClientRect );
	ClientToScreen( &m_ClientRect );
	for(int i=0; i<m_Wnd_Height; i++)
	{
		for(int j=0; j<m_Wnd_Width; j++)
		{
			m_pColorArray[i*m_Wnd_Width+j] = ::GetPixel(m_hDesktopDC, m_ClientRect.left+j, m_ClientRect.top+i);
		}
	}
	//End:   取得視窗的背景影像

	//Begin: 將背景影像重新畫出
	ShowWindow( SW_SHOW );
	DrawBackground();
	//End:   將背景影像重新畫出
}

void CMainWindow::DrawBackground()
{
	//Begin: 將背景影像重新畫出
	CClientDC dc(this);

	int i, j, h, k, x, y;
	int Height = m_Wnd_Height / m_Multiple;
	int Width  = m_Wnd_Width / m_Multiple;
	for(i=0; i<Height; i++)
	{
		for(j=0; j<Width; j++)
		{
			//Begin: 依照倍數畫出單一個像素
			for(h=0; h<m_Multiple; h++)
			{
				for(k=0; k<m_Multiple; k++)
				{
					x = j*m_Multiple+h;
					y = i*m_Multiple+k;
					dc.SetPixel(x, y, m_pColorArray[i*m_Wnd_Width+j]);
				}
			}
			//End:   依照倍數畫出單一個像素
		}
	}
	//End:   將背景影像重新畫出
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
