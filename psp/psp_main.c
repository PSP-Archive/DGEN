///////////////////////////////////////////////////////////////////////////
/// psp_main.c
///////////////////////////////////////////////////////////////////////////
#include "emu_core.h"
#include "psp_main.h"
#include "psp_screen.h"
#include "psp_menu.h"
#include "psp_pad.h"
#include "psp_ex.h"
///////////////////////////////////////////////////////////////////////////

#define _USE_SOUND_THREAD

#define THREAD_ATTR_MAIN   (0x80000000)
#define THREAD_ATTR_SOUND  (0x80000000)
//#define THREAD_ATTR_MAIN   (0x80004000)

///////////////////////////////////////////////////////////////////////////
#ifdef _USE_PSPSDK ///////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
#include <pspkernel.h>
#include <pspdebug.h>
#include <pspctrl.h>
#include <pspdisplay.h>

#ifdef _NON_KERNEL

PSP_MODULE_INFO("dgen_psp", 0, 1, 1);
PSP_MAIN_THREAD_ATTR(PSP_THREAD_ATTR_USER);

#else //_NON_KERNEL

PSP_MODULE_INFO("dgen_psp", 0x1000, 1, 1);
PSP_MAIN_THREAD_ATTR(0);

#endif // _NON_KERNEL

///////////////////////////////////////////////////////////////////////////
#endif //_USE_PSPSDK //////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
#ifdef _NON_KERNEL ///////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////

#define PSP_Adhoc_Init() 
#define PSP_Comm_Term() 

#else /////////////////////////////////////////////////////////////////////

#include "psp_comm.h"
#include "psp_adhoc.h"

///////////////////////////////////////////////////////////////////////////
#endif //_NON_KERNEL //////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
enum{ UPDATE_NORMAL, UPDATE_CFG, UPDATE_MENU, UPDATE_COMM_ERROR } ;
///////////////////////////////////////////////////////////////////////////
int           PSP_Power_Init           ( void ) ;
int           PSP_Power_CheckSleep     ( void ) ;
int           PSP_Sound_Init           ( void ) ;
int           PSP_Ctrlpad_Init         ( void ) ;
PSP_PAD_DATA* PSP_Ctrlpad_GetPadData   () ;
void          PSP_Sound_Enable         ( int nEnable );
///////////////////////////////////////////////////////////////////////////
#define MAX_PSP_SOUND_BUFFER   (PSP_SOUND_SAMPLE * (2+2) * 2)
#define MAX_PSP_VOLUME         (0x8000)
#define PSP_SOUND_SAMPLE_W     (PSP_SOUND_SAMPLE/2)

int psp_sound_vol = (0) ;

volatile int   psp_sound_terminate = 0 ;
volatile int   psp_sound_enable  ;
unsigned long  psp_sound_handle  ;
unsigned long  psp_sound_thid    ;
unsigned long  psp_sound_handle2 [2]  ;

 short   psp_sound_playbuf [2][ PSP_SOUND_SAMPLE * 2 ]  __attribute__((aligned(64))) ;
 short   psp_sound_playbuf2[2][ PSP_SOUND_SAMPLE     ]  __attribute__((aligned(64))) ;

int PSP_Sound_Update2( void ) ;

unsigned long do_proc_param[8] __attribute__((aligned(64)))  ;

///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
extern void* __moduleinfo ;
///////////////////////////////////////////////////////////////////////////
extern EMU_CONFIG* emc ;
///////////////////////////////////////////////////////////////////////////
#define _OVER_CLOCK 
//#define _DEBUG 


#ifdef _DEBUG
unsigned long tm_debug[16] ;
#endif //_DEBUG

//int debug1, debug2 ;

unsigned long key_menu  = CTRL_LTRIGGER ;
unsigned long tm_ed     = 0         ;
unsigned long tm_pd     = 0         ;
unsigned long tm_60     = 0         ;
unsigned long tm_st     = 0         ;
unsigned long tm_run    = TM_1FRAME ;
unsigned long tm_skip   = TM_1FRAME ;
         long tm_res    = 0         ;
unsigned char skip      = 0         ;
unsigned char skip_s    = 0         ;
unsigned char skip_t    = 0         ;
unsigned char fps       = 0         ;
unsigned char fps_last  = 0         ;
unsigned char turbo_on  = 0         ;
signed   char exec_cfg  = 0         ;
char          exec_cfg_msg[40]      ;
char          exec_key  = -1        ;
short         snd_outbank = 0 ;

unsigned long menu_keys[] =
{
	CTRL_LTRIGGER               ,
	CTRL_LTRIGGER | CTRL_START  ,
	CTRL_LTRIGGER | CTRL_SELECT ,
	CTRL_RTRIGGER               ,
	CTRL_RTRIGGER | CTRL_START  ,
	CTRL_RTRIGGER | CTRL_SELECT ,
	CTRL_START                  ,
	CTRL_SELECT                  
} ;

/*
inline void __wait( unsigned long tm )
{
	//unsigned long t ;

	//t = sceKernelLibcClock() ;
	//while( (sceKernelLibcClock() - t) < tm ){ ; }
	sceKernelDelayThread( tm ) ;
}
*/

#define __wait(tm)  sceKernelDelayThread(tm)

#define  MAX_TM_SKIP ( TM_1FRAME/4)
#define  MIN_TM_SKIP (-TM_1FRAME/4)

unsigned char calc_skip( unsigned long tm_frame )
{
	if( turbo_on )
	{
		if( skip_t < emc->skip_turbo ){ skip_t++ ; return 1 ; }
		skip_t = 0 ;
	}
	else if( skip_s == 0 )
	{ 
		tm_res += (long)(TM_60FRAME / emc->max_fps) - (long)tm_frame ;
		//skip_t = 0 ;
		if( tm_res > MAX_TM_SKIP )
		{
			__wait( tm_res - MAX_TM_SKIP ) ;
			tm_res = MAX_TM_SKIP ;
			tm_ed  = sceKernelLibcClock() ;
		}
		else if( tm_res < MIN_TM_SKIP ){ tm_res = MIN_TM_SKIP ; }
		return 0 ; 
	}
	else
	{
		switch( emc->skip_mode )
		{
		case SKIP_MODE_A :
			{
				tm_res += (long)(TM_60FRAME / emc->max_fps) - (long)tm_frame ;

				if( tm_res < 0 )
				{
					if( skip_t < skip_s ){ skip_t++ ; return 1; }
					skip_t = 0 ;
					if( tm_res < -TM_1FRAME ){ tm_res = -TM_1FRAME ; }
				}
				else if( tm_res > TM_1FRAME*3 )
				{
					__wait( tm_res - TM_1FRAME*3 ) ;
					tm_res = TM_1FRAME*3 ;
					tm_ed  = sceKernelLibcClock() ;
				}
			}
			break ;
		case SKIP_MODE_F :
			{
				tm_res += (long)(TM_60FRAME / emc->max_fps) - (long)tm_frame ;
				if( skip_t < skip_s ){ skip_t++ ; return 1; }
				skip_t = 0 ;
				if( tm_res > MAX_TM_SKIP )
				{
					__wait( tm_res - MAX_TM_SKIP ) ;
					tm_res = MAX_TM_SKIP ;
					tm_ed  = sceKernelLibcClock() ;
				}
				else if( tm_res < MIN_TM_SKIP ){ tm_res = MIN_TM_SKIP ; }
			}
			break ;
		case SKIP_MODE_R :
			{
				tm_res += (long)(TM_60FRAME / emc->max_fps) - (long)tm_frame ;
				if( skip_t < skip_s ){ skip_t++ ; return 0; }
				skip_t = 0 ;
				if( tm_res > MAX_TM_SKIP )
				{
					__wait( tm_res - MAX_TM_SKIP ) ;
					tm_res = MAX_TM_SKIP ;
					tm_ed  = sceKernelLibcClock() ;
				}
				else if( tm_res < MIN_TM_SKIP ){ tm_res = MIN_TM_SKIP ; }

				return 1 ;
			}
			break ;
		}
	}

	return 0 ;
}

