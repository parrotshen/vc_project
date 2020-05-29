// NetworkBoardView.cpp : implementation of the CNetworkBoardView class
//

#include "stdafx.h"
#include "MainFrm.h"
#include "NetworkBoard.h"
#include "NetworkBoardDoc.h"
#include "NetworkBoardView.h"

#include "ServerSocket.h"
#include "UserSocket.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define SERVER_PORT  7770

/////////////////////////////////////////////////////////////////////////////
// CNetworkBoardView

IMPLEMENT_DYNCREATE(CNetworkBoardView, CEditView)

BEGIN_MESSAGE_MAP(CNetworkBoardView, CEditView)
	//{{AFX_MSG_MAP(CNetworkBoardView)
	ON_COMMAND(ID_APP_LISTEN, OnAppListen)
	ON_UPDATE_COMMAND_UI(ID_APP_LISTEN, OnUpdateAppListen)
	ON_COMMAND(ID_APP_USER, OnAppUser)
	ON_COMMAND(ID_APP_REFRESH, OnAppRefresh)
	ON_COMMAND(ID_EDIT_SELECT, OnEditSelect)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CEditView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CEditView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CEditView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNetworkBoardView construction/destruction

CNetworkBoardView::CNetworkBoardView()
{
	// TODO: add construction code here
	m_Server = NULL;

	m_HeadSocket = NULL;
	m_TailSocket = NULL;
	m_CurrentSocket = NULL;
	m_Index = 0;
}

CNetworkBoardView::~CNetworkBoardView()
{
	CUserSocket* pSocket = m_HeadSocket;

	//清除所有連線的 Socket
	for (int i=0; i<(int)m_Index; i++)
	{
		if (pSocket != NULL)
		{
			pSocket->Close();
			delete pSocket;
		}
		pSocket = pSocket->m_Next;	
	}

	//清除伺服器的 Socket
	if (m_Server != NULL)
	{
		m_Server->Close();
		delete m_Server;
		m_Server = NULL;
	}
}

BOOL CNetworkBoardView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	BOOL bPreCreated = CEditView::PreCreateWindow(cs);
	//cs.style &= ~(ES_AUTOHSCROLL|WS_HSCROLL);	// Enable word-wrapping

	return bPreCreated;
}

/////////////////////////////////////////////////////////////////////////////
// CNetworkBoardView drawing

void CNetworkBoardView::OnDraw(CDC* pDC)
{
	CNetworkBoardDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CNetworkBoardView printing

BOOL CNetworkBoardView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default CEditView preparation
	return CEditView::OnPreparePrinting(pInfo);
}

void CNetworkBoardView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	// Default CEditView begin printing.
	CEditView::OnBeginPrinting(pDC, pInfo);
}

void CNetworkBoardView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	// Default CEditView end printing
	CEditView::OnEndPrinting(pDC, pInfo);
}

/////////////////////////////////////////////////////////////////////////////
// CNetworkBoardView diagnostics

#ifdef _DEBUG
void CNetworkBoardView::AssertValid() const
{
	CEditView::AssertValid();
}

void CNetworkBoardView::Dump(CDumpContext& dc) const
{
	CEditView::Dump(dc);
}

