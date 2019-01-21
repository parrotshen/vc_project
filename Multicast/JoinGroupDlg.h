#if !defined(AFX_JOINGROUPDLG_H__33640B14_204A_11D3_8EF4_0000C0FD25F8__INCLUDED_)
#define AFX_JOINGROUPDLG_H__33640B14_204A_11D3_8EF4_0000C0FD25F8__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// JoinGroupDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CJoinGroupDlg dialog

class CJoinGroupDlg : public CDialog
{
// Construction
public:
	CJoinGroupDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CJoinGroupDlg)
	enum { IDD = IDD_JOINGROUP_DIALOG };
	CString	m_IP;
	BOOL	m_Loopback;
	UINT	m_nPort;
	UINT	m_nTTL;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CJoinGroupDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CJoinGroupDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_JOINGROUPDLG_H__33640B14_204A_11D3_8EF4_0000C0FD25F8__INCLUDED_)