inline void resize_image_320to80( unsigned short* src, unsigned short* dst )
{
	int x, y ;

	for( y=0; y < 56 ; y++ )
	{
		for( x=0; x < 80 ; x++ )
		{
			dst[x] = src[x*4] ;
		}

		dst += 80 ;
		src += PSP_LINESIZE * 4 ;
	}
}

int EmuCfg_ExecKey( int pad_cfg )
{
	switch( pad_cfg )
	{
	case MD_PAD_SKIP_U    : 
		emc->skip_count++  ; 
		if(emc->skip_count > MAX_FRAME_SKIP){ emc->skip_count = MAX_FRAME_SKIP ; } 
		Menu_GetMsg_Skip( exec_cfg_msg ) ;
		break ;
	case MD_PAD_SKIP_D    : 
		emc->skip_count--  ; 
		if(emc->skip_count < MIN_FRAME_SKIP){ emc->skip_count = MIN_FRAME_SKIP ; } 
		Menu_GetMsg_Skip( exec_cfg_msg ) ;
		break ;
	case MD_PAD_SCREEN    : 
		emc->screen_mode++ ; 
		if(emc->screen_mode > SCREEN_FIT43){ emc->screen_mode = SCREEN_1X ; } 
		Menu_DrawBG() ;
		Menu_GetMsg_Screen( exec_cfg_msg ) ;
		break ;
	case MD_PAD_FPS       : 
		emc->show_fps++ ; 
		if( emc->show_fps > SHOW_FPS_RB ){ emc->show_fps = 0 ; } 
		Menu_DrawBG() ;
		Menu_GetMsg_FPS( exec_cfg_msg ) ;
		break ;
	case MD_PAD_VSYNC     : 
		emc->show_flags ^= SHOW_VSYNC ;
		Menu_GetMsg_Vsync( exec_cfg_msg ) ;
		break ;
	case MD_PAD_SOUND_U   : 
		emc->sound_on++ ; 
		if( emc->sound_on > MD_SOUND_44K ){ emc->sound_on = MD_SOUND_44K ; }
		Menu_GetMsg_SoundMode( exec_cfg_msg ) ;
		break ;
	case MD_PAD_SOUND_D   : 
		emc->sound_on-- ; 
		if( emc->sound_on < MD_SOUND_OFF ){ emc->sound_on = MD_SOUND_OFF ; }
		Menu_GetMsg_SoundMode( exec_cfg_msg ) ;
		break ;
	case MD_PAD_SOUND_V_U : 
		emc->sound_vol++ ;
		if( emc->sound_vol > VOL_16 ){ emc->sound_vol = VOL_16 ; }
		Menu_GetMsg_SoundVol( exec_cfg_msg ) ;
		break ;
	case MD_PAD_SOUND_V_D : 
		emc->sound_vol-- ;
		if( emc->sound_vol < VOL_08 ){ emc->sound_vol = VOL_08 ; }
		Menu_GetMsg_SoundVol( exec_cfg_msg ) ;
		break ;
	case MD_PAD_STATE_SUB: // select state slot 
		emu_state.cfg.slot_sel-- ;
		if( emu_state.cfg.slot_sel < 0 ){ emu_state.cfg.slot_sel = MAX_STATE_SLOT-1; }
		Menu_GetMsg_State( exec_cfg_msg, 0, emu_state.cfg.slot_sel ) ;
		break ; 
	case MD_PAD_STATE_ADD: // select state slot 
		emu_state.cfg.slot_sel++ ;
		if( emu_state.cfg.slot_sel > (MAX_STATE_SLOT-1) ){ emu_state.cfg.slot_sel = 0 ; }
		Menu_GetMsg_State( exec_cfg_msg, 0, emu_state.cfg.slot_sel ) ;
		break ; 
	case MD_PAD_STATE_LD : // load state
		if( EmuStateSlot_Load_MEM( &emu_state.slot[ emu_state.cfg.slot_sel ] ) )
		{
			Menu_GetMsg_State( exec_cfg_msg, 1, emu_state.cfg.slot_sel ) ;
			exec_key = MD_PAD_STATE_LD ;
		}
		else{ return 0 ; }
		break ; 
	case MD_PAD_STATE_SV : // save state
		resize_image_320to80( 
			emc->screen_mode == SCREEN_1X ? PSP_Screen_GetBackBuffer( 80, 16+8 ) : PSP_Screen_GetTempBuffer( 16, 16 ), 
			&state_temp_image[0][0] ) ;

		EmuState_SaveSlot( &emu_state, emu_state.cfg.slot_sel, &state_temp_image[0][0], path_state ) ;
		Menu_GetMsg_State( exec_cfg_msg, 2, emu_state.cfg.slot_sel ) ;
		break ; 
	case MD_PAD_STATE_LDL: // load last slot
		if( EmuStateSlot_Load_MEM( &emu_state.slot[ emu_state.cfg.slot_inc ] ) )
		{
			Menu_GetMsg_State( exec_cfg_msg, 3, emu_state.cfg.slot_inc ) ;
			exec_key = MD_PAD_STATE_LD ; // MD_PAD_STATE_LDL
		}
		else{ return 0 ; }
		break ; 
	case MD_PAD_STATE_SVL: // save slot+
		resize_image_320to80( 
			emc->screen_mode == SCREEN_1X ? PSP_Screen_GetBackBuffer( 80, 16+8 ) : PSP_Screen_GetTempBuffer( 16, 16 ), 
			&state_temp_image[0][0] ) ;

		EmuState_CalcInc( &emu_state ) ; 
		EmuState_SaveSlot( &emu_state, emu_state.cfg.slot_inc, &state_temp_image[0][0], path_state ) ;
		Menu_GetMsg_State( exec_cfg_msg, 4, emu_state.cfg.slot_inc ) ;
		break ; 
	case MD_PAD_CHEAT_ON :
		if( emc->comm_mode == COMM_NON )
		{
			emu_cheat.all_on = emu_cheat.all_on ? 0 : 1 ;
			Menu_GetMsg_CheatOn( exec_cfg_msg, emu_cheat.all_on ) ;
		}
		else{ return 0 ; }
		break ;
	case MD_PAD_CORE  :
		if( emc->comm_mode == COMM_NON )
		{
			emc->show_flags ^= FLAG_CORE  ;
			Menu_GetMsg_Core( exec_cfg_msg ) ;
		}
		else{ return 0 ; }
		//exec_key = MD_PAD_CORE ;
		break ;
	case MD_PAD_RESET :
		md_reset() ;
		Menu_GetMsg_Reset( exec_cfg_msg ) ;
		exec_key = MD_PAD_RESET ;
		break ;
	case MD_PAD_TURBO :
		turbo_on = turbo_on ? 0 : 1 ;
		Menu_GetMsg_Turbo( exec_cfg_msg, turbo_on ) ;
		exec_key = MD_PAD_TURBO ;

		PSP_Sound_Enable( emc->sound_on && ((turbo_on == 0) || ((emc->sound_flag & MD_SND_TURBO)==0)) ) ;
		break ;
	default : return 0 ;
	}

	return 1 ;
}

