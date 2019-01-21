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

//�]�wList���������Ҧ�(Report)
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

//��l��List��������D
void CReportView::OnInitialUpdate()
{
	CListView::OnInitialUpdate();

	//Begin �إ߼��D���(Column)
	LV_COLUMN col;
	col.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
	col.fmt = LVCFMT_RIGHT;
	col.iSubItem = 0;
	col.pszText = "�s��";
	col.cx = 80;
	GetListCtrl().InsertColumn(0, &col);

	col.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
	col.fmt = LVCFMT_RIGHT;
	col.iSubItem = 1;
	col.pszText = "�줸��m(DEC)";
	col.cx = 100;
	GetListCtrl().InsertColumn(1, &col);

	col.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
	col.fmt = LVCFMT_RIGHT;
	col.iSubItem = 2;
	col.pszText = "�줸��m(HEX)";
	col.cx = 100;
	GetListCtrl().InsertColumn(2, &col);

	col.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
	col.fmt = LVCFMT_CENTER;
	col.iSubItem = 3;
	col.pszText = "�ɮ� 1 (HEX)";
	col.cx = 100;
	GetListCtrl().InsertColumn(3, &col);

	col.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
	col.fmt = LVCFMT_CENTER;
	col.iSubItem = 4;
	col.pszText = "�ɮ� 2 (HEX)";
	col.cx = 100;
	GetListCtrl().InsertColumn(4, &col);

	col.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
	col.fmt = LVCFMT_CENTER;
	col.iSubItem = 5;
	col.pszText = "�ɮ� 1 (NOR)";
	col.cx = 100;
	GetListCtrl().InsertColumn(5, &col);

	col.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
	col.fmt = LVCFMT_CENTER;
	col.iSubItem = 6;
	col.pszText = "�ɮ� 2 (NOR)";
	col.cx = 100;
	GetListCtrl().InsertColumn(6, &col);
	//End �إ߼��D���(Column)

	//��ListCtrl���}�G�@�I
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
		//���o�줸��m(DEC)����
		CString strOffset = GetListCtrl().GetItemText(idx, 1);
		int offset = atoi(strOffset);

		//����ɮפ��P���B
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
	//�s�W�@����ƦC
	int ndx = GetListCtrl().GetItemCount();

	//[1] ���J�s���ܲM��
	CString msg;
	msg.Format("%d", ndx+1);
	GetListCtrl().InsertItem(ndx, msg);

	//[2] ���J�줸��m(DEC)�ܲM��
	CString offset_dec;
	offset_dec.Format("%d", offset);
	GetListCtrl().SetItemText(ndx, 1, offset_dec);

	//[3] ���J�줸��m(HEX)�ܲM��
	CString offset_hex;
	offset_hex.Format("%08X", offset);
	GetListCtrl().SetItemText(ndx, 2, offset_hex);

	//[4] ���J file1(HEX) �����e�ܲM��
	CString data1;
	data1.Format("%02X", char1);
	GetListCtrl().SetItemText(ndx, 3, data1);

	//[5] ���J file2(HEX) �����e�ܲM��
	CString data2;
	data2.Format("%02X", char2);
	GetListCtrl().SetItemText(ndx, 4, data2);

	//[6] ���J file1(NOR) �����e�ܲM��
	data1.Format("%c", char1);
	GetListCtrl().SetItemText(ndx, 5, data1);

	//[7] ���J file2(NOR) �����e�ܲM��
	data2.Format("%c", char2);
	GetListCtrl().SetItemText(ndx, 6, data2);
}

