// LogCheckDoc.cpp : implementation of the CLogCheckDoc class
//

#include "stdafx.h"
#include "LogCheck.h"
#include "LogCheckEditView.h"
#include "LogCheckListView.h"
#include "LogCheckDoc.h"
#include "ConfigDialog.h"
#include "MainFrm.h"
#include "XMLite.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define XML_CONF_FILE "LogCheck.xml"


/////////////////////////////////////////////////////////////////////////////
// CLogCheckDoc

IMPLEMENT_DYNCREATE(CLogCheckDoc, CDocument)

BEGIN_MESSAGE_MAP(CLogCheckDoc, CDocument)
	//{{AFX_MSG_MAP(CLogCheckDoc)
	ON_COMMAND(ID_APP_CONNECT, OnAppConnect)
	ON_UPDATE_COMMAND_UI(ID_APP_CONNECT, OnUpdateAppConnect)
	ON_COMMAND(ID_FILE_NEW, OnFileNew)
	ON_COMMAND(ID_APP_CONFIG, OnAppConfig)
	ON_UPDATE_COMMAND_UI(ID_APP_CONFIG, OnUpdateAppConfig)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_COMMAND(ID_FILE_SAVE, OnFileSave)
	ON_COMMAND(ID_FILE_SAVE_AS, OnFileSaveAs)
	ON_UPDATE_COMMAND_UI(ID_FILE_OPEN, OnUpdateFileOpen)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE, OnUpdateFileSave)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE_AS, OnUpdateFileSaveAs)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLogCheckDoc construction/destruction

CLogCheckDoc::CLogCheckDoc()
{
	// TODO: add one-time construction code here
	m_pEditView = NULL;
	m_pListView = NULL;

	m_SerialParam.port_num  = 8;
	m_SerialParam.baud_rate = 115200;
	m_SerialParam.parity    = 'N';
	m_SerialParam.data_bits = 8;
	m_SerialParam.stop_bits = 1;
	m_bConnected = FALSE;

	m_fileName = XML_CONF_FILE;
	m_timeout = 60;
	m_keywordNum = 0;
}

CLogCheckDoc::~CLogCheckDoc()
{
}

BOOL CLogCheckDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	((CEditView*)m_viewList.GetHead())->SetWindowText(NULL);

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	//Begin: ?–å? View
	POSITION pos = GetFirstViewPosition();
	if (pos == NULL) return FALSE;

	//[1] ?–å? CLogCheckEditView
	m_pEditView = (CLogCheckEditView*)GetNextView( pos );
	if (pos == NULL) return FALSE;

	//[2] ?–å? CLogCheckListView
	m_pListView = (CLogCheckListView*)GetNextView( pos );

	// set EditView read only
	m_pEditView->GetEditCtrl().SetReadOnly();
	//End:   ?–å? View

	/* Read configuration from .xml */
	ReadXML( m_fileName );

	return TRUE;
}

void CLogCheckDoc::OnCloseDocument() 
{
	// TODO: Add your specialized code here and/or call the base class

	CDocument::OnCloseDocument();
}


/////////////////////////////////////////////////////////////////////////////
// CLogCheckDoc serialization

void CLogCheckDoc::Serialize(CArchive& ar)
{
	// CEditView contains an edit control which handles all serialization
	((CEditView*)m_viewList.GetHead())->SerializeRaw(ar);
}

/////////////////////////////////////////////////////////////////////////////
// CLogCheckDoc diagnostics

#ifdef _DEBUG
void CLogCheckDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CLogCheckDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CLogCheckDoc commands

BOOL CLogCheckDoc::SaveModified() 
{
	// TODO: Add your specialized code here and/or call the base class
	//?–æ? è©¢å??¯å¦è¦å?æª??„å?è©±ç?
	SetModifiedFlag(FALSE);

	return CDocument::SaveModified();
}

