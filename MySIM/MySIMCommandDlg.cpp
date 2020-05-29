// MySIMCommandDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MySIM.h"
#include "MySIMCommandDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// Global variables

typedef struct _tCmdTable
{
	char  *name;
	char  *apdu;
} tCmdTable;

tCmdTable _cmdTable[] = {
	{ "SELECT",            "A0 A4 00 00 02" },
	{ "STATUS",            "A0 F2 00 00"    },
	{ "READ BINARY",       "A0 B0 00 00"    },
	{ "UPDATE BINARY",     "A0 D6 00 00"    },
	{ "READ RECORD",       "A0 B2 00 00"    },
	{ "UPDATE RECORD",     "A0 DC 00 00"    },
	{ "SEEK",              "A0 A2 00 00"    },
	{ "INCREASE",          "A0 32 00 00 03" },
	{ "VERIFY CHV",        "A0 20 00 01 08" },
	{ "CHANGE CHV",        "A0 24 00 01 10" },
	{ "DISABLE CHV",       "A0 26 00 01 08" },
	{ "ENABLE CHV",        "A0 28 00 01 08" },
	{ "UNBLOCK CHV",       "A0 2C 00 01 10" },
	{ "INVALIDATE",        "A0 04 00 00 00" },
	{ "REHABILITATE",      "A0 44 00 00 00" },
	{ "RUN GSM ALGORITHM", "A0 88 00 00 10" },
	{ "SLEEP",             "A0 FA 00 00 00" },
	{ "GET RESPONSE",      "A0 C0 00 00"    },
	{ "TERMINAL PROFILE",  "A0 10 00 00"    },
	{ "ENVELOPE",          "A0 C2 00 00"    },
	{ "FETCH",             "A0 12 00 00"    },
	{ "TERMINAL RESPONSE", "A0 14 00 00"    },

	{ "<NULL>",            "" }
};
int _cmdTableSize = (sizeof( _cmdTable ) / sizeof( tCmdTable ));


/////////////////////////////////////////////////////////////////////////////
// CMySIMCommandDlg dialog

CMySIMCommandDlg::CMySIMCommandDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMySIMCommandDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMySIMCommandDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_cmdAPDU = "";
}


void CMySIMCommandDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMySIMCommandDlg)
	DDX_Control(pDX, IDC_COMBO_COMMAND, m_ComboCmd);
	DDX_Control(pDX, IDC_EDIT_RAWDATA, m_EditCmd);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMySIMCommandDlg, CDialog)
	//{{AFX_MSG_MAP(CMySIMCommandDlg)
	ON_BN_CLICKED(IDC_BUTTON_OK, OnButtonOk)
	ON_CBN_SELCHANGE(IDC_COMBO_COMMAND, OnSelchangeComboCommand)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMySIMCommandDlg message handlers

void CMySIMCommandDlg::OnOK() 
{
	// TODO: Add extra validation here

	CDialog::OnOK();
}

void CMySIMCommandDlg::OnButtonOk() 
{
	// TODO: Add your control notification handler code here
	m_EditCmd.GetWindowText( m_cmdAPDU );

	CDialog::OnOK();
}

BOOL CMySIMCommandDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	//CComboBox* combo = (CComboBox*)GetDlgItem(IDC_COMBO_COMMAND);
	int i;

	for (i=0; i<_cmdTableSize; i++)
	{
		m_ComboCmd.AddString( _cmdTable[i].name );
	}
	m_ComboCmd.SetWindowText( "Please choice a command ..." );

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMySIMCommandDlg::OnSelchangeComboCommand() 
{
	// TODO: Add your control notification handler code here
	CString cmd, apdu;
	int i;

	m_ComboCmd.GetLBText(m_ComboCmd.GetCurSel(), cmd);

	for (i=0; i<_cmdTableSize; i++)
	{
		if (cmd.Compare( _cmdTable[i].name ) == 0)
		{
			apdu.Format("%s", _cmdTable[i].apdu);
			m_EditCmd.SetWindowText( apdu );
		}
	}
}
