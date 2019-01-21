#if !defined(AFX_TRAYICON_H__B6D15C7D_2D3A_496D_9146_D917097FDAEB__INCLUDED_)
#define AFX_TRAYICON_H__B6D15C7D_2D3A_496D_9146_D917097FDAEB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TrayIcon.h : header file
//


/////////////////////////////////////////////////////////////////////////////
// CTrayIcon command target

class CTrayIcon : public CCmdTarget
{
	DECLARE_DYNCREATE(CTrayIcon)

	CTrayIcon(); // protected constructor used by dynamic creation

// Attributes
public:

// Operations
public:
	UINT SetTrayIcon(UINT uResourceID, const char* pTip=NULL);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTrayIcon)
	//}}AFX_VIRTUAL

// Implementation
protected:
	NOTIFYICONDATA m_MyIcon;
	BOOL m_bIconSet;

public:
	BOOL RemoveTrayIcon();
	BOOL Initial(UINT uID, CWnd *pNotifyWnd, UINT uCallBackMsg=0);
	virtual ~CTrayIcon();

	// Generated message map functions
	//{{AFX_MSG(CTrayIcon)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TRAYICON_H__B6D15C7D_2D3A_496D_9146_D917097FDAEB__INCLUDED_)
