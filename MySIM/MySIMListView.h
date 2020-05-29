#if !defined(AFX_MYSIMLISTVIEW_H__95F5DCED_201A_4FA2_A3E3_D1307A8B7F24__INCLUDED_)
#define AFX_MYSIMLISTVIEW_H__95F5DCED_201A_4FA2_A3E3_D1307A8B7F24__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MySIMListView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMySIMListView view

#include "SimFileSys.h"

class CMySIMListView : public CListView
{
protected:
	CMySIMListView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CMySIMListView)

// Attributes
public:

// Operations
public:
	void DisplayEF(struct _tSimFileSys *pFile);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMySIMListView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CMySIMListView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CMySIMListView)
	afx_msg void OnDblclk(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYSIMLISTVIEW_H__95F5DCED_201A_4FA2_A3E3_D1307A8B7F24__INCLUDED_)
