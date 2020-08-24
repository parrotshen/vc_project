// UsimExample.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Winscard.h"

#define MAX_BUF_LENGTH  255

#define LENGTH_OF_RID   5
#define LENGTH_OF_PIX   11

#define CHV_INFO_NUM    57

#define FIRST_CLA       0x00
#define USIM_CLA        0xA0

#define EF_ICCID_LENGTH 10
#define EF_IMSI_LENGTH  9

typedef enum
{
    CHV_ALWAYS  = 0x00,
    CHV_PIN     = 0x01,
    CHV_PIN2    = 0x02,
    CHV_RFU     = 0x03,
    CHV_ADM     = 0x05,
    CHV_NEVER   = 0x07,

    CHV_UNKNOWN = 0xFF
} tChvType;

typedef struct _tResSW1SW2
{
    BYTE  SW1;
    BYTE  SW2;
} tResSW1SW2;

typedef struct _tAID
{
    BYTE RID[LENGTH_OF_RID]; //Registered application provider IDentifier
    BYTE PIX[LENGTH_OF_PIX]; //Proprietary application Identifier eXtension
} tAID;

typedef struct _tPinStatus
{
    BYTE  ps_do;
    BOOL  pin_enable;
    BYTE  pin_key_ref;
    BOOL  pin2_enable;
    BYTE  pin2_key_ref;
    BOOL  adm_enable;
    BYTE  adm_key_ref;
} tPinStatus;

typedef struct _tRecordInfo
{
    DWORD recordNum;
    DWORD recordLen;
} tRecordInfo;

typedef struct _tEFDirRecord
{
    BYTE appTempTag;   //'61'
    BYTE lenOfAppTemp; //'03'~'7F'
    BYTE appIdTag;     //'4F'
    BYTE lenOfAID;     //'01'~'10'
    tAID AIDvalue;     //for transparent and linear fixed EF
    BYTE appLabelTag;  //'50'
    BYTE appLabelLen;
    BYTE appLabelValue[256];
} tEFDirRecord;

typedef struct _tEFRsp
{
    BYTE RFU[2];
    BYTE fileSize[2];
    BYTE fileID[2];
    BYTE typeOfFile;
    BYTE byte8RFU; //for transparent and linear fixed EF
    BYTE accessCond[3];
    BYTE fileStatus;
    BYTE lengthOfFollowData;
    BYTE strucOfEF;
    BYTE lenOfRecord;
    BYTE dummy[8];
} tEFRsp;


SCARDCONTEXT        hContext = 0;
SCARDHANDLE         hCard = 0;
unsigned long       dwActProtocol;
SCARD_IO_REQUEST    ioRequest;
char                readerName[256];
DWORD               readerNameLen = 256;
DWORD               SendLen;
DWORD               RecvLen;
BYTE                SendBuff[MAX_BUF_LENGTH+1],
                    RecvBuff[MAX_BUF_LENGTH+1];

/* records of EF_DIR */
tEFDirRecord EFDirRecord[4];
DWORD        EFDirRecordNum;

/* file ID */
BYTE MF[2]         = { 0x3F, 0x00 };
BYTE EF_DIR[2]     = { 0x2F, 0x00 };
BYTE EF_ICCID[2]   = { 0x2F, 0xE2 };
/* USIM application */
BYTE EF_IMSI[2]    = { 0x6F, 0x07 };