int exec_key_cfg( PSP_PAD_DATA* pPad )
{
	if( pPad->buttons & CTRL_LTRIGGER )
	{
		if( emc->pad_cfg[MD_PAD_CFG_L] )
		{
			if( EmuCfg_ExecKey( emc->pad_cfg[MD_PAD_CFG_L] ) ){ return 1 ; }
		}
	}

	if( pPad->buttons & CTRL_RTRIGGER )
	{
		if( emc->pad_cfg[MD_PAD_CFG_R] )
		{
			if( EmuCfg_ExecKey( emc->pad_cfg[MD_PAD_CFG_R] ) ){ return 1 ; }
		}
		else
		{
			if( (pPad->buttons & CTRL_CIRCLE  ) && EmuCfg_ExecKey( emc->pad_cfg[MD_PAD_CFG_R_CIRCLE  ] ) ){ return 1 ; }
			if( (pPad->buttons & CTRL_CROSS   ) && EmuCfg_ExecKey( emc->pad_cfg[MD_PAD_CFG_R_CROSS   ] ) ){ return 1 ; }
			if( (pPad->buttons & CTRL_TRIANGLE) && EmuCfg_ExecKey( emc->pad_cfg[MD_PAD_CFG_R_TRIANGLE] ) ){ return 1 ; }
			if( (pPad->buttons & CTRL_SQUARE  ) && EmuCfg_ExecKey( emc->pad_cfg[MD_PAD_CFG_R_SQUARE  ] ) ){ return 1 ; }
			if( (pPad->buttons & CTRL_SELECT  ) && EmuCfg_ExecKey( emc->pad_cfg[MD_PAD_CFG_R_SELECT  ] ) ){ return 1 ; }
			if( (pPad->buttons & CTRL_START   ) && EmuCfg_ExecKey( emc->pad_cfg[MD_PAD_CFG_R_START   ] ) ){ return 1 ; }

			if( emc->analog_mode == PSP_ANALOG_SWAP )
			{
				if( (pPad->buttons & CTRL_UP   ) && EmuCfg_ExecKey( emc->pad_cfg[MD_PAD_CFG_R_DPAD_T] ) ){ return 1 ; }
				if( (pPad->buttons & CTRL_DOWN ) && EmuCfg_ExecKey( emc->pad_cfg[MD_PAD_CFG_R_DPAD_B] ) ){ return 1 ; }
				if( (pPad->buttons & CTRL_LEFT ) && EmuCfg_ExecKey( emc->pad_cfg[MD_PAD_CFG_R_DPAD_L] ) ){ return 1 ; }
				if( (pPad->buttons & CTRL_RIGHT) && EmuCfg_ExecKey( emc->pad_cfg[MD_PAD_CFG_R_DPAD_R] ) ){ return 1 ; }
			}
			else
			{
				if( (pPad->analog[CTRL_ANALOG_Y] < LOWER_THRESHOLD) && EmuCfg_ExecKey( emc->pad_cfg[MD_PAD_CFG_R_DPAD_T] ) ){ return 1 ; }
				if( (pPad->analog[CTRL_ANALOG_Y] > UPPER_THRESHOLD) && EmuCfg_ExecKey( emc->pad_cfg[MD_PAD_CFG_R_DPAD_B] ) ){ return 1 ; }
				if( (pPad->analog[CTRL_ANALOG_X] < LOWER_THRESHOLD) && EmuCfg_ExecKey( emc->pad_cfg[MD_PAD_CFG_R_DPAD_L] ) ){ return 1 ; }
				if( (pPad->analog[CTRL_ANALOG_X] > UPPER_THRESHOLD) && EmuCfg_ExecKey( emc->pad_cfg[MD_PAD_CFG_R_DPAD_R] ) ){ return 1 ; }
			}
		}
	}
	else
	{
		if( (pPad->buttons & CTRL_CIRCLE  ) && EmuCfg_ExecKey( emc->pad_cfg[MD_PAD_CFG_CIRCLE  ] ) ){ return 1 ; }
		if( (pPad->buttons & CTRL_CROSS   ) && EmuCfg_ExecKey( emc->pad_cfg[MD_PAD_CFG_CROSS   ] ) ){ return 1 ; }
		if( (pPad->buttons & CTRL_TRIANGLE) && EmuCfg_ExecKey( emc->pad_cfg[MD_PAD_CFG_TRIANGLE] ) ){ return 1 ; }
		if( (pPad->buttons & CTRL_SQUARE  ) && EmuCfg_ExecKey( emc->pad_cfg[MD_PAD_CFG_SQUARE  ] ) ){ return 1 ; }
		if( (pPad->buttons & CTRL_SELECT  ) && EmuCfg_ExecKey( emc->pad_cfg[MD_PAD_CFG_SELECT  ] ) ){ return 1 ; }
		if( (pPad->buttons & CTRL_START   ) && EmuCfg_ExecKey( emc->pad_cfg[MD_PAD_CFG_START   ] ) ){ return 1 ; }

		if( emc->analog_mode == PSP_ANALOG_SWAP )
		{
			if( (pPad->buttons & CTRL_UP   ) && EmuCfg_ExecKey( emc->pad_cfg[MD_PAD_CFG_DPAD_T] ) ){ return 1 ; }
			if( (pPad->buttons & CTRL_DOWN ) && EmuCfg_ExecKey( emc->pad_cfg[MD_PAD_CFG_DPAD_B] ) ){ return 1 ; }
			if( (pPad->buttons & CTRL_LEFT ) && EmuCfg_ExecKey( emc->pad_cfg[MD_PAD_CFG_DPAD_L] ) ){ return 1 ; }
			if( (pPad->buttons & CTRL_RIGHT) && EmuCfg_ExecKey( emc->pad_cfg[MD_PAD_CFG_DPAD_R] ) ){ return 1 ; }
		}
		else if( emc->analog_mode == PSP_ANALOG_CFG )
		{
			if( (pPad->analog[CTRL_ANALOG_Y] < LOWER_THRESHOLD) && EmuCfg_ExecKey( emc->pad_cfg[MD_PAD_CFG_DPAD_T] ) ){ return 1 ; }
			if( (pPad->analog[CTRL_ANALOG_Y] > UPPER_THRESHOLD) && EmuCfg_ExecKey( emc->pad_cfg[MD_PAD_CFG_DPAD_B] ) ){ return 1 ; }
			if( (pPad->analog[CTRL_ANALOG_X] < LOWER_THRESHOLD) && EmuCfg_ExecKey( emc->pad_cfg[MD_PAD_CFG_DPAD_L] ) ){ return 1 ; }
			if( (pPad->analog[CTRL_ANALOG_X] > UPPER_THRESHOLD) && EmuCfg_ExecKey( emc->pad_cfg[MD_PAD_CFG_DPAD_R] ) ){ return 1 ; }
		}
	}

	return 0 ;
}

