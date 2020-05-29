// BookmarkDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TextViewer.h"
#include "BookmarkDlg.h"

#include <sys/stat.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBookmarkDlg dialog


CBookmarkDlg::CBookmarkDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBookmarkDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBookmarkDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_ID = ""; // Section Name in INI File
}


void CBookmarkDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBookmarkDlg)
	DDX_Control(pDX, IDC_EDIT_NAME, m_Name);
	DDX_Control(pDX, IDC_STATIC_LINE, m_Line);
	DDX_Control(pDX, IDC_STATIC_FILE, m_File);
	DDX_Control(pDX, IDC_LIST_BOOKMARK, m_Bookmark);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBookmarkDlg, CDialog)
	//{{AFX_MSG_MAP(CBookmarkDlg)
	ON_BN_CLICKED(IDC_BUTTON_ADD, OnButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, OnButtonDelete)
	ON_LBN_SELCHANGE(IDC_LIST_BOOKMARK, OnSelchangeListBookmark)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBookmarkDlg message handlers

BOOL CBookmarkDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	//先判斷預設目錄是否存在
	struct _stat buf;
	int ret = _stat((LPCTSTR)m_INI_Path, &buf);
	if(ret == -1)
		return TRUE;

	//Begin - 列出 INI 檔案的節區名稱
	char pAllSection[MAX_PATH];
	int total = GetPrivateProfileSectionNames(pAllSection, MAX_PATH, m_INI_Path);

	CString SectionName = "";
	for(int i=0; i<total; i++)
	{
		if(pAllSection[i] == 0x00)
		{
			m_Bookmark.AddString(SectionName);
			SectionName = "";
		}
		else
		{
			SectionName += pAllSection[i];
		}
	}
	//End   - 列出 INI 檔案的節區名稱

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/////////////////////////////////////////////////////////////////////////////
// 子控制的事件處理函式

void CBookmarkDlg::OnSelchangeListBookmark() 
{
	//Begin - 讀出 INI 檔案的內容
	int ret;
	char pFileName[MAX_PATH], pFilePath[MAX_PATH], pFileLine[MAX_PATH];
	// DWORD GetPrivateProfileString(
	//			LPCTSTR lpAppName,         // section name
	//			LPCTSTR lpKeyName,         // key name
	//			LPCTSTR lpDefault,         // default string
	//			LPTSTR  lpReturnedString,  // destination buffer
	//			DWORD   nSize,             // size of destination buffer
	//			LPCTSTR lpFileName         // initialization file name
	//		);

	m_Bookmark.GetText(m_Bookmark.GetCurSel(), m_ID);
	ret = GetPrivateProfileString(m_ID, "File", "未命名.txt", pFileName, MAX_PATH, m_INI_Path);
	ret = GetPrivateProfileString(m_ID, "Path", "C:\\", pFilePath, MAX_PATH, m_INI_Path);
	ret = GetPrivateProfileString(m_ID, "Line", "0", pFileLine, MAX_PATH, m_INI_Path);

	CString FullFileName;
	FullFileName.Format("%s%s", pFilePath, pFileName);
	m_File.SetWindowText(FullFileName);
	m_Line.SetWindowText(pFileLine);
	//End   - 讀出 INI 檔案的內容
}

//增加書籤
void CBookmarkDlg::OnButtonAdd() 
{
	CString Section, Temp;
	m_Name.GetWindowText(Section);

	//檢查名稱是否重複
	BOOL IsExist = FALSE;
	for(int i=0; i<m_Bookmark.GetCount(); i++)
	{
		m_Bookmark.GetText(i, Temp);
		if(Section.CompareNoCase(Temp) == 0)
		{
			IsExist = TRUE;
			break;
		}
	}
	if( IsExist )
		m_Bookmark.SetCurSel(i);
	else
		m_Bookmark.AddString(Section);

	//加入一筆 INI 檔案內容
	Temp.Format("%d", m_MarkLine);
	// BOOL WritePrivateProfileString(
	//			LPCTSTR lpAppName,  // section name
	//			LPCTSTR lpKeyName,  // key name
	//			LPCTSTR lpString,   // string to add
	//			LPCTSTR lpFileName  // initialization file
	//		);
	WritePrivateProfileString(Section, "File", m_FileInfo_Name, m_INI_Path);
	WritePrivateProfileString(Section, "Path", m_FileInfo_Path, m_INI_Path);
	WritePrivateProfileString(Section, "Line", Temp, m_INI_Path);
}

//刪除書籤
void CBookmarkDlg::OnButtonDelete() 
{
	//先檢查是否有選取到項目
	if(m_ID != "")
	{
		m_Bookmark.DeleteString(m_Bookmark.GetCurSel());
		//更新 INI 檔案設定
		UpdateINIFile();
	}
}

//跳到書籤指定處
void CBookmarkDlg::OnOK() 
{
	if(m_ID != "")
	{
		int ret;
		char pFileName[MAX_PATH], pFilePath[MAX_PATH], pFileLine[MAX_PATH];
		ret = GetPrivateProfileString(m_ID, "File", "未命名.txt", pFileName, MAX_PATH, m_INI_Path);
		ret = GetPrivateProfileString(m_ID, "Path", "C:\\", pFilePath, MAX_PATH, m_INI_Path);
		ret = GetPrivateProfileString(m_ID, "Line", "0", pFileLine, MAX_PATH, m_INI_Path);

		//Begin - 傳回檔案及書籤的行數
		m_FileInfo_Path = pFilePath;
		m_FileInfo_Name = pFileName;
		m_MarkLine  = atoi(pFileLine);
		//End   - 傳回檔案及書籤的行數
	}

	CDialog::OnOK();
}

/////////////////////////////////////////////////////////////////////////////
// Class Member Methods

void CBookmarkDlg::UpdateINIFile()
{
	//取得欲儲存的 INI 檔案內容
	CString Temp, Section, DataToWrite = "";
	int ret;
	char pFileName[MAX_PATH], pFilePath[MAX_PATH], pFileLine[MAX_PATH];
	for(int i=0; i<m_Bookmark.GetCount(); i++)
	{
		m_Bookmark.GetText(i, Section);
		ret = GetPrivateProfileString(Section, "File", "未命名.txt", pFileName, MAX_PATH, m_INI_Path);
		ret = GetPrivateProfileString(Section, "Path", "C:\\", pFilePath, MAX_PATH, m_INI_Path);
		ret = GetPrivateProfileString(Section, "Line", "0", pFileLine, MAX_PATH, m_INI_Path);

		Temp.Format("[%s]\r\nFile=%s\r\nPath=%s\r\nLine=%s\r\n", Section, pFileName, pFilePath, pFileLine);
		DataToWrite += Temp;
	}

	//Begin - Save INI File
	CFile file;
	if( !file.Open(m_INI_Path, CFile::modeCreate|CFile::modeWrite|CFile::typeBinary, NULL) )
	{
		AfxMessageBox("Open TextViewer.ini Error!!");
	}
	else
	{
		CFileException ex;
		try{
			file.Write(DataToWrite, DataToWrite.GetLength());
			file.Close();
		}
		catch(CFileException ex){
			TCHAR szError[1024];
			ex.GetErrorMessage(szError, 1024);
			AfxMessageBox(szError);
		}
	}
	//End   - Save INI File
}
