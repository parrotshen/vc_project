// MySIMEditView.cpp : implementation file
//

#include "stdafx.h"
#include "MySIM.h"
#include "MySIMDoc.h"
#include "MySIMEditView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMySIMEditView

IMPLEMENT_DYNCREATE(CMySIMEditView, CEditView)

CMySIMEditView::CMySIMEditView()
{
}

CMySIMEditView::~CMySIMEditView()
{
}


BEGIN_MESSAGE_MAP(CMySIMEditView, CEditView)
	//{{AFX_MSG_MAP(CMySIMEditView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMySIMEditView drawing

void CMySIMEditView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CMySIMEditView diagnostics

#ifdef _DEBUG
void CMySIMEditView::AssertValid() const
{
	CEditView::AssertValid();
}

void CMySIMEditView::Dump(CDumpContext& dc) const
{
	CEditView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMySIMEditView message handlers

void CMySIMEditView::OnInitialUpdate() 
{
	CEditView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	//Begin: Change Font to Fixedsys
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
	//End:   Change Font to Fixedsys

	//Begin - 設定 Edit Control 的邊界
	//GetEditCtrl().SetMargins(15, 0);
	//End   - 設定 Edit Control 的邊界
}


/////////////////////////////////////////////////////////////////////////////
// CMySIMEditView member methods

void CMySIMEditView::PrintMessage(LPCTSTR fmt, ...)
{
	CString str;

	va_list argptr;
	va_start(argptr, fmt);
	str.FormatV(fmt, argptr);
	va_end(argptr);

	int len = GetWindowTextLength();
	GetEditCtrl().SetSel(len, len);
	GetEditCtrl().ReplaceSel(str);
}

void CMySIMEditView::DumpMem(LPCTSTR name, BYTE *pMem, int len)
{
	PrintMessage(_T("MEM-DUMP> '%s'\r\n"), name);

	if (pMem)
	{
		for (int i = 0; i < len; i++)
		{
			PrintMessage(_T("%02X "), pMem[i]);

			if (15 == (i % 16))
			{
				PrintMessage(_T("\r\n"));
			}
		}
		if (0 != (i % 16))
		{
			PrintMessage(_T("\r\n"));
		}
	}

	PrintMessage(_T("size = %d\r\n"), len);
	PrintMessage(_T("MEM-DUMP>\r\n\r\n"));
}

