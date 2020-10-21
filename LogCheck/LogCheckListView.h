#if !defined(AFX_MONETONOFFLISTVIEW_H__80FD7A4A_FE6F_408F_8DE5_5347F1CBE6EF__INCLUDED_)
#define AFX_MONETONOFFLISTVIEW_H__80FD7A4A_FE6F_408F_8DE5_5347F1CBE6EF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LogCheckListView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLogCheckListView view

class CLogCheckListView : public CListView
{
protected:
	CLogCheckListView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CLogCheckListView)

// Attributes
public:
	int m_ItemCurr;
	int m_ItemNext;
	int m_ColumnTime[2];

// Operations
public:
	void CleanListColumn();
	void AddListColumn(int column, char *pLabel);
	void CleanListItem();
	void AddListItem();
	void UpdateListItem(int column, char *pWord);
	void UpdateStartTime();
	void UpdateEndTime();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLogCheckListView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CLogCheckListView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CLogCheckListView)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MONETONOFFLISTVIEW_H__80FD7A4A_FE6F_408F_8DE5_5347F1CBE6EF__INCLUDED_)
