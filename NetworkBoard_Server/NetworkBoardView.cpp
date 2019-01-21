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

#define  SERVER_PORT  7770

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
	m_index = 0;
}

CNetworkBoardView::~CNetworkBoardView()
{
	CUserSocket* pSocket = m_HeadSocket;

	//�M���Ҧ��s�u�� Socket
	for(int i=0; i<m_index; i++)
	{
		if(pSocket != NULL)
		{
			pSocket->Close();
			delete pSocket;
		}
		pSocket = pSocket->m_rear;	
	}

	//�M�����A���� Socket
	if(m_Server != NULL)
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
}

/////////////////////////////////////////////////////////////////////////////
// �\����T���B�z�禡

void CNetworkBoardView::OnAppRefresh() 
{
	if(m_index == 0)
	{
		AfxMessageBox("�ثe�S���ϥΪ̦b�u�W!!");
		return; 
	}

	//���o Edit Control ����r����
	int length = GetWindowTextLength();

	//���o Edit Control ����r���e
	HLOCAL h = GetEditCtrl().GetHandle();
	LPCTSTR lpszText = (LPCTSTR) ::LocalLock(h);
	::LocalUnlock(h);

	//���θ�ưe����L�Ҧ��ϥΪ�
	// +----+--------+
	// |Flag|  Data  |
	// +----+--------+
	m_CurrentSocket = NULL;
	if(length == 0)
	{
		SendToAllUser(NULL, 0);
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

			SendToAllUser((LPCTSTR)buf, 1024);
			//Sleep(1);
		}
		delete buf;
	}
}

void CNetworkBoardView::OnAppUser() 
{
	//�έp����ܥثe�s�u�H��
	m_connection.Format("Total %d User Connect!!", m_index);
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	pMainFrame->SetStatusText(0, m_connection);
}

void CNetworkBoardView::OnAppListen() 
{
	if(m_Server == NULL)
	{
		// Socket Example (20010910):
		// [1] �إ� Server Socket
		m_Server = new CServerSocket(this);
		if( !m_Server->Create(SERVER_PORT, SOCK_STREAM) )
		{
			AfxMessageBox("Socket �إߥ���!");
			m_Server->Close();
			return;
		}

		// [2] Socket Listen
		m_Server->Listen();

		m_index = 0;
		CString msg;
		msg.Format("NetworkBoard - Listen [%d]", SERVER_PORT);
		AfxGetMainWnd()->SetWindowText( msg );
	}
	else
	{
		CUserSocket* pSocket = m_HeadSocket;

		//�M���Ҧ��s�u�� Socket
		for(int i=0; i<m_index; i++)
		{
			if(pSocket != NULL)
			{
				pSocket->Close();
				delete pSocket;
			}
			pSocket = pSocket->m_rear;	
		}

		// [9] ���� Server Socket
		m_Server->Close();
		delete m_Server;
		m_Server = NULL;

		m_index = 0;
		AfxGetMainWnd()->SetWindowText( "NetworkBoard" );
	}
}

void CNetworkBoardView::OnUpdateAppListen(CCmdUI* pCmdUI) 
{
	//���� "Listen" �o���s�O On.. Off..
	if(m_Server == NULL)
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
	//�����Ӥ�󤺮e
	GetEditCtrl().SetSel(0, -1, TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// ���O�������禡

void CNetworkBoardView::ProcessingAccept()
{
	//�����ϥΪ̪��s�u
	CUserSocket* pSocket = new CUserSocket(this);
	if( !(m_Server->Accept(*pSocket)) )
	{
		pSocket->Close();
		delete pSocket;
		pSocket = NULL;
		return;
	}

	m_index++;
	if(m_index == 1)
	{
		//�O����C��
		m_HeadSocket = pSocket;
		//�O����C��
		m_TailSocket = pSocket;
	}
	else
	{
		//�N�s�� Socket ��b�᭱
		m_TailSocket->m_rear = pSocket;
		pSocket->m_front = m_TailSocket;
		m_TailSocket = pSocket;
	}

	//�N�ثe��r��ưe���s�u���ϥΪ�
	SendToClient();

	//�έp����ܥثe�s�u�H��
	OnAppUser();
}

void CNetworkBoardView::DeleteClient()
{
	if(m_index == 1)
	{
		//1 - �u���@�Ӹ`�I
		m_HeadSocket = NULL;
		m_TailSocket = NULL;
	}
	else if(m_CurrentSocket == m_HeadSocket)
	{
		//2 - �R����C��
		m_HeadSocket = m_CurrentSocket->m_rear;
		m_HeadSocket->m_front = NULL;
	}
	else if(m_CurrentSocket == m_TailSocket)
	{
		//3 - �R����C��
		m_TailSocket = m_CurrentSocket->m_front;
		m_TailSocket->m_rear = NULL;
	}
	else
	{
		//4 - �@�뱡��
		m_CurrentSocket->m_front->m_rear = m_CurrentSocket->m_rear;
		m_CurrentSocket->m_rear->m_front = m_CurrentSocket->m_front;
	}

	//�R�� Socket �`�I
	m_CurrentSocket->Close();
	delete m_CurrentSocket;
	m_CurrentSocket = NULL;
	m_index--;

	//�έp����ܥثe�s�u�H��
	OnAppUser();
}

void CNetworkBoardView::SendToClient()
{
	//���o Edit Control ����r����
	int length = GetWindowTextLength();

	//���o Edit Control ����r���e
	HLOCAL h = GetEditCtrl().GetHandle();
	LPCTSTR lpszText = (LPCTSTR) ::LocalLock(h);
	::LocalUnlock(h);

	//���θ�ưe�����w���ϥΪ�
	// +----+--------+
	// |Flag|  Data  |
	// +----+--------+
	if(length == 0)
	{
		m_TailSocket->Send(&length, sizeof(int));
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

			m_TailSocket->Send(buf, 1024);
		}
		delete buf;
	}
}

void CNetworkBoardView::SendToAllUser(LPCTSTR lpszMessage, int length)
{
	CUserSocket* pSocket = m_HeadSocket;

	//Begin �N��r��ưe���Ҧ��s�u���ϥΪ�
	for(int i=0; i<m_index; i++)
	{
		if(pSocket != m_CurrentSocket)
		{
			if(length == 0)
				pSocket->Send(&length, sizeof(int));
			else
				pSocket->Send(lpszMessage, length);
		}
		pSocket = pSocket->m_rear;
	}
	//End �N��r��ưe���Ҧ��s�u���ϥΪ�
}

void CNetworkBoardView::AppendMessage(LPCSTR lpszMessage)
{
	//�N��r���[�b�᭱
	CString strTemp = lpszMessage;

	int len = GetWindowTextLength();
	GetEditCtrl().SetSel(len, len);
	GetEditCtrl().ReplaceSel(strTemp);
}
