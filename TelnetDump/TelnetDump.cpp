// TelnetDump.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "TelnetDump.h"

//#include <Wincon.h>
#include "econio.h"
#include "Winsock2.h"
#include "pcap.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// ANSI Control Code

unsigned short ANSI_COLOR[8] = {
	0,  /* 0-黑色   */
	4,  /* 1-紅色   */
	2,  /* 2-綠色   */
	6,  /* 3-黃色   */
	1,  /* 4-藍色   */
	5,  /* 5-洋紅色 */
	3,  /* 6-水藍色 */
	7   /* 7-白色   */
};

unsigned int ParseANSI(char* data, unsigned int next)
{
	CString Temp, Code = "";
	char end;
	int  index = 0;

	/* ANSI ~[x;x;x;xm 彩色控制碼 */
	int  parameter[5];
	parameter[0] = 0;
	parameter[1] = 0;
	parameter[2] = 0;
	parameter[3] = 0;
	parameter[4] = 0;
	do
	{
		next++;
		end = *(data+next);

		//Begin: 取得控制參數
		if(end == ';')
		{
			if(Code != "") parameter[index] = atoi(Code);
			index++;
			Code = "";
		}
		else
		{
			Temp.Format("%c", end);
			Code += Temp;
		}
		//End:   取得控制參數
	}while(end < 0x41 || end > 0x7A);
	parameter[index] = atoi(Code);

	/* 預設為黑底白字的正常顯示色彩 */
	static int fg = 7, bg = 0, light = 0;
	int i, x, y;
	switch( end )
	{
		case 'J':
			//清除螢幕
			clrscr();
			break;
		case 'H':
			//游標移至定位
			y = (parameter[0] == 0) ? 1 : parameter[0];
			x = (parameter[1] == 0) ? 1 : parameter[1];
			gotoxy(x, y);
			break;
		case 'K':
			//刪除目前所在列
			x = wherex();
			y = wherey();
			for(i=fg; i<=80; i++)
			{
				printf(" ");
			}
			gotoxy(x, y);
			break;
		case 'A':
			//Go Upper
			gotoxy(wherex(), wherey()-1);
			break;
		case 'B':
			//Go Down
			gotoxy(wherex(), wherey()+1);
			break;
		case 'C':
			//Go Right
			gotoxy(wherex()+1, wherey());
			break;
		case 'D':
			//Go Left
			gotoxy(wherex()-1, wherey());
			break;
		case 'm':
			//Foreground & Background Color
			for(i=0; i<5; i++)
			{
				if(i == 0 && parameter[0] == 0)
				{
					fg = 7;
					bg = 0;
					light = 0;
				}

				if(parameter[i] == 1)
					light = 8;

				if(parameter[i] >= 30 && parameter[i] <= 39)
					fg = ANSI_COLOR[ parameter[i]%30 ];

				if(parameter[i] >= 40)
					bg = ANSI_COLOR[ parameter[i]%40 ];
			}
			settextcolor(fg|light, bg);
			break;
	}

	/* 傳回控制碼結束的位置 */
	return next;
}

/////////////////////////////////////////////////////////////////////////////
// Data Struct of WPdpack

#define ETHER_ADDR_LEN 6
#define IP_ADDR_LEN    4
#define TCP_SEQ_LEN    4
#define TCP_ACK_LEN    4
#define IP_BYTE_LEN    15

/* Ethernet Header (14 bytes) */
struct sniff_ethernet
{
	byte    ether_dhost[ETHER_ADDR_LEN]; /* Destination host address */
	byte    ether_shost[ETHER_ADDR_LEN]; /* Source host address */
	u_short ether_type;                  /* IP? ARP? RARP? etc */
};

/* IP Header (20 bytes) */
struct sniff_ip
{
	byte    ip_vhl;              /* IP version (4 bits) */
					             /* header length (4 bits) */
	byte    ip_tos;              /* type of service */
	u_short ip_len;              /* total length */
	u_short ip_id;               /* identification */
	u_short ip_off;              /* fragment offset field */
	#define IP_RF      0x8000    /* reserved fragment flag */
	#define IP_DF      0x4000    /* dont fragment flag */
	#define IP_MF      0x2000    /* more fragments flag */
	#define IP_OFFMASK 0x1fff    /* mask for fragmenting bits */

