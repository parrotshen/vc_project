// LogCheck.h : main header file for the MONETONOFF application
//

#if !defined(AFX_MONETONOFF_H__36F32493_0E6D_4305_B928_20453425BB13__INCLUDED_)
#define AFX_MONETONOFF_H__36F32493_0E6D_4305_B928_20453425BB13__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CLogCheckApp:
// See LogCheck.cpp for the implementation of this class
//

class CLogCheckApp : public CWinApp
{
public:
	CLogCheckApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLogCheckApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CLogCheckApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MONETONOFF_H__36F32493_0E6D_4305_B928_20453425BB13__INCLUDED_)