void ShowFPS( signed char pos, unsigned char fps )
{
//	const int show_x[] = { 1, 1, PSP_SCREEN_WIDTH - 5*6 -1, 1, PSP_SCREEN_WIDTH - 5*6 -1 } ;
//	const int show_y[] = { 1, 1, 1, PSP_SCREEN_HEIGHT -11, PSP_SCREEN_HEIGHT -11 } ;
	const short show_x [] = { 80  , 80  , 80 + 320 - 5*6 -1, 80       , 80+ 320 - 5*6 -1 } ;
	const short show_x2[] = { 80  , 80  , 80 + 320 - 5*7 -1, 80       , 80+ 320 - 5*7 -1 } ;
	const short show_y [] = { 16-2, 16-2, 16-2             , 16-7+240 , 16-7+240         } ;

	unsigned long* p_proc  = (unsigned long*)( (unsigned long)(do_proc_param) | 0x40000000 ) ;

	PSP_Screen_FillRect( 80, 16-7+240, PSP_SCREEN_WIDTH - 80, 16-7+240 +9, 0 ) ;

	if( fps > 99 )
	{
		PSP_Screen_DrawInt3( show_x2[pos]     , show_y[pos] , fps    , 0xFFFF ) ;
		PSP_Screen_DrawText( show_x2[pos] +5*3, show_y[pos] , "FPS " , 0xFFFF ) ;
	}
	else
	{
		PSP_Screen_DrawInt2( show_x[pos]     , show_y[pos] , fps    , 0xFFFF ) ;
		PSP_Screen_DrawText( show_x[pos] +5*2, show_y[pos] , "FPS " , 0xFFFF ) ;
	}

	//PSP_Screen_DrawSIntN( 0,10, tm_res, 8, 0xFFFF ) ;

	/*
	PSP_Screen_DrawHex8( 0, 10, p_proc[0], 0xFFFF ) ;
	PSP_Screen_DrawHex8( 0, 20, p_proc[7], 0xFFFF ) ;
	PSP_Screen_DrawHex8( 0, 30, p_proc[5], 0xFFFF ) ;
	PSP_Screen_DrawHex8( 0, 40, p_proc[6], 0xFFFF ) ;
	*/

}

void ShowFps2( signed char pos, unsigned char fps )
{
	const short show_x [] = { 0, 0, 480 - 5*5,      0, 480 - 5*5 } ;
	const short show_x2[] = { 0, 0, 480 - 5*6,      0, 480 - 5*6 } ;
	const short show_y [] = { 0, 0,         0, 272 -9, 272 - 9   } ;

	if( fps > 99 )
	{
		PSP_Screen_DrawInt3( show_x2[pos]     , show_y[pos] , fps    , 0xFFFF ) ;
		PSP_Screen_DrawText( show_x2[pos] +5*3, show_y[pos] , "FPS " , 0xFFFF ) ;
	}
	else
	{
		PSP_Screen_DrawInt2( show_x[pos]     , show_y[pos] , fps    , 0xFFFF ) ;
		PSP_Screen_DrawText( show_x[pos] +5*2, show_y[pos] , "FPS " , 0xFFFF ) ;
	}
}

inline void ShowExecCfgMsg()
{
//	PSP_Screen_FillRect( 80,     16-2, PSP_SCREEN_WIDTH - 80,     16-2 +9, 0 ) ;
//	PSP_Screen_FillRect( 80, 16-7+240, PSP_SCREEN_WIDTH - 80, 16-7+240 +9, 0 ) ;
	if( exec_cfg > 20 )
	{ 
		PSP_Screen_DrawText( 240 - strlen(exec_cfg_msg) * 5 / 2, 16-7+240, exec_cfg_msg, 0xFFFF ) ; 
	}
}

inline void ShowExecCfgMsg2()
{
	PSP_Screen_DrawText( 240 - strlen(exec_cfg_msg) * 5 / 2, 272-9, exec_cfg_msg, 0xFFFF ) ;
}

int UpdateTimer()
{
	int ret = UPDATE_NORMAL ;
	int bPad = 0 ;
	fps++ ;
	tm_st = tm_ed ;

	if( (tm_ed - tm_pd) > TM_1FRAME )
	{
		if( PSP_Power_CheckSleep() )
		{
			return UPDATE_MENU ;
		}

		tm_pd = tm_ed ;

		PSP_Ctrlpad_Update() ;

		if( (PSP_Ctrlpad_GetButtons() & key_menu) == key_menu )
		{ 
			resize_image_320to80( PSP_Screen_GetBackBuffer( 80, 16+8 ), &state_temp_image[0][0] ) ;
			ret = UPDATE_MENU ;
		}
		else
		{
			if( exec_cfg > 0 )
			{
				exec_cfg-- ;
				md_setpaddata( PSP_Ctrlpad_GetPadData(), emc ) ; 
				bPad = 1 ;
			}
			else if( exec_key_cfg( PSP_Ctrlpad_GetPadData() ) )
			{
				exec_cfg = 60 ;
				ret = UPDATE_CFG ;
			}
			else
			{
				md_setpaddata( PSP_Ctrlpad_GetPadData(), emc ) ; 
				bPad = 1 ;
			}
		}
	}

	if( (tm_ed - tm_60) > TM_60FRAME )
	{
		fps_last = fps   ;
		fps      = 0     ;
		tm_60    = tm_ed ;

#ifdef _DEBUG
		PSP_Screen_DrawIntN( 1, 10, tm_run      , 8, 0xFFFF ) ;
		PSP_Screen_DrawIntN( 1, 20, tm_skip     , 8, 0xFFFF ) ;
		PSP_Screen_DrawIntN( 1, 30, tm_debug[ 2], 8, 0xFFFF ) ;
		PSP_Screen_DrawIntN( 1, 60, tm_debug[ 3], 8, 0xFFFF ) ;
		PSP_Screen_DrawIntN( 1, 70, tm_debug[ 4], 8, 0xFFFF ) ;
		PSP_Screen_DrawIntN( 1, 80, tm_debug[ 5], 8, 0xFFFF ) ;
		PSP_Screen_DrawIntN( 1, 90, tm_debug[ 6], 8, 0xFFFF ) ;
		PSP_Screen_DrawIntN( 1,100, tm_debug[ 7], 8, 0xFFFF ) ;
		PSP_Screen_DrawIntN( 1,110, tm_debug[ 8], 8, 0xFFFF ) ;
		PSP_Screen_DrawIntN( 1,120, tm_debug[ 9], 8, 0xFFFF ) ;
		PSP_Screen_DrawIntN( 1,130, tm_debug[10], 8, 0xFFFF ) ;
		PSP_Screen_DrawIntN( 1,140, tm_debug[11], 8, 0xFFFF ) ;
		PSP_Screen_DrawIntN( 1,150, tm_debug[12], 8, 0xFFFF ) ;
		PSP_Screen_DrawIntN( 1,160, tm_debug[13], 8, 0xFFFF ) ;
		PSP_Screen_DrawIntN( 1,170, tm_debug[14], 8, 0xFFFF ) ;
		PSP_Screen_DrawIntN( 1,180, tm_debug[15], 8, 0xFFFF ) ;

		tm_debug[4] = tm_debug[5] = tm_debug[6] = tm_debug[7] = 0 ;
		tm_debug[8] = tm_debug[9] = tm_debug[10] = tm_debug[11] = 0 ;
#endif //_DEBUG

	}

	//*
///////////////////////////////////////////////////////////////////////////
#ifndef _NON_KERNEL ///////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
	if( emc->comm_mode )
	{
		if( PSP_Comm_WaitSync() != COMM_ERROR_OK )
		{
			return UPDATE_COMM_ERROR ;
		}

		if( ret == UPDATE_MENU )
		{
			if( PSP_Comm_SyncCmd( 
				( PSP_Ctrlpad_GetButtons() & CTRL_RTRIGGER ) ? COMM_CMD_KILL : COMM_CMD_MENU ,
				0, (emc->comm_mode == COMM_1P) ) != COMM_ERROR_OK )
			{
				//return UPDATE_COMM_ERROR ;
			}
		}
		else if( ret == UPDATE_CFG )
		{
			{
				if( emc->comm_mode == COMM_1P )
				{
					md_setpad( PSP_Comm_GetPad_R2(), 1 ) ; 
					md_setpad( PSP_Comm_GetPad_S (), 0 ) ; 
				}
				else
				{
					md_setpad( PSP_Comm_GetPad_R1(), 0 ) ; 
					md_setpad( PSP_Comm_GetPad_S (), 1 ) ; 
				}

				md_run_skip( (emc->show_flags & FLAG_Z80ASYNC) );
			}

			if( exec_key != -1 )
			{
				if( PSP_Comm_SyncCmd( COMM_CMD_EXECKEY, exec_key, (emc->comm_mode == COMM_1P) ) == COMM_ERROR_OK )
				{
					if( exec_key == MD_PAD_STATE_LD )
					{
						PSP_Sound_Enable(0);
						switch( PSP_Comm_SyncState( 1, 0 ) )
						{
						case COMM_ERROR_OK     : ret = UPDATE_CFG        ; break ;
						case COMM_ERROR_CANCEL : ret = UPDATE_MENU       ; break ;
						default                : ret = UPDATE_COMM_ERROR ; break ;
						}
					}
				}
				else
				{
					ret = UPDATE_COMM_ERROR ;
				}

				exec_key = -1 ;
			}
		}
		else // if( ret == UPDATE_NORMAL ) 
		{
			switch( PSP_Comm_GetCmd() )
			{
			default :
			case COMM_CMD_PAD1 :
			case COMM_CMD_PAD2 :
				break ;

			case COMM_CMD_EXECKEY :
				{
					if( PSP_Comm_GetSyncData_R() == MD_PAD_STATE_LD )
					{
						PSP_Sound_Enable(0);
						switch( PSP_Comm_SyncState( 0, 0 ) )
						{
						case COMM_ERROR_OK     : ret = UPDATE_CFG        ; break ;
						case COMM_ERROR_CANCEL : ret = UPDATE_MENU       ; break ;
						default                : ret = UPDATE_COMM_ERROR ; break ;
						}
					}
					else
					{
						if( EmuCfg_ExecKey( PSP_Comm_GetSyncData_R() ) )
						{
							exec_cfg = 60 ;
							ret = UPDATE_CFG ;
						}
						else
						{
						}

						exec_key = -1 ;
					}
				}
				break ;

			case COMM_CMD_MENU :
				ret = UPDATE_MENU ;
				break ;

			case COMM_CMD_MSG :
				break ;

			case COMM_CMD_KILL :
				PSP_Adhoc_Term() ;
				ret = UPDATE_MENU ;
				return ret ;
				break ;
			}
		}

		if( emc->comm_mode == COMM_1P )
		{
			//int data = bPad ? md_getpad( 0 ) : PSP_Comm_GetPad_S() ;

			if( bPad ){ PSP_Comm_SetSyncData( md_getpad( 0 ) ) ; }

			md_setpad( PSP_Comm_GetPad_R2(), 1 ) ; 
			md_setpad( PSP_Comm_GetPad_S (), 0 ) ; 
			//md_setpad( PSP_Comm_GetPad1() | md_getpad( 0 ), 0 ) ; 

			//PSP_Comm_StartSync( COMM_CMD_PAD1, data, (emc->comm_mode == COMM_1P) ) ;
			PSP_Comm_StartSync2( COMM_CMD_PAD1, (emc->comm_mode == COMM_1P) ) ;
		}
		else //if( emc->comm_mode == COMM_2P )
		{
			//int data = bPad ? md_getpad( 1 ) : PSP_Comm_GetPad_S() ;
			if( bPad ){ PSP_Comm_SetSyncData( md_getpad( 1 ) ) ; }

			md_setpad( PSP_Comm_GetPad_R1(), 0 ) ; 
			md_setpad( PSP_Comm_GetPad_S (), 1 ) ; 

			//PSP_Comm_StartSync( COMM_CMD_PAD2, data, (emc->comm_mode == COMM_1P) ) ;
			PSP_Comm_StartSync2( COMM_CMD_PAD2, (emc->comm_mode == COMM_1P) ) ;
		}
	}
	else
	//*/
///////////////////////////////////////////////////////////////////////////
#endif //_NON_KERNEL //////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
	{
		EmuCheat_Exec( &emu_cheat ) ;
	}

	return ret ;
}

