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

	//接收伺服器傳來的資料內容
	// +----+--------+
	// |Flag|  Data  |
	// +----+--------+
	int ReceDataLen = 1024;
	byte* ReceData = new byte[ReceDataLen+1];
	memset(ReceData, 0x00, ReceDataLen+1);
	int ret = Receive(ReceData, ReceDataLen);

	//取出文字訊息的長度 -> Flag
	int flag;
	memcpy(&flag, ReceData, sizeof(int));

	if(flag == 0)
	{
		m_MyView->SetWindowText( NULL );
		return;
	}

	//取出文字訊息的內容 -> Data
	int size = 1024 - sizeof(int);
	byte* buf = new byte[size];
	memset(buf, 0x00, size);
	memcpy(buf, ReceData+sizeof(int), size);
	if(flag == 1)
		m_MyView->SetWindowText( (LPCTSTR)buf );
	else
		m_MyView->AppendMessage( (LPCTSTR)buf );
	delete buf;

	delete ReceData;
}
