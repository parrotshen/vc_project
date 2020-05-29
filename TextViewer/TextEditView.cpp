// TextEditView.cpp : implementation file
//

#include "stdafx.h"
#include "TextViewer.h"
#include "TextViewerDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define BlockSize 8192

/////////////////////////////////////////////////////////////////////////////
// CTextEditView

IMPLEMENT_DYNCREATE(CTextEditView, CEditView)

CTextEditView::CTextEditView()
{
	m_pFRDlg  = NULL;
	m_Find    = "";
	m_Replace = "";
	m_FR      = TRUE;
}

CTextEditView::~CTextEditView()
{
	if(m_pFRDlg != NULL)
	{
		delete m_pFRDlg;
		m_pFRDlg = NULL;
	}
}

//���U FindText ���T��
static UINT WM_FINDREPLACE = ::RegisterWindowMessage( FINDMSGSTRING );
BEGIN_MESSAGE_MAP(CTextEditView, CEditView)
	//{{AFX_MSG_MAP(CTextEditView)
	ON_WM_KEYUP()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE(WM_FINDREPLACE, OnFindReplace)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTextEditView drawing

void CTextEditView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CTextEditView diagnostics

#ifdef _DEBUG
void CTextEditView::AssertValid() const
{
	CEditView::AssertValid();
}

void CTextEditView::Dump(CDumpContext& dc) const
{
	CEditView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTextEditView message handlers

void CTextEditView::OnInitialUpdate() 
{
	CEditView::OnInitialUpdate();
	
	//Begin - Change Font to Fixedsys
	m_Font.DeleteObject();
	m_Font.CreateFont(
		-16,                  // nHeight
		0,                    // nWidth
		0,                    // nEscapement
		0,                    // nOrientation
		FW_NORMAL,            // nWeight
		FALSE,                // bItalic
		FALSE,                // bUnderline
		0,                    // cStrikeOut
		DEFAULT_CHARSET,      // nCharSet
		OUT_DEFAULT_PRECIS,   // nOutPrecision
		CLIP_DEFAULT_PRECIS,  // nClipPrecision
		DEFAULT_QUALITY,      // nQuality
		DEFAULT_PITCH,        // nPitchAndFamily
		"Fixedsys"            // lpszFacename
	);
	GetEditCtrl().SetFont(&m_Font, TRUE);
	//End   - Change Font to Fixedsys

	//Begin - �]�w Edit Control �����
	GetEditCtrl().SetMargins(15, 0);
	//End   - �]�w Edit Control �����
}

//��L���ƥ�
void CTextEditView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	//���s�p���r�����
	if( GetEditCtrl().GetModify() )
	{
		CTextViewerDoc* doc = (CTextViewerDoc*)GetDocument();
		doc->m_Total = GetEditCtrl().GetLineCount();

		int line = GetEditCtrl().LineFromChar()+1;
		CString str;
		str.Format("%d / %d", line, doc->m_Total);
		doc->DisplayLines(str);
	}

	//���o�ثe��ЩҦb�����
	// [Upper] nChar = 38
	// [Down]  nChar = 40
	if(nChar == 38 || nChar == 40)
	{
		CTextViewerDoc* doc = (CTextViewerDoc*)GetDocument();

		int line = GetEditCtrl().LineFromChar()+1;
		CString str;
		str.Format("%d / %d", line, doc->m_Total);
		doc->DisplayLines(str);
	}

	CEditView::OnKeyUp(nChar, nRepCnt, nFlags);
}

//�ƹ����ƥ�
void CTextEditView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	CTextViewerDoc* doc = (CTextViewerDoc*)GetDocument();
	int total = doc->m_Total;

	int line = GetEditCtrl().LineFromChar()+1;
	CString str;
	str.Format("%d / %d", line, total);
	doc->DisplayLines(str);

	CEditView::OnLButtonUp(nFlags, point);
}

