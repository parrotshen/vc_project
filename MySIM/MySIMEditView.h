#if !defined(AFX_MYSIMEDITVIEW_H__8402CBF9_FDE2_4A06_9593_1DE00AF276D7__INCLUDED_)
#define AFX_MYSIMEDITVIEW_H__8402CBF9_FDE2_4A06_9593_1DE00AF276D7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MySIMEditView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMySIMEditView view

class CMySIMEditView : public CEditView
{
protected:
	CMySIMEditView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CMySIMEditView)

// Attributes
public:
	CFont    m_font;

// Operations
public:
	void PrintMessage(LPCTSTR fmt, ...);
	void DumpMem(LPCTSTR name, BYTE *pMem, int len);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMySIMEditView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CMySIMEditView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CMySIMEditView)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYSIMEDITVIEW_H__8402CBF9_FDE2_4A06_9593_1DE00AF276D7__INCLUDED_)
