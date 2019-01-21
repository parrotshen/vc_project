// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "Compare.h"
#include "MainFrm.h"
#include "CompareDoc.h"

#include "FirstView.h"
#include "SecondView.h"
#include "ReportView.h"

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
	ON_BN_CLICKED(IDC_BUTTON_FILE1, OnButtonFile1)
	ON_BN_CLICKED(IDC_BUTTON_FILE2, OnButtonFile2)
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
	
	if (!m_wndToolBar.CreateEx(this) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
	if (!m_wndDlgBar.Create(this, IDR_MAINFRAME, 
		CBRS_ALIGN_TOP, AFX_IDW_DIALOGBAR))
	{
		TRACE0("Failed to create dialogbar\n");
		return -1;		// fail to create
	}

	if (!m_wndReBar.Create(this) ||
		!m_wndReBar.AddBar(&m_wndToolBar) ||
		!m_wndReBar.AddBar(&m_wndDlgBar))
	{
		TRACE0("Failed to create rebar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	// TODO: Remove this if you don't want tool tips
	m_wndToolBar.SetBarStyle(m_wndToolBar.GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

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
	//return CFrameWnd::OnCreateClient(lpcs, pContext);

	// Begin Splitter Window
	// +---------+
	// |   0.0   |
	// +---------+
	// |   1.0   |
	// +---------+
	// |   2.0   |
	// +---------+
	if( !m_splitter.CreateStatic(this, 3, 1) )
		return FALSE;
	if( !m_splitter.CreateView(0, 0, RUNTIME_CLASS(CFirstView), CSize(100, 150), pContext) )
		return FALSE;
	if( !m_splitter.CreateView(1, 0, RUNTIME_CLASS(CSecondView), CSize(100, 150), pContext) )
		return FALSE;
	if( !m_splitter.CreateView(2, 0, RUNTIME_CLASS(CReportView), CSize(100, 0), pContext) )
		return FALSE;
	return TRUE;
	// End Splitter Window
}

/////////////////////////////////////////////////////////////////////////////
// Tool Button Events

void CMainFrame::OnButtonFile1() 
{
	//開啟檔案-1
	CCompareDoc* doc = (CCompareDoc*)GetActiveDocument();
	doc->LoadFile1();

	CEdit* edit = (CEdit*)m_wndDlgBar.GetDlgItem(IDC_EDIT_FILE1);
	edit->SetWindowText( doc->m_name1 );
}

void CMainFrame::OnButtonFile2() 
{
	//開啟檔案-2
	CCompareDoc* doc = (CCompareDoc*)GetActiveDocument();
	doc->LoadFile2();

	CEdit* edit = (CEdit*)m_wndDlgBar.GetDlgItem(IDC_EDIT_FILE2);
	edit->SetWindowText( doc->m_name2 );
}

/////////////////////////////////////////////////////////////////////////////
// Class Member Function

void CMainFrame::UpdateFileName1(CString name)
{
	CEdit* edit = (CEdit*)m_wndDlgBar.GetDlgItem(IDC_EDIT_FILE1);
	edit->SetWindowText( name );
}

void CMainFrame::UpdateFileName2(CString name) 
{
	CEdit* edit = (CEdit*)m_wndDlgBar.GetDlgItem(IDC_EDIT_FILE2);
	edit->SetWindowText( name );
}
