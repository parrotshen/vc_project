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
	m_ColumnTime[0] = 1;
	m_ColumnTime[1] = 2;
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
	GetListCtrl().SetExtendedStyle( style );

	// TODO: Add your specialized code here and/or call the base class
	//Begin: å»ºç?æ¨™é?æ¬„ä?(Column)
	LV_COLUMN col;
	col.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
	col.fmt = LVCFMT_CENTER;
	col.iSubItem = 0;
	col.pszText = "NO";
	col.cx = 60;
	GetListCtrl().InsertColumn(0, &col);

	col.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
	col.fmt = LVCFMT_CENTER;
	col.iSubItem = 1;
	col.pszText = "Start time";
	col.cx = 120;
	GetListCtrl().InsertColumn(1, &col);

	col.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
	col.fmt = LVCFMT_CENTER;
	col.iSubItem = 2;
	col.pszText = "End time";
	col.cx = 120;
	GetListCtrl().InsertColumn(2, &col);
	//End:   å»ºç?æ¨™é?æ¬„ä?(Column)
}


/////////////////////////////////////////////////////////////////////////////
// é¡žåˆ¥?„æ??¡å‡½å¼?

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
		m_ColumnTime[0] = 1;
		m_ColumnTime[1] = 2;
	}
}

void CLogCheckListView::AddListColumn(int column, char *pLabel)
{
	LV_COLUMN param;
	int width = 80;

	if (strcmp(pLabel, "Start time") == 0)
	{
		m_ColumnTime[0] = column;
		width = 120;
	}
	else if (strcmp(pLabel, "End time") == 0)
	{
		m_ColumnTime[1] = column;
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

void CLogCheckListView::UpdateStartTime(void)
{
	CString string = CTime::GetCurrentTime().Format("%H:%M:%S");

	GetListCtrl().SetItemText(m_ItemCurr, m_ColumnTime[0], string);
}

void CLogCheckListView::UpdateEndTime(void)
{
	CString string = CTime::GetCurrentTime().Format("%H:%M:%S");

	GetListCtrl().SetItemText(m_ItemCurr, m_ColumnTime[1], string);
}

