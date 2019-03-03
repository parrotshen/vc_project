// NetworkBoardView.h : interface of the CNetworkBoardView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_NETWORKBOARDVIEW_H__8DD60F8E_464E_4365_A72B_4B14E1ED65F1__INCLUDED_)
#define AFX_NETWORKBOARDVIEW_H__8DD60F8E_464E_4365_A72B_4B14E1ED65F1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CNetworkBoardDoc;
class CClientSocket;

class CNetworkBoardView : public CEditView
{
protected: // create from serialization only
	CNetworkBoardView();
	DECLARE_DYNCREATE(CNetworkBoardView)

// Attributes
public:
	CNetworkBoardDoc* GetDocument();

	CClientSocket* m_Client;
	CString m_ServerIp;
	UINT    m_ServerPort;
	CFont   m_Font;
	BOOL    m_IsModify;

// Operations
public:
	void ConnectToServer();
	void DisconnectToServer();
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
	afx_msg void OnUpdate();
	afx_msg void OnToolConnect();
	afx_msg void OnUpdateToolConnect(CCmdUI* pCmdUI);
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

#endif // !defined(AFX_NETWORKBOARDVIEW_H__8DD60F8E_464E_4365_A72B_4B14E1ED65F1__INCLUDED_)