#define CHECK_SKIP()  ((skip == 0)) // || (skip_s == 0))

#define CALC_SKIP_RUN() \
			{\
				tm_ed  = sceKernelLibcClock() ; \
				tm_run = tm_ed - tm_st ;        \
				skip   = calc_skip( tm_run ) ;  \
			}

#define DO_SKIP() \
			{\
				md_run_skip( (emc->show_flags & FLAG_Z80ASYNC) ) ;  \
				tm_ed   = sceKernelLibcClock() ; \
				tm_skip = tm_ed - tm_st ;        \
				skip    = calc_skip( tm_skip ) ; \
			}

#define DO_RUN(a,b) \
			{\
				if( exec_cfg > 0 ){ ShowExecCfgMsg2(); }\
				else if( emc->show_fps ){ ShowFps2( emc->show_fps, fps_last ) ;  }\
				PSP_Screen_Flip() ;\
				PSP_Screen_TempFlip() ;\
				md_setframe( PSP_Screen_GetTempBuffer( a, b ), PSP_LINESIZE * 2 ) ;\
				md_run( (emc->show_flags & FLAG_Z80ASYNC) ) ;\
			}


int main_thread( int argc, char *argv ) ;

int main(int argc, char *argv[])
{
	return xmain( strlen(argv[0]), argv[0] ) ;
}

int xmain(int argc, char *argp)
{

#ifdef _USE_ME
	md_init_sub_proc() ;
#endif //_USE_ME

	PSP_Adhoc_Init() ;

	int thid = sceKernelCreateThread("main_thread", main_thread, 0x18, 0x40000, THREAD_ATTR_MAIN, NULL);
	if( thid < 0 )
	{
		sceKernelSleepThread();
	}

	sceKernelStartThread( thid, argc, argp );

	//sceKernelWaitThreadEnd(thid, NULL);

	sceKernelExitDeleteThread(0);

	return 0 ;
}

