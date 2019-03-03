// TextTreeView.cpp : implementation of the CTextTreeView class
//

#include "stdafx.h"
#include "TextViewer.h"
#include "TextViewerDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTextTreeView

IMPLEMENT_DYNCREATE(CTextTreeView, CTreeView)

BEGIN_MESSAGE_MAP(CTextTreeView, CTreeView)
	//{{AFX_MSG_MAP(CTextTreeView)
	ON_NOTIFY_REFLECT(TVN_ITEMEXPANDING, OnItemExpanding)
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, OnSelChanged)
	ON_NOTIFY_REFLECT(NM_DBLCLK, OnDblclk)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CTreeView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CTreeView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CTreeView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTextTreeView construction/destruction

CTextTreeView::CTextTreeView()
{
	// TODO: add construction code here

}

CTextTreeView::~CTextTreeView()
{
}

BOOL CTextTreeView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	cs.style |=TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | TVS_SHOWSELALWAYS;	

	return CTreeView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CTextTreeView drawing

void CTextTreeView::OnDraw(CDC* pDC)
{
	CTextViewerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}

void CTextTreeView::OnInitialUpdate()
{
	CTreeView::OnInitialUpdate();

	//產生樹狀目錄的圖示
	m_ilFolderImage.Create(IDB_TREEICON, 16, 1, RGB(255, 255, 255));
	GetTreeCtrl().SetImageList(&m_ilFolderImage, TVSIL_NORMAL);

	//取得程式前次瀏覽的資料夾路徑
	CTextViewerDoc* doc = (CTextViewerDoc*)GetDocument();
	m_ExpandDirectory = doc->m_ViewDirectory;

	//開始掃描電腦
	Rescan();
	//展開至目前資料夾之路徑
	ExpandCurrentFolder();
}

/////////////////////////////////////////////////////////////////////////////
// CTextTreeView printing

BOOL CTextTreeView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CTextTreeView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CTextTreeView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CTextTreeView diagnostics

#ifdef _DEBUG
void CTextTreeView::AssertValid() const
{
	CTreeView::AssertValid();
}

void CTextTreeView::Dump(CDumpContext& dc) const
{
	CTreeView::Dump(dc);
}

CTextViewerDoc* CTextTreeView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTextViewerDoc)));
	return (CTextViewerDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTextTreeView message handlers

//Event->展開資料夾
void CTextTreeView::OnItemExpanding(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;

	CString FullPath;
	int pic1, pic2;
	//The selected item 
	HTREEITEM hItem = pNMTreeView->itemNew.hItem;
	//Get the child item's Image
	HTREEITEM hChildItem = GetTreeCtrl().GetChildItem(hItem);
	if( !GetTreeCtrl().GetItemImage(hChildItem, pic1, pic2) )
		return;
	if(pic1 == 0)
	{
		if( !GetTreeCtrl().DeleteItem(hChildItem) )
			return;
		if( !GetPath(FullPath, hItem) )
			return;
		ScanPath((char *)(LPCTSTR)FullPath, hItem, FALSE);
	}

	*pResult = 0;
}

//Event->選取到資料夾
void CTextTreeView::OnSelChanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;

	CString FullPath;
	if( !GetPath(FullPath, GetTreeCtrl().GetSelectedItem()) )
		return;	

	//更新檔案清單列表
	CTextViewerDoc* doc = (CTextViewerDoc*)GetDocument();
	doc->SetNewDirectory(FullPath);

	*pResult = 0;
}

//Event->雙擊資料夾
void CTextTreeView::OnDblclk(NMHDR* pNMHDR, LRESULT* pResult) 
{
	//在狀態列上顯示目前目錄名稱
	CTextViewerDoc* doc = (CTextViewerDoc*)GetDocument();
	doc->SetNewDirectory( "" );

	*pResult = 0;
}

/////////////////////////////////////////////////////////////////////////////
// 處理樹狀資料夾的函式

