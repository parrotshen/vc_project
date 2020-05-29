#if !defined(_SIM_FILE_SYS_H_)
#define _SIM_FILE_SYS_H_


#define MAX_BUF_LENGTH  255

#define ITEM_MF         1
#define ITEM_DF_1st     2
#define ITEM_DF_2nd     3
#define ITEM_EF_1st     4
#define ITEM_EF_2nd     5
#define ITEM_EF_3rd     6
#define ITEM_DUMMY      9
#define ITEM_UNKNOWN    0

typedef enum _tFileType {
	FILE_TYPE_RFU = 0x0,
	FILE_TYPE_MF  = 0x1,
	FILE_TYPE_DF  = 0x2,
	FILE_TYPE_EF  = 0x4,

	FILE_TYPE_INVALID
} tFileType;

typedef enum _tFileStruct {
	FILE_STRUCT_BINARY = 0x0,
	FILE_STRUCT_RECORD = 0x1,

	FILE_STRUCT_INVALID
} tFileStruct;

typedef enum _tAccessCond {
	ACCESS_ALW   = 0x0,
	ACCESS_CHV1  = 0x1,
	ACCESS_CHV2  = 0x2,
	ACCESS_RFU   = 0x3,
	ACCESS_ADM4  = 0x4,
	ACCESS_ADM5  = 0x5,
	ACCESS_ADM6  = 0x6,
	ACCESS_ADM7  = 0x7,
	ACCESS_ADM8  = 0x8,
	ACCESS_ADM9  = 0x9,
	ACCESS_ADM10 = 0xA,
	ACCESS_ADM11 = 0xB,
	ACCESS_ADM12 = 0xC,
	ACCESS_ADM13 = 0xD,
	ACCESS_ADM14 = 0xE,
	ACCESS_NEW   = 0xF
} tAccessCond;


typedef struct _tMfDfParam
{
	int    size;
	BYTE   fid[2];
	BYTE   type;  /* ==> tFileType */
	BOOL   chv1Enable;
	BYTE   numDf;
	BYTE   numEf;
	BYTE   numChv;
	BOOL   chv1Init;
	BYTE   chv1Remain;
	BOOL   chv1InitUnblock;
	BYTE   chv1RemainUnblock;
	BOOL   chv2Init;
	BYTE   chv2Remain;
	BOOL   chv2InitUnblock;
	BYTE   chv2RemainUnblock;
} tMfDfParam;

typedef struct _tEfParam
{
	int    size;
	BYTE   fid[2];
	BYTE   type;  /* ==> tFileType */
	BYTE   condUpdate;  /* ==> tAccessCond */
	BYTE   condRead;  /* ==> tAccessCond */
	BYTE   condIncrease;  /* ==> tAccessCond */
	BYTE   condInvalidate;  /* ==> tAccessCond */
	BYTE   condRehabilitate;  /* ==> tAccessCond */
	BYTE   structure;  /* ==> tFileStruct */
	BYTE   recLen;
	int    recNum;
} tEfParam;

typedef struct _tEfData
{
	struct _tEfData *next;
	BYTE  *data;
	int    dataLen;
} tEfData;

typedef struct _tFileInfo
{
	tMfDfParam *paramMfDf;
	tEfParam   *paramEf;
	tEfData    *dataEf;
} tFileInfo;

typedef struct _tSimFileSys
{
	HTREEITEM  item;
	int        type;
	BYTE       fid[2];
	char      *name;
	int        jump;
	tFileInfo *info;
} tSimFileSys;


#endif // !defined(_SIM_FILE_SYS_H_)
