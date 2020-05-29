// SkyeyeDlg.h : header file
//

#if !defined(AFX_SKYEYEDLG_H__313B5400_7248_4B6D_9291_59DCA961B83B__INCLUDED_)
#define AFX_SKYEYEDLG_H__313B5400_7248_4B6D_9291_59DCA961B83B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


/////////////////////////////////////////////////////////////////////////////
// CSkyeyeDlg dialog

class CSkyeyeDlg : public CDialog
{
// Construction
public:
	CMenu* m_MyMenu;

	CSkyeyeDlg(CWnd* pParent = NULL);	// standard constructor
	void InitialBMPFormat();
	void InitSubControl();
	void StartPreview();
	void StopPreview();

// Dialog Data
	//{{AFX_DATA(CSkyeyeDlg)
	enum { IDD = IDD_SKYEYE_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSkyeyeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CSkyeyeDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnFuncStart();
	afx_msg void OnFuncStop();
	afx_msg void OnHelpAbout();
	afx_msg void OnReleasedcaptureSliderTh(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonStart();
	afx_msg void OnButtonStop();
	afx_msg void OnToolSource();
	afx_msg void OnToolFormat();
	afx_msg void OnToolDisplay();
	afx_msg void OnToolSetting();
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SKYEYEDLG_H__313B5400_7248_4B6D_9291_59DCA961B83B__INCLUDED_)
