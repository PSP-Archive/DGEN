////////////////////////////////////////////////////////////
/// psp_adhoc.c
////////////////////////////////////////////////////////////
#include "adhoc.h"
#include "psp_main.h"
#include "psp_adhoc.h"
#include "psp_screen.h"
////////////////////////////////////////////////////////////
typedef int           SceUID  ;
typedef unsigned int  SceSize ;

#ifndef NULL
#define NULL (0)
#endif //NULL

////////////////////////////////////////////////////////////
#define TIMEOUT_CONNECT   (30*1000*1000)
#define TIMEOUT_SEND      (3*1000*1000)
#define TIMEOUT_RECV      (3*1000*1000)
#define DELAY_RECV()      sceKernelDelayThread(1000)
////////////////////////////////////////////////////////////
/*
volatile unsigned long psp_adhoc_delay = 1000 ;

inline void PSP_Adhoc_SetDelay ( unsigned long delay )
{
	psp_adhoc_delay = delay ;
}
*/


/*
typedef struct _SceModuleInfo 
{
    unsigned short      modattribute  ;
    unsigned char       modversion[2] ;
    char                modname[27]   ;
    char                terminal      ;
    void                *gp_value     ;
    const void          *ent_top      ;
    const void          *ent_end      ;
    const void          *stub_top     ;
    const void          *stub_end     ;
} SceModuleInfo ;
*/

typedef struct _scemoduleinfo {
	unsigned short		modattribute;
	unsigned char		modversion[2];
	char			modname[27];
	char			terminal;
	void *			gp_value;
	void *			ent_top;
	void *			ent_end;
	void *			stub_top;
	void *			stub_end;
} _sceModuleInfo;

typedef const _sceModuleInfo SceModuleInfo;

typedef struct _SceModule 
{
        struct SceModule        *next          ;
        unsigned short          attribute      ;
        unsigned char           version[2]     ;
        char                    modname[27]    ;
        char                    terminal       ;
        unsigned int            unknown1       ;
        unsigned int            unknown2       ;
        SceUID                  modid          ;
        unsigned int            unknown3[4]    ;
        void *                  ent_top        ;
        unsigned int            ent_size       ;
        void *                  stub_top       ;
        unsigned int            stub_size      ;
        unsigned int            unknown4[4]    ;
        unsigned int            entry_addr     ;
        unsigned int            gp_value       ;
        unsigned int            text_addr      ;
        unsigned int            text_size      ;
        unsigned int            data_size      ;
        unsigned int            bss_size       ;
        unsigned int            nsegment       ;
        unsigned int            segmentaddr[4] ;
        unsigned int            segmentsize[4] ;
} SceModule ;

typedef struct _SceLibraryEntryTable 
{
        const char *            libname    ;
        unsigned char           version[2] ;
        unsigned short          attribute  ;
        unsigned char           len        ;
        unsigned char           vstubcount ;
        unsigned short          stubcount  ;
        void *                  entrytable ;
} SceLibraryEntryTable ;

typedef struct _SceKernelLMOption 
{
	SceSize 		size         ;
	SceUID 			mpidtext     ;
	SceUID 			mpiddata     ;
	unsigned int 	flags        ;
	char 			position     ;
	char 			access       ;
	char 			creserved[2] ;
} SceKernelLMOption ;

#define PSP_MEMORY_PARTITION_KERNEL (1)
#define PSP_MEMORY_PARTITION_USER   (2)

////////////////////////////////////////////////////////////

SceUID PSP_Module_LoadStart(SceModuleInfo* modInfoPtr, const char* szFile, const char* szFolder) ;

////////////////////////////////////////////////////////////
int  g_AdhocLoaded            = 0 ;
int  g_AdhocConnect           = 0 ;
int  g_AdhocDisonnect         = 0 ;

int  g_PDPID                  = 0 ;
int  g_MatchingID             = 0 ;

int  g_NetInit                = 0 ;
int  g_NetAdhocInit           = 0 ;
int  g_NetAdhocctlInit        = 0 ;
int  g_NetAdhocctlConnect     = 0 ;
int  g_NetAdhocPdpCreate      = 0 ;
int  g_NetAdhocMatchingInit   = 0 ;
int  g_NetAdhocMatchingCreate = 0 ;
int  g_NetAdhocMatchingStart  = 0 ;

