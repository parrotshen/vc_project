// Clock.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "Clock.h"
#include "ClockDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// Callback Function (讓程式只執行一次)

BOOL CALLBACK EnumWindowsProc(HWND hWnd, LPARAM lParam)
{
	//int GetWindowText(
	//			HWND hWnd,        // handle to window or control
	//			LPTSTR lpString,  // text buffer
	//			int nMaxCount     // maximum number of characters to copy
	//		);
	int nMaxCount = MAX_PATH;
	char lpString[MAX_PATH];
	GetWindowText(hWnd, lpString, nMaxCount);

	CString lpTest = lpString;
	if(lpTest.FindOneOf("小時鐘") >= 0)
	{
		//Begin 讓視窗回復原來的大小
		/*
		RECT lpRect;
		GetWindowRect(hWnd, &lpRect);
		POINT pt;
		pt.x = lpRect.left;
		pt.y = lpRect.top;
		WINDOWPLACEMENT lpwndpl;
		GetWindowPlacement(hWnd, &lpwndpl);
		lpwndpl.flags  = WPF_RESTORETOMAXIMIZED;
		lpwndpl.showCmd = SW_RESTORE;
		SetWindowPlacement(hWnd, &lpwndpl);
		*/
		//End 讓視窗回復原來的大小

		AfxMessageBox("這程式已經執行!!", MB_OK|MB_ICONERROR, 0);
		PostQuitMessage(WM_QUIT);
		return FALSE;
	}

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CClockApp

BEGIN_MESSAGE_MAP(CClockApp, CWinApp)
	//{{AFX_MSG_MAP(CClockApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CClockApp construction

CClockApp::CClockApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CClockApp object

CClockApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CClockApp initialization

BOOL CClockApp::InitInstance()
{
	AfxEnableControlContainer();

	// Standard initialization
	//Begin 列舉所有的視窗(讓程式只執行一次)
	//
	//BOOL EnumWindows(
	//			WNDENUMPROC lpEnumFunc,  // callback function
	//			LPARAM lParam            // application-defined value
	//		);
	EnumWindows(EnumWindowsProc, 0);
	//End 列舉所有的視窗

	//Begin 將底色畫成黑色
	//SetDialogBkColor(RGB(0, 0, 0));
	//End 將底色畫成黑色

	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	CClockDlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
