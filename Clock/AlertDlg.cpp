// AlertDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Clock.h"
#include "AlertDlg.h"
#include "MyDatabase.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAlertDlg dialog


CAlertDlg::CAlertDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAlertDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAlertDlg)
	m_Month = 0;
	m_Day = 0;
	m_Hour = 0;
	m_Minute = 0;
	m_Message = _T("�ɶ���F!!");
	//}}AFX_DATA_INIT
	m_IsSelect = FALSE;
}


void CAlertDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAlertDlg)
	DDX_Control(pDX, IDC_LIST_SCHEDULE, m_Schedule);
	DDX_CBIndex(pDX, IDC_COMBO_HOUR, m_Hour);
	DDX_CBIndex(pDX, IDC_COMBO_MINUTE, m_Minute);
	DDX_Text(pDX, IDC_EDIT_MESSAGE, m_Message);
	DDX_CBIndex(pDX, IDC_COMBO_DAY, m_Day);
	DDX_CBIndex(pDX, IDC_COMBO_MONTH, m_Month);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAlertDlg, CDialog)
	//{{AFX_MSG_MAP(CAlertDlg)
	ON_BN_CLICKED(IDC_BUTTON_ADD, OnButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_DEL, OnButtonDel)
	ON_NOTIFY(NM_CLICK, IDC_LIST_SCHEDULE, OnClickListSchedule)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAlertDlg message handlers

BOOL CAlertDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	//��l�Ƥ��
	InitialDate();

	//��l�� List Control
	InitialListCtrl();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/////////////////////////////////////////////////////////////////////////////
// ��ܲ��l����T���B�z�禡

void CAlertDlg::OnClickListSchedule(NMHDR* pNMHDR, LRESULT* pResult) 
{
	//�P�_ List �����جO�_���Q���
	int ndx = m_Schedule.GetSelectionMark();
	if(ndx != -1)
		m_IsSelect = TRUE;

	*pResult = 0;
}

void CAlertDlg::OnButtonAdd() 
{
	//�W�[�@���Ƶ{����
	UpdateData(TRUE);

	CString time;
	time.Format("%02d/%02d %02d:%02d", m_Month+1, m_Day+1, m_Hour, m_Minute);
	AddListItem(time, m_Message);
}

void CAlertDlg::OnButtonDel() 
{
	//�R���@���Ƶ{����
	if( m_IsSelect )
	{
		int ndx = m_Schedule.GetSelectionMark();
		m_Schedule.DeleteItem(ndx);
		m_IsSelect = FALSE;
	}
}

void CAlertDlg::OnOK() 
{
	CMyDatabase schedule;
	schedule.Open(dbOpenDynaset, NULL, 0);
	//Begin �M����Ʈw
	while( !schedule.IsEOF() )
	{
		schedule.Delete();
		schedule.MoveNext();
	}
	//End �M����Ʈw

	//Begin ��s��Ʈw���e
	CString strList;
	for(int i=0; i<m_Schedule.GetItemCount(); i++)
	{
		strList = m_Schedule.GetItemText(i, 0);

		schedule.AddNew();
		schedule.m_Month  = atoi(strList.Mid(0, 2));
		schedule.m_Day    = atoi(strList.Mid(3, 2));
		schedule.m_Hour   = atoi(strList.Mid(6, 2));
		schedule.m_Minute = atoi(strList.Mid(9, 2));
		schedule.m_Memo   = strList.Mid(15, strList.GetLength()-15);
		schedule.Update();
	}
	//End ��s��Ʈw���e
	schedule.Close();

	CDialog::OnOK();
}

/////////////////////////////////////////////////////////////////////////////
// ���O�������禡

void CAlertDlg::InitialDate()
{
	CTime t = CTime::GetCurrentTime();

	//��l�Ƥ���P�ɶ�����
	CComboBox* month = (CComboBox*)GetDlgItem(IDC_COMBO_MONTH);
	month->SetCurSel( t.GetMonth()-1 );
	CComboBox* day = (CComboBox*)GetDlgItem(IDC_COMBO_DAY);
	day->SetCurSel( t.GetDay()-1 );
}

void CAlertDlg::InitialListCtrl()
{
	//Begin �إ߼��D���(Column)
	LV_COLUMN col;
	col.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
	col.fmt = LVCFMT_LEFT;
	col.iSubItem = 0;
	col.pszText = "���-�ɶ��@�@�ƥ󤺮e";
	col.cx = 300;
	m_Schedule.InsertColumn(0, &col);
	//End �إ߼��D���(Column)

	//Begin Ū���Ƶ{����Ʈw
	CString time, msg;
	CMyDatabase schedule;
	schedule.Open(dbOpenDynaset, NULL, 0);
	while( !schedule.IsEOF() )
	{
		int ndx = m_Schedule.GetItemCount();
		time.Format("%02d/%02d %02d:%02d", schedule.m_Month, schedule.m_Day, schedule.m_Hour, schedule.m_Minute);
		msg.Format("%s�@�@%s", time, schedule.m_Memo);
		m_Schedule.InsertItem(ndx, msg);

		schedule.MoveNext();
	}
	schedule.Close();
	//End Ū���Ƶ{����Ʈw
}

void CAlertDlg::AddListItem(CString field1, CString field2)
{
	int ndx = m_Schedule.GetItemCount();
	CString time, msg;
	msg.Format("%s�@�@%s", field1, field2);

	for(int i=0; i<ndx; i++)
	{
		//Begin �P�_���-�ɶ��O�_�ۦP
		time = m_Schedule.GetItemText(i, 0).Left(11);
		if(time == field1)
		{
			m_Schedule.SetItemText(i, 0, msg);
			return;
		}
		//End �P�_���-�ɶ��O�_�ۦP
	}
	m_Schedule.InsertItem(ndx, msg);
}
