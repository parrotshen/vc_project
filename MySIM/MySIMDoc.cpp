// MySIMDoc.cpp : implementation of the CMySIMDoc class
//

#include "stdafx.h"
#include "MySIM.h"
#include "MySIMDoc.h"
#include "MainFrm.h"

#include "MySIMTreeView.h"
#include "MySIMListView.h"
#include "MySIMEditView.h"
#include "MySIMReaderDlg.h"
#include "MySIMCommandDlg.h"
#include "MySIMDirInfoDlg.h"
#include "MySIMFileInfoDlg.h"
#include "MySIMPinDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// Global variables

/*
*  MF
*   |-- DF_TELECOM
*   |    |-- DF_GRAPHICS
*   |    |    `-- EF_IMG
*   |    |-- EF_ADN
*   |    |-- EF_FDN
*   |    |-- EF_SMS
*   |    |-- EF_CCP
*   |    |-- EF_MSISDN
*   |    |-- EF_SMSP
*   |    |-- EF_SMSS
*   |    |-- EF_LND
*   |    |-- EF_SMSR
*   |    |-- EF_SDN
*   |    |-- EF_EXT1
*   |    |-- EF_EXT2
*   |    |-- EF_EXT3
*   |    |-- EF_BDN
*   |    `-- EF_EXT4
*   |-- DF_GSM
*   |    |-- DF_IRIDIUM
*   |    |-- DF_GLOBST
*   |    |-- DF_ICO
*   |    |-- DF_ACeS
*   |    |-- DF_MExE
*   |    |    |-- EF_MExE-ST
*   |    |    |-- EF_ORPK
*   |    |    |-- EF_ARPK
*   |    |    `-- EF_TPRPK
*   |    |-- DF_EIA
*   |    |-- DF_CTS
*   |    |-- DF_SOLSA
*   |    |    |-- EF_SAI
*   |    |    `-- EF_SLL
*   |    |-- EF_LP
*   |    |-- EF_IMSI
*   |    |-- EF_Kc
*   |    |-- EF_PLMNsel
*   |    |-- EF_HPPLMN
*   |    |-- EF_ACMmax
*   |    |-- EF_SST
*   |    |-- EF_ACM
*   |    |-- EF_GID1
*   |    |-- EF_GID2
*   |    |-- EF_PUCT
*   |    |-- EF_CBMI
*   |    |-- EF_SPN
*   |    |-- EF_CBMID
*   |    |-- EF_CBMIR
*   |    |-- EF_NIA
*   |    |-- EF_KcGPRS
*   |    |-- EF_LOCIGPRS
*   |    |-- EF_SUME
*   |    |-- EF_PLMNwAct
*   |    |-- EF_OPLMNwAct
*   |    |-- EF_HPLMNAct
*   |    |-- EF_CPBCCH
*   |    |-- EF_INVSCAN
*   |    |-- EF_BCCH
*   |    |-- EF_ACC
*   |    |-- EF_FPLMN
*   |    |-- EF_LOCI
*   |    |-- EF_AD
*   |    |-- EF_PHASE
*   |    |-- EF_VGCS
*   |    |-- EF_VGCSS
*   |    |-- EF_VBS
*   |    |-- EF_VBSS
*   |    |-- EF_eMLPP
*   |    |-- EF_AAeM
*   |    `-- EF_ECC
*   |-- DF_IS-41
*   |-- DF_FP-CTS
*   |-- EF_ELP
*   `-- EF_ICCID
*/
tSimFileSys g_fileSys[] = {
	/* item  type           fid           name            jump info */
	{  NULL, ITEM_MF,     { 0x3F, 0x00 }, "MF",           96,  NULL  },  /* 0 */
	{  NULL, ITEM_DF_1st, { 0x7F, 0x10 }, "DF(TELECOM)",  20,  NULL  },
	{  NULL, ITEM_DF_2nd, { 0x5F, 0x50 }, "DF(GRAPHICS)",  3,  NULL  },
	{  NULL, ITEM_EF_3rd, { 0x4F, 0x20 }, "EF(IMG)",       0,  NULL  },
	{  NULL, ITEM_DUMMY,  { 0x3F, 0x00 }, "",              0,  NULL  },
	{  NULL, ITEM_DUMMY,  { 0x7F, 0x10 }, "",              0,  NULL  },  /* 5 */
	{  NULL, ITEM_EF_2nd, { 0x6F, 0x3A }, "EF(ADN)",       0,  NULL  },
	{  NULL, ITEM_EF_2nd, { 0x6F, 0x3B }, "EF(FDN)",       0,  NULL  },
	{  NULL, ITEM_EF_2nd, { 0x6F, 0x3C }, "EF(SMS)",       0,  NULL  },
	{  NULL, ITEM_EF_2nd, { 0x6F, 0x3D }, "EF(CCP)",       0,  NULL  },
	{  NULL, ITEM_EF_2nd, { 0x6F, 0x40 }, "EF(MSISDN)",    0,  NULL  },  /* 10 */
	{  NULL, ITEM_EF_2nd, { 0x6F, 0x42 }, "EF(SMSP)",      0,  NULL  },
	{  NULL, ITEM_EF_2nd, { 0x6F, 0x43 }, "EF(SMSS)",      0,  NULL  },
	{  NULL, ITEM_EF_2nd, { 0x6F, 0x44 }, "EF(LND)",       0,  NULL  },
	{  NULL, ITEM_EF_2nd, { 0x6F, 0x47 }, "EF(SMSR)",      0,  NULL  },
	{  NULL, ITEM_EF_2nd, { 0x6F, 0x49 }, "EF(SDN)",       0,  NULL  },  /* 15 */
	{  NULL, ITEM_EF_2nd, { 0x6F, 0x4A }, "EF(EXT1)",      0,  NULL  },
	{  NULL, ITEM_EF_2nd, { 0x6F, 0x4B }, "EF(EXT2)",      0,  NULL  },
	{  NULL, ITEM_EF_2nd, { 0x6F, 0x4C }, "EF(EXT3)",      0,  NULL  },
	{  NULL, ITEM_EF_2nd, { 0x6F, 0x4D }, "EF(BDN)",       0,  NULL  },
	{  NULL, ITEM_EF_2nd, { 0x6F, 0x4E }, "EF(EXT4)",      0,  NULL  },  /* 20 */
	{  NULL, ITEM_DUMMY,  { 0x3F, 0x00 }, "",              0,  NULL  },
	{  NULL, ITEM_DF_1st, { 0x7F, 0x20 }, "DF(GSM)",      89,  NULL  },
	{  NULL, ITEM_DF_2nd, { 0x5F, 0x30 }, "DF(IRIDIUM)",   0,  NULL  },
	{  NULL, ITEM_DUMMY,  { 0x3F, 0x00 }, "",              0,  NULL  },
	{  NULL, ITEM_DUMMY,  { 0x7F, 0x20 }, "",              0,  NULL  },  /* 25 */
	{  NULL, ITEM_DF_2nd, { 0x5F, 0x31 }, "DF(GLOBST)",    0,  NULL  },
	{  NULL, ITEM_DUMMY,  { 0x3F, 0x00 }, "",              0,  NULL  },
	{  NULL, ITEM_DUMMY,  { 0x7F, 0x20 }, "",              0,  NULL  },
	{  NULL, ITEM_DF_2nd, { 0x5F, 0x32 }, "DF(ICO)",       0,  NULL  },
	{  NULL, ITEM_DUMMY,  { 0x3F, 0x00 }, "",              0,  NULL  },  /* 30 */
	{  NULL, ITEM_DUMMY,  { 0x7F, 0x20 }, "",              0,  NULL  },
	{  NULL, ITEM_DF_2nd, { 0x5F, 0x33 }, "DF(ACeS)",      0,  NULL  },
	{  NULL, ITEM_DUMMY,  { 0x3F, 0x00 }, "",              0,  NULL  },
	{  NULL, ITEM_DUMMY,  { 0x7F, 0x20 }, "",              0,  NULL  },
	{  NULL, ITEM_DF_2nd, { 0x5F, 0x3C }, "DF(MExE)",     39,  NULL  },  /* 35 */
	{  NULL, ITEM_EF_3rd, { 0x4F, 0x40 }, "EF(MExE-ST)",   0,  NULL  },
	{  NULL, ITEM_EF_3rd, { 0x4F, 0x41 }, "EF(ORPK)",      0,  NULL  },
	{  NULL, ITEM_EF_3rd, { 0x4F, 0x42 }, "EF(ARPK)",      0,  NULL  },
	{  NULL, ITEM_EF_3rd, { 0x4F, 0x43 }, "EF(TPRPK)",     0,  NULL  },
	{  NULL, ITEM_DUMMY,  { 0x3F, 0x00 }, "",              0,  NULL  },  /* 40 */
	{  NULL, ITEM_DUMMY,  { 0x7F, 0x20 }, "",              0,  NULL  },
	{  NULL, ITEM_DF_2nd, { 0x5F, 0x40 }, "DF(EIA)",       0,  NULL  },
	{  NULL, ITEM_DUMMY,  { 0x3F, 0x00 }, "",              0,  NULL  },
	{  NULL, ITEM_DUMMY,  { 0x7F, 0x20 }, "",              0,  NULL  },
	{  NULL, ITEM_DF_2nd, { 0x5F, 0x60 }, "DF(CTS)",       0,  NULL  },  /* 45 */
	{  NULL, ITEM_DUMMY,  { 0x3F, 0x00 }, "",              0,  NULL  },
	{  NULL, ITEM_DUMMY,  { 0x7F, 0x20 }, "",              0,  NULL  },
	{  NULL, ITEM_DF_2nd, { 0x5F, 0x70 }, "DF(SOLSA)",    50,  NULL  },
	{  NULL, ITEM_EF_3rd, { 0x4F, 0x30 }, "EF(SAI)",       0,  NULL  },
	{  NULL, ITEM_EF_3rd, { 0x4F, 0x31 }, "EF(SLL)",       0,  NULL  },  /* 50 */
	{  NULL, ITEM_DUMMY,  { 0x3F, 0x00 }, "",              0,  NULL  },
	{  NULL, ITEM_DUMMY,  { 0x7F, 0x20 }, "",              0,  NULL  },
	{  NULL, ITEM_EF_2nd, { 0x6F, 0x05 }, "EF(LP)",        0,  NULL  },
	{  NULL, ITEM_EF_2nd, { 0x6F, 0x07 }, "EF(IMSI)",      0,  NULL  },
	{  NULL, ITEM_EF_2nd, { 0x6F, 0x20 }, "EF(Kc)",        0,  NULL  },  /* 55 */
	{  NULL, ITEM_EF_2nd, { 0x6F, 0x30 }, "EF(PLMNsel)",   0,  NULL  },
	{  NULL, ITEM_EF_2nd, { 0x6F, 0x31 }, "EF(HPPLMN)",    0,  NULL  },
	{  NULL, ITEM_EF_2nd, { 0x6F, 0x37 }, "EF(ACMmax)",    0,  NULL  },
	{  NULL, ITEM_EF_2nd, { 0x6F, 0x38 }, "EF(SST)",       0,  NULL  },
	{  NULL, ITEM_EF_2nd, { 0x6F, 0x39 }, "EF(ACM)",       0,  NULL  },  /* 60 */
	{  NULL, ITEM_EF_2nd, { 0x6F, 0x3E }, "EF(GID1)",      0,  NULL  },
	{  NULL, ITEM_EF_2nd, { 0x6F, 0x3F }, "EF(GID2)",      0,  NULL  },
	{  NULL, ITEM_EF_2nd, { 0x6F, 0x41 }, "EF(PUCT)",      0,  NULL  },
	{  NULL, ITEM_EF_2nd, { 0x6F, 0x45 }, "EF(CBMI)",      0,  NULL  },
	{  NULL, ITEM_EF_2nd, { 0x6F, 0x46 }, "EF(SPN)",       0,  NULL  },  /* 65 */
	{  NULL, ITEM_EF_2nd, { 0x6F, 0x48 }, "EF(CBMID)",     0,  NULL  },
	{  NULL, ITEM_EF_2nd, { 0x6F, 0x50 }, "EF(CBMIR)",     0,  NULL  },
	{  NULL, ITEM_EF_2nd, { 0x6F, 0x51 }, "EF(NIA)",       0,  NULL  },
	{  NULL, ITEM_EF_2nd, { 0x6F, 0x52 }, "EF(KcGPRS)",    0,  NULL  },
	{  NULL, ITEM_EF_2nd, { 0x6F, 0x53 }, "EF(LOCIGPRS)",  0,  NULL  },  /* 70 */
	{  NULL, ITEM_EF_2nd, { 0x6F, 0x54 }, "EF(SUME)",      0,  NULL  },
	{  NULL, ITEM_EF_2nd, { 0x6F, 0x60 }, "EF(PLMNwAct)",  0,  NULL  },
	{  NULL, ITEM_EF_2nd, { 0x6F, 0x61 }, "EF(OPLMNwAct)", 0,  NULL  },
	{  NULL, ITEM_EF_2nd, { 0x6F, 0x63 }, "EF(HPLMNAct)",  0,  NULL  },
	{  NULL, ITEM_EF_2nd, { 0x6F, 0x63 }, "EF(CPBCCH)",    0,  NULL  },  /* 75 */
	{  NULL, ITEM_EF_2nd, { 0x6F, 0x64 }, "EF(INVSCAN)",   0,  NULL  },
	{  NULL, ITEM_EF_2nd, { 0x6F, 0x74 }, "EF(BCCH)",      0,  NULL  },
	{  NULL, ITEM_EF_2nd, { 0x6F, 0x78 }, "EF(ACC)",       0,  NULL  },
	{  NULL, ITEM_EF_2nd, { 0x6F, 0x7B }, "EF(FPLMN)",     0,  NULL  },
	{  NULL, ITEM_EF_2nd, { 0x6F, 0x7E }, "EF(LOCI)",      0,  NULL  },  /* 80 */
	{  NULL, ITEM_EF_2nd, { 0x6F, 0xAD }, "EF(AD)",        0,  NULL  },
	{  NULL, ITEM_EF_2nd, { 0x6F, 0xAE }, "EF(PHASE)",     0,  NULL  },
	{  NULL, ITEM_EF_2nd, { 0x6F, 0xB1 }, "EF(VGCS)",      0,  NULL  },
	{  NULL, ITEM_EF_2nd, { 0x6F, 0xB2 }, "EF(VGCSS)",     0,  NULL  },
	{  NULL, ITEM_EF_2nd, { 0x6F, 0xB3 }, "EF(VBS)",       0,  NULL  },  /* 85 */
	{  NULL, ITEM_EF_2nd, { 0x6F, 0xB4 }, "EF(VBSS)",      0,  NULL  },
	{  NULL, ITEM_EF_2nd, { 0x6F, 0xB5 }, "EF(eMLPP)",     0,  NULL  },
	{  NULL, ITEM_EF_2nd, { 0x6F, 0xB6 }, "EF(AAeM)",      0,  NULL  },
	{  NULL, ITEM_EF_2nd, { 0x6F, 0xB7 }, "EF(ECC)",       0,  NULL  },
	{  NULL, ITEM_DUMMY,  { 0x3F, 0x00 }, "",              0,  NULL  },  /* 90 */
	{  NULL, ITEM_DF_1st, { 0x7F, 0x22 }, "DF(IS-41)",     0,  NULL  },
	{  NULL, ITEM_DUMMY,  { 0x3F, 0x00 }, "",              0,  NULL  },
	{  NULL, ITEM_DF_1st, { 0x7F, 0x23 }, "DF(FP-CTS)",    0,  NULL  },
	{  NULL, ITEM_DUMMY,  { 0x3F, 0x00 }, "",              0,  NULL  },
	{  NULL, ITEM_EF_1st, { 0x2F, 0x05 }, "EF(ELP)",       0,  NULL  },  /* 95 */
	{  NULL, ITEM_EF_1st, { 0x2F, 0xE2 }, "EF(ICCID)",     0,  NULL  }
};
int g_fileSysSize = (sizeof( g_fileSys ) / sizeof( tSimFileSys ));


