///////////////////////////////////////////////////////////////////////////
/// psp_comm.c
///////////////////////////////////////////////////////////////////////////
#include "psp_comm.h"
///////////////////////////////////////////////////////////////////////////
#include "psp_main.h"
#include "psp_screen.h"
#include "psp_pad.h"
#include "psp_menu.h"
#include "emu_state.h"
///////////////////////////////////////////////////////////////////////////
#define _USE_ADHOC
///////////////////////////////////////////////////////////////////////////
#define PSP_COMM_VERSION  (4)
///////////////////////////////////////////////////////////////////////////
const char  SZ_COMM_CONNECT[] = "Connect..."                                          ;
const char  SZ_COMM_CANCEL [] = "< Anykey : Cancel connection >"                      ;
const char  SZ_COMM_CANCEL2[] = "< Anykey(long time push!) : Cancel connection >"     ;
const char  SZ_COMM_TITLE  [] = "Connection error!"                                   ;
const char  SZ_COMM_MSG    [] = "< ○ : Retry / Anykey : Return menu >"               ;
const char  SZ_COMM_SYNC   [] = "Initialize data..."                                  ;
      char* SZ_COMM_ERROR_J[] = { 
								"通信 API のエラーです。"                              ,
								"通信タイムアウトです。"                               ,
								"リトライ回数のエラーです。"                           ,
								"通信がキャンセルされました。"                         ,
								"DGEN の通信バージョンが違います。"                    ,
								"ロードされたロムが違います。"                         ,
								"通信モード設定が正しくありません。"                   ,
								"通信が正しく行われませんでした。再試行してください。" 
								} ;

  char* SZ_COMM_ERROR_E[] = { 
								//"[ No Error ]"             , // COMM_ERROR_NON     ,
								//"[ OK ]"                   , // COMM_ERROR_OK      ,
								"Connection API error"           , // COMM_ERROR_NG      ,
								"Connection timeout"             , // COMM_ERROR_TIMEOUT ,
								"Max retry count"                , // COMM_ERROR_RETRY   ,
								"Connection is canceled"         , // COMM_ERROR_CANCEL  
								"DGEN Version isn't same."       , // COMM_ERROR_VERSION    
								"Loaded rom isn't same."         , // COMM_ERROR_CHECKSUM   
								"Connection mode isn't correct." , // COMM_ERROR_MODE       
								"Unknown error, please retry."     // COMM_ERROR_CONNECTION1
								/*
								"[ Connection error (2) ]"           , // COMM_ERROR_CONNECTION2
								"[ Connection error (3) ]"           , // COMM_ERROR_CONNECTION3
								"[ Connection error (4) ]"           , // COMM_ERROR_CONNECTION4
								"[ Connection error (5) ]"             // COMM_ERROR_CMD        
								*/
								 } ;

///////////////////////////////////////////////////////////////////////////
#define COMM_PAD_NON     (0xFFFF)
int                      comm_cmd    ;
int                      comm_server ;
PSP_COMM_DATA            comm_data_s ;
unsigned long            comm_data_r ;
unsigned long            comm_pad_s  ;
unsigned long            comm_pad_r1 ; // recv pad1
unsigned long            comm_pad_r2 ; // recv pad2
int                      comm_error  ;
///////////////////////////////////////////////////////////////////////////
/// error codes
enum
{
	COMM_ERROR_EX          = 100 ,
	COMM_ERROR_VERSION     ,
	COMM_ERROR_CHECKSUM    ,
	COMM_ERROR_MODE        ,
	COMM_ERROR_CONNECTION1 ,
	COMM_ERROR_CONNECTION2 ,
	COMM_ERROR_CONNECTION3 ,
	COMM_ERROR_CONNECTION4 ,
	COMM_ERROR_CMD         ,
	MAX_COMM_ERROR_EX       
} ;
///////////////////////////////////////////////////////////////////////////
#define SET_ERROR(code)  comm_error=code
///////////////////////////////////////////////////////////////////////////
inline int           PSP_Comm_GetCmd        (){ return comm_cmd         ; }
inline unsigned long PSP_Comm_GetSyncData_S (){ return comm_data_s.data ; }
inline unsigned long PSP_Comm_GetSyncData_R (){ return comm_data_r      ; }
inline unsigned long PSP_Comm_GetPad_R1     (){ return comm_pad_r1      ; }
inline unsigned long PSP_Comm_GetPad_R2     (){ return comm_pad_r2      ; }
inline unsigned long PSP_Comm_GetPad_S      (){ return comm_pad_s       ; }
inline int  PSP_Comm_SetSyncData ( unsigned long data ){ comm_data_s.data = data ; }
///////////////////////////////////////////////////////////////////////////
extern EMU_CONFIG*     emc  ;
extern EMU_SYS_CONFIG* emsc ;
///////////////////////////////////////////////////////////////////////////