	byte    ip_ttl;              /* time to live */
	byte    ip_p;                /* protocol above IP */
	u_short ip_sum;              /* header checksum */
	byte    ip_src[IP_ADDR_LEN]; /* source ip address */
	byte    ip_dst[IP_ADDR_LEN]; /* destination ip address */
};

/* TCP header (20 bytes) */
struct sniff_tcp
{
	u_short tcp_sport;            /* source port */
	u_short tcp_dport;            /* destination port */
	byte    tcp_seq[TCP_SEQ_LEN]; /* sequence number */
	byte    tcp_ack[TCP_ACK_LEN]; /* acknowledgement number */
	u_short tcp_off;              /* data offset (4 bits) */
                                  /* flag (12 bits) */
	#define TH_FIN   0x01
	#define TH_SYN   0x02
	#define TH_RST   0x04
	#define TH_PUSH  0x08
	#define TH_ACK   0x10
	#define TH_URG   0x20
	#define TH_ECE   0x40
	#define TH_CWR   0x80
	#define TH_FLAGS (TH_FIN|TH_SYN|TH_RST|TH_ACK|TH_URG|TH_ECE|TH_CWR)

	u_short tcp_win;              /* window */
	u_short tcp_sum;              /* checksum */
	u_short tcp_urp;              /* urgent pointer */
};

/////////////////////////////////////////////////////////////////////////////
// Callback Function of WPdpack

void got_packet(u_char *args, const struct pcap_pkthdr *header, const u_char *packet)
{
	//Begin: Define pointers for packet's attributes
	const struct sniff_ethernet *ethernet; /* The ethernet header */
	const struct sniff_ip       *ip;       /* The IP header */
	const struct sniff_tcp      *tcp;      /* The TCP header */
	const byte                  *payload;  /* Packet payload */
	//End:   Define pointers for packet's attributes

	//Begin: And define the size of the structures we're using
	int size_ethernet = sizeof(struct sniff_ethernet);
	int size_ip       = sizeof(struct sniff_ip);
	int size_tcp      = sizeof(struct sniff_tcp);
	//End:   And define the size of the structures we're using

	//Begin: Define our packet's attributes
	ethernet = (struct sniff_ethernet*)(packet);
	ip       = (struct sniff_ip*)(packet + size_ethernet);
	tcp      = (struct sniff_tcp*)(packet + size_ethernet + size_ip);
	payload  = (byte *)(packet + size_ethernet + size_ip + size_tcp);
	//End:   Define our packet's attributes

	//Begin: 找出資料真正的長度
	unsigned __int8 IP_Header_Len   = (ip->ip_vhl & 0x0F) * 4;
	unsigned __int8 offset          = (tcp->tcp_off & 0x00F0) >> 4;
	unsigned __int8 TCP_Header_Len  = offset * 4;
	unsigned int    Data_Packet_Len = ntohs(ip->ip_len) - IP_Header_Len - TCP_Header_Len;
	//End:   找出資料真正的長度

	if(Data_Packet_Len > 0)
	{
		CString packet_ip, target_ip;
		packet_ip.Format("%d.%d.%d.%d", ip->ip_dst[0], ip->ip_dst[1], ip->ip_dst[2], ip->ip_dst[3]);
		target_ip.Format("%s", args);

		//Begin: 過濾封包(compare ip & port number)
		BOOL IsWant = (ntohs(tcp->tcp_sport) == 23) && (packet_ip.Compare(target_ip) == 0);
		if( IsWant )
		{
			CString Content, Data_Packet = "";
			for(unsigned int i=0; i<Data_Packet_Len; i++)
			{
				//Begin: 過濾 ANSI 控制碼
				if((payload[i] == 0x1B) && (payload[i+1] == 0x5B))
				{
					i++; // -> '['
					i = ParseANSI((char*)payload, i);
				}
				else
				{
					/* 過濾 Telnet 控制碼 */
					if(payload[i] == 0xFF)
						i++;
					else
						printf("%c", payload[i]);
				}
				//End:   過濾 ANSI 控制碼

				//printf("%c", payload[i]);
			}
		}
		//End:   過濾封包
	}
}

/////////////////////////////////////////////////////////////////////////////
// Start to capture packet

