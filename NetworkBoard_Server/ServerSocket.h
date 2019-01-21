#if !defined(AFX_SERVERSOCKET_H__4A149D1A_E23D_483F_98FB_0BCF546918B8__INCLUDED_)
#define AFX_SERVERSOCKET_H__4A149D1A_E23D_483F_98FB_0BCF546918B8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ServerSocket.h : header file
//


class CNetworkBoardView;

/////////////////////////////////////////////////////////////////////////////
// CServerSocket command target

class CServerSocket : public CSocket
{
// Attributes
public:
	CNetworkBoardView* m_MyView;

// Operations
public:
	CServerSocket(CNetworkBoardView* view);
	virtual ~CServerSocket();

// Overrides
public:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CServerSocket)
	public:
	virtual void OnAccept(int nErrorCode);
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CServerSocket)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Implementation
protected:
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SERVERSOCKET_H__4A149D1A_E23D_483F_98FB_0BCF546918B8__INCLUDED_)
