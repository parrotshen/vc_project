#if !defined(AFX_FIRSTVIEW_H__D5088445_FCB6_48A4_B991_36BF8DE35A7C__INCLUDED_)
#define AFX_FIRSTVIEW_H__D5088445_FCB6_48A4_B991_36BF8DE35A7C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FirstView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFirstView view

class CFirstView : public CEditView
{
protected:
	CFirstView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CFirstView)

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
	//{{AFX_VIRTUAL(CFirstView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CFirstView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CFirstView)
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FIRSTVIEW_H__D5088445_FCB6_48A4_B991_36BF8DE35A7C__INCLUDED_)
