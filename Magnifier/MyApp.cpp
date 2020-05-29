// MyApp.cpp : implementation file
//

#include "stdafx.h"
#include "MyApp.h"
#include "MainWindow.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyApp

CMyApp theApp;

CMyApp::CMyApp()
{
}

CMyApp::~CMyApp()
{
}

BOOL CMyApp::InitInstance()
{
	// TODO:  perform and per-thread initialization here
    m_pMainWnd = new CMainWindow;
    m_pMainWnd->ShowWindow (m_nCmdShow);
    m_pMainWnd->UpdateWindow ();

	HICON hIcon=LoadIcon(IDI_MYICON);
	m_pMainWnd->SetIcon(hIcon,TRUE);

    return TRUE;
}


BEGIN_MESSAGE_MAP(CMyApp, CWinApp)
	//{{AFX_MSG_MAP(CMyApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyApp message handlers
