#if !defined(AFX_SERVERDLG_H__F6E88B39_73F8_4A81_8CCB_70AEAFD22209__INCLUDED_)
#define AFX_SERVERDLG_H__F6E88B39_73F8_4A81_8CCB_70AEAFD22209__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ServerDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CServerDlg dialog

class CServerDlg : public CDialog
{
// Construction
public:
	CServerDlg(CWnd* pParent = NULL);   // standard constructor
	CString m_ip;
	int m_port;

// Dialog Data
	//{{AFX_DATA(CServerDlg)
	enum { IDD = IDD_SERVERDLG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CServerDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CServerDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SERVERDLG_H__F6E88B39_73F8_4A81_8CCB_70AEAFD22209__INCLUDED_)