int main_thread( int argc, char *argv )
{
	/*
	argv[argc] = 0 ;
	PSP_Screen_Init  ( ) ;
	for(;;)
	{
		int h = 10 ;
		PSP_Screen_DrawText( 10,h, "TEST!!!", 0xFFFF ) ; h+=10 ;
		PSP_Screen_DrawInt2( 10,h, argc, 0xFFFF ) ;h+=10 ;
		PSP_Screen_DrawHex8( 10,h, argv, 0xFFFF ) ;h+=10 ;
		PSP_Screen_DrawHex8( 10,h, EmuCore_GetBuffer(MAX_EMU_CORE_BUFF), 0xFFFF ) ;h+=10 ;
		PSP_Screen_DrawHex8( 10,h, sceKernelMaxFreeMemSize  (), 0xFFFF ) ;h+=10 ;
		PSP_Screen_DrawHex8( 10,h, sceKernelTotalFreeMemSize(), 0xFFFF ) ;h+=10 ;
		
		if( argv )
		{
			PSP_Screen_DrawText( 10,h, argv, 0xFFFF ) ;h+=10 ;			
		}
		PSP_Screen_WaitVsync() ;
		PSP_Screen_Flip() ;

		PSP_Ctrlpad_Update() ;
		//if( PSP_Ctrlpad_GetButtons() != 0 ){ break ; }
	}
	//*/

	argv[argc] = 0 ;
	EmuCore_GetBuffer(MAX_EMU_CORE_BUFF) ;
	Menu_InitPath ( argc, argv ) ;
	
	EmuCfg_init( &emu_config[ EMU_MD  ] ) ;
	EmuCfg_init( &emu_config[ EMU_PCE ] ) ;

	Menu_Init        ( ) ;
	PSP_Power_Init   ( ) ;
	PSP_Screen_Init  ( ) ;
	PSP_Sound_Init   ( ) ;
	PSP_Ctrlpad_Init ( ) ;

#ifdef _USE_MD
	md_init_memory ( EmuCore_GetBuffer(MAX_EMU_CORE_BUFF), MAX_EMU_CORE_BUFF ) ;
	md_init  () ;
#endif //_USE_MD


_MAIN_MENU :
	//PSP_IrDA_Close() ;
	PSP_Screen_SetVBlankInt(0) ;
	PSP_Sound_Enable(0) ;
	scePowerSetClockFrequency(222,222,111);
	if( PSP_Ctrlpad_GetButtons() & CTRL_RTRIGGER ){ PSP_Comm_Term() ; }

	switch( Menu_MainMenu () )
	{
	default:
	case MENU_CMD_CANCEL   : 
		if( !md_is_loadrom() ){ goto _MAIN_MENU ; }

		PSP_Ctrlpad_Update() ;
		if( PSP_Ctrlpad_GetPadData()->analog[CTRL_ANALOG_Y] < LOWER_THRESHOLD2 )
		{
			md_reset() ;
		}
		break ;

#ifdef _USE_MD
	case MENU_CMD_LOAD_MD       : 
	case MENU_CMD_LOAD_MD_ROM_S :
		if( md_load_rom( path_rom, 0 ) == 0 ){ emu_mode = EMU_MD ; }
		else { goto _MAIN_MENU ; }

		turbo_on = 0 ;

		md_load_sram( path_sram ) ;

		emc = &emu_config[ emu_mode  ] ;
		EmuCfg_Load( emc, path_cfg ) ;

		if( emc->show_flags & FLAG_CHECKSUM )
		{
			md_fix_checksum() ;
		}

		EmuCheat_Load( &emu_cheat, path_cheat ) ;
			
		if( menu_cmd == MENU_CMD_LOAD_MD_ROM_S )
		{
			//md_run_skip() ;
			//EmuStateSlot_Load_MEM( &emu_state.slot[ menu_load_state_slot ] ) ;
			for( fps=0; fps < 120; fps++ ){ md_run_skip(0) ; }
			EmuStateSlot_Load_MEM( &emu_state.slot[ menu_load_state_slot ] ) ;
		}
		Menu_DrawBG() ;
		break ;
	case MENU_CMD_LOAD_MD_STATE :
		EmuStateSlot_Load_MEM( &emu_state.slot[ menu_load_state_slot ] ) ;
		break ;

#endif //_USE_MD

#ifdef _USE_PCE
	case MENU_CMD_LOAD_PCE : 
		if( pce_load_rom( path_rom ) == 0 )
		{
			emu_mode = EMU_PCE ;
		}
		else { goto _MAIN_MENU ; }
		emc = &emu_config[ emu_mode  ] ;
		EmuCfg_Load( emc, path_cfg ) ;
		Menu_DrawBG() ;
		break ;
#endif //_USE_PCE
	}

	emc = &emu_config[ emu_mode  ] ;

///////////////////////////////////////////////////////////////////////////
#ifndef _NON_KERNEL ///////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
_Retry_Comm :
	if( emc->comm_mode )
	{
		switch( PSP_Comm_Connect( emc->comm_mode == COMM_1P ) )
		{
		case COMM_ERROR_OK     : break ;
		case COMM_ERROR_CANCEL : PSP_Comm_Term() ; goto _MAIN_MENU ;
		default                : goto _COMM_ERROR ;
		}

		switch( PSP_Comm_SyncState( (emc->comm_mode == COMM_1P), 1 ) )
		{
		case COMM_ERROR_OK     : break ;
		case COMM_ERROR_CANCEL : goto _MAIN_MENU          ;
		default                : goto _COMM_ERROR ;
		}

		if( emc->comm_mode == COMM_1P )
		{
			md_select_paddata( 0 ) ;

			PSP_Comm_StartSync( COMM_CMD_PAD1, md_getpad( 0 ), (emc->comm_mode == COMM_1P) ) ;
		}
		else //if( emc->comm_mode == COMM_2P )
		{
			md_select_paddata( 1 ) ;

			PSP_Comm_StartSync( COMM_CMD_PAD2, md_getpad( 1 ), (emc->comm_mode == COMM_1P) ) ;
		}

		PSP_Comm_InitThread() ;
	}
	else
///////////////////////////////////////////////////////////////////////////
#endif //_NON_KERNEL //////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////

	{
		md_select_paddata( 0 ) ;
	}

#ifdef _OVER_CLOCK
	switch( emc->cpu_freq )
	{
	default :
	case CPU_FREQ_222: scePowerSetClockFrequency(222,222,111); break ;
	case CPU_FREQ_266: scePowerSetClockFrequency(266,266,133); break ;
	case CPU_FREQ_333: scePowerSetClockFrequency(333,333,166); break ;
	}
#endif

_UPDATE_CFG :

	md_select_core( (emc->show_flags & FLAG_CORE) ? 1 : 0 ) ;

	switch( emc->sound_vol )
	{
	case VOL_08 : psp_sound_vol = 0x8000 ; break ;
	case VOL_10 : psp_sound_vol = 0xA000 ; break ;
	case VOL_12 : psp_sound_vol = 0xC000 ; break ;
	case VOL_14 : psp_sound_vol = 0xE000 ; break ;
	case VOL_16 : psp_sound_vol = 0xFFFF ; break ;
	}

	fps      = 0 ;
	fps_last = 0 ;
	skip     = 0 ;
	skip_s   = emc->skip_count ;
	tm_ed    = sceKernelLibcClock() ;
	tm_run   = TM_1FRAME ;
	tm_skip  = TM_1FRAME ;
	tm_res   = 0 ;

	key_menu = menu_keys[ EmuSysCfg_Get()->key_menu ] ;

	if( emc->show_flags & SHOW_VSYNC ){ PSP_Screen_SetVBlankInt(1) ; }

	//*
	if( !md_is_loadrom() ){ goto _MAIN_MENU ; }
	md_setconfig( emc ) ;
	PSP_Sound_Enable( emc->sound_on && ((turbo_on == 0) || ((emc->sound_flag & MD_SND_TURBO)==0)) ) ;

	//sceKernelDcacheWritebackAll();
	sceKernelDcacheWritebackInvalidateAll() ;

	for(;;)
	{
		switch( UpdateTimer() )
		{
		case UPDATE_CFG        : goto _UPDATE_CFG ;
		case UPDATE_MENU       : goto _MAIN_MENU  ;
		case UPDATE_COMM_ERROR : goto _COMM_ERROR ;
		}

		if( CHECK_SKIP() )
		{
			switch( emc->screen_mode )
			{
			default :
				{
					md_setframe( PSP_Screen_GetBackBuffer( 80, 16 ), PSP_LINESIZE * 2 ) ;
					md_run( (emc->show_flags & FLAG_Z80ASYNC) ) ;

					if( exec_cfg > 0 ){ ShowExecCfgMsg(); }
					else if( emc->show_fps ){ ShowFPS( emc->show_fps, fps_last ) ;  }
					PSP_Screen_Flip() ;
				}
				break ;

			case SCREEN_FIT   : 
				{
					/*
					if( exec_cfg > 0 ){ ShowExecCfgMsg2(); }
					else if( emc->show_fps ){ ShowFps2( emc->show_fps, fps_last ) ;  }
					PSP_Screen_Flip() ;
					PSP_Screen_TempFlip() ;
					md_setframe( PSP_Screen_GetTempBuffer( 8, 8 ), PSP_LINESIZE * 2 ) ;
					md_run() ;
					*/
					DO_RUN( 8, 8 ) ;

					PSP_Screen_BitBltGe( 
						PSP_Screen_GetTempBuffer( 8, 16 ), 
						320, 224,
						(480 - 388)/2, 0, (480 - 388)/2 + 388, PSP_SCREEN_HEIGHT ) ;
				}
				break ;

			case SCREEN_FULL  : 
				{
					/*
					if( exec_cfg > 0 ){ ShowExecCfgMsg2(); }
					else if( emc->show_fps ){ ShowFps2( emc->show_fps, fps_last ) ;  }
					PSP_Screen_Flip() ;
					PSP_Screen_TempFlip() ;
					md_setframe( PSP_Screen_GetTempBuffer( 16, 8 ), PSP_LINESIZE * 2 ) ;
					md_run() ;
					*/
					DO_RUN( 16, 8 ) ;

					if( md_is_width320() )
					{
						PSP_Screen_BitBltGe( 
							PSP_Screen_GetTempBuffer( 16, 16 ), 
							320, 224,
							0, 0, PSP_SCREEN_WIDTH, PSP_SCREEN_HEIGHT ) ;
					}
					else
					{
						PSP_Screen_BitBltGe( 
							PSP_Screen_GetTempBuffer( 48, 16 ), 
							256, 224,
							0, 0, PSP_SCREEN_WIDTH, PSP_SCREEN_HEIGHT ) ;
					}
				}
				break ;

			case SCREEN_FIT43 : 
				{
					/*
					if( exec_cfg > 0 ){ ShowExecCfgMsg2(); }
					else if( emc->show_fps ){ ShowFps2( emc->show_fps, fps_last ) ;  }
					PSP_Screen_Flip() ;
					PSP_Screen_TempFlip() ;
					md_setframe( PSP_Screen_GetTempBuffer( 16, 8 ), PSP_LINESIZE * 2 ) ;
					md_run() ;
					*/
					DO_RUN( 16, 8 ) ;

					if( md_is_width320() )
					{
						PSP_Screen_BitBltGe( 
							PSP_Screen_GetTempBuffer( 16, 16 ), 
							320, 224,
							(PSP_SCREEN_WIDTH - 362)/2, 0, (PSP_SCREEN_WIDTH + 362)/2, PSP_SCREEN_HEIGHT ) ;
					}
					else
					{
						PSP_Screen_BitBltGe( 
							PSP_Screen_GetTempBuffer( 48, 16 ), 
							256, 224,
							(PSP_SCREEN_WIDTH - 362)/2, 0, (PSP_SCREEN_WIDTH + 362)/2, PSP_SCREEN_HEIGHT ) ;
					}
				}
				break ;
			}

			CALC_SKIP_RUN() ;
		}
		else
		{
			DO_SKIP() ;
		}


#ifndef _USE_SOUND_THREAD
		{
			/*
			md_sound_update2( 
				(short*)( (unsigned long)psp_sound_playbuf [outbank] | 0x40000000 ), 
				(short*)( (unsigned long)psp_sound_playbuf2[outbank] | 0x40000000 ), 
				PSP_SOUND_SAMPLE ) ;
				*/
			//*
			md_sound_update2( 
				(emc->show_flags & FLAG_Z80ASYNC) ,
				(short*)( (unsigned long)psp_sound_playbuf [snd_outbank]  ), 
				(short*)( (unsigned long)psp_sound_playbuf2[snd_outbank]  ), 
				PSP_SOUND_SAMPLE ) ;
			//	*/

			sceAudioOutputPannedBlocking( psp_sound_handle, psp_sound_vol, psp_sound_vol, psp_sound_playbuf[snd_outbank] );
			sceAudioOutputPanned( psp_sound_handle2[snd_outbank], psp_sound_vol, psp_sound_vol, psp_sound_playbuf2[snd_outbank] );
			snd_outbank ^= 1 ;
			//md_start_sub_proc() ;
		}
#endif //

	}
	//*/

_COMM_ERROR :
	{
		PSP_Sound_Enable(0) ;

///////////////////////////////////////////////////////////////////////////
#ifndef _NON_KERNEL ///////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
		if( PSP_Comm_ShowError( COMM_SHOW_SYNC ) == COMM_ERROR_OK )
		{
			goto _Retry_Comm ;
		}
		else
		{
		}
///////////////////////////////////////////////////////////////////////////
#endif //_NON_KERNEL //////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////

		goto _MAIN_MENU  ;
	}


//#endif //_USE_MD


	return 0 ;
}