/* PIN mapping into key reference (ETSI 102.221 Table 9.3) */
BYTE CHV_INFO[CHV_INFO_NUM][4] = {
    /* PIN */
    { 0x83, 0x01, 0x01, CHV_PIN },
    { 0x83, 0x01, 0x02, CHV_PIN },
    { 0x83, 0x01, 0x03, CHV_PIN },
    { 0x83, 0x01, 0x04, CHV_PIN },
    { 0x83, 0x01, 0x05, CHV_PIN },
    { 0x83, 0x01, 0x06, CHV_PIN },
    { 0x83, 0x01, 0x07, CHV_PIN },
    { 0x83, 0x01, 0x08, CHV_PIN },
    /* RFU */
    { 0x83, 0x01, 0x09, CHV_RFU },
    /* ADM */
    { 0x83, 0x01, 0x0A, CHV_ADM },
    { 0x83, 0x01, 0x0B, CHV_ADM },
    { 0x83, 0x01, 0x0C, CHV_ADM },
    { 0x83, 0x01, 0x0D, CHV_ADM },
    { 0x83, 0x01, 0x0E, CHV_ADM },
    /* Universal PIN */
    { 0x83, 0x01, 0x11, CHV_PIN },
    /* RFU */
    { 0x83, 0x01, 0x12, CHV_RFU },
    { 0x83, 0x01, 0x13, CHV_RFU },
    { 0x83, 0x01, 0x14, CHV_RFU },
    { 0x83, 0x01, 0x15, CHV_RFU },
    { 0x83, 0x01, 0x16, CHV_RFU },
    { 0x83, 0x01, 0x17, CHV_RFU },
    { 0x83, 0x01, 0x18, CHV_RFU },
    { 0x83, 0x01, 0x19, CHV_RFU },
    { 0x83, 0x01, 0x1A, CHV_RFU },
    { 0x83, 0x01, 0x1B, CHV_RFU },
    { 0x83, 0x01, 0x1C, CHV_RFU },
    { 0x83, 0x01, 0x1D, CHV_RFU },
    { 0x83, 0x01, 0x1E, CHV_RFU },
    /* PIN2 */
    { 0x83, 0x01, 0x81, CHV_PIN2 },
    { 0x83, 0x01, 0x82, CHV_PIN2 },
    { 0x83, 0x01, 0x83, CHV_PIN2 },
    { 0x83, 0x01, 0x84, CHV_PIN2 },
    { 0x83, 0x01, 0x85, CHV_PIN2 },
    { 0x83, 0x01, 0x86, CHV_PIN2 },
    { 0x83, 0x01, 0x87, CHV_PIN2 },
    { 0x83, 0x01, 0x88, CHV_PIN2 },
    /* RFU */
    { 0x83, 0x01, 0x89, CHV_RFU },
    /* ADM */
    { 0x83, 0x01, 0x8A, CHV_ADM },
    { 0x83, 0x01, 0x8B, CHV_ADM },
    { 0x83, 0x01, 0x8C, CHV_ADM },
    { 0x83, 0x01, 0x8D, CHV_ADM },
    { 0x83, 0x01, 0x8E, CHV_ADM },
    /* RFU */
    { 0x83, 0x01, 0x90, CHV_RFU },
    { 0x83, 0x01, 0x91, CHV_RFU },
    { 0x83, 0x01, 0x92, CHV_RFU },
    { 0x83, 0x01, 0x93, CHV_RFU },
    { 0x83, 0x01, 0x94, CHV_RFU },
    { 0x83, 0x01, 0x95, CHV_RFU },
    { 0x83, 0x01, 0x96, CHV_RFU },
    { 0x83, 0x01, 0x97, CHV_RFU },
    { 0x83, 0x01, 0x98, CHV_RFU },
    { 0x83, 0x01, 0x99, CHV_RFU },
    { 0x83, 0x01, 0x9A, CHV_RFU },
    { 0x83, 0x01, 0x9B, CHV_RFU },
    { 0x83, 0x01, 0x9C, CHV_RFU },
    { 0x83, 0x01, 0x9D, CHV_RFU },
    { 0x83, 0x01, 0x9E, CHV_RFU }
};

tPinStatus  PIN_STATUS;
BOOL  g_verbose = FALSE;

void mem_dump(BYTE *data, int size)
{
    int i;

    if (NULL == data)
    {
        printf(" NULL pointer\n\n");
    }

    for (i=0; i<size; i++)
    {
        if ((i != 0) && ((i % 16) == 0))
        {
            printf("\n");
        }
        printf(" %02X", data[i]);
    }
    printf("\n");

    printf(" (%d bytes)\n\n", (int)size);
}

