// MySIMTreeView.cpp : implementation of the CMySIMTreeView class
//

#include "stdafx.h"
#include "MySIM.h"
#include "MySIMDoc.h"
#include "MySIMTreeView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMySIMTreeView

IMPLEMENT_DYNCREATE(CMySIMTreeView, CTreeView)

BEGIN_MESSAGE_MAP(CMySIMTreeView, CTreeView)
	//{{AFX_MSG_MAP(CMySIMTreeView)
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, OnSelchanged)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CTreeView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CTreeView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CTreeView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMySIMTreeView construction/destruction

CMySIMTreeView::CMySIMTreeView()
{
	// TODO: add construction code here

}

CMySIMTreeView::~CMySIMTreeView()
{
}

BOOL CMySIMTreeView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CTreeView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CMySIMTreeView drawing

void CMySIMTreeView::OnDraw(CDC* pDC)
{
	CMySIMDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}

void CMySIMTreeView::OnInitialUpdate()
{
	CTreeView::OnInitialUpdate();


	// TODO: You may populate your TreeView with items by directly accessing
	//  its tree control through a call to GetTreeCtrl().

	//設定本視窗的樣式
	GetTreeCtrl().ModifyStyle(0, TVS_HASLINES|TVS_LINESATROOT|TVS_HASBUTTONS);
	CTreeView::OnInitialUpdate();

	//產生目錄的圖示
	m_ilFolderImage.Create(IDB_TREEICON, 16, 1, RGB(255, 255, 255));
	GetTreeCtrl().SetImageList(&m_ilFolderImage, TVSIL_NORMAL);
}

/////////////////////////////////////////////////////////////////////////////
// CMySIMTreeView printing

BOOL CMySIMTreeView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CMySIMTreeView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CMySIMTreeView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CMySIMTreeView diagnostics

#ifdef _DEBUG
void CMySIMTreeView::AssertValid() const
{
	CTreeView::AssertValid();
}

void CMySIMTreeView::Dump(CDumpContext& dc) const
{
	CTreeView::Dump(dc);
}

CMySIMDoc* CMySIMTreeView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMySIMDoc)));
	return (CMySIMDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMySIMTreeView message handlers

void CMySIMTreeView::OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here

	//儲存選取的分類名稱
	m_hCurrentItem = GetTreeCtrl().GetSelectedItem();

	//Begin: 更新 Secret View 的列表
	CMySIMDoc* doc = (CMySIMDoc*)GetDocument();
	if ( doc )
	{
		doc->DumpEfData( m_hCurrentItem );
	}

	*pResult = 0;
}


/////////////////////////////////////////////////////////////////////////////
// CMySIMTreeView member methods

void CMySIMTreeView::ExpandRootTree()
{
	//展開資料夾
	GetTreeCtrl().Expand(m_hRootItem, TVE_EXPAND);

	//選取資料夾
	GetTreeCtrl().SelectItem( m_hRootItem );
}

HTREEITEM CMySIMTreeView::SetupRootItem(CString ItemName, BOOL success)
{
	GetTreeCtrl().DeleteAllItems();

	//產生 Root Node
	CMySIMDoc* doc = (CMySIMDoc*)GetDocument();

	int image1 = (( success ) ? 0 : 4);
	int image2 = (( success ) ? 0 : 4);
	m_hRootItem = GetTreeCtrl().InsertItem(ItemName, image1, image2, TVI_ROOT);

	return m_hRootItem;
}

HTREEITEM CMySIMTreeView::AddDirectoryItem(CString ItemName, HTREEITEM parent, BOOL success)
{
	int image1 = (( success ) ? 2 : 6);
	int image2 = (( success ) ? 3 : 7);
	HTREEITEM child = GetTreeCtrl().InsertItem(ItemName, image1, image2, parent);

	return child;
}

HTREEITEM CMySIMTreeView::AddFileItem(CString ItemName, HTREEITEM parent, BOOL success)
{
	int image1 = (( success ) ? 1 : 5);
	int image2 = (( success ) ? 1 : 5);
	HTREEITEM child = GetTreeCtrl().InsertItem(ItemName, image1, image2, parent);

	return child;
}