int  g_unk1                   = 0 ;
int  g_matchOptLen            = 0 ;
volatile int  g_matchEvent             = 0 ;
volatile int  g_matchChanged           = 0 ;
//char g_matchOptData[1000]         ;
volatile int  g_mac_matching           = 0 ;
volatile char g_mac_match[6]               ;
char g_mac     [8]                ;
char g_mac_this[8]                ;

	//char path_adhoc[128] ;
	int  debug_adhoc ;
	//extern unsigned long debug_tm[10] ;

////////////////////////////////////////////////////////////

#define  CheckCancel() \
			if( (sceKernelLibcClock() - tm) > TM_1FRAME )\
			{\
				PSP_Ctrlpad_Update() ; \
				unsigned long pad = PSP_Ctrlpad_GetButtons() ;\
				if( (pad_old != 0) && (pad == 0) ){ return PSP_ADHOC_CANCEL ; }\
				pad_old = pad ;\
				tm = sceKernelLibcClock() ;\
			}

#define CheckTimeout() \
			if( (sceKernelLibcClock() - tm) > TIMEOUT_CONNECT )\
			{ \
				return PSP_ADHOC_TIMEOUT ; \
			}

//#define __DEBUG
#ifdef __DEBUG
void TRACE_ADHOC()
{
	int px = 20, line = 20 ;
	PSP_Screen_DrawInt2( px, line, debug_adhoc     , 0xFFFF ) ; line += 10 ;
	PSP_Screen_DrawInt2( px, line, g_mac_matching  , 0xFFFF ) ; line += 10 ;
	PSP_Screen_DrawInt2( px, line, g_matchChanged  , 0xFFFF ) ; line += 10 ;
	PSP_Screen_DrawInt2( px, line, g_matchEvent    , 0xFFFF ) ; line += 10 ;
	line +=10 ;
	PSP_Screen_DrawHex2( px, line, g_mac_match[0]  , 0xFFFF ) ; line += 10 ;
	PSP_Screen_DrawHex2( px, line, g_mac_match[1]  , 0xFFFF ) ; line += 10 ;
	PSP_Screen_DrawHex2( px, line, g_mac_match[2]  , 0xFFFF ) ; line += 10 ;
	PSP_Screen_DrawHex2( px, line, g_mac_match[3]  , 0xFFFF ) ; line += 10 ;
	PSP_Screen_DrawHex2( px, line, g_mac_match[4]  , 0xFFFF ) ; line += 10 ;
	PSP_Screen_DrawHex2( px, line, g_mac_match[5]  , 0xFFFF ) ; line += 10 ;
	line +=10 ;
	PSP_Screen_DrawHex2( px, line, g_mac[0]        , 0xFFFF ) ; line += 10 ;
	PSP_Screen_DrawHex2( px, line, g_mac[1]        , 0xFFFF ) ; line += 10 ;
	PSP_Screen_DrawHex2( px, line, g_mac[2]        , 0xFFFF ) ; line += 10 ;
	PSP_Screen_DrawHex2( px, line, g_mac[3]        , 0xFFFF ) ; line += 10 ;
	PSP_Screen_DrawHex2( px, line, g_mac[4]        , 0xFFFF ) ; line += 10 ;
	PSP_Screen_DrawHex2( px, line, g_mac[5]        , 0xFFFF ) ; line += 10 ;
	/*
	PSP_Screen_DrawInt2( px, line, g_NetInit               , 0xFFFF ) ; line += 10 ;
	PSP_Screen_DrawInt2( px, line, g_NetAdhocInit          , 0xFFFF ) ; line += 10 ;
	PSP_Screen_DrawInt2( px, line, g_NetAdhocctlInit       , 0xFFFF ) ; line += 10 ;
	PSP_Screen_DrawInt2( px, line, g_NetAdhocctlConnect    , 0xFFFF ) ; line += 10 ;
	PSP_Screen_DrawInt2( px, line, g_NetAdhocPdpCreate     , 0xFFFF ) ; line += 10 ;
	PSP_Screen_DrawInt2( px, line, g_NetAdhocMatchingInit  , 0xFFFF ) ; line += 10 ;
	PSP_Screen_DrawInt2( px, line, g_NetAdhocMatchingCreate, 0xFFFF ) ; line += 10 ;
	PSP_Screen_DrawInt2( px, line, g_NetAdhocMatchingStart , 0xFFFF ) ; line += 10 ;
	PSP_Screen_DrawInt2( px, line, g_PDPID                 , 0xFFFF ) ; line += 10 ;
	PSP_Screen_DrawInt2( px, line, g_MatchingID            , 0xFFFF ) ; line += 10 ;
	*/
	PSP_Screen_WaitVsync() ;
	PSP_Screen_Flip() ;
}
#else
#define TRACE_ADHOC()
#endif