int PSP_Comm_Connect ( int bServer )
{
	if( PSP_Comm_IsConnect() ){ return COMM_ERROR_OK ; }

	comm_data_s.hdr.len = sizeof(PSP_COMM_DATA) ;

	//for(;;)
	{
		Menu_DrawBG_1X() ;

		PSP_Screen_DrawText( 
			(PSP_SCREEN_WIDTH - sizeof(SZ_COMM_CONNECT)*PSP_FONT_WIDTH)/2 ,
			PSP_SCREEN_HEIGHT / 2 - PSP_FONT_HEIGHT ,
			SZ_COMM_CONNECT, emsc->color_text ) ;

		PSP_Screen_DrawText( 
			(PSP_SCREEN_WIDTH - sizeof(SZ_COMM_CANCEL)*PSP_FONT_WIDTH)/2 ,
			PSP_SCREEN_HEIGHT - PSP_FONT_HEIGHT * 3 ,
			SZ_COMM_CANCEL, emsc->color_text ) ;

		sceKernelDcacheWritebackAll();

		PSP_Screen_WaitVsync() ;
		PSP_Screen_Flip() ;

		comm_error = PSP_Comm_Init( bServer ) ;

		/*
		if( comm_error == COMM_ERROR_OK ){ return COMM_ERROR_OK ; }
		else 
		{
			if( PSP_Comm_ShowError( COMM_SHOW_CONNECT ) != COMM_ERROR_OK )
			{
				//break ;
			}
		}
		//*/
	}

	return comm_error ;
}

//#define _DEBUG_COMM
#ifdef  _DEBUG_COMM
//* for Debug
extern int  g_NetInit                 ;
extern int  g_NetAdhocInit            ;
extern int  g_NetAdhocctlInit         ;
extern int  g_NetAdhocctlConnect      ;
extern int  g_NetAdhocPdpCreate       ;
extern int  g_NetAdhocMatchingInit    ;
extern int  g_NetAdhocMatchingCreate  ;
extern int  g_NetAdhocMatchingStart   ;
extern char g_mac_this[8]             ;
extern char path_exec [128]           ;
extern int  g_AdhocLoaded             ;
extern char path_adhoc[128]           ;
extern int  debug_adhoc               ;
extern char g_mac_match[6]            ;
extern int  g_PDPID                   ;
extern int  g_MatchingID              ;
//*/
#endif // _DEBUG_COMM

