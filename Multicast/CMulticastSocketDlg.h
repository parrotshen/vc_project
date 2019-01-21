// CMulticastSocketDlg.h : header file
//

#if !defined(AFX_CMULTICASTSOCKETDLG_H__269E2C77_2037_11D3_8EF3_0000C0FD25F8__INCLUDED_)
#define AFX_CMULTICASTSOCKETDLG_H__269E2C77_2037_11D3_8EF3_0000C0FD25F8__INCLUDED_

#include "JoinGroupDlg.h"	// Added by ClassView
#include "MulticastSocket.h"	// Added by ClassView
#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

/////////////////////////////////////////////////////////////////////////////
// CCMulticastSocketDlg dialog

class CCMulticastSocketDlg : public CDialog
{
// Construction
public:
	CCMulticastSocketDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CCMulticastSocketDlg)
	enum { IDD = IDD_CMULTICASTSOCKET_DIALOG };
	CString	m_ChatMessage;
	CString	m_ChatWindow;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMulticastSocketDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CCMulticastSocketDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnJoin();
	afx_msg void OnSend();
	afx_msg void OnQuit();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnLeave();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CMulticastSocket m_Socket;
	CJoinGroupDlg m_JoinDlg;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMULTICASTSOCKETDLG_H__269E2C77_2037_11D3_8EF3_0000C0FD25F8__INCLUDED_)