////////////////////////////////////////////////////////////
int PSP_Adhoc_IsLoad          ( ) { return g_AdhocLoaded ; }

#ifdef _USE_PSPSDK
#define __moduleinfo  module_info
#endif //

//extern void* __moduleinfo ;
extern SceModuleInfo __moduleinfo ;
extern char path_exec [128] ;


int PSP_Adhoc_Init ()
{
	SceModuleInfo* mod_info_ptr = (SceModuleInfo*)(&__moduleinfo) ;

	if( mod_info_ptr == 0 ){ return -1 ; }

	const char flash0[] = "flash0:/kd/" ;
	const char* adhoc_modules[] = 
	{
		"ifhandle.prx"              , 
		"memab.prx"                 ,
		"pspnet_adhoc_auth.prx"     , 
		"pspnet.prx"                , 
		"pspnet_adhoc.prx"          , 
		"pspnet_adhocctl.prx"       ,
		"pspnet_adhoc_matching.prx" 
	} ;

	int n ;
	/*
	char path_modules[128] ;

	if( strlen( path_exec ) > 0 )
	{
		strcpy( path_modules, path_exec ) ;
		strcat( path_modules, "MODULES/" ) ;
	}
	else{ path_modules[0] = 0 ; }
	*/

	for( n=0; n < sizeof(adhoc_modules) / sizeof(char*) ; n++ )
	{
		//if( PSP_Module_LoadStart( modInfoPtr, adhoc_modules[n], path_modules ) < 0 ){ return -1 ; }

		//if(    (PSP_Module_LoadStart( modInfoPtr, adhoc_modules[n], path_modules ) < 0)
		//	&& (PSP_Module_LoadStart( modInfoPtr, adhoc_modules[n], flash0       ) < 0) ){ return -1 ; }
		if( PSP_Module_LoadStart( mod_info_ptr, adhoc_modules[n], flash0 ) < 0 ){ return -1 ; }
	}

	g_AdhocLoaded = 1 ;

	return 0 ;
}

int PSP_Adhoc_Term ()
{
    int retVal ;

	if(g_NetAdhocMatchingStart)
	{
		retVal = sceNetAdhocMatchingStop( g_MatchingID );
		if(retVal == 0)
		{
			g_NetAdhocMatchingStart = 0 ;
		}
	}

	if(g_NetAdhocMatchingCreate)
	{
		retVal = sceNetAdhocMatchingDelete( g_MatchingID );
		if(retVal == 0)
		{
			g_NetAdhocMatchingCreate = 0 ;
		}
	}
	
	if(g_NetAdhocMatchingInit)
	{
		retVal = sceNetAdhocMatchingTerm();
		if(retVal == 0)
		{
			g_NetAdhocMatchingInit = 0 ;
		}
	}

	if(g_NetAdhocPdpCreate)
	{
		retVal = sceNetAdhocPdpDelete(g_PDPID,0);
		if(retVal == 0)
		{
			g_NetAdhocPdpCreate = 0 ;
		}
	}

	if(g_NetAdhocctlConnect)
	{
		retVal = sceNetAdhocctlDisconnect();
		if(retVal == 0)
		{
			g_NetAdhocctlConnect = 0 ;
		}
	}
	
	if(g_NetAdhocctlInit)
	{
		retVal = sceNetAdhocctlTerm();
		if(retVal == 0)
		{
			g_NetAdhocctlInit = 0 ;
		}
	}

	if(g_NetAdhocInit)
	{
		retVal = sceNetAdhocTerm();
		if(retVal == 0)
		{
			g_NetAdhocInit = 0 ;
		}
	}

	if(g_NetInit)
	{
		retVal = sceNetTerm();
		if(retVal == 0)
		{
			g_NetInit = 0 ;
		}
		else
		{

		}
	}

	g_AdhocConnect = 0 ;

    return 0; // assume it worked
}

void PSP_Adhoc_TermMatching()
{
	int retVal ;

	if(g_NetAdhocMatchingStart)
	{
		retVal = sceNetAdhocMatchingStop( g_MatchingID );
		if(retVal == 0)
		{
			g_NetAdhocMatchingStart = 0 ;
		}
	}

	if(g_NetAdhocMatchingCreate)
	{
		retVal = sceNetAdhocMatchingDelete( g_MatchingID );
		if(retVal == 0)
		{
			g_NetAdhocMatchingCreate = 0 ;
		}
	}
}

////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
//char temp[100];
//char debug[1000];

