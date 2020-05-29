//------------------------------------------------------------------//
#define _Width     176              //���T�榡���e��
#define _Height    144              //���T�榡������
#define _ImageSize _Width*_Height   //�������v�����j�p
#define _ImageBits _Width*_Height*3 //�������v���� Bit ��
//------------------------------------------------------------------//

BOOL vidcapInitHardware(HWND hwndCap);
void AdjustWindowSize(HWND hwndMain,HWND hwndCap);
void SetCaptureRate(HWND hwndCap, float FramesPerSec);

void SaveBitmapFile(CString filename, byte* DibBits);
void InitBitmapHeader();

LRESULT PASCAL FrameCallbackProc(HWND hWnd, LPVIDEOHDR lpVHdr);
LRESULT PASCAL VideoCallbackProc(HWND hWnd, LPVIDEOHDR lpVHdr);

// CVidCap message handlers
// ========================================================================================
//  FrameCallbackProc: frame callback function 
//  hWnd:              capture window handle 
//  lpVHdr:            pointer to struct containing captured frame information 
//                     which defined in vfw.h
// ========================================================================================

// ========================================================================================
//  VideoCallbackProc: video-stream callback function 
//  hWnd:              capture window handle 
//  lpVHdr:            pointer to struct containing captured frame information 
//                     which defined in vfw.h
// ========================================================================================

// ============================== video data block header =================================
//  typedef struct videohdr_tag {
//     LPBYTE      lpData;           // pointer to locked data buffer 
//     DWORD       dwBufferLength;   // Length of data buffer 
//     DWORD       dwBytesUsed;      // Bytes actually used 
//     DWORD       dwTimeCaptured;   // Milliseconds from start of stream 
//     DWORD       dwUser;           // for client's use 
//     DWORD       dwFlags;          // assorted flags (see defines) 
//     DWORD       dwReserved[4];    // reserved for driver 
//  } VIDEOHDR, NEAR *PVIDEOHDR, FAR * LPVIDEOHDR;
// =========================================================================================