void CLogCheckDoc::OnAppConnect() 
{
	// TODO: Add your command handler code here
	if ( !m_bConnected )
	{
		m_bConnected = MySerialIF_OpenConnectionEx(
							(CWnd *)m_pEditView,
							m_SerialParam.port_num,
							m_SerialParam.baud_rate,
							m_SerialParam.parity,
							m_SerialParam.data_bits,
							m_SerialParam.stop_bits,
							&m_SerialParam
						);
		if ( m_bConnected )
		{
			m_pEditView->InitialBuffer( TRUE );
		}
	}
	else
	{
		MySerialIF_CloseConnection();
		m_pEditView->CancelCheck();
		m_bConnected = FALSE;
	}
}

void CLogCheckDoc::OnUpdateAppConnect(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck( m_bConnected );
}

void CLogCheckDoc::OnAppConfig() 
{
	// TODO: Add your command handler code here
	CConfigDialog dlg;
	UINT i;

	dlg.m_timeout = m_timeout;
	dlg.m_keywordNum = m_keywordNum;
	for (i=0; i<m_keywordNum; i++)
	{
		strcpy(dlg.m_label[i], m_label[i]);
		strcpy(dlg.m_keyword[i], m_keyword[i]);
	}

	if (dlg.DoModal() == IDOK)
	{
		m_pListView->CleanListColumn();

		/* Update timeout and keywords */
		m_timeout = dlg.m_timeout;
		m_keywordNum = dlg.m_keywordNum;
		for (i=0; i<m_keywordNum; i++)
		{
			strcpy(m_label[i], dlg.m_label[i]);
			strcpy(m_keyword[i], dlg.m_keyword[i]);
			m_pListView->AddListColumn((i + 1), m_label[i]);
		}

		m_pListView->AddListColumn((i + 1), "Start time");
		m_pListView->AddListColumn((i + 2), "End time");

		CMainFrame *frame = (CMainFrame *)AfxGetMainWnd();
		if (frame != NULL)
		{
			CString tout;
			tout.Format("Timeout %u sec", m_timeout);
			frame->SetStatusMessage( tout );
		}
	}
}

void CLogCheckDoc::OnUpdateAppConfig(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable( !m_bConnected );
}

void CLogCheckDoc::OnFileNew() 
{
	// TODO: Add your command handler code here
	m_pEditView->InitialBuffer( TRUE );
	m_pListView->CleanListItem();
}

void CLogCheckDoc::OnFileOpen() 
{
	// TODO: Add your command handler code here
	CFileDialog dlg(
		TRUE,
		NULL,
		"*.xml",
		NULL,
		"XML File (*.xml)|*.xml|All File (*.*)|*.*|"
	);
	if (dlg.DoModal() == IDOK)
	{
		m_fileName = dlg.GetPathName();
		if ( ReadXML( m_fileName ) )
		{
			m_pEditView->InitialBuffer( TRUE );
			m_pListView->CleanListColumn();
		}
	}
}

void CLogCheckDoc::OnUpdateFileOpen(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable( !m_bConnected );
}

void CLogCheckDoc::OnFileSave() 
{
	// TODO: Add your command handler code here
	WriteXML( m_fileName );
}

void CLogCheckDoc::OnUpdateFileSave(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable( !m_bConnected );
}

void CLogCheckDoc::OnFileSaveAs() 
{
	// TODO: Add your command handler code here
	CFileDialog dlg(
		TRUE,
		NULL,
		"*.xml",
		NULL,
		"XML File (*.xml)|*.xml|All File (*.*)|*.*|"
	);
	if (dlg.DoModal() == IDOK)
	{
		m_fileName = dlg.GetPathName();
		WriteXML( m_fileName );
	}
}

void CLogCheckDoc::OnUpdateFileSaveAs(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable( !m_bConnected );
}


/////////////////////////////////////////////////////////////////////////////
// CLogCheckDoc member functions

void CLogCheckDoc::CloseConnection()
{
	if ( m_bConnected )
	{
		MySerialIF_CloseConnection();
		m_bConnected = FALSE;
	}
	m_pEditView->InitialBuffer( FALSE );
}

