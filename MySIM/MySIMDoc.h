// MySIMDoc.h : interface of the CMySIMDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYSIMDOC_H__03A2C35B_C0B7_4340_BB3E_D72DC2FB5C12__INCLUDED_)
#define AFX_MYSIMDOC_H__03A2C35B_C0B7_4340_BB3E_D72DC2FB5C12__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Winscard.h"
#include "SimFileSys.h"

class CMainFrame;
class CMySIMTreeView;
class CMySIMListView;
class CMySIMEditView;

class CMySIMDoc : public CDocument
{
protected: // create from serialization only
	CMySIMDoc();
	DECLARE_DYNCREATE(CMySIMDoc)

// Attributes
public:
	CMainFrame     *m_pMainFrame;
	CMySIMTreeView *m_pTreeView;
	CMySIMListView *m_pListView;
	CMySIMEditView *m_pEditView;

	BOOL            m_isConnected;
	BOOL            m_isShowFileTree;

// Operations
public:
	BOOL ConnectSIM();
	void DisconnectSIM();
	void ScanFileSystem();
	int  SendCommandAPDU(BYTE *cmdApdu, int cmdApduLen, BYTE *rspApdu, int rspApduLen);
	BOOL DoSelect(BYTE *fid, BYTE *rspApdu, int *rspApduLen);
	int  DoGetResponse(BYTE *rspApdu, int rspApduLenMax);
	int  DoReadBinary(BYTE *rspApdu, int rspApduLenMax);
	int  DoReadRecord(BYTE recNum, BYTE *rspApdu, int rspApduLenMax);
	void ShowErrorCode(LONG code);
	void PrintFileInfo(HTREEITEM hFile);
	void DumpEfData(HTREEITEM hFile);

private:
	tFileInfo *ReadFile(BYTE *rspApdu, int rspApduLen);
	tSimFileSys *FindFile(HTREEITEM hFile);
	UINT PrepareByteData(const char *inData, BYTE *outData, UINT outLen);;
	char* GetToken(char *token, char *line);
	char* ReadLine(char *line, char *text);
	BYTE Hex2Dec(char c);
	BYTE ConvertHex(char *hex);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMySIMDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	protected:
	virtual BOOL SaveModified();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMySIMDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CMySIMDoc)
	afx_msg void OnToolScard();
	afx_msg void OnUpdateToolScard(CCmdUI* pCmdUI);
	afx_msg void OnToolCommand();
	afx_msg void OnUpdateToolCommand(CCmdUI* pCmdUI);
	afx_msg void OnToolClean();
	afx_msg void OnToolTree();
	afx_msg void OnUpdateToolTree(CCmdUI* pCmdUI);
	afx_msg void OnToolPin();
	afx_msg void OnUpdateToolPin(CCmdUI* pCmdUI);
	afx_msg void OnToolParam();
	afx_msg void OnUpdateToolParam(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYSIMDOC_H__03A2C35B_C0B7_4340_BB3E_D72DC2FB5C12__INCLUDED_)