void matchingCallback(int unk1, int event, char *mac2, int optLen, char *optData)
{
	// This is called when there is an event, dont seem to be able to call
	// procs from here otherwise it causes a crash
	int n ;
			
	debug_adhoc = 20 + event ;

	if( event == MATCHING_JOINED )
	{
		//memcpy( g_mac_match, mac2, 6 ) ;
		for( n=0; n < 6; n++ ){ g_mac_match[n] = mac2[n] ; }
		g_mac_matching = 1 ;
	}
	else if( event == MATCHING_DISCONNECT )
	{
		g_mac_matching = 0 ;
		g_AdhocDisonnect = 1 ;
	}
	else
	{
		g_unk1        = unk1   ;
		g_matchEvent  = event  ;
		g_matchOptLen = optLen ;

		// Copy across the optional data
		//strncpy( g_matchOptData, optData, optLen );

		// And the MAC address
		//memcpy( g_mac, mac2, sizeof(char) * 6 );
		for( n=0; n < 6; n++ ){ g_mac[n] = mac2[n] ; }

		g_matchChanged = 1 ;
	}
}

////////////////////////////////////////////////////////////
int PSP_Adhoc_IsConnect () 
{ 
	return g_AdhocConnect ; //&& (g_AdhocDisonnect == 0) ; 
}

int PSP_Adhoc_Create ( char *MatchingData )
{
	struct productStruct product;

	if( g_AdhocLoaded == 0 )
	{
		return PSP_ADHOC_NOTLOAD ;
	}

	PSP_Adhoc_Term() ;

	g_mac_matching = 0 ;
	debug_adhoc = 1 ;

	strcpy( product.product, "ULUS99999" );
	product.unknown = 0;

    int err ;

	if( g_NetInit == 0 )
	{
		err = sceNetInit( 0x20000, 0x20, 0x1000, 0x20, 0x1000 );
		if (err != 0){ return err; }
		g_NetInit = 1 ;
	}

	if( g_NetAdhocInit == 0 )
	{
		err = sceNetAdhocInit();
		if (err != 0){ return err ; }
		g_NetAdhocInit = 1 ;
	}
	
	if( g_NetAdhocctlInit == 0 )
	{
		err = sceNetAdhocctlInit( 0x2000, 0x20, &product );
		if (err != 0){ return err; }
		g_NetAdhocctlInit = 1 ;
	}
	debug_adhoc = 2 ;

    // Connect
	if( g_NetAdhocctlConnect == 0 )
	{
		err = sceNetAdhocctlConnect("DGEN");
		//err = sceNetAdhocctlCreate("12345678");
		//if (err != 0)
		//	return err; // No error for FW 1.0
		g_NetAdhocctlConnect = 1 ;
	}

	debug_adhoc = 3 ;

	{
		unsigned long tm = sceKernelLibcClock() ;
		unsigned long pad_old = 0 ;
		int stateLast = -1;

		for(;;)
		{
			int state;
			err = sceNetAdhocctlGetState( &state );
			if (err != 0)
			{
				//sceKernelDelayThread(10*1000000); // 10sec to read before exit
				//return PSP_ADHOC_ERROR_NG;
			}
			if( state > stateLast )
			{
				stateLast = state;
			}
			if( state == 1 )
				break ;  // connected

			// wait a little before polling again
			sceKernelDelayThread( 100*1000 ); // 100ms

			CheckCancel() ;
			CheckTimeout() ;
		}
	}

	debug_adhoc = 4 ;

	g_mac_this[0] = 0 ;

	sceWlanGetEtherAddr( g_mac_this );

	if( g_NetAdhocPdpCreate == 0 )
	{
		g_PDPID = sceNetAdhocPdpCreate(
				 g_mac_this ,
				 0x309,		// 0x309 in lumines
				 0x400, 	// 0x400 in lumines
				 0);		// 0 in lumines

		if( g_PDPID <= 0 )
		{
			return PSP_ADHOC_ERROR_NG ;
		}
		g_NetAdhocPdpCreate = 1 ;
	}
	debug_adhoc = 6 ;

	if( g_NetAdhocMatchingInit == 0 )
	{
		err = sceNetAdhocMatchingInit(0x20000);
		if( err != 0 )
		{
			return PSP_ADHOC_ERROR_NG ;
		}
		g_NetAdhocMatchingInit = 1 ;
	}
	debug_adhoc = 7 ;

	if( g_NetAdhocMatchingCreate == 0 )
	{
		g_MatchingID = sceNetAdhocMatchingCreate( 
						3,
						0xa,
						0x22b,
						0x800,
						0x2dc6c0,
						0x5b8d80,
						3,
						0x7a120,
						(MatchingCallback)matchingCallback );

		if( g_MatchingID < 0 )
		{
			return PSP_ADHOC_ERROR_NG ;
		}
		g_NetAdhocMatchingCreate = 1 ;
	}
	debug_adhoc = 8 ;

	if( g_NetAdhocMatchingStart == 0 )
	{
		char tempStr[32];
		//tempStr[0] = '\0';
		memset( tempStr, 0, 32 ) ;

		//*
		if( MatchingData && strlen(MatchingData) )
		{
			strcpy( tempStr, MatchingData ) ;
		}
		else
		{

		}
		//*/

		err = sceNetAdhocMatchingStart(
				 g_MatchingID , 	// 1 in lumines (presuming what is returned from create)
				 0x10,				// 0x10
				 0x2000,			// 0x2000
				 0x10,				// 0x10
				 0x2000,			// 0x2000
				 strlen(tempStr)+1,
				 tempStr );		

		if( err != 0 )
		{
			return PSP_ADHOC_ERROR_NG ;
		}
		g_NetAdhocMatchingStart = 1 ;
	}
	debug_adhoc = 9 ;

	// All the init functions have passed
	return PSP_ADHOC_ERROR_OK ;
}

