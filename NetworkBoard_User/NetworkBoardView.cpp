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
		//�ǳƻP���A���_�u
		int length = -1;
		m_Client->Send(&length, sizeof(int));
		Sleep(100);

		//���� Client Socket
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

	//Begin �P���A���s�u
	//�q Registry �����X Server IP
	CWinApp* pApp = AfxGetApp();
	m_server_ip = pApp->GetProfileString("NetworkBoard", "ServerIP", "140.116.247.62");

	if(m_Client == NULL)
		ConnectToServer();
	//End �P���A���s�u
}

void CNetworkBoardView::OnUpdate() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CEditView::OnInitDialog()
	// function to send the EM_SETEVENTMASK message to the control
	// with the ENM_UPDATE flag ORed into the lParam mask.
	//
	// �`�N�GSetWindowText()���|Ĳ�o���ƥ�!!

	//Begin �P�_�O�_���s�W Server ��
	// �Y�S���M Server �ݳs�u
	// �h�����ǰe��ƥX�h
	if(m_Client == NULL)
		return;
	//End �P�_�O�_���s�W Server ��

	//Begin �P�_�O�_�n�N��ƶǰe�X�h
	// �Y�O�q Server �ݦ�����
	// �h���ݭn�A�N��ưe�X
	if( !m_is_modify )
	{
		//���\Ĳ�o��r���e�Q�ק諸�ƥ�
		m_is_modify = !m_is_modify;
		return;
	}
	//End �P�_�O�_�n�N��ƶǰe�X�h

	//Begin �N��ưe�� Server ��
	// [1]���o Edit Control ����r���e������
	int length = GetWindowTextLength();

	// [2]���o Edit Control ����r���e
	HLOCAL h = GetEditCtrl().GetHandle();
	LPCTSTR lpszText = (LPCTSTR) ::LocalLock(h);
	::LocalUnlock(h);

	// [3]���r���e�����ܮɡA�N�s�����e�ǵ����A��
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
	//End �N��ưe�� Server ��
}

/////////////////////////////////////////////////////////////////////////////
// �\����T���B�z�禡

void CNetworkBoardView::OnToolConnect() 
{
	if(m_Client == NULL)
	{
		//�s�u�ܦ��A������ܲ�
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
		//�P���A���_�u
		int length = -1;
		m_Client->Send(&length, sizeof(int));
		Sleep(100);

		//���� Client Socket
		m_Client->Close();
		delete m_Client;
		m_Client = NULL;

		AfxGetMainWnd()->SetWindowText( "NetworkBoard" );
	}
}

void CNetworkBoardView::OnUpdateToolConnect(CCmdUI* pCmdUI) 
{
	//���� "Connect" �o���s�O On.. Off..
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
	//�����Ӥ�󤺮e
	GetEditCtrl().SetSel(0, -1, TRUE);
}


/////////////////////////////////////////////////////////////////////////////
// ���O�������禡

void CNetworkBoardView::ConnectToServer() 
{
	m_Client = new CClientSocket(this);
	if( !m_Client->Create() )
	{
		AfxMessageBox("Socket �إߥ���!!");
		//���� Client Socket
		m_Client->Close();
		delete m_Client;
		m_Client = NULL;
		return;
	}

	if( !m_Client->Connect(m_server_ip, m_server_port) )
	{
		AfxMessageBox("Socket �s�u����!!");
		//���� Client Socket
		m_Client->Close();
		delete m_Client;
		m_Client = NULL;
		return;
	}

	//�s�u���\
	AfxGetMainWnd()->SetWindowText( "NetworkBoard - Connect" );

	//�N Server IP �g�J Registry
	CWinApp* pApp = AfxGetApp();
	pApp->WriteProfileString("NetworkBoard", "ServerIP", m_server_ip);
}

void CNetworkBoardView::AppendMessage(LPCSTR lpszMessage)
{
	//�� Update() �ƥ�L��
	m_is_modify = FALSE;
	CString strTemp = lpszMessage;

	//�N��r���[�b�᭱
	int len = GetWindowTextLength();
	GetEditCtrl().SetSel(len, len);
	GetEditCtrl().ReplaceSel(strTemp);
}
