#include <stdafx.h>
#include <vfw.h> //使用Window Video API時所需的標頭檔
#include "VidCapture.h"

CAPDRIVERCAPS CapDriverCaps;
CAPSTATUS     CapStatus;
bool isConnect = false; //判定是否已經Connect過Capture Driver了

BITMAPFILEHEADER bmfileheader;
BITMAPINFO* DibInfo;

//初始化Camera
BOOL vidcapInitHardware(HWND hwndCap)
{
	char szDeviceName[80];
	char szDeviceVersion[80];
	BOOL Error;

	//判斷是否已連接上Capture Driver,若否則進行Connect
	if( !isConnect )
	{
		// Try connecting a capture window to a capture driver(Hardware)
		Error = capDriverConnect(hwndCap, 0);
		isConnect = true;

		// Get the capabilities of the capture driver
		Error = capDriverGetCaps(hwndCap, &CapDriverCaps, sizeof(CAPDRIVERCAPS));
	}

	// Get the settings for the capture window
	Error = capGetStatus(hwndCap, &CapStatus , sizeof(CAPSTATUS));

	// Retrieves the version description of the capture driver
	capGetDriverDescription (0, szDeviceName, 
			sizeof (szDeviceName), szDeviceVersion, 
			sizeof (szDeviceVersion)); 

	//設定 capPreview 的 Frame Rate (2 Frames / Second)
	capPreviewRate(hwndCap, 500); // rate, in milliseconds

	// Determine whether a capture driver has overlay capabilities
	if( CapDriverCaps.fHasOverlay )
		capOverlay(hwndCap, TRUE);
 
	return Error;
}

//調整Dialog及裡面元件的大小
void AdjustWindowSize(HWND hwndMain,HWND hwndCap)
{
	//讀取Video Format Dialog裡面影像大小的設定值
	capGetStatus(hwndCap, &CapStatus, sizeof (CAPSTATUS));
		
	//更改Capture Window的大小,一定要加::,不然不會呼叫此API,會變成呼叫CWnd::SetWindowPos
	::SetWindowPos(hwndCap, NULL, 0, 0, CapStatus.uiImageWidth,
                   CapStatus.uiImageHeight, SWP_NOZORDER | SWP_NOMOVE);

	//更改Dialog的大小
//	::SetWindowPos(hwndMain, NULL, 0, 0, CapStatus.uiImageWidth+100,
//                   CapStatus.uiImageHeight+400, SWP_NOZORDER | SWP_NOMOVE);
}

//設定 Capture Rate -> Frames / Second
void SetCaptureRate(HWND hwndCap, float FramesPerSec)
{
	CAPTUREPARMS CaptureParms;
	//float FramesPerSec = 10.0;

	capCaptureGetSetup(hwndCap, &CaptureParms, sizeof(CAPTUREPARMS));

	//設定 Capture 的 Frame Rate 為每秒一張
	CaptureParms.dwRequestMicroSecPerFrame = (DWORD) (1.0e6 / FramesPerSec);
	capCaptureSetSetup(hwndCap, &CaptureParms, sizeof(CAPTUREPARMS));
}

//建立 Bitmap File 的檔頭資料
void InitBitmapHeader()
{
	//圖形資料大小 = 176*144*3 = 76032(bytes)
	long size = _ImageBits;

	//[1] 建立 Bitmap File Header
	//BITMAPFILEHEADER bmfileheader;
	bmfileheader.bfType = 0x4d42;
	bmfileheader.bfReserved1 = bmfileheader.bfReserved2 = 0;
	bmfileheader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
	bmfileheader.bfSize = bmfileheader.bfOffBits + size;

	//[2] 建立 Bitmap Information Header
	int cbHeaderSize = sizeof(BITMAPINFOHEADER);
	BITMAPINFOHEADER bmheader;
	bmheader.biSize          = cbHeaderSize;
	bmheader.biWidth         = _Width;
	bmheader.biHeight        = _Height;
	bmheader.biPlanes        = 1;
	bmheader.biBitCount      = 24;
	bmheader.biCompression   = 0;
	bmheader.biSizeImage     = size;
	bmheader.biXPelsPerMeter = 3780;
	bmheader.biYPelsPerMeter = 3780;
	bmheader.biClrUsed       = 0;
	bmheader.biClrImportant  = 0;

	DibInfo = (BITMAPINFO*)new char[cbHeaderSize];
	DibInfo->bmiHeader = bmheader;
}

//儲存 Bitmap 圖檔
void SaveBitmapFile(CString filename, byte* DibBits)
{
	CFile fp(filename, CFile::modeCreate|CFile::modeWrite|CFile::typeBinary);

	fp.Write(&bmfileheader, sizeof(BITMAPFILEHEADER));
	fp.Write(DibInfo, sizeof(BITMAPINFOHEADER));
	fp.Write(DibBits, _ImageBits);
	fp.Close();
}
