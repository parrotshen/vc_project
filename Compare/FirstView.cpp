// FirstView.cpp : implementation file
//

#include "stdafx.h"
#include "Compare.h"
#include "CompareDoc.h"
#include "FirstView.h"
#include "SecondView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFirstView

IMPLEMENT_DYNCREATE(CFirstView, CEditView)

CFirstView::CFirstView()
{
}

CFirstView::~CFirstView()
{
}


BEGIN_MESSAGE_MAP(CFirstView, CEditView)
	//{{AFX_MSG_MAP(CFirstView)
	ON_WM_KEYUP()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFirstView drawing

void CFirstView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CFirstView diagnostics

#ifdef _DEBUG
void CFirstView::AssertValid() const
{
	CEditView::AssertValid();
}

void CFirstView::Dump(CDumpContext& dc) const
{
	CEditView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CFirstView message handlers

void CFirstView::OnInitialUpdate() 
{
	CEditView::OnInitialUpdate();

	//Begin Change Font to Fixedsys
	m_font.DeleteObject();
	m_font.CreateFont(
		-16,                   // nHeight
		0,                     // nWidth
		0,                     // nEscapement
		0,                     // nOrientation
		FW_NORMAL,             // nWeight
		FALSE,                 // bItalic
		FALSE,                 // bUnderline
		0,                     // cStrikeOut
		DEFAULT_CHARSET,       // nCharSet
		OUT_DEFAULT_PRECIS,    // nOutPrecision
		CLIP_DEFAULT_PRECIS,   // nClipPrecision
		DEFAULT_QUALITY,       // nQuality
		DEFAULT_PITCH,         // nPitchAndFamily
		"Fixedsys"             // lpszFacename
	);
	GetEditCtrl().SetFont(&m_font, TRUE);
	//End Change Font to Fixedsys
}

void CFirstView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	CCompareDoc* doc = (CCompareDoc*)GetDocument();
	if( doc->m_is_cursor )
	{
		//使兩個 View 的游標同步
		int nST, nEd;
		GetEditCtrl().GetSel(nST, nEd);
		doc->m_pSecondView->GetEditCtrl().SetSel(nST, nEd+1);
	}

	CEditView::OnKeyUp(nChar, nRepCnt, nFlags);
}

void CFirstView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	CCompareDoc* doc = (CCompareDoc*)GetDocument();
	if( doc->m_is_cursor )
	{
		//使兩個 View 的游標同步
		int nST, nEd;
		GetEditCtrl().GetSel(nST, nEd);
		doc->m_pSecondView->GetEditCtrl().SetSel(nST, nEd+1);
	}

	CEditView::OnLButtonUp(nFlags, point);
}

/////////////////////////////////////////////////////////////////////////////
// Class Member Function

