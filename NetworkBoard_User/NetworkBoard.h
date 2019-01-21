// NetworkBoard.h : main header file for the NETWORKBOARD application
//

#if !defined(AFX_NETWORKBOARD_H__3C48157E_81BF_470F_8FFE_62C5B844A475__INCLUDED_)
#define AFX_NETWORKBOARD_H__3C48157E_81BF_470F_8FFE_62C5B844A475__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CNetworkBoardApp:
// See NetworkBoard.cpp for the implementation of this class
//

class CNetworkBoardApp : public CWinApp
{
public:
	CNetworkBoardApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNetworkBoardApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CNetworkBoardApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NETWORKBOARD_H__3C48157E_81BF_470F_8FFE_62C5B844A475__INCLUDED_)
