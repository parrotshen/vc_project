// XMLiteTestDoc.cpp : implementation of the CXMLiteTestDoc class
//

#include "stdafx.h"
#include "XMLiteTest.h"
#include "XMLiteTestView.h"
#include "XMLiteTestDoc.h"
#include "XMLite.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXMLiteTestDoc

IMPLEMENT_DYNCREATE(CXMLiteTestDoc, CDocument)

BEGIN_MESSAGE_MAP(CXMLiteTestDoc, CDocument)
	//{{AFX_MSG_MAP(CXMLiteTestDoc)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_COMMAND(ID_XML_TEST1, OnXmlTest1)
	ON_COMMAND(ID_XML_TEST2, OnXmlTest2)
	ON_COMMAND(ID_XML_TEST3, OnXmlTest3)
	ON_COMMAND(ID_XML_TEST4, OnXmlTest4)
	ON_COMMAND(ID_XML_TEST5, OnXmlTest5)
	ON_COMMAND(ID_XML_TEST6, OnXmlTest6)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXMLiteTestDoc construction/destruction

CXMLiteTestDoc::CXMLiteTestDoc()
{
	// TODO: add one-time construction code here
	m_pBuf = NULL;
	m_Len  = 0;
}

CXMLiteTestDoc::~CXMLiteTestDoc()
{
	if (m_pBuf != NULL)
	{
		delete m_pBuf;
		m_pBuf = NULL;
	}
}

BOOL CXMLiteTestDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	m_pEditView = (CXMLiteTestView *)m_viewList.GetHead();
	if ( m_pEditView )
	{
		((CEditView*)m_pEditView)->SetWindowText(NULL);
	}

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CXMLiteTestDoc serialization

void CXMLiteTestDoc::Serialize(CArchive& ar)
{
	// CEditView contains an edit control which handles all serialization
	((CEditView*)m_viewList.GetHead())->SerializeRaw(ar);
}

/////////////////////////////////////////////////////////////////////////////
// CXMLiteTestDoc diagnostics

#ifdef _DEBUG
void CXMLiteTestDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CXMLiteTestDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CXMLiteTestDoc commands

BOOL CXMLiteTestDoc::SaveModified() 
{
	// TODO: Add your specialized code here and/or call the base class
	//取消 詢問是否要存檔 的對話盒
	SetModifiedFlag(FALSE);

	return CDocument::SaveModified();
}

void CXMLiteTestDoc::OnFileOpen() 
{
	CFileDialog dlg(
		TRUE,
		NULL,
		"*.xml",
		NULL,
		"XML File (*.xml)|*.xml|All File (*.*)|*.*|"
	);
	if (dlg.DoModal() == IDOK)
	{
		CString name = dlg.GetPathName();
		CFile file;
		if ( !file.Open(name, CFile::modeRead|CFile::typeBinary, NULL) )
		{
			AfxMessageBox("Cannot open file: " + name);
			return;
		}

		m_Len = file.GetLength();
		if (0 == m_Len)
		{
			file.Close();
			m_pBuf[0] = 0;
			m_pEditView->CleanText();
			return;
		}

		if (m_pBuf != NULL)
		{
			delete m_pBuf;
		}
		m_pBuf = new BYTE[m_Len + 1];

		file.Read(m_pBuf, m_Len);

		m_pBuf[m_Len] = 0;

		file.Close();

		m_pEditView->SetWindowText( (LPCTSTR)m_pBuf );
	}
}

void CXMLiteTestDoc::OnXmlTest1() 
{
	CString text;
	m_pEditView->GetWindowText( text );

	if (text.GetLength() > 0)
	{
		//m_pEditView->CleanText();

		//Test. 1: simple plane XML parse
		XNode xml;
		PARSEINFO pi;
		pi.trim_value = true;

		if (NULL == xml.Load(text, &pi))
		{
			m_pEditView->AppendMessage( _T("ERROR\r\n\r\n") );
			if ( pi.erorr_occur )
			{
				m_pEditView->SetWindowText( pi.error_string );
			}
		}
		else
		{
			m_pEditView->SetWindowText( xml.GetXML() );
		}
	}
}

void CXMLiteTestDoc::OnXmlTest2() 
{
	CString text;
	m_pEditView->GetWindowText( text );

	if (text.GetLength() > 0)
	{
		XNode xml;
		if (NULL == xml.Load( text ))
		{
			m_pEditView->AppendMessage( _T("ERROR\r\n") );
			return;
		}

		m_pEditView->CleanText();

		//Test. 2: get child element name and value
		ChildNameValue(&xml, 0);
	}
}

