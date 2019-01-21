// HexConvert.h : main header file for the HEXCONVERT application
//

#if !defined(AFX_HEXCONVERT_H__0F33E864_B70E_4E8A_830E_E572194C85F8__INCLUDED_)
#define AFX_HEXCONVERT_H__0F33E864_B70E_4E8A_830E_E572194C85F8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CHexConvertApp:
// See HexConvert.cpp for the implementation of this class
//

class CHexConvertApp : public CWinApp
{
public:
	CHexConvertApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHexConvertApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CHexConvertApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HEXCONVERT_H__0F33E864_B70E_4E8A_830E_E572194C85F8__INCLUDED_)
