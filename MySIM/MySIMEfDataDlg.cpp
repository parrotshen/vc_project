// MySIMEfData.cpp : implementation file
//

#include "stdafx.h"
#include "MySIM.h"
#include "MySIMEfDataDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMySIMEfDataDlg dialog


CMySIMEfDataDlg::CMySIMEfDataDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMySIMEfDataDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMySIMEfDataDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CMySIMEfDataDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMySIMEfDataDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMySIMEfDataDlg, CDialog)
	//{{AFX_MSG_MAP(CMySIMEfDataDlg)
	ON_BN_CLICKED(IDC_OK, OnOk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMySIMEfDataDlg message handlers

BOOL CMySIMEfDataDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CEdit *pEdit = (CEdit *)GetDlgItem(IDC_EDIT_EF_DATA);

	if ( pEdit )
	{
		//Begin: Change Font to Fixedsys
		m_font.DeleteObject();
		m_font.CreateFont(
			-16,                   // nHeight
			0,                     // nWidth
			0,                     // nEscapement
			0,                     // nOrientation
			FW_NORMAL,             // nWeight
			FALSE,                 // bItalic
			FALSE,                 // bUnderline
			0,                     // cStrikeOut
			DEFAULT_CHARSET,       // nCharSet
			OUT_DEFAULT_PRECIS,    // nOutPrecision
			CLIP_DEFAULT_PRECIS,   // nClipPrecision
			DEFAULT_QUALITY,       // nQuality
			DEFAULT_PITCH,         // nPitchAndFamily
			"Fixedsys"             // lpszFacename
		);
		pEdit->SetFont(&m_font, TRUE);
		//End:   Change Font to Fixedsys

		CString string;
		int i, j;

		string.Empty();
		for (i=0, j=0; i<m_data.GetLength(); i+=3, j++)
		{
			if ((j != 0) && ((j % 8) == 0))
			{
				string += "\r\n";
			}
			string += m_data.Mid(i, 3);
		}
		pEdit->SetWindowText( string );
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMySIMEfDataDlg::OnOk() 
{
	// TODO: Add your control notification handler code here
	CDialog::OnOK();
}
