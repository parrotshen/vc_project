// ClockDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Clock.h"
#include "ClockDlg.h"
#include "AlertDlg.h"
#include "TrayIcon.h"
#include "MyDatabase.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CClockDlg dialog

CClockDlg::CClockDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CClockDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CClockDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon   = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_MyMenu  = NULL;
	m_PopMenu = NULL;

	m_IsTopmost     = FALSE; //1-�O�_�̤W�h��ܤ��X��
	m_IsMinimized   = FALSE; //2-�O�_�̤p�Ƥ��X��
	m_IsTransparent = FALSE; //2-�O�_�b�z���ƺX��
	m_IsAlert       = FALSE; //4-�O�_�]�w�x�����X��
	m_IsBeep        = FALSE; //5-�O�_�]�w���I���ɤ��X��
	m_IsMute        = FALSE; //6-�O�_�n�x�a�R�����X��
	m_IsTiming      = FALSE; //7-�O�_�n��ܮɶ����X��
}

void CClockDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CClockDlg)
	DDX_Control(pDX, IDC_STATIC_S2, m_S2);
	DDX_Control(pDX, IDC_STATIC_S1, m_S1);
	DDX_Control(pDX, IDC_STATIC_M2, m_M2);
	DDX_Control(pDX, IDC_STATIC_M1, m_M1);
	DDX_Control(pDX, IDC_STATIC_H2, m_H2);
	DDX_Control(pDX, IDC_STATIC_H1, m_H1);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CClockDlg, CDialog)
	//{{AFX_MSG_MAP(CClockDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_COMMAND(ID_SETTING_TOPMOST, OnSettingTopmost)
	ON_COMMAND(ID_HELP_ABOUT, OnHelpAbout)
	ON_WM_TIMER()
	ON_WM_CLOSE()
	ON_COMMAND(ID_SETTING_TRAYICON, OnSettingTrayicon)
	ON_WM_SIZE()
	ON_COMMAND(ID_FUNC_BEEP, OnFuncBeep)
	ON_COMMAND(ID_FUNC_ALERT, OnFuncAlert)
	ON_COMMAND(ID_FUNC_EXIT, OnFuncExit)
	ON_COMMAND(ID_SETTING_ALERT, OnSettingAlert)
	ON_COMMAND(ID_FUNC_MUTE, OnFuncMute)
	ON_COMMAND(ID_SETTING_TRANSPARENT, OnSettingTransparent)
	//}}AFX_MSG_MAP

	//Begin �d�I WM_MY_TRAY ���T��
	ON_MESSAGE (WM_MY_TRAY, OnNotifyTray)
	//End �d�I WM_MY_TRAY ���T��
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CClockDlg message handlers

