// SkyeyeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Skyeye.h"
#include "SkyeyeDlg.h"
#include "SettingDlg.h"

#include <sys/stat.h>
#include <math.h>
#include <vfw.h>
#include "VidCapture.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

//================================== 宣告全域變數及外部變數 ==============================
HWND hwndMain;    //Main Dialog 的 Handle
HWND hwndCapture; //Capture Window 的 Handle
HWND hwndShow;    //Draw Picture Window 的 Handle
HDC  hdcShow;

CStatic*       pLED;
CStatic*       pTH;
CSliderCtrl*   pSlide;
CProgressCtrl* pBar;

//記錄影像的 Bitmap 格式
LPBITMAPINFOHEADER lpbi = new BITMAPINFOHEADER;

BOOL IsDir    = TRUE;  //存檔用的資料夾是否建立
BOOL IsSave   = FALSE; //影像是否存檔
int threshold = 30;    //偵測動作的門檻值(PSNR)
int counter   = 1;     //檔案計數
int limit     = 20;    //存檔數量的上限
int interval  = 500;   //抓取影像的時間區間(second)

byte Red[_ImageSize], Green[_ImageSize], Blue[_ImageSize];
byte oldGray[_ImageSize], newGray[_ImageSize];
byte RGBData[_ImageBits]; //存檔用

bool   InitHardware = false; //判定是否已經有驅動硬體
extern CAPDRIVERCAPS CapDriverCaps;
extern CAPSTATUS     CapStatus;
extern bool isConnect;
//========================================================================================


// capPreview() 的 Callback Function
LRESULT PASCAL FrameCallbackProc(HWND hWnd, LPVIDEOHDR lpVHdr) 
{
	//Capture Window 的大小 = 176*144
	//影像格式 RGB24
	byte RColor, GColor, BColor;
	int i, j, k, PSNR;
	double newp, oldp;
	double MSE, diff = 0.0;

	for(i=0,j=0; i<_ImageBits; i+=3,j++)
	{
		oldGray[j] = newGray[j];

		Red[j]     = lpVHdr->lpData[i];
		Green[j]   = lpVHdr->lpData[i+1];
		Blue[j]    = lpVHdr->lpData[i+2];
		newGray[j] = (byte)((Red[j]/(float)256)*0.299 + (Green[j]/(float)256)*0.587 + (Blue[j]/(float)256)*0.114)*256;

		newp = (double)(int)newGray[j];
		oldp = (double)(int)oldGray[j];
		diff += (newp - oldp)*(newp - oldp);
	}
	
	MSE = diff / 25344.0;
	if(MSE == 0.0)
		MSE = 0.00001;

	PSNR = (int)(10 * log10( 255*255/MSE ));
	pBar->SetPos(40-PSNR);

	if(PSNR <= threshold)
	{
        //[1] 偵測到動作
		if(IsDir && IsSave && (counter<=limit))
		{
			memcpy(RGBData, lpVHdr->lpData, _ImageBits);

			//依目前時間製作檔名
			CTime t = CTime::GetCurrentTime();
			CString str;
			str.Format("%s", t.Format("%b%d_%H_%M_%S"));
			CString filename;
			filename.Format("Catch\\[%3d]", counter);
			filename += str + ".bmp";

			//開始存檔
			SaveBitmapFile(filename, RGBData);
			counter++;
		}

		//LED -> Red
		CBitmap bitmap;
		bitmap.LoadBitmap(IDB_BITMAP_RED);
		pLED->SetBitmap((HBITMAP)bitmap);
		bitmap.DeleteObject();

		for(i=0,k=_Height-1; i<_Height; i++,k--)
		{
			for(int j=0; j<_Width; j++)
			{
				RColor = Red[i*_Width+j];
				GColor = Green[i*_Width+j];
				BColor = Blue[i*_Width+j];
				SetPixel(hdcShow, j, k, RGB(BColor, GColor, RColor));
			}
		}
	}
	else
	{
        //[2] 未偵測到動作
		//LED -> Green
		CBitmap bitmap;
		bitmap.LoadBitmap(IDB_BITMAP_GREEN);
		pLED->SetBitmap((HBITMAP)bitmap);
		bitmap.DeleteObject();
	}

	return (LRESULT) TRUE; 
}

