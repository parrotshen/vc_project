// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "TextViewer.h"
#include "MainFrm.h"
#include "TextViewerDoc.h"

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
	ON_CBN_SELCHANGE(IDC_COMBO_FILTER, OnSelchangeComboFilter)
	ON_BN_CLICKED(IDC_BUTTON_GOTO, OnButtonGoto)
	ON_BN_CLICKED(IDC_BUTTON_FILTER, OnButtonFilter)
	ON_BN_CLICKED(IDC_BUTTON_FIND, OnButtonFind)
	ON_COMMAND(ID_BUTTON_BACK, OnButtonBack)
	ON_COMMAND(ID_BUTTON_DOWN, OnButtonDown)
	ON_COMMAND(ID_BUTTON_UPPER, OnButtonUpper)
	ON_WM_CLOSE()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static UINT indicators[] =
{
	// Status line indicator
	ID_SEPARATOR,        // Index 0
	ID_INDICATOR_CAPS,   // Index 1
	ID_INDICATOR_NUM,    // Index 2
	ID_INDICATOR_SCRL,   // Index 3
	ID_INDICATOR_LINES,  // Index 4 (new added)
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	
}

CMainFrame::~CMainFrame()
{
	//detach image list with system icon(s)
	m_largeImageList.Detach();
	m_smallImageList.Detach();
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
	//�]�w DlgBar ����������
	m_wndDlgBar.SetDlgItemText(IDC_COMBO_FILTER, "*.*");

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
	//�s�W���A�C�����
	m_wndStatusBar.SetPaneInfo(4, ID_INDICATOR_LINES, SBPS_POPOUT, 80);
	m_wndStatusBar.SetPaneText(4, "Line / Total");

	// TODO: Remove this if you don't want tool tips
	m_wndToolBar.SetBarStyle(m_wndToolBar.GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY);

	//�]�w List ������ϥܬ����Ψt�ιϥ�
	InitializeSystemImageList();
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

	//Begin - Splitter Window
	// +-----+-------+
	// | 0.0 |       |
	// +-----+  0.1  |
	// | 1.0 |       |
	// +-----+-------+
	if( !m_Splitter.CreateStatic(this, 1, 2) )
		return FALSE;
	m_Splitter.SetColumnInfo(0, 200, 0);

	if( !m_SplitterSub.CreateStatic(&m_Splitter, 2, 1) )
		return FALSE;

	if( !m_SplitterSub.CreateView(0, 0, RUNTIME_CLASS(CTextTreeView), CSize(200, 150), pContext) )
		return FALSE;
	if( !m_SplitterSub.CreateView(1, 0, RUNTIME_CLASS(CTextListView), CSize(200, 350), pContext) )
		return FALSE;
	if( !m_Splitter.CreateView(0, 1, RUNTIME_CLASS(CTextEditView), CSize(300, 300), pContext) )
		return FALSE;

	return TRUE;
	//End   - Splitter Window
}

void CMainFrame::OnSize(UINT nType, int cx, int cy) 
{
	//CFrameWnd::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	if(::IsWindow(m_Splitter.m_hWnd) && ::IsWindow(m_SplitterSub.m_hWnd))
	{
		m_Splitter.SetColumnInfo(0, cx*1/4, 10);

		m_SplitterSub.SetRowInfo(0, cy*1/3, 10);
		m_SplitterSub.SetRowInfo(1, cy*2/3, 10);

		RecalcLayout();
	}
}

void CMainFrame::OnClose() 
{
	//Begin - �{�������ɱN�ثe���|�g�J Registry
	CWinApp* pApp = AfxGetApp();
	CTextViewerDoc* doc = (CTextViewerDoc*)GetActiveDocument();
	pApp->WriteProfileString("TextViewer", "ViewPath", doc->m_pListView->m_ScanPath);
	//End   - �{�������ɱN�ثe���|�g�J Registry

	CFrameWnd::OnClose();
}

