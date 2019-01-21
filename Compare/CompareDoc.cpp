// CompareDoc.cpp : implementation of the CCompareDoc class
//

#include "stdafx.h"
#include "MainFrm.h"
#include "Compare.h"
#include "CompareDoc.h"

#include "FirstView.h"
#include "SecondView.h"
#include "ReportView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCompareDoc

IMPLEMENT_DYNCREATE(CCompareDoc, CDocument)

BEGIN_MESSAGE_MAP(CCompareDoc, CDocument)
	//{{AFX_MSG_MAP(CCompareDoc)
	ON_COMMAND(ID_TOOL_COMPARE, OnToolCompare)
	ON_COMMAND(ID_TOOL_HEX, OnToolHex)
	ON_UPDATE_COMMAND_UI(ID_TOOL_HEX, OnUpdateToolHex)
	ON_COMMAND(ID_TOOL_CURSOR, OnToolCursor)
	ON_UPDATE_COMMAND_UI(ID_TOOL_CURSOR, OnUpdateToolCursor)
	ON_COMMAND(ID_FILE_NEW_1, OnFileNew1)
	ON_COMMAND(ID_FILE_NEW_2, OnFileNew2)
	ON_COMMAND(ID_FILE_SAVE_1, OnFileSave1)
	ON_COMMAND(ID_FILE_SAVE_2, OnFileSave2)
	ON_COMMAND(ID_FILE_OPEN_1, OnFileOpen1)
	ON_COMMAND(ID_FILE_OPEN_2, OnFileOpen2)
	ON_COMMAND(ID_FILE_SAVE_AS_1, OnFileSaveAs1)
	ON_COMMAND(ID_FILE_SAVE_AS_2, OnFileSaveAs2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCompareDoc construction/destruction

CCompareDoc::CCompareDoc()
{
	// TODO: add one-time construction code here
	m_pFirstView  = NULL;
	m_pSecondView = NULL;
	m_pReportView = NULL;

	m_file1 = NULL;
	m_file2 = NULL;
	m_filelen1 = 0;
	m_filelen2 = 0;

	m_is_hex = TRUE;
	m_is_cursor = FALSE;
}

CCompareDoc::~CCompareDoc()
{
	if(m_file1 != NULL)
	{
		delete m_file1;
		m_file1 = NULL;
	}

	if(m_file2 != NULL)
	{
		delete m_file2;
		m_file2 = NULL;
	}
}

BOOL CCompareDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	//Begin 取得 View
	POSITION pos = GetFirstViewPosition();
	if(pos == NULL) return FALSE;
	//[1] 取得 CFirstView
	m_pFirstView = (CFirstView*)GetNextView(pos);
	if(pos == NULL) return FALSE;
	//[2] 取得 CSecondView
	m_pSecondView = (CSecondView*)GetNextView(pos);
	if(pos == NULL) return FALSE;
	//[3] 取得 CReportView
	m_pReportView = (CReportView*)GetNextView(pos);
	//End 取得 View

	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CCompareDoc serialization

void CCompareDoc::Serialize(CArchive& ar)
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
// CCompareDoc diagnostics

#ifdef _DEBUG
void CCompareDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CCompareDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CCompareDoc commands

BOOL CCompareDoc::SaveModified() 
{
	//取消 詢問是否要存檔 的對話盒
	SetModifiedFlag(FALSE);

	return CDocument::SaveModified();
}

/////////////////////////////////////////////////////////////////////////////
// Menu Event Function

void CCompareDoc::OnFileNew1() 
{
	//清除 EditView 1
	m_pFirstView->SetWindowText(NULL);
	//清除檔名 1
	m_name1 = "";
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	pMainFrame->UpdateFileName1(m_name1);

	delete m_file1;
	m_file1 = NULL;
	m_filelen1 = 0;
}

void CCompareDoc::OnFileNew2() 
{
	//清除 EditView 2
	m_pSecondView->SetWindowText(NULL);
	//清除檔名 2
	m_name2 = "";
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	pMainFrame->UpdateFileName2(m_name2);

	delete m_file2;
	m_file2 = NULL;
	m_filelen2 = 0;
}

void CCompareDoc::OnFileOpen1() 
{
	//Open File 1
	LoadFile1();
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	pMainFrame->UpdateFileName1(m_name1);
}

void CCompareDoc::OnFileOpen2() 
{
	//Open File 2
	LoadFile2();
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	pMainFrame->UpdateFileName2(m_name2);
}

void CCompareDoc::OnFileSave1() 
{
	if( !m_is_hex )
	{
		//Save File 1
		CFile file;
		if( !file.Open(m_name1, CFile::modeCreate|CFile::modeWrite|CFile::typeBinary, NULL) )
		{
			CString msg;
			msg.Format("Open File %s Error!!", m_name1);
			AfxMessageBox(msg);
		}
		else
		{
			CFileException ex;
			CString WriteData;
			m_pFirstView->GetEditCtrl().GetWindowText(WriteData);
			try{
				file.Write(WriteData, WriteData.GetLength());
				file.Close();
				AfxMessageBox("檔案-1 存檔成功!!");
			}
			catch(CFileException ex){
				TCHAR szError[1024];
				ex.GetErrorMessage(szError, 1024);
				AfxMessageBox(szError);
			}
		}
	}
}

void CCompareDoc::OnFileSave2() 
{
	if( !m_is_hex )
	{
		//Save File 2
		CFile file;
		if( !file.Open(m_name2, CFile::modeCreate|CFile::modeWrite|CFile::typeBinary, NULL) )
		{
			CString msg;
			msg.Format("Open File %s Error!!", m_name2);
			AfxMessageBox(msg);
		}
		else
		{
			CFileException ex;
			CString WriteData;
			m_pFirstView->GetEditCtrl().GetWindowText(WriteData);
			try{
				file.Write(WriteData, WriteData.GetLength());
				file.Close();
				AfxMessageBox("檔案-2 存檔成功!!");
			}
			catch(CFileException ex){
				TCHAR szError[1024];
				ex.GetErrorMessage(szError, 1024);
				AfxMessageBox(szError);
			}
		}
	}
}

void CCompareDoc::OnFileSaveAs1() 
{
	//Begin Save File Dialog
	CFileDialog dlg(FALSE, NULL, "*.txt", NULL, "Text File (*.txt)|*.txt|All File (*.*)|*.*|");
	if(dlg.DoModal() == IDOK)
	{
		//Save File As ...
		CFile file;
		if( !file.Open(dlg.GetPathName(), CFile::modeCreate|CFile::modeWrite|CFile::typeBinary, NULL) )
		{
			CString msg;
			msg.Format("Open File %s Error!!", dlg.GetPathName());
			AfxMessageBox(msg);
		}
		else
		{
			CFileException ex;
			CString WriteData;
			m_pFirstView->GetEditCtrl().GetWindowText(WriteData);
			try{
				file.Write(WriteData, WriteData.GetLength());
				file.Close();
				CString msg;
				msg.Format("%s 存檔成功!!", dlg.GetFileName());
				AfxMessageBox(msg);

				//修改檔案名稱
				m_name1 = dlg.GetFileName();
				CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
				pMainFrame->UpdateFileName1(m_name1);
			}
			catch(CFileException ex){
				TCHAR szError[1024];
				ex.GetErrorMessage(szError, 1024);
				AfxMessageBox(szError);
			}
		}
	}
	//End Save File Dialog
}

void CCompareDoc::OnFileSaveAs2() 
{
	//Begin Save File Dialog
	CFileDialog dlg(FALSE, NULL, "*.txt", NULL, "Text File (*.txt)|*.txt|All File (*.*)|*.*|");
	if(dlg.DoModal() == IDOK)
	{
		//Save File As ...
		CFile file;
		if( !file.Open(dlg.GetPathName(), CFile::modeCreate|CFile::modeWrite|CFile::typeBinary, NULL) )
		{
			CString msg;
			msg.Format("Open File %s Error!!", dlg.GetPathName());
			AfxMessageBox(msg);
		}
		else
		{
			CFileException ex;
			CString WriteData;
			m_pSecondView->GetEditCtrl().GetWindowText(WriteData);
			try{
				file.Write(WriteData, WriteData.GetLength());
				file.Close();
				CString msg;
				msg.Format("%s 存檔成功!!", dlg.GetFileName());
				AfxMessageBox(msg);

				//修改檔案名稱
				m_name2 = dlg.GetFileName();
				CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
				pMainFrame->UpdateFileName2(m_name2);
			}
			catch(CFileException ex){
				TCHAR szError[1024];
				ex.GetErrorMessage(szError, 1024);
				AfxMessageBox(szError);
			}
		}
	}
	//End Save File Dialog
}

void CCompareDoc::OnToolCompare() 
{
	//讀取 View 中的資料
	if( !m_is_hex )
	{
		//Begin of File-1
		delete m_file1;
		m_file1 = NULL;
		m_filelen1 = m_pFirstView->GetWindowTextLength();

		HLOCAL h;
		h = m_pFirstView->GetEditCtrl().GetHandle();
		LPCTSTR text1 = (LPCTSTR) ::LocalLock(h);
		::LocalUnlock(h);

		m_file1 = new byte[m_filelen1];
		memset(m_file1, 0x00, m_filelen1);
		memcpy(m_file1, text1, m_filelen1);
		//End of File-1

		//Begin of File-2
		delete m_file2;
		m_file2 = NULL;
		m_filelen2 = m_pSecondView->GetWindowTextLength();

		h = m_pSecondView->GetEditCtrl().GetHandle();
		LPCTSTR text2 = (LPCTSTR) ::LocalLock(h);
		::LocalUnlock(h);

		m_file2 = new byte[m_filelen2];
		memset(m_file2, 0x00, m_filelen2);
		memcpy(m_file2, text2, m_filelen2);
		//End of File-2
	}

	//只要有一個檔案長為０，就不能比
	if((m_filelen1 == 0) || (m_filelen2 == 0))
		return;

	//清除目前所有顯示的項目(Row)
	m_pReportView->GetListCtrl().DeleteAllItems();
	byte char1, char2;

	unsigned long i, count = 0;
	unsigned long length = (m_filelen1 <= m_filelen2) ? m_filelen1 : m_filelen2;
	for(i=0; i<length; i++)
	{
		if(m_file1[i] != m_file2[i])
		{
			char1 = m_file1[i];
			char2 = m_file2[i];
			m_pReportView->AddReportItem(i, char1, char2);
			count++;
		}
	}

	//比較結果
	if(count == 0)
	{
		if(m_filelen1 == m_filelen2)
			AfxMessageBox("兩個檔案的內容完全相同!!");
		else
			AfxMessageBox("檔案前面相同，但長度不同!!");
	}
}

void CCompareDoc::OnToolHex() 
{
	m_is_hex = !m_is_hex;
	if( m_is_hex )
	{
		//以十六進制顯示
		m_pFirstView->DisplayByHex();
		m_pSecondView->DisplayByHex();
	}
	else
	{
		//以正常方式顯示
		m_pFirstView->DisplayByNormal();
		m_pSecondView->DisplayByNormal();
	}
}

void CCompareDoc::OnUpdateToolHex(CCmdUI* pCmdUI) 
{
	//保持 Hex 按鈕的狀態
	if( m_is_hex )
		pCmdUI->SetCheck(1); //按鈕壓下
	else
		pCmdUI->SetCheck(0); //按鈕彈起
}

void CCompareDoc::OnToolCursor() 
{
	m_is_cursor = !m_is_cursor;
}

void CCompareDoc::OnUpdateToolCursor(CCmdUI* pCmdUI) 
{
	//保持 游標同步 的狀態
	if( m_is_cursor )
		pCmdUI->SetCheck(1); //按鈕壓下
	else
		pCmdUI->SetCheck(0); //按鈕彈起
}

/////////////////////////////////////////////////////////////////////////////
// Class Member Function

void CCompareDoc::LoadFile1() 
{
	//Begin Open File Dialog
	CFileDialog dlg(TRUE, NULL, "*.*", NULL, "All File (*.*)|*.*|");
	if(dlg.DoModal() == IDOK)
	{
		CString FilePath = dlg.GetPathName();
		m_name1 = dlg.GetFileName();

		CFile file;
		if( !file.Open(FilePath, CFile::modeRead|CFile::typeBinary, NULL) )
		{
			AfxMessageBox("Open File Error!!");
			m_name1 = "";
		}
		else
		{
			//Begin 讀出檔案內容
			m_filelen1 = file.GetLength();
			m_file1 = new byte[m_filelen1];
			memset(m_file1, 0x00, m_filelen1);
			file.Read(m_file1, m_filelen1);
			file.Close();
			//End 讀出檔案內容

			//以 十六進制/正常方式 來顯示內容
			if( m_is_hex)
				m_pFirstView->DisplayByHex();
			else
				m_pFirstView->DisplayByNormal();
		}
	}
	//End Open File Dialog
}

void CCompareDoc::LoadFile2() 
{
	//Begin Open File Dialog
	CFileDialog dlg(TRUE, NULL, "*.*", NULL, "All File (*.*)|*.*|");
	if(dlg.DoModal() == IDOK)
	{
		CString FilePath = dlg.GetPathName();
		m_name2 = dlg.GetFileName();

		CFile file;
		if( !file.Open(FilePath, CFile::modeRead|CFile::typeBinary, NULL) )
		{
			AfxMessageBox("Open File Error!!");
			m_name2 = "";
		}
		else
		{
			//Begin 讀出檔案內容
			m_filelen2 = file.GetLength();
			m_file2 = new byte[m_filelen2];
			memset(m_file2, 0x00, m_filelen2);
			file.Read(m_file2, m_filelen2);
			file.Close();
			//End 讀出檔案內容

			//以 十六進制/正常方式 來顯示內容
			if( m_is_hex )
				m_pSecondView->DisplayByHex();
			else
				m_pSecondView->DisplayByNormal();
		}
	}
	//End Open File Dialog
}
