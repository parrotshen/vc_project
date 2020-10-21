// LogCheckEditView.h : interface of the CLogCheckEditView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MONETONOFFEDITVIEW_H__CED5B4B7_9EA2_4835_A828_0FADD3415057__INCLUDED_)
#define AFX_MONETONOFFEDITVIEW_H__CED5B4B7_9EA2_4835_A828_0FADD3415057__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CLogCheckDoc;

class CLogCheckEditView : public CEditView
{
protected: // create from serialization only
	CLogCheckEditView();
	DECLARE_DYNCREATE(CLogCheckEditView)

// Attributes
public:
	CLogCheckDoc *GetDocument();

	CLogCheckDoc *m_AppDoc;
	CFont    m_Font;

	UINT     m_charCnt;
	UINT     m_keywordCnt;
	CHAR     m_log[1024];

// Operations
public:
	void AppendMessage(LPCSTR lpszMessage);
	void InitialBuffer(BOOL clean);
	void DoCheck(CHAR *pLog);
	void CancelCheck(void);
	void GenErrorLog(UINT no);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLogCheckEditView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CLogCheckEditView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	// Begin of Serial Port Rx
	afx_msg LONG OnCommunication(WPARAM ch, LPARAM port);
	// End   of Serial Port Rx

	//{{AFX_MSG(CLogCheckEditView)
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in LogCheckEditView.cpp
inline CLogCheckDoc* CLogCheckEditView::GetDocument()
   { return (CLogCheckDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MONETONOFFEDITVIEW_H__CED5B4B7_9EA2_4835_A828_0FADD3415057__INCLUDED_)