void CFirstView::DisplayByHex() 
{
	CCompareDoc* doc = (CCompareDoc*)GetDocument();
	SetWindowText( "" );
	//CWaitCursor busy;

	//取得 Edit 的文字內容
	LPTSTR text = (LPTSTR)doc->m_file1;
	//取得 Edit 的文字內容的長度
	unsigned long len = doc->m_filelen1;
	if(len == 0)
		return;

	//將檔切割成大小為 BlockSize 的區塊，再各別處理
	byte block[BlockSize];
	int EndOfText;
	unsigned long i, j, k, Q, R, Loop, Last;
	CString result, offset, hex, content;

	//計算區塊總數(Loop)及最後一個區塊的大小(Last)
	Last = len % BlockSize;
	Loop = (len / BlockSize) + ((Last == 0) ? 0 : 1);
	len = BlockSize;
	for(k=0; k<Loop; k++)
	{
		//判斷是否為最後一個區塊
		if((k == Loop-1) && (Last != 0))
			len = Last;
		memset(block, 0x00, BlockSize);
		memcpy(block, text+BlockSize*k, len);

		// Q for quotient & R for remainder
		Q = len / 16;
		R = len % 16;
		result = "";

		if(Q != 0)
		{
			byte* buf = new byte[16];
			memset(buf, 0x00, 16);
			for(i=0; i<Q; i++)
			{
				//1 - 編輯 offset
				offset.Format("%08xh: ", BlockSize*k+i*16);
				//2 - 編輯 hex
				memcpy(buf, block+i*16, 16);
				hex.Format(" %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X ; ", buf[0], buf[1], buf[2], buf[3], buf[4], buf[5], buf[6], buf[7], buf[8], buf[9], buf[10], buf[11], buf[12], buf[13], buf[14], buf[15]);
				//3 - 編輯 content
				for(j=0; j<16; j++)
				{
					if(buf[j] < 0x20)
						buf[j] = 0x2E;
				}
				content.Format(" %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c\r\n", buf[0], buf[1], buf[2], buf[3], buf[4], buf[5], buf[6], buf[7], buf[8], buf[9], buf[10], buf[11], buf[12], buf[13], buf[14], buf[15]);
				//4 - 合成全文
				result += offset + hex + content;
			}
			delete buf;
		}

		if(R != 0)
		{
			CString str;
			byte* buf = new byte[R];
			memset(buf, 0x00, R);
			memcpy(buf, block+Q*16, R);

			//1 - 編輯 offset
			offset.Format("%08xh: ", BlockSize*k+Q*16);
			//2 - 編輯 hex
			hex = " ";
			for(i=0; i<16; i++)
			{
				if(i < R)
					str.Format("%02X ", buf[i]);
				else
					str = "   ";
				hex += str;
			}
			hex += "; ";
			//3 - 編輯 content
			content = " ";
			for(i=0; i<R; i++)
			{
				if(buf[i] < 0x20)
					str = 0x2E;
				else
					str.Format("%c", buf[i]);
				content += str;
			}
			//4 - 合成全文
			result += offset + hex + content;

			delete buf;
		}

		//顯示結果
		EndOfText = GetWindowTextLength();
		GetEditCtrl().SetSel(EndOfText, EndOfText, TRUE);
		GetEditCtrl().ReplaceSel(result);
	}

	//捲回頁首
	int Line = GetEditCtrl().LineFromChar()+1;
	GetEditCtrl().LineScroll( Line*(-1) );
}

void CFirstView::DisplayByNormal() 
{
	CCompareDoc* doc = (CCompareDoc*)GetDocument();
	SetWindowText( "" );
	//CWaitCursor busy;

	//取得 Edit 的文字內容
	LPTSTR text = (LPTSTR)doc->m_file1;
	//取得 Edit 的文字內容的長度
	unsigned long len = doc->m_filelen1;
	if(len == 0)
		return;

	//將檔切割成大小為 BlockSize 的區塊，再各別處理
	byte block[BlockSize];
	int EndOfText;
	unsigned long i, k, Loop, Last;
	CString result, content;

	//計算區塊總數(Loop)及最後一個區塊的大小(Last)
	Last = len % BlockSize;
	Loop = (len / BlockSize) + ((Last == 0) ? 0 : 1);
	len = BlockSize;
	for(k=0; k<Loop; k++)
	{
		//判斷是否為最後一個區塊
		if((k == Loop-1) && (Last != 0))
			len = Last;
		memset(block, 0x00, BlockSize);
		memcpy(block, text+BlockSize*k, len);

		result = "";
		for(i=0; i<len; i++)
		{
			content.Format("%c", block[i]);
			result += content;
		}

		//顯示結果
		EndOfText = GetWindowTextLength();
		GetEditCtrl().SetSel(EndOfText, EndOfText, TRUE);
		GetEditCtrl().ReplaceSel(result);
	}

	//捲回頁首
	int Line = GetEditCtrl().LineFromChar()+1;
	GetEditCtrl().LineScroll( Line*(-1) );
}

void CFirstView::SelectOffsetByHex(int offset) 
{
	int Q, R;
	Q = offset / 16;
	R = offset % 16;
	if(Q != 0)
		Q = Q * 81;
	if(R != 0)
		R = 12 + R * 3;

	GetEditCtrl().SetSel(Q+R, Q+R+2);
}

void CFirstView::SelectOffsetByNormal(int offset) 
{
	GetEditCtrl().SetSel(offset, offset+1);
}
