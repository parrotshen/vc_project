// LogCheckListView.cpp : implementation file
//

#include "stdafx.h"
#include "LogCheck.h"
#include "LogCheckListView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLogCheckListView

IMPLEMENT_DYNCREATE(CLogCheckListView, CListView)

CLogCheckListView::CLogCheckListView()
{
	m_ItemCurr = 0;
	m_ItemNext = 0;
	m_ColumnTime = 1;
}

CLogCheckListView::~CLogCheckListView()
{
}


BEGIN_MESSAGE_MAP(CLogCheckListView, CListView)
	//{{AFX_MSG_MAP(CLogCheckListView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLogCheckListView drawing

void CLogCheckListView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CLogCheckListView diagnostics

#ifdef _DEBUG
void CLogCheckListView::AssertValid() const
{
	CListView::AssertValid();
}

void CLogCheckListView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CLogCheckListView message handlers

BOOL CLogCheckListView::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	//return CListView::PreCreateWindow(cs);

	CListView::PreCreateWindow(cs);
	cs.style |= LVS_REPORT | LVS_SINGLESEL | WS_VISIBLE;

	return TRUE;
}

void CLogCheckListView::OnInitialUpdate() 
{
	CListView::OnInitialUpdate();
	
	DWORD style = GetListCtrl().GetExtendedStyle();
	style |= LVS_EX_FULLROWSELECT;
	style |= LVS_EX_GRIDLINES;
	GetListCtrl().SetExtendedStyle( style );

	// TODO: Add your specialized code here and/or call the base class
	//Begin: 建立標題欄位(Column)
	LV_COLUMN col;
	col.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
	col.fmt = LVCFMT_LEFT;
	col.iSubItem = 0;
	col.pszText = "NO";
	col.cx = 60;
	GetListCtrl().InsertColumn(0, &col);

	col.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
	col.fmt = LVCFMT_CENTER;
	col.iSubItem = 1;
	col.pszText = "Time";
	col.cx = 100;
	GetListCtrl().InsertColumn(1, &col);

	//End:   建立標題欄位(Column)
}


/////////////////////////////////////////////////////////////////////////////
// 類別的成員函式

void CLogCheckListView::CleanListColumn()
{
	CHeaderCtrl *pHeader = (CHeaderCtrl *)GetListCtrl().GetHeaderCtrl();
	int count = 0;
	int i;

	CleanListItem();

	if ( pHeader )
	{
		count = pHeader->GetItemCount();
		for (i=1; i<count; i++)
		{
			/* Keep the first column */
			GetListCtrl().DeleteColumn( 1 );
		}
	}
}

void CLogCheckListView::AddListColumn(int column, char *pLabel)
{
	LV_COLUMN param;
	int width = 80;

	if (strcmp(pLabel, "Time") == 0)
	{
		m_ColumnTime = column;
		width = 120;
	}

	param.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
	param.fmt = LVCFMT_CENTER;
	param.iSubItem = column;
	param.pszText = pLabel;
	param.cx = width;
	GetListCtrl().InsertColumn(column, &param);
}

void CLogCheckListView::CleanListItem()
{
	GetListCtrl().DeleteAllItems();
	m_ItemCurr = 0;
	m_ItemNext = 0;
}

void CLogCheckListView::AddListItem()
{
	//int ndx = GetListCtrl().GetItemCount();
	CString string;

	m_ItemCurr = m_ItemNext;	
	m_ItemNext = (m_ItemCurr + 1);
	string.Format("%d", m_ItemNext);

	// Add New Row
	GetListCtrl().InsertItem(m_ItemCurr, string);
	GetListCtrl().SetItemText(m_ItemCurr, 1, (LPCTSTR)"OK");
}

void CLogCheckListView::UpdateListItem(int column, char *pWord)
{
	GetListCtrl().SetItemText(m_ItemCurr, column, (LPCTSTR)pWord);
}

void CLogCheckListView::UpdateTime()
{
	CString t = CTime::GetCurrentTime().Format("%H:%M:%S");

	GetListCtrl().SetItemText(m_ItemCurr, m_ColumnTime, t);
}

