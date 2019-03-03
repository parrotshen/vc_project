// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__4FC7DA9C_F2C8_4FC3_8C74_5F8A6D57E7BE__INCLUDED_)
#define AFX_MAINFRM_H__4FC7DA9C_F2C8_4FC3_8C74_5F8A6D57E7BE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMainFrame : public CFrameWnd
{
	
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:
	CSplitterWnd m_Splitter, m_SplitterSub; //分裂視窗
	CImageList m_smallImageList; //小Icon
	CImageList m_largeImageList; //大Icon

// Operations
public:
	void SetStatusText(int pane, CString str);
	void InitializeSystemImageList();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;
	CReBar      m_wndReBar;
	CDialogBar      m_wndDlgBar;

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSelchangeComboFilter();
	afx_msg void OnButtonGoto();
	afx_msg void OnButtonFilter();
	afx_msg void OnButtonFind();
	afx_msg void OnButtonBack();
	afx_msg void OnButtonDown();
	afx_msg void OnButtonUpper();
	afx_msg void OnClose();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__4FC7DA9C_F2C8_4FC3_8C74_5F8A6D57E7BE__INCLUDED_)
