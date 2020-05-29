// XMLiteTest.h : main header file for the XMLITE application
//

#if !defined(AFX_XMLITE_H__AC3F9DD7_273D_44EB_8B7B_402E41790EFD__INCLUDED_)
#define AFX_XMLITE_H__AC3F9DD7_273D_44EB_8B7B_402E41790EFD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CXMLiteTestApp:
// See XMLiteTest.cpp for the implementation of this class
//

class CXMLiteTestApp : public CWinApp
{
public:
	CXMLiteTestApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXMLiteTestApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CXMLiteTestApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XMLITE_H__AC3F9DD7_273D_44EB_8B7B_402E41790EFD__INCLUDED_)
