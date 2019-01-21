// NetworkBoardDoc.h : interface of the CNetworkBoardDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_NETWORKBOARDDOC_H__DA9BFAA6_ECA5_4DCC_ACFF_710E696C01D1__INCLUDED_)
#define AFX_NETWORKBOARDDOC_H__DA9BFAA6_ECA5_4DCC_ACFF_710E696C01D1__INCLUDED_

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
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NETWORKBOARDDOC_H__DA9BFAA6_ECA5_4DCC_ACFF_710E696C01D1__INCLUDED_)
