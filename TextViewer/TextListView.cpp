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

//�]�w List ���������Ҧ�(Report)
BOOL CTextListView::PreCreateWindow(CREATESTRUCT& cs) 
{
	CListView::PreCreateWindow(cs);
	cs.style |= LVS_REPORT | LVS_SHOWSELALWAYS | LVS_SINGLESEL | WS_VISIBLE;

	return TRUE;
}

//��l�� List ��������D
void CTextListView::OnInitialUpdate() 
{
	CListView::OnInitialUpdate();

	//�إ߼��D���(Column)
	LV_COLUMN col;
	col.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
	col.fmt = LVCFMT_LEFT;
	col.iSubItem = 0;
	col.pszText = "�ɮצW��";
	col.cx = 180;
	GetListCtrl().InsertColumn(0, &col);

	col.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
	col.fmt = LVCFMT_RIGHT;
	col.iSubItem = 1;
	col.pszText = "�j�p";
	col.cx = 80;
	GetListCtrl().InsertColumn(1, &col);

	col.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
	col.fmt = LVCFMT_LEFT;
	col.iSubItem = 2;
	col.pszText = "�ק���";
	col.cx = 110;
	GetListCtrl().InsertColumn(2, &col);

	col.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
	col.fmt = LVCFMT_CENTER;
	col.iSubItem = 3;
	col.pszText = "�ݩ�";
	col.cx = 60;
	GetListCtrl().InsertColumn(3, &col);

	//�� List �������ܾ�C���
	GetListCtrl().SetExtendedStyle( GetListCtrl().GetExtendedStyle() | LVS_EX_FULLROWSELECT );

	//Begin - �]�w List ������ϥ�
	GetListCtrl().SetImageList(&((CMainFrame*)GetParentFrame())->m_smallImageList, LVSIL_SMALL); 
	GetListCtrl().SetImageList(&((CMainFrame*)GetParentFrame())->m_largeImageList, LVSIL_NORMAL); 
	//End   - �]�w List ������ϥ�

	//Begin - �C�X�ثe���|���ɮ�
	CTextViewerDoc* doc = (CTextViewerDoc*)GetDocument();
	m_ScanPath   = doc->m_ViewDirectory;
	m_ScanFilter = "*.*";
	ReadDirectoryInfo();
	//End   - �C�X�ثe���|���ɮ�
}

void CTextListView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	CTextViewerDoc* doc = (CTextViewerDoc*)GetDocument();
	if( doc )
	{
		int idx = GetListCtrl().GetSelectionMark();
		CString sFileName = GetListCtrl().GetItemText(idx, 0);

		//EditView ����ɮת����e
		if( doc->DisplayFile(m_ScanPath+sFileName) )
		{
			//���]�ɮ׸��|
			doc->m_FileName = m_ScanPath + sFileName;

			//����ɦW��������D
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

		//EditView ����ɮת����e
		if( doc->DisplayFile(m_ScanPath+sFileName) )
		{
			//���]�ɮ׸��|
			doc->m_FileName = m_ScanPath + sFileName;

			//����ɦW��������D
			CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
			pMainFrame->SetWindowText(sFileName + " - TextViewer");
		}
	}

	CListView::OnLButtonDblClk(nFlags, point);
}

//�P�_�H���@���ӱƧ�
void CTextListView::OnColumnclick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	//�̷��I�諸���i��Ƨ�(Filename & Date Only)
	if(pNMListView->iSubItem == 0 || pNMListView->iSubItem == 2)
		AscendingSortBy( pNMListView->iSubItem );

	*pResult = 0;
}

/////////////////////////////////////////////////////////////////////////////
// Class Member Methods

//Ū���Y�ӥؿ������Ҧ��ɮײM��
void CTextListView::ReadDirectoryInfo()
{
	//�M���ثe�Ҧ���ܪ�����(Row)
	GetListCtrl().DeleteAllItems();

	_finddata_t fblk;
	long handle, result;
	char child[_MAX_PATH];

	//�� findfirst, findnext ���o�ɮײM��
	wsprintf(child, "%s%s", m_ScanPath, m_ScanFilter);
	result = handle = _findfirst(child, &fblk);

	while(result != -1)
	{
		//�p�G�����O�ؿ�
		if( ( ( fblk.attrib & _A_SUBDIR ) == 0 ) && strcmp ( fblk.name, "." ) && strcmp ( fblk.name, ".." ) )
		{
			//���쪺�ɮ׸�ƥ[�i List Control ��
			AddFileItem( fblk );
		}
		result = _findnext(handle, &fblk);
	}	
	_findclose( handle );
}

//��Ū�쪺�ɮץ[�� CListView �M�椤
void CTextListView::AddFileItem(_finddata_t& fblk)
{	
	int itemno;
	char drive[_MAX_DRIVE];
	char dir[_MAX_DIR];
	char fname[_MAX_FNAME];
	char ext[_MAX_EXT];
	struct tm *ptime;
	LV_ITEM	lvitem;
	
	//���Χ�����|�W��
	_splitpath(fblk.name, drive, dir, fname, ext);
	//strlwr( ext );

	//���o�ɮת�������|�W��
	CString sFullName = m_ScanPath + fblk.name;

	//[1] ���J�ɮצW�٦ܲM��(�H�ɦW�@���W�Ƨ�)
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

	//�c���C�����ҭn��ܪ����
	lvitem.mask    = LVIF_TEXT;
	lvitem.iItem   = itemno;
	lvitem.pszText = fname;

	//[2] ���J�ɮפj�p�ܲM��
	wsprintf(fname, "%d", fblk.size);
	lvitem.iSubItem = 1;
	GetListCtrl().SetItem( &lvitem );

	//[3] ���J�ɮ׭ק����ܲM��
	ptime = localtime(&fblk.time_write);
	wsprintf(fname, "%d/%02d/%02d %02d:%02d", ptime->tm_year+1900, ptime->tm_mon+1, ptime->tm_mday, ptime->tm_hour, ptime->tm_min);
	lvitem.iSubItem = 2;
	GetListCtrl().SetItem( &lvitem );

	//[4] ���J�ɮ��ݩʦܲM��
	wsprintf(fname, "----");
	if((fblk.attrib & _A_ARCH) != 0)   fname[0] = 'A';
	if((fblk.attrib & _A_HIDDEN) != 0) fname[1] = 'H';
	if((fblk.attrib & _A_RDONLY) != 0) fname[2] = 'R';
	if((fblk.attrib & _A_SYSTEM) != 0) fname[3] = 'S';
	lvitem.iSubItem = 3;
	GetListCtrl().SetItem( &lvitem );
}

//���o�ϥܪ����ޭ�
int CTextListView::GetIconIndex(const CString &csFileName)
{
	SHFILEINFO sfi;
	SHGetFileInfo((LPCTSTR)csFileName, 0, &sfi, sizeof(SHFILEINFO), SHGFI_SYSICONINDEX | SHGFI_SMALLICON );
	return sfi.iIcon;
}

//�N List Item Mark �V�U����
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

//�N List Item Mark �V�W����
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

//���W�ƧǺt��k(Bubble Sort)
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
				//Begin - �洫��C�����
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
				//End   - �洫��C�����

				flag = TRUE;
			}
		}

		if( !flag ) break;
	}
}