BOOL ParseFCP(
    BYTE       *buf,
    DWORD       buf_len,
    tPinStatus *pin_status,
    DWORD      *file_len,
    DWORD      *record_len,
    DWORD      *record_num
)
{
#define USIM_FSP_TEMPL_TAG            0x62
#define USIM_TLV_FILE_SIZE            0x80
#define USIM_TLV_TOTAL_FILE_SIZE      0x81
#define USIM_TLV_FILE_DESC            0x82
#define USIM_TLV_FILE_ID              0x83
#define USIM_TLV_DF_NAME              0x84
#define USIM_TLV_SHORT_FILE_ID        0x88
#define USIM_TLV_LIFE_CYCLE_STATUS    0x8A
#define USIM_TLV_PROPR_INFO           0xA5
#define USIM_TLV_PIN_STATUS_TEMPLATE  0xC6
#define USIM_PS_DO_TAG                0x90

    BYTE *pos, *end;

    //if ( pin_status )
    //{
    //    pin_status->ps_do = 0x0;
    //    pin_status->pin_enable  = FALSE;
    //    pin_status->pin_key_ref  = 0x01;
    //    pin_status->pin2_enable = FALSE;
    //    pin_status->pin2_key_ref = 0x81;
    //    pin_status->adm_enable  = FALSE;
    //    pin_status->adm_key_ref  = 0x0A;
    //}
    if ( file_len )
    {
        *file_len = 0;
    }
    if ( record_len )
    {
        *record_len = 0;
    }
    if ( record_num )
    {
        *record_num = 0;
    }

    pos = buf;
    end = pos + buf_len;
    if ((*pos) != USIM_FSP_TEMPL_TAG)
    {
        printf(
            "ERROR: file header did not start with FSP template tag\r\n"
        );
        return FALSE;
    }
    pos++;
    if (pos >= end)
    {
        printf(
            "ERROR: length of FCP is un-enough\r\n"
        );
        return FALSE;
    }
    if ((pos + pos[0]) < end)
    {
        end = pos + 1 + pos[0];
    }
    pos++;
    //printf("FCP: file header FSP template\r\n");
    //mem_dump(pos, end - pos);

    while ((pos + 1) < end)
    {
        //printf("FCP: file header TLV 0x%02x len=%d\r\n", pos[0], pos[1]);
        if (pos + 2 + pos[1] > end)
        {
            break;
        }

        if ((pos[0] == USIM_TLV_FILE_SIZE) &&
            ((pos[1] == 1) || (pos[1] == 2)) &&
            ( file_len ))
        {
            if (pos[1] == 1)
            {
                *file_len = pos[2];
            }
            else
            {
                *file_len = ((pos[2] << 8) | (pos[3]));
            }
            //printf("FCP: file_size=%d\r\n", (int)(*file_len));
        }
        else if ((pos[0] == USIM_TLV_FILE_DESC) &&
                 (pos[1] > 2) &&
                 ( record_len ) &&
                 ( record_num ))
        {
            *record_len = ((pos[4] << 8) | (pos[5]));
            *record_num = pos[6];
            //printf(
            //    "FCP: record_len=%d, record_num=%d\r\n",
            //    (int)(*record_len),
            //    (int)(*record_num)
            //);
        }
        else if ((pos[0] == USIM_TLV_PIN_STATUS_TEMPLATE) &&
                 (pos[1] >= 2) &&
                 (pos[2] == USIM_PS_DO_TAG) &&
                 (pos[3] >= 1) &&
                 ( pin_status ))
        {
            BYTE *key_ref_tag;
            BYTE  mask;
            BYTE  i, j;

            //printf("FCP: PS_DO=0x%02X\r\n", pos[4]);

            pin_status->ps_do = pos[4];

            /* check CHV(PIN, PIN2, ADM) enable/disable */
            mask = 0x80;
            key_ref_tag = (pos + 4 + pos[3]);
            for (i=0; i<((pos[1] / 3) - 1); i++)
            {
                for (j=0; j<CHV_INFO_NUM; j++)
                {
                    if ((key_ref_tag[0] == CHV_INFO[j][0]) &&
                        (key_ref_tag[1] == CHV_INFO[j][1]) &&
                        (key_ref_tag[2] == CHV_INFO[j][2]))
                    {
                        switch ( CHV_INFO[j][3] )
                        {
                            case CHV_PIN:
                                if (pin_status->ps_do & mask)
                                {
                                    pin_status->pin_enable  = TRUE;
                                }
                                if (pin_status->pin_key_ref == 0)
                                {
                                    pin_status->pin_key_ref = key_ref_tag[2];
                                }
                                break;
                            case CHV_PIN2:
                                if (pin_status->ps_do & mask)
                                {
                                    pin_status->pin2_enable = TRUE;
                                }
                                if (pin_status->pin2_key_ref == 0)
                                {
                                    pin_status->pin2_key_ref = key_ref_tag[2];
                                }
                                break;
                            case CHV_ADM:
                                if (pin_status->ps_do & mask)
                                {
                                    pin_status->adm_enable  = TRUE;
                                }
                                if (pin_status->adm_key_ref == 0)
                                {
                                    pin_status->adm_key_ref = key_ref_tag[2];
                                }
                                break;
                            default:
                                ;
                        }
                        break;
                    }
                }

                mask >>= 1;
                key_ref_tag += 3;
            }
        }

        pos += (2 + pos[1]);

        if (pos == end)
        {
            //printf("\r\n");
            return TRUE;
        }
    }

    printf(
        "ERROR: fail to parse FCP\r\n"
    );
    return FALSE;
}

void ResetSW1SW2(tResSW1SW2* sw)
{
    sw->SW1 = 0;
    sw->SW2 = 0;
}

