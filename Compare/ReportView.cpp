// ReportView.cpp : implementation of the CReportView class
//

#include "stdafx.h"
#include "Compare.h"
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
// CReportView

IMPLEMENT_DYNCREATE(CReportView, CListView)

BEGIN_MESSAGE_MAP(CReportView, CListView)
	//{{AFX_MSG_MAP(CReportView)
	ON_WM_LBUTTONDBLCLK()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CListView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CListView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CListView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CReportView construction/destruction

CReportView::CReportView()
{
	// TODO: add construction code here

}

CReportView::~CReportView()
{
}

//設定List控制項為報表模式(Report)
BOOL CReportView::PreCreateWindow(CREATESTRUCT& cs)
{
	CListView::PreCreateWindow(cs);
	cs.style |= LVS_REPORT | LVS_SINGLESEL | WS_VISIBLE;

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CReportView drawing

void CReportView::OnDraw(CDC* pDC)
{
	CCompareDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}

//初始化List控制項的標題
void CReportView::OnInitialUpdate()
{
	CListView::OnInitialUpdate();

	//Begin 建立標題欄位(Column)
	LV_COLUMN col;
	col.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
	col.fmt = LVCFMT_RIGHT;
	col.iSubItem = 0;
	col.pszText = "編號";
	col.cx = 80;
	GetListCtrl().InsertColumn(0, &col);

	col.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
	col.fmt = LVCFMT_RIGHT;
	col.iSubItem = 1;
	col.pszText = "位元位置(DEC)";
	col.cx = 100;
	GetListCtrl().InsertColumn(1, &col);

	col.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
	col.fmt = LVCFMT_RIGHT;
	col.iSubItem = 2;
	col.pszText = "位元位置(HEX)";
	col.cx = 100;
	GetListCtrl().InsertColumn(2, &col);

	col.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
	col.fmt = LVCFMT_CENTER;
	col.iSubItem = 3;
	col.pszText = "檔案 1 (HEX)";
	col.cx = 100;
	GetListCtrl().InsertColumn(3, &col);

	col.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
	col.fmt = LVCFMT_CENTER;
	col.iSubItem = 4;
	col.pszText = "檔案 2 (HEX)";
	col.cx = 100;
	GetListCtrl().InsertColumn(4, &col);

	col.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
	col.fmt = LVCFMT_CENTER;
	col.iSubItem = 5;
	col.pszText = "檔案 1 (NOR)";
	col.cx = 100;
	GetListCtrl().InsertColumn(5, &col);

	col.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
	col.fmt = LVCFMT_CENTER;
	col.iSubItem = 6;
	col.pszText = "檔案 2 (NOR)";
	col.cx = 100;
	GetListCtrl().InsertColumn(6, &col);
	//End 建立標題欄位(Column)

	//把ListCtrl做漂亮一點
	DWORD rgb = GetSysColor(COLOR_WINDOW);
	BYTE r = GetRValue(rgb) - 30;
	BYTE g = GetGValue(rgb) - 30;
	BYTE b = GetBValue(rgb) - 30;
	GetListCtrl().SetTextBkColor(RGB(r, g, b));
}

/////////////////////////////////////////////////////////////////////////////
// CReportView printing

BOOL CReportView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CReportView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CReportView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CReportView diagnostics

#ifdef _DEBUG
void CReportView::AssertValid() const
{
	CListView::AssertValid();
}

void CReportView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}

CCompareDoc* CReportView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCompareDoc)));
	return (CCompareDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CReportView message handlers

void CReportView::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	CCompareDoc* doc = (CCompareDoc*)GetDocument();
	if( doc )
	{
		int idx = GetListCtrl().GetSelectionMark();
		//取得位元位置(DEC)之值
		CString strOffset = GetListCtrl().GetItemText(idx, 1);
		int offset = atoi(strOffset);

		//選取檔案不同之處
		if( doc->m_is_hex )
		{
			//By Hex
			doc->m_pFirstView->SelectOffsetByHex(offset);
			doc->m_pSecondView->SelectOffsetByHex(offset);
		}
		else
		{
			//By Normal
			doc->m_pFirstView->SelectOffsetByNormal(offset);
			doc->m_pSecondView->SelectOffsetByNormal(offset);
		}
	}

	CListView::OnLButtonDblClk(nFlags, point);
}

/////////////////////////////////////////////////////////////////////////////
// Class Member Function

void CReportView::AddReportItem(int offset, byte char1, byte char2)
{
	//新增一筆資料列
	int ndx = GetListCtrl().GetItemCount();

	//[1] 插入編號至清單
	CString msg;
	msg.Format("%d", ndx+1);
	GetListCtrl().InsertItem(ndx, msg);

	//[2] 插入位元位置(DEC)至清單
	CString offset_dec;
	offset_dec.Format("%d", offset);
	GetListCtrl().SetItemText(ndx, 1, offset_dec);

	//[3] 插入位元位置(HEX)至清單
	CString offset_hex;
	offset_hex.Format("%08X", offset);
	GetListCtrl().SetItemText(ndx, 2, offset_hex);

	//[4] 插入 file1(HEX) 的內容至清單
	CString data1;
	data1.Format("%02X", char1);
	GetListCtrl().SetItemText(ndx, 3, data1);

	//[5] 插入 file2(HEX) 的內容至清單
	CString data2;
	data2.Format("%02X", char2);
	GetListCtrl().SetItemText(ndx, 4, data2);

	//[6] 插入 file1(NOR) 的內容至清單
	data1.Format("%c", char1);
	GetListCtrl().SetItemText(ndx, 5, data1);

	//[7] 插入 file2(NOR) 的內容至清單
	data2.Format("%c", char2);
	GetListCtrl().SetItemText(ndx, 6, data2);
}

