#if !defined(AFX_MYSIMEFDATA_H__0F908C42_903A_4C72_8903_7DA1894380B3__INCLUDED_)
#define AFX_MYSIMEFDATA_H__0F908C42_903A_4C72_8903_7DA1894380B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MySIMEfData.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMySIMEfDataDlg dialog

class CMySIMEfDataDlg : public CDialog
{
// Construction
public:
	CMySIMEfDataDlg(CWnd* pParent = NULL);   // standard constructor

	CFont    m_font;
	CString  m_fid;
	CString  m_data;

// Dialog Data
	//{{AFX_DATA(CMySIMEfDataDlg)
	enum { IDD = IDD_EF_DATA };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMySIMEfDataDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMySIMEfDataDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnOk();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYSIMEFDATA_H__0F908C42_903A_4C72_8903_7DA1894380B3__INCLUDED_)
