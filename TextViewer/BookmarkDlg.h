#if !defined(AFX_BOOKMARKDLG_H__8E819E16_D7CC_479B_A29F_449F59A6AA9E__INCLUDED_)
#define AFX_BOOKMARKDLG_H__8E819E16_D7CC_479B_A29F_449F59A6AA9E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BookmarkDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBookmarkDlg dialog

class CBookmarkDlg : public CDialog
{
// Construction
public:
	CBookmarkDlg(CWnd* pParent = NULL);   // standard constructor
	void UpdateINIFile();

	CString m_INI_Path;
	CString m_ID;            // [Section]
	CString m_FileInfo_Name; // Key=File
	CString m_FileInfo_Path; // Key=Path
	int m_MarkLine;          // Key=Line

// Dialog Data
	//{{AFX_DATA(CBookmarkDlg)
	enum { IDD = IDD_BOOKMARK };
	CEdit	m_Name;
	CStatic	m_Line;
	CStatic	m_File;
	CListBox	m_Bookmark;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBookmarkDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CBookmarkDlg)
	afx_msg void OnButtonAdd();
	afx_msg void OnButtonDelete();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeListBookmark();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BOOKMARKDLG_H__8E819E16_D7CC_479B_A29F_449F59A6AA9E__INCLUDED_)