int PSP_Comm_ShowError ( int nError )
{
	Menu_DrawBG_1X() ;

	for(;;)
	{
		PSP_Ctrlpad_Update() ;
		if( PSP_Ctrlpad_GetButtons() == 0 ){ break ; }
	}

	int nCode = comm_error ;
	unsigned long buttons_new, buttons_old = 0 ;
	unsigned long tm_ed, tm_pd = 0 ; 

	for(;;)
	{
		PSP_Screen_DrawText( 
			(PSP_SCREEN_WIDTH - sizeof(SZ_COMM_TITLE)*PSP_FONT_WIDTH)/2 ,
			PSP_SCREEN_HEIGHT / 2 - PSP_FONT_HEIGHT * 2 ,
			SZ_COMM_TITLE, emsc->color_text ) ;

		PSP_Screen_DrawText( 
			(PSP_SCREEN_WIDTH - sizeof(SZ_COMM_MSG)*PSP_FONT_WIDTH)/2 ,
			PSP_SCREEN_HEIGHT - PSP_FONT_HEIGHT * 3 ,
			SZ_COMM_MSG, emsc->color_msg ) ;

		{
			int sel = -1 ;

			switch( comm_error )
			{
			case COMM_ERROR_NG          : sel = 0 ; break ;
			case COMM_ERROR_TIMEOUT     : sel = 1 ; break ;
			case COMM_ERROR_RETRY       : sel = 2 ; break ;
			case COMM_ERROR_CANCEL      : sel = 3 ; break ;
			case COMM_ERROR_VERSION     : sel = 4 ; break ;
			case COMM_ERROR_CHECKSUM    : sel = 5 ; break ;
			case COMM_ERROR_MODE        : sel = 6 ; break ;
			case COMM_ERROR_CONNECTION1 : 
			case COMM_ERROR_CONNECTION2 : 
			case COMM_ERROR_CONNECTION3 : 
			case COMM_ERROR_CONNECTION4 : 
			case COMM_ERROR_CMD         : sel = 7 ; break ;
			}

			char buff[64] ; 
			if( sel >= 0 )
			{
				strcpy( buff, "[ " ) ;
				strcat( buff, SZ_COMM_ERROR_J[sel] ) ; 
				strcat( buff, " ]" ) ;
				int len = strlen( buff ) ;

				PSP_Screen_DrawText( 
					(PSP_SCREEN_WIDTH - len * PSP_FONT_WIDTH)/2 ,
					PSP_SCREEN_HEIGHT / 2  ,
					buff, emsc->color_msgbox ) ;

				strcpy( buff, "[ " ) ;
				strcat( buff, SZ_COMM_ERROR_E[sel] ) ; 
				strcat( buff, " ]" ) ;
				len = strlen( buff ) ;

				PSP_Screen_DrawText( 
					(PSP_SCREEN_WIDTH - len * PSP_FONT_WIDTH)/2 ,
					PSP_SCREEN_HEIGHT / 2 + PSP_FONT_HEIGHT + 1 ,
					buff, emsc->color_msgbox ) ;
			}
		}

#ifdef  _DEBUG_COMM
		//*
		int line = 30 ;
		int px   = 30 ;
		//PSP_Screen_DrawText( 0, line, path_exec               , 0xFFFF ) ; line += 10 ;
		//PSP_Screen_DrawText( 0, line, path_adhoc              , 0xFFFF ) ; line += 10 ;
		PSP_Screen_DrawHex2( px, line, g_mac_match[0]          , 0xFFFF ) ; line += 10 ;
		PSP_Screen_DrawHex2( px, line, g_mac_match[1]          , 0xFFFF ) ; line += 10 ;
		PSP_Screen_DrawHex2( px, line, g_mac_match[2]          , 0xFFFF ) ; line += 10 ;
		PSP_Screen_DrawHex2( px, line, g_mac_match[3]          , 0xFFFF ) ; line += 10 ;
		PSP_Screen_DrawHex2( px, line, g_mac_match[4]          , 0xFFFF ) ; line += 10 ;
		PSP_Screen_DrawHex2( px, line, g_mac_match[5]          , 0xFFFF ) ; line += 10 ;
		line += 10 ;
		//*/

		//*
		//int line = 0 ;
		PSP_Screen_DrawInt2( px, line, comm_error              , 0xFFFF ) ; line += 10 ;
		PSP_Screen_DrawInt2( px, line, comm_cmd                , 0xFFFF ) ; line += 10 ;
		PSP_Screen_DrawInt2( px, line, debug_adhoc             , 0xFFFF ) ; line += 10 ;
		PSP_Screen_DrawInt2( px, line, g_AdhocLoaded           , 0xFFFF ) ; line += 10 ;
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
		//*/
#endif //  _DEBUG_COMM

		/*
		//if( g_NetAdhocctlConnect )
		{
			int state;
			int err = sceNetAdhocctlGetState( &state );
			if( err   != 0 ){  ; }
			if( state == 0 ){  ; }

			PSP_Screen_DrawInt2( 0, line, err   , 0xFFFF ) ; line += 10 ;
			PSP_Screen_DrawInt2( 0, line, state , 0xFFFF ) ; line += 10 ;
		}
		*/

		PSP_Screen_WaitVsync() ;
		PSP_Screen_Flip() ;

		//tm_ed   = sceKernelLibcClock() ;

		//if( (tm_ed - tm_pd) > TM_1FRAME )
		{
		//	tm_pd = tm_ed ;
			PSP_Ctrlpad_Update() ;

			buttons_new = PSP_Ctrlpad_GetButtons() ;

			if( (buttons_new & CTRL_CIRCLE) == 0 )
			{
				if( buttons_old & CTRL_CIRCLE )
				{ 
					return COMM_ERROR_OK ;
				}
				else if( buttons_old )
				{
					return COMM_ERROR_NG ;
				}
			}

			buttons_old = buttons_new ;
		}

		PSP_Power_CheckSleep() ;
	}

	return COMM_ERROR_NG ;
}

