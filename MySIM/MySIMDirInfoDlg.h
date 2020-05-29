#if !defined(AFX_MYSIMDIRINFODLG_H__0F5257CD_C256_4684_962B_EC874AE36B97__INCLUDED_)
#define AFX_MYSIMDIRINFODLG_H__0F5257CD_C256_4684_962B_EC874AE36B97__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MySIMDirInfoDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMySIMDirInfoDlg dialog

#include "SimFileSys.h"

class CMySIMDirInfoDlg : public CDialog
{
// Construction
public:
	CMySIMDirInfoDlg(CWnd* pParent = NULL);   // standard constructor

	void InitialListCtrl();
	void AddListItem(tMfDfParam *paramMfDf);

	tMfDfParam *m_pParamMfDf;

// Dialog Data
	//{{AFX_DATA(CMySIMDirInfoDlg)
	enum { IDD = IDD_DIR_INFO };
	CListCtrl	m_listParam;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMySIMDirInfoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMySIMDirInfoDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYSIMDIRINFODLG_H__0F5257CD_C256_4684_962B_EC874AE36B97__INCLUDED_)
