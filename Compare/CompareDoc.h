// CompareDoc.h : interface of the CCompareDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_COMPAREDOC_H__CEC64C69_48F7_40BE_8B57_0BE3D3457AE6__INCLUDED_)
#define AFX_COMPAREDOC_H__CEC64C69_48F7_40BE_8B57_0BE3D3457AE6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define BlockSize 8192

class CFirstView;
class CSecondView;
class CReportView;

class CCompareDoc : public CDocument
{
protected: // create from serialization only
	CCompareDoc();
	DECLARE_DYNCREATE(CCompareDoc)

// Attributes
public:
	CFirstView*  m_pFirstView;
	CSecondView* m_pSecondView;
	CReportView* m_pReportView;

	CString m_name1;
	CString m_name2;
	byte* m_file1;
	byte* m_file2;
	unsigned long m_filelen1;
	unsigned long m_filelen2;

	BOOL m_is_hex;
	BOOL m_is_cursor;

// Operations
public:
	void LoadFile1();
	void LoadFile2();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCompareDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	protected:
	virtual BOOL SaveModified();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCompareDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CCompareDoc)
	afx_msg void OnToolCompare();
	afx_msg void OnToolHex();
	afx_msg void OnUpdateToolHex(CCmdUI* pCmdUI);
	afx_msg void OnToolCursor();
	afx_msg void OnUpdateToolCursor(CCmdUI* pCmdUI);
	afx_msg void OnFileNew1();
	afx_msg void OnFileNew2();
	afx_msg void OnFileSave1();
	afx_msg void OnFileSave2();
	afx_msg void OnFileOpen1();
	afx_msg void OnFileOpen2();
	afx_msg void OnFileSaveAs1();
	afx_msg void OnFileSaveAs2();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COMPAREDOC_H__CEC64C69_48F7_40BE_8B57_0BE3D3457AE6__INCLUDED_)
