// TextViewerDoc.cpp : implementation of the CTextViewerDoc class
//

#include "stdafx.h"
#include "MainFrm.h"
#include "TextViewer.h"
#include "TextViewerDoc.h"
#include "BookmarkDlg.h"
#include <sys/stat.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CTextViewerDoc

IMPLEMENT_DYNCREATE(CTextViewerDoc, CDocument)

BEGIN_MESSAGE_MAP(CTextViewerDoc, CDocument)
	//{{AFX_MSG_MAP(CTextViewerDoc)
	ON_COMMAND(ID_FILE_NEW, OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_COMMAND(ID_FILE_SAVE_AS, OnFileSaveAs)
	ON_COMMAND(ID_FILE_SAVE, OnFileSave)
	ON_COMMAND(ID_TOOL_HEX, OnToolHex)
	ON_COMMAND(ID_TOOL_FONT, OnToolFont)
	ON_UPDATE_COMMAND_UI(ID_TOOL_HEX, OnUpdateToolHex)
	ON_COMMAND(ID_TOOL_FIND, OnToolFind)
	ON_COMMAND(ID_TOOL_REPLACE, OnToolReplace)
	ON_COMMAND(ID_TOOL_BREAK, OnToolBreak)
	ON_UPDATE_COMMAND_UI(ID_TOOL_BREAK, OnUpdateToolBreak)
	ON_COMMAND(ID_EDIT_SELECT, OnEditSelect)
	ON_COMMAND(ID_BUTTON_BOOKMARK, OnButtonBookmark)
	ON_COMMAND(ID_BUTTON_HISTORY, OnButtonHistory)
	//}}AFX_MSG_MAP

	ON_COMMAND(ID_RECORD+1, OnHistoryFile1)
	ON_COMMAND(ID_RECORD+2, OnHistoryFile2)
	ON_COMMAND(ID_RECORD+3, OnHistoryFile3)
	ON_COMMAND(ID_RECORD+4, OnHistoryFile4)
	ON_COMMAND(ID_RECORD+5, OnHistoryFile5)
	ON_COMMAND(ID_RECORD+6, OnHistoryFile6)
	ON_COMMAND(ID_RECORD+7, OnHistoryFile7)
	ON_COMMAND(ID_RECORD+8, OnHistoryFile8)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTextViewerDoc construction/destruction

CTextViewerDoc::CTextViewerDoc()
{
	// TODO: add one-time construction code here
	m_pTreeView    = NULL;
	m_pListView    = NULL;
	m_pEditView    = NULL;
	m_Text_Content = NULL;
	m_Text_Length  = 0;
	m_MenuNumber   = 0;
	m_Is_Hex       = FALSE;
	m_Is_Brk       = TRUE;
	m_Is_ReadOnly  = FALSE;
	m_Total        = 1;
}

CTextViewerDoc::~CTextViewerDoc()
{
	if(m_Text_Content != NULL)
	{
		delete m_Text_Content;
		m_Text_Content = NULL;
	}
}

BOOL CTextViewerDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	//Begin - 取得各個 View 的指標
	POSITION pos = GetFirstViewPosition();
	if(pos == NULL) return FALSE;
	//[1] 取得 CTreeView
	m_pTreeView = (CTextTreeView*)GetNextView(pos);
	if(pos == NULL) return FALSE;
	//[2] 取得 CListView
	m_pListView = (CTextListView*)GetNextView(pos);
	if(pos == NULL) return FALSE;
	//[3] 取得 CEditView
	m_pEditView = (CTextEditView*)GetNextView(pos);
	//End   - 取得各個 View 的指標

	//Begin - 取得目前路徑
	char path_buffer[MAX_PATH];
	memset(path_buffer, 0x00, MAX_PATH);
	int path_len = GetCurrentDirectory(MAX_PATH, path_buffer);
	m_MyDirectory = path_buffer;

	if(m_MyDirectory.GetLength() > 3)
		m_MyDirectory += "\\";
	m_FileName = m_MyDirectory + "未命名.txt";
	//End   - 取得目前路徑

	//Begin - 取得前次瀏覽時的資料夾
	//[1] 從 Registry 中取出前次的資料夾路徑
	CWinApp* pApp = AfxGetApp();
	m_ViewDirectory = pApp->GetProfileString("TextViewer", "ViewPath", "");

	//[2] 判斷該路徑是否存在
	struct _stat buf;
	CString testpath = m_ViewDirectory;
	if (testpath.GetLength() > 3)
	{
		testpath.TrimRight("\\");
	}
	int ret = _stat((LPCTSTR)testpath, &buf);
	if (testpath == "" || ret == -1)
	{
		m_ViewDirectory = m_MyDirectory;
	}
	//End   - 取得前次瀏覽時的資料夾

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CTextViewerDoc serialization

void CTextViewerDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CTextViewerDoc diagnostics

#ifdef _DEBUG
void CTextViewerDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CTextViewerDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTextViewerDoc commands

void CTextViewerDoc::OnFileNew() 
{
	//清除 EditView
	m_pEditView->SetWindowText(NULL);
	if(m_Text_Content != NULL)
	{
		delete m_Text_Content;
		m_Text_Content = NULL;
		m_Text_Length  = 0;
	}

	//重設檔案路徑
	m_FileName = m_pListView->m_ScanPath + "未命名.txt";

	//修改視窗標題
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	pMainFrame->SetWindowText("未命名 - TextViewer");

	//恢復正常方式顯示
	m_Is_Hex      = FALSE;
	m_Is_Brk      = TRUE;
	m_Is_ReadOnly = FALSE;
	m_pEditView->GetEditCtrl().SetReadOnly( m_Is_ReadOnly );

	//清除狀態列的行數欄
	m_Total = 1;
	DisplayLines("Line / Total");
}

void CTextViewerDoc::OnFileOpen() 
{
	//Begin - Open File Dialog
	CFileDialog dlg(TRUE, NULL, "*.txt", NULL, "Text File (*.txt)|*.txt|All File (*.*)|*.*|");
	if(dlg.DoModal() == IDOK)
	{
		if( DisplayFile(dlg.GetPathName()) )
		{
			//重設檔案路徑
			m_FileName = dlg.GetPathName();

			//修改視窗標題
			CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
			pMainFrame->SetWindowText(dlg.GetFileName() + " - TextViewer");
		}
	}
	//End   - Open File Dialog
}

void CTextViewerDoc::OnFileSave()
{
	//Begin - Save File Dialog
	CFile file;
	if( !file.Open(m_FileName, CFile::modeCreate|CFile::modeWrite|CFile::typeBinary, NULL) )
	{
		AfxMessageBox("Open File Error!!");
	}
	else
	{
		CFileException ex;
		CString WriteData;
		m_pEditView->GetEditCtrl().GetWindowText(WriteData);
		try{
			file.Write(WriteData, WriteData.GetLength());
			file.Close();
			AfxMessageBox("存檔成功!!");
			SetModifiedFlag( FALSE );
		}
		catch(CFileException ex){
			TCHAR szError[1024];
			ex.GetErrorMessage(szError, 1024);
			AfxMessageBox(szError);
		}
	}
	//End   - Save File Dialog
}

void CTextViewerDoc::OnFileSaveAs() 
{
	//Begin - Save File As Dialog
	CFileDialog dlg(FALSE, NULL, "*.txt", NULL, "Text File (*.txt)|*.txt|All File (*.*)|*.*|");
	if(dlg.DoModal() == IDOK)
	{
		//Save File As ...
		CFile file;
		if( !file.Open(dlg.GetPathName(), CFile::modeCreate|CFile::modeWrite|CFile::typeBinary, NULL) )
		{
			AfxMessageBox("Open File Error!!");
		}
		else
		{
			CFileException ex;
			CString WriteData;
			m_pEditView->GetEditCtrl().GetWindowText(WriteData);
			try{
				file.Write(WriteData, WriteData.GetLength());
				file.Close();
				CString msg;
				msg.Format("%s 存檔成功!!", dlg.GetFileName());
				AfxMessageBox(msg);
				SetModifiedFlag( FALSE );

				//修改視窗標題
				CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
				pMainFrame->SetWindowText(dlg.GetFileName() + " - TextViewer");
				m_FileName = dlg.GetPathName();
			}
			catch(CFileException ex){
				TCHAR szError[1024];
				ex.GetErrorMessage(szError, 1024);
				AfxMessageBox(szError);
			}
		}
	}
	//End   - Save File As Dialog
}

void CTextViewerDoc::OnEditSelect() 
{
	//選取整個文件內容
	m_pEditView->GetEditCtrl().SetSel(0, -1, TRUE);
}

void CTextViewerDoc::OnToolFind() 
{
	//進行字串搜尋
	m_pEditView->InitFindReplaceDlg(TRUE);
}

void CTextViewerDoc::OnToolReplace() 
{
	//進行字串取代
	m_pEditView->InitFindReplaceDlg(FALSE);
}

void CTextViewerDoc::OnToolFont() 
{
	//改變顯示的字型
	m_pEditView->ChangeEditFont();
}

void CTextViewerDoc::OnToolHex() 
{
	m_Is_Hex = !m_Is_Hex;
	if( m_Is_Hex )
	{
		//儲存目前顯示的文字
		m_pEditView->KeepEditContent();

		//以十六進制顯示
		m_pEditView->DisplayByHex();
	}
	else
	{
		//以正常方式顯示 (預設)
		m_pEditView->DisplayByNormal();
	}
}

void CTextViewerDoc::OnUpdateToolHex(CCmdUI* pCmdUI) 
{
	//保持 Hex 按鈕的狀態
	if( m_Is_Hex )
	{
		//按鈕壓下
		pCmdUI->SetCheck(1);
	}
	else
	{
		//按鈕彈起
		pCmdUI->SetCheck(0);
	}
}

void CTextViewerDoc::OnToolBreak() 
{
	m_Is_Brk = !m_Is_Brk;
	if( m_Is_Brk )
	{
		//以 Windows 斷行格式顯示 (預設)
		m_pEditView->BreakLineByWindows();
	}
	else
	{
		//以 Unix 斷行格式顯示
		m_pEditView->BreakLineByUnix();
	}
}

void CTextViewerDoc::OnUpdateToolBreak(CCmdUI* pCmdUI) 
{
	//保持 Break-Line 按鈕的狀態
	if( m_Is_Brk )
	{
		//按鈕壓下
		pCmdUI->SetCheck(1);
	}
	else
	{
		//按鈕彈起
		pCmdUI->SetCheck(0);
	}
}

//檔案書籤功能
void CTextViewerDoc::OnButtonBookmark() 
{
	//從完整路徑中取出檔案名稱
	int line = m_pEditView->GetEditCtrl().LineFromChar()+1;
	int pos  = m_FileName.ReverseFind( '\\' );
	CString path = m_FileName.Mid(0, pos+1);
	CString name = m_FileName.Mid(pos+1, m_FileName.GetLength()-pos+1);

	//將目前書籤設定傳給對話盒
	CBookmarkDlg dlg;
	dlg.m_INI_Path      = m_MyDirectory + "TextViewer.ini";
	dlg.m_FileInfo_Name = name;
	dlg.m_FileInfo_Path = path;
	dlg.m_MarkLine      = line;
	if(dlg.DoModal() == IDOK)
	{
		//開啟該檔案並跳到指定行數
		CString WhichFile = dlg.m_FileInfo_Path + dlg.m_FileInfo_Name;
		if( DisplayFile(WhichFile) )
		{
			//重設檔案路徑
			m_FileName = WhichFile;

			//顯示檔名於視窗標題
			CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
			pMainFrame->SetWindowText(dlg.m_FileInfo_Name + " - TextViewer");

			//展開資料夾
			m_pTreeView->m_ExpandDirectory = dlg.m_FileInfo_Path;
			m_pTreeView->ExpandCurrentFolder();

			//跳到書籤記錄之行數
			m_pEditView->Button_GotoLines( dlg.m_MarkLine );
		}
	}
}

//檔案開啟記錄功能
void CTextViewerDoc::OnButtonHistory() 
{
	//判斷該檔案是否存在
	struct _stat buf;
	int ret = _stat((LPCTSTR)m_FileName, &buf);
	if (ret == -1)
	{
		CString msg;
		msg.Format("%s\n此檔案不存在!!", m_FileName);
		AfxMessageBox( msg );
	}
	else
	{
		//將檔名加到歷史記錄
		AddHistoryRecord();
	}
}

/////////////////////////////////////////////////////////////////////////////
// 檔案歷史記錄的處理函式

// 檔案記錄選單的結構如下：
// 
// 記錄(R)
//   + 新增瀏覽記錄   ID_BUTTON_HISTORY
//   + -------------  Separator
//   + Record File 1  ID_RECORD+1
//     ...            ...
//   + Record File 8  ID_RECORD+8
//
// 其中 ID_RECORD 作為子選單的資源起始號碼，
// 本身並未在程式中使用。

void CTextViewerDoc::OnHistoryFile1()
{
	OpenRecordFile(ID_RECORD+1);
}

void CTextViewerDoc::OnHistoryFile2()
{
	OpenRecordFile(ID_RECORD+2);
}

void CTextViewerDoc::OnHistoryFile3()
{
	OpenRecordFile(ID_RECORD+3);
}

void CTextViewerDoc::OnHistoryFile4()
{
	OpenRecordFile(ID_RECORD+4);
}

void CTextViewerDoc::OnHistoryFile5()
{
	OpenRecordFile(ID_RECORD+5);
}

void CTextViewerDoc::OnHistoryFile6()
{
	OpenRecordFile(ID_RECORD+6);
}

void CTextViewerDoc::OnHistoryFile7()
{
	OpenRecordFile(ID_RECORD+7);
}

void CTextViewerDoc::OnHistoryFile8()
{
	OpenRecordFile(ID_RECORD+8);
}

/////////////////////////////////////////////////////////////////////////////
// Class Member Methods

BOOL CTextViewerDoc::DisplayFile(CString filename)
{
	CFile file;
	if( !file.Open(filename, CFile::modeRead|CFile::typeBinary, NULL) )
	{
		AfxMessageBox("Open File Error!!");
		return FALSE;
	}
	else
	{
		//Begin - 判斷檔案是否為唯讀
		CFileStatus status;
		if( file.GetStatus( status ) )
		{
			m_Is_ReadOnly = (status.m_attribute & 0x01) ? TRUE : FALSE;
		}
		//End   - 判斷檔案是否為唯讀

		//Begin - 讀出檔案內容
		unsigned long length = file.GetLength();
		byte *ReadData = new byte[length+1];
		memset(ReadData, 0x00, length+1);
		file.Read(ReadData, length);
		file.Close();
		//End   - 讀出檔案內容

		//Begin - 儲存目前顯示的文字
		if(m_Text_Content != NULL)
		{
			delete m_Text_Content;
			m_Text_Content = NULL;
		}

		byte* content = new byte[length+1];
		memset(content, 0x00, length+1);
		memcpy(content, ReadData, length);
		m_Text_Content = (LPTSTR)content;
		m_Text_Length = length;
		delete ReadData;
		//End   - 儲存目前顯示的文字

		//Begin 顯示檔案內容
		m_pEditView->GetEditCtrl().SetReadOnly( m_Is_ReadOnly );
		m_pEditView->SetWindowText( m_Text_Content );
		//End 顯示檔案內容

		//Begin - 判斷換行符號格式
		m_Is_Hex = FALSE;
		m_pEditView->GetEditCtrl().SetSel(0, 0, TRUE);
		m_Is_Brk = m_pEditView->FindText("\r\n");
		m_pEditView->GetEditCtrl().SetSel(0, 0, TRUE);
		//End   - 判斷換行符號格式

		//Begin - 控制狀態列
		m_Total = m_pEditView->GetEditCtrl().GetLineCount();
		CString msg;
		msg.Format("1 / %d", m_Total);

		CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
		pMainFrame->SetStatusText(4, msg);
		//End   - 控制狀態列
	}

	return TRUE;
}

void CTextViewerDoc::DisplayLines(CString msg)
{
	//Begin - 控制狀態列
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	pMainFrame->SetStatusText(4, msg);
	//End   - 控制狀態列
}

void CTextViewerDoc::SetNewDirectory(CString dir)
{
	if(dir != "")
	{
		//改變新的檔案掃描路徑
		SetCurrentDirectory(dir);
		m_pListView->m_ScanPath = dir;

		//更新 List 的檔案列表
		m_pListView->ReadDirectoryInfo();
	}

	//在狀態列上顯示目前目錄名稱
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	pMainFrame->SetStatusText(0, m_pListView->m_ScanPath);
}

void CTextViewerDoc::AddHistoryRecord()
{
	//Begin - 取得 Menu 的指標
	CMenu* pMainMenu, *pRecordMenu;
	pMainMenu = AfxGetApp()->m_pMainWnd->GetMenu();
	pRecordMenu = pMainMenu->GetSubMenu(4);
	pMainMenu->Detach();
	//End   - 取得 Menu 的指標

	CString MenuString;
	int ExistIndex = -1;
	for(unsigned int i=0; i<pRecordMenu->GetMenuItemCount(); i++)
	{
		//清除子項目的勾核記號
		UINT state = pRecordMenu->GetMenuState(ID_RECORD+i+1, MF_BYCOMMAND);
		if(state & MF_CHECKED)
			pRecordMenu->CheckMenuItem(ID_RECORD+i+1, MF_UNCHECKED|MF_BYCOMMAND);
		//判斷記錄是否重複
		pRecordMenu->GetMenuString(ID_RECORD+i+1, MenuString, MF_BYCOMMAND);
		if(MenuString == m_FileName) ExistIndex = i;
	}

	if(ExistIndex == -1)
	{
		int a = pRecordMenu->GetMenuItemCount();
		if(pRecordMenu->GetMenuItemCount() < 8+2)
		{
			//在 Menu 增加一個新的子選單
			m_MenuNumber++;
			pRecordMenu->AppendMenu(MF_STRING|MF_CHECKED, ID_RECORD+m_MenuNumber, m_FileName);
		}
		else
		{
			//調整記錄的項目(將最舊的記錄移除)
			for(int i=1; i<8; i++)
			{
				pRecordMenu->GetMenuString(ID_RECORD+i+1, MenuString, MF_BYCOMMAND);
				pRecordMenu->ModifyMenu(ID_RECORD+i, MF_STRING|MF_BYCOMMAND, ID_RECORD+i, MenuString);
			}
			pRecordMenu->ModifyMenu(ID_RECORD+8, MF_STRING|MF_CHECKED|MF_BYCOMMAND, ID_RECORD+8, m_FileName);
		}
	}
	else
	{
		//勾核目前顯示的檔案
		pRecordMenu->CheckMenuItem(ID_RECORD+ExistIndex+1, MF_CHECKED|MF_BYCOMMAND);
	}
}

void CTextViewerDoc::OpenRecordFile(UINT nID)
{
	//Begin - 取得 Menu 的指標
	CMenu *pMainMenu, *pRecordMenu;
	pMainMenu = AfxGetApp()->m_pMainWnd->GetMenu();
	pRecordMenu = pMainMenu->GetSubMenu(4);
	pMainMenu->Detach();
	//End   - 取得 Menu 的指標

	//Begin - 清除子項目的勾核記號
	for(unsigned int i=0; i<pRecordMenu->GetMenuItemCount(); i++)
	{
		UINT state = pRecordMenu->GetMenuState(ID_RECORD+i+1, MF_BYCOMMAND);
		if(state & MF_CHECKED)
			pRecordMenu->CheckMenuItem(ID_RECORD+i+1, MF_UNCHECKED|MF_BYCOMMAND);
	}
	//End   - 清除子項目的勾核記號

	CString MenuString;
	pRecordMenu->GetMenuString(nID, MenuString, MF_BYCOMMAND);
	pRecordMenu->CheckMenuItem(nID, MF_CHECKED|MF_BYCOMMAND);
	if( DisplayFile(MenuString) )
	{
		//重設檔案路徑
		m_FileName = MenuString;

		//顯示檔名於視窗標題
		int pos = m_FileName.ReverseFind( '\\' );
		CString name = m_FileName.Mid(pos+1, m_FileName.GetLength()-pos+1);
		CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
		pMainFrame->SetWindowText(name + " - TextViewer");
	}
}
