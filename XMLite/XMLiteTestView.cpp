// XMLiteTestView.cpp : implementation of the CXMLiteTestView class
//

#include "stdafx.h"
#include "XMLiteTest.h"

#include "XMLiteTestDoc.h"
#include "XMLiteTestView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXMLiteTestView

IMPLEMENT_DYNCREATE(CXMLiteTestView, CEditView)

BEGIN_MESSAGE_MAP(CXMLiteTestView, CEditView)
	//{{AFX_MSG_MAP(CXMLiteTestView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CEditView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CEditView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CEditView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXMLiteTestView construction/destruction

CXMLiteTestView::CXMLiteTestView()
{
	// TODO: add construction code here

}

CXMLiteTestView::~CXMLiteTestView()
{
}

BOOL CXMLiteTestView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	BOOL bPreCreated = CEditView::PreCreateWindow(cs);
	cs.style &= ~(ES_AUTOHSCROLL|WS_HSCROLL);	// Enable word-wrapping

	return bPreCreated;
}

/////////////////////////////////////////////////////////////////////////////
// CXMLiteTestView drawing

void CXMLiteTestView::OnDraw(CDC* pDC)
{
	CXMLiteTestDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CXMLiteTestView printing

BOOL CXMLiteTestView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default CEditView preparation
	return CEditView::OnPreparePrinting(pInfo);
}

void CXMLiteTestView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	// Default CEditView begin printing.
	CEditView::OnBeginPrinting(pDC, pInfo);
}

void CXMLiteTestView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	// Default CEditView end printing
	CEditView::OnEndPrinting(pDC, pInfo);
}

/////////////////////////////////////////////////////////////////////////////
// CXMLiteTestView diagnostics

#ifdef _DEBUG
void CXMLiteTestView::AssertValid() const
{
	CEditView::AssertValid();
}

void CXMLiteTestView::Dump(CDumpContext& dc) const
{
	CEditView::Dump(dc);
}

CXMLiteTestDoc* CXMLiteTestView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CXMLiteTestDoc)));
	return (CXMLiteTestDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CXMLiteTestView message handlers

void CXMLiteTestView::OnInitialUpdate() 
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
}


/////////////////////////////////////////////////////////////////////////////
// CXMLiteTestView member methods

void CXMLiteTestView::CleanText()
{
	GetEditCtrl().SetWindowText( NULL );
}

void CXMLiteTestView::AppendMessage(LPCSTR lpszMessage)
{
	CString str = lpszMessage;

	//將文字附加在後面
	int len = GetWindowTextLength();
	GetEditCtrl().SetSel(len, len);
	GetEditCtrl().ReplaceSel( str );
}

