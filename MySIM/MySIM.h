// MySIM.h : main header file for the MYSIM application
//

#if !defined(AFX_MYSIM_H__7EEE33E5_029F_4043_8BEE_58E6F273BDBC__INCLUDED_)
#define AFX_MYSIM_H__7EEE33E5_029F_4043_8BEE_58E6F273BDBC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CMySIMApp:
// See MySIM.cpp for the implementation of this class
//

class CMySIMApp : public CWinApp
{
public:
	CMySIMApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMySIMApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CMySIMApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYSIM_H__7EEE33E5_029F_4043_8BEE_58E6F273BDBC__INCLUDED_)
