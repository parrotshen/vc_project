#if !defined(AFX_TEXTLISTVIEW_H__1C804FB5_3C84_4835_A0D8_49866E9872CB__INCLUDED_)
#define AFX_TEXTLISTVIEW_H__1C804FB5_3C84_4835_A0D8_49866E9872CB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TextListView.h : header file
//
#include <io.h>

/////////////////////////////////////////////////////////////////////////////
// CTextListView view

class CTextListView : public CListView
{
protected:
	CTextListView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CTextListView)

// Attributes
public:
	CString m_ScanPath;   //掃描檔案的路徑
	CString m_ScanFilter; //掃描檔案的副檔名

// Operations
public:
	void ReadDirectoryInfo();
	void AddFileItem(_finddata_t& fblk);
	int GetIconIndex(const CString &csFileName);

	void MoveItemMarkDown();
	void MoveItemMarkUpper();
	void AscendingSortBy(int column);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTextListView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CTextListView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CTextListView)
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnColumnclick(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TEXTLISTVIEW_H__1C804FB5_3C84_4835_A0D8_49866E9872CB__INCLUDED_)