void CMainFrame::OnSelchangeComboFilter() 
{
	//���o ComboBox ���������
	CComboBox* combo = (CComboBox*)m_wndDlgBar.GetDlgItem(IDC_COMBO_FILTER);
	CString filter;
	combo->GetLBText(combo->GetCurSel(), filter);

	//���ܰ��ɦW���L�o�ﶵ
	CTextViewerDoc* doc = (CTextViewerDoc*)GetActiveDocument();
	doc->m_pListView->m_ScanFilter = filter;
	doc->m_pListView->ReadDirectoryInfo();
}

/////////////////////////////////////////////////////////////////////////////
// Tool Button Events

void CMainFrame::OnButtonFilter() 
{
	//���o ComboBox ���������
	CComboBox* combo = (CComboBox*)m_wndDlgBar.GetDlgItem(IDC_COMBO_FILTER);
	CString filter;
	combo->GetWindowText(filter);

	//�N�s���z��榡�[�J�C�ܲ�
	//combo->AddString(filter);
	combo->InsertString(0, filter);

	//���ܰ��ɦW���L�o�ﶵ
	CTextViewerDoc* doc = (CTextViewerDoc*)GetActiveDocument();
	doc->m_pListView->m_ScanFilter = filter;
	doc->m_pListView->ReadDirectoryInfo();
}

void CMainFrame::OnButtonFind() 
{
	//���o Find Edit ��J�����e
	CString str;
	m_wndDlgBar.GetDlgItemText(IDC_EDIT_FIND, str);

	//�}�l�j�M�r��
	CTextViewerDoc* doc = (CTextViewerDoc*)GetActiveDocument();
	doc->m_pEditView->Button_FindText(str);
}

void CMainFrame::OnButtonGoto() 
{
	//���o Goto Edit ��J�����e
	CString str;
	m_wndDlgBar.GetDlgItemText(IDC_EDIT_GOTO, str);

	//���ܫ��w�����
	CTextViewerDoc* doc = (CTextViewerDoc*)GetActiveDocument();
	doc->m_pEditView->Button_GotoLines( atoi(str) );
}

void CMainFrame::OnButtonBack() 
{
	//�^�W�h��Ƨ�
	CTextViewerDoc* doc = (CTextViewerDoc*)GetActiveDocument();
	doc->m_pTreeView->BackToUpperFolder();
}

void CMainFrame::OnButtonDown() 
{
	//�ثe�� List Item �V�U����
	CTextViewerDoc* doc = (CTextViewerDoc*)GetActiveDocument();
	doc->m_pListView->MoveItemMarkDown();
}

void CMainFrame::OnButtonUpper() 
{
	//�ثe�� List Item �V�W����
	CTextViewerDoc* doc = (CTextViewerDoc*)GetActiveDocument();
	doc->m_pListView->MoveItemMarkUpper();
}

/////////////////////////////////////////////////////////////////////////////
// Class Member Methods

//Ū���t���ɮת��ϥ�
void CMainFrame::InitializeSystemImageList()
{
	HIMAGELIST hSystemSmallImageList, hSystemLargeImageList; 
	SHFILEINFO ssfi, lsfi; 

	//get a handle to the system small icon list 
	hSystemSmallImageList = (HIMAGELIST)SHGetFileInfo((LPCTSTR)_T("C:\\"), 0, &ssfi, sizeof(SHFILEINFO), SHGFI_SYSICONINDEX | SHGFI_SMALLICON); 

	//attach it to the small image list 
	//--DON'T FORGET TO PUT m_smallImageList.Detach();  in your destructor 
	if( !m_smallImageList.Attach(hSystemSmallImageList) )
		AfxMessageBox("Image attach error!"); 

	//do the same for the large 
	hSystemLargeImageList = (HIMAGELIST)SHGetFileInfo((LPCTSTR)_T("C:\\"), 0, &lsfi, sizeof(SHFILEINFO), SHGFI_SYSICONINDEX | SHGFI_ICON);
	m_largeImageList.Attach(hSystemLargeImageList);
}

//�]�w���A�C������
void CMainFrame::SetStatusText(int pane, CString str)
{
	//pane = status line index
	m_wndStatusBar.SetPaneText(pane, str);
}
