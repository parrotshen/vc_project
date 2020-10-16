// LogCheckDoc.h : interface of the CLogCheckDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MONETONOFFDOC_H__4D4388E0_9A8B_4158_B477_1549A3CDF61C__INCLUDED_)
#define AFX_MONETONOFFDOC_H__4D4388E0_9A8B_4158_B477_1549A3CDF61C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MySerialIF.h"


class CLogCheckEditView;
class CLogCheckListView;

class CLogCheckDoc : public CDocument
{
protected: // create from serialization only
	CLogCheckDoc();
	DECLARE_DYNCREATE(CLogCheckDoc)

// Attributes
public:
	CLogCheckEditView* m_pEditView;
	CLogCheckListView* m_pListView;

	tMySerialParam  m_SerialParam;
	BOOL  m_bConnected;

	CHAR  m_label[16][64];
	CHAR  m_keyword[16][256];
	UINT  m_keywordNum;
	UINT  m_timeout;

// Operations
public:
	void CloseConnection();
	BOOL ReadXML(CString name);
	void WriteXML(CString name);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLogCheckDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual void OnCloseDocument();
	protected:
	virtual BOOL SaveModified();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CLogCheckDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CLogCheckDoc)
	afx_msg void OnAppConnect();
	afx_msg void OnUpdateAppConnect(CCmdUI* pCmdUI);
	afx_msg void OnFileNew();
	afx_msg void OnAppConfig();
	afx_msg void OnUpdateAppConfig(CCmdUI* pCmdUI);
	afx_msg void OnFileOpen();
	afx_msg void OnFileSave();
	afx_msg void OnFileSaveAs();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MONETONOFFDOC_H__4D4388E0_9A8B_4158_B477_1549A3CDF61C__INCLUDED_)
