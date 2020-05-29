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
	m_Prev = NULL;
	m_Next = NULL;
	memset(m_RecvData, 0, 1024);
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

	memset(m_RecvData, 0, 1024);
	// ---------------------
	//  �ʥ]�榡(1024 bytes)
	// ---------------------
	// +----+--------+
	// |Flag|  Data  |
	// +----+--------+
	int len = Receive(m_RecvData, 1024);
	if (len >= 4)
	{
		// --------------------------
		//  �����ϥΪ̶ǨӪ���ƪ���
		// --------------------------
		// -1    = �ϥΪ������s�u
		// 0     = �M���ثe��r�����e
		// other = ���`��ܱ��������
		int flag;
		memcpy(&flag, m_RecvData, 4);
		switch ( flag )
		{
			case -1:
				//�ϥΪ̺������s�u
				m_MyView->DeleteClient();
				break;
			case 0:
				//�M���Ҧ���r
				m_MyView->SetWindowText( NULL );
				//�e����L�Ҧ��ϥΪ�
				m_MyView->SendToAllUser(NULL, 0);
				break;
			default:
				//��������ܨϥΪ̺ݪ����
				char *pStr = (char *)(m_RecvData + 4);
				if (flag == 1)
				{
					m_MyView->SetWindowText( (LPCTSTR)pStr );
				}
				else
				{
					m_MyView->AppendMessage( (LPCTSTR)pStr );
				}
				//�e����L�Ҧ��ϥΪ�
				m_MyView->SendToAllUser((LPCTSTR)m_RecvData, len);
				break;
		}
	}
}

