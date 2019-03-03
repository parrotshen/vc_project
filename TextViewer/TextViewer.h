// TextViewer.h : main header file for the TEXTVIEWER application
//

#if !defined(AFX_TEXTVIEWER_H__0AF30C9A_CD6C_4C2D_B5DB_BA8E6CA1D25B__INCLUDED_)
#define AFX_TEXTVIEWER_H__0AF30C9A_CD6C_4C2D_B5DB_BA8E6CA1D25B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CTextViewerApp:
// See TextViewer.cpp for the implementation of this class
//

class CTextViewerApp : public CWinApp
{
public:
	CTextViewerApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTextViewerApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CTextViewerApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TEXTVIEWER_H__0AF30C9A_CD6C_4C2D_B5DB_BA8E6CA1D25B__INCLUDED_)
