// NetworkBoardView.h : interface of the CNetworkBoardView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_NETWORKBOARDVIEW_H__D0242500_6FB3_4369_A7D5_C1CAE5054E7B__INCLUDED_)
#define AFX_NETWORKBOARDVIEW_H__D0242500_6FB3_4369_A7D5_C1CAE5054E7B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CNetworkBoardDoc;
class CServerSocket;
class CUserSocket;

class CNetworkBoardView : public CEditView
{
protected: // create from serialization only
	CNetworkBoardView();
	DECLARE_DYNCREATE(CNetworkBoardView)

// Attributes
public:
	CNetworkBoardDoc* GetDocument();
	CServerSocket* m_Server;
	CUserSocket* m_HeadSocket;    //串列首的指標
	CUserSocket* m_TailSocket;    //串列尾的指標
	CUserSocket* m_CurrentSocket; //目前節點的指標
	UINT  m_Index;                //串列長度

	CString m_Connection;
	CFont m_Font;

// Operations
public:
	void ProcessingAccept();
	void DeleteClient();
	void SendToClient();
	void SendToAllUser(LPCTSTR lpszMessage, int length);
	void AppendMessage(LPCSTR lpszMessage);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNetworkBoardView)
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
	virtual ~CNetworkBoardView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CNetworkBoardView)
	afx_msg void OnAppListen();
	afx_msg void OnUpdateAppListen(CCmdUI* pCmdUI);
	afx_msg void OnAppUser();
	afx_msg void OnAppRefresh();
	afx_msg void OnEditSelect();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in NetworkBoardView.cpp
inline CNetworkBoardDoc* CNetworkBoardView::GetDocument()
   { return (CNetworkBoardDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NETWORKBOARDVIEW_H__D0242500_6FB3_4369_A7D5_C1CAE5054E7B__INCLUDED_)
