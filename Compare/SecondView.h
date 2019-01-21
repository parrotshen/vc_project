#if !defined(AFX_SECONDVIEW_H__AD8379DD_CEC7_4730_9C1C_9D9605BD61C6__INCLUDED_)
#define AFX_SECONDVIEW_H__AD8379DD_CEC7_4730_9C1C_9D9605BD61C6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SecondView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSecondView view

class CSecondView : public CEditView
{
protected:
	CSecondView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CSecondView)

// Attributes
public:
	CFont m_font;

// Operations
public:
	void DisplayByHex();
	void DisplayByNormal();
	void SelectOffsetByHex(int offset);
	void SelectOffsetByNormal(int offset);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSecondView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CSecondView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CSecondView)
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SECONDVIEW_H__AD8379DD_CEC7_4730_9C1C_9D9605BD61C6__INCLUDED_)