// capCaptureSequenceNoFile() 的 Callback Function
LRESULT PASCAL VideoCallbackProc(HWND hWnd, LPVIDEOHDR lpVHdr)
{
	byte RColor, GColor, BColor;
	int i, j, k;

	for(i=0,j=0; i<_ImageBits; i+=3,j++)
	{
		Red[j]   = lpVHdr->lpData[i];
		Green[j] = lpVHdr->lpData[i+1];
		Blue[j]  = lpVHdr->lpData[i+2];
	}
	
	for(i=0,k=_Height-1; i<_Height; i++,k--)
	{
		for(int j=0; j<_Width; j++)
		{
			RColor = Red[i*_Width+j];
			GColor = Green[i*_Width+j];
			BColor = Blue[i*_Width+j];
			SetPixel(hdcShow, j, k, RGB(BColor, GColor, RColor));
		}
	}

	return (LRESULT) TRUE ; 
}


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog

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
// CSkyeyeDlg dialog

CSkyeyeDlg::CSkyeyeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSkyeyeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSkyeyeDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_MyMenu = NULL;
}

void CSkyeyeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSkyeyeDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSkyeyeDlg, CDialog)
	//{{AFX_MSG_MAP(CSkyeyeDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_COMMAND(ID_FUNC_START, OnFuncStart)
	ON_COMMAND(ID_FUNC_STOP, OnFuncStop)
	ON_COMMAND(ID_HELP_ABOUT, OnHelpAbout)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_TH, OnReleasedcaptureSliderTh)
	ON_BN_CLICKED(IDC_BUTTON_START, OnButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_STOP, OnButtonStop)
	ON_COMMAND(ID_TOOL_SOURCE, OnToolSource)
	ON_COMMAND(ID_TOOL_FORMAT, OnToolFormat)
	ON_COMMAND(ID_TOOL_DISPLAY, OnToolDisplay)
	ON_COMMAND(ID_TOOL_SETTING, OnToolSetting)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CSkyeyeDlg message handlers

BOOL CSkyeyeDlg::OnInitDialog()
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
	
	//取得Main Dialog的Handle
	hwndMain = AfxGetApp()->m_pMainWnd->m_hWnd;

	//Begin 插入功能表
	m_MyMenu = new CMenu;
	m_MyMenu->LoadMenu(IDR_MYMENU);
	SetMenu(m_MyMenu);
	//End 插入功能表
	
	//Begin 產生 Video Capture Window 
	hwndCapture = capCreateCaptureWindow(
                   NULL,
                   WS_CHILD | WS_VISIBLE,
                   25, 60, _Width, _Height,
                   hwndMain, // parent window(Dialog)
                   1         // child window id
             );

	hwndShow = capCreateCaptureWindow(
                   NULL,
                   WS_CHILD | WS_VISIBLE,
                   210, 60, _Width, _Height,
                   hwndMain, // parent window(Dialog)
                   2         // child window id
             );

	hdcShow = ::GetDC(hwndShow);
	//End 產生 Video Capture Window

	//設定捕捉影像的 Bitmap 格式
	InitialBMPFormat();
	//初始設定子控制項
	InitSubControl();
	//填入 Bitmap File 的檔頭
	InitBitmapHeader();
	//開始進行預覽
	StartPreview();
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSkyeyeDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CSkyeyeDlg::OnPaint() 
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
HCURSOR CSkyeyeDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CSkyeyeDlg::OnClose() 
{
	//停止預覽
	StopPreview();
	delete lpbi;

	CDialog::OnClose();
}


/////////////////////////////////////////////////////////////////////////////
// 功能表的處理函式

