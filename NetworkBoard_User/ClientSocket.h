#if !defined(AFX_CLIENTSOCKET_H__72AC57DA_2A13_4E74_BFD5_C1399C18F4C6__INCLUDED_)
#define AFX_CLIENTSOCKET_H__72AC57DA_2A13_4E74_BFD5_C1399C18F4C6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ClientSocket.h : header file
//


class CNetworkBoardView;

/////////////////////////////////////////////////////////////////////////////
// CClientSocket command target

class CClientSocket : public CSocket
{
// Attributes
public:
	CNetworkBoardView* m_MyView;

// Operations
public:
	CClientSocket(CNetworkBoardView* view);
	virtual ~CClientSocket();

// Overrides
public:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CClientSocket)
	public:
	virtual void OnReceive(int nErrorCode);
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CClientSocket)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Implementation
protected:
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CLIENTSOCKET_H__72AC57DA_2A13_4E74_BFD5_C1399C18F4C6__INCLUDED_)
