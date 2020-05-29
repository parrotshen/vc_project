// MySIMDirInfoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MySIM.h"
#include "MySIMDirInfoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMySIMDirInfoDlg dialog


CMySIMDirInfoDlg::CMySIMDirInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMySIMDirInfoDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMySIMDirInfoDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pParamMfDf = NULL;
}


void CMySIMDirInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMySIMDirInfoDlg)
	DDX_Control(pDX, IDC_LIST_DIR_PARAM, m_listParam);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMySIMDirInfoDlg, CDialog)
	//{{AFX_MSG_MAP(CMySIMDirInfoDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMySIMDirInfoDlg message handlers

BOOL CMySIMDirInfoDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	InitialListCtrl();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


/////////////////////////////////////////////////////////////////////////////
// CMySIMDirInfoDlg member methods

void CMySIMDirInfoDlg::InitialListCtrl()
{
	//建立標題欄位(Column)
	LV_COLUMN col;
	col.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
	col.fmt = LVCFMT_LEFT;
	col.iSubItem = 0;
	col.pszText = "Item";
	col.cx = 150;
	m_listParam.InsertColumn(0, &col);

	col.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
	col.fmt = LVCFMT_LEFT;
	col.iSubItem = 1;
	col.pszText = "Data";
	col.cx = 150;
	m_listParam.InsertColumn(1, &col);


	//m_listParam.EnableWindow(FALSE);

	if ( m_pParamMfDf )
	{
		AddListItem( m_pParamMfDf );
	}
}


/////////////////////////////////////////////////////////////////////////////
// CMySIMDirInfoDlg member methods

void CMySIMDirInfoDlg::AddListItem(tMfDfParam *paramMfDf)
{
	int index = m_listParam.GetItemCount();
	CString string;


	m_listParam.InsertItem(index, "Memory size of directory");
	string.Format("%d", paramMfDf->size);
	m_listParam.SetItemText(index, 1, string);
	index++;

	m_listParam.InsertItem(index, "File ID");
	string.Format("%02X %02X", paramMfDf->fid[0], paramMfDf->fid[1]);
	m_listParam.SetItemText(index, 1, string);
	index++;

	m_listParam.InsertItem(index, "Type of file");
	switch ( paramMfDf->type )
	{
		case FILE_TYPE_MF:
			string.Format("MF");
			break;
		case FILE_TYPE_DF:
			string.Format("DF");
			break;
		case FILE_TYPE_EF:
			string.Format("EF");
			break;
		default:
			string.Empty();
	}
	m_listParam.SetItemText(index, 1, string);
	index++;

	m_listParam.InsertItem(index, "CHV1 enable/disable");
	string.Format(
		"%s",
		((paramMfDf->chv1Enable) ? "Enable" : "Disable")
	);
	m_listParam.SetItemText(index, 1, string);
	index++;

	m_listParam.InsertItem(index, "Number of DFs");
	string.Format("%d", paramMfDf->numDf);
	m_listParam.SetItemText(index, 1, string);
	index++;

	m_listParam.InsertItem(index, "Number of EFs");
	string.Format("%d", paramMfDf->numEf);
	m_listParam.SetItemText(index, 1, string);
	index++;

	m_listParam.InsertItem(index, "Number of CHVs, UNBLOCK CHVs");
	string.Format("%d", paramMfDf->numChv);
	m_listParam.SetItemText(index, 1, string);
	index++;

	m_listParam.InsertItem(index, "CHV1 initial/un-initial");
	string.Format(
		"%s",
		((paramMfDf->chv1Init) ? "Initialised" : "Not initialised")
	);
	m_listParam.SetItemText(index, 1, string);
	index++;

	m_listParam.InsertItem(index, "Number of CHV1 false remainder");
	if (paramMfDf->chv1Remain == 0)
	{
		string.Format("Blocked");
	}
	else
	{
		string.Format("%d", paramMfDf->chv1Remain);
	}
	m_listParam.SetItemText(index, 1, string);
	index++;

	m_listParam.InsertItem(index, "UNBLOCK CHV1 initial/un-initial");
	string.Format(
		"%s",
		((paramMfDf->chv1InitUnblock) ? "Initialised" : "Not initialised")
	);
	m_listParam.SetItemText(index, 1, string);
	index++;

	m_listParam.InsertItem(index, "Number of UNBLOCK CHV1 false remainder");
	if (paramMfDf->chv1RemainUnblock == 0)
	{
		string.Format("Blocked");
	}
	else
	{
		string.Format("%d", paramMfDf->chv1RemainUnblock);
	}
	m_listParam.SetItemText(index, 1, string);
	index++;

	m_listParam.InsertItem(index, "CHV2 initial/un-initial");
	string.Format(
		"%s",
		((paramMfDf->chv2Init) ? "Initialised" : "Not initialised")
	);
	m_listParam.SetItemText(index, 1, string);
	index++;

	m_listParam.InsertItem(index, "Number of CHV2 false remainder");
	if (paramMfDf->chv2Remain == 0)
	{
		string.Format("Blocked");
	}
	else
	{
		string.Format("%d", paramMfDf->chv2Remain);
	}
	m_listParam.SetItemText(index, 1, string);
	index++;

	m_listParam.InsertItem(index, "UNBLOCK CHV2 initial/un-initial");
	string.Format(
		"%s",
		((paramMfDf->chv2InitUnblock) ? "Initialised" : "Not initialised")
	);
	m_listParam.SetItemText(index, 1, string);
	index++;

	m_listParam.InsertItem(index, "Number of UNBLOCK CHV2 false remainder");
	if (paramMfDf->chv2RemainUnblock == 0)
	{
		string.Format("Blocked");
	}
	else
	{
		string.Format("%d", paramMfDf->chv2RemainUnblock);
	}
	m_listParam.SetItemText(index, 1, string);
	index++;
}
