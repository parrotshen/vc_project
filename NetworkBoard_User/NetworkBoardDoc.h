// NetworkBoardDoc.h : interface of the CNetworkBoardDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_NETWORKBOARDDOC_H__A273BD61_94C9_4EE6_901B_88D37C9B4DF4__INCLUDED_)
#define AFX_NETWORKBOARDDOC_H__A273BD61_94C9_4EE6_901B_88D37C9B4DF4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CNetworkBoardDoc : public CDocument
{
protected: // create from serialization only
	CNetworkBoardDoc();
	DECLARE_DYNCREATE(CNetworkBoardDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNetworkBoardDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	protected:
	virtual BOOL SaveModified();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CNetworkBoardDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CNetworkBoardDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NETWORKBOARDDOC_H__A273BD61_94C9_4EE6_901B_88D37C9B4DF4__INCLUDED_)