CNetworkBoardDoc* CNetworkBoardView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CNetworkBoardDoc)));
	return (CNetworkBoardDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CNetworkBoardView message handlers

void CNetworkBoardView::OnInitialUpdate() 
{
	CEditView::OnInitialUpdate();
	
	//Begin Change Font to Fixedsys
	m_Font.DeleteObject();
	m_Font.CreateFont(
		-16,                   // nHeight
		0,                     // nWidth
		0,                     // nEscapement
		0,                     // nOrientation
		FW_NORMAL,             // nWeight
		FALSE,                 // bItalic
		FALSE,                 // bUnderline
		0,                     // cStrikeOut
		DEFAULT_CHARSET,       // nCharSet
		OUT_DEFAULT_PRECIS,    // nOutPrecision
		CLIP_DEFAULT_PRECIS,   // nClipPrecision
		DEFAULT_QUALITY,       // nQuality
		DEFAULT_PITCH,         // nPitchAndFamily
		"Fixedsys"             // lpszFacename
	);
	GetEditCtrl().SetFont(&m_Font, TRUE);
	//End Change Font to Fixedsys
}

/////////////////////////////////////////////////////////////////////////////
// 功能表的訊息處理函式

void CNetworkBoardView::OnAppRefresh() 
{
	if (m_Index == 0)
	{
		AfxMessageBox("目前沒有使用者在線上!!");
		return; 
	}

	//取得 Edit Control 的文字長度
	int length = GetWindowTextLength();

	//取得 Edit Control 的文字內容
	HLOCAL h = GetEditCtrl().GetHandle();
	LPCTSTR lpszText = (LPCTSTR) ::LocalLock(h);
	::LocalUnlock(h);

	//切割資料送給其他所有使用者
	// +----+--------+
	// |Flag|  Data  |
	// +----+--------+
	m_CurrentSocket = NULL;
	if (length == 0)
	{
		SendToAllUser(NULL, 0);
	}
	else
	{
		BYTE *pData = (BYTE *)lpszText;
		BYTE  buf[1024];
		int count;
		int loop;
		int size;

		loop = (length / 1020) + (((length % 1020) > 0) ? 1 : 0);
		for (int i=0; i<loop; i++)
		{
			count = (i + 1);
			if (length >= 1020)
			{
				size = 1020;
				length -= 1020;
			}
			else
			{
				size = length;
			}
			//memset(buf, 0, 1024);
			memcpy(buf, &count, 4);
			memcpy(buf+4, pData, size);
			pData += size;

			SendToAllUser((LPCTSTR)buf, size+4);
			//Sleep(1);
		}
	}
}

void CNetworkBoardView::OnAppUser() 
{
	//統計並顯示目前連線人數
	m_Connection.Format("Total %d User Connect", m_Index);
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	pMainFrame->SetStatusText(0, m_Connection);
}

void CNetworkBoardView::OnAppListen() 
{
	if (m_Server == NULL)
	{
		// Socket Example (20010910):
		// [1] 建立 Server Socket
		m_Server = new CServerSocket(this);
		if ( !m_Server->Create(SERVER_PORT, SOCK_STREAM) )
		{
			AfxMessageBox("Socket 建立失敗!");
			m_Server->Close();
			return;
		}

		// [2] Socket Listen
		m_Server->Listen();

		m_Index = 0;
		CString msg;
		msg.Format("NetworkBoard - Listen [%d]", SERVER_PORT);
		AfxGetMainWnd()->SetWindowText( msg );
	}
	else
	{
		CUserSocket* pSocket = m_HeadSocket;

		//清除所有連線的 Socket
		for (int i=0; i<(int)m_Index; i++)
		{
			if (pSocket != NULL)
			{
				pSocket->Close();
				delete pSocket;
			}
			pSocket = pSocket->m_Next;	
		}

		// [9] 關閉 Server Socket
		m_Server->Close();
		delete m_Server;
		m_Server = NULL;

		m_Index = 0;
		AfxGetMainWnd()->SetWindowText( "NetworkBoard" );
	}
}

void CNetworkBoardView::OnUpdateAppListen(CCmdUI* pCmdUI) 
{
	//控制 "Listen" 這按鈕是 On.. Off..
	if (m_Server == NULL)
	{
		// Socket Close
		pCmdUI->SetCheck(0);
	}
	else
	{
		// Socket Open
		pCmdUI->SetCheck(1);
	}
}

void CNetworkBoardView::OnEditSelect() 
{
	//選取整個文件內容
	GetEditCtrl().SetSel(0, -1, TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// 類別的成員函式

void CNetworkBoardView::ProcessingAccept()
{
	//接收使用者的連線
	CUserSocket* pSocket = new CUserSocket(this);
	if ( !(m_Server->Accept(*pSocket)) )
	{
		pSocket->Close();
		delete pSocket;
		pSocket = NULL;
		return;
	}

	m_Index++;
	if (m_Index == 1)
	{
		//記錄串列首
		m_HeadSocket = pSocket;
		//記錄串列尾
		m_TailSocket = pSocket;
	}
	else
	{
		//將新的 Socket 串在後面
		m_TailSocket->m_Next = pSocket;
		pSocket->m_Prev = m_TailSocket;
		m_TailSocket = pSocket;
	}

	//將目前文字資料送給連線的使用者
	SendToClient();

	//統計並顯示目前連線人數
	OnAppUser();
}

void CNetworkBoardView::DeleteClient()
{
	if (m_Index == 1)
	{
		//1 - 只有一個節點
		m_HeadSocket = NULL;
		m_TailSocket = NULL;
	}
	else if (m_CurrentSocket == m_HeadSocket)
	{
		//2 - 刪除串列首
		m_HeadSocket = m_CurrentSocket->m_Next;
		m_HeadSocket->m_Prev = NULL;
	}
	else if (m_CurrentSocket == m_TailSocket)
	{
		//3 - 刪除串列尾
		m_TailSocket = m_CurrentSocket->m_Prev;
		m_TailSocket->m_Next = NULL;
	}
	else
	{
		//4 - 一般情形
		m_CurrentSocket->m_Prev->m_Next = m_CurrentSocket->m_Next;
		m_CurrentSocket->m_Next->m_Prev = m_CurrentSocket->m_Prev;
	}

	//刪除 Socket 節點
	m_CurrentSocket->Close();
	delete m_CurrentSocket;
	m_CurrentSocket = NULL;
	if (m_Index > 0) m_Index--;

	//統計並顯示目前連線人數
	OnAppUser();
}

void CNetworkBoardView::SendToClient()
{
	//取得 Edit Control 的文字長度
	int length = GetWindowTextLength();

	//取得 Edit Control 的文字內容
	HLOCAL h = GetEditCtrl().GetHandle();
	LPCTSTR lpszText = (LPCTSTR) ::LocalLock(h);
	::LocalUnlock(h);

	//切割資料送給指定的使用者
	// +----+--------+
	// |Flag|  Data  |
	// +----+--------+
	if (length == 0)
	{
		m_TailSocket->Send(&length, 4);
	}
	else
	{
		BYTE *pData = (BYTE *)lpszText;
		BYTE  buf[1024];
		int count;
		int loop;
		int size;

		loop = (length / 1020) + (((length % 1020) > 0) ? 1 : 0);
		for (int i=0; i<loop; i++)
		{
			count = (i + 1);
			if (length >= 1020)
			{
				size = 1020;
				length -= 1020;
			}
			else
			{
				size = length;
			}
			//memset(buf, 0, 1024);
			memcpy(buf, &count, 4);
			memcpy(buf+4, pData, size);
			pData += size;

			m_TailSocket->Send(buf, size+4);
		}
	}
}

void CNetworkBoardView::SendToAllUser(LPCTSTR lpszMessage, int length)
{
	CUserSocket* pSocket = m_HeadSocket;

	//Begin 將文字資料送給所有連線的使用者
	for (int i=0; i<(int)m_Index; i++)
	{
		if (pSocket != m_CurrentSocket)
		{
			if (length == 0)
			{
				pSocket->Send(&length, 4);
			}
			else
			{
				pSocket->Send(lpszMessage, length);
			}
		}
		pSocket = pSocket->m_Next;
	}
	//End 將文字資料送給所有連線的使用者
}

void CNetworkBoardView::AppendMessage(LPCSTR lpszMessage)
{
	//將文字附加在後面
	CString strTemp = lpszMessage;

	int len = GetWindowTextLength();
	GetEditCtrl().SetSel(len, len);
	GetEditCtrl().ReplaceSel(strTemp);
}

