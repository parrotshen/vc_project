// MySIMReaderDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MySIM.h"
#include "MySIMReaderDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMySIMReaderDlg dialog


CMySIMReaderDlg::CMySIMReaderDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMySIMReaderDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMySIMReaderDlg)
	//}}AFX_DATA_INIT

	m_selectName   = "";
	m_readerName   = NULL;
	m_readerNumber = 0;
}


void CMySIMReaderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMySIMReaderDlg)
	DDX_Control(pDX, IDC_LIST_READER, m_listReader);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMySIMReaderDlg, CDialog)
	//{{AFX_MSG_MAP(CMySIMReaderDlg)
	ON_LBN_SELCHANGE(IDC_LIST_READER, OnSelchangeListReader)
	ON_LBN_DBLCLK(IDC_LIST_READER, OnDblclkListReader)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMySIMReaderDlg message handlers

BOOL CMySIMReaderDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	char *p = m_readerName;
	while ( *p )
	{
    	for (int i=0; p[i]; i++);
	    i++;
	    if (*p != 0)
		{
     		m_listReader.AddString(p);
			m_readerNumber++;
		}
		p = &p[i];
	}

	if ( m_readerNumber )
	{
		m_listReader.SetCurSel(0);

		OnSelchangeListReader();
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMySIMReaderDlg::OnSelchangeListReader() 
{
	// TODO: Add your control notification handler code here
	m_listReader.GetText(m_listReader.GetCurSel(), m_selectName);
}

void CMySIMReaderDlg::OnDblclkListReader() 
{
	// TODO: Add your control notification handler code here
	m_listReader.GetText(m_listReader.GetCurSel(), m_selectName);
	OnOK();
}

void CMySIMReaderDlg::OnOK()
{
	// TODO: Add extra validation here
	m_readerName = (char *)(LPCTSTR)m_selectName;
	
	CDialog::OnOK();
}