BOOL CClockDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	//Begin ���J�\���
	m_MyMenu = new CMenu;
	m_MyMenu->LoadMenu(IDR_MYMENU);
	SetMenu(m_MyMenu);

	m_PopMenu = m_MyMenu->GetSubMenu(0);
	//End ���J�\���

	//Begin ��l�� CTrayIcon ����
	m_MyTray.Initial(ID_TRAY, this, WM_MY_TRAY);
	//End ��l�� CTrayIcon ����

	//Begin �ϥξx�a�\��
	m_IsAlert = TRUE;
	m_MyMenu->CheckMenuItem(ID_FUNC_ALERT, MF_CHECKED);
	//End �ϥξx�a�\��

	//Begin Ū����Ʈw
	m_Current_Time  = CTime::GetCurrentTime();
	m_Current_Month = m_Current_Time.GetMonth();
	m_Current_Day   = m_Current_Time.GetDay();
	ReadScheduleRecord();
	//End Ū����Ʈw

	//Begin �Ұʤp����
	m_IsTiming = TRUE;
	m_MyTimer = SetTimer(ID_TIMER, 1000, 0);
	if( !m_MyTimer ) AfxMessageBox("�L�k�ϥ� Timer �귽!!");
	//End �Ұʤp����

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CClockDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CClockDlg::OnPaint() 
{
	//Begin �N����e���¦�
	CPaintDC dc(this);
	CRect rect;
	GetClientRect(rect);
	CRgn rgn;
	rgn.CreateRectRgn(rect.left, rect.top, rect.right, rect.bottom);
	CBrush brush(RGB(0, 0, 0));
	dc.FillRgn(&rgn, &brush);
	//End �N����e���¦�

	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

void CClockDlg::OnClose() 
{
	//���� Timer �귽
	KillTimer(m_MyTimer);
	//delete m_MyMenu;
	//delete m_PopMenu;

	CDialog::OnClose();
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CClockDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

/////////////////////////////////////////////////////////////////////////////
// �\����B�z�禡

void CClockDlg::OnFuncAlert() 
{
	//�x���\��
	m_IsAlert = !m_IsAlert;
	if( m_IsAlert )
		m_MyMenu->CheckMenuItem(ID_FUNC_ALERT, MF_CHECKED);
	else
		m_MyMenu->CheckMenuItem(ID_FUNC_ALERT, MF_UNCHECKED);
}

void CClockDlg::OnFuncBeep() 
{
	//���I���ɥ\��
	m_IsBeep = !m_IsBeep;
	if( m_IsBeep )
		m_MyMenu->CheckMenuItem(ID_FUNC_BEEP, MF_CHECKED);
	else
		m_MyMenu->CheckMenuItem(ID_FUNC_BEEP, MF_UNCHECKED);
}

void CClockDlg::OnFuncMute() 
{
	//�x�a�R���\��
	m_IsMute = !m_IsMute;
	if( m_IsMute )
		m_MyMenu->CheckMenuItem(ID_FUNC_MUTE, MF_CHECKED);
	else
		m_MyMenu->CheckMenuItem(ID_FUNC_MUTE, MF_UNCHECKED);
}

void CClockDlg::OnFuncExit() 
{
	CDialog::OnOK();
}

void CClockDlg::OnSettingAlert() 
{
	//�]�w�x���ɶ�
	CAlertDlg dlg;
	if(dlg.DoModal() == IDOK)
	{
		//Ū���Ĥ@���O��(���o Month, Day, Hour, Minute)
		ReadScheduleRecord();
	}
}

void CClockDlg::OnSettingTopmost() 
{
	//�����̤W�h���
	//BOOL b = ModifyStyleEx(0, WS_EX_TOPMOST);
	RECT rect;
	GetWindowRect( &rect );
	int x  = rect.left;
	int y  = rect.top;
	int cx = rect.right - x;
	int cy = rect.bottom - y;

	m_IsTopmost = !m_IsTopmost;
	if( m_IsTopmost )
	{
		m_MyMenu->CheckMenuItem(ID_SETTING_TOPMOST, MF_CHECKED);
		SetWindowPos(&wndTopMost, x, y, cx, cy, SWP_NOMOVE|SWP_SHOWWINDOW);
	}
	else
	{
		m_MyMenu->CheckMenuItem(ID_SETTING_TOPMOST, MF_UNCHECKED);
		SetWindowPos(&wndNoTopMost, x, y, cx, cy, SWP_NOMOVE|SWP_SHOWWINDOW);
	}
}

void CClockDlg::OnSettingTransparent() 
{
	//�����ܬ��b�z��
	int degree;
	m_IsTransparent = !m_IsTransparent;
	if( m_IsTransparent )
	{
		m_MyMenu->CheckMenuItem(ID_SETTING_TRANSPARENT, MF_CHECKED);
		degree = 144;
	}
	else
	{
		m_MyMenu->CheckMenuItem(ID_SETTING_TRANSPARENT, MF_UNCHECKED);
		degree = 255;
	}

	//Begin �b�z�����A
	SetWindowLong(this->GetSafeHwnd(), GWL_EXSTYLE, GetWindowLong(this->GetSafeHwnd(), GWL_EXSTYLE)^0x80000);
	HINSTANCE hInst = LoadLibrary( "User32.DLL" );
	if( hInst )
	{
		typedef BOOL (WINAPI *MYFUNC)(HWND, COLORREF, BYTE, DWORD);
		MYFUNC fun = NULL;

		fun = (MYFUNC)GetProcAddress(hInst, "SetLayeredWindowAttributes");
		if( fun ) fun(this->GetSafeHwnd(), 0, degree, 2);
		FreeLibrary( hInst );
	}
	//End �b�z�����A
}

void CClockDlg::OnSettingTrayicon() 
{
	//�����Y��t�Τu��C
	m_IsTiming = FALSE;
	m_MyTray.SetTrayIcon(IDR_MAINFRAME, "�p����");
	ShowWindow( SW_HIDE );
}

void CClockDlg::OnHelpAbout() 
{
	CAboutDlg dlg;
	dlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// �����j�p���ܪ��T���B�z�禡

void CClockDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);

	//�P�_�����O�_���̤p��
	if(nType == SIZE_MINIMIZED)
	{
		m_IsMinimized = TRUE;
	}
	else
	{
		m_IsMinimized = FALSE;
		SetWindowText("�p����");
	}
}

/////////////////////////////////////////////////////////////////////////////
// TIMER ���T���B�z�禡

void CClockDlg::OnTimer(UINT nIDEvent) 
{
	switch( nIDEvent )
	{
		case ID_TIMER:
			m_Current_Time   = CTime::GetCurrentTime();
			m_Current_Month  = m_Current_Time.GetMonth();
			m_Current_Day    = m_Current_Time.GetDay();
			m_Current_Hour   = m_Current_Time.GetHour();
			m_Current_Minute = m_Current_Time.GetMinute();
			m_Current_Second = m_Current_Time.GetSecond();

			//Begin �ˬd��{�ɶ��O�_���
			if(m_Current_Month==m_Alert_Month && m_Current_Day==m_Alert_Day)
			{
				if(m_Current_Hour==m_Alert_Hour && m_Current_Minute==m_Alert_Minute && m_Current_Second==0)
				{
					//�O�_�ϥξx�a�\��
					if( m_IsAlert )
					{
						m_Count = 0;
						if( !m_IsMute )
							m_MyAlert = SetTimer(ID_ALERT, 1000, 0);

						AfxMessageBox(m_Alert_Memo, MB_OK|MB_SYSTEMMODAL|MB_ICONINFORMATION, 0);
						KillTimer(m_MyAlert);
					}
					//�R���ثe���O��
					RemoveScheduleRecord();
					//Ū���Ĥ@���O��
					ReadScheduleRecord();
				}
			}
			//End �ˬd��{�ɶ��O�_���

			//Begin �O�_�n���I����
			if( m_IsBeep )
			{
				if(m_Current_Minute==0 && m_Current_Second==0)
				{
					Beep(800, 100);
					Beep(800, 100);
				}
			}
			//End �O�_�n���I����

			//Begin ��ܮɶ�
			if( m_IsTiming )
			{
				if( m_IsMinimized )
				{
					m_Show_Time.Format("�p���� - %s", m_Current_Time.Format("%H:%M:%S"));
					SetWindowText(m_Show_Time);
				}
				else
				{
					SetClockNumber(1, m_Current_Hour);
					SetClockNumber(2, m_Current_Minute);
					SetClockNumber(3, m_Current_Second);
				}
			}
			//End ��ܮɶ�
			break;

		case ID_ALERT:
			//Begin �o�X 10 ���a�n
			m_Count++;
			if(m_Count >= 10) KillTimer(m_MyAlert);

			Beep(1000, 200);
			Beep(700, 200);
			//End �o�X 10 ���a�n
			break;
	}

	CDialog::OnTimer(nIDEvent);
}

/////////////////////////////////////////////////////////////////////////////
// �B�z WM_MY_TRAY �T�����禡

LRESULT CClockDlg::OnNotifyTray(WPARAM uID, LPARAM lEvent)
{
	if(lEvent == WM_RBUTTONUP)
	{
		//��ܼu�X���\���
		CPoint position;
		GetCursorPos( &position );
		SetForegroundWindow();
		m_PopMenu->TrackPopupMenu(TPM_LEFTALIGN, position.x, position.y, this);
	}

	if(lEvent == WM_LBUTTONUP)
	{
		//��ܥX����
		m_IsTiming = TRUE;
		m_MyTray.RemoveTrayIcon();
		ShowWindow( SW_SHOW );
	}

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// ���O�������禡

void CClockDlg::SetClockNumber(int nID, UINT nNumber)
{
	//���ɡB���B���Ʀr�ϧ�
	m_Bitmap_1.LoadBitmap(nNumber/10 + 130);
	m_Bitmap_2.LoadBitmap(nNumber%10 + 130);
	switch( nID )
	{
		case 1:
			m_H1.SetBitmap((HBITMAP)m_Bitmap_1);
			m_H2.SetBitmap((HBITMAP)m_Bitmap_2);
			break;
		case 2:
			m_M1.SetBitmap((HBITMAP)m_Bitmap_1);
			m_M2.SetBitmap((HBITMAP)m_Bitmap_2);
			break;
		case 3:
			m_S1.SetBitmap((HBITMAP)m_Bitmap_1);
			m_S2.SetBitmap((HBITMAP)m_Bitmap_2);
			break;
	}
	m_Bitmap_1.DeleteObject();
	m_Bitmap_2.DeleteObject();
}

void CClockDlg::SetAlertLabel(BOOL value)
{
	//Begin �����\�����r
	CString msg;
	if( value )
		msg.Format("�x�� (%02d:%02d:00)", m_Alert_Hour, m_Alert_Minute);
	else
		msg = "�x�� (--:--:--)";
	m_PopMenu->ModifyMenu(ID_FUNC_ALERT, MF_BYCOMMAND|MF_STRING, ID_FUNC_ALERT, msg);
	//End �����\�����r

	if( m_IsAlert )
		m_MyMenu->CheckMenuItem(ID_FUNC_ALERT, MF_CHECKED);
	else
		m_MyMenu->CheckMenuItem(ID_FUNC_ALERT, MF_UNCHECKED);
}

void CClockDlg::ReadScheduleRecord()
{
	m_Alert_Month = 0;
	m_Alert_Day = 0;

	//Begin Ū���@���O��
	CMyDatabase schedule;
	schedule.Open(dbOpenDynaset, NULL, 0);
	while( !schedule.IsEOF() )
	{
		//Ū���Ĥ@����{���ɶ�
		//schedule.MoveFirst();
		m_Alert_Month  = schedule.m_Month;
		m_Alert_Day    = schedule.m_Day;
		m_Alert_Hour   = schedule.m_Hour;
		m_Alert_Minute = schedule.m_Minute;
		m_Alert_Memo   = schedule.m_Memo;

		//�N�ߩ�ثe�ɶ�����{�O���R��
		if(m_Alert_Month < m_Current_Time.GetMonth())
		{
			schedule.Delete();
		}
		else if(m_Alert_Month == m_Current_Time.GetMonth())
		{
			if(m_Alert_Day < m_Current_Time.GetDay())
			{
				schedule.Delete();
			}
			else if(m_Alert_Day == m_Current_Time.GetDay())
			{
				if(m_Alert_Hour < m_Current_Time.GetHour())
				{
					schedule.Delete();
				}
				else if(m_Alert_Hour == m_Current_Time.GetHour())
				{
					if(m_Alert_Minute < m_Current_Time.GetMinute())
						schedule.Delete();
					else
						break;
				}
				else
				{
					break;
				}
			}
			else
			{
				break;
			}
		}
		else
		{
			break;
		}
		schedule.MoveNext();
	}
	schedule.Close();
	//End Ū���@���O��

	//Begin ���ܥ\����x���ɶ�
	if(m_Alert_Month==m_Current_Month && m_Alert_Day==m_Current_Day)
		SetAlertLabel(TRUE);
	else
		SetAlertLabel(FALSE);
	//End ���ܥ\����x���ɶ�
}

void CClockDlg::RemoveScheduleRecord()
{
	//Begin �����@����ưO��
	CMyDatabase schedule;
	schedule.Open(dbOpenDynaset, NULL, 0);
	if( !schedule.IsEOF() )
	{
		schedule.MoveFirst();
		schedule.Delete();
	}
	schedule.Close();
	//End �����@����ưO��
}
