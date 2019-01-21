// HexConvertDlg.h : header file
//

#if !defined(AFX_HEXCONVERTDLG_H__DC5BB5AF_77A1_4372_B399_179C662E1437__INCLUDED_)
#define AFX_HEXCONVERTDLG_H__DC5BB5AF_77A1_4372_B399_179C662E1437__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CHexConvertDlg dialog

#define MAX_BUF_LENGTH  8190

class CHexConvertDlg : public CDialog
{
// Construction
public:
	CHexConvertDlg(CWnd* pParent = NULL);	// standard constructor

private:
	UINT  PrepareByteData(const char *inData, BYTE *outData, UINT outLen);;

	CString  m_StringAscii;
	CString  m_StringHex;
	BYTE     m_Buffer[MAX_BUF_LENGTH+2];
	DWORD    m_BufferLen;


// Dialog Data
	//{{AFX_DATA(CHexConvertDlg)
	enum { IDD = IDD_HEXCONVERT_DIALOG };
	CButton	m_ButtonHex;
	CButton	m_ButtonAscii;
	CEdit	m_EditHex;
	CEdit	m_EditAscii;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHexConvertDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CHexConvertDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButtonExit();
	afx_msg void OnButtonToHex();
	afx_msg void OnButtonToAscii();
	afx_msg void OnSetfocusEditAscii();
	afx_msg void OnSetfocusEditHex();
	afx_msg void OnButtonClear();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HEXCONVERTDLG_H__DC5BB5AF_77A1_4372_B399_179C662E1437__INCLUDED_)