void CXMLiteTestDoc::OnXmlTest3() 
{
	CString text;
	m_pEditView->GetWindowText( text );

	if (text.GetLength() > 0)
	{
		XNode xml;
		if (NULL == xml.Load( text ))
		{
			m_pEditView->AppendMessage( _T("ERROR\r\n") );
			return;
		}

		m_pEditView->CleanText();

		//Test. 3: get attribute vaule of child
		if (xml.name == "Catalog")
		{
			m_pEditView->AppendMessage( "Information ... number: " );
			m_pEditView->AppendMessage(
							xml.GetChildAttrValue(_T("Information"), _T("number"))
						);
			m_pEditView->AppendMessage( "\r\n" );

			XNodes childs = xml.GetChilds( _T("Product") );
			for (int i=0; i<childs.size(); i++)
			{
				m_pEditView->AppendMessage( "Price ... currency: " );
				m_pEditView->AppendMessage(
								childs[i]->GetChildAttrValue(_T("Price"), _T("currency"))
							);
				m_pEditView->AppendMessage( "\r\n" );
			}
		}
	}
}

void CXMLiteTestDoc::OnXmlTest4() 
{
	CString text;
	m_pEditView->GetWindowText( text );

	if (text.GetLength() > 0)
	{
		XNode xml;
		if (NULL == xml.Load( text ))
		{
			m_pEditView->AppendMessage( _T("ERROR\r\n") );
			return;
		}

		m_pEditView->CleanText();

		//Test. 4: remove a brother node
		LPXNode brother = xml.GetChild( 0 );
		xml.RemoveChild( brother );

		m_pEditView->AppendMessage( xml.GetXML() );
		m_pEditView->AppendMessage( "\r\n" );
	}
}

void CXMLiteTestDoc::OnXmlTest5() 
{
	CString text;
	m_pEditView->GetWindowText( text );

	if (text.GetLength() > 0)
	{
		XNode xml;
		if (NULL == xml.Load( text ))
		{
			m_pEditView->AppendMessage( _T("ERROR\r\n") );
			return;
		}

		m_pEditView->CleanText();

		//Test. 5: append a child node
		// <Product>
		//     <Name>Zyxel router</Name>
		//     <Price currency="NT">5500</Price>
		//     <Mark/>
		// </Product>
		LPXNode product = xml.AppendChild("Product", NULL);
		LPXNode name = product->AppendChild("Name", "Zyxel router");
		LPXNode price = product->AppendChild("Price", "5500");
		price->AppendAttr("curency", "NT");
		LPXNode mark = product->AppendChild("Mark", NULL);

		m_pEditView->SetWindowText( xml.GetXML() );
	}
}

void CXMLiteTestDoc::OnXmlTest6() 
{
	CString text;
	m_pEditView->GetWindowText( text );

	if (text.GetLength() > 0)
	{
		XNode xml;
		if (NULL == xml.Load( text ))
		{
			m_pEditView->AppendMessage( _T("ERROR\r\n") );
			return;
		}

		m_pEditView->CleanText();

		//Test. 6: modify a child node
		LPXNode target;
		for (int i=0; i<xml.GetChildCount(); i++)
		{
			LPXNode node = xml.GetChild( i );
			if (( node ) && (node->name == "Product"))
			{
				if ( ChildSearch(node, "Name", "IBM desktop") )
				{
					target = (LPXNode)ChildSearch(node, "Price", NULL);
					if ( target )
					{
						target->value = "60000";
					}
					target = (LPXNode)ChildSearch(node, "Mark", NULL);
					if ( target )
					{
						target->value = "New model";
					}
					m_pEditView->SetWindowText( xml.GetXML() );
					break;
				}
			}
		}
	}
}


/////////////////////////////////////////////////////////////////////////////
// CXMLiteTestDoc member methods

CString RemoveSpace(CString input)
{
	CString output;
	output.Empty();

	if (input.GetLength() > 0)
	{
		int i = 0;
		int j = 0;
		if (input.GetAt(0) == 0x0d) i++;
		if (input.GetAt(1) == 0x0a) i++;
		for (; i<input.GetLength(); i++)
		{
			if (input.GetAt(i) != 0x20) break;
		}
		for (; i<input.GetLength(); i++, j++)
		{
			output.SetAt(j, input.GetAt(i));
		}
	}

	return output;
}

void CXMLiteTestDoc::ChildNameValue(void *obj, int level)
{
	LPXNode node = (LPXNode)obj;
	CString str;
	int i;

	if (level > 0)
	{
		str.Empty();
		for (i=0; i<level; i++)
		{
			str += "."; 
		}
		str += " "; 
		m_pEditView->AppendMessage( str );
	}
	str.Format("<%s> %s\r\n", node->name, RemoveSpace(node->value)); 
	m_pEditView->AppendMessage( str );

	// DOM tree Childs Traveling 
	// method 1: Using GetChildCount() and GetChild()
	for (i=0; i<node->GetChildCount(); i++)
	{
		LPXNode child = node->GetChild( i );
		if ( child )
		{
			ChildNameValue(child, (level + 3));
		}
	}

	//m_pEditView->AppendMessage( "\r\n" );
}

void *CXMLiteTestDoc::ChildSearch(void *obj, char *name, char *value)
{
	LPXNode parent = (LPXNode)obj;
	int i;

	for (i=0; i<parent->GetChildCount(); i++)
	{
		LPXNode child = parent->GetChild( i );
		if ( child )
		{
			if (child->name == name)
			{
				if ( value )
				{
					if (child->value == value)
					{
						return child;
					}
				}
				else
				{
					return child;
				}
			}
		}
	}

	return NULL;
}

