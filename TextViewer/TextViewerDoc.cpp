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

	//Begin - ���o�U�� View ������
	POSITION pos = GetFirstViewPosition();
	if(pos == NULL) return FALSE;
	//[1] ���o CTreeView
	m_pTreeView = (CTextTreeView*)GetNextView(pos);
	if(pos == NULL) return FALSE;
	//[2] ���o CListView
	m_pListView = (CTextListView*)GetNextView(pos);
	if(pos == NULL) return FALSE;
	//[3] ���o CEditView
	m_pEditView = (CTextEditView*)GetNextView(pos);
	//End   - ���o�U�� View ������

	//Begin - ���o�ثe���|
	char path_buffer[MAX_PATH];
	memset(path_buffer, 0x00, MAX_PATH);
	int path_len = GetCurrentDirectory(MAX_PATH, path_buffer);
	m_MyDirectory = path_buffer;

	if(m_MyDirectory.GetLength() > 3)
		m_MyDirectory += "\\";
	m_FileName = m_MyDirectory + "���R�W.txt";
	//End   - ���o�ثe���|

	//Begin - ���o�e���s���ɪ���Ƨ�
	//[1] �q Registry �����X�e������Ƨ����|
	CWinApp* pApp = AfxGetApp();
	m_ViewDirectory = pApp->GetProfileString("TextViewer", "ViewPath", "");

	//[2] �P�_�Ӹ��|�O�_�s�b
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
	//End   - ���o�e���s���ɪ���Ƨ�

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
	//�M�� EditView
	m_pEditView->SetWindowText(NULL);
	if(m_Text_Content != NULL)
	{
		delete m_Text_Content;
		m_Text_Content = NULL;
		m_Text_Length  = 0;
	}

	//���]�ɮ׸��|
	m_FileName = m_pListView->m_ScanPath + "���R�W.txt";

	//�ק�������D
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	pMainFrame->SetWindowText("���R�W - TextViewer");

	//��_���`�覡���
	m_Is_Hex      = FALSE;
	m_Is_Brk      = TRUE;
	m_Is_ReadOnly = FALSE;
	m_pEditView->GetEditCtrl().SetReadOnly( m_Is_ReadOnly );

	//�M�����A�C�������
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
			//���]�ɮ׸��|
			m_FileName = dlg.GetPathName();

			//�ק�������D
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
			AfxMessageBox("�s�ɦ��\!!");
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
				msg.Format("%s �s�ɦ��\!!", dlg.GetFileName());
				AfxMessageBox(msg);
				SetModifiedFlag( FALSE );

				//�ק�������D
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
	//�����Ӥ�󤺮e
	m_pEditView->GetEditCtrl().SetSel(0, -1, TRUE);
}

void CTextViewerDoc::OnToolFind() 
{
	//�i��r��j�M
	m_pEditView->InitFindReplaceDlg(TRUE);
}

void CTextViewerDoc::OnToolReplace() 
{
	//�i��r����N
	m_pEditView->InitFindReplaceDlg(FALSE);
}

void CTextViewerDoc::OnToolFont() 
{
	//������ܪ��r��
	m_pEditView->ChangeEditFont();
}

void CTextViewerDoc::OnToolHex() 
{
	m_Is_Hex = !m_Is_Hex;
	if( m_Is_Hex )
	{
		//�x�s�ثe��ܪ���r
		m_pEditView->KeepEditContent();

		//�H�Q���i�����
		m_pEditView->DisplayByHex();
	}
	else
	{
		//�H���`�覡��� (�w�])
		m_pEditView->DisplayByNormal();
	}
}

void CTextViewerDoc::OnUpdateToolHex(CCmdUI* pCmdUI) 
{
	//�O�� Hex ���s�����A
	if( m_Is_Hex )
	{
		//���s���U
		pCmdUI->SetCheck(1);
	}
	else
	{
		//���s�u�_
		pCmdUI->SetCheck(0);
	}
}

void CTextViewerDoc::OnToolBreak() 
{
	m_Is_Brk = !m_Is_Brk;
	if( m_Is_Brk )
	{
		//�H Windows �_��榡��� (�w�])
		m_pEditView->BreakLineByWindows();
	}
	else
	{
		//�H Unix �_��榡���
		m_pEditView->BreakLineByUnix();
	}
}