void StartToDump()
{
	pcap_t *descr = NULL;                /* Sniff handler */
	char   *devs  = NULL;                /* Sniffing devise */
	int     dev_idx[16];
	char    errbuf[PCAP_ERRBUF_SIZE];    /* Error buffer */
	memset(errbuf, 0x00, PCAP_ERRBUF_SIZE);

	struct bpf_program fp;               /* hold compiled program */
	bpf_u_int32        maskp;            /* subnet mask */
	bpf_u_int32        netp;             /* ip */
	char               filter_app[] = "port 23";
	int                numOfPackets = 0; /* How many packets to sniff */

	int   i = 0;
	int   j = 0;
	int   done   = 0;
	int   offset = 0;
	char  index[8];
	char  target_ip[IP_BYTE_LEN];

	memset(index,     0x00, 8);
	memset(target_ip, 0x00, IP_BYTE_LEN);

	//Begin: Search Net Device
	devs = pcap_lookupdev( errbuf );
	if (devs == NULL)
	{
		printf("ERROR: Can't find any net device !!\r\n");
		return;
	}


	#if 0 //for debug
	{
		int k;
		printf("---- DEBUG ----\r\n");
		for (k=0; k<280; k++)
		{
			printf("%c", devs[k]);
		}
		printf("*\r\n");
		printf("---- DEBUG ----\r\n");
		printf("\r\n");
	}
	#endif


	dev_idx[j] = 0;
	do
	{
		printf("Device %d       : [", j);
		while (devs[i] != 0x00)
		{
			printf("%c", devs[i]);
			i += 2;
		}
		printf("]\r\n");

		i += 2;
		if (devs[i] == 0)
			done = 1;
		else
			dev_idx[++j] = i;
	} while ( !done );
	//End:   Search Net Device

	//Begin: Print devise to the user
	printf("Num of packets : [%d]\r\n", numOfPackets);
	printf("Filter apply   : [%s]\r\n", filter_app);
	printf("\r\n");
	//End:   Print devise to the user

	//Begin: Set our device
	printf("請選擇網路裝置 : ");
	settextcolor(15, 0);
	scanf("%s", index);
	settextcolor(7, 0);
	if (index[0] == 'q'|| index[0] == 'Q')
		return;

	offset = dev_idx[ atoi( index ) ];
	pcap_lookupnet(devs+offset, &netp, &maskp, errbuf);
	//End:   Set our device


	//Begin: Open the device so we can spy
	//[buffer size = 4096 bytes]
	descr = pcap_open_live(devs+offset, 4096, 1, 1000, errbuf);
	if (descr == NULL)
	{
		printf("pcap_open_live(): %s\r\n", errbuf);
		return;
	}
	//End:   Open the device so we can spy

	//Begin: Apply the rules
	if (pcap_compile(descr, &fp, filter_app, 0, maskp) == -1)
	{
		printf("pcap_compile borked!!");
		return;
	}
	if (pcap_setfilter(descr, &fp) == -1)
	{
		printf("pcap_setfilter said 'eat shit'!!");
		return;
	}
	//End:   Apply the rules


	//Begin: Get sniffer IP
	printf("請輸入目標 IP  : ");
	settextcolor(15, 0);
	scanf("%s", target_ip);
	settextcolor(7, 0);
	//End:   Get sniffer IP

	// Now we can set our callback function
	pcap_loop(descr, numOfPackets, got_packet, (u_char*)target_ip);
	pcap_close( descr );
}

/////////////////////////////////////////////////////////////////////////////
// The one and only application object

CWinApp theApp;
using namespace std;

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	// initialize MFC and print and error on failure
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: change error code to suit your needs
		cerr << _T("Fatal Error: MFC initialization failed") << endl;
		nRetCode = 1;
	}
	else
	{
		// TODO: code your application's behavior here.
		settextcolor(6, 3);
		printf("███████████████████\r\n");
		settextcolor(14, 3);
		printf(" Capture & Display of Telnet Packets! \r\n");
		printf(" Written by Y.J.Shen 2002/04!         \r\n");
		printf(" Enter [q/Q] to quit!                 \r\n");
		settextcolor(6, 3);
		printf("███████████████████\r\n");
		settextcolor(7, 0);

		/* 開始抓 Telnet 的封包 */
		StartToDump();
	}

	return nRetCode;
}