int PSP_Comm_SyncState ( int bServer, int bCancel )
{
	//comm_pad_s.frame = 0 ;

	//return COMM_ERROR_OK ;

	int buff_id = sceKernelAllocPartitionMemory(2, "buff", 0, sizeof(EMU_STATE_SLOT), NULL ) ;

	if( buff_id < 0 ){ return COMM_ERROR_NG ; }

	int n ;
	int nRet = COMM_ERROR_NG ;
	PSP_COMM_INIT init ;

	unsigned char* comm_buff_r = (unsigned char*)sceKernelGetBlockHeadAddr( buff_id ) ;
	EMU_STATE_SLOT* ess = (EMU_STATE_SLOT*)comm_buff_r ;

	if( ess == 0 ){ return COMM_ERROR_NG ; }

#define MAX_RETRY   (5)

#define _USE_GUI
#ifdef  _USE_GUI
#define PROGRESS_L  (PSP_SCREEN_WIDTH  / 4    )
#define PROGRESS_T  (PSP_SCREEN_HEIGHT - PSP_FONT_HEIGHT * 6)
#define PROGRESS_R  (PSP_SCREEN_WIDTH  * 3 / 4)
#define PROGRESS_B  (PSP_SCREEN_HEIGHT - PSP_FONT_HEIGHT * 4)
#define PROGRESS_W  (PROGRESS_R - PROGRESS_L)
#define PROGRESS_CR (emsc->color_text)
#define PROGRESS_CF (PSP_RGB(  0, 64,255))
#endif //_USE_GUI

#define  CHECK_CANCEL() \
			if( bCancel ) \
			{\
				PSP_Ctrlpad_Update() ; \
				unsigned long pad = PSP_Ctrlpad_GetButtons() ;\
				if( pad ){ nRet = COMM_ERROR_CANCEL ; SET_ERROR( COMM_ERROR_CANCEL ) ; goto _Ret ; }\
			}


	#ifdef  _USE_GUI
	{ // GUI
		//PSP_Screen_SetVBlankInt( 0 ) ;
		//PSP_Screen_WaitVsync() ;
		//PSP_Screen_WaitVsync() ;
		//PSP_Screen_Flip() ;

		Menu_DrawBG_1X() ;

		for( n=0; n < 2; n++ )
		{
			PSP_Screen_DrawText( 
				(PSP_SCREEN_WIDTH - sizeof(SZ_COMM_SYNC)*PSP_FONT_WIDTH)/2 ,
				PSP_SCREEN_HEIGHT / 2 - PSP_FONT_HEIGHT ,
				SZ_COMM_SYNC, emsc->color_msg ) ;

			if( bCancel )
			{
				PSP_Screen_DrawText( 
					(PSP_SCREEN_WIDTH - sizeof(SZ_COMM_CANCEL2)*PSP_FONT_WIDTH)/2 ,
					PSP_SCREEN_HEIGHT - PSP_FONT_HEIGHT * 3 ,
					SZ_COMM_CANCEL2, emsc->color_msg ) ;
			}

			PSP_Screen_Rectangle( PROGRESS_L, PROGRESS_T, PROGRESS_R, PROGRESS_B, PROGRESS_CR ) ;

			PSP_Screen_WaitVsync() ;
			PSP_Screen_Flip() ;
		}
	}
	#endif //_USE_GUI

	for(;;)
	{
		PSP_Ctrlpad_Update() ;
		if( PSP_Ctrlpad_GetButtons() == 0 ){ break ; }
		PSP_Screen_WaitVsync() ;
	}

	if( bServer )
	{
		md_get_state( ess ) ;

		int size = EmuState_Compress( ess, sizeof(EMU_STATE_SLOT), sizeof(PSP_COMM_HEADER) ) ;

		if( size > 0 )
		{
			// send
			{ // size
				init.hdr.cmd = COMM_CMD_INIT         ;
				init.hdr.len = sizeof(PSP_COMM_INIT) ;
				init.version = PSP_COMM_VERSION      ;
				init.chksum  = md_get_checksum()     ;
				init.size    = size                  ;
				init.emc     = *emc                  ;

				for( n=0; !bCancel || (n < MAX_RETRY) ; n++ )
				{
					sceKernelDelayThread(1000) ;

					if( PSP_Comm_SyncData( &init, comm_buff_r, 1 ) != COMM_ERROR_OK )
					{
						//goto _Ret ;
					}

					if( (int)((PSP_COMM_HEADER*)comm_buff_r)->cmd == COMM_CMD_INIT )
					{
						break ;
					}

					CHECK_CANCEL() ;
		
					PSP_Power_CheckSleep() ;
				}

				if( !bCancel && (n >= MAX_RETRY) )
				{ 
					SET_ERROR( COMM_ERROR_TIMEOUT ) ; 
					goto _Ret ; 
				}

				if( init.version != ((PSP_COMM_INIT*)comm_buff_r)->version )
				{ 
					SET_ERROR( COMM_ERROR_VERSION ) ; 
					goto _Ret ; 
				}

				if( init.chksum != ((PSP_COMM_INIT*)comm_buff_r)->chksum )
				{ 
					SET_ERROR( COMM_ERROR_CHECKSUM ) ; 
					goto _Ret ; 
				}

				if( init.emc.comm_mode == ((PSP_COMM_INIT*)comm_buff_r)->emc.comm_mode )
				{ 
					SET_ERROR( COMM_ERROR_MODE ) ;
					goto _Ret ; 
				}
			}

			{ // data
				int            size_t = size ;
				unsigned char* buff_t = EmuState_GetBuffer() ;

				n=0;
				for(;;)
				{
					//sceKernelDelayThread( 1000 ) ;

					((PSP_COMM_HEADER*)buff_t)->cmd = COMM_CMD_INIT_D ;

					if( size_t > MAX_COMM_BUFF_S )
					{
						((PSP_COMM_HEADER*)buff_t)->len = sizeof(PSP_COMM_HEADER) + MAX_COMM_BUFF_S ;
					}
					else
					{
						((PSP_COMM_HEADER*)buff_t)->len = sizeof(PSP_COMM_HEADER) + size_t ;
					}

					if( PSP_Comm_SyncData( buff_t, comm_buff_r, 1 ) != COMM_ERROR_OK )
					{
						SET_ERROR( COMM_ERROR_CONNECTION1 ) ; 
						goto _Ret ;
					}

					size_t -= MAX_COMM_BUFF_S ;
					buff_t += MAX_COMM_BUFF_S ;
					if( size_t <= 0 ){ break ; }

					#ifdef  _USE_GUI
					if( n & 1 )
					{ // GUI
						PSP_Screen_FillRect( PROGRESS_L+1, PROGRESS_T+1, 
							PROGRESS_L + 1 + (PROGRESS_W-2) * (size - size_t) / size , 
							PROGRESS_B , PROGRESS_CF ) ;

						//PSP_Screen_WaitVsync() ;
						PSP_Screen_Flip() ;
					}
					n++;
					#endif // _USE_GUI
				}
			}

			{ // end ( x2 )
				PSP_COMM_HEADER init ;
				init.cmd = COMM_CMD_INIT_E         ;
				init.len = sizeof(PSP_COMM_HEADER) ;

				for( n=0; n < 2 ; n++ )
				{
					if( PSP_Comm_SyncData( &init, comm_buff_r, 1 ) != COMM_ERROR_OK )
					{
						SET_ERROR( COMM_ERROR_CONNECTION2 ) ;
						goto _Ret ;
					}

					if( (int)((PSP_COMM_HEADER*)comm_buff_r)->cmd == COMM_CMD_ERROR )
					{
						SET_ERROR( COMM_ERROR_CONNECTION3 ) ;
						goto _Ret ;
					}
				}
			}

			emc->show_flags |=  FLAG_Z80ASYNC ;

			nRet = COMM_ERROR_OK ;
		}
	}
	else
	{
		int size, size_t = 0 ;

		{ // recv
			{ // size
				init.hdr.cmd = COMM_CMD_INIT         ;
				init.hdr.len = sizeof(PSP_COMM_INIT) ;
				init.version = PSP_COMM_VERSION      ;
				init.chksum  = md_get_checksum()     ;
				init.size    = 0                     ;
				init.emc     = *emc                  ;

				for( n=0; !bCancel || (n < MAX_RETRY) ; n++ )
				{
					sceKernelDelayThread(1000) ;

					if( PSP_Comm_SyncData( &init, comm_buff_r, 0 ) != COMM_ERROR_OK )
					{
						//goto _Ret ;
					}

					if( (int)((PSP_COMM_HEADER*)comm_buff_r)->cmd == COMM_CMD_INIT )
					{
						size = ((PSP_COMM_INIT*)comm_buff_r)->size ;
						break ;
					}

					CHECK_CANCEL() ;
		
					PSP_Power_CheckSleep() ;
				}

				if( !bCancel && (n >= MAX_RETRY) )
				{ 
					SET_ERROR( COMM_ERROR_TIMEOUT ) ; 
					goto _Ret ; 
				}

				if( init.version != ((PSP_COMM_INIT*)comm_buff_r)->version )
				{ 
					SET_ERROR( COMM_ERROR_VERSION ) ;
					goto _Ret ; 
				}

				if( init.chksum != ((PSP_COMM_INIT*)comm_buff_r)->chksum )
				{ 
					SET_ERROR( COMM_ERROR_CHECKSUM ) ;
					goto _Ret ; 
				}

				if( init.emc.comm_mode == ((PSP_COMM_INIT*)comm_buff_r)->emc.comm_mode )
				{ 
					SET_ERROR( COMM_ERROR_MODE ) ;
					goto _Ret ; 
				}

				memcpy( &init, comm_buff_r, sizeof(PSP_COMM_INIT) ) ;
			}

			{ // data

				PSP_COMM_HEADER hd ;
				hd.cmd = COMM_CMD_INIT_D         ;
				hd.len = sizeof(PSP_COMM_HEADER) ;

				unsigned char* buff_t = comm_buff_r + sizeof(PSP_COMM_HEADER) + MAX_COMM_BUFF_S ;

				n=0;
				for(;;)
				{
					//sceKernelDelayThread( 1000 ) ;

					if( PSP_Comm_SyncData( &hd, comm_buff_r, 0 ) != COMM_ERROR_OK )
					{
						SET_ERROR( COMM_ERROR_CONNECTION1 ) ;
						goto _Ret ;
					}

					switch( (int)((PSP_COMM_HEADER*)comm_buff_r)->cmd )
					{
					case COMM_CMD_INIT_D :
						{
							int len_r = (int)((PSP_COMM_HEADER*)comm_buff_r)->len - sizeof(PSP_COMM_HEADER) ;

							memcpy( buff_t, ((PSP_COMM_INIT_DATA*)comm_buff_r)->data, len_r ) ;

							buff_t += len_r ;
							size_t += len_r ;
						}
						break ;

					case COMM_CMD_INIT_E :
						{
							goto _ExitLoop ;
						}
						break ;
					}

					#ifdef  _USE_GUI
					if( n & 1 )
					{ // GUI
						PSP_Screen_FillRect( PROGRESS_L+1, PROGRESS_T+1, 
							PROGRESS_L + 1 + (PROGRESS_W-2) * size_t / size , 
							PROGRESS_B , PROGRESS_CF ) ;

						//PSP_Screen_WaitVsync() ;
						PSP_Screen_Flip() ;
					}
					n++ ;
					#endif // _USE_GUI
				}
			}

		_ExitLoop :

			if( size_t != size )
			{ 
				SET_ERROR( COMM_ERROR_CONNECTION2 ) ;
				goto _Ret ; 
			} 
		}

		sceKernelDcacheWritebackAll() ;

		int len_slot = EmuState_Uncompress( (comm_buff_r + sizeof(PSP_COMM_HEADER) + MAX_COMM_BUFF_S), size ) ;

		if( len_slot == sizeof(EMU_STATE_SLOT) )
		{
			emc->pad_option = init.emc.pad_option ;
			emc->country    = init.emc.country    ;
			if( init.emc.show_flags & FLAG_CORE )
				{ emc->show_flags |=  FLAG_CORE ; }
			else{ emc->show_flags &= ~FLAG_CORE ; }

			md_select_core( (emc->show_flags & FLAG_CORE) ? 1 : 0 ) ;

			/*
			if( init.emc.show_flags & FLAG_Z80ASYNC )
				{ emc->show_flags |=  FLAG_Z80ASYNC ; }
			else{ emc->show_flags &= ~FLAG_Z80ASYNC ; }
			*/

			emc->show_flags |=  FLAG_Z80ASYNC ;

			unsigned char* buff = EmuState_GetBuffer() ;

			md_set_state( (EMU_STATE_SLOT*)buff ) ;
	
			nRet = COMM_ERROR_OK ;
		}
		else
		{
			SET_ERROR( COMM_ERROR_CONNECTION3 ) ;
		}

		{ // end
			PSP_COMM_HEADER init ;
			init.cmd = (nRet == COMM_ERROR_OK) ? COMM_CMD_INIT_E : COMM_CMD_ERROR ;
			init.len = sizeof(PSP_COMM_HEADER) ;

			if( PSP_Comm_SyncData( &init, comm_buff_r, 0 ) != COMM_ERROR_OK )
			{
				SET_ERROR( COMM_ERROR_CONNECTION4 ) ;
				nRet = COMM_ERROR_NG ;
				goto _Ret ;
			}
		}
	}

_Ret :
	sceKernelFreePartitionMemory( buff_id ) ;
					
	#ifdef  _USE_GUI
	{ // GUI
		Menu_DrawBG() ;
	}
	#endif // _USE_GUI

	sceKernelDcacheWritebackAll();

	return nRet ;
}

