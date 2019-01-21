// CMulticastSocket.h : main header file for the CMULTICASTSOCKET application
//

#if !defined(AFX_CMULTICASTSOCKET_H__269E2C75_2037_11D3_8EF3_0000C0FD25F8__INCLUDED_)
#define AFX_CMULTICASTSOCKET_H__269E2C75_2037_11D3_8EF3_0000C0FD25F8__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CCMulticastSocketApp:
// See CMulticastSocket.cpp for the implementation of this class
//

class CCMulticastSocketApp : public CWinApp
{
public:
	CCMulticastSocketApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMulticastSocketApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CCMulticastSocketApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMULTICASTSOCKET_H__269E2C75_2037_11D3_8EF3_0000C0FD25F8__INCLUDED_)
