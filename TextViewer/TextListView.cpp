// TextListView.cpp : implementation file
//

#include "stdafx.h"
#include "MainFrm.h"
#include "TextViewer.h"
#include "TextViewerDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTextListView

IMPLEMENT_DYNCREATE(CTextListView, CListView)

CTextListView::CTextListView()
{
}

CTextListView::~CTextListView()
{
}


BEGIN_MESSAGE_MAP(CTextListView, CListView)
	//{{AFX_MSG_MAP(CTextListView)
	ON_WM_KEYUP()
	ON_WM_LBUTTONDBLCLK()
	ON_NOTIFY_REFLECT(LVN_COLUMNCLICK, OnColumnclick)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTextListView drawing

void CTextListView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CTextListView diagnostics

#ifdef _DEBUG
void CTextListView::AssertValid() const
{
	CListView::AssertValid();
}

void CTextListView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTextListView message handlers

//設定 List 控制項為報表模式(Report)
BOOL CTextListView::PreCreateWindow(CREATESTRUCT& cs) 
{
	CListView::PreCreateWindow(cs);
	cs.style |= LVS_REPORT | LVS_SHOWSELALWAYS | LVS_SINGLESEL | WS_VISIBLE;

	return TRUE;
}

//初始化 List 控制項的標題
void CTextListView::OnInitialUpdate() 
{
	CListView::OnInitialUpdate();

	//建立標題欄位(Column)
	LV_COLUMN col;
	col.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
	col.fmt = LVCFMT_LEFT;
	col.iSubItem = 0;
	col.pszText = "檔案名稱";
	col.cx = 180;
	GetListCtrl().InsertColumn(0, &col);

	col.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
	col.fmt = LVCFMT_RIGHT;
	col.iSubItem = 1;
	col.pszText = "大小";
	col.cx = 80;
	GetListCtrl().InsertColumn(1, &col);

	col.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
	col.fmt = LVCFMT_LEFT;
	col.iSubItem = 2;
	col.pszText = "修改日期";
	col.cx = 110;
	GetListCtrl().InsertColumn(2, &col);

	col.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
	col.fmt = LVCFMT_CENTER;
	col.iSubItem = 3;
	col.pszText = "屬性";
	col.cx = 60;
	GetListCtrl().InsertColumn(3, &col);

	//使 List 控制項能顯示整列選取
	GetListCtrl().SetExtendedStyle( GetListCtrl().GetExtendedStyle() | LVS_EX_FULLROWSELECT );

	//Begin - 設定 List 控制項的圖示
	GetListCtrl().SetImageList(&((CMainFrame*)GetParentFrame())->m_smallImageList, LVSIL_SMALL); 
	GetListCtrl().SetImageList(&((CMainFrame*)GetParentFrame())->m_largeImageList, LVSIL_NORMAL); 
	//End   - 設定 List 控制項的圖示

	//Begin - 列出目前路徑的檔案
	CTextViewerDoc* doc = (CTextViewerDoc*)GetDocument();
	m_ScanPath   = doc->m_ViewDirectory;
	m_ScanFilter = "*.*";
	ReadDirectoryInfo();
	//End   - 列出目前路徑的檔案
}

void CTextListView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	CTextViewerDoc* doc = (CTextViewerDoc*)GetDocument();
	if( doc )
	{
		int idx = GetListCtrl().GetSelectionMark();
		CString sFileName = GetListCtrl().GetItemText(idx, 0);

		//EditView 顯示檔案的內容
		if( doc->DisplayFile(m_ScanPath+sFileName) )
		{
			//重設檔案路徑
			doc->m_FileName = m_ScanPath + sFileName;

			//顯示檔名於視窗標題
			CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
			pMainFrame->SetWindowText(sFileName + " - TextViewer");
		}
	}

	CListView::OnKeyUp(nChar, nRepCnt, nFlags);
}

void CTextListView::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	CTextViewerDoc* doc = (CTextViewerDoc*)GetDocument();
	if( doc )
	{
		int idx = GetListCtrl().GetSelectionMark();
		CString sFileName = GetListCtrl().GetItemText(idx, 0);

		//EditView 顯示檔案的內容
		if( doc->DisplayFile(m_ScanPath+sFileName) )
		{
			//重設檔案路徑
			doc->m_FileName = m_ScanPath + sFileName;

			//顯示檔名於視窗標題
			CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
			pMainFrame->SetWindowText(sFileName + " - TextViewer");
		}
	}

	CListView::OnLButtonDblClk(nFlags, point);
}

//判斷以那一欄位來排序
void CTextListView::OnColumnclick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	//依照點選的欄位進行排序(Filename & Date Only)
	if(pNMListView->iSubItem == 0 || pNMListView->iSubItem == 2)
		AscendingSortBy( pNMListView->iSubItem );

	*pResult = 0;
}

/////////////////////////////////////////////////////////////////////////////
// Class Member Methods

//讀取某個目錄內的所有檔案清單
void CTextListView::ReadDirectoryInfo()
{
	//清除目前所有顯示的項目(Row)
	GetListCtrl().DeleteAllItems();

	_finddata_t fblk;
	long handle, result;
	char child[_MAX_PATH];

	//用 findfirst, findnext 取得檔案清單
	wsprintf(child, "%s%s", m_ScanPath, m_ScanFilter);
	result = handle = _findfirst(child, &fblk);

	while(result != -1)
	{
		//如果它不是目錄
		if( ( ( fblk.attrib & _A_SUBDIR ) == 0 ) && strcmp ( fblk.name, "." ) && strcmp ( fblk.name, ".." ) )
		{
			//把抓到的檔案資料加進 List Control 中
			AddFileItem( fblk );
		}
		result = _findnext(handle, &fblk);
	}	
	_findclose( handle );
}

