#if !defined(AFX_MYSIMFILEINFODLG_H__02AA4BE3_E29E_446F_96B4_F0639D3F4D9C__INCLUDED_)
#define AFX_MYSIMFILEINFODLG_H__02AA4BE3_E29E_446F_96B4_F0639D3F4D9C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MySIMFileInfoDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMySIMFileInfoDlg dialog

#include "SimFileSys.h"

class CMySIMFileInfoDlg : public CDialog
{
// Construction
public:
	CMySIMFileInfoDlg(CWnd* pParent = NULL);   // standard constructor

	void InitialListCtrl();
	void AddListItem(tEfParam *paramEf);
	CString GetAccessCondition(BYTE cond);

	tEfParam *m_pParamEf;

// Dialog Data
	//{{AFX_DATA(CMySIMFileInfoDlg)
	enum { IDD = IDD_FILE_INFO };
	CListCtrl	m_listParam;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMySIMFileInfoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMySIMFileInfoDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYSIMFILEINFODLG_H__02AA4BE3_E29E_446F_96B4_F0639D3F4D9C__INCLUDED_)
