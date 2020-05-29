#if !defined(AFX_MAINWND_H__931717F5_8AEA_11D3_993E_0080C83ABF4B__INCLUDED_)
#define AFX_MAINWND_H__931717F5_8AEA_11D3_993E_0080C83ABF4B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MainWindow.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMainWindow frame

class CMainWindow : public CFrameWnd
{
public:
	CMainWindow(); // protected constructor used by dynamic creation

// Attributes
public:
	// DWORD = 4*byte
	COLORREF* m_pColorArray;
	CRect m_ClientRect;
	HDC   m_hDesktopDC;

	int   m_Wnd_Width;  //視窗寬度
	int   m_Wnd_Height; //視窗高度
	int   m_Multiple;   //放大倍數

// Operations
public:
	void StartWindowsHook();
	void StopWindowsHook();
	void ResizeColorArray(BOOL IsButtonUp);
	void ShowWindowBackground();
	void DrawBackground();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainWindow)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CMainWindow();

	// Generated message map functions
	//{{AFX_MSG(CMainWindow)
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
	afx_msg void OnNcLButtonUp(UINT nHitTest, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINWND_H__931717F5_8AEA_11D3_993E_0080C83ABF4B__INCLUDED_)
