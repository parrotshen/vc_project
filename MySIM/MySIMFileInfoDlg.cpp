// MySIMFileInfoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MySIM.h"
#include "MySIMFileInfoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMySIMFileInfoDlg dialog


CMySIMFileInfoDlg::CMySIMFileInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMySIMFileInfoDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMySIMFileInfoDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pParamEf = NULL;
}


void CMySIMFileInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMySIMFileInfoDlg)
	DDX_Control(pDX, IDC_LIST_FILE_PARAM, m_listParam);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMySIMFileInfoDlg, CDialog)
	//{{AFX_MSG_MAP(CMySIMFileInfoDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMySIMFileInfoDlg message handlers

BOOL CMySIMFileInfoDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	InitialListCtrl();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


/////////////////////////////////////////////////////////////////////////////
// CMySIMDirInfoDlg member methods

void CMySIMFileInfoDlg::InitialListCtrl()
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

	if ( m_pParamEf )
	{
		AddListItem( m_pParamEf );
	}
}


/////////////////////////////////////////////////////////////////////////////
// CMySIMFileInfoDlg member methods

CString CMySIMFileInfoDlg::GetAccessCondition(BYTE cond)
{
	CString string;

	switch ( cond )
	{
		case ACCESS_ALW:
			string.Format("Always");
			break;
		case ACCESS_CHV1:
			string.Format("CHV1");
			break;
		case ACCESS_CHV2:
			string.Format("CHV2");
			break;
		case ACCESS_RFU:
			string.Format("RFU");
			break;
		case ACCESS_ADM4:
		case ACCESS_ADM5:
		case ACCESS_ADM6:
		case ACCESS_ADM7:
		case ACCESS_ADM8:
		case ACCESS_ADM9:
		case ACCESS_ADM10:
		case ACCESS_ADM11:
		case ACCESS_ADM12:
		case ACCESS_ADM13:
		case ACCESS_ADM14:
			string.Format("Administrator");
			break;
		case ACCESS_NEW:
		default:
			string.Format("New");
	}

	return string;
}

void CMySIMFileInfoDlg::AddListItem(tEfParam *paramEf)
{
	int index = m_listParam.GetItemCount();
	CString string;


	m_listParam.InsertItem(index, "File size");
	string.Format("%d", paramEf->size);
	m_listParam.SetItemText(index, 1, string);
	index++;

	m_listParam.InsertItem(index, "File ID");
	string.Format("%02X %02X", paramEf->fid[0], paramEf->fid[1]);
	m_listParam.SetItemText(index, 1, string);
	index++;

	m_listParam.InsertItem(index, "Type of file");
	switch ( paramEf->type )
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

	m_listParam.InsertItem(index, "Access condition of update");
	string = GetAccessCondition( paramEf->condUpdate );
	m_listParam.SetItemText(index, 1, string);
	index++;

	m_listParam.InsertItem(index, "Access condition of read");
	string = GetAccessCondition( paramEf->condRead );
	m_listParam.SetItemText(index, 1, string);
	index++;

	m_listParam.InsertItem(index, "Access condition of increase");
	string = GetAccessCondition( paramEf->condIncrease );
	m_listParam.SetItemText(index, 1, string);
	index++;

	m_listParam.InsertItem(index, "Access condition of invalidate");
	string = GetAccessCondition( paramEf->condInvalidate );
	m_listParam.SetItemText(index, 1, string);
	index++;

	m_listParam.InsertItem(index, "Access condition of rehabilitate");
	string = GetAccessCondition( paramEf->condRehabilitate );
	m_listParam.SetItemText(index, 1, string);
	index++;

	m_listParam.InsertItem(index, "File structure");
	switch ( paramEf->structure )
	{
		case FILE_STRUCT_BINARY:
			string.Format("Binary");
			break;
		case FILE_STRUCT_RECORD:
			string.Format("Record");
			break;
		default:
			string.Empty();
	}
	m_listParam.SetItemText(index, 1, string);
	index++;

	if (paramEf->structure == FILE_STRUCT_RECORD)
	{
		m_listParam.InsertItem(index, "Length of a record");
		string.Format("%d", paramEf->recLen);
		m_listParam.SetItemText(index, 1, string);
		index++;
		
		m_listParam.InsertItem(index, "Number of records");
		string.Format("%d", paramEf->recNum);
		m_listParam.SetItemText(index, 1, string);
		index++;
	}
}

