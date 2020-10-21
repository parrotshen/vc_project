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

	m_charCnt = 0;
	m_keywordCnt = 0;

	memset(m_log, 0x00, 1024);
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
		/* Here is the end of line */
		m_log[ m_charCnt++ ] = '\r';
		m_log[ m_charCnt++ ] = '\n';
		m_log[ m_charCnt   ] = 0x00;
		m_charCnt = 0;

		DoCheck( m_log );
	}
	else if (ch == 0x0D)
	{
		/* Ignored */
		;
	}
	else
	{
		if (m_charCnt > 1020)
		{
			/* Wrap the line */
			m_log[ m_charCnt++ ] = '\r';
			m_log[ m_charCnt++ ] = '\n';
			m_log[ m_charCnt   ] = 0x00;
			m_charCnt = 0;

			DoCheck( m_log );
		}
		m_log[ m_charCnt++ ] = ch;
		m_log[ m_charCnt   ] = 0x00;
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

		/* Power on was failed */
		InitialBuffer( FALSE );

		m_AppDoc->m_pListView->UpdateListItem(column, "FAIL");
		m_AppDoc->m_pListView->UpdateEndTime();

		GenErrorLog(m_AppDoc->m_pListView->m_ItemCurr + 1);
	}

	CEditView::OnTimer(nIDEvent);
}


/////////////////////////////////////////////////////////////////////////////
// é¡žåˆ¥?„æ??¡å‡½å¼?

void CLogCheckEditView::AppendMessage(LPCSTR lpszMessage)
{
	CString strTemp = lpszMessage;

	//å°‡æ?å­—é?? åœ¨å¾Œé¢
	int len = GetWindowTextLength();
	GetEditCtrl().SetSel(len, len);
	GetEditCtrl().ReplaceSel( strTemp );
}

void CLogCheckEditView::InitialBuffer(BOOL clean)
{
	KillTimer( LOG_TIMER_ID );

	if ( clean )
	{
		GetEditCtrl().SetWindowText( NULL );
		m_charCnt = 0;
		m_keywordCnt = 0;
	}
}

void CLogCheckEditView::DoCheck(CHAR *pLog)
{
	if ((m_AppDoc->m_keywordNum > 0) &&
		(m_AppDoc->m_pListView->m_ColumnTime[0] > 1))
	{
		if (strncmp(
				pLog,
				m_AppDoc->m_keyword[0],
				strlen(m_AppDoc->m_keyword[0])) == 0)
		{
			/* Reset log buffer */
			InitialBuffer( TRUE );

			/* Begin of a new power on check */
			if (m_AppDoc->m_keywordNum == 1)
			{
				/* Power on was finished */
				m_AppDoc->m_pListView->AddListItem();
				m_AppDoc->m_pListView->UpdateStartTime();
				m_AppDoc->m_pListView->UpdateEndTime();
			}
			else
			{
				SetTimer(LOG_TIMER_ID, (m_AppDoc->m_timeout * 1000), NULL);
				m_AppDoc->m_pListView->AddListItem();
				m_AppDoc->m_pListView->UpdateStartTime();
				m_keywordCnt = 1;
			}
		}
		else if (strncmp(
					pLog,
					m_AppDoc->m_keyword[m_keywordCnt],
					strlen(m_AppDoc->m_keyword[m_keywordCnt])) == 0)
		{
			int column = (m_keywordCnt + 1);

			if (m_keywordCnt == (m_AppDoc->m_keywordNum - 1))
			{
				/* Power on was finished */
				InitialBuffer( FALSE );
				m_AppDoc->m_pListView->UpdateListItem(column, "OK");
				m_AppDoc->m_pListView->UpdateEndTime();
			}
			else
			{
				m_AppDoc->m_pListView->UpdateListItem(column, "OK");
				m_keywordCnt++;
			}
		}
	}

	/* Display the log */
	AppendMessage( pLog );
}

void CLogCheckEditView::CancelCheck(void)
{
	int column = (m_keywordCnt + 1);

	KillTimer( LOG_TIMER_ID );

	if (m_keywordCnt > 0)
	{
		m_AppDoc->m_pListView->UpdateListItem(column, "CANCEL");
		m_AppDoc->m_pListView->UpdateEndTime();
	}
}

void CLogCheckEditView::GenErrorLog(UINT no)
{
	CString name;
	CFile file;

	name.Format("Report\\%u.txt", no);
	if ( file.Open(name, CFile::modeCreate|CFile::modeWrite|CFile::typeBinary, NULL) )
	{
		CFileException ex;
		CString text;

		GetEditCtrl().GetWindowText( text );

		try {
			file.Write(text, text.GetLength());
			file.Close();
		}
		catch (CFileException ex) {
			TCHAR szError[128];
			ex.GetErrorMessage(szError, 128);
			AfxMessageBox(szError);
		}
	}
}

