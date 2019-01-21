// JoinGroupDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CMulticastSocket.h"
#include "JoinGroupDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CJoinGroupDlg dialog


CJoinGroupDlg::CJoinGroupDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CJoinGroupDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CJoinGroupDlg)
	m_Loopback = FALSE;
	m_nPort = 0;
	m_nTTL = 0;
	//}}AFX_DATA_INIT
}


void CJoinGroupDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CJoinGroupDlg)
	DDX_Text(pDX, IDC_IPADDRESS, m_IP);
	DDX_Check(pDX, IDC_LOOPBACK, m_Loopback);
	DDX_Text(pDX, IDC_PORT, m_nPort);
	DDV_MinMaxUInt(pDX, m_nPort, 1024, 65535);
	DDX_Text(pDX, IDC_TTL, m_nTTL);
	DDV_MinMaxUInt(pDX, m_nTTL, 0, 128);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CJoinGroupDlg, CDialog)
	//{{AFX_MSG_MAP(CJoinGroupDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CJoinGroupDlg message handlers


