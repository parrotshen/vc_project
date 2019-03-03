// ClientSocket.cpp : implementation file
//

#include "stdafx.h"
#include "NetworkBoard.h"
#include "NetworkBoardView.h"
#include "ClientSocket.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CClientSocket

CClientSocket::CClientSocket(CNetworkBoardView* view)
{
	m_MyView = view;
	memset(m_RecvData, 0, 1024);
}

CClientSocket::~CClientSocket()
{
}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CClientSocket, CSocket)
	//{{AFX_MSG_MAP(CClientSocket)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CClientSocket member functions

void CClientSocket::OnReceive(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	CSocket::OnReceive(nErrorCode);

	memset(m_RecvData, 0, 1024);
	//接收伺服器傳來的資料內容
	// +----+--------+
	// |Flag|  Data  |
	// +----+--------+
	int len = Receive(m_RecvData, 1024);
	if (len >= 4)
	{
		//取出文字訊息的長度 -> Flag
		int flag;
		memcpy(&flag, m_RecvData, 4);
		if (flag == 0)
		{
			m_MyView->SetWindowText( NULL );
			return;
		}
		
		//取出文字訊息的內容 -> Data
		char *pMsg = (char *)(m_RecvData + 4);
		if (flag == 1)
		{
			m_MyView->SetWindowText( (LPCTSTR)pMsg );
		}
		else
		{
			m_MyView->AppendMessage( (LPCTSTR)pMsg );
		}
	}
}

void CClientSocket::OnClose(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	CSocket::OnClose(nErrorCode);

	// Disconnect to Server
	m_MyView->DisconnectToServer();
}