void CSkyeyeDlg::OnFuncStart() 
{
	//開始動作偵測
	OnButtonStart();
}

void CSkyeyeDlg::OnFuncStop() 
{
	//停止動作偵測
	OnButtonStop();
}

//設定視訊來源
void CSkyeyeDlg::OnToolSource() 
{
	//判斷是否已連接上Capture Driver,若否則進行Connect
	if( !isConnect )
	{
		capDriverConnect(hwndCapture, 0);
		capDriverGetCaps(hwndCapture, &CapDriverCaps, sizeof(CAPDRIVERCAPS));
		isConnect = true;
	}

	// Video source dialog box. 
	if( CapDriverCaps.fHasDlgVideoSource )
		capDlgVideoSource(hwndCapture);
}

//設定視訊格式
void CSkyeyeDlg::OnToolFormat() 
{
	if(!isConnect)
	{
		capDriverConnect(hwndCapture, 0);
		capDriverGetCaps(hwndCapture, &CapDriverCaps, sizeof (CAPDRIVERCAPS)); 
		isConnect = true;
	}

	// Video format dialog box. 
	if (CapDriverCaps.fHasDlgVideoFormat) 
	{
		capDlgVideoFormat(hwndCapture); 
		//調整視窗大小
		AdjustWindowSize(hwndMain, hwndCapture);
	} 
}

//設定視訊顯示
void CSkyeyeDlg::OnToolDisplay() 
{
	if(!isConnect)
	{
		capDriverConnect(hwndCapture, 0);
		capDriverGetCaps(hwndCapture, &CapDriverCaps, sizeof(CAPDRIVERCAPS));
		isConnect = true;
	}

	capDlgVideoCompression( hwndCapture );

	// Video display dialog box. 
	if( CapDriverCaps.fHasDlgVideoDisplay )
		capDlgVideoDisplay(hwndCapture);
}

//設定動作偵測的參數
void CSkyeyeDlg::OnToolSetting() 
{
	CSettingDlg dlg;
	dlg.m_threshold = threshold;
	dlg.m_catch = IsSave;
	dlg.m_number = limit;
	dlg.m_interval = (float)interval;
	if(dlg.DoModal() == IDOK)
	{
		threshold = dlg.m_threshold; //動作偵測的門檻值
		IsSave = dlg.m_catch;        //抓取的影像是否存檔
		limit = dlg.m_number;        //抓取的影像的數量上限
		interval = (int)(dlg.m_interval * 1000); //設定 Frame Rate
		//capPreviewRate(hwndCapture, interval);

		CString str;
		str.Format("%d", threshold);
		pTH->SetWindowText(str);
		pSlide->SetPos(40-threshold);
	}
}

void CSkyeyeDlg::OnHelpAbout() 
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}


/////////////////////////////////////////////////////////////////////////////
// 子控制項的處理函式

void CSkyeyeDlg::OnReleasedcaptureSliderTh(NMHDR* pNMHDR, LRESULT* pResult) 
{
	threshold = 40 - pSlide->GetPos();
	CString str;
	str.Format("%d", threshold);
	pTH->SetWindowText(str);

	*pResult = 0;
}

void CSkyeyeDlg::OnButtonStart() 
{
	counter = 1;

	//LED -> Green
	CBitmap bitmap;
	bitmap.LoadBitmap(IDB_BITMAP_GREEN);
	pLED->SetBitmap((HBITMAP)bitmap);
	bitmap.DeleteObject();

	//註冊 Callback Function -> capPreview()
	capSetCallbackOnFrame(hwndCapture, FrameCallbackProc);
}

void CSkyeyeDlg::OnButtonStop() 
{
	//取消 Callback Function -> capPreview()
	capSetCallbackOnFrame(hwndCapture, NULL);

	//LED -> Gray
	CBitmap bitmap;
	bitmap.LoadBitmap(IDB_BITMAP_GRAY);
	pLED->SetBitmap((HBITMAP)bitmap);
	bitmap.DeleteObject();

	//清除視窗晝面
	RECT* r = new RECT;
	r->top = 0;
	r->left = 0;
	r->bottom = _Height;
	r->right = _Width;
	::InvalidateRect(hwndShow, r, FALSE);
	delete r;
}


