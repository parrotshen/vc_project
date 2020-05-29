#if !defined(AFX_MYSIMCOMMANDDLG_H__7BF4AA8E_AD7D_4A48_BCFE_791AF4DEC9B4__INCLUDED_)
#define AFX_MYSIMCOMMANDDLG_H__7BF4AA8E_AD7D_4A48_BCFE_791AF4DEC9B4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MySIMCommandDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMySIMCommandDlg dialog

class CMySIMCommandDlg : public CDialog
{
// Construction
public:
	CMySIMCommandDlg(CWnd* pParent = NULL);   // standard constructor

	CString  m_cmdAPDU;

// Dialog Data
	//{{AFX_DATA(CMySIMCommandDlg)
	enum { IDD = IDD_COMMAND_DIALOG };
	CComboBox	m_ComboCmd;
	CEdit	m_EditCmd;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMySIMCommandDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMySIMCommandDlg)
	virtual void OnOK();
	afx_msg void OnButtonOk();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeComboCommand();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYSIMCOMMANDDLG_H__7BF4AA8E_AD7D_4A48_BCFE_791AF4DEC9B4__INCLUDED_)
