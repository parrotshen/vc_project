// SettingDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Skyeye.h"
#include "SettingDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSettingDlg dialog


CSettingDlg::CSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSettingDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSettingDlg)
	m_number = 0;
	m_threshold = 0;
	m_catch = FALSE;
	m_interval = 0.5f;
	//}}AFX_DATA_INIT
}


void CSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSettingDlg)
	DDX_Text(pDX, IDC_EDIT_NUMBER, m_number);
	DDX_Text(pDX, IDC_EDIT_THRESHOLD, m_threshold);
	DDX_Check(pDX, IDC_CHECK_CATCH, m_catch);
	DDX_Text(pDX, IDC_EDIT_INTERVAL, m_interval);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSettingDlg, CDialog)
	//{{AFX_MSG_MAP(CSettingDlg)
	ON_BN_CLICKED(IDC_CHECK_CATCH, OnCheckCatch)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CSettingDlg message handlers

BOOL CSettingDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	//Enable or Disable edit control
	CEdit* control1 = (CEdit*)GetDlgItem(IDC_EDIT_NUMBER);
	control1->EnableWindow(m_catch);
	CStatic* control2 = (CStatic*)GetDlgItem(IDC_STATIC_NUMBER);
	control2->EnableWindow(m_catch);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSettingDlg::OnCheckCatch() 
{
	UpdateData(TRUE);
	CEdit* control1 = (CEdit*)GetDlgItem(IDC_EDIT_NUMBER);
	control1->EnableWindow(m_catch);
	CStatic* control2 = (CStatic*)GetDlgItem(IDC_STATIC_NUMBER);
	control2->EnableWindow(m_catch);
}