///////////////////////////////////////////////////////////////////////////
/// PSP Sound
///////////////////////////////////////////////////////////////////////////

/*
int PSP_Sound_Update2( void )
{
	static short outbank = 0 ;

	if( psp_sound_enable )
	{
		memset4x( (unsigned long*)psp_sound_playbuf2[outbank], 0, PSP_SOUND_SAMPLE ) ;

		md_sound_update2( psp_sound_playbuf2[outbank], PSP_SOUND_SAMPLE ) ;

		sceAudioOutputPanned( psp_sound_handle2[outbank], psp_sound_vol, psp_sound_vol, psp_sound_playbuf2[outbank] );

		outbank ^= 1 ;
	}
}
*/

static int PSP_Sound_Thread( int args, void *argp )
{
	short outbank = 0 ;

#ifdef _DEBUG
	int n ;
	unsigned long tm_sound ;
#endif //_DEBUG

	while ( psp_sound_terminate == 0 ) 
	{

#ifdef _DEBUG
		tm_sound = sceKernelLibcClock() ;
#endif //_DEBUG

		if( psp_sound_enable ) // && ((turbo_on == 0) || ((emc->sound_flag & MD_SND_TURBO)==0)) )
		{
			outbank ^= 1 ;

			/*
			sceKernelDcacheWritebackInvalidateAll() ;
			md_sound_update2( 
				(short*)( (unsigned long)psp_sound_playbuf [outbank] | 0x40000000 ), 
				(short*)( (unsigned long)psp_sound_playbuf2[outbank] | 0x40000000 ), 
				PSP_SOUND_SAMPLE ) ;
			*/
			md_sound_update2( 
				(emc->show_flags & FLAG_Z80ASYNC) ,
				(short*)( psp_sound_playbuf [outbank]  ), 
				(short*)( psp_sound_playbuf2[outbank]  ), 
				PSP_SOUND_SAMPLE ) ;


			sceAudioOutputPannedBlocking( psp_sound_handle, psp_sound_vol, psp_sound_vol, psp_sound_playbuf[outbank] );

			sceAudioOutputPanned( psp_sound_handle2[outbank], psp_sound_vol, psp_sound_vol, psp_sound_playbuf2[outbank] );
		}
		else
		{
			outbank ^= 1 ;
			memset4x( (unsigned long*)psp_sound_playbuf[outbank], 0, PSP_SOUND_SAMPLE ) ;
#ifdef _DEBUG
			tm_debug[2] = sceKernelLibcClock() - tm_sound ;
#endif //_DEBUG
			//sceAudioOutputPannedBlocking( psp_sound_handle, 0, 0, psp_sound_playbuf[outbank] );

			sceKernelSleepThread() ;
		}
	}

	sceKernelExitThread(0);
	return 0 ;
}

