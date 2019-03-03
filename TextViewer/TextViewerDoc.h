// TextViewerDoc.h : interface of the CTextViewerDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_TEXTVIEWERDOC_H__61A47C37_328F_4DA6_A2AF_7EC5D1A1E55B__INCLUDED_)
#define AFX_TEXTVIEWERDOC_H__61A47C37_328F_4DA6_A2AF_7EC5D1A1E55B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CTextViewerDoc : public CDocument
{
protected: // create from serialization only
	CTextViewerDoc();
	DECLARE_DYNCREATE(CTextViewerDoc)

// Attributes
public:
	CTextTreeView* m_pTreeView;
	CTextListView* m_pListView;
	CTextEditView* m_pEditView;
	unsigned long m_Text_Length; //記錄開啟的檔案內容的長度
	LPTSTR m_Text_Content;       //記錄開啟的檔案內容

	CString m_MyDirectory;   //程式執行的路徑
	CString m_ViewDirectory; //前次瀏覽檔案的路徑
	CString m_FileName;      //開啟或儲存檔案的完整檔名
	UINT    m_MenuNumber;
	BOOL    m_Is_Hex;
	BOOL    m_Is_Brk;
	BOOL    m_Is_ReadOnly;
	int     m_Total;

// Operations
public:
	BOOL DisplayFile(CString filename);
	void DisplayLines(CString filename);
	void SetNewDirectory(CString dir);
	void AddHistoryRecord();
	void OpenRecordFile(UINT nID);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTextViewerDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTextViewerDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CTextViewerDoc)
	afx_msg void OnFileNew();
	afx_msg void OnFileOpen();
	afx_msg void OnFileSaveAs();
	afx_msg void OnFileSave();
	afx_msg void OnToolHex();
	afx_msg void OnToolFont();
	afx_msg void OnUpdateToolHex(CCmdUI* pCmdUI);
	afx_msg void OnToolFind();
	afx_msg void OnToolReplace();
	afx_msg void OnToolBreak();
	afx_msg void OnUpdateToolBreak(CCmdUI* pCmdUI);
	afx_msg void OnEditSelect();
	afx_msg void OnButtonBookmark();
	afx_msg void OnButtonHistory();
	//}}AFX_MSG

	afx_msg void OnHistoryFile1();
	afx_msg void OnHistoryFile2();
	afx_msg void OnHistoryFile3();
	afx_msg void OnHistoryFile4();
	afx_msg void OnHistoryFile5();
	afx_msg void OnHistoryFile6();
	afx_msg void OnHistoryFile7();
	afx_msg void OnHistoryFile8();
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TEXTVIEWERDOC_H__61A47C37_328F_4DA6_A2AF_7EC5D1A1E55B__INCLUDED_)
