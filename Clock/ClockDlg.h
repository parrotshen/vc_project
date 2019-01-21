// ClockDlg.h : header file
//

#if !defined(AFX_CLOCKDLG_H__637D04AE_43E8_40C9_965A_6365EC3D66DD__INCLUDED_)
#define AFX_CLOCKDLG_H__637D04AE_43E8_40C9_965A_6365EC3D66DD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CClockDlg dialog
#include "TrayIcon.h"

#define WM_MY_TRAY WM_USER+0
#define ID_TIMER   2000
#define ID_ALERT   2001
#define ID_TRAY    2002


class CClockDlg : public CDialog
{
// Construction
public:
	CMenu*    m_MyMenu;
	CMenu*    m_PopMenu;
	CBitmap   m_Bitmap_1;
	CBitmap   m_Bitmap_2;
	CTime     m_Current_Time;
	CString   m_Show_Time;
	CString   m_Alert_Memo;
	CTrayIcon m_MyTray;

	UINT m_MyTimer;
	UINT m_MyAlert;
	BOOL m_IsTopmost;     //1-是否最上層顯示之旗號
	BOOL m_IsTransparent; //2-是否半透明化旗號
	BOOL m_IsMinimized;   //3-是否最小化之旗號
	BOOL m_IsAlert;       //4-是否設定鬧鐘之旗號
	BOOL m_IsBeep;        //5-是否設定整點報時之旗號
	BOOL m_IsMute;        //6-是否要鬧鈴靜音之旗號
	BOOL m_IsTiming;      //7-是否要顯示時間之旗號
	int  m_Current_Month;
	int  m_Current_Day;
	int  m_Current_Hour;
	int  m_Current_Minute;
	int  m_Current_Second;
	int  m_Count;
	int  m_Alert_Month;
	int  m_Alert_Day;
	int  m_Alert_Hour;
	int  m_Alert_Minute;

	CClockDlg(CWnd* pParent = NULL); // standard constructor
	void SetClockNumber(int nID, UINT nNumber);
	void SetAlertLabel(BOOL value);
	void ReadScheduleRecord();
	void RemoveScheduleRecord();
	// **********************************
	// * CTrayIcon 是由 Reinhard 所贊助 *
	// **********************************

// Dialog Data
	//{{AFX_DATA(CClockDlg)
	enum { IDD = IDD_CLOCK_DIALOG };
	CStatic	m_S2;
	CStatic	m_S1;
	CStatic	m_M2;
	CStatic	m_M1;
	CStatic	m_H2;
	CStatic	m_H1;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CClockDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CClockDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSettingTopmost();
	afx_msg void OnHelpAbout();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnClose();
	afx_msg void OnSettingTrayicon();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnFuncBeep();
	afx_msg void OnFuncAlert();
	afx_msg void OnFuncExit();
	afx_msg void OnSettingAlert();
	afx_msg void OnFuncMute();
	afx_msg void OnSettingTransparent();
	//}}AFX_MSG

	//Begin 攔截 WM_MY_TRAY 的訊息
	afx_msg LRESULT OnNotifyTray(WPARAM uID, LPARAM lEvent);
	//End 攔截 WM_MY_TRAY 的訊息
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CLOCKDLG_H__637D04AE_43E8_40C9_965A_6365EC3D66DD__INCLUDED_)