/////////////////////////////////////////////////////////////////////////////
// 類別的成員函式

void CSkyeyeDlg::InitialBMPFormat()
{
	//設定捕捉影像的 Bitmap 格式
	lpbi->biSize          = sizeof(BITMAPINFOHEADER);
	lpbi->biWidth         = _Width;
	lpbi->biHeight        = _Height;
	lpbi->biPlanes        = 1;
	lpbi->biCompression   = BI_RGB; // standard RGB bitmap
	lpbi->biBitCount      = 24;     // 24 bits-per-pixel format
	lpbi->biSizeImage     = 0;
	lpbi->biXPelsPerMeter = lpbi->biYPelsPerMeter = 0;
	lpbi->biClrUsed       = lpbi->biClrImportant  = 0;

	//Begin 預設 CCD 的捕捉格式 176*144 RGB24
	capDriverConnect(hwndCapture, 0);
	capSetVideoFormat(hwndCapture, (BITMAPINFO *)lpbi, sizeof(BITMAPINFO));
	capDriverDisconnect(hwndCapture);
	//End 預設 CCD 的捕捉格式 176*144 RGB24
	isConnect = false;
}

void CSkyeyeDlg::InitSubControl() 
{
	//LED
	pLED = (CStatic*)GetDlgItem(IDC_STATIC_BALL);
	//門檻值標註
	pTH = (CStatic*)GetDlgItem(IDC_STATIC_TH);
	CString str;
	str.Format("%d", threshold);
	pTH->SetWindowText(str);
	//Slider
	pSlide = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_TH);
	pSlide->SetRange(0, 40);
	pSlide->SetLineSize(1);
	pSlide->SetPos(40-threshold);
	//Progress Bar
	pBar = (CProgressCtrl*)GetDlgItem(IDC_PROGRESS_TH);
	pBar->SetRange(0, 40);
	pBar->SetStep(1);
	pBar->SetPos(0);

	//Begin 建立存檔的資料夾 - Catch
	//
	//Begin 取得目前路徑
	char* path_buffer = new char[100];
	memset(path_buffer, 0x00, 100);
	int path_len = GetCurrentDirectory(100, path_buffer);
	CString CurrentDirectory = path_buffer;
	delete path_buffer;
	//End 取得目前路徑

	//Begin 判斷路徑是否存在
	CString TestDirectory = CurrentDirectory + "\\Catch";
	struct _stat buf;
	int ret = _stat((LPCTSTR)TestDirectory, &buf);
	if(ret == -1)
	{
		//此目錄不存在
		if( CreateDirectory(TestDirectory, NULL) == 0)
		{
			AfxMessageBox("建立資料夾發生錯誤!!");
			IsDir = FALSE;
		}
	}
	//End 判斷路徑是否存在
	//
	//End 建立存檔的資料夾 - Catch
}

void CSkyeyeDlg::StartPreview() 
{
	//先初始化 Capture Hardware
	if( !InitHardware )
	{
		if( !vidcapInitHardware(hwndCapture) )
		{
			MessageBox("無法驅動影像裝置,請檢查該裝置是否已正確安裝\n或正被其他程式使用中!",_T("錯誤"));
			return;
		}
		InitHardware = true;
	}

	//調整視窗大小
	AdjustWindowSize(hwndMain, hwndCapture);
	//開始預覽
	capPreview(hwndCapture, TRUE);
}

void CSkyeyeDlg::StopPreview() 
{
	//取消 Callback Function -> capPreview()
	capSetCallbackOnFrame(hwndCapture, NULL);
	// Disables preview
	capPreview(hwndCapture, FALSE);
}