void CheckResult(long result, char *funcName)
{
    if (result != SCARD_S_SUCCESS)
    {
        printf("%s error: ", funcName);

        switch (result)
        {
            case SCARD_E_INVALID_PARAMETER:
                printf("SCARD_E_INVALID_PARAMETER.\r\n");
                break;
            case SCARD_E_INVALID_VALUE:
                printf("SCARD_E_INVALID_VALUE.\r\n");
                break;
            case SCARD_E_NO_MEMORY:
                printf("SCARD_E_NO_MEMORY.\r\n");
                break;
            case SCARD_E_NO_SERVICE:
                printf("SCARD_E_NO_SERVICE.\r\n");
                break;
            case SCARD_F_COMM_ERROR:
                printf("SCARD_F_COMM_ERROR.\r\n");
                break;
            case SCARD_F_INTERNAL_ERROR:
                printf("SCARD_F_INTERNAL_ERROR.\r\n");
                break;
            case SCARD_E_INSUFFICIENT_BUFFER:
                printf("SCARD_E_INSUFFICIENT_BUFFER.\r\n");
                break;
            case SCARD_E_INVALID_HANDLE:
                printf("SCARD_E_INVALID_HANDLE.\r\n");
                break;
            case SCARD_E_READER_UNAVAILABLE:
                printf("SCARD_E_READER_UNAVAILABLE.\r\n");
                break;
            case SCARD_E_TIMEOUT:
                printf("SCARD_E_TIMEOUT.\r\n");
                break;
            case SCARD_E_NO_SMARTCARD:
                printf("SCARD_E_NO_SMARTCARD.\r\n");
                break;
            case SCARD_E_NOT_READY:
                printf("SCARD_E_NOT_READY.\r\n");
                break;
            case SCARD_E_PROTO_MISMATCH:
                printf("SCARD_E_PROTO_MISMATCH.\r\n");
                break;
            case SCARD_E_SHARING_VIOLATION:
                printf("SCARD_E_SHARING_VIOLATION.\r\n");
                break;
            case SCARD_E_UNKNOWN_READER:
                printf("SCARD_E_UNKNOWN_READER.\r\n");
                break;
            case SCARD_W_UNPOWERED_CARD:
                printf("SCARD_W_UNPOWERED_CARD.\r\n");
                break;
            case SCARD_W_UNRESPONSIVE_CARD:
                printf("SCARD_W_UNRESPONSIVE_CARD.\r\n");
                break;
            case SCARD_E_NOT_TRANSACTED:
                printf("SCARD_E_NOT_TRANSACTED.\r\n");
                break;
            case SCARD_W_RESET_CARD:
                printf("SCARD_W_RESET_CARD.\r\n");
                break;
            case SCARD_W_REMOVED_CARD:
                printf("SCARD_W_REMOVED_CARD.\r\n");
                break;

            default:
                printf("Unknown result (%Xh).\r\n", result);
        }
    }
}

BOOL CheckSW1SW2(BYTE *rspApdu, DWORD rspApduLen)
{
    BOOL  retval = FALSE;
    BYTE *SW;

    if (rspApduLen > 2)
    {
        SW = (rspApdu + (rspApduLen - 2));
    }
    else if (rspApduLen == 2)
    {
        SW = rspApdu;
    }
    else
    {
        return FALSE;
    }

    switch ((unsigned char) SW[0])
    {
        case 0x61:
        case 0x90:
        case 0x9F:
            retval = TRUE;
            break;

        default:
            printf("SW1 error (0x%02X 0x%02X)\r\n", SW[0], SW[1]);
            retval = FALSE;
    }

    return retval;
}

tRecordInfo GetRecordInfo(BYTE* RecvData, DWORD dataLen)
{
    static tRecordInfo recordInfo;
    tEFRsp EFRsp;

    /* get record num(s) and per record size */
    if (0x62 == RecvData[0])
    {
        /* [1] FCP parameters */
        DWORD recordLen;
        DWORD recordNum;
        BOOL success;

        success = ParseFCP(RecvData, dataLen, NULL, NULL, &recordLen, &recordNum);
        if ( !success )
        {
            printf("ERROR: fail to get record info\r\n");
            recordInfo.recordLen = 0;
            recordInfo.recordNum = 0;
        }
        else
        {
            recordInfo.recordLen = recordLen;
            recordInfo.recordNum = recordNum;
        }
    }
    else
    {
        /* [2] EF response parameters */
        EFRsp.RFU[0]        = RecvData[0];
        EFRsp.RFU[1]        = RecvData[1];
        EFRsp.fileSize[0]   = RecvData[2];
        EFRsp.fileSize[1]   = RecvData[3];
        EFRsp.fileID[0]     = RecvData[4];
        EFRsp.fileID[1]     = RecvData[5];
        EFRsp.typeOfFile    = RecvData[6];
        EFRsp.byte8RFU      = RecvData[7];
        EFRsp.accessCond[0] = RecvData[8];
        EFRsp.accessCond[1] = RecvData[9];
        EFRsp.accessCond[2] = RecvData[10];
        EFRsp.fileStatus    = RecvData[11];
        EFRsp.lengthOfFollowData = RecvData[12];
        EFRsp.strucOfEF     = RecvData[13];
        EFRsp.lenOfRecord   = RecvData[14];

        recordInfo.recordLen = EFRsp.lenOfRecord;
        recordInfo.recordNum = (EFRsp.fileSize[0]*256 + EFRsp.fileSize[1]) / recordInfo.recordLen;
    }

    return recordInfo;
}