/*
int PSP_Adhoc_Reconnect( char *ssid )
{
	int err = 0;

	debug_adhoc = 41 ;

	// Disconnect Wifi
	if(g_NetAdhocctlConnect)
	{
		err = sceNetAdhocctlDisconnect();
		if(err != 0)
		{
		}
		g_NetAdhocctlConnect = 0 ;
	}

	if(g_NetAdhocPdpCreate)
	{
		err = sceNetAdhocPdpDelete(g_PDPID,0);
		if(err != 0)
		{
		}
		g_NetAdhocPdpCreate = 0 ;
	}

	if(g_NetAdhocMatchingStart)
	{
		err = sceNetAdhocMatchingStop( g_MatchingID );
		if(err != 0)
		{
		}
		g_NetAdhocMatchingStart = 0 ;
	}

	if(g_NetAdhocMatchingCreate)
	{
		err = sceNetAdhocMatchingDelete( g_MatchingID );
		if(err != 0)
		{
		}
		g_NetAdhocMatchingCreate = 0 ;
	}
	
	if(g_NetAdhocMatchingInit)
	{
		err = sceNetAdhocMatchingTerm();
		if(err != 0)
		{
		}
		g_NetAdhocMatchingInit = 0 ;
	}

	debug_adhoc = 42 ;

	{
		unsigned long tm = sceKernelLibcClock() ;
		unsigned long pad_old = 0 ;
		int stateLast = -1;
		while (1)
		{
			int state;
			err = sceNetAdhocctlGetState( &state );
			if (err != 0)
			{
				sceKernelDelayThread( 10*1000000 ); // 10sec to read before exit
				return -1;
			}
			if (state > stateLast)
			{
				stateLast = state;
			}
			if (state == 0)
				break;  // connected

			// wait a little before polling again
			sceKernelDelayThread( 50*1000 ); // 50ms

			CheckCancel() ;
			CheckTimeout() ;
		}
	}

	debug_adhoc = 43 ;

	// Now connect to the PSP using a new SSID
    // Connect
    err = sceNetAdhocctlConnect( ssid );
    if (err != 0)
	{
		//for(;;)sceDisplayWaitVblankStart();
        return err;
	}
	g_NetAdhocctlConnect = 1 ;

	debug_adhoc = 44 ;
	
	{
		unsigned long tm = sceKernelLibcClock() ;
		unsigned long pad_old = 0 ;
		int stateLast = -1;
		while (1)
		{
			int state;
			err = sceNetAdhocctlGetState(&state);
			if (err != 0)
			{
				sceKernelDelayThread( 10*1000000 ); // 10sec to read before exit
				return -1;
			}
			if (state > stateLast)
			{
				stateLast = state;
			}
			if (state == 1)
				break;  // connected

			// wait a little before polling again
			sceKernelDelayThread(50*1000); // 50ms

			CheckCancel() ;
			CheckTimeout() ;
		}
	}

	debug_adhoc = 45 ;

	char macAddr[10];
	sceWlanGetEtherAddr( macAddr );

	g_PDPID = sceNetAdhocPdpCreate(
			 macAddr,
		     0x309  ,   // 0x309 in lumines
		     0x800  ,   // 0x400 in lumines
		     0      ) ; // 0 in lumines

	if(g_PDPID <= 0)
	{
		return -1;
	}
	g_NetAdhocPdpCreate = 1 ;

	return PSP_ADHOC_ERROR_OK ;
}
*/


