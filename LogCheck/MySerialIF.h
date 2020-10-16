#ifndef __MySerialIF_H__
#define __MySerialIF_H__

#ifdef MySerial_DLL_EXPORT
#define MySerial_DLL_API extern "C" _declspec(dllexport)
#else
#define MySerial_DLL_API extern "C" _declspec(dllimport)
#endif


#define WM_COMM_BREAK_DETECTED		WM_USER+1	// A break was detected on input.
#define WM_COMM_CTS_DETECTED		WM_USER+2	// The CTS (clear-to-send) signal changed state. 
#define WM_COMM_DSR_DETECTED		WM_USER+3	// The DSR (data-set-ready) signal changed state. 
#define WM_COMM_ERR_DETECTED		WM_USER+4	// A line-status error occurred. Line-status errors are CE_FRAME, CE_OVERRUN, and CE_RXPARITY. 
#define WM_COMM_RING_DETECTED		WM_USER+5	// A ring indicator was detected. 
#define WM_COMM_RLSD_DETECTED		WM_USER+6	// The RLSD (receive-line-signal-detect) signal changed state. 
#define WM_COMM_RXCHAR				WM_USER+7	// A character was received and placed in the input buffer. 
#define WM_COMM_RXFLAG_DETECTED		WM_USER+8	// The event character was received and placed in the input buffer.  
#define WM_COMM_TXEMPTY_DETECTED	WM_USER+9	// The last character in the output buffer was sent.  


typedef struct _tMySerialParam
{
	UINT   port_num;	// portnumber (1..4)
	UINT   baud_rate;	// baudrate
	char   parity;		// parity 
	UINT   data_bits;	// databits 
	UINT   stop_bits;	// stopbits 
} tMySerialParam;


MySerial_DLL_API BOOL MySerialIF_OpenConnection(
							CWnd           *owner,
							tMySerialParam *params
						);
MySerial_DLL_API BOOL MySerialIF_OpenConnectionEx(
							CWnd           *owner,
							UINT            port_num,
							UINT            baud_rate,
							char            parity,
							UINT            data_bits, 
							UINT            stop_bits,
							tMySerialParam *params
						);
MySerial_DLL_API void MySerialIF_CloseConnection(void);
MySerial_DLL_API BOOL MySerialIF_SendData(BYTE *data, UINT len);

#endif