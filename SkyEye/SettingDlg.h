#if !defined(AFX_SETTINGDLG_H__168CB4A3_9B08_4092_A090_CE32970659CA__INCLUDED_)
#define AFX_SETTINGDLG_H__168CB4A3_9B08_4092_A090_CE32970659CA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SettingDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSettingDlg dialog

class CSettingDlg : public CDialog
{
// Construction
public:
	CSettingDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSettingDlg)
	enum { IDD = IDD_SETTING_DIALOG };
	int		m_number;
	int		m_threshold;
	BOOL	m_catch;
	float	m_interval;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSettingDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSettingDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnCheckCatch();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETTINGDLG_H__168CB4A3_9B08_4092_A090_CE32970659CA__INCLUDED_)