SCARDCONTEXT		hContext = 0;
SCARDHANDLE			hCard = 0;
unsigned long		dwActProtocol;
LPCBYTE				pbSend;
DWORD				dwSend, dwRecv, size = 64;
LPBYTE				pbRecv;
SCARD_IO_REQUEST	ioRequest;
int					retCode;
char				readerName [256];
DWORD				SendLen,
					RecvLen,
					nBytesRet;
BYTE				SendBuff[MAX_BUF_LENGTH+1],
					RecvBuff[MAX_BUF_LENGTH+1];


/////////////////////////////////////////////////////////////////////////////
// CMySIMDoc

IMPLEMENT_DYNCREATE(CMySIMDoc, CDocument)

BEGIN_MESSAGE_MAP(CMySIMDoc, CDocument)
	//{{AFX_MSG_MAP(CMySIMDoc)
	ON_COMMAND(ID_TOOL_SCARD, OnToolScard)
	ON_UPDATE_COMMAND_UI(ID_TOOL_SCARD, OnUpdateToolScard)
	ON_COMMAND(ID_TOOL_COMMAND, OnToolCommand)
	ON_UPDATE_COMMAND_UI(ID_TOOL_COMMAND, OnUpdateToolCommand)
	ON_COMMAND(ID_TOOL_CLEAN, OnToolClean)
	ON_COMMAND(ID_TOOL_TREE, OnToolTree)
	ON_UPDATE_COMMAND_UI(ID_TOOL_TREE, OnUpdateToolTree)
	ON_COMMAND(ID_TOOL_PIN, OnToolPin)
	ON_UPDATE_COMMAND_UI(ID_TOOL_PIN, OnUpdateToolPin)
	ON_COMMAND(ID_TOOL_PARAM, OnToolParam)
	ON_UPDATE_COMMAND_UI(ID_TOOL_PARAM, OnUpdateToolParam)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMySIMDoc construction/destruction