int  PSP_Comm_SyncCmd ( unsigned short cmd, unsigned long data, int b1 )
{
	//*
	PSP_COMM_DATA pcd                   ;
	pcd.hdr.cmd = cmd                   ;
	pcd.hdr.len = sizeof(PSP_COMM_DATA) ;
	pcd.data    = data                  ;

	unsigned char comm_buff_r[ MAX_COMM_BUFF ] __attribute__((aligned(32))) ;

	comm_error = PSP_Comm_SyncData( &pcd, comm_buff_r, b1 ) ;

	if( comm_error == COMM_ERROR_OK )
	{

		return COMM_ERROR_OK ;
	}
	
	return COMM_ERROR_NG ;
	//*/

	/*
	PSP_Comm_StartSync( cmd, data, b1 ) ;

	return PSP_Comm_WaitSync() ;
	*/
}


int PSP_Comm_CheckCmd( void* get )
{
	comm_cmd = (int)((PSP_COMM_HEADER*)get)->cmd ;

	switch( comm_cmd )
	{
	case COMM_CMD_PAD1 :
		//if( ((PSP_COMM_DATA*)get)->frame < ((PSP_COMM_DATA*)set)->frame ){ return COMM_ERROR_RETRY ; }
		comm_pad_r1 = ((PSP_COMM_DATA*)get)->data ;
		comm_pad_s  = comm_data_s.data            ;
		break ;
	case COMM_CMD_PAD2 :
		//if( ((PSP_COMM_DATA*)get)->frame < ((PSP_COMM_DATA*)set)->frame ){ return COMM_ERROR_RETRY ; }
		comm_pad_r2 = ((PSP_COMM_DATA*)get)->data ;
		comm_pad_s  = comm_data_s.data            ;
		break ;
	case COMM_CMD_MSG :
		break ;

	default : 
		if( (int)((PSP_COMM_HEADER*)get)->cmd >= MAX_COMM_CMD )
		{
			SET_ERROR( COMM_ERROR_CMD ) ;
			return COMM_ERROR_NG ;
		}

		break ;
	}

	comm_data_r = ((PSP_COMM_DATA*)get)->data ;


	return COMM_ERROR_OK ;
}