int PSP_Adhoc_Connect ( )
{
	char tempStr[100] ;
	char ssid   [ 10] ;
	char mac    [  6] ;
	int  n            ;

	unsigned long tm = sceKernelLibcClock() ;
	unsigned long pad_old = 0 ;

	debug_adhoc = 11 ;
	for(;;)
	{
		TRACE_ADHOC() ;

		if( g_mac_matching )
		{
			debug_adhoc = 12 ;

			for( n=0; n < 6; n++ ){ mac[n] = g_mac_match[n] ; }

			sceNetAdhocMatchingSelectTarget( g_MatchingID, mac, 0, 0 );

			//sceNetEtherNtostr( mac, tempStr );

			for(;;)
			{
				//sceNetAdhocMatchingSelectTarget( g_MatchingID, mac, 0, 0 );

				TRACE_ADHOC() ;

				if( g_matchChanged && (g_matchEvent == MATCHING_ESTABLISHED) )
				{
					debug_adhoc = 13 ;

					sceNetEtherNtostr( g_mac , tempStr );

					sprintf(ssid, "%c%c%c%c%c%c", 
							tempStr[ 9], tempStr[10] , 
							tempStr[12], tempStr[13] , 
							tempStr[15], tempStr[16] ) ;

					//return PSP_Adhoc_Reconnect( ssid ) ;

					PSP_Adhoc_TermMatching() ;

					g_AdhocConnect = 1 ;

					return PSP_ADHOC_ERROR_OK ;
				}

				sceKernelDelayThread( 100*1000 ) ;

				//if( (sceKernelLibcClock() - tm) > TIMEOUT_CONNECT ){ return PSP_ADHOC_TIMEOUT ; }
				CheckCancel() ;
			}
		}
		else
		{
			if( g_matchChanged && (g_matchEvent == MATCHING_SELECTED) )
			{
				debug_adhoc = 14 ;

				memcpy( mac, g_mac, 6 );

				sceNetEtherNtostr( mac, tempStr ) ;

				sceNetAdhocMatchingSelectTarget( g_MatchingID, mac, 0, 0 ) ;

				for(;;)
				{
					TRACE_ADHOC() ;

					if( g_matchChanged && (g_matchEvent == MATCHING_ESTABLISHED) )
					{
						debug_adhoc = 15 ;

						sceWlanGetEtherAddr( mac );
						sceNetEtherNtostr( mac, tempStr );

						sprintf(ssid, "%c%c%c%c%c%c", 
								tempStr[ 9], tempStr[10] , 
								tempStr[12], tempStr[13] , 
								tempStr[15], tempStr[16] ) ;

						//return PSP_Adhoc_Reconnect( ssid ) ;
						PSP_Adhoc_TermMatching() ;

						g_AdhocConnect = 1 ;

						return PSP_ADHOC_ERROR_OK ;
					}

					sceKernelDelayThread( 100*1000 ) ;

					//if( (sceKernelLibcClock() - tm) > TIMEOUT_CONNECT ){ return PSP_ADHOC_TIMEOUT ; }
					CheckCancel() ;
				}
			}
		}

		sceKernelDelayThread( 100*1000 ) ;

		//if( (sceKernelLibcClock() - tm) > TIMEOUT_CONNECT ){ return PSP_ADHOC_TIMEOUT ; }
		CheckCancel() ;
	}

	return PSP_ADHOC_ERROR_NG ;
}

int PSP_Adhoc_Send( void *buffer, int length )
{
	int err ;

	err = sceNetAdhocPdpSend( 
			g_PDPID   ,
			&g_mac[0] ,
			0x309     ,
			buffer    ,
			length    ,
			TIMEOUT_SEND, 
			0 ) ;
			//0         ,	   // 0 in lumines
			//1         ) ;  // 1 in lumines

	return err;
}