LRESULT CTextEditView::OnFindReplace(WPARAM wParam, LPARAM lParam)
{
	//1 - ������ܲ�
	if( m_pFRDlg->IsTerminating() )
	{
		// m_pFRDlg->DestroyWindow();
		m_pFRDlg = NULL;
		return 0;
	}

	//�M�䪺�ؼ�
	m_Find = m_pFRDlg->GetFindString();
	//���N�����G
	m_Replace = m_pFRDlg->GetReplaceString();
	//�M�䪺��V
	BOOL direction = m_pFRDlg->SearchDown();
	//�r���j�p�g�O�_����
	BOOL match_case = m_pFRDlg->MatchCase();

	//2 - �M��r��(Find)
	if( m_pFRDlg->FindNext() )
	{
		if( !FindText(m_Find, direction, match_case) )
		{
			AfxMessageBox("�䤣��r��!!");
			return 0;
		}
	}

	//3 - ���N�r��(Replace)
	if( !m_FR )
	{
		direction = FR_DOWN;

		if( m_pFRDlg->ReplaceCurrent() )
		{
			//�q��Хثe��m�}�l���N
			int pt1, pt2;
			GetEditCtrl().GetSel(pt1, pt2);
			GetEditCtrl().SetSel(pt1, pt1, TRUE);

			if( !FindText(m_Find, direction, match_case) )
			{
				AfxMessageBox("�䤣��r��!!");
				return 0;
			}

			GetEditCtrl().ReplaceSel( m_Replace );
			return 0;
		}

		if( m_pFRDlg->ReplaceAll() )
		{
			//�h�^�Ĥ@��A�q�Y�}�l�j�M�_
			GetEditCtrl().SetSel(0, 0, TRUE);
			int count = 0;
			CString msg;

			if( !FindText(m_Find, direction, match_case) )
			{
				AfxMessageBox("�䤣��r��!!");
				return 0;
			}

			do
			{
				GetEditCtrl().ReplaceSel(m_Replace, TRUE);
				count++;
			}
			while( FindText(m_Find, direction, match_case) );

			msg.Format("�������N %d �Ӧr��!!", count);
			AfxMessageBox(msg);
			return 0;
		}
	}

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// Class Member Methods

//��l�� CFindReplaceDialog
void CTextEditView::InitFindReplaceDlg(BOOL style)
{
	//���� Find-Replace Dialog
	if(m_pFRDlg == NULL)
	{
		m_pFRDlg = new CFindReplaceDialog(); // Must be created on the heap
		m_pFRDlg->Create(style, m_Find, m_Replace, FR_DOWN, this);
		m_FR = style;
	}
}

//���U Goto �����s
void CTextEditView::Button_GotoLines(int jump)
{
	//���ܫ��w�����
	CTextViewerDoc* doc = (CTextViewerDoc*)GetDocument();
	int line1 = GetEditCtrl().LineFromChar()+1;
	GetEditCtrl().LineScroll( line1*(-1) );
	GetEditCtrl().LineScroll( jump-1 );

	//�N���w����Ƥϥ�
	int index1 = GetEditCtrl().LineIndex( jump-1 );
	int index2 = GetEditCtrl().LineIndex( jump );
	GetEditCtrl().SetSel(index1, index2, TRUE);

	//��s���A�C����T
	int total = doc->m_Total;
	int line2 = GetEditCtrl().LineFromChar()+1;
	CString str;
	str.Format("%d / %d", line2, total);
	doc->DisplayLines(str);
}

//���U Find �����s
void CTextEditView::Button_FindText(CString target)
{
	// FindText(LPCTSTR lpszFind, BOOL bNext, BOOL bCase);
	//    lpszFind = The text to be found
	//    bNext    = Direction to Up/Down
	//    bCase    = Is case sensitive
	if( !FindText(target, TRUE, TRUE) )
		AfxMessageBox("�䤣��r��!!");
}

//���� Edit Control ���r��
void CTextEditView::ChangeEditFont()
{
	//Begin - Change Edit Control Font
	CFontDialog dlg;
	if(dlg.DoModal() == IDOK)
	{
		LOGFONT logfont;
		dlg.GetCurrentFont(&logfont);

		m_Font.DeleteObject();
		m_Font.CreateFontIndirect(&logfont);
		GetEditCtrl().SetFont(&m_Font, TRUE);
	}
	//End   - Change Edit Control Font
}

//�x�s Edit Control �����e
void CTextEditView::KeepEditContent()
{
	if( !GetEditCtrl().GetModify() )
		return;

	//���o Edit Control ����r���e
	HLOCAL h = GetEditCtrl().GetHandle();
	LPCTSTR lpszText = (LPCTSTR) ::LocalLock(h);
	::LocalUnlock(h);

	//���o Edit Control ����r���e������
	CTextViewerDoc* doc = (CTextViewerDoc*)GetDocument();
	doc->m_Text_Length = GetWindowTextLength();

	//Begin - �x�s�ثe��ܪ���r
	if(doc->m_Text_Content != NULL)
	{
		delete doc->m_Text_Content;
		doc->m_Text_Content = NULL;
	}

	int length = doc->m_Text_Length;
	byte* content = new byte[length+1];
	memset(content, 0x00, length+1);
	memcpy(content, lpszText, length);

	doc->m_Text_Content = (LPTSTR)content;
	//delete content;
	//End   - �x�s�ثe��ܪ���r
}

//�H���`�覡���
void CTextEditView::DisplayByNormal()
{
	CTextViewerDoc* doc = (CTextViewerDoc*)GetDocument();
	GetEditCtrl().SetReadOnly( doc->m_Is_ReadOnly );

	//��ܵ��G
	SetWindowText( doc->m_Text_Content );
}

//�H�Q���i������
void CTextEditView::DisplayByHex()
{
	CTextViewerDoc* doc = (CTextViewerDoc*)GetDocument();
	GetEditCtrl().SetReadOnly( TRUE );
	SetWindowText( "" );
	CWaitCursor busy;

	//���o Edit ����r���e������
	unsigned long len = doc->m_Text_Length;
	if(len == 0) return;
	//���o Edit ����r���e
	LPTSTR text = doc->m_Text_Content;

	//�N�ɤ��Φ��j�p�� BlockSize ���϶��A�A�U�O�B�z
	byte block[BlockSize];
	int  EndOfText;
	unsigned long i, j, k;
	unsigned long Q, R, Loop, Last;
	CString result, offset, hex, content;

	//�p��϶��`��(Loop)�γ̫�@�Ӱ϶����j�p(Last)
	Last = len % BlockSize;
	Loop = (len / BlockSize) + ((Last == 0) ? 0 : 1);

	//Begin - �}�l�B�z�U�Ӱ϶�
	len = BlockSize;
	for(k=0; k<Loop; k++)
	{
		//�P�_�O�_���̫�@�Ӱ϶�
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
				//1 - �s�� offset
				offset.Format("%08xh: ", BlockSize*k+i*16);
				//2 - �s�� hex
				memcpy(buf, block+i*16, 16);
				hex.Format(" %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X ; ", buf[0], buf[1], buf[2], buf[3], buf[4], buf[5], buf[6], buf[7], buf[8], buf[9], buf[10], buf[11], buf[12], buf[13], buf[14], buf[15]);
				//3 - �s�� content
				for(j=0; j<16; j++)
				{
					if(buf[j] < 0x20)
						buf[j] = 0x2E;
				}
				content.Format(" %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c\r\n", buf[0], buf[1], buf[2], buf[3], buf[4], buf[5], buf[6], buf[7], buf[8], buf[9], buf[10], buf[11], buf[12], buf[13], buf[14], buf[15]);
				//4 - �X������
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

			//1 - �s�� offset
			offset.Format("%08xh: ", BlockSize*k+Q*16);
			//2 - �s�� hex
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
			//3 - �s�� content
			content = " ";
			for(i=0; i<R; i++)
			{
				if(buf[i] < 0x20)
					str = 0x2E;
				else
					str.Format("%c", buf[i]);
				content += str;
			}
			//4 - �X������
			result += offset + hex + content;

			delete buf;
		}

		//��ܵ��G
		EndOfText = GetWindowTextLength();
		GetEditCtrl().SetSel(EndOfText, EndOfText, TRUE);
		GetEditCtrl().ReplaceSel(result);
	}
	//End   - �}�l�B�z�U�Ӱ϶�

	//���^����
	//int Line = GetEditCtrl().LineFromChar()+1;
	//GetEditCtrl().LineScroll( Line*(-1) );
	GetEditCtrl().SetSel(0, 0, FALSE);
}

