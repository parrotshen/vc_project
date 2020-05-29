// Skyeye.h : main header file for the SKYEYE application
//

#if !defined(AFX_SKYEYE_H__03F29596_3E9C_403F_89CA_8210EA9FA461__INCLUDED_)
#define AFX_SKYEYE_H__03F29596_3E9C_403F_89CA_8210EA9FA461__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CSkyeyeApp:
// See Skyeye.cpp for the implementation of this class
//

class CSkyeyeApp : public CWinApp
{
public:
	CSkyeyeApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSkyeyeApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CSkyeyeApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SKYEYE_H__03F29596_3E9C_403F_89CA_8210EA9FA461__INCLUDED_)
