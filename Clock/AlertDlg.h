#if !defined(AFX_ALERTDLG_H__6BA5A38C_B749_41AF_94EB_877BD6544848__INCLUDED_)
#define AFX_ALERTDLG_H__6BA5A38C_B749_41AF_94EB_877BD6544848__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AlertDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAlertDlg dialog

class CAlertDlg : public CDialog
{
// Construction
public:
	BOOL    m_IsSelect;

	CAlertDlg(CWnd* pParent = NULL);   // standard constructor
	void InitialDate();
	void InitialListCtrl();
	void AddListItem(CString field1, CString field2);

// Dialog Data
	//{{AFX_DATA(CAlertDlg)
	enum { IDD = IDD_ALERTDLG };
	CListCtrl	m_Schedule;
	int		m_Month;
	int		m_Day;
	int		m_Hour;
	int		m_Minute;
	CString	m_Message;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAlertDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAlertDlg)
	afx_msg void OnButtonAdd();
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonDel();
	virtual void OnOK();
	afx_msg void OnClickListSchedule(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ALERTDLG_H__6BA5A38C_B749_41AF_94EB_877BD6544848__INCLUDED_)