CMySIMDoc::CMySIMDoc()
{
	// TODO: add one-time construction code here
	m_pMainFrame = NULL;
	m_pTreeView  = NULL;
	m_pListView  = NULL;
	m_pEditView  = NULL;

	m_isConnected = FALSE;
	m_isShowFileTree = FALSE;
}

CMySIMDoc::~CMySIMDoc()
{
	if ( m_isConnected )
	{
		DisconnectSIM();
		m_isConnected = FALSE;
	}
}

BOOL CMySIMDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	//Begin: 取得 View
	POSITION pos = GetFirstViewPosition();
	if (pos == NULL) return FALSE;

	//[1] 取得 CDagCtrlTreeView
	m_pTreeView = (CMySIMTreeView*)GetNextView( pos );
	if (pos == NULL) return FALSE;

	//[2] 取得 CDagCtrlListView
	m_pListView = (CMySIMListView*)GetNextView( pos );
	if (pos == NULL) return FALSE;

	//[3] 取得 CDagCtrlEditView
	m_pEditView = (CMySIMEditView*)GetNextView( pos );

	// set EditView read only
	m_pEditView->GetEditCtrl().SetReadOnly();
	//End:   取得 View

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CMySIMDoc serialization

void CMySIMDoc::Serialize(CArchive& ar)
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
// CMySIMDoc diagnostics

#ifdef _DEBUG
void CMySIMDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMySIMDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMySIMDoc commands

BOOL CMySIMDoc::SaveModified() 
{
	// TODO: Add your specialized code here and/or call the base class
	//取消 詢問是否要存檔 的對話盒
	SetModifiedFlag(FALSE);

	return CDocument::SaveModified();
}

void CMySIMDoc::OnToolScard() 
{
	// TODO: Add your command handler code here
	if ( m_isConnected )
	{
		DisconnectSIM();

		m_pMainFrame->SetWindowText(_T("Disconnect - MySIM"));

		m_isShowFileTree = FALSE;
		m_isConnected = FALSE;
	}
	else
	{
		m_pMainFrame = (CMainFrame *)AfxGetMainWnd();

		if ( ConnectSIM() )
		{
			m_pMainFrame->SetWindowText(_T("Connect - MySIM"));
			m_isConnected = TRUE;
		}
		else
		{
			DisconnectSIM();
			m_pMainFrame->SetWindowText(_T("Disconnect - MySIM"));
		}
	}
}

void CMySIMDoc::OnUpdateToolScard(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck( m_isConnected );
}

void CMySIMDoc::OnToolTree() 
{
	// TODO: Add your command handler code here
	if ( m_isShowFileTree )
	{
		m_pTreeView->GetTreeCtrl().DeleteAllItems();

		m_isShowFileTree = FALSE;
	}
	else
	{
		ScanFileSystem();

		m_pTreeView->ExpandRootTree();

		m_isShowFileTree = TRUE;
	}
}

void CMySIMDoc::OnUpdateToolTree(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck( m_isShowFileTree );
	pCmdUI->Enable( m_isConnected );
}

