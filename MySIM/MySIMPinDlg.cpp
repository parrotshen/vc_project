// MySIMPinDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MySIM.h"
#include "MySIMPinDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMySIMPinDlg dialog


CMySIMPinDlg::CMySIMPinDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMySIMPinDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMySIMPinDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CMySIMPinDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMySIMPinDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMySIMPinDlg, CDialog)
	//{{AFX_MSG_MAP(CMySIMPinDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMySIMPinDlg message handlers