///////////////////////////////////////////////////////////////////////////
#ifdef  _USE_ADHOC
///////////////////////////////////////////////////////////////////////////
#include "psp_adhoc.h"
///////////////////////////////////////////////////////////////////////////
#define TIMEOUT_COMM  (3 * 1000 * 1000)
#define TIMEOUT_COMM2 (1000 * 1000)
#define MATCH_KEY     "DGEN for PSP"
///////////////////////////////////////////////////////////////////////////
void PSP_Comm_Term()
{
	PSP_Adhoc_Term() ;
}

int PSP_Comm_IsConnect()
{
	return PSP_Adhoc_IsConnect() ;
}

int PSP_Comm_Init( int bServer )
{
	switch( PSP_Adhoc_Create( MATCH_KEY ) )
	{
	case PSP_ADHOC_ERROR_OK :
		{
			switch( PSP_Adhoc_Connect() )
			{
			case PSP_ADHOC_CANCEL   : return COMM_ERROR_CANCEL  ;
			case PSP_ADHOC_ERROR_OK : return COMM_ERROR_OK      ;
			case PSP_ADHOC_TIMEOUT  : return COMM_ERROR_TIMEOUT ;
			}
		}
		break ;
	case PSP_ADHOC_CANCEL : return COMM_ERROR_CANCEL ;
	}

	return COMM_ERROR_NG ;
}