BOOL CmdSelect(
    BYTE        CLA,
    BYTE        P1,
    BYTE        P2,
    BYTE       *file,
    tResSW1SW2 *sw
)
{
    long status;

    ResetSW1SW2( sw );

    SendBuff[0] = CLA;
    SendBuff[1] = 0xA4;
    SendBuff[2] = P1;
    SendBuff[3] = P2;
    SendBuff[4] = 0x02;
    SendBuff[5] = file[0];
    SendBuff[6] = file[1];
    SendLen = 7;
    RecvLen = sizeof( RecvBuff );

    if ( g_verbose )
    {
        printf("-> SELECT\r\n");
        mem_dump(SendBuff, SendLen);
    }

    status = SCardTransmit(
                 hCard,
                 &ioRequest,
                 SendBuff,
                 SendLen,
                 NULL,
                 RecvBuff,
                 &RecvLen
             );

    CheckResult(status, "SCardTransmit");

    if (status == SCARD_S_SUCCESS)
    {
        if ( g_verbose )
        {
            printf("<-\r\n");
            mem_dump(RecvBuff, RecvLen);
        }

        if ( CheckSW1SW2(RecvBuff, RecvLen) )
        {
            sw->SW1 = RecvBuff[0];
            sw->SW2 = RecvBuff[1];
            //printf("SW1=%02X, SW2=%02X\r\n", sw->SW1, sw->SW2);
            return TRUE;
        }
    }

    return FALSE;
}

BOOL CmdSelectADF(
    BYTE        CLA,
    BYTE        AIDlen,
    BYTE       *AIDvalue,
    tResSW1SW2 *sw
)
{
    long status;

    ResetSW1SW2( sw );

    SendBuff[0] = CLA;
    SendBuff[1] = 0xA4;
    SendBuff[2] = 0x04;
    SendBuff[3] = 0x04;
    SendLen = 4;
    *(SendBuff + SendLen) = AIDlen;
    SendLen++;
    memcpy(SendBuff+SendLen, AIDvalue, AIDlen);
    SendLen += AIDlen;
    RecvLen = sizeof(RecvBuff);

    if ( g_verbose )
    {
        printf("-> SELECT ADF\r\n");
        mem_dump(SendBuff, SendLen);
    }

    status = SCardTransmit(
                 hCard,
                 &ioRequest,
                 SendBuff,
                 SendLen,
                 NULL,
                 RecvBuff,
                 &RecvLen
             );

    CheckResult(status, "SCardTransmit");

    if (status == SCARD_S_SUCCESS)
    {
        if ( g_verbose )
        {
            printf("<-\r\n");
            mem_dump(RecvBuff, RecvLen);
        }

        if ( CheckSW1SW2(RecvBuff, RecvLen) )
        {
            sw->SW1 = RecvBuff[0];
            sw->SW2 = RecvBuff[1];
            //printf("SW1=%02X, SW2=%02X\r\n", sw->SW1, sw->SW2);
            return TRUE;
        }
    }

    return FALSE;
}

BOOL CmdGetResponse(BYTE CLA, BYTE len, tResSW1SW2 *sw)
{
    long status;

    ResetSW1SW2( sw );

    SendBuff[0] = CLA;
    SendBuff[1] = 0xC0;
    SendBuff[2] = 0x00;
    SendBuff[3] = 0x00;
    SendBuff[4] = len;
    SendLen = 5;
    RecvLen = sizeof(RecvBuff);

    if ( g_verbose )
    {
        printf("-> GET RESPONSE\r\n");
        mem_dump(SendBuff, SendLen);
    }

    status = SCardTransmit(
                 hCard,
                 &ioRequest,
                 SendBuff,
                 SendLen,
                 NULL,
                 RecvBuff,
                 &RecvLen
             );

    CheckResult(status, "SCardTransmit");

    if (status == SCARD_S_SUCCESS)
    {
        if ( g_verbose )
        {
            printf("<-\r\n");
            mem_dump(RecvBuff, RecvLen);
        }

        if ( CheckSW1SW2(RecvBuff, RecvLen) )
        {
            sw->SW1 = RecvBuff[RecvLen-2];
            sw->SW2 = RecvBuff[RecvLen-1];
            //printf("SW1=%02X, SW2=%02X\r\n", sw->SW1, sw->SW2);
            return TRUE;
        }
    }

    return FALSE;
}

BOOL CmdReadBinary(BYTE CLA, BYTE len)
{
    long status;

    SendBuff[0] = CLA;
    SendBuff[1] = 0xB0;
    SendBuff[2] = 0x00;
    SendBuff[3] = 0x00;
    SendBuff[4] = len;
    SendLen = 5;
    RecvLen = sizeof(RecvBuff);

    if ( g_verbose )
    {
        printf("-> READ BINARY\r\n");
        mem_dump(SendBuff, SendLen);
    }

    status = SCardTransmit(
                 hCard,
                 &ioRequest,
                 SendBuff,
                 SendLen,
                 NULL,
                 RecvBuff,
                 &RecvLen
             );

    CheckResult(status, "SCardTransmit");

    if (status == SCARD_S_SUCCESS)
    {
        if ( g_verbose )
        {
            printf("<-\r\n");
            mem_dump(RecvBuff, RecvLen);
        }

        if ( CheckSW1SW2(RecvBuff, RecvLen) )
        {
            //printf("SW1=%02X, SW2=%02X\r\n", sw->SW1, sw->SW2);
            return TRUE;
        }
    }

    return FALSE;
}

