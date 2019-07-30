// HexConvertDlg.cpp : implementation file
//

#include "stdafx.h"
#include "HexConvert.h"
#include "HexConvertDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHexConvertDlg dialog

CHexConvertDlg::CHexConvertDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CHexConvertDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CHexConvertDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CHexConvertDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHexConvertDlg)
	DDX_Control(pDX, IDC_BUTTON_TO_HEX, m_ButtonHex);
	DDX_Control(pDX, IDC_BUTTON_TO_ASCII, m_ButtonAscii);
	DDX_Control(pDX, IDC_EDIT_HEX, m_EditHex);
	DDX_Control(pDX, IDC_EDIT_ASCII, m_EditAscii);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CHexConvertDlg, CDialog)
	//{{AFX_MSG_MAP(CHexConvertDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_EXIT, OnButtonExit)
	ON_BN_CLICKED(IDC_BUTTON_TO_HEX, OnButtonToHex)
	ON_BN_CLICKED(IDC_BUTTON_TO_ASCII, OnButtonToAscii)
	ON_EN_SETFOCUS(IDC_EDIT_ASCII, OnSetfocusEditAscii)
	ON_EN_SETFOCUS(IDC_EDIT_HEX, OnSetfocusEditHex)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, OnButtonClear)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHexConvertDlg message handlers

BOOL CHexConvertDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CHexConvertDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CHexConvertDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CHexConvertDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}


void CHexConvertDlg::OnSetfocusEditAscii()
{
	// TODO: Add your control notification handler code here
	m_ButtonAscii.EnableWindow( FALSE );
	m_ButtonHex.EnableWindow( TRUE );
}

void CHexConvertDlg::OnSetfocusEditHex()
{
	// TODO: Add your control notification handler code here
	m_ButtonHex.EnableWindow( FALSE );
	m_ButtonAscii.EnableWindow( TRUE );
}

void CHexConvertDlg::OnButtonClear() 
{
	// TODO: Add your control notification handler code here
	m_EditHex.SetSel(0, -1);
	m_EditHex.Clear();

	m_EditAscii.SetSel(0, -1);
	m_EditAscii.Clear();
}

void CHexConvertDlg::OnButtonExit()
{
	// TODO: Add your control notification handler code here
	CDialog::OnOK();
}

void CHexConvertDlg::OnButtonToHex() 
{
	// TODO: Add your control notification handler code here
	CString temp;

	m_EditAscii.GetWindowText( m_StringAscii );
	m_BufferLen = m_StringAscii.GetLength();

	m_StringHex.Empty();
	for (DWORD i=0; i<m_BufferLen; i++)
	{
		#if 0
		if ((i != 0) && ((i % 16) == 0))
		#else
		if ((i != 0) && ((i % 8) == 0))
		#endif
		{
			m_StringHex +=(_T("\r\n"));
		}

		temp.Format("%02X ", m_StringAscii.GetAt(i));
		m_StringHex += temp;
	}
	m_StringHex +=(_T("\r\n"));

	m_EditHex.SetWindowText( m_StringHex );
}

void CHexConvertDlg::OnButtonToAscii() 
{
	// TODO: Add your control notification handler code here
	m_EditHex.GetWindowText( m_StringHex );

	m_BufferLen = PrepareByteData(
					(LPCTSTR)m_StringHex,
					m_Buffer,
					MAX_BUF_LENGTH
				);
	m_Buffer[m_BufferLen] = 0;
	#if 0
	for (DWORD i=0; i<m_BufferLen; i++)
	{
		if ((m_Buffer[i] < 32) || (m_Buffer[i] > 126))
		{
			if ((m_Buffer[i] != '\r') && (m_Buffer[i] != '\n'))
			{
				m_Buffer[i] = '.';
			}
		}
	}
	#endif
	m_StringAscii = (LPCTSTR)m_Buffer;
	m_EditAscii.SetWindowText( m_StringAscii );
}


/////////////////////////////////////////////////////////////////////////////
// CHexConvertDlg member methods

#define IS_SPACE(ch) \
	((ch ==  ' ') || \
	 (ch == '\t') || \
	 (ch == '\r') || \
	 (ch == '\n'))

char *GetToken(char *pToken, char *pText)
{
	char *pCh;
	int   i;

	if (0x00 == pText[0])
	{
		pToken[0] = 0x00;
		return NULL;
	}

	/* Pass space, tab, CR and LF characters */
	for (pCh=pText; *pCh && IS_SPACE(*pCh); pCh++);

	/* Get the high and low nibble */
	for (i=0; i<2; i++)
	{
		if ( *pCh )
		{
			pToken[i] = *pCh;
			pCh++;
		}
		else
		{
			pToken[i] = 0x00;
		}
	}
	pToken[i] = 0x00;

	return pCh;
}

BYTE Hex2Dec(char c)
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

BYTE ConvertHex(char *hex)
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

UINT CHexConvertDlg::PrepareByteData(const char *inData, BYTE *outData, UINT outLen) 
{
	char *text = (char *)inData;
	char  token[16];
	UINT  count = 0;


	while ( text )
	{
		text = GetToken(token, text);
		if ( token[0] )
		{
			outData[count] = ConvertHex( token );

			#if 1  /* Convert un-ascii character to CR/LF */
			if ((outData[count] < 32) || (outData[count] > 126))
			{
				if ((outData[count] != '\r') && (outData[count] != '\n'))
				{
					outData[count++] = '\r';
					outData[count++] = '\n';
				}
				else
				{
					count++;
				}
			}
			else
			#endif
			{
				count++;
			}
		}

		if (count >= outLen) break;
	}

	return count;
}

