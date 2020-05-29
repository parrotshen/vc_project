#if !defined(AFX_MYSIMPINDLG_H__70A614A2_F2B9_41BC_B7CA_D4C98A69C419__INCLUDED_)
#define AFX_MYSIMPINDLG_H__70A614A2_F2B9_41BC_B7CA_D4C98A69C419__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MySIMPinDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMySIMPinDlg dialog

class CMySIMPinDlg : public CDialog
{
// Construction
public:
	CMySIMPinDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CMySIMPinDlg)
	enum { IDD = IDD_PIN_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMySIMPinDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMySIMPinDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYSIMPINDLG_H__70A614A2_F2B9_41BC_B7CA_D4C98A69C419__INCLUDED_)