//把讀到的檔案加到 CListView 清單中
void CTextListView::AddFileItem(_finddata_t& fblk)
{	
	int itemno;
	char drive[_MAX_DRIVE];
	char dir[_MAX_DIR];
	char fname[_MAX_FNAME];
	char ext[_MAX_EXT];
	struct tm *ptime;
	LV_ITEM	lvitem;
	
	//切割完整路徑名稱
	_splitpath(fblk.name, drive, dir, fname, ext);
	//strlwr( ext );

	//取得檔案的完整路徑名稱
	CString sFullName = m_ScanPath + fblk.name;

	//[1] 插入檔案名稱至清單(以檔名作遞增排序)
	CString testname1;
	CString testname2 = fblk.name;
	testname2.MakeLower();
	for(itemno=0; itemno<GetListCtrl().GetItemCount(); itemno++)
	{
		testname1 = GetListCtrl().GetItemText(itemno, 0);
		testname1.MakeLower();
		if(testname2 < testname1)
			break;
	}
	GetListCtrl().InsertItem(itemno, fblk.name, GetIconIndex(sFullName));

	//構成每個欄位所要顯示的資料
	lvitem.mask    = LVIF_TEXT;
	lvitem.iItem   = itemno;
	lvitem.pszText = fname;

	//[2] 插入檔案大小至清單
	wsprintf(fname, "%d", fblk.size);
	lvitem.iSubItem = 1;
	GetListCtrl().SetItem( &lvitem );

	//[3] 插入檔案修改日期至清單
	ptime = localtime(&fblk.time_write);
	wsprintf(fname, "%d/%02d/%02d %02d:%02d", ptime->tm_year+1900, ptime->tm_mon+1, ptime->tm_mday, ptime->tm_hour, ptime->tm_min);
	lvitem.iSubItem = 2;
	GetListCtrl().SetItem( &lvitem );

	//[4] 插入檔案屬性至清單
	wsprintf(fname, "----");
	if((fblk.attrib & _A_ARCH) != 0)   fname[0] = 'A';
	if((fblk.attrib & _A_HIDDEN) != 0) fname[1] = 'H';
	if((fblk.attrib & _A_RDONLY) != 0) fname[2] = 'R';
	if((fblk.attrib & _A_SYSTEM) != 0) fname[3] = 'S';
	lvitem.iSubItem = 3;
	GetListCtrl().SetItem( &lvitem );
}

//取得圖示的索引值
int CTextListView::GetIconIndex(const CString &csFileName)
{
	SHFILEINFO sfi;
	SHGetFileInfo((LPCTSTR)csFileName, 0, &sfi, sizeof(SHFILEINFO), SHGFI_SYSICONINDEX | SHGFI_SMALLICON );
	return sfi.iIcon;
}

//將 List Item Mark 向下移動
void CTextListView::MoveItemMarkDown()
{
	int idx = GetListCtrl().GetSelectionMark();
	if(idx != (GetListCtrl().GetItemCount()-1))
	{
		idx++;
		GetListCtrl().SetSelectionMark( idx );
		GetListCtrl().SetItemState(idx, LVIS_SELECTED, LVIS_SELECTED);
		SendMessage( WM_KEYUP );
	}
}

//將 List Item Mark 向上移動
void CTextListView::MoveItemMarkUpper()
{
	int idx = GetListCtrl().GetSelectionMark();
	if(idx != 0)
	{
		idx--;
		GetListCtrl().SetSelectionMark( idx );
		GetListCtrl().SetItemState(idx, LVIS_SELECTED, LVIS_SELECTED);
		SendMessage( WM_KEYUP );
	}
}

//遞增排序演算法(Bubble Sort)
void CTextListView::AscendingSortBy(int column)
{
	int i, j;
	BOOL flag;
	CString swap0, swap1, swap2, swap3;
	CString sFullName;

	for(i=0; i<GetListCtrl().GetItemCount(); i++)
	{
		flag = FALSE;
		for(j=0; j<GetListCtrl().GetItemCount()-1; j++)
		{
			if(GetListCtrl().GetItemText(j, column) > GetListCtrl().GetItemText(j+1, column))
			{
				//Begin - 交換兩列的資料
				swap0 = GetListCtrl().GetItemText(j, 0); //Filename
				swap1 = GetListCtrl().GetItemText(j, 1); //Size
				swap2 = GetListCtrl().GetItemText(j, 2); //Date
				swap3 = GetListCtrl().GetItemText(j, 3); //Attribute

				sFullName = m_ScanPath + GetListCtrl().GetItemText(j+1, 0);
				GetListCtrl().SetItem(j, 0, LVIF_TEXT|LVIF_IMAGE, GetListCtrl().GetItemText(j+1, 0), GetIconIndex(sFullName), 0, 0, 0);
				GetListCtrl().SetItemText(j, 1, GetListCtrl().GetItemText(j+1, 1));
				GetListCtrl().SetItemText(j, 2, GetListCtrl().GetItemText(j+1, 2));
				GetListCtrl().SetItemText(j, 3, GetListCtrl().GetItemText(j+1, 3));

				sFullName = m_ScanPath + swap0;
				GetListCtrl().SetItem(j+1, 0, LVIF_TEXT|LVIF_IMAGE, swap0, GetIconIndex(sFullName), 0, 0, 0);
				GetListCtrl().SetItemText(j+1, 1, swap1);
				GetListCtrl().SetItemText(j+1, 2, swap2);
				GetListCtrl().SetItemText(j+1, 3, swap3);
				//End   - 交換兩列的資料

				flag = TRUE;
			}
		}

		if( !flag ) break;
	}
}
