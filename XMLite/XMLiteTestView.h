// XMLiteTestView.h : interface of the CXMLiteTestView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_XMLITEVIEW_H__54C8F048_2F67_47CF_9EB3_404C5400EDE4__INCLUDED_)
#define AFX_XMLITEVIEW_H__54C8F048_2F67_47CF_9EB3_404C5400EDE4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CXMLiteTestDoc;

class CXMLiteTestView : public CEditView
{
protected: // create from serialization only
	CXMLiteTestView();
	DECLARE_DYNCREATE(CXMLiteTestView)

// Attributes
public:
	CXMLiteTestDoc* GetDocument();
	CFont m_Font;

// Operations
public:
	void CleanText();
	void AppendMessage(LPCSTR lpszMessage);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXMLiteTestView)
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
	virtual ~CXMLiteTestView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CXMLiteTestView)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in XMLiteTestView.cpp
inline CXMLiteTestDoc* CXMLiteTestView::GetDocument()
   { return (CXMLiteTestDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XMLITEVIEW_H__54C8F048_2F67_47CF_9EB3_404C5400EDE4__INCLUDED_)
