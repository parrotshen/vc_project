#if !defined(AFX_MYSIMREADERDLG_H__55255A75_4235_42E0_AF64_C3768700C606__INCLUDED_)
#define AFX_MYSIMREADERDLG_H__55255A75_4235_42E0_AF64_C3768700C606__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MySIMReaderDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMySIMReaderDlg dialog

class CMySIMReaderDlg : public CDialog
{
// Construction
public:
	CMySIMReaderDlg(CWnd* pParent = NULL);   // standard constructor

	CString  m_selectName;
	char    *m_readerName;
	int      m_readerNumber;

// Dialog Data
	//{{AFX_DATA(CMySIMReaderDlg)
	enum { IDD = IDD_READER_DIALOG };
	CListBox	m_listReader;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMySIMReaderDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMySIMReaderDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeListReader();
	virtual void OnOK();
	afx_msg void OnDblclkListReader();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYSIMREADERDLG_H__55255A75_4235_42E0_AF64_C3768700C606__INCLUDED_)
