// TextTreeView.h : interface of the CTextTreeView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_TEXTTREEVIEW_H__F4A765F5_9D69_4556_B016_85136907CB89__INCLUDED_)
#define AFX_TEXTTREEVIEW_H__F4A765F5_9D69_4556_B016_85136907CB89__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CTextViewerDoc;

class CTextTreeView : public CTreeView
{
protected: // create from serialization only
	CTextTreeView();
	DECLARE_DYNCREATE(CTextTreeView)

	CImageList m_ilFolderImage;
	DWORD m_dwDriveMask, m_dwTotalDrives, m_dwCurrentDrive;
	HTREEITEM m_hRootItem;    //�̤W�h��Item
	HTREEITEM m_hCurrentItem; //�ثe��Item

// Attributes
public:
	CTextViewerDoc* GetDocument();
	CString m_ExpandDirectory;

// Operations
public:
	HTREEITEM AddPathItem(char* path, HTREEITEM hParent, BOOL root); //�[�JItem
	BOOL ScanPath(char * root, HTREEITEM hParent, BOOL first=FALSE); //��ؿ��[�J
	BOOL GetPath(CString & path, HTREEITEM hitem);	//���o���wItem���ؿ�
	void ScanDrive(int drvno);						//���y�Ҧ��Ϻ�
	void SetupRootItem();							//�]�w�̤W�h�ؿ�
	void ScanFilesystem();							//���y�Ϻ�
	void Rescan();									//���y�Ϻ�
	void AddNullChild(HTREEITEM hTreeItem);			//���J�@�Ӫťժ��`�I
	void ExpandCurrentFolder();
	void BackToUpperFolder();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTextTreeView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTextTreeView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CTextTreeView)
	afx_msg void OnItemExpanding(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelChanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclk(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in TextTreeView.cpp
inline CTextViewerDoc* CTextTreeView::GetDocument()
   { return (CTextViewerDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TEXTTREEVIEW_H__F4A765F5_9D69_4556_B016_85136907CB89__INCLUDED_)