/*
int PSP_Comm_StartSync ( unsigned short cmd, unsigned long data )
{
	comm_pad_r1        = COMM_PAD_NON ;
	comm_pad_r2        = COMM_PAD_NON ;
	comm_pad_s.data    = data         ;
	comm_pad_s.hdr.cmd = cmd          ;
	comm_pad_s.frame   ++             ;

	if( PSP_Adhoc_Send( &comm_pad_s, comm_pad_s.hdr.len ) == PSP_ADHOC_ERROR_OK )
	{
		return COMM_ERROR_OK ;
	}

	return COMM_ERROR_NG ;
}

int PSP_Comm_WaitSync ()
{
	unsigned char comm_buff_r[ MAX_COMM_BUFF ] __attribute__((aligned(32))) ;
	int len_r ;

	if( PSP_Adhoc_RecvBlocked( comm_buff_r, &len_r, TIMEOUT_COMM ) == PSP_ADHOC_ERROR_OK )
	{
		return PSP_Comm_CheckCmd( comm_buff_r ) ;
	}

	return COMM_ERROR_NG ;
}
*/


/////////////////////////////////////////////////////////////////////////////////
/// for Thread
///////////////////////////////////////////////////////////////////////////
int                      comm_thid   = -1 ;
volatile unsigned short  comm_flag   ;
void PSP_Comm_Thread( int args, void *argp ) ;
///////////////////////////////////////////////////////////////////////////