// Used to send the data and wait for an ack
/*
int PSP_Adhoc_SendRecvAck( void *buffer, int length )
{
	int err                ;
	int recvTemp  = 0      ;
	int recvLen   = 1      ;
	int tempLen   = length ;
	int sentCount = 0      ;

	do
	{
		if(tempLen > 0x400)
		{
			tempLen = 0x400;
		}

		err = PSP_Adhoc_Send( buffer, tempLen );

		if(err < 0)
			return err;

		// wait for an ack
		err = PSP_Adhoc_RecvBlocked( &recvTemp, &recvLen, 10 * 1000 * 1000 );
		if( err < 0 )
		{
			return err;
		}

		buffer    += 0x400 ;
		sentCount += 0x400 ;
		tempLen    = length - sentCount;

	} while( sentCount < length ) ;

	return err ;
}

// Used to receive data and then send an ack
int PSP_Adhoc_RecvSendAck( void *buffer, int *length )
{
	int temp      = 1       ;
	int err       = 0       ;
	int tempLen   = *length ;
	int rcvdCount = 0       ;

	do
	{
		if(tempLen > 0x400)
		{
			tempLen = 0x400;
		}

		err = PSP_Adhoc_RecvBlocked( buffer, length, 10 * 1000 * 1000 );
		if( err < 0 ){ return err; }

		err = PSP_Adhoc_Send( &temp, 1 ) ;

		rcvdCount += 0x400               ;
		buffer    += 0x400               ;
		tempLen    = *length - rcvdCount ;

	} while( rcvdCount < *length ) ;

	// wait for an ack

	return err ;
}
*/

/*

int PSP_Adhoc_Recv(void *buffer, int *length)
{
	char          mac[20] ;
	int           pdpStatLength = 20 ;
	int           pdpRecvLength = 0  ;
	int           nRet               ;
	int           port          = 0  ;
	pdpStatStruct pspStat            ;

	nRet = sceNetAdhocGetPdpStat( &pdpStatLength, &pspStat );
	if( nRet < 0 )
	{
		return nRet ;
	}

	if( pspStat.rcvdData > 0 )
	{		
		pdpRecvLength = pspStat.rcvdData ;

		nRet = sceNetAdhocPdpRecv( 
					g_PDPID ,
					mac,
					&port,
					buffer,
					length,
					0,  	// 0 in lumines
					1 );	// 1 in lumines

		if( nRet < 0 )
		{
			return nRet ;
		}
		else
		{
			return 1 ;
		}
	}

	return 0 ;
}

int PSP_Adhoc_RecvBlocked( void *buffer, int *length, unsigned long timeout )
{
	char mac[20] ;
	int  nRet    ;

	unsigned long tm = sceKernelLibcClock() ;

	for(;;)
	{
		nRet = PSP_Adhoc_Recv( buffer, length ) ;

		debug_tm[4] = sceKernelLibcClock() - tm ;

		if( nRet == 1 ){ return PSP_ADHOC_ERROR_OK ; }

		DELAY_RECV();

		if( (sceKernelLibcClock() - tm) > timeout )
		{
			return PSP_ADHOC_TIMEOUT ;
		}
	} 

	return nRet ;
}
*/

//*
int PSP_Adhoc_RecvBlocked( void *buffer, int *length, unsigned long timeout )
{
	pdpStatStruct pspStat            ;
	char          mac[20]            ;
//	int           nRet               ;
	int           pdpStatLength = 20 ;
//	int           pdpRecvLength = 0  ;
	int           port          = 0  ;
	unsigned long tm = sceKernelLibcClock() ;

	for(;;)
	{
//		if( sceNetAdhocGetPdpStat( &pdpStatLength, &pspStat ) >= 0 )
		{
//			if( pspStat.rcvdData > 0 )
			{
				//pdpRecvLength = pspStat.rcvdData ;

				if( sceNetAdhocPdpRecv( 
							g_PDPID ,
							mac,
							&port,
							buffer,
							length,
							TIMEOUT_RECV,
							0 ) 
							//0,  	// 0 in lumines
							//1 )		// 1 in lumines
					>= 0 )
				{
					return PSP_ADHOC_ERROR_OK ;
				}
			}
		}

		DELAY_RECV() ;

		if( (sceKernelLibcClock() - tm) > timeout )
		{
			return PSP_ADHOC_TIMEOUT ;
		}
	} 

	return PSP_ADHOC_ERROR_NG ;
}
//*/

/*
int PSP_Adhoc_RecvBlocked( void *buffer, int *length, unsigned long timeout )
{
	char          mac[20]    ;
	int           port  = 0  ;

	if( sceNetAdhocPdpRecv( 
				g_PDPID ,
				mac,
				&port,
				buffer,
				length,
				TIMEOUT_RECV,
				0 ) 
		>= 0 )
	{
		return PSP_ADHOC_ERROR_OK ;
	}

	return PSP_ADHOC_TIMEOUT ;
}
*/