void CMySIMDoc::OnToolCommand() 
{
	// TODO: Add your command handler code here
	CMySIMCommandDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		SendLen = PrepareByteData((LPCTSTR)dlg.m_cmdAPDU, SendBuff, MAX_BUF_LENGTH);

		m_pEditView->DumpMem("CMD APDU", SendBuff, SendLen);

		ioRequest.dwProtocol = dwActProtocol;
		ioRequest.cbPciLength = sizeof(SCARD_IO_REQUEST);
		RecvLen = MAX_BUF_LENGTH;
		retCode = SCardTransmit(
						hCard,
						&ioRequest,
						SendBuff,
						SendLen,
						NULL,
						RecvBuff,
						&RecvLen
					);
 		if (retCode != SCARD_S_SUCCESS)
		{
			m_pEditView->PrintMessage("SCardTransmit ........... ERR\r\n");
			ShowErrorCode( retCode );
			return;
		}
		m_pEditView->PrintMessage("SCardTransmit ........... OK\r\n");

		m_pEditView->DumpMem("RSP APDU", RecvBuff, RecvLen);
	}
}

void CMySIMDoc::OnUpdateToolCommand(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable( m_isConnected );
}


void CMySIMDoc::OnToolParam() 
{
	// TODO: Add your command handler code here
	PrintFileInfo( m_pTreeView->m_hCurrentItem );
}

void CMySIMDoc::OnUpdateToolParam(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable( m_isConnected );
}


void CMySIMDoc::OnToolPin() 
{
	// TODO: Add your command handler code here
#if 0
	CMySIMPinDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		
	}
#else
	AfxMessageBox( "Not supported yet !!" );
#endif
}

void CMySIMDoc::OnUpdateToolPin(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable( m_isConnected );
}


void CMySIMDoc::OnToolClean() 
{
	// TODO: Add your command handler code here
	m_pEditView->SetWindowText( NULL );
}


/////////////////////////////////////////////////////////////////////////////
// CMySIMDoc member methods

BOOL CMySIMDoc::ConnectSIM()
{
	int   retCode, i, j;
	char *rName;

	/* [1] Establish context */
	retCode = SCardEstablishContext(
					SCARD_SCOPE_USER,
					NULL,
					NULL,
					&hContext
				);
	if (retCode != SCARD_S_SUCCESS)
	{
		m_pEditView->PrintMessage("SCardEstablishContext ... ERR\r\n");
		ShowErrorCode( retCode );
		return FALSE;
	}
	m_pEditView->PrintMessage( "SCardEstablishContext ... OK\r\n" );


	/* [2] List readers */
	retCode = SCardListReaders(
					hContext,
					NULL,
					readerName,
					&size
				);
	if (retCode != SCARD_S_SUCCESS)
	{
        m_pEditView->PrintMessage("SCardListReaders ........ ERR\r\n");
		ShowErrorCode( retCode );
        return FALSE;
	}
	if (readerName == NULL)
	{
        m_pEditView->PrintMessage("SCardListReaders ........ No PC/SC reader is detected\r\n");
    	return FALSE;
	}
	m_pEditView->PrintMessage( "SCardListReaders ........ OK\r\n" );


	CMySIMReaderDlg dlg;
	dlg.m_readerName   = readerName;
	dlg.m_readerNumber = 0;
	if (dlg.DoModal() == IDOK)
	{
		rName = dlg.m_readerName;
		m_pEditView->PrintMessage("[Reader] %s\r\n", rName);
	}
	else
	{
		return FALSE;
	}


	/* [3] Connect */
	retCode = SCardConnect(
					hContext,
					rName,
					SCARD_SHARE_SHARED,
					SCARD_PROTOCOL_T0,
					&hCard,
					&dwActProtocol
				);
	if (retCode != SCARD_S_SUCCESS)
	{
        m_pEditView->PrintMessage("SCardConnect ............ ERR\r\n");
		ShowErrorCode( retCode );
        return FALSE;
	}
    m_pEditView->PrintMessage("SCardConnect ............ OK\r\n");



	/* [3] Status */
	DWORD ReaderLen = 100, ATRLen = 32, dwState;
	BYTE  ATRVal[40];
    retCode = SCardStatusA(
					hCard,
					rName,
					&ReaderLen,
					&dwState,
					&dwActProtocol,
					ATRVal,
					&ATRLen
				);
	if (retCode != SCARD_S_SUCCESS)
	{
        m_pEditView->PrintMessage("SCardStatusA ............ ERR\r\n");
		ShowErrorCode( retCode );
        return FALSE;
	}
    m_pEditView->PrintMessage("SCardStatusA ............ OK\r\n");


	char tmpArray[150];
	for (i=j=0; i<(int)ATRLen; i++)
	{
		sprintf(&tmpArray[j], "%02X ", ATRVal[i]);
		j += 3;
	}
	m_pEditView->PrintMessage("ATR: %s\r\n", tmpArray);
	
	switch (dwState)
	{
		case SCARD_UNKNOWN    : m_pEditView->PrintMessage("State: SCARD_UNKNOWN\r\n");    break;
		case SCARD_ABSENT     : m_pEditView->PrintMessage("State: SCARD_ABSENT\r\n");     break;
		case SCARD_PRESENT    : m_pEditView->PrintMessage("State: SCARD_PRESENT\r\n");    break;
		case SCARD_SWALLOWED  : m_pEditView->PrintMessage("State: SCARD_SWALLOWED\r\n");  break;
		case SCARD_POWERED    : m_pEditView->PrintMessage("State: SCARD_POWERED\r\n");    break;
		case SCARD_NEGOTIABLE : m_pEditView->PrintMessage("State: SCARD_NEGOTIABLE\r\n"); break;
		case SCARD_SPECIFIC   : m_pEditView->PrintMessage("State: SCARD_SPECIFIC\r\n");   break;
	}; // switch case 

	switch (dwActProtocol)
	{
		case SCARD_PROTOCOL_UNDEFINED : m_pEditView->PrintMessage("Protocol: SCARD_PROTOCOL_UNDEFINED\r\n"); break;
		case SCARD_PROTOCOL_T0        : m_pEditView->PrintMessage("Protocol: SCARD_PROTOCOL_T0\r\n");        break;		
		case SCARD_PROTOCOL_T1        : m_pEditView->PrintMessage("Protocol: SCARD_PROTOCOL_T1\r\n");        break;			
		case SCARD_PROTOCOL_RAW       : m_pEditView->PrintMessage("Protocol: SCARD_PROTOCOL_RAW\r\n");       break;			
		case SCARD_PROTOCOL_DEFAULT   : m_pEditView->PrintMessage("Protocol: SCARD_PROTOCOL_DEFAULT\r\n");   break;			
	}; // switch case


	return TRUE;
}

