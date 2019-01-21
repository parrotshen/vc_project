// NetworkBoardView.cpp : implementation of the CNetworkBoardView class
//

#include "stdafx.h"
#include "MainFrm.h"
#include "NetworkBoard.h"
#include "NetworkBoardDoc.h"
#include "NetworkBoardView.h"

#include "ServerDlg.h"
#include "ClientSocket.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNetworkBoardView

IMPLEMENT_DYNCREATE(CNetworkBoardView, CEditView)

BEGIN_MESSAGE_MAP(CNetworkBoardView, CEditView)
	//{{AFX_MSG_MAP(CNetworkBoardView)
	ON_CONTROL_REFLECT(EN_UPDATE, OnUpdate)
	ON_COMMAND(ID_TOOL_CONNECT, OnToolConnect)
	ON_UPDATE_COMMAND_UI(ID_TOOL_CONNECT, OnUpdateToolConnect)
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
	m_Client = NULL;
	m_server_ip = "140.116.247.62";
	m_server_port = 7770;
	m_is_modify = TRUE;
}

CNetworkBoardView::~CNetworkBoardView()
{
	if(m_Client != NULL)
	{
		//準備與伺服器斷線
		int length = -1;
		m_Client->Send(&length, sizeof(int));
		Sleep(100);

		//關閉 Client Socket
		m_Client->Close();
		delete m_Client;
		m_Client = NULL;
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
	m_font.DeleteObject();
	m_font.CreateFont(
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
	GetEditCtrl().SetFont(&m_font, TRUE);
	//End Change Font to Fixedsys

	//Begin 與伺服器連線
	//從 Registry 中取出 Server IP
	CWinApp* pApp = AfxGetApp();
	m_server_ip = pApp->GetProfileString("NetworkBoard", "ServerIP", "140.116.247.62");

	if(m_Client == NULL)
		ConnectToServer();
	//End 與伺服器連線
}

void CNetworkBoardView::OnUpdate() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CEditView::OnInitDialog()
	// function to send the EM_SETEVENTMASK message to the control
	// with the ENM_UPDATE flag ORed into the lParam mask.
	//
	// 注意：SetWindowText()不會觸發此事件!!

	//Begin 判斷是否有連上 Server 端
	// 若沒有和 Server 端連線
	// 則不必傳送資料出去
	if(m_Client == NULL)
		return;
	//End 判斷是否有連上 Server 端

	//Begin 判斷是否要將資料傳送出去
	// 若是從 Server 端收到資料
	// 則不需要再將資料送出
	if( !m_is_modify )
	{
		//允許觸發文字內容被修改的事件
		m_is_modify = !m_is_modify;
		return;
	}
	//End 判斷是否要將資料傳送出去

	//Begin 將資料送給 Server 端
	// [1]取得 Edit Control 的文字內容的長度
	int length = GetWindowTextLength();

	// [2]取得 Edit Control 的文字內容
	HLOCAL h = GetEditCtrl().GetHandle();
	LPCTSTR lpszText = (LPCTSTR) ::LocalLock(h);
	::LocalUnlock(h);

	// [3]當文字內容有改變時，將新的內容傳給伺服器
	if(length == 0)
	{
		m_Client->Send(&length, sizeof(int));
	}
	else
	{
		int count;
		int size = 1024 - sizeof(int);
		int loop = length / size + 1;

		byte* buf = new byte[1024];
		for(int i=0; i<loop; i++)
		{
			count = i + 1;
			memset(buf, 0x00, 1024);
			memcpy(buf, &count, sizeof(int));
			memcpy(buf+sizeof(int), lpszText+i*size, size);

			m_Client->Send(buf, 1024);
			//Sleep(1);
		}
		delete buf;
	}
	//End 將資料送給 Server 端
}

/////////////////////////////////////////////////////////////////////////////
// 功能表的訊息處理函式

void CNetworkBoardView::OnToolConnect() 
{
	if(m_Client == NULL)
	{
		//連線至伺服器的對話盒
		CServerDlg dlg;
		dlg.m_ip = m_server_ip;
		dlg.m_port = m_server_port;
		if(dlg.DoModal() == IDOK)
		{
			m_server_ip = dlg.m_ip;
			m_server_port = dlg.m_port;
			ConnectToServer();
		}
	}
	else
	{
		//與伺服器斷線
		int length = -1;
		m_Client->Send(&length, sizeof(int));
		Sleep(100);

		//關閉 Client Socket
		m_Client->Close();
		delete m_Client;
		m_Client = NULL;

		AfxGetMainWnd()->SetWindowText( "NetworkBoard" );
	}
}

void CNetworkBoardView::OnUpdateToolConnect(CCmdUI* pCmdUI) 
{
	//控制 "Connect" 這按鈕是 On.. Off..
	if(m_Client == NULL)
	{
		// Socket Connect
		pCmdUI->SetCheck(0);
	}
	else
	{
		// Socket Disconnect
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

void CNetworkBoardView::ConnectToServer() 
{
	m_Client = new CClientSocket(this);
	if( !m_Client->Create() )
	{
		AfxMessageBox("Socket 建立失敗!!");
		//關閉 Client Socket
		m_Client->Close();
		delete m_Client;
		m_Client = NULL;
		return;
	}

	if( !m_Client->Connect(m_server_ip, m_server_port) )
	{
		AfxMessageBox("Socket 連線失敗!!");
		//關閉 Client Socket
		m_Client->Close();
		delete m_Client;
		m_Client = NULL;
		return;
	}

	//連線成功
	AfxGetMainWnd()->SetWindowText( "NetworkBoard - Connect" );

	//將 Server IP 寫入 Registry
	CWinApp* pApp = AfxGetApp();
	pApp->WriteProfileString("NetworkBoard", "ServerIP", m_server_ip);
}

void CNetworkBoardView::AppendMessage(LPCSTR lpszMessage)
{
	//讓 Update() 事件無效
	m_is_modify = FALSE;
	CString strTemp = lpszMessage;

	//將文字附加在後面
	int len = GetWindowTextLength();
	GetEditCtrl().SetSel(len, len);
	GetEditCtrl().ReplaceSel(strTemp);
}