BOOL CmdReadRecord(
    BYTE  CLA,
    BYTE  P1,
    BYTE  P2,
    BYTE  len
)
{
    long status;

    SendBuff[0] = CLA;
    SendBuff[1] = 0xB2;
    SendBuff[2] = P1;
    SendBuff[3] = P2;
    SendBuff[4] = len;
    SendLen = 5;
    RecvLen = sizeof(RecvBuff);

    if ( g_verbose )
    {
        printf("-> READ RECORD\r\n");
        mem_dump(SendBuff, SendLen);
    }

    status = SCardTransmit(
                 hCard,
                 &ioRequest,
                 SendBuff,
                 SendLen,
                 NULL,
                 RecvBuff,
                 &RecvLen
             );

    CheckResult(status, "SCardTransmit");

    if (status == SCARD_S_SUCCESS)
    {
        if ( g_verbose )
        {
            printf("<-\r\n");
            mem_dump(RecvBuff, RecvLen);
        }

        if ( CheckSW1SW2(RecvBuff, RecvLen) )
        {
            //printf("SW1=%02X, SW2=%02X\r\n", sw->SW1, sw->SW2);
            return TRUE;
        }
    }

    return FALSE;
}

BOOL CmdVerify(
    BYTE        CLA,
    BYTE        P1,
    BYTE        P2,
    BYTE        len,
    BYTE       *code,
    tResSW1SW2 *sw
)
{
    long status;

    ResetSW1SW2( sw );

    SendBuff[0] = CLA;
    SendBuff[1] = 0x20;
    SendBuff[2] = P1;
    SendBuff[3] = P2;
    SendLen = 4;
    *(SendBuff+SendLen) = len;
    SendLen++;
    if (len > 0)
    {
        memcpy(SendBuff+SendLen, code, len);
        SendLen += len;
    }
    RecvLen = sizeof(RecvBuff);

    if ( g_verbose )
    {
        printf("-> VERIFY\r\n");
        mem_dump(SendBuff, SendLen);
    }

    status = SCardTransmit(
                 hCard,
                 &ioRequest,
                 SendBuff,
                 SendLen,
                 NULL,
                 RecvBuff,
                 &RecvLen
             );

    CheckResult(status, "SCardTransmit");

    if (status == SCARD_S_SUCCESS)
    {
        if ( g_verbose )
        {
            printf("<-\r\n");
            mem_dump(RecvBuff, RecvLen);
        }

        sw->SW1 = RecvBuff[0];
        sw->SW2 = RecvBuff[1];
        //printf("SW1=%02X, SW2=%02X\r\n", sw->SW1, sw->SW2);

        return TRUE;
    }

    return FALSE;
}

BOOL ReadEfDir(void)
{
    tResSW1SW2 sw;
    BOOL  success;
    DWORD rspLen;
    tRecordInfo recordInfo;
    BYTE  offset;
    BYTE  i;


    success = CmdSelect(FIRST_CLA, 0x00, 0x0C, MF, &sw);
    if ( !success )
    {
        printf("ERROR: Select MF fail\r\n");
        return FALSE;
    }

    success = CmdSelect(FIRST_CLA, 0x00, 0x04, EF_DIR, &sw);
    if ( !success )
    {
        printf("ERROR: Select MF/EF_DIR fail\r\n");
        return FALSE;
    }

    if (0 == sw.SW2)
    {
        printf("ERROR: MF/EF_DIR has no content\r\n");
        return FALSE;
    }

    success = CmdGetResponse(FIRST_CLA, sw.SW2, &sw);
    if ( !success )
    {
        printf("ERROR: Get response fail\r\n");
        return FALSE;
    }
    rspLen = RecvLen - 2;

    //get record num(s) and per record size
    recordInfo = GetRecordInfo(RecvBuff, rspLen);

    EFDirRecordNum = 0;
    for (i=0; i<recordInfo.recordNum; i++)
    {
        success = CmdReadRecord(FIRST_CLA, i+1, 0x04, (BYTE)recordInfo.recordLen);
        if ( !success )
        {
            printf("ERROR: Read record(%d) fail\r\n", i+1);
            return FALSE;
        }
        rspLen = RecvLen - 2;

        /*
        *   Length | Description                        | Status
        *  --------+------------------------------------+--------
        *     1    | Application template tag           |  M
        *     1    | Length of the application template |  M
        *     1    | Application Identifier tag         |  M
        *     1    | AID length                         |  M
        *  12 - 16 | AID value                          |  M
        *     1    | Application label tag              |  O
        *     1    | Application label length           |  O
        *   1 - 32 | Application label value            |  O
        */
        memset(&(EFDirRecord[i]), 0x00, sizeof( tEFDirRecord ));
        offset = 0;
        EFDirRecord[i].appTempTag = *(RecvBuff+offset);
        offset++;
        EFDirRecord[i].lenOfAppTemp = *(RecvBuff+offset);
        offset++;
        EFDirRecord[i].appIdTag = *(RecvBuff+offset);
        offset++;
        EFDirRecord[i].lenOfAID= *(RecvBuff+offset);
        offset++;
        memcpy(&(EFDirRecord[i].AIDvalue), RecvBuff+offset, EFDirRecord[i].lenOfAID);
        offset += EFDirRecord[i].lenOfAID;

        if (rspLen > offset)
        {
            EFDirRecord[i].appLabelTag = *(RecvBuff+offset);
            offset++;
        }

        if (rspLen > offset)
        {
            EFDirRecord[i].appLabelLen = *(RecvBuff+offset);
            offset++;
        }

        if (EFDirRecord[i].appLabelLen > 0)
        {
            memcpy(&(EFDirRecord[i].appLabelValue[0]), RecvBuff+offset,  EFDirRecord[i].appLabelLen);
        }

        EFDirRecordNum++;
    }

    return TRUE;
}

