#if !defined(AFX_MYAPP_H__931717F6_8AEA_11D3_993E_0080C83ABF4B__INCLUDED_)
#define AFX_MYAPP_H__931717F6_8AEA_11D3_993E_0080C83ABF4B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyApp.h : header file
//

#include "Resource.h"

/////////////////////////////////////////////////////////////////////////////
// CMyApp thread

class CMyApp : public CWinApp
{
public:
	CMyApp();           // protected constructor used by dynamic creation

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMyApp();

	// Generated message map functions
	//{{AFX_MSG(CMyApp)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYAPP_H__931717F6_8AEA_11D3_993E_0080C83ABF4B__INCLUDED_)
