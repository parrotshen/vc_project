#if !defined(AFX_CONFIGDIALOG_H__143624E6_EBA9_4D38_93D1_B24F28DB3192__INCLUDED_)
#define AFX_CONFIGDIALOG_H__143624E6_EBA9_4D38_93D1_B24F28DB3192__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ConfigDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CConfigDialog dialog

#include "EditListCtrl.h"

class CConfigDialog : public CDialog
{
// Construction
public:
	CConfigDialog(CWnd* pParent = NULL);   // standard constructor
	CHAR  m_label[16][64];
	CHAR  m_keyword[16][256];
	UINT  m_keywordNum;
	UINT  m_timeout;

// Dialog Data
	//{{AFX_DATA(CConfigDialog)
	enum { IDD = IDD_DIALOG_CONFIG };
	CEdit	m_EditTout;
	CEditListCtrl	m_CheckList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CConfigDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CConfigDialog)
	afx_msg void OnConfigOk();
	virtual BOOL OnInitDialog();
	afx_msg void OnConfigAdd();
	afx_msg void OnConfigDelete();
	afx_msg void OnConfigCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONFIGDIALOG_H__143624E6_EBA9_4D38_93D1_B24F28DB3192__INCLUDED_)