//[1] 開始掃描電腦
void CTextTreeView::Rescan()
{
	m_dwTotalDrives  = 0;
	m_dwCurrentDrive = 2;
	m_dwDriveMask    = ::GetLogicalDrives(); //Get all availiable drive letters

	//掃描出所有磁碟機
	ScanFilesystem();

	//設定目前節點為 C:
	// HTREEITEM hitem = GetTreeCtrl().GetChildItem(m_hRootItem);
	// while(GetTreeCtrl().GetItemText(hitem) != "C:\\")
	//	 hitem = GetTreeCtrl().GetNextSiblingItem(hitem);
	// GetTreeCtrl().SelectItem(m_hCurrentItem = hitem);
}

//[2] 掃描檔案系統
void CTextTreeView::ScanFilesystem()
{
	//設定 Root Node
	SetupRootItem();

	for(int i=0; i<32; i++)
	{
		if((m_dwDriveMask&(1 << i)) != 0)
		{
			ScanDrive(i);
		}
	}
}

//[3] 設定樹根節點(電腦名稱)
void CTextTreeView::SetupRootItem()
{
	char name[MAX_COMPUTERNAME_LENGTH+1];
	DWORD len = MAX_COMPUTERNAME_LENGTH+1;
	::GetComputerName(name, &len);

	GetTreeCtrl().DeleteAllItems();
	m_hRootItem = GetTreeCtrl().InsertItem(name, 0, 0, TVI_ROOT);
}

//[4] 掃描磁碟機
void CTextTreeView::ScanDrive(int drvno)
{
	char disk[_MAX_PATH];
	wsprintf(disk, "%C:\\", char(drvno+'A'));
	if(drvno < 2)
		AddPathItem(disk, m_hRootItem, TRUE);
	else if(drvno == 2)
		m_hCurrentItem = AddPathItem(disk, m_hRootItem, TRUE);
	else
		AddPathItem(disk, m_hRootItem, TRUE);
}

//[5] 加入成樹狀目錄的節點
HTREEITEM CTextTreeView::AddPathItem(char* path, HTREEITEM hParent, BOOL root)
{
	HTREEITEM hret;
	if( root )
	{
		//此為磁碟機的節點
		switch( GetDriveType(path) )
		{
		case DRIVE_REMOVABLE:
			hret = GetTreeCtrl().InsertItem(path, 1, 1, hParent);
			break;
		case DRIVE_FIXED:
			hret = GetTreeCtrl().InsertItem(path, 2, 2, hParent);
			break;
		case DRIVE_REMOTE:
			hret = GetTreeCtrl().InsertItem(path, 3, 3, hParent);
			break;
		case DRIVE_CDROM:
			hret = GetTreeCtrl().InsertItem(path, 4, 4, hParent);
			break;
		}
		//插入空白的節點
		AddNullChild( hret );
	}
	else
	{
		//此為資料夾的節點
		hret = GetTreeCtrl().InsertItem(path, 5, 6, hParent, TVI_SORT);
	}

	return hret;
}

BOOL CTextTreeView::ScanPath(char* root, HTREEITEM hParent, BOOL first)
{
	AfxGetApp()->DoWaitCursor(1);
	char child[_MAX_PATH];
	wsprintf(child, "%s*.*", root);

	BOOL ret = FALSE;
	HTREEITEM hitem;
	CFileFind finder;
	int working = finder.FindFile(child);
	while( working ) //Non-efficient codes :(
	{
		working = finder.FindNextFile();
		if( finder.IsDirectory() && (!finder.IsDots()) )
		{
			hitem = AddPathItem((char*)(LPCTSTR)finder.GetFileName(), hParent, FALSE);
			wsprintf(child, "%s%s\\", root, finder.GetFileName());
			//插入空白的節點
			AddNullChild( hitem );
		}
	}
	AfxGetApp()->DoWaitCursor(-1);
	return ret;
}