// nChannel : 0 - 2
int PSP_Sound_Init( void )
{
	int n ;

	char thread_name[] = "pgasndX" ;

	memset( psp_sound_playbuf, 0, sizeof(psp_sound_playbuf) ) ;

	/*
	for( n=0; n < MAX_PSP_SOUND_BANK ; n++ )
	{
		memset( psp_sound_buffer[0][n], 0, MAX_PSP_SOUND_BUFFER * sizeof(short) ) ;
	}
	*/

	psp_sound_terminate = 0 ;

	psp_sound_handle = -1 ;
	psp_sound_thid   = -1 ;
	psp_sound_enable = 0  ;

	psp_sound_handle  = sceAudioChReserve( -1, PSP_SOUND_SAMPLE, 0 ) ;

	psp_sound_handle2[0] = sceAudioChReserve( -1, PSP_SOUND_SAMPLE, 0x10 ) ;
	psp_sound_handle2[1] = sceAudioChReserve( -1, PSP_SOUND_SAMPLE, 0x10 ) ;

#ifdef _USE_SOUND_THREAD
	if( psp_sound_handle >= 0 )
	{
		thread_name[6] = '0' ;

		psp_sound_thid = sceKernelCreateThread(
								thread_name,
								&PSP_Sound_Thread,
								//0x12,0x10000,0,NULL );
								0x12,0x10000,THREAD_ATTR_SOUND,NULL );

		if( psp_sound_thid >= 0 )
		{
			sceKernelStartThread( psp_sound_thid, 0, 0 );
		}
	}
#endif // 1

	return 0 ;
}

void PSP_Sound_Enable( int nEnable )
{
	psp_sound_enable = nEnable ;

	if( nEnable )
	{
		sceKernelWakeupThread( psp_sound_thid ) ;
	}
}


void PSP_Sound_Term(void)
{
	int n ; 

	psp_sound_terminate = 1 ;

	{
		if( psp_sound_thid >= 0 )
		{
			sceKernelWaitThreadEnd( psp_sound_thid , NULL );
			sceKernelDeleteThread ( psp_sound_thid );

			psp_sound_thid = -1 ;
		}

		if( psp_sound_handle >= 0 )
		{
			sceAudioChRelease( psp_sound_handle );
		}
	}

}

///////////////////////////////////////////////////////////////////////////
/// PSP Ctrlpad
///////////////////////////////////////////////////////////////////////////
PSP_PAD_DATA  psp_pad_data ;

int PSP_Ctrlpad_Init( void )
{
	sceCtrlSetSamplingCycle(0);
	sceCtrlSetSamplingMode (1);
}

void PSP_Ctrlpad_Update( void )
{
	sceCtrlReadBufferPositive( &psp_pad_data, 1 );
}

unsigned long PSP_Ctrlpad_GetButtons( void )
{
	return psp_pad_data.buttons ;
}

PSP_PAD_DATA* PSP_Ctrlpad_GetPadData()
{
	return &psp_pad_data ;
}

///////////////////////////////////////////////////////////////////////////
/// PSP Power
///////////////////////////////////////////////////////////////////////////
int bSleep=0;

int PSP_Power_CheckSleep(void)
{
	int n ;

	if( bSleep )
	{
		PSP_Comm_Term() ;
		PSP_Sound_Enable(0) ;

		while( bSleep )
		{
			sceDisplayWaitVblankStart();
		}

		PSP_Sound_Enable(emc->sound_on) ;

		return 1 ;
	}

	return 0 ;
}

////////////////////////////////////
/// from RIN.  thanks for MIRAKICHI.
////////////////////////////////////
#define POWER_CB_POWER		0x80000000
#define POWER_CB_HOLDON		0x40000000
#define POWER_CB_STANDBY	0x00080000
#define POWER_CB_RESCOMP	0x00040000
#define POWER_CB_RESUME		0x00020000
#define POWER_CB_SUSPEND	0x00010000
#define POWER_CB_EXT		0x00001000
#define POWER_CB_BATLOW		0x00000100
#define POWER_CB_BATTERY	0x00000080
#define POWER_CB_BATTPOWER	0x0000007F

void  PSP_Main_OnExitGame( void )
{
#ifdef _OVER_CLOCK
		scePowerSetClockFrequency(222,222,111);
#endif

	sceKernelPowerLock(0);
	PSP_Comm_Term() ;
	EmuSysCfg_Save( path_exec ) ;
	if( strlen( path_cfg   ) > 0                  ){ EmuCfg_Save      ( emc       , path_cfg   ) ; }
	if( strlen( path_cheat ) > 0                  ){ EmuCheat_Save    ( &emu_cheat, path_cheat ) ; }
	if( strlen( path_sram  ) > 0 && emc->autosram ){ EmuCore_SaveSRAM ( path_sram              ) ; }
	if( strlen( path_state ) > 0                  ){ EmuState_Save    ( &emu_state, path_state ) ; }
	sceKernelPowerUnlock(0);
}

// ホームボタン終了時にコールバック
int exit_callback(void)
{
	bSleep=1;

	PSP_Main_OnExitGame() ;

	sceKernelExitGame();
	return 0;
}

// スリープ時や不定期にコールバック
void power_callback(int unknown, int pwrflags)
{
	if(pwrflags & POWER_CB_POWER)
	{
		if( !bSleep )
		{
			bSleep=1;

			PSP_Main_OnExitGame() ;
		}
	}
	else if(pwrflags & POWER_CB_BATLOW)
	{
		//if( EmuSysCfg_Get()->option_flags & OPTION_LOWBATT )
		{
			if( !bSleep )
			{
				bSleep=1;

				PSP_Main_OnExitGame() ;

				scePowerRequestSuspend() ;
			}
		}
	}
	else if(pwrflags & POWER_CB_RESCOMP)
	{
		bSleep=0;

#ifdef _OVER_CLOCK
		switch( emc->cpu_freq )
		{
		default :
		case CPU_FREQ_222: scePowerSetClockFrequency(222,222,111); break ;
		case CPU_FREQ_266: scePowerSetClockFrequency(266,266,133); break ;
		case CPU_FREQ_333: scePowerSetClockFrequency(333,333,166); break ;
		}
#endif

	}
	/*
	else if( pwrflags & POWER_CB_BATTPOWER )
	{
		if( EmuSysCfg_Get()->option_flags & OPTION_SHOW_BATT )
		{
		}
	}
	*/

	// コールバック関数の再登録
	// （一度呼ばれたら再登録しとかないと次にコールバックされない）
	int cbid = sceKernelCreateCallback("Power Callback", power_callback,0);
	scePowerRegisterCallback(0, cbid);
}

// ポーリング用スレッド
int CallbackThread(int args, void *argp)
{
	int cbid;
	
	// コールバック関数の登録
	cbid = sceKernelCreateCallback("Exit Callback", exit_callback,0);
	sceKernelRegisterExitCallback(cbid);
	cbid = sceKernelCreateCallback("Power Callback", power_callback,0);
	scePowerRegisterCallback(0, cbid);
	
	// ポーリング
	sceKernelSleepThreadCB();
}

int PSP_Power_Init(void)
{
	int thid = 0;
	
	// ポーリング用スレッドの生成
	thid = sceKernelCreateThread("update_thread", CallbackThread, 0x11/*0x20*/, 0xFA0, 0, 0);
	if(thid >= 0)
		sceKernelStartThread(thid, 0, 0);
	
	return thid;
}

///////////////////////////////////////////////////////////////////////////
