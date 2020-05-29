// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "MySIM.h"
#include "MainFrm.h"
#include "MySIMTreeView.h"
#include "MySIMListView.h"
#include "MySIMEditView.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	int width  = GetSystemMetrics(SM_CXSCREEN);
	int height = GetSystemMetrics(SM_CYSCREEN);

	cs.cx = (width  / 2);
	cs.cy = (height / 2);

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG


/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	//return CFrameWnd::OnCreateClient(lpcs, pContext);

	// Begin Splitter Window
	// +-----+-----+
	// | 0.0 | 0.1 |
	// +-----+-----+
	// |    1.0    |
	// +-----+-----+

	CRect clientSize;
	GetClientRect( clientSize );
	int height = clientSize.bottom / 2;
	int width  = clientSize.right  / 4;

	// [1] Create a upper and bottom window
	if( !m_splitter.CreateStatic(this, 2, 1) )
		return FALSE;
	m_splitter.SetRowInfo(0, height, 0);

	// [2] Create a left and right window
	if( !m_splitterSub.CreateStatic(&m_splitter, 1, 2) )
		return FALSE;

	if( !m_splitterSub.CreateView(0, 0, RUNTIME_CLASS(CMySIMTreeView), CSize(width, height), pContext) )
		return FALSE;
	if( !m_splitterSub.CreateView(0, 1, RUNTIME_CLASS(CMySIMListView), CSize(width*2, height), pContext) )
		return FALSE;
	if( !m_splitter.CreateView(1, 0, RUNTIME_CLASS(CMySIMEditView), CSize(width*3, height), pContext) )
		return FALSE;

	return TRUE;
	// End Splitter Window
}

void CMainFrame::OnSize(UINT nType, int cx, int cy) 
{
	//CFrameWnd::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if(::IsWindow(m_splitter.m_hWnd) && ::IsWindow(m_splitterSub.m_hWnd))
	{
		m_splitter.SetRowInfo(0, cy/2, 10);

		m_splitterSub.SetColumnInfo(0, cx/4, 10);
		m_splitterSub.SetColumnInfo(1, cx*3/4, 10);

		RecalcLayout();
	}
}

