// ReportView.h : interface of the CReportView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_REPORTVIEW_H__3544E3FF_8DD9_4D1B_9B27_F3CC958A7F34__INCLUDED_)
#define AFX_REPORTVIEW_H__3544E3FF_8DD9_4D1B_9B27_F3CC958A7F34__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CCompareDoc;

class CReportView : public CListView
{
protected: // create from serialization only
	CReportView();
	DECLARE_DYNCREATE(CReportView)

// Attributes
public:
	CCompareDoc* GetDocument();

// Operations
public:
	void AddReportItem(int offset, byte char1, byte char2);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CReportView)
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
	virtual ~CReportView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CReportView)
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in ReportView.cpp
inline CCompareDoc* CReportView::GetDocument()
   { return (CCompareDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REPORTVIEW_H__3544E3FF_8DD9_4D1B_9B27_F3CC958A7F34__INCLUDED_)