void CMySIMDoc::DisconnectSIM()
{
	tEfData *pData;
	tEfData *pNext;
	int  i;

	for (i=0; i<g_fileSysSize; i++)
	{
		g_fileSys[i].item = NULL;

		if ( g_fileSys[i].info )
		{
			if ( g_fileSys[i].info->paramMfDf )
			{
				free( g_fileSys[i].info->paramMfDf );
				g_fileSys[i].info->paramMfDf = NULL;
			}

			if ( g_fileSys[i].info->paramEf )
			{
				free( g_fileSys[i].info->paramEf );
				g_fileSys[i].info->paramEf = NULL;
			}

			pData = g_fileSys[i].info->dataEf;
			while ( pData )
			{
				if ( pData->data )
				{
					free( pData->data );
				}
				pNext = pData->next;
				free( pData );
				pData = pNext;
			}
			g_fileSys[i].info->dataEf = NULL;

			free( g_fileSys[i].info );
			g_fileSys[i].info = NULL;
		}
	}

	if (hCard != 0)
	{
		retCode = SCardDisconnect(hCard, SCARD_RESET_CARD);
		if (retCode != SCARD_S_SUCCESS)
		{
			m_pEditView->PrintMessage("SCardDisconnect ......... ERR\r\n");
			ShowErrorCode( retCode );
			return;
		}
		m_pEditView->PrintMessage("SCardDisconnect ......... OK\r\n");
	}


	if (hContext != 0)
	{
		retCode = SCardReleaseContext( hContext );
		if (retCode != SCARD_S_SUCCESS)
		{
			m_pEditView->PrintMessage("SCardReleaseContext ..... ERR\r\n");
			ShowErrorCode( retCode );
			return;
		}
		m_pEditView->PrintMessage( "SCardReleaseContext ..... OK\r\n" );
	}
}

void CMySIMDoc::ScanFileSystem()
{
	CWaitCursor busy;
	HTREEITEM  hMF;
	HTREEITEM  hDF1st;
	HTREEITEM  hDF2nd;
	int  jump;
	int  i;

	jump = -1;
	for (i=0; i<g_fileSysSize; i++)
	{
		BOOL selected = FALSE;

		if (i > jump)
		{
			BYTE rspApdu[MAX_BUF_LENGTH];
			int  rspApduLen;

			selected = DoSelect(g_fileSys[i].fid, rspApdu, &rspApduLen);
			if ( selected )
			{
				BYTE SW2 = rspApdu[1];

				rspApduLen = DoGetResponse(rspApdu, SW2);
				if (rspApduLen > 0)
				{
					g_fileSys[i].info = ReadFile(rspApdu, rspApduLen-2);
				}
				else
				{
					m_pEditView->PrintMessage("Fail to GET RESPONSE %s\r\n", g_fileSys[i].name);
				}
			}
			else
			{
				m_pEditView->PrintMessage(
					"Fail to select file: %s\r\n",
					g_fileSys[i].name
				);

				if (g_fileSys[i].jump > 0)
				{
					/* ignore some files ... */
					jump = g_fileSys[i].jump;
				}
			}
		}


		/* create the file system tree ... */
    	CString  name;
		name.Format(
			"%02X%02X %s",
			g_fileSys[i].fid[0],
			g_fileSys[i].fid[1],
			g_fileSys[i].name
		);

		switch ( g_fileSys[i].type )
		{
			case ITEM_MF:
				g_fileSys[i].item = m_pTreeView->SetupRootItem(
										name,
										selected
									);
				hMF = g_fileSys[i].item;
				break;
			case ITEM_DF_1st:
				g_fileSys[i].item = m_pTreeView->AddDirectoryItem(
										name,
										hMF,
										selected
									);
				hDF1st = g_fileSys[i].item;
				break;
			case ITEM_DF_2nd:
				g_fileSys[i].item = m_pTreeView->AddDirectoryItem(
										name,
										hDF1st,
										selected
									);
				hDF2nd = g_fileSys[i].item;
				break;
			case ITEM_EF_1st:
				g_fileSys[i].item = m_pTreeView->AddFileItem(
										name,
										hMF,
										selected
									);
				break;
			case ITEM_EF_2nd:
				g_fileSys[i].item = m_pTreeView->AddFileItem(
										name,
										hDF1st,
										selected
									);
				break;
			case ITEM_EF_3rd:
				g_fileSys[i].item = m_pTreeView->AddFileItem(
										name,
										hDF2nd,
										selected
									);
				break;
			case ITEM_DUMMY:
				/* ignore the dummy file */
				break;
			case ITEM_UNKNOWN:
			default:
				m_pEditView->PrintMessage("Unknown file type: %s\r\n", g_fileSys[i].name);
		}
	}
}

int CMySIMDoc::SendCommandAPDU(BYTE *cmdApdu, int cmdApduLen, BYTE *rspApdu, int rspApduLen)
{
	int  swIndex;

	SendLen = cmdApduLen;
	memcpy(SendBuff, cmdApdu, SendLen);

	//m_pEditView->DumpMem("CMD APDU", SendBuff, SendLen);
	ioRequest.dwProtocol = dwActProtocol;
	ioRequest.cbPciLength = sizeof(SCARD_IO_REQUEST);
	RecvLen = rspApduLen;
	retCode = SCardTransmit(
					hCard,
					&ioRequest,
					SendBuff,
					SendLen,
					NULL,
					RecvBuff,
					&RecvLen
				);
 	if (retCode != SCARD_S_SUCCESS)
	{
        m_pEditView->PrintMessage("SCardTransmit ........... ERR\r\n");
		ShowErrorCode( retCode );
        return 0;
	}
	//m_pEditView->PrintMessage("SCardTransmit ........... OK\r\n");

	if (RecvLen < 2)
	{
		m_pEditView->PrintMessage("incorrect response length %d\r\n", RecvLen);
		return 0;
	}

	swIndex = RecvLen - 2;
	if ((0x90 != RecvBuff[swIndex]) &&
		(0x9F != RecvBuff[swIndex]) &&
		(0x61 != RecvBuff[swIndex]))
	{
		m_pEditView->PrintMessage("SW1(%02X) SW2(%02X)\r\n", RecvBuff[swIndex], RecvBuff[swIndex+1]);
        //m_pEditView->PrintMessage("RSP: Fail to send command 0x%02X\r\n", cmdApdu[1]);
		//m_pEditView->DumpMem("SW1-SW2", RecvBuff, RecvLen);
        return 0;
	}

	if ( rspApdu )
	{
		memcpy(rspApdu, RecvBuff, RecvLen);
	}
	return RecvLen;
}

BOOL CMySIMDoc::DoSelect(BYTE *fid, BYTE *rspApdu, int *rspApduLen)
{
	BYTE cmdApdu[8];
	int  cmdApduLen;

	cmdApdu[0] = 0xA0;  /* CLA (GSM) */
	cmdApdu[1] = 0xA4;  /* INS (SELECT) */
	cmdApdu[2] = 0x00;  /* P1 */
	cmdApdu[3] = 0x00;  /* P2 */
	cmdApdu[4] = 0x02;
	cmdApdu[5] = fid[0];
	cmdApdu[6] = fid[1];
	cmdApduLen = 7;

	(*rspApduLen) = SendCommandAPDU(cmdApdu, cmdApduLen, rspApdu, MAX_BUF_LENGTH);
	return (((*rspApduLen) > 0) ? TRUE : FALSE);
}

