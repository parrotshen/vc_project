// XMLiteTestDoc.h : interface of the CXMLiteTestDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_XMLITEDOC_H__2E3574D7_900A_4F8A_B040_E3225F224214__INCLUDED_)
#define AFX_XMLITEDOC_H__2E3574D7_900A_4F8A_B040_E3225F224214__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CXMLiteTestView;

class CXMLiteTestDoc : public CDocument
{
protected: // create from serialization only
	CXMLiteTestDoc();
	DECLARE_DYNCREATE(CXMLiteTestDoc)

// Attributes
public:
	CXMLiteTestView *m_pEditView;
	BYTE *m_pBuf;
	UINT  m_Len;

// Operations
public:
	void  ChildNameValue(void *obj, int level);
	void *ChildSearch(void *obj, char *name, char *value);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXMLiteTestDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	protected:
	virtual BOOL SaveModified();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CXMLiteTestDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CXMLiteTestDoc)
	afx_msg void OnFileOpen();
	afx_msg void OnXmlTest1();
	afx_msg void OnXmlTest2();
	afx_msg void OnXmlTest3();
	afx_msg void OnXmlTest4();
	afx_msg void OnXmlTest5();
	afx_msg void OnXmlTest6();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XMLITEDOC_H__2E3574D7_900A_4F8A_B040_E3225F224214__INCLUDED_)
