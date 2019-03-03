#if !defined(AFX_TEXTEDITVIEW_H__6207D729_B2F3_4693_8513_CDD1D58C49F7__INCLUDED_)
#define AFX_TEXTEDITVIEW_H__6207D729_B2F3_4693_8513_CDD1D58C49F7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TextEditView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTextEditView view

class CTextEditView : public CEditView
{
protected:
	CTextEditView(); // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CTextEditView)

// Attributes
public:
	CFindReplaceDialog* m_pFRDlg;
	CFont   m_Font;
	CString m_Find;
	CString m_Replace;
	BOOL    m_FR; //設定 CFindReplaceDialog 具有 Find/Replace 的樣式

// Operations
public:
	void InitFindReplaceDlg(BOOL style);
	void Button_GotoLines(int jump);
	void Button_FindText(CString target);
	void ChangeEditFont();

	void KeepEditContent();
	void DisplayByNormal();
	void DisplayByHex();
	void BreakLineByWindows();
	void BreakLineByUnix();


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTextEditView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CTextEditView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CTextEditView)
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG
	afx_msg LRESULT OnFindReplace(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TEXTEDITVIEW_H__6207D729_B2F3_4693_8513_CDD1D58C49F7__INCLUDED_)