BOOL CTextTreeView::GetPath(CString &path, HTREEITEM hitem)
{
	CString parent;
	//The selected Item can't be RootItem
	if(hitem == m_hRootItem)
	{
		BOOL b = GetTreeCtrl().SelectItem(m_hCurrentItem);
		return FALSE;
	}
	m_hCurrentItem = hitem;
	//To calculate the FullPath of Item
	while(hitem != m_hRootItem)
	{
		parent = GetTreeCtrl().GetItemText(hitem);
		if(parent[parent.GetLength()-1] == '\\')
			path = parent + path;
		else
			path = parent + "\\" + path;
		hitem = GetTreeCtrl().GetParentItem(hitem);
	}
	return TRUE;
}

void CTextTreeView::AddNullChild(HTREEITEM hTreeItem)
{
	//插入一個空的子節點讓+永遠存在
	GetTreeCtrl().InsertItem("  ", 0, 0, hTreeItem);
}

/////////////////////////////////////////////////////////////////////////////
// Class Member Methods

//將樹狀目錄展開到目前路徑
void CTextTreeView::ExpandCurrentFolder()
{
	CTextViewerDoc* doc = (CTextViewerDoc*)GetDocument();
	HTREEITEM hNextItem, hChildItem;
	CString Folder1, Folder2;
	BOOL GotIt = FALSE;

	// "Disk:\\Folder1\\Folder2\\Folder3\\"
	//  ^    ^
	//  st   ed
	int pos_start = 0;
	int pos_end = m_ExpandDirectory.FindOneOf("\\");

	//1-過濾磁碟機代號
	if(pos_end >=0 )
	{
		Folder1 = m_ExpandDirectory.Mid(pos_start, pos_end-pos_start+1);
		pos_start = pos_end + 1;
		pos_end = m_ExpandDirectory.Find("\\", pos_start);

		hChildItem = GetTreeCtrl().GetChildItem(m_hRootItem);
		while(hChildItem != NULL)
		{
			Folder2 = GetTreeCtrl().GetItemText(hChildItem);
			if(Folder1.CompareNoCase(Folder2) == 0)
			{
				//展開資料夾
				GetTreeCtrl().Expand(hChildItem, TVE_EXPAND);
				GotIt = TRUE;
				break;
			}
			hNextItem = GetTreeCtrl().GetNextSiblingItem(hChildItem);
			hChildItem = hNextItem;
		}
	}

	//2-過濾個別資料夾
	while(pos_end >= 0)
	{
		GotIt = FALSE;
		Folder1 = m_ExpandDirectory.Mid(pos_start, pos_end-pos_start);
		pos_start = pos_end + 1;
		pos_end = m_ExpandDirectory.Find("\\", pos_start);

		hChildItem = GetTreeCtrl().GetChildItem(hChildItem);
		while(hChildItem != NULL)
		{
			Folder2 = GetTreeCtrl().GetItemText(hChildItem);
			if(Folder1.CompareNoCase(Folder2) == 0)
			{
				//展開資料夾
				GetTreeCtrl().Expand(hChildItem, TVE_EXPAND);
				GotIt = TRUE;
				break;
			}
			hNextItem = GetTreeCtrl().GetNextSiblingItem(hChildItem);
			hChildItem = hNextItem;
		}
	}

	//3-打開末尾最後的資料夾
	if( GotIt )
	{
		//設定並選取目前節點的資料夾
		GetTreeCtrl().SelectItem( m_hCurrentItem = hChildItem );
	}
	else
	{
		//設定目前節點為 C:
		HTREEITEM hitem = GetTreeCtrl().GetChildItem(m_hRootItem);
		while(GetTreeCtrl().GetItemText(hitem) != "C:\\")
			hitem = GetTreeCtrl().GetNextSiblingItem(hitem);
		GetTreeCtrl().SelectItem(m_hCurrentItem = hitem);
	}
}

//回到上層的資料夾
void CTextTreeView::BackToUpperFolder()
{
	//取得目前選取的資料夾
	HTREEITEM hCurrentItem = GetTreeCtrl().GetSelectedItem();
	//取得其上層的資料夾
	HTREEITEM hParentItem = GetTreeCtrl().GetParentItem( hCurrentItem );
	//回到上層目錄
	GetTreeCtrl().SelectItem( hParentItem );
}
