// LogCheckEditView.cpp : implementation of the CLogCheckEditView class
//

#include "stdafx.h"
#include "LogCheck.h"
#include "LogCheckDoc.h"
#include "LogCheckEditView.h"
#include "LogCheckListView.h"
#include "MySerialIF.h"
#include <sys/stat.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define LOG_TIMER_ID 1


/////////////////////////////////////////////////////////////////////////////
// CLogCheckEditView

IMPLEMENT_DYNCREATE(CLogCheckEditView, CEditView)

BEGIN_MESSAGE_MAP(CLogCheckEditView, CEditView)
	// Begin of Serial Port Rx
	ON_MESSAGE(WM_COMM_RXCHAR, OnCommunication)
	// End   of Serial Port Rx

	//{{AFX_MSG_MAP(CLogCheckEditView)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CEditView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CEditView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CEditView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLogCheckEditView construction/destruction

CLogCheckEditView::CLogCheckEditView()
{
	// TODO: add construction code here
	m_AppDoc = NULL;

	m_lineCnt = 0;
	m_charCnt = 0;
	m_keywordCnt = 0;

	for (int i=0; i<2048; i++)
	{
		m_log[i][0] = 0x00;
	}
}

CLogCheckEditView::~CLogCheckEditView()
{
}

BOOL CLogCheckEditView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	BOOL bPreCreated = CEditView::PreCreateWindow(cs);
	cs.style &= ~(ES_AUTOHSCROLL|WS_HSCROLL);	// Enable word-wrapping

	return bPreCreated;
}

/////////////////////////////////////////////////////////////////////////////
// CLogCheckEditView drawing

void CLogCheckEditView::OnDraw(CDC* pDC)
{
	CLogCheckDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CLogCheckEditView printing

BOOL CLogCheckEditView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default CEditView preparation
	return CEditView::OnPreparePrinting(pInfo);
}

void CLogCheckEditView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	// Default CEditView begin printing.
	CEditView::OnBeginPrinting(pDC, pInfo);
}

void CLogCheckEditView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	// Default CEditView end printing
	CEditView::OnEndPrinting(pDC, pInfo);
}

/////////////////////////////////////////////////////////////////////////////
// CLogCheckEditView diagnostics

#ifdef _DEBUG
void CLogCheckEditView::AssertValid() const
{
	CEditView::AssertValid();
}

void CLogCheckEditView::Dump(CDumpContext& dc) const
{
	CEditView::Dump(dc);
}

CLogCheckDoc* CLogCheckEditView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CLogCheckDoc)));
	return (CLogCheckDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CLogCheckEditView message handlers

void CLogCheckEditView::OnInitialUpdate() 
{
	CEditView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	//Begin: Change Font to Fixedsys
	m_Font.DeleteObject();
	m_Font.CreateFont(
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
	GetEditCtrl().SetFont(&m_Font, TRUE);
	//End:   Change Font to Fixedsys

    struct _stat buf;
    if (_stat("Report", &buf) == -1)
    {
        if (CreateDirectory("Report", NULL) == 0)
        {
            AfxMessageBox("Fail to create directory: Report");
        }
    }

	m_AppDoc = (CLogCheckDoc *)GetDocument();
}

// Begin of Serial Port RX
LONG CLogCheckEditView::OnCommunication(WPARAM ch, LPARAM port)
{
	if (ch == 0x0A)
	{
		CHAR *pLog = m_log[m_lineCnt];

		/* Here is the end of line */
		m_log[m_lineCnt][ m_charCnt++ ] = '\r';
		m_log[m_lineCnt][ m_charCnt++ ] = '\n';
		m_log[m_lineCnt][ m_charCnt   ] = 0x00;
		m_charCnt = 0;
		if (m_lineCnt < 2047)
		{
			m_lineCnt++;
		}

		if (m_AppDoc->m_keywordNum > 0)
		{
			if (strncmp(
					pLog,
					m_AppDoc->m_keyword[0],
					strlen(m_AppDoc->m_keyword[0])) == 0)
			{
				/* Reset log buffer */
				InitialBuffer( FALSE );

				if (m_log[0] != pLog)
				{
					strcpy(m_log[0], pLog);
					m_lineCnt = 1;
				}
				m_keywordCnt = 1;

				/* Begin of a new power on check */
				m_AppDoc->m_pListView->AddListItem();
				SetTimer(LOG_TIMER_ID, (m_AppDoc->m_timeout * 1000), NULL);
			}
			else if (strncmp(
						pLog,
						m_AppDoc->m_keyword[m_keywordCnt],
						strlen(m_AppDoc->m_keyword[m_keywordCnt])) == 0)
			{
				int column = (m_keywordCnt + 1);

				m_AppDoc->m_pListView->UpdateListItem(column, "OK");

				if (m_keywordCnt == (m_AppDoc->m_keywordNum - 1))
				{
					/* Power on was finished */
					InitialBuffer( FALSE );
					m_AppDoc->m_pListView->UpdateTime();
				}
				else
				{
					m_keywordCnt++;
				}
			}
		}

		AppendMessage( pLog );
	}
	else if (ch == 0x0D)
	{
		;
	}
	else
	{
		if (m_charCnt >= 253)
		{
			m_log[m_lineCnt][ m_charCnt++ ] = '\r';
			m_log[m_lineCnt][ m_charCnt++ ] = '\n';
			m_log[m_lineCnt][ m_charCnt   ] = 0x00;
			m_charCnt = 0;
			if (m_lineCnt < 2047)
			{
				m_lineCnt++;
			}
		}
		m_log[m_lineCnt][ m_charCnt++ ] = ch;
	}

	return 0;
}
// End   of Serial Port RX


void CLogCheckEditView::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if (nIDEvent == LOG_TIMER_ID)
	{
		int column = (m_keywordCnt + 1);

		KillTimer( LOG_TIMER_ID );
		m_AppDoc->m_pListView->UpdateListItem(column, "ERROR");
		m_AppDoc->m_pListView->UpdateTime();
		m_keywordCnt = 0;

		GenErrorLog(m_AppDoc->m_pListView->m_ItemCurr + 1);
	}

	CEditView::OnTimer(nIDEvent);
}


/////////////////////////////////////////////////////////////////////////////
// 類別的成員函式

void CLogCheckEditView::AppendMessage(LPCSTR lpszMessage)
{
	CString strTemp = lpszMessage;

	//將文字附加在後面
	int len = GetWindowTextLength();
	GetEditCtrl().SetSel(len, len);
	GetEditCtrl().ReplaceSel( strTemp );
}

void CLogCheckEditView::InitialBuffer(BOOL clean)
{
	KillTimer( LOG_TIMER_ID );

	m_lineCnt = 0;
	m_charCnt = 0;
	m_keywordCnt = 0;

	if ( clean )
	{
		GetEditCtrl().SetWindowText( NULL );
	}
}

void CLogCheckEditView::GenErrorLog(UINT no)
{
	CString name;
	CFile file;
	UINT  i;

	name.Format("Report\\%u.txt", no);
	if ( file.Open(name, CFile::modeCreate|CFile::modeWrite|CFile::typeBinary, NULL) )
	{
		CFileException ex;
		CString text;
		try {
			for (i=0; i<m_lineCnt; i++)
			{
				text = (LPCTSTR)m_log[i];
				file.Write(text, text.GetLength());
			}
			file.Close();
		}
		catch (CFileException ex) {
			TCHAR szError[256];
			ex.GetErrorMessage(szError, 256);
			AfxMessageBox(szError);
		}
	}
}

