// NetworkBoardDoc.cpp : implementation of the CNetworkBoardDoc class
//

#include "stdafx.h"
#include "NetworkBoard.h"
#include "NetworkBoardDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNetworkBoardDoc

IMPLEMENT_DYNCREATE(CNetworkBoardDoc, CDocument)

BEGIN_MESSAGE_MAP(CNetworkBoardDoc, CDocument)
	//{{AFX_MSG_MAP(CNetworkBoardDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNetworkBoardDoc construction/destruction

CNetworkBoardDoc::CNetworkBoardDoc()
{
	// TODO: add one-time construction code here

}

CNetworkBoardDoc::~CNetworkBoardDoc()
{
}

BOOL CNetworkBoardDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	((CEditView*)m_viewList.GetHead())->SetWindowText(NULL);

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CNetworkBoardDoc serialization

void CNetworkBoardDoc::Serialize(CArchive& ar)
{
	// CEditView contains an edit control which handles all serialization
	((CEditView*)m_viewList.GetHead())->SerializeRaw(ar);
}

/////////////////////////////////////////////////////////////////////////////
// CNetworkBoardDoc diagnostics

#ifdef _DEBUG
void CNetworkBoardDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CNetworkBoardDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CNetworkBoardDoc commands

BOOL CNetworkBoardDoc::SaveModified() 
{
	//取消 詢問是否要存檔 的對話盒
	SetModifiedFlag(FALSE);
	
	return CDocument::SaveModified();
}
