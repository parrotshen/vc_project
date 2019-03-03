#if !defined(AFX_USERSOCKET_H__6EDE7747_47F8_4AB5_839C_591AA2D01B5A__INCLUDED_)
#define AFX_USERSOCKET_H__6EDE7747_47F8_4AB5_839C_591AA2D01B5A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// UserSocket.h : header file
//


class CNetworkBoardView;

/////////////////////////////////////////////////////////////////////////////
// CUserSocket command target

class CUserSocket : public CSocket
{
// Attributes
public:
	CNetworkBoardView* m_MyView;
	CUserSocket* m_Prev;
	CUserSocket* m_Next;
	BYTE  m_RecvData[1024];

	///////////////////////////////////////////////
	// Socket-Link-List Structure:               //
	//        +-+------+-+   +-+------+-+        //
	// NULL<--| |1     | |<--| |2     | |        //
	//        |F|Socket|R|   |F|Socket|R|        //
	//        | |      | |-->| |      | |-->NULL //
	//        +-+------+-+   +-+------+-+        //
	///////////////////////////////////////////////

// Operations
public:
	CUserSocket(CNetworkBoardView* view);
	virtual ~CUserSocket();

// Overrides
public:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUserSocket)
	public:
	virtual void OnReceive(int nErrorCode);
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CUserSocket)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Implementation
protected:
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_USERSOCKET_H__6EDE7747_47F8_4AB5_839C_591AA2D01B5A__INCLUDED_)
