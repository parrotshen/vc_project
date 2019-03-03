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

	//���;𪬥ؿ����ϥ�
	m_ilFolderImage.Create(IDB_TREEICON, 16, 1, RGB(255, 255, 255));
	GetTreeCtrl().SetImageList(&m_ilFolderImage, TVSIL_NORMAL);

	//���o�{���e���s������Ƨ����|
	CTextViewerDoc* doc = (CTextViewerDoc*)GetDocument();
	m_ExpandDirectory = doc->m_ViewDirectory;

	//�}�l���y�q��
	Rescan();
	//�i�}�ܥثe��Ƨ������|
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

//Event->�i�}��Ƨ�
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

//Event->������Ƨ�
void CTextTreeView::OnSelChanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;

	CString FullPath;
	if( !GetPath(FullPath, GetTreeCtrl().GetSelectedItem()) )
		return;	

	//��s�ɮײM��C��
	CTextViewerDoc* doc = (CTextViewerDoc*)GetDocument();
	doc->SetNewDirectory(FullPath);

	*pResult = 0;
}

//Event->������Ƨ�
void CTextTreeView::OnDblclk(NMHDR* pNMHDR, LRESULT* pResult) 
{
	//�b���A�C�W��ܥثe�ؿ��W��
	CTextViewerDoc* doc = (CTextViewerDoc*)GetDocument();
	doc->SetNewDirectory( "" );

	*pResult = 0;
}

/////////////////////////////////////////////////////////////////////////////
// �B�z�𪬸�Ƨ����禡

//[1] �}�l���y�q��
void CTextTreeView::Rescan()
{
	m_dwTotalDrives  = 0;
	m_dwCurrentDrive = 2;
	m_dwDriveMask    = ::GetLogicalDrives(); //Get all availiable drive letters

	//���y�X�Ҧ��Ϻо�
	ScanFilesystem();

	//�]�w�ثe�`�I�� C:
	// HTREEITEM hitem = GetTreeCtrl().GetChildItem(m_hRootItem);
	// while(GetTreeCtrl().GetItemText(hitem) != "C:\\")
	//	 hitem = GetTreeCtrl().GetNextSiblingItem(hitem);
	// GetTreeCtrl().SelectItem(m_hCurrentItem = hitem);
}

//[2] ���y�ɮרt��
void CTextTreeView::ScanFilesystem()
{
	//�]�w Root Node
	SetupRootItem();

	for(int i=0; i<32; i++)
	{
		if((m_dwDriveMask&(1 << i)) != 0)
		{
			ScanDrive(i);
		}
	}
}

//[3] �]�w��ڸ`�I(�q���W��)
void CTextTreeView::SetupRootItem()
{
	char name[MAX_COMPUTERNAME_LENGTH+1];
	DWORD len = MAX_COMPUTERNAME_LENGTH+1;
	::GetComputerName(name, &len);

	GetTreeCtrl().DeleteAllItems();
	m_hRootItem = GetTreeCtrl().InsertItem(name, 0, 0, TVI_ROOT);
}

//[4] ���y�Ϻо�
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

//[5] �[�J���𪬥ؿ����`�I
HTREEITEM CTextTreeView::AddPathItem(char* path, HTREEITEM hParent, BOOL root)
{
	HTREEITEM hret;
	if( root )
	{
		//�����Ϻо����`�I
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
		//���J�ťժ��`�I
		AddNullChild( hret );
	}
	else
	{
		//������Ƨ����`�I
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
			//���J�ťժ��`�I
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
	//���J�@�ӪŪ��l�`�I��+�û��s�b
	GetTreeCtrl().InsertItem("  ", 0, 0, hTreeItem);
}

/////////////////////////////////////////////////////////////////////////////
// Class Member Methods

//�N�𪬥ؿ��i�}��ثe���|
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

	//1-�L�o�Ϻо��N��
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
				//�i�}��Ƨ�
				GetTreeCtrl().Expand(hChildItem, TVE_EXPAND);
				GotIt = TRUE;
				break;
			}
			hNextItem = GetTreeCtrl().GetNextSiblingItem(hChildItem);
			hChildItem = hNextItem;
		}
	}

	//2-�L�o�ӧO��Ƨ�
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
				//�i�}��Ƨ�
				GetTreeCtrl().Expand(hChildItem, TVE_EXPAND);
				GotIt = TRUE;
				break;
			}
			hNextItem = GetTreeCtrl().GetNextSiblingItem(hChildItem);
			hChildItem = hNextItem;
		}
	}

	//3-���}�����̫᪺��Ƨ�
	if( GotIt )
	{
		//�]�w�ÿ���ثe�`�I����Ƨ�
		GetTreeCtrl().SelectItem( m_hCurrentItem = hChildItem );
	}
	else
	{
		//�]�w�ثe�`�I�� C:
		HTREEITEM hitem = GetTreeCtrl().GetChildItem(m_hRootItem);
		while(GetTreeCtrl().GetItemText(hitem) != "C:\\")
			hitem = GetTreeCtrl().GetNextSiblingItem(hitem);
		GetTreeCtrl().SelectItem(m_hCurrentItem = hitem);
	}
}

//�^��W�h����Ƨ�
void CTextTreeView::BackToUpperFolder()
{
	//���o�ثe�������Ƨ�
	HTREEITEM hCurrentItem = GetTreeCtrl().GetSelectedItem();
	//���o��W�h����Ƨ�
	HTREEITEM hParentItem = GetTreeCtrl().GetParentItem( hCurrentItem );
	//�^��W�h�ؿ�
	GetTreeCtrl().SelectItem( hParentItem );
}
