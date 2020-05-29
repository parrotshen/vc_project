// MySIMTreeView.h : interface of the CMySIMTreeView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYSIMTREEVIEW_H__4B92F77C_937D_4DBA_A8F5_D34609FC9031__INCLUDED_)
#define AFX_MYSIMTREEVIEW_H__4B92F77C_937D_4DBA_A8F5_D34609FC9031__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMySIMDoc;

class CMySIMTreeView : public CTreeView
{
protected: // create from serialization only
	CMySIMTreeView();
	DECLARE_DYNCREATE(CMySIMTreeView)

// Attributes
public:
	CMySIMDoc* GetDocument();

	CImageList m_ilFolderImage;
	HTREEITEM  m_hRootItem;     //最上層的Item
	HTREEITEM  m_hCurrentItem;  //目前的Item

// Operations
public:
	void ExpandRootTree();
	HTREEITEM SetupRootItem(CString ItemName, BOOL success);
	HTREEITEM AddDirectoryItem(CString ItemName, HTREEITEM parent, BOOL success);
	HTREEITEM AddFileItem(CString ItemName, HTREEITEM parent, BOOL success);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMySIMTreeView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMySIMTreeView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CMySIMTreeView)
	afx_msg void OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in MySIMTreeView.cpp
inline CMySIMDoc* CMySIMTreeView::GetDocument()
   { return (CMySIMDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYSIMTREEVIEW_H__4B92F77C_937D_4DBA_A8F5_D34609FC9031__INCLUDED_)
