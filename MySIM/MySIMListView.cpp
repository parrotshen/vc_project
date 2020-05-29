// MySIMListView.cpp : implementation file
//

#include "stdafx.h"
#include "MySIM.h"
#include "MySIMListView.h"
#include "MySIMEfDataDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMySIMListView

IMPLEMENT_DYNCREATE(CMySIMListView, CListView)

CMySIMListView::CMySIMListView()
{
}

CMySIMListView::~CMySIMListView()
{
}


BEGIN_MESSAGE_MAP(CMySIMListView, CListView)
	//{{AFX_MSG_MAP(CMySIMListView)
	ON_NOTIFY_REFLECT(NM_DBLCLK, OnDblclk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMySIMListView drawing

void CMySIMListView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CMySIMListView diagnostics

#ifdef _DEBUG
void CMySIMListView::AssertValid() const
{
	CListView::AssertValid();
}

void CMySIMListView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMySIMListView message handlers

BOOL CMySIMListView::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: Add your specialized code here and/or call the base class
	//設定 List 控制項為報表模式(Report)
	cs.style |= LVS_REPORT | LVS_SINGLESEL | WS_VISIBLE;

	return CListView::PreCreateWindow(cs);
}

void CMySIMListView::OnInitialUpdate() 
{
	CListView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	//Begin: 建立標題欄位(Column)
	LV_COLUMN col;
	col.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
	col.fmt = LVCFMT_LEFT;
	col.iSubItem = 0;
	col.pszText = "EF Name";
	col.cx = 150;
	GetListCtrl().InsertColumn(0, &col);

	col.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
	col.fmt = LVCFMT_LEFT;
	col.iSubItem = 1;
	col.pszText = "ID";
	col.cx = 100;
	GetListCtrl().InsertColumn(1, &col);

	col.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
	col.fmt = LVCFMT_LEFT;
	col.iSubItem = 2;
	col.pszText = "Length";
	col.cx = 100;
	GetListCtrl().InsertColumn(4, &col);

	col.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
	col.fmt = LVCFMT_LEFT;
	col.iSubItem = 3;
	col.pszText = "Data";
	col.cx = 400;
	GetListCtrl().InsertColumn(5, &col);
	//End:   建立標題欄位(Column)

	//設定顯示整列選取
	GetListCtrl().SetExtendedStyle( GetListCtrl().GetExtendedStyle() | LVS_EX_FULLROWSELECT );
}

void CMySIMListView::OnDblclk(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	int ndx = GetListCtrl().GetSelectionMark();

	if (ndx >= 0)
	{
		CMySIMEfDataDlg dlg;
		dlg.m_fid  = GetListCtrl().GetItemText(ndx, 1);
		dlg.m_data = GetListCtrl().GetItemText(ndx, 3);
		dlg.DoModal();
	}

	*pResult = 0;
}


/////////////////////////////////////////////////////////////////////////////
// CMySIMListView member methods

void CMySIMListView::DisplayEF(struct _tSimFileSys *pFile)
{
	//Begin: 變數區
	tEfData *pData;
	CString  string;
	CString  append;
	int index = 0;
	//End:   變數區

	//清除目前所有顯示的項目(Row)
	GetListCtrl().DeleteAllItems();

	for (pData=pFile->info->dataEf; pData != NULL; pData=pData->next)
	{
		// [EF Name]
		string.Format("%s", pFile->name);
		GetListCtrl().InsertItem(index, string);

		// [ID]
		string.Format("%02X%02X", pFile->fid[0], pFile->fid[1]);
		GetListCtrl().SetItemText(index, 1, string);

		// [Length]
		string.Format("%d", pData->dataLen);
		GetListCtrl().SetItemText(index, 2, string);

		// [Data]
		string.Empty();
		if ( pData->data )
		{
			for (int i=0; i<pData->dataLen; i++)
			{
				append.Format("%02X ", pData->data[i]);
				string += append;
			}
		}
		GetListCtrl().SetItemText(index, 3, string);

		index++;
	}
}