BOOL ReadEfIccid(void)
{
    tResSW1SW2 sw;
    BOOL  success;


    success = CmdSelect(FIRST_CLA, 0x00, 0x0C, MF, &sw);
    if ( !success )
    {
        printf("ERROR: Select MF fail\r\n");
        return FALSE;
    }

    success = CmdSelect(FIRST_CLA, 0x00, 0x04, EF_ICCID, &sw);
    if ( !success )
    {
        printf("ERROR: Select MF/EF_ICCID fail\r\n");
        return FALSE;
    }

    success = CmdReadBinary(FIRST_CLA, EF_ICCID_LENGTH);
    if ( !success )
    {
        printf("ERROR: Read MF/EF_ICCID fail\r\n");
        return FALSE;
    }

    printf("EF_ICCID:\r\n");
    mem_dump(RecvBuff, EF_ICCID_LENGTH);

    return TRUE;
}

BOOL SelectUsimADF()
{
    BYTE AID_3GPP[] = {
             0xA0, 0x00, 0x00, 0x00, 0x87,
             0x10, 0x02  // USIM App code (TS 31.110 Annex A)
         };

    tResSW1SW2 sw;
    BOOL  success;
    DWORD rspLen;
    BYTE  i;


    PIN_STATUS.ps_do        = 0x0;
    PIN_STATUS.pin_enable   = FALSE;
    PIN_STATUS.pin_key_ref  = 0x0;
    PIN_STATUS.pin2_enable  = FALSE;
    PIN_STATUS.pin2_key_ref = 0x0;
    PIN_STATUS.adm_enable   = FALSE;
    PIN_STATUS.adm_key_ref  = 0x0;


    for (i=0; i<EFDirRecordNum; i++)
    {
        if (0 == memcmp(AID_3GPP, &EFDirRecord[i].AIDvalue, (LENGTH_OF_RID+2)))
        {
            goto _AID_3GPP_FOUND;
            break;
        }
    }
    printf("ERROR: cannot find 3GPP USIM AID from EF_DIR\r\n");
    return FALSE;

_AID_3GPP_FOUND:
    success = CmdSelect(FIRST_CLA, 0x00, 0x0C, MF, &sw);
    if ( !success )
    {
        printf("ERROR: Select MF fail\r\n");
        return FALSE;
    }

    success = CmdSelectADF(
                    FIRST_CLA,
                    EFDirRecord[i].lenOfAID,
                    (BYTE *)&(EFDirRecord[i].AIDvalue),
                    &sw
              );
    if ( !success )
    {
        printf("ERROR: Select ADF fail\r\n");
        return FALSE;
    }

    if (sw.SW2 > 0)
    {
        success = CmdGetResponse(FIRST_CLA, sw.SW2, &sw);
        if ( !success )
        {
            printf("ERROR: Get response fail\r\n");
            return FALSE;
        }
        rspLen = RecvLen - 2;

        // find PIN status indication (0xC6)
        success = ParseFCP(RecvBuff, rspLen, &PIN_STATUS, NULL, NULL, NULL);
        if ( !success )
        {
            printf("WARN: cannot find the PIN status indication\r\n");
        }
    }

    return TRUE;
}

