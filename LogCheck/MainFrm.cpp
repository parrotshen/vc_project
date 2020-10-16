// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "LogCheck.h"
#include "LogCheckDoc.h"
#include "LogCheckEditView.h"
#include "LogCheckListView.h"
#include "MainFrm.h"
#include <afxpriv.h>

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
	ON_WM_CLOSE()
	ON_MESSAGE(WM_SETMESSAGESTRING, OnSetMessageString)
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
	m_StatusMsg = "尚未進行設定";
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

#if 1
	cs.cx = (width  / 2);
	cs.cy = (height / 2);
#else
	cs.cx = width;
	cs.cy = height;
#endif

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
	// +---------+
	// |   0.0   |
	// +---------+
	// |   1.0   |
	// +---------+
	CRect ClientSize;
	GetClientRect(ClientSize);
	int height = ClientSize.bottom / 2;
	int width  = ClientSize.right;
	if( !m_splitter.CreateStatic(this, 2, 1) )
		return FALSE;
	if( !m_splitter.CreateView(
			0,
			0,
			RUNTIME_CLASS(CLogCheckEditView),
			CSize(width, height * 1),
			pContext) )
		return FALSE;
	if( !m_splitter.CreateView(
			1,
			0,
			RUNTIME_CLASS(CLogCheckListView),
			CSize(width, height * 1),
			pContext) )
		return FALSE;
	return TRUE;
	// End Splitter Window
}

void CMainFrame::OnClose() 
{
	// TODO: Add your message handler code here and/or call default

	CLogCheckDoc* doc = (CLogCheckDoc *)GetActiveDocument();
	if (doc != NULL)
	{
		doc->CloseConnection();
	}

	CFrameWnd::OnClose();
}

LRESULT CMainFrame::OnSetMessageString(WPARAM wParam, LPARAM lParam)
{
	LRESULT lr = CFrameWnd::OnSetMessageString(wParam, lParam);

	if (wParam == AFX_IDS_IDLEMESSAGE)
	{
		m_wndStatusBar.SetPaneText(0, m_StatusMsg, TRUE);
	}

	return lr;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame member methods

void CMainFrame::SetStatusMessage(CString msg)
{
	m_wndStatusBar.SetPaneText(0, msg, TRUE);
	m_StatusMsg = msg;
}

