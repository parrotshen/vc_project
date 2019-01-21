// ServerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NetworkBoard.h"
#include "ServerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CServerDlg dialog


CServerDlg::CServerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CServerDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CServerDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CServerDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CServerDlg, CDialog)
	//{{AFX_MSG_MAP(CServerDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CServerDlg message handlers

BOOL CServerDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	//初始化 IP Address 值
	SetDlgItemText(IDC_IPADDRESS_IP, m_ip);
	//初始化 Port 值
	char* port = new char[5];
	memset(port, 0x00, 5);
	_itoa(m_port, port, 10);
	SetDlgItemText(IDC_EDIT_PORT, port);
	delete port;

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CServerDlg::OnOK() 
{
	//取得伺服器的 IP
	GetDlgItemText(IDC_IPADDRESS_IP, m_ip);
	//取得伺服器的 Port
	CString strPort;
	GetDlgItemText(IDC_EDIT_PORT, strPort);
	m_port = atoi( (LPCTSTR)strPort );

	CDialog::OnOK();
}