void CTextViewerDoc::OnUpdateToolBreak(CCmdUI* pCmdUI) 
{
	//�O�� Break-Line ���s�����A
	if( m_Is_Brk )
	{
		//���s���U
		pCmdUI->SetCheck(1);
	}
	else
	{
		//���s�u�_
		pCmdUI->SetCheck(0);
	}
}

//�ɮ׮��ҥ\��
void CTextViewerDoc::OnButtonBookmark() 
{
	//�q������|�����X�ɮצW��
	int line = m_pEditView->GetEditCtrl().LineFromChar()+1;
	int pos  = m_FileName.ReverseFind( '\\' );
	CString path = m_FileName.Mid(0, pos+1);
	CString name = m_FileName.Mid(pos+1, m_FileName.GetLength()-pos+1);

	//�N�ثe���ҳ]�w�ǵ���ܲ�
	CBookmarkDlg dlg;
	dlg.m_INI_Path      = m_MyDirectory + "TextViewer.ini";
	dlg.m_FileInfo_Name = name;
	dlg.m_FileInfo_Path = path;
	dlg.m_MarkLine      = line;
	if(dlg.DoModal() == IDOK)
	{
		//�}�Ҹ��ɮרø�����w���
		CString WhichFile = dlg.m_FileInfo_Path + dlg.m_FileInfo_Name;
		if( DisplayFile(WhichFile) )
		{
			//���]�ɮ׸��|
			m_FileName = WhichFile;

			//����ɦW��������D
			CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
			pMainFrame->SetWindowText(dlg.m_FileInfo_Name + " - TextViewer");

			//�i�}��Ƨ�
			m_pTreeView->m_ExpandDirectory = dlg.m_FileInfo_Path;
			m_pTreeView->ExpandCurrentFolder();

			//������ҰO�������
			m_pEditView->Button_GotoLines( dlg.m_MarkLine );
		}
	}
}

//�ɮ׶}�ҰO���\��
void CTextViewerDoc::OnButtonHistory() 
{
	//�P�_���ɮ׬O�_�s�b
	struct _stat buf;
	int ret = _stat((LPCTSTR)m_FileName, &buf);
	if (ret == -1)
	{
		CString msg;
		msg.Format("%s\n���ɮפ��s�b!!", m_FileName);
		AfxMessageBox( msg );
	}
	else
	{
		//�N�ɦW�[����v�O��
		AddHistoryRecord();
	}
}

/////////////////////////////////////////////////////////////////////////////
// �ɮ׾��v�O�����B�z�禡

// �ɮװO����檺���c�p�U�G
// 
// �O��(R)
//   + �s�W�s���O��   ID_BUTTON_HISTORY
//   + -------------  Separator
//   + Record File 1  ID_RECORD+1
//     ...            ...
//   + Record File 8  ID_RECORD+8
//
// �䤤 ID_RECORD �@���l��檺�귽�_�l���X�A
// �����å��b�{�����ϥΡC

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
		//Begin - �P�_�ɮ׬O�_����Ū
		CFileStatus status;
		if( file.GetStatus( status ) )
		{
			m_Is_ReadOnly = (status.m_attribute & 0x01) ? TRUE : FALSE;
		}
		//End   - �P�_�ɮ׬O�_����Ū

		//Begin - Ū�X�ɮפ��e
		unsigned long length = file.GetLength();
		byte *ReadData = new byte[length+1];
		memset(ReadData, 0x00, length+1);
		file.Read(ReadData, length);
		file.Close();
		//End   - Ū�X�ɮפ��e

		//Begin - �x�s�ثe��ܪ���r
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
		//End   - �x�s�ثe��ܪ���r

		//Begin ����ɮפ��e
		m_pEditView->GetEditCtrl().SetReadOnly( m_Is_ReadOnly );
		m_pEditView->SetWindowText( m_Text_Content );
		//End ����ɮפ��e

		//Begin - �P�_����Ÿ��榡
		m_Is_Hex = FALSE;
		m_pEditView->GetEditCtrl().SetSel(0, 0, TRUE);
		m_Is_Brk = m_pEditView->FindText("\r\n");
		m_pEditView->GetEditCtrl().SetSel(0, 0, TRUE);
		//End   - �P�_����Ÿ��榡

		//Begin - ����A�C
		m_Total = m_pEditView->GetEditCtrl().GetLineCount();
		CString msg;
		msg.Format("1 / %d", m_Total);

		CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
		pMainFrame->SetStatusText(4, msg);
		//End   - ����A�C
	}

	return TRUE;
}