BOOL RetryCounter(void)
{
    tResSW1SW2 sw;
    BOOL success;

    CHAR name[3][8] = { "PIN", "PIN2", "ADM" };
    BOOL enable;
    BYTE key;
    BYTE i;

    for (i=0; i<3; i++)
    {
        enable = ((0 == i) ? PIN_STATUS.pin_enable  :
                  (1 == i) ? PIN_STATUS.pin2_enable : PIN_STATUS.adm_enable);
        key = ((0 == i) ? PIN_STATUS.pin_key_ref  :
               (1 == i) ? PIN_STATUS.pin2_key_ref : PIN_STATUS.adm_key_ref);

        success = CmdVerify(FIRST_CLA, 0x00, key, 0, NULL, &sw);
        if ( !success )
        {
            printf("ERROR: Verify retry counter fail\r\n");
            //return FALSE;
        }

        printf(
            "%4s ... %s (retry counter %c)\r\n",
            name[i],
            (enable ? "enabled" : "disabled"),
            ((sw.SW1 == 0x63) ? ((sw.SW2 & 0xF) + '0') : '?')
        );
    }
    printf("\r\n");

    return TRUE;
}

BOOL ReadIMSI(void)
{
    tResSW1SW2 sw;
    BOOL success;

    success = CmdSelect(FIRST_CLA, 0x00, 0x04, EF_IMSI, &sw);
    if ( !success )
    {
        printf("ERROR: Select ADF/EF_IMSI fail\r\n");
        return FALSE;
    }

    success = CmdReadBinary(FIRST_CLA, EF_IMSI_LENGTH);
    if ( !success )
    {
        printf("ERROR: Read ADF/EF_IMSI fail\r\n");
        return FALSE;
    }

    printf("EF_IMSI:\r\n");
    mem_dump(RecvBuff, EF_IMSI_LENGTH);

    return TRUE;
}

int main(int argc, char* argv[])
{
    int retCode;

    if ((argc > 1) && (0 == strcmp("-v", argv[1])))
    {
        g_verbose = TRUE;
    }

    /* [1] Establish context */
    retCode = SCardEstablishContext(
                    SCARD_SCOPE_USER,
                    NULL,
                    NULL,
                    &hContext
                );
    if (retCode != SCARD_S_SUCCESS)
    {
        printf("ERROR: SCardEstablishContext %Xh\r\n", retCode);
        goto _EXIT;
    }
    printf("SCardEstablishContext ... OK\r\n");


    /* [2] List readers */
    memset(readerName, 0x00, 256);
    retCode = SCardListReaders(
                    hContext,
                    NULL,
                    readerName,
                    &readerNameLen
                );
    if (retCode != SCARD_S_SUCCESS)
    {
        printf("ERROR: SCardListReaders %Xh\r\n", retCode);
        goto _EXIT;
    }
    if (readerName[0] == 0x00)
    {
        printf("ERROR: No PC/SC reader is detected\r\n");
        goto _EXIT;
    }
    printf("SCardListReaders ........ OK\r\n");
    printf("\"%s\"\r\n", readerName);


    /* [3] Connect */
    retCode = SCardConnect(
                    hContext,
                    readerName,
                    SCARD_SHARE_SHARED,
                    SCARD_PROTOCOL_T0,
                    &hCard,
                    &dwActProtocol
                );
    if (retCode != SCARD_S_SUCCESS)
    {
        printf("ERROR: SCardConnect %Xh\r\n", retCode);
        goto _EXIT;
    }
    printf("SCardConnect ............ OK\r\n");

    switch ( dwActProtocol )
    {
        case SCARD_PROTOCOL_T0:
            ioRequest = *SCARD_PCI_T0;
            break;
        case SCARD_PROTOCOL_T1:
            ioRequest = *SCARD_PCI_T1;
            break;
        default:
            printf("ERROR: Unknown protocol %d\r\n", dwActProtocol);
            goto _EXIT;
    }
    printf("\r\n");


    /* [4] Read USIM IMSI */
    /*
    *  MF
    *   |-- EF_DIR
    *   |-- EF_ICCID
    *   `-- ...
    *  ADF_USIM
    *   |-- EF_IMSI
    *   `-- ...
    */
    if ( !ReadEfDir() )
    {
        printf("ERROR: Read EF_DIR fail\r\n");
        goto _EXIT;
    }

    if ( !ReadEfIccid() )
    {
        printf("ERROR: Read EF_ICCID fail\r\n");
        goto _EXIT;
    }

    if ( !SelectUsimADF() )
    {
        printf("ERROR: Select ADF_USIM fail\r\n");
        goto _EXIT;
    }

    RetryCounter();

    if ( !ReadIMSI() )
    {
        printf("ERROR: Read EF_IMSI fail\r\n");
    }


_EXIT:
    printf("\r\n");

    if (hCard != 0)
    {
        retCode = SCardDisconnect(hCard, SCARD_RESET_CARD);
        if (retCode != SCARD_S_SUCCESS)
        {
            printf("ERROR: SCardDisconnect %Xh\r\n", retCode);
        }
    }
    printf("SCardDisconnect ....... OK\r\n");

    if (hContext != 0)
    {
        retCode = SCardReleaseContext( hContext );
        if (retCode != SCARD_S_SUCCESS)
        {
            printf("ERROR: SCardReleaseContext %Xh\r\n", retCode);
        }
    }
    printf("SCardReleaseContext ... OK\r\n");

    printf("\r\n");
    return 0;
}

