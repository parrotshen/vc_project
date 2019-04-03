#include <stdafx.h>
#include <vfw.h> //�ϥ�Window Video API�ɩһݪ����Y��
#include "VidCapture.h"

CAPDRIVERCAPS CapDriverCaps;
CAPSTATUS     CapStatus;
bool isConnect = false; //�P�w�O�_�w�gConnect�LCapture Driver�F

BITMAPFILEHEADER bmfileheader;
BITMAPINFO* DibInfo;

//��l��Camera
BOOL vidcapInitHardware(HWND hwndCap)
{
	char szDeviceName[80];
	char szDeviceVersion[80];
	BOOL Error;

	//�P�_�O�_�w�s���WCapture Driver,�Y�_�h�i��Connect
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

	//�]�w capPreview �� Frame Rate (2 Frames / Second)
	capPreviewRate(hwndCap, 500); // rate, in milliseconds

	// Determine whether a capture driver has overlay capabilities
	if( CapDriverCaps.fHasOverlay )
		capOverlay(hwndCap, TRUE);
 
	return Error;
}

//�վ�Dialog�θ̭����󪺤j�p
void AdjustWindowSize(HWND hwndMain,HWND hwndCap)
{
	//Ū��Video Format Dialog�̭��v���j�p���]�w��
	capGetStatus(hwndCap, &CapStatus, sizeof (CAPSTATUS));
		
	//���Capture Window���j�p,�@�w�n�[::,���M���|�I�s��API,�|�ܦ��I�sCWnd::SetWindowPos
	::SetWindowPos(hwndCap, NULL, 0, 0, CapStatus.uiImageWidth,
                   CapStatus.uiImageHeight, SWP_NOZORDER | SWP_NOMOVE);

	//���Dialog���j�p
//	::SetWindowPos(hwndMain, NULL, 0, 0, CapStatus.uiImageWidth+100,
//                   CapStatus.uiImageHeight+400, SWP_NOZORDER | SWP_NOMOVE);
}

//�]�w Capture Rate -> Frames / Second
void SetCaptureRate(HWND hwndCap, float FramesPerSec)
{
	CAPTUREPARMS CaptureParms;
	//float FramesPerSec = 10.0;

	capCaptureGetSetup(hwndCap, &CaptureParms, sizeof(CAPTUREPARMS));

	//�]�w Capture �� Frame Rate ���C��@�i
	CaptureParms.dwRequestMicroSecPerFrame = (DWORD) (1.0e6 / FramesPerSec);
	capCaptureSetSetup(hwndCap, &CaptureParms, sizeof(CAPTUREPARMS));
}

//�إ� Bitmap File �����Y���
void InitBitmapHeader()
{
	//�ϧθ�Ƥj�p = 176*144*3 = 76032(bytes)
	long size = _ImageBits;

	//[1] �إ� Bitmap File Header
	//BITMAPFILEHEADER bmfileheader;
	bmfileheader.bfType = 0x4d42;
	bmfileheader.bfReserved1 = bmfileheader.bfReserved2 = 0;
	bmfileheader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
	bmfileheader.bfSize = bmfileheader.bfOffBits + size;

	//[2] �إ� Bitmap Information Header
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

//�x�s Bitmap ����
void SaveBitmapFile(CString filename, byte* DibBits)
{
	CFile fp(filename, CFile::modeCreate|CFile::modeWrite|CFile::typeBinary);

	fp.Write(&bmfileheader, sizeof(BITMAPFILEHEADER));
	fp.Write(DibInfo, sizeof(BITMAPINFOHEADER));
	fp.Write(DibBits, _ImageBits);
	fp.Close();
}