int CMySIMDoc::DoGetResponse(BYTE *rspApdu, int rspApduLenMax)
{
	BYTE cmdApdu[8];
	int  cmdApduLen;
	int  rspApduLen;

	cmdApdu[0] = 0xA0;  /* CLA (GSM) */
	cmdApdu[1] = 0xC0;  /* INS (SELECT) */
	cmdApdu[2] = 0x00;  /* P1 */
	cmdApdu[3] = 0x00;  /* P2 */
	cmdApdu[4] = rspApduLenMax;
	cmdApduLen = 5;

	rspApduLen = SendCommandAPDU(cmdApdu, cmdApduLen, rspApdu, MAX_BUF_LENGTH);
	return rspApduLen;
}

int CMySIMDoc::DoReadBinary(BYTE *rspApdu, int rspApduLenMax)
{
	BYTE cmdApdu[8];
	int  cmdApduLen;
	int  rspApduLen;

	cmdApdu[0] = 0xA0;  /* CLA (GSM) */
	cmdApdu[1] = 0xB0;  /* INS (READ BINARY) */
	cmdApdu[2] = 0x00;  /* P1 */
	cmdApdu[3] = 0x00;  /* P2 */
	cmdApdu[4] = rspApduLenMax;
	cmdApduLen = 5;

	rspApduLen = SendCommandAPDU(cmdApdu, cmdApduLen, rspApdu, MAX_BUF_LENGTH);
	return rspApduLen;
}

int CMySIMDoc::DoReadRecord(BYTE recNum, BYTE *rspApdu, int rspApduLenMax)
{
	BYTE cmdApdu[8];
	int  cmdApduLen;
	int  rspApduLen;

	cmdApdu[0] = 0xA0;  /* CLA (GSM) */
	cmdApdu[1] = 0xB2;  /* INS (READ BINARY) */
	cmdApdu[2] = recNum;  /* P1 */
	cmdApdu[3] = 0x04;  /* P2 */
	cmdApdu[4] = rspApduLenMax;
	cmdApduLen = 5;

	rspApduLen = SendCommandAPDU(cmdApdu, cmdApduLen, rspApdu, MAX_BUF_LENGTH);
	return rspApduLen;
}


void CMySIMDoc::ShowErrorCode(LONG code)
{
	CString msg;

	switch ( code )
	{
		case SCARD_E_CANCELLED:
			msg = "The action was canceled by an CasCancel request.";
			break;
		case SCARD_E_CANT_DISPOSE:
			msg = "The system could not dispose of the media in the requested manner.";
			break;
		case SCARD_E_CARD_UNSUPPORTED:
			msg = "The smart card does not meet minimal requirements for support.";
			break;
		case SCARD_E_DUPLICATE_READER:
			msg = "The reader driver didn't produce a unique reader name.";
			break;
		case SCARD_E_INSUFFICIENT_BUFFER:
			msg = "The data buffer for returned data is too small for the returned data.";
			break;
		case SCARD_E_INVALID_ATR:
			msg = "An ATR string obtained from the registry is not a valid ATR string.";
			break;
		case SCARD_E_INVALID_HANDLE:
			msg = "The supplied handle was invalid.";
			break;
		case SCARD_E_INVALID_PARAMETER:
			msg = "One or more of the supplied parameters could not be properly interpreted.";
			break;
		case SCARD_E_INVALID_TARGET:
			msg = "Registry startup information is missing or invalid.";
			break;
		case SCARD_E_INVALID_VALUE:
			msg = "One or more of the supplied parameter values could not be properly interpreted.";
			break;
		case SCARD_E_NOT_READY:
			msg = "The reader or card is not ready to accept commands.";
			break;
		case SCARD_E_NOT_TRANSACTED:
			msg = "An attempt was made to end a non-existent transaction.";
			break;
		case SCARD_E_NO_MEMORY:
			msg = "Not enough memory available to complete this command.";
			break;
		case SCARD_E_NO_SERVICE:
			msg = "The smart card resource manager is not running.";
			break;
		case SCARD_E_NO_SMARTCARD:
			msg = "The operation requires a smart card, but no smart card is currently in the device";
			break;
		case SCARD_E_PCI_TOO_SMALL:
			msg = "The PCI receive buffer was too small";
			break;
		case SCARD_E_PROTO_MISMATCH:
			msg = "The requested protocols are incompatible with the protocol currently in use with the card";
			break;
		case SCARD_E_READER_UNAVAILABLE:
			msg = "The specified reader is not currently available for use.";
			break;
		case SCARD_E_READER_UNSUPPORTED:
			msg = "The reader driver does not meet minimal requirements for support.";
			break;
		case SCARD_E_SERVICE_STOPPED:
			msg = "The smart card resource manager has shut down.";
			break;
		case SCARD_E_SHARING_VIOLATION:
			msg = "The smart card cannot be accessed because of other outstanding connections.";
			break;
		case SCARD_E_SYSTEM_CANCELLED:
			msg = "The action was canceled by the system, presumably to log off or shut down.";
			break;
		case SCARD_E_TIMEOUT:
			msg = "The user-specified timeout value has expired.";
			break;
		case SCARD_E_UNKNOWN_CARD:
			msg = "The specified smart card name is not recognized.";
			break;
		case SCARD_E_UNKNOWN_READER:
			msg = "The specified reader name is not recognized.";
			break;
		case SCARD_F_COMM_ERROR:
			msg = "An internal communications error has been detected.";
			break;
		case SCARD_F_INTERNAL_ERROR:
			msg = "An internal consistency check failed.";
			break;
		case SCARD_F_UNKNOWN_ERROR:
			msg = "An internal error has been detected, but the source is unknown.";
			break;
		case SCARD_F_WAITED_TOO_LONG:
			msg = "An internal consistency timer has expired.";
			break;
		case SCARD_S_SUCCESS:
			msg = "No error was encountered.";
			break;
		case SCARD_W_REMOVED_CARD:
			msg = "The smart card has been removed, so that further communication is not possible.";
			break;
		case SCARD_W_RESET_CARD:
			msg = "The smart card has been reset, so any shared state information is invalid.";
			break;
		case SCARD_W_UNPOWERED_CARD:
			msg = "Power has been removed from the smart card, so that further communication is not possible.";
			break;
		case SCARD_W_UNRESPONSIVE_CARD:
			msg = "The smart card is not responding to a reset.";
			break;
		case SCARD_W_UNSUPPORTED_CARD:
			msg = "The reader cannot communicate with the card, due to ATR string configuration conflicts.";
			break;
		default:
			msg.Format("Unknow error code 0x%08X.", code);
			//msg = "Unknow error code";
	}

	m_pEditView->PrintMessage( " -> " );
	m_pEditView->PrintMessage( msg );
	m_pEditView->PrintMessage( "\r\n" );
}

UINT CMySIMDoc::PrepareByteData(const char *inData, BYTE *outData, UINT outLen) 
{
	UINT  count = 0;
	char  token[16];
	char *text = (char *)inData;


	while ( *text )
	{
		text = GetToken(token, text);
		if ( token[0] )
		{
			outData[count++] = ConvertHex( token );
		}
	}

	return count;
}


#define IS_SPACE(ch)   ((ch == ' ') || (ch == '\t') || (ch == '\r') || (ch == '\n'))