int PSP_Comm_InitThread  ()
{
	comm_flag           = 0 ;
	//comm_data.frame   = 0 ;
	comm_data_s.hdr.len = sizeof(PSP_COMM_DATA) ;

	if( comm_thid == -1 )
	{
		comm_thid = sceKernelCreateThread( "pspcomm", &PSP_Comm_Thread, 0x16, 0xFA0, 0x80000000, NULL ) ;

		if( comm_thid >= 0 )
		{
			sceKernelStartThread( comm_thid, 0,0 ) ;
		}
		else
		{
			comm_thid = -1 ;
			return -1 ;
		}
	}

	return comm_thid ;
}

void PSP_Comm_TermThread()
{
	/*
	if( comm_thid != -1 )
	{
		comm_flag = COMM_EXIT | COMM_REQ1 ;
		sceKernelWaitThreadEnd( comm_thid, NULL ) ;
	}
	*/
}

int PSP_Comm_StartSync ( unsigned short cmd, unsigned long data, int b1 )
{
	comm_data_s.data    = data           ;

	return PSP_Comm_StartSync2( cmd, b1 ) ;
}

int PSP_Comm_StartSync2 ( unsigned short cmd, int b1 )
{
	comm_pad_r1         = COMM_PAD_NON   ;
	comm_pad_r2         = COMM_PAD_NON   ;
	//comm_data_s.data    = data           ;
	comm_data_s.hdr.cmd = cmd            ;
	//comm_pad_s.frame   ++              ;
	comm_flag           = COMM_ERROR_NON ;
	comm_cmd            = cmd            ;
	comm_server         = b1             ;

	sceKernelWakeupThread( comm_thid ) ;

	sceKernelDelayThread( 1000 ) ;

	return COMM_ERROR_OK ;
}

int PSP_Comm_WaitSync()
{
	/*
	if( comm_flag == COMM_ERROR_NON )
	{
		for(;;)
		{
			sceKernelDelayThread( 1000 ) ;
			if( comm_flag != COMM_ERROR_NON ){ break ; }
		}
	}
	*/

	//PSP_Adhoc_SetDelay( 500 ) ;

	while( comm_flag == COMM_ERROR_NON )
	{
		sceKernelDelayThread( 1000 ) ;
	}

	return comm_flag ; //(comm_cmd == COMM_CMD_ERROR) ? COMM_ERROR_NG : COMM_ERROR_OK ;
}


// for Thread
void PSP_Comm_Thread( int args, void *argp )
{
	unsigned char comm_buff_r[ MAX_COMM_BUFF ] __attribute__((aligned(32))) ;

	for(;;)
	{
		if( comm_flag == COMM_ERROR_NON )
		{
			/*
			if(  )
			{
				sceKernelExitDeleteThread(0);
				return ;
			}
			*/

			//int b = (comm_cmd == COMM_CMD_PAD1) ? 1 : 0 ;

			comm_error = PSP_Comm_SyncData( &comm_data_s, comm_buff_r, comm_server ) ;

			comm_cmd = ((PSP_COMM_HEADER*)comm_buff_r)->cmd ;

			comm_flag = comm_error ; 

			sceKernelSleepThread() ;
		}
		else
		{
			sceKernelSleepThread() ;
			//sceKernelDelayThread( 4000 ) ;
		}
	}
}

int PSP_Comm_SyncData( void* set, void* get, int b1 )
{
	int len_s, len_r ;

	len_s = ((PSP_COMM_HEADER*)set)->len ;

	if( b1 )
	{
		if( PSP_Adhoc_Send( set, len_s ) == PSP_ADHOC_ERROR_OK )
		{
			//sceKernelDelayThread( 1000 ) ;
			//PSP_Adhoc_SetDelay( 1500 ) ;

			switch( PSP_Adhoc_RecvBlocked( get, &len_r, TIMEOUT_COMM ) )
			{
			case PSP_ADHOC_TIMEOUT  : return COMM_ERROR_TIMEOUT ;
			case PSP_ADHOC_ERROR_OK : return PSP_Comm_CheckCmd( get ) ;
			}
		}
	}
	else //if( cmd == COMM_CMD_PAD2 )
	{
		//sceKernelDelayThread( 1000 ) ;
		//PSP_Adhoc_SetDelay( 1000 ) ;

		switch( PSP_Adhoc_RecvBlocked( get, &len_r, TIMEOUT_COMM ) )
		{
		case PSP_ADHOC_TIMEOUT  : return COMM_ERROR_TIMEOUT ;
		case PSP_ADHOC_ERROR_OK : 
			{
				if( PSP_Adhoc_Send( set, len_s ) == PSP_ADHOC_ERROR_OK )
				{
					return PSP_Comm_CheckCmd( get ) ;
				}
			}
			break ;
		}
	}

	return COMM_ERROR_NG ;
}

///////////////////////////////////////////////////////////////////////////
#endif // _USE_ADHOC
///////////////////////////////////////////////////////////////////////////

