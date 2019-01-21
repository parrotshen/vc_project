// TrayIcon.cpp : implementation file
//

#include "stdafx.h"
#include "Clock.h"
#include "TrayIcon.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTrayIcon

IMPLEMENT_DYNCREATE(CTrayIcon, CCmdTarget)

CTrayIcon::CTrayIcon()
{
	memset(&m_MyIcon, 0x00, sizeof(NOTIFYICONDATA));
	m_MyIcon.cbSize = sizeof(NOTIFYICONDATA);
	m_bIconSet = FALSE;
}

CTrayIcon::~CTrayIcon()
{
	if(m_bIconSet == TRUE)
	{
		Shell_NotifyIcon(NIM_DELETE,&m_MyIcon);
	}
}


BEGIN_MESSAGE_MAP(CTrayIcon, CCmdTarget)
	//{{AFX_MSG_MAP(CTrayIcon)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTrayIcon message handlers

UINT CTrayIcon::SetTrayIcon(UINT uResourceID, const char* pTip)
{
	if(m_MyIcon.hWnd == 0)
		return 1; //Not Initial

	HICON hIcon = AfxGetApp()->LoadIcon(uResourceID);
	if(hIcon == NULL)
		return 2; //Load Icon Failed

	m_MyIcon.hIcon = hIcon;
	m_MyIcon.uFlags |= NIF_ICON;

	memset(&m_MyIcon.szTip, 0x00, 64);
	if(pTip != NULL)
	{
		int nTipLen = strlen(pTip);
		if(nTipLen > 64)
			return 3; //Tip String too long
		memcpy(&m_MyIcon.szTip, pTip, nTipLen);
		m_MyIcon.uFlags |= NIF_TIP;
	}
	if(m_bIconSet == FALSE) //Set a new Icon
	{

		if( !Shell_NotifyIcon(NIM_ADD, &m_MyIcon) )
			return 4; //Notify Icon Failed
	}
	else //Else change Icon
	{
		if( !Shell_NotifyIcon(NIM_MODIFY, &m_MyIcon) )
			return 4;
	}

	m_bIconSet = TRUE;
	return 0;
}

BOOL CTrayIcon::Initial(UINT uID, CWnd *pNotifyWnd, UINT uCallBackMsg)
{
	m_MyIcon.uID = uID;

	if(pNotifyWnd == NULL)
		return FALSE;
	HWND hPWnd = pNotifyWnd->GetSafeHwnd();
	if( !IsWindow(hPWnd) )
		return FALSE;
	m_MyIcon.hWnd = hPWnd;

	if(uCallBackMsg == 0)
		return FALSE;
	m_MyIcon.uCallbackMessage = uCallBackMsg;
	m_MyIcon.uFlags |= NIF_MESSAGE;

	return TRUE;
}

BOOL CTrayIcon::RemoveTrayIcon()
{
	if( !Shell_NotifyIcon(NIM_DELETE,&m_MyIcon) )
		return FALSE;

	m_bIconSet = FALSE;
	return TRUE;
}