void CTextViewerDoc::DisplayLines(CString msg)
{
	//Begin - ����A�C
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	pMainFrame->SetStatusText(4, msg);
	//End   - ����A�C
}

void CTextViewerDoc::SetNewDirectory(CString dir)
{
	if(dir != "")
	{
		//���ܷs���ɮױ��y���|
		SetCurrentDirectory(dir);
		m_pListView->m_ScanPath = dir;

		//��s List ���ɮצC��
		m_pListView->ReadDirectoryInfo();
	}

	//�b���A�C�W��ܥثe�ؿ��W��
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	pMainFrame->SetStatusText(0, m_pListView->m_ScanPath);
}

void CTextViewerDoc::AddHistoryRecord()
{
	//Begin - ���o Menu ������
	CMenu* pMainMenu, *pRecordMenu;
	pMainMenu = AfxGetApp()->m_pMainWnd->GetMenu();
	pRecordMenu = pMainMenu->GetSubMenu(4);
	pMainMenu->Detach();
	//End   - ���o Menu ������

	CString MenuString;
	int ExistIndex = -1;
	for(unsigned int i=0; i<pRecordMenu->GetMenuItemCount(); i++)
	{
		//�M���l���ت��ĮְO��
		UINT state = pRecordMenu->GetMenuState(ID_RECORD+i+1, MF_BYCOMMAND);
		if(state & MF_CHECKED)
			pRecordMenu->CheckMenuItem(ID_RECORD+i+1, MF_UNCHECKED|MF_BYCOMMAND);
		//�P�_�O���O�_����
		pRecordMenu->GetMenuString(ID_RECORD+i+1, MenuString, MF_BYCOMMAND);
		if(MenuString == m_FileName) ExistIndex = i;
	}

	if(ExistIndex == -1)
	{
		int a = pRecordMenu->GetMenuItemCount();
		if(pRecordMenu->GetMenuItemCount() < 8+2)
		{
			//�b Menu �W�[�@�ӷs���l���
			m_MenuNumber++;
			pRecordMenu->AppendMenu(MF_STRING|MF_CHECKED, ID_RECORD+m_MenuNumber, m_FileName);
		}
		else
		{
			//�վ�O��������(�N���ª��O������)
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
		//�Į֥ثe��ܪ��ɮ�
		pRecordMenu->CheckMenuItem(ID_RECORD+ExistIndex+1, MF_CHECKED|MF_BYCOMMAND);
	}
}

void CTextViewerDoc::OpenRecordFile(UINT nID)
{
	//Begin - ���o Menu ������
	CMenu *pMainMenu, *pRecordMenu;
	pMainMenu = AfxGetApp()->m_pMainWnd->GetMenu();
	pRecordMenu = pMainMenu->GetSubMenu(4);
	pMainMenu->Detach();
	//End   - ���o Menu ������

	//Begin - �M���l���ت��ĮְO��
	for(unsigned int i=0; i<pRecordMenu->GetMenuItemCount(); i++)
	{
		UINT state = pRecordMenu->GetMenuState(ID_RECORD+i+1, MF_BYCOMMAND);
		if(state & MF_CHECKED)
			pRecordMenu->CheckMenuItem(ID_RECORD+i+1, MF_UNCHECKED|MF_BYCOMMAND);
	}
	//End   - �M���l���ت��ĮְO��

	CString MenuString;
	pRecordMenu->GetMenuString(nID, MenuString, MF_BYCOMMAND);
	pRecordMenu->CheckMenuItem(nID, MF_CHECKED|MF_BYCOMMAND);
	if( DisplayFile(MenuString) )
	{
		//���]�ɮ׸��|
		m_FileName = MenuString;

		//����ɦW��������D
		int pos = m_FileName.ReverseFind( '\\' );
		CString name = m_FileName.Mid(pos+1, m_FileName.GetLength()-pos+1);
		CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
		pMainFrame->SetWindowText(name + " - TextViewer");
	}
}
