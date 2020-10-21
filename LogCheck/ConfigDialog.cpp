// ConfigDialog.cpp : implementation file
//

#include "stdafx.h"
#include "LogCheck.h"
#include "ConfigDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CConfigDialog dialog


CConfigDialog::CConfigDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CConfigDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CConfigDialog)
	//}}AFX_DATA_INIT
	m_keywordNum = 0;
	m_timeout = 60;
}


void CConfigDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConfigDialog)
	DDX_Control(pDX, IDC_EDIT_TIMEOUT, m_EditTout);
	DDX_Control(pDX, IDC_LIST_CONFIG, m_CheckList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CConfigDialog, CDialog)
	//{{AFX_MSG_MAP(CConfigDialog)
	ON_BN_CLICKED(ID_CONFIG_OK, OnConfigOk)
	ON_BN_CLICKED(ID_CONFIG_ADD, OnConfigAdd)
	ON_BN_CLICKED(ID_CONFIG_DELETE, OnConfigDelete)
	ON_BN_CLICKED(ID_CONFIG_CANCEL, OnConfigCancel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConfigDialog message handlers

BOOL CConfigDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here
	LV_COLUMN col;
	DWORD style;

	col.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
	col.fmt = LVCFMT_LEFT;
	col.iSubItem = 0;
	col.pszText = "Label";
	col.cx = 100;
	m_CheckList.InsertColumn(0, &col);

	col.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
	col.fmt = LVCFMT_LEFT;
	col.iSubItem = 1;
	col.pszText = "Keyword";
	col.cx = 400;
	m_CheckList.InsertColumn(1, &col);

	style = m_CheckList.GetExtendedStyle();
	style |= LVS_EX_FULLROWSELECT;
	style |= LVS_EX_GRIDLINES;
	m_CheckList.SetExtendedStyle( style );


	/* Show configuration */
	CString tout;
	UINT i;

	for (i=0; i<m_keywordNum; i++)
	{
		m_CheckList.InsertItem(i, m_label[i]);
		m_CheckList.SetItemText(i, 1, (LPCTSTR)m_keyword[i]);
	}

	tout.Format("%u", m_timeout);
	m_EditTout.SetWindowText( tout );

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CConfigDialog::OnConfigOk() 
{
	// TODO: Add your control notification handler code here
	UpdateData( TRUE );

	CString tout;
	UINT i;

	m_EditTout.GetWindowText( tout );
	m_timeout = _ttoi( tout );
	m_keywordNum = m_CheckList.GetItemCount();
	for (i=0; i<m_keywordNum; i++)
	{
		memset(m_label[i], 0, 64);
		strncpy(
			m_label[i],
			(char *)(LPCTSTR)m_CheckList.GetItemText(i, 0),
			63
		);

		memset(m_keyword[i], 0, 256);
		strncpy(
			m_keyword[i],
			(char *)(LPCTSTR)m_CheckList.GetItemText(i, 1),
			255
		);	
	}

	CDialog::OnOK();
}

void CConfigDialog::OnConfigCancel() 
{
	// TODO: Add your control notification handler code here
	CDialog::OnCancel();
}

void CConfigDialog::OnConfigAdd() 
{
	// TODO: Add your control notification handler code here
	POSITION pos = m_CheckList.GetFirstSelectedItemPosition();
	int ndx = m_CheckList.GetItemCount();

	if (ndx >= 16)
	{
		AfxMessageBox("Cannot add more than 16 items!!");
		return;
	}

	// Add New Row
	if ( pos )
	{
		ndx = m_CheckList.GetNextSelectedItem( pos );
	}
	m_CheckList.InsertItem(ndx, "Label");
	m_CheckList.SetItemText(ndx, 1, (LPCTSTR)"Keyword to check ...");
}

void CConfigDialog::OnConfigDelete() 
{
	// TODO: Add your control notification handler code here
	POSITION pos = m_CheckList.GetFirstSelectedItemPosition();
	while ( pos )
	{
		int ndx = m_CheckList.GetNextSelectedItem( pos );
		m_CheckList.DeleteItem( ndx );
	}
}