////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////
// SLIME NOTE: Module library linkage trickery
//  we need to be in special kernel mode to load flash0: libraries
//   (created kernel thread + kernel memory module flag set)
//  for everything else program runs in user mode
//  the system does not patch the stubs properly, so we do it ourselves

// slightly cleaned up version

unsigned int PSP_Module_FindProc( unsigned int oid, unsigned int nid )
{
    SceModule* modP = (SceModule*)sceKernelFindModuleByUID(oid);
    if (modP == NULL)
    {
        //printf("ERROR: Failed to find mod $%x\n", oid);
        return 0;
    }
    SceLibraryEntryTable* entP = (SceLibraryEntryTable*)modP->ent_top;
    while ((unsigned int)entP < ((unsigned int)modP->ent_top + modP->ent_size))
    {
        int count = entP->stubcount + entP->vstubcount;
        unsigned int* nidtable = (unsigned int*)entP->entrytable;
        int i;
        for (i = 0; i < count; i++)
        {
            if (nidtable[i] == nid)
            {
                unsigned int procAddr = nidtable[count+i];
                return procAddr;
            }
        }
        entP++;
    }
    return 0;
}

int PSP_Module_PatchEntry( SceModuleInfo* modInfoPtr, SceUID oidLib )
{
    //dumb version
    int nPatched = 0;

    int* stubPtr; // 20 byte structure
    for (stubPtr = (int*)modInfoPtr->stub_top;
        stubPtr + 5 <= (int*)modInfoPtr->stub_end;
        stubPtr += 5)
    {
        int count = (stubPtr[2] >> 16) & 0xFFFF;
        int* idPtr = (int*)stubPtr[3];
        int* procPtr = (int*)stubPtr[4];

        if (stubPtr[1] != 0x90000)
            continue;   // skip non-lazy loaded modules
        while (count--)
        {
            if (procPtr[0] == 0x54C && procPtr[1] == 0)
            {
                // SWI - scan for NID
                unsigned int proc = PSP_Module_FindProc( oidLib, *idPtr );
                if (proc != 0)
                {
                    if (((unsigned int)procPtr & 0xF0000000) != (proc & 0xF0000000))
                    {
                        // if not in user space we can't use it
                        //dprintf("!! NOT PATCH\n");
                    }
                    else
                    {
                        unsigned int val = (proc & 0x03FFFFFF) >> 2;
                        procPtr[0] = 0x0A000000 | val;
                        procPtr[1] = 0;
                        
                        nPatched++;
                    }
                }
            }
            idPtr++;
            procPtr += 2; // 2 opcodes
        }
    }

    sceKernelDcacheWritebackAll ();
    sceKernelIcacheInvalidateAll();

    return nPatched;
}


SceUID PSP_Module_LoadStart(SceModuleInfo* modInfoPtr, const char* szFile, const char* szFolder)
{
	char path[128] ;

	if( szFolder && (strlen( szFolder ) > 0) )
	{
		strcpy( path, szFolder ) ;
		strcat( path, szFile   ) ;

		//int fd = sceIoOpen( path, 0x0001, 0777 ) ;
		//if( fd >= 0 ){ sceIoClose( fd ) ; }
		//else{ return -1 ; }
	}

	int  bMS = strncmp( path, "ms0:/", 5 ) ; 


    SceUID modID       ;
	int    retVal      ;
    int    startStatus ;

	if( modInfoPtr == NULL ){ return -1 ; }

	/*
	SceKernelLMOption opt ;
	memset( &opt, 0, sizeof(opt) ) ;
	opt.size     = sizeof(opt)     ;
	opt.mpidtext = mpid ;
	opt.mpiddata = mpid ;
	opt.position = 0    ;
	opt.access   = 1    ;
	*/

    //modID = sceKernelLoadModule( szFile, 0, &opt );

	if( bMS == 0 )
	{
	    //modID = sceKernelLoadModuleMs( path, 0, &opt );
	    modID = sceKernelLoadModule( path, 0, 0 );
	}
	else
	{
	    modID = sceKernelLoadModule( path, 0, 0 );
	}
    if( modID < 0 ){ return modID ; }

    retVal = sceKernelStartModule( modID, 0, 0, &startStatus, 0);
    if( retVal < 0 )
	{ 
		sceKernelUnloadModule( modID ) ;
		return retVal ; 
	}

    if( PSP_Module_PatchEntry( modInfoPtr, modID ) == 0 ){ ; } //return -1 ; }

    return modID ;
}


////////////////////////////////////////////////////////////
