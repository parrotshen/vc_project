// UserSocket.cpp : implementation file
//

#include "stdafx.h"
#include "NetworkBoard.h"
#include "NetworkBoardView.h"
#include "UserSocket.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUserSocket

CUserSocket::CUserSocket(CNetworkBoardView* view)
{
	m_MyView = view;
	m_front = NULL;
	m_rear = NULL;
}

CUserSocket::~CUserSocket()
{
}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CUserSocket, CSocket)
	//{{AFX_MSG_MAP(CUserSocket)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CUserSocket member functions

void CUserSocket::OnReceive(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	CSocket::OnReceive(nErrorCode);

	m_MyView->m_CurrentSocket = this;

	// ---------------------
	//  封包格式(1024 bits)
	// ---------------------
	// +----+--------+
	// |Flag|  Data  |
	// +----+--------+
	int ReceDataLen = 1024;
	byte* ReceData = new byte[ReceDataLen+1];
	memset(ReceData, 0x00, ReceDataLen+1);
	int ret = Receive(ReceData, ReceDataLen);

	// --------------------------
	//  接收使用者傳來的資料長度
	// --------------------------
	// -1    = 使用者關閉連線
	// 0     = 清除目前文字的內容
	// other = 正常顯示接收的資料
	int flag;
	memcpy(&flag, ReceData, sizeof(int));
	switch( flag )
	{
		case -1:
			//使用者端關閉連線
			m_MyView->DeleteClient();
			break;
		case 0:
			//清除所有文字
			m_MyView->SetWindowText( NULL );
			//送給其他所有使用者
			m_MyView->SendToAllUser(NULL, 0);
			break;
		default:
			//接收並顯示使用者端的資料
			int size = 1024 - sizeof(int);
			byte* buf = new byte[size];
			memset(buf, 0x00, size);
			memcpy(buf, ReceData+sizeof(int), size);
			if(flag == 1)
				m_MyView->SetWindowText( (LPCTSTR)buf );
			else
				m_MyView->AppendMessage( (LPCTSTR)buf );
			delete buf;
			//送給其他所有使用者
			m_MyView->SendToAllUser((LPCTSTR)ReceData, ReceDataLen);
			break;
	}
	delete ReceData;
}