char* CMySIMDoc::GetToken(char *token, char *line)
{
	char *p1; /* pointer to token */
	char *p2; /* pointer to line  */

	if ( !*line )
	{
		token[0] = 0x00;
		return NULL;
	}

	/* Pass space, tab, CR and LF characters */
	for (p2=line; *p2 && IS_SPACE(*p2); p2++);

	/* Get the separation token */
	for (p1=token; *p2 && !IS_SPACE(*p2); p2++)
	{
		*p1++ = *p2;
	}
	*p1 = 0x00;

	return p2;
}

BYTE CMySIMDoc::Hex2Dec(char c)
{
	int retval = 0;

	switch ( c )
	{
		case '1':
			retval = 1;
			break;
		case '2':
			retval = 2;
			break;
		case '3':
			retval = 3;
			break;
		case '4':
			retval = 4;
			break;
		case '5':
			retval = 5;
			break;
		case '6':
			retval = 6;
			break;
		case '7':
			retval = 7;
			break;
		case '8':
			retval = 8;
			break;
		case '9':
			retval = 9;
			break;
		case 'A':
		case 'a':
			retval = 10;
            break;
		case 'B':
		case 'b':
			retval = 11;
			break;
        case 'C':
		case 'c':
			retval = 12;
			break;
		case 'D':
		case 'd':
			retval = 13;
			break;
		case 'E':
		case 'e':
			retval = 14;
			break;
		case 'F':
		case 'f':
			retval = 15;
			break;
		default:
			retval = 0;
	}
	
	return retval;
}

BYTE CMySIMDoc::ConvertHex(char *hex)
{
	BYTE acc = 0;

	if (strlen(hex) != 2)
	{
		return acc;
	}

	acc  = Hex2Dec( hex[0] ) * 16;
	acc += Hex2Dec( hex[1] );

	return acc;
}

#define TYPE_OF_FILE(n) { \
	switch (n) \
	{ \
		case 0x00: \
			m_pEditView->PrintMessage(" 7      : Type          : RFU (0x00)\r\n"); \
			break; \
		case 0x01: \
			m_pEditView->PrintMessage(" 7      : Type          : MF (0x01)\r\n"); \
			break; \
		case 0x02: \
			m_pEditView->PrintMessage(" 7      : Type          : DF (0x02)\r\n"); \
			break; \
		case 0x04: \
			m_pEditView->PrintMessage(" 7      : Type          : EF (0x04)\r\n"); \
			break; \
		default: \
			m_pEditView->PrintMessage(" 7      : Type          : unknown (0x%02X)\r\n", n); \
	} \
}

#define STRUCTURE_OF_FILE(n) { \
	switch (n) \
	{ \
		case 0x00: \
			m_pEditView->PrintMessage("14      : Structure  : transparent (0x00)\r\n"); \
			break; \
		case 0x01: \
			m_pEditView->PrintMessage("14      : Structure  : linear fixed (0x01)\r\n"); \
			break; \
		case 0x03: \
			m_pEditView->PrintMessage("14      : Structure  : cyclic (0x03)\r\n"); \
			break; \
		default: \
			m_pEditView->PrintMessage("14      : Structure  : unknown (0x%02X)\r\n", n); \
	} \
}

void CMySIMDoc::PrintFileInfo(HTREEITEM hFile)
{
	tSimFileSys *pFileSys;
	tMfDfParam  *paramMfDf;
	tEfParam    *paramEf;

	pFileSys = FindFile( hFile );
	if (( pFileSys ) && ( pFileSys->info ))
	{
		paramMfDf = pFileSys->info->paramMfDf;
		if ( paramMfDf )
		{
			CMySIMDirInfoDlg dlg;

			dlg.m_pParamMfDf = paramMfDf;
			dlg.DoModal();
		}

		paramEf = pFileSys->info->paramEf;
		if ( paramEf )
		{
			CMySIMFileInfoDlg dlg;

			dlg.m_pParamEf = paramEf;
			dlg.DoModal();
		}		
	}


#if 0
	if (type == 0)
	{
		/* MF or DF */
		m_pEditView->PrintMessage("File information of %s\r\n", name);
		m_pEditView->PrintMessage(" 1 - 2  : RFU           : 0x%02X 0x%02X\r\n", rspApdu[0], rspApdu[1]);
		WORD size;
		size  = (rspApdu[2] << 8);
		size |= (rspApdu[3]);
		m_pEditView->PrintMessage(" 3 - 4  : Memory Size   : %d\r\n", size);
		m_pEditView->PrintMessage(" 5 - 6  : FID           : 0x%02X 0x%02X\r\n", rspApdu[4], rspApdu[5]);
		TYPE_OF_FILE( rspApdu[6] );
		m_pEditView->PrintMessage(" 8 - 12 : RFU           : 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X\r\n",
			rspApdu[7],
			rspApdu[8],
			rspApdu[9],
			rspApdu[10],
			rspApdu[11]
		);
		m_pEditView->PrintMessage("13      : Length        : %d\r\n", rspApdu[12]);
		m_pEditView->PrintMessage("14      : Characterstic : 0x%02X\r\n", rspApdu[13]);
		m_pEditView->PrintMessage("15      : DF#           : %d\r\n", rspApdu[14]);
		m_pEditView->PrintMessage("16      : EF#           : %d\r\n", rspApdu[15]);
		m_pEditView->PrintMessage("17      : CHV#          : %d\r\n", rspApdu[16]);
		m_pEditView->PrintMessage("18      : RFU           : 0x%02X\r\n", rspApdu[17]);
		m_pEditView->PrintMessage("19      : CHV1          : 0x%02X\r\n", rspApdu[18]);
		m_pEditView->PrintMessage("20      : Unblock CHV1  : 0x%02X\r\n", rspApdu[19]);
		m_pEditView->PrintMessage("21      : CHV2          : 0x%02X\r\n", rspApdu[20]);
		m_pEditView->PrintMessage("22      : Unblock CHV2  : 0x%02X\r\n", rspApdu[21]);
		m_pEditView->PrintMessage("\r\n");
	}
	else
	{
		/* EF */
		m_pEditView->PrintMessage(" 1 - 2  : RFU        : 0x%02X 0x%02X\r\n", rspApdu[0], rspApdu[1]);
		WORD size;
		size  = (rspApdu[2] << 8);
		size |= (rspApdu[3]);
		m_pEditView->PrintMessage(" 3 - 4  : Size       : %d\r\n", size);
		m_pEditView->PrintMessage(" 5 - 6  : FID        : 0x%02X 0x%02X\r\n", rspApdu[4], rspApdu[5]);
		TYPE_OF_FILE( rspApdu[6] );
		m_pEditView->PrintMessage(" 8      : RFU        : 0x%02X\r\n", rspApdu[7]);
		m_pEditView->PrintMessage(" 9 - 11 : Access     : 0x%02X 0x%02X 0x%02X\r\n",
			rspApdu[8],
			rspApdu[9],
			rspApdu[10]
		);
		m_pEditView->PrintMessage("12      : Status     : 0x%02X\r\n", rspApdu[11]);
		m_pEditView->PrintMessage("13      : Length     : %d\r\n", rspApdu[12]);
		STRUCTURE_OF_FILE( rspApdu[13] );
		m_pEditView->PrintMessage("15      : Record Len : %d\r\n", rspApdu[14]);
		m_pEditView->PrintMessage("\r\n");
	}
#endif
}