//�H Windows ����榡���
void CTextEditView::BreakLineByWindows()
{
	char OneChar;
	CString UnixFormat;
	CString WindowsFormat = "";
	CWaitCursor busy;

	//�Y�ثe���Q���i����ܫh��_���`���
	CTextViewerDoc* doc = (CTextViewerDoc*)GetDocument();
	if( doc->m_Is_Hex )
	{
		doc->m_Is_Hex = FALSE;
		GetEditCtrl().SetReadOnly( doc->m_Is_ReadOnly );
		UnixFormat = doc->m_Text_Content;
	}
	else
	{
		GetEditCtrl().GetWindowText(UnixFormat);
	}

	//Windows ������榡�� CR(0x0D)-LF(0x0A)
	for(int i=0; i<UnixFormat.GetLength(); i++)
	{
		OneChar = UnixFormat.GetAt(i);
		if(OneChar == 0x0A)
		{
			WindowsFormat += "\r\n";
		}
		else
		{
			WindowsFormat += OneChar;
		}
	}
	GetEditCtrl().SetWindowText(WindowsFormat);
}

//�H Unix �_��榡���
void CTextEditView::BreakLineByUnix()
{
	char OneChar, NextChar;
	CString WindowsFormat;
	CString UnixFormat = "";
	CWaitCursor busy;

	//�Y�ثe���Q���i����ܫh��_���`���
	CTextViewerDoc* doc = (CTextViewerDoc*)GetDocument();
	if( doc->m_Is_Hex )
	{
		doc->m_Is_Hex = FALSE;
		GetEditCtrl().SetReadOnly( doc->m_Is_ReadOnly );
		WindowsFormat = doc->m_Text_Content;
	}
	else
	{
		GetEditCtrl().GetWindowText(WindowsFormat);
	}

	//Unix ������榡�� LF(0x0A)
	for(int i=0; i<WindowsFormat.GetLength(); i++)
	{
		OneChar = WindowsFormat.GetAt(i);
		if(OneChar == 0x0D)
		{
			NextChar = WindowsFormat.GetAt(i+1);
			if(NextChar == 0x0A)
			{
				UnixFormat += "\n";
				i++;
			}
		}
		else
		{
			UnixFormat += OneChar;
		}
	}
	GetEditCtrl().SetWindowText(UnixFormat);
}