BOOL CLogCheckDoc::ReadXML(CString name)
{
	char *pBuf = NULL;
	UINT  size;
	UINT  i;

	CFile file;
	if ( !file.Open(name, CFile::modeRead|CFile::typeBinary, NULL) )
	{
		AfxMessageBox("Cannot open file: " + name);
		return FALSE;
	}

	size = file.GetLength();
	if (0 == size)
	{
		file.Close();
		AfxMessageBox("Incorrect file size: " + name);
		return FALSE;
	}

	pBuf = new char[size + 1];

	file.Read(pBuf, size);

	pBuf[size] = 0;

	file.Close();


	/* Load XML file */
	XDoc xml;
	PARSEINFO pi;

	pi.trim_value = true;
	if (NULL == xml.Load((LPCTSTR)pBuf, &pi))
	{
		CString msg = "Fail to load XML: " + name;
		if ( pi.erorr_occur )
		{
			msg += "\r\n" + pi.error_string;
		}
		AfxMessageBox( msg );

		delete pBuf;
		pBuf = NULL;

		return FALSE;
	}

	delete pBuf;
	pBuf = NULL;


	/* Parse XML tags */
	LPXNode pRoot;
	LPXNode pNode;

	pRoot = xml.GetRoot();
	if ((pRoot != NULL) && (pRoot->name.Compare("PowerOn") != 0))
	{
		AfxMessageBox( "Cannot find the tag <PowerOn>" );
		xml.Close();
		return FALSE;
	}

	m_timeout = _ttoi( pRoot->GetAttrValue( "Timeout" ));
	if (m_timeout == 0)
	{
		m_timeout = 60;
	}

	m_keywordNum = 0;
	for (i=0; i<pRoot->GetChildCount() && i<16; i++)
	{
		pNode = pRoot->GetChild( i );
		if ( pNode )
		{
			memset(m_label[m_keywordNum], 0, 64);
			strncpy(
				m_label[m_keywordNum],
				(CHAR *)(pNode->GetAttrValue( "Label" )),
				63
			);
			memset(m_keyword[m_keywordNum], 0, 256);
			strncpy(
				m_keyword[m_keywordNum],
				(CHAR *)(LPCTSTR)(pNode->GetText()),
				255
			);
			m_keywordNum++;
		}
	}

	xml.Close();

	SetTitle( name );

	return TRUE;
}

void CLogCheckDoc::WriteXML(CString name)
{
	FILE *pFile;
	UINT  i;

	pFile = fopen(name, "w");
	if (NULL == pFile)
	{
		CString msg;
		msg.Format("Fail to open %s", (char *)(LPCTSTR)name);
		AfxMessageBox( msg );
		return;
	}

	/* Create XML document */
	XDoc xml;
	PARSEINFO pi;
	LPXNode pRoot;
	LPXNode pNode;

	pi.trim_value = true;
	if (NULL == xml.Load("<?xml version=\"1.0\"?>\r\n<PowerOn>\r\n</PowerOn>", &pi))
	{
		CString msg = "Fail to load XML:\r\n";
		if ( pi.erorr_occur )
		{
			msg += pi.error_string;
		}
		AfxMessageBox( msg );
		fclose( pFile );
		return;
	}

	pRoot = xml.GetRoot();
	if ( pRoot )
	{
		CString t;
		t.Format("%u", m_timeout);
		pRoot->AppendAttr("Timeout", t);

		for (i=0; i<m_keywordNum; i++)
		{
			pNode = pRoot->AppendChild("Keyword", (LPCTSTR)m_keyword[i]);
			if ( pNode )
			{
				pNode->AppendAttr("Label", (LPCTSTR)m_label[i]);
			}
		}
	}

	CString text = xml.GetXML();
	text += "\r\n";

	fwrite((BYTE *)(LPCTSTR)text, 1, text.GetLength(), pFile);

	fclose( pFile );

	SetTitle( name );
}