void CMySIMDoc::DumpEfData(HTREEITEM hFile)
{
	tSimFileSys *pFileSys;

	pFileSys = FindFile( hFile );
	if ( pFileSys )
	{
		switch ( pFileSys->type )
		{
			case ITEM_MF:
				break;
			case ITEM_DF_1st:
			case ITEM_DF_2nd:
				break;
			case ITEM_EF_1st:
			case ITEM_EF_2nd:
			case ITEM_EF_3rd:
				if ( pFileSys->info )
				{
					m_pListView->DisplayEF( pFileSys );

#if 0
					if ( pFileSys->info->dataEf )
					{
						if (pFileSys->info->dataEf->dataLen > 0)
						{
							CString  name;

							//m_pEditView->PrintMessage("%s ...\r\n", pFileSys->name);
							name.Format("%s", pFileSys->name);
							m_pEditView->DumpMem(
								name,
								pFileSys->info->dataEf->data,
								pFileSys->info->dataEf->dataLen
							);
						}
					}
#endif
				}
				else
				{
					m_pListView->GetListCtrl().DeleteAllItems();
				}
				break;
			default:
				;
		}
	}
}


tFileInfo *CMySIMDoc::ReadFile(BYTE *rspApdu, int rspApduLen)
{
	tFileInfo  *pFileInfo  = NULL;
	tMfDfParam *pMfDfParam = NULL;
	tEfParam   *pEfParam   = NULL;

	pFileInfo = (tFileInfo *)malloc( sizeof(tFileInfo) );
    if ( pFileInfo )
	{
		pFileInfo->paramMfDf = NULL;
		pFileInfo->paramEf   = NULL;
		pFileInfo->dataEf    = NULL;

		switch ( rspApdu[6] )
		{
			case FILE_TYPE_MF:
			case FILE_TYPE_DF:
				pMfDfParam = (tMfDfParam *)malloc( sizeof(tMfDfParam) );
				if ( pMfDfParam )
				{
					//memory size
					pMfDfParam->size  = (rspApdu[2] << 8);
					pMfDfParam->size |= (rspApdu[3]);
					//file ID
					pMfDfParam->fid[0] = rspApdu[4];
					pMfDfParam->fid[1] = rspApdu[5];
					//file type
					pMfDfParam->type = rspApdu[6];
					//CHV1 enable/disable
					pMfDfParam->chv1Enable = ((rspApdu[13] & 0x80) ? FALSE : TRUE);
					//number of DFs
					pMfDfParam->numDf = rspApdu[14];
					//number of EFs
					pMfDfParam->numDf = rspApdu[15];
					//number of CHVs
					pMfDfParam->numChv = rspApdu[16];
					//CHV1 secret code initial
					pMfDfParam->chv1Remain = ((rspApdu[18] & 0x80) ? TRUE : FALSE);
					//CHV1 false remain number
					pMfDfParam->chv1Remain = (rspApdu[18] & 0x0F);
					//Unblock CHV1 secret code initial
					pMfDfParam->chv1RemainUnblock = ((rspApdu[19] & 0x80) ? TRUE : FALSE);
					//Unblock CHV1 false remain number
					pMfDfParam->chv1RemainUnblock = (rspApdu[19] & 0x0F);
					//CHV2 secret code initial
					pMfDfParam->chv2Remain = ((rspApdu[20] & 0x80) ? TRUE : FALSE);
					//CHV2 false remain number
					pMfDfParam->chv2Remain = (rspApdu[20] & 0x0F);
					//Unblock CHV2 secret code initial
					pMfDfParam->chv2RemainUnblock = ((rspApdu[21] & 0x80) ? TRUE : FALSE);
					//Unblock CHV2 false remain number
					pMfDfParam->chv2RemainUnblock = (rspApdu[21] & 0x0F);
				}
				pFileInfo->paramMfDf = pMfDfParam;
				break;
		
			case FILE_TYPE_EF:
				pEfParam = (tEfParam *)malloc( sizeof(tEfParam) );
				if ( pEfParam )
				{
					tEfData *pNext = NULL;
					tEfData *pData = NULL;
					int i;
		
					//file size
					pEfParam->size  = (rspApdu[2] << 8);
					pEfParam->size |= (rspApdu[3]);
					//file ID
					pEfParam->fid[0] = rspApdu[4];
					pEfParam->fid[1] = rspApdu[5];
					//file type
					pEfParam->type = rspApdu[6];
					//update condition
					pEfParam->condUpdate = (rspApdu[8] & 0x0F);
					//read condition
					pEfParam->condRead = ((rspApdu[8] & 0xF0) >> 4);
					//increase condition
					pEfParam->condIncrease = ((rspApdu[9] & 0xF0) >> 4);
					//invalidate condition
					pEfParam->condInvalidate = (rspApdu[10] & 0x0F);
					//rehabilitate condition
					pEfParam->condRehabilitate = ((rspApdu[10] & 0xF0) >> 4);
					//file structure
					pEfParam->structure = rspApdu[13];
		
					switch ( pEfParam->structure )
					{
						case FILE_STRUCT_BINARY:
							//record length
							pEfParam->recLen = 0;
							//record number
							pEfParam->recNum = 0;
		
							pData = (tEfData *)malloc( sizeof(tEfData) );
							if ( pData )
							{
								pData->next    = NULL;
								pData->data    = (BYTE *)malloc( pEfParam->size );
								pData->dataLen = pEfParam->size;
								DoReadBinary(pData->data, pData->dataLen);
							}
							pFileInfo->dataEf = pData;
							break;
		
						case FILE_STRUCT_RECORD:
							//record length
							pEfParam->recLen = rspApdu[14];
							//record number
							pEfParam->recNum = (pEfParam->size / pEfParam->recLen);
		
							pNext = NULL;
							for (i=0; i<pEfParam->recNum; i++)
							{
								pData = (tEfData *)malloc( sizeof(tEfData) );
								if ( pData )
								{
									pData->next    = NULL;
									pData->data    = (BYTE *)malloc( pEfParam->recLen );
									pData->dataLen = pEfParam->recLen;
									DoReadRecord(i+1, pData->data, pData->dataLen);
								}
		
								if ( pNext )
								{
									pNext->next = pData;
								}
								else
								{
									pFileInfo->dataEf = pData;
								}
								pNext = pData;
							}
							break;
		
						default:
							;
					}
		
				}
				pFileInfo->paramEf = pEfParam;
				break;

			default:
				free( pFileInfo );
				pFileInfo = NULL;
		}
	}

	return pFileInfo;
}

tSimFileSys *CMySIMDoc::FindFile(HTREEITEM hFile)
{
	tSimFileSys *pFileSys = NULL;
	int  i;

	for (i=0; i<g_fileSysSize; i++)
	{
		if (hFile == g_fileSys[i].item )
		{
			pFileSys = &(g_fileSys[i]);
			break;
		}
	}

	return pFileSys;
}
