////////////////////////////////////////////////
/// mdcore_main.c
////////////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
////////////////////////////////////////////////
#include "../mdcore/md.h"
////////////////////////////////////////////////
#include "emu_config.h"
#include "emu_state.h"
#include "psp_pad.h"
#include "psp_me.h"
////////////////////////////////////////////////
#define _USE_FM_CORE
/////////////////////////////////////////////////
extern int pal_dirty;

int sound_is_okay = 0;
FILE *debug_log = NULL;
md_core*  mdcore ;

/////////////////////////////////////////////////
#ifdef __cplusplus
extern "C" {
#endif
//#ifdef __cplusplus
//}
//#endif
/////////////////////////////////////////////////

#define MD_SOUND_BUFF_LEN  (768) // 
#define MD_SCREEN_CX       (320)
#define MD_SCREEN_CY       (224)

//static char ramdir[128]   ;
//static char temp[0x20000] ;
//short sound_buff_l[MD_SOUND_BUFF_LEN];
//short sound_buff_r[MD_SOUND_BUFF_LEN];
//static unsigned char  mdpal[256];
//static short* md_sound_ptr ;

int is_md_loadrom = 0 ;

//static int wp, rp ;
//const int dgen_soundsegs = 8     ;
//const int dgen_soundrate = 44100 ;
int sound_on = 0 ;
int sound_flags ;
/////////////////////////////////////////////////
#ifdef _USE_ME
/////////////////////////////////////////////////
extern void sceKernelDelayThread(unsigned long delay);
extern void sceKernelDcacheWritebackAll(void);

extern unsigned long do_proc_param[]  ;
//VDP_DATA data_sub ;
extern  short   psp_sound_playbuf [2][ PSP_SOUND_SAMPLE * 2 ]  ;
extern  short   psp_sound_playbuf2[2][ PSP_SOUND_SAMPLE     ]  ;
extern  short   snd_outbank ;

void md_sound_update2( short* p1, short* p2, short len );

void _wait_sub()
{
	volatile unsigned long wt = 0 ;
			
	for( wt=0 ; wt < 0xFFFF ; wt++ ){ ; }
}
	

unsigned char  draw_cashe_sub [ MAX_PAL_CACHE ] ; // 352 bytes

static void md_sub_proc( int param )
{
	int n ;

	FMSAMPLE* buf[2] ;

	volatile unsigned long* p_proc  = (unsigned long*)( (int)do_proc_param | 0x40000000 ) ;

	//md_core* p_core = (md_core*)( (int)&data_sub | 0x40000000 ) ;
	md_core* p_core = (md_core*)( (int)param | 0x40000000 ) ;

			
	unsigned long *src, *dst ; 

	for(;;)
	{
		/*
		p_proc[0] ++ ;
		p_proc[1] ++ ;
		p_proc[2] ++ ;
		*/
		p_proc[7] ++ ;


		if( p_proc[0] != 0 )
		{
			buf[0] = (FMSAMPLE*)( p_proc[1] ) ;
			buf[1] = (FMSAMPLE*)( p_proc[2] ) ;

			__asm__ volatile ( 
				"lw $28,4(%0)\n" 
				: 
				: "r"(p_proc) 
				) ;

			YM2612UpdateOne( buf, PSP_SOUND_SAMPLE );

			/*
			md_sound_update2( 
				(short*)( (unsigned long)psp_sound_playbuf [snd_outbank] | 0x40000000 ), 
				(short*)( (unsigned long)psp_sound_playbuf2[snd_outbank] | 0x40000000 ), 
				PSP_SOUND_SAMPLE ) ;
			*/

			//_vdp_draw_bg ( 
			//	(unsigned char*)p_proc[1], 
			//	(unsigned char*)p_proc[4]
			//	) ;

			/*
			_vdp_draw_line_norm( 
				(unsigned char*)p_proc[1], 
				(unsigned char*)p_proc[2], 
				(unsigned char*)p_proc[3], 
				(unsigned char*)p_proc[4], 
				(unsigned short)p_proc[5] 
				) ;
			*/

			/*
			src = (unsigned long*)draw_cashe_sub ;
			dst = (unsigned long*)p_proc[4]      ;

			for( n=0; n < MAX_PAL_CACHE/4 ; n++ )
			{
				dst[n] = src[n] ;
			}
			*/

			//*p_proc ++ ;
			//_wait_sub() ;

			p_proc[0] = 0 ;
		}

		//_wait_sub() ;
	}
}

void md_init_sub_proc()
{
	me_start_proc( (unsigned long)md_sub_proc, (unsigned long)mdcore ) ;
}

int md_start_sub_proc()
{
	int n ;
	volatile unsigned long* p_proc  = (unsigned long*)( (int)do_proc_param | 0x40000000 ) ;

	for(;;)
	//for( n=0; n < 8 ; n++ )
	{
		if( p_proc[0] == 0 )
		{
			//sceKernelDelayThread(1) ;

			//mdcore->vdp_draw_pal2dest() ;

			//memcpy( &data_sub, mdcore, sizeof(VDP_DATA) ) ;

			/*
			p_proc[1] = (unsigned long)mdcore->vdp_reg    | 0x40000000 ;
			p_proc[2] = (unsigned long)mdcore->vdp_vsram  | 0x40000000 ;
			p_proc[3] = (unsigned long)mdcore->vdp_vram   | 0x40000000 ;
			p_proc[4] = (unsigned long)mdcore->draw_cashe | 0x40000000 ;
			p_proc[5] = (unsigned long)mdcore->line_count ;
			*/

			p_proc[1] = (unsigned long)( (unsigned long)psp_sound_playbuf [snd_outbank] | 0x40000000 ) ;
			p_proc[2] = (unsigned long)( (unsigned long)psp_sound_playbuf [snd_outbank] | 0x40000000 ) ;

			__asm__ volatile ( 
				"sw $28,4(%0)\n" 
				: 
				: "r"(p_proc) 
				) ;

			sceKernelDcacheWritebackAll() ;

			p_proc[0] = 1 ;
			return 1 ;
		}
		
		//_wait_sub() ;
		//sceKernelDelayThread(1) ;
	}

	return 0 ;
}

/////////////////////////////////////////////////
#endif //_USE_ME
/////////////////////////////////////////////////

void md_init_memory( unsigned char* pCoreBuff, unsigned long nBuffSize )
{
	mdcore = (md_core*)pCoreBuff ;
	/*
	mdcore.md_malloc_mem     = pCoreBuff                          ;
	mdcore.md_malloc_saveram = mdcore.md_malloc_mem     + 0x20000 ;
	mdcore.md_mem_vdp        = mdcore.md_malloc_saveram + 0x20000 ;
	mdcore.md_rom_buf_ptr    = mdcore.md_mem_vdp        + 0x20000 ;

	mdcore.md_max_rom_size   = nBuffSize - 1 * 1024 * 1024 ;
	*/
}

void md_init()
{
	//md_sound_ptr = 0 ;

	mdcore->init() ;

#ifdef _USE_FM_CORE
	if( !YM2612Init  (1, 7520000L, 22050, NULL, NULL) &&
		!SN76496_init(0, 3478000L, 44100, 16) )
	{
		sound_on      = MD_SOUND_22K ;
		sound_is_okay = 1 ;
	}
#endif //_USE_FM_CORE


}

void md_reset(){ mdcore->reset() ; }

int  md_get_checksum()
{
	return mdcore->rom_get_checksum() ;
}

int md_is_loadrom(){ return is_md_loadrom ; }

int  md_select_core( int n )
{
	return mdcore->change_cpu_emu( n ) ;
}

void md_setconfig( EMU_CONFIG* emc )
{
	//mdcore.adjust_clock = emc->emu_clock ;
	//mdcore.adjust_z80   = emc->z80_clock ;

	mdcore->sound_flag = emc->sound_flag ;

	mdcore->country_ver = emc->country ;

	mdcore->pad_set_use_six( emc->pad_option & PAD_OPT_6PAD ) ;

	mdcore->set_palette_mode( emc->palette_mode ) ;

	if( (emc->sound_on > 0) && (emc->sound_on != sound_on) )
	{
		sound_on = emc->sound_on ;

		switch( sound_on )
		{
		case MD_SOUND_11K : YM2612Init( 1, 7520000L, 11025, NULL, NULL) ; break ;
		case MD_SOUND_22K : YM2612Init( 1, 7520000L, 22050, NULL, NULL) ; break ;
		case MD_SOUND_44K : YM2612Init( 1, 7520000L, 44100, NULL, NULL) ; break ;
		}

		mdcore->fm_flush_reg() ;
	}
}

int md_load_rom( const char* path, int fix_checksum )
{
	int ret ;
	ret = mdcore->rom_load( (char*)path ) ;
	if( ret )
	{
		is_md_loadrom = 0 ;
		return ret ;
	}

	is_md_loadrom = 1 ;

	if( fix_checksum )
	{
		mdcore->rom_fix_checksum();
	}

	mdcore->reset() ;
	//mdcore->pad[0] = mdcore.pad[1] = 0xF303F;

	//mdcore.pal = 0 ; //pal_mode;
	//mdcore.pal = 1 ; //pal_mode;

	//memset( sndi.l, 0, MD_SOUND_BUFF_LEN * sizeof(short) ) ;
	//memset( sndi.r, 0, MD_SOUND_BUFF_LEN * sizeof(short) ) ;

	return 0 ;
}

void md_set_adjust_clock( int sysclk, int z80clk )
{
	//mdcore.adjust_clock = sysclk ;
	//mdcore.adjust_z80   = z80clk ;
}

void md_fix_checksum()
{
	mdcore->rom_fix_checksum();
}

int md_is_save_sram ()
{
	return mdcore->rom_is_save_sram() ;
}

int md_load_sram( const char* path )
{
	return mdcore->rom_load_sram( path ) ;
}

int md_save_sram( const char* path )
{
	return mdcore->rom_save_sram( path ) ;
}

int md_get_state( EMU_STATE_SLOT* ess )
{
	ess->buff_len = mdcore->get_state( ess->buff ) ;
	return ess->buff_len ;
}

int md_set_state( EMU_STATE_SLOT* ess )
{
	return mdcore->set_state( ess->buff, ess->buff_len, ess->version ) ;
}

void md_setframe( unsigned char* p, int width )
{
	mdcore->set_frame_info( p, width ) ;
}

void md_set_bpp( unsigned char bpp )
{
	mdcore->set_bpp( bpp ) ;
}

/* 
void clear_frameout()
{
	unsigned long* p = (unsigned long*)mdscr.data ;
	int x,y;

	for( y=0; y < MD_SCREEN_CY + 16 ; y++ )
	{
		p[0] = p[1] = p[2] = p[3] = 0 ;
		p += mdscr.pitch/ 4 ;
	}

	p = (unsigned long*)(mdscr.data+MD_SCREEN_CX*2 +16) ;

	for( y=0; y < MD_SCREEN_CY + 16 ; y++ )
	{
		p[0] = p[1] = p[2] = p[3] = 0 ;
		p += mdscr.pitch / 4 ;
	}
}
*/

int md_is_width320() 
{
	return mdcore->is_width320() ;
}


int md_pad_data[] =
{
	0 ,

	MD_PAD_DATA_A, MD_PAD_DATA_B, MD_PAD_DATA_C, 
	MD_PAD_DATA_X, MD_PAD_DATA_Y, MD_PAD_DATA_Z, 

	MD_PAD_DATA_MODE, MD_PAD_DATA_START,

	MD_PAD_DATA_A, MD_PAD_DATA_B, MD_PAD_DATA_C, 
	MD_PAD_DATA_X, MD_PAD_DATA_Y, MD_PAD_DATA_Z,

	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
} ;

int md_pad_rapid   ;
int md_pad_sel = 0 ;

#define MAX_MD_PAD_NUM        (MD_PAD_START   +1)
#define MAX_MD_PAD_RAPID_NUM  (MD_PAD_RAPID_Z +1)

#define MD_PAD  mdcore->pad

inline void set_md_pad( PSP_PAD_DATA* pPad, int flag, int pad_cfg_num )
{
	if(      pad_cfg_num == 0 ){ return ; }
	else if( pad_cfg_num < MAX_MD_PAD_NUM )
	{
		if( pPad->buttons & flag ){ MD_PAD[md_pad_sel] &= ~md_pad_data[ pad_cfg_num ] ; }
	}
	else if( pad_cfg_num < MAX_MD_PAD_RAPID_NUM )
	{
		if( pPad->buttons & flag ){ md_pad_rapid |= md_pad_data[ pad_cfg_num ] ; }
	}
}

inline void set_md_pad_au( int a_val, int pad_cfg_num )
{
	if(      pad_cfg_num == 0 ){ return ; }
	else if( pad_cfg_num < MAX_MD_PAD_NUM )
	{
		if( a_val > UPPER_THRESHOLD ){ MD_PAD[md_pad_sel] &= ~md_pad_data[ pad_cfg_num ] ; }
	}
	else if( pad_cfg_num < MAX_MD_PAD_RAPID_NUM )
	{
		if( a_val > UPPER_THRESHOLD ){ md_pad_rapid |= md_pad_data[ pad_cfg_num ] ; }
	}
}

inline void set_md_pad_ad( int a_val, int pad_cfg_num )
{
	if(      pad_cfg_num == 0 ){ return ; }
	else if( pad_cfg_num < MAX_MD_PAD_NUM )
	{
		if( a_val < LOWER_THRESHOLD ){ MD_PAD[md_pad_sel] &= ~md_pad_data[ pad_cfg_num ] ; }
	}
	else if( pad_cfg_num < MAX_MD_PAD_RAPID_NUM )
	{
		if( a_val < LOWER_THRESHOLD ){ md_pad_rapid |= md_pad_data[ pad_cfg_num ] ; }
	}
}

inline void set_md_pad_au2( int a_val, int pad_cfg_num )
{
	if(      pad_cfg_num == 0 ){ return ; }
	else if( pad_cfg_num < MAX_MD_PAD_NUM )
	{
		if( a_val > UPPER_THRESHOLD2 ){ MD_PAD[md_pad_sel] &= ~md_pad_data[ pad_cfg_num ] ; }
	}
	else if( pad_cfg_num < MAX_MD_PAD_RAPID_NUM )
	{
		if( a_val > UPPER_THRESHOLD2 ){ md_pad_rapid |= md_pad_data[ pad_cfg_num ] ; }
	}
}

inline void set_md_pad_ad2( int a_val, int pad_cfg_num )
{
	if(      pad_cfg_num == 0 ){ return ; }
	else if( pad_cfg_num < MAX_MD_PAD_NUM )
	{
		if( a_val < LOWER_THRESHOLD2 ){ MD_PAD[md_pad_sel] &= ~md_pad_data[ pad_cfg_num ] ; }
	}
	else if( pad_cfg_num < MAX_MD_PAD_RAPID_NUM )
	{
		if( a_val < LOWER_THRESHOLD2 ){ md_pad_rapid |= md_pad_data[ pad_cfg_num ] ; }
	}
}

void md_setpaddata( PSP_PAD_DATA* pPad, EMU_CONFIG* emc )
{
	/*
#define MD_PAD_UP    (0x00000001)
#define MD_PAD_DOWN  (0x00000002)
#define MD_PAD_LEFT  (0x00000004)
#define MD_PAD_RIGHT (0x00000008)
#define MD_PAD_A     (0x00001000)
#define MD_PAD_B     (0x00000010)
#define MD_PAD_C     (0x00000020)
#define MD_PAD_X     (0x00040000)
#define MD_PAD_Y     (0x00020000)
#define MD_PAD_Z     (0x00010000)
#define MD_PAD_MODE  (0x00080000)
#define MD_PAD_START (0x00002000)

	if( buttons & CTRL_UP       ){ mdcore.pad[0] &= ~MD_PAD_UP    ; }else{ mdcore.pad[0] |= MD_PAD_UP    ; }
	if( buttons & CTRL_DOWN     ){ mdcore.pad[0] &= ~MD_PAD_DOWN  ; }else{ mdcore.pad[0] |= MD_PAD_DOWN  ; }
	if( buttons & CTRL_LEFT     ){ mdcore.pad[0] &= ~MD_PAD_LEFT  ; }else{ mdcore.pad[0] |= MD_PAD_LEFT  ; }
	if( buttons & CTRL_RIGHT    ){ mdcore.pad[0] &= ~MD_PAD_RIGHT ; }else{ mdcore.pad[0] |= MD_PAD_RIGHT ; }
	if( buttons & CTRL_SQUARE   ){ mdcore.pad[0] &= ~MD_PAD_A     ; }else{ mdcore.pad[0] |= MD_PAD_A     ; }
	if( buttons & CTRL_CROSS    ){ mdcore.pad[0] &= ~MD_PAD_B     ; }else{ mdcore.pad[0] |= MD_PAD_B     ; }
	if( buttons & CTRL_CIRCLE   ){ mdcore.pad[0] &= ~MD_PAD_C     ; }else{ mdcore.pad[0] |= MD_PAD_C     ; }
	if( buttons & CTRL_TRIANGLE ){ mdcore.pad[0] &= ~MD_PAD_X     ; }else{ mdcore.pad[0] |= MD_PAD_X     ; }
	if( buttons & CTRL_LTRIGGER ){ mdcore.pad[0] &= ~MD_PAD_Y     ; }else{ mdcore.pad[0] |= MD_PAD_Y     ; }
	if( buttons & CTRL_RTRIGGER ){ mdcore.pad[0] &= ~MD_PAD_Z     ; }else{ mdcore.pad[0] |= MD_PAD_Z     ; }
	if( buttons & CTRL_SELECT   ){ mdcore.pad[0] &= ~MD_PAD_MODE  ; }else{ mdcore.pad[0] |= MD_PAD_MODE  ; }
	if( buttons & CTRL_START    ){ mdcore.pad[0] &= ~MD_PAD_START ; }else{ mdcore.pad[0] |= MD_PAD_START ; }
	*/

	static unsigned char rapid_ct[2] ;

	md_pad_rapid = 0 ;
	MD_PAD[md_pad_sel] = 0xF303F ;

	switch( emc->analog_mode )
	{
	case PSP_ANALOG_CFG  :
		if( pPad->buttons & CTRL_UP    ){ MD_PAD[md_pad_sel] &= ~MD_PAD_DATA_UP    ; }else{ MD_PAD[md_pad_sel] |= MD_PAD_DATA_UP    ; }
		if( pPad->buttons & CTRL_DOWN  ){ MD_PAD[md_pad_sel] &= ~MD_PAD_DATA_DOWN  ; }else{ MD_PAD[md_pad_sel] |= MD_PAD_DATA_DOWN  ; }
		if( pPad->buttons & CTRL_LEFT  ){ MD_PAD[md_pad_sel] &= ~MD_PAD_DATA_LEFT  ; }else{ MD_PAD[md_pad_sel] |= MD_PAD_DATA_LEFT  ; }
		if( pPad->buttons & CTRL_RIGHT ){ MD_PAD[md_pad_sel] &= ~MD_PAD_DATA_RIGHT ; }else{ MD_PAD[md_pad_sel] |= MD_PAD_DATA_RIGHT ; }
		break ;
	case PSP_ANALOG_MOVE :
		if((pPad->buttons & CTRL_UP   )||(pPad->analog[CTRL_ANALOG_Y] < LOWER_THRESHOLD)){ MD_PAD[md_pad_sel] &= ~MD_PAD_DATA_UP    ; }else{ MD_PAD[md_pad_sel] |= MD_PAD_DATA_UP    ; }
		if((pPad->buttons & CTRL_DOWN )||(pPad->analog[CTRL_ANALOG_Y] > UPPER_THRESHOLD)){ MD_PAD[md_pad_sel] &= ~MD_PAD_DATA_DOWN  ; }else{ MD_PAD[md_pad_sel] |= MD_PAD_DATA_DOWN  ; }
		if((pPad->buttons & CTRL_LEFT )||(pPad->analog[CTRL_ANALOG_X] < LOWER_THRESHOLD)){ MD_PAD[md_pad_sel] &= ~MD_PAD_DATA_LEFT  ; }else{ MD_PAD[md_pad_sel] |= MD_PAD_DATA_LEFT  ; }
		if((pPad->buttons & CTRL_RIGHT)||(pPad->analog[CTRL_ANALOG_X] > UPPER_THRESHOLD)){ MD_PAD[md_pad_sel] &= ~MD_PAD_DATA_RIGHT ; }else{ MD_PAD[md_pad_sel] |= MD_PAD_DATA_RIGHT ; }
		break ;
	case PSP_ANALOG_SWAP :
		if( pPad->analog[CTRL_ANALOG_Y] < LOWER_THRESHOLD ){ MD_PAD[md_pad_sel] &= ~MD_PAD_DATA_UP    ; }else{ MD_PAD[md_pad_sel] |= MD_PAD_DATA_UP    ; }
		if( pPad->analog[CTRL_ANALOG_Y] > UPPER_THRESHOLD ){ MD_PAD[md_pad_sel] &= ~MD_PAD_DATA_DOWN  ; }else{ MD_PAD[md_pad_sel] |= MD_PAD_DATA_DOWN  ; }
		if( pPad->analog[CTRL_ANALOG_X] < LOWER_THRESHOLD ){ MD_PAD[md_pad_sel] &= ~MD_PAD_DATA_LEFT  ; }else{ MD_PAD[md_pad_sel] |= MD_PAD_DATA_LEFT  ; }
		if( pPad->analog[CTRL_ANALOG_X] > UPPER_THRESHOLD ){ MD_PAD[md_pad_sel] &= ~MD_PAD_DATA_RIGHT ; }else{ MD_PAD[md_pad_sel] |= MD_PAD_DATA_RIGHT ; }
		break ;
	}

	set_md_pad( pPad, CTRL_LTRIGGER, emc->pad_cfg[ MD_PAD_CFG_L ] ) ;

	if( pPad->buttons & CTRL_RTRIGGER )
	{
		if( emc->pad_cfg[ MD_PAD_CFG_R ] )
		{
			set_md_pad( pPad, CTRL_RTRIGGER, emc->pad_cfg[ MD_PAD_CFG_R ] ) ;
		}
		else
		{
			set_md_pad( pPad, CTRL_CIRCLE  , emc->pad_cfg[ MD_PAD_CFG_R_CIRCLE   ] ) ;
			set_md_pad( pPad, CTRL_CROSS   , emc->pad_cfg[ MD_PAD_CFG_R_CROSS    ] ) ;
			set_md_pad( pPad, CTRL_TRIANGLE, emc->pad_cfg[ MD_PAD_CFG_R_TRIANGLE ] ) ;
			set_md_pad( pPad, CTRL_SQUARE  , emc->pad_cfg[ MD_PAD_CFG_R_SQUARE   ] ) ;
			set_md_pad( pPad, CTRL_SELECT  , emc->pad_cfg[ MD_PAD_CFG_R_SELECT   ] ) ;
			set_md_pad( pPad, CTRL_START   , emc->pad_cfg[ MD_PAD_CFG_R_START    ] ) ;

			if( emc->analog_mode == PSP_ANALOG_SWAP )
			{
				set_md_pad( pPad, CTRL_UP    , emc->pad_cfg[ MD_PAD_CFG_R_DPAD_T ] ) ;
				set_md_pad( pPad, CTRL_DOWN  , emc->pad_cfg[ MD_PAD_CFG_R_DPAD_B ] ) ;
				set_md_pad( pPad, CTRL_LEFT  , emc->pad_cfg[ MD_PAD_CFG_R_DPAD_L ] ) ;
				set_md_pad( pPad, CTRL_RIGHT , emc->pad_cfg[ MD_PAD_CFG_R_DPAD_R ] ) ;
			}
			else //if( emc->analog_mode == PSP_ANALOG_CFG )
			{
				set_md_pad_ad( pPad->analog[CTRL_ANALOG_Y], emc->pad_cfg[ MD_PAD_CFG_R_DPAD_T ] ) ;
				set_md_pad_au( pPad->analog[CTRL_ANALOG_Y], emc->pad_cfg[ MD_PAD_CFG_R_DPAD_B ] ) ;
				set_md_pad_ad( pPad->analog[CTRL_ANALOG_X], emc->pad_cfg[ MD_PAD_CFG_R_DPAD_L ] ) ;
				set_md_pad_au( pPad->analog[CTRL_ANALOG_X], emc->pad_cfg[ MD_PAD_CFG_R_DPAD_R ] ) ;
			}
		}
	}
	else
	{
		set_md_pad( pPad, CTRL_CIRCLE  , emc->pad_cfg[ MD_PAD_CFG_CIRCLE   ] ) ;
		set_md_pad( pPad, CTRL_CROSS   , emc->pad_cfg[ MD_PAD_CFG_CROSS    ] ) ;
		set_md_pad( pPad, CTRL_TRIANGLE, emc->pad_cfg[ MD_PAD_CFG_TRIANGLE ] ) ;
		set_md_pad( pPad, CTRL_SQUARE  , emc->pad_cfg[ MD_PAD_CFG_SQUARE   ] ) ;
		set_md_pad( pPad, CTRL_SELECT  , emc->pad_cfg[ MD_PAD_CFG_SELECT   ] ) ;
		set_md_pad( pPad, CTRL_START   , emc->pad_cfg[ MD_PAD_CFG_START    ] ) ;

		if( emc->analog_mode == PSP_ANALOG_SWAP )
		{
			set_md_pad( pPad, CTRL_UP    , emc->pad_cfg[ MD_PAD_CFG_DPAD_T ] ) ;
			set_md_pad( pPad, CTRL_DOWN  , emc->pad_cfg[ MD_PAD_CFG_DPAD_B ] ) ;
			set_md_pad( pPad, CTRL_LEFT  , emc->pad_cfg[ MD_PAD_CFG_DPAD_L ] ) ;
			set_md_pad( pPad, CTRL_RIGHT , emc->pad_cfg[ MD_PAD_CFG_DPAD_R ] ) ;
		}
		else if( emc->analog_mode == PSP_ANALOG_CFG )
		{
			set_md_pad_ad2( pPad->analog[CTRL_ANALOG_Y], emc->pad_cfg[ MD_PAD_CFG_R_DPAD_T ] ) ;
			set_md_pad_au2( pPad->analog[CTRL_ANALOG_Y], emc->pad_cfg[ MD_PAD_CFG_R_DPAD_B ] ) ;
			set_md_pad_ad2( pPad->analog[CTRL_ANALOG_X], emc->pad_cfg[ MD_PAD_CFG_R_DPAD_L ] ) ;
			set_md_pad_au2( pPad->analog[CTRL_ANALOG_X], emc->pad_cfg[ MD_PAD_CFG_R_DPAD_R ] ) ;
		}
	}

	if( md_pad_rapid )
	{
		if( rapid_ct[md_pad_sel] & 0x01 )
		{
			MD_PAD[md_pad_sel] |=  md_pad_rapid ;
		}
		else
		{
			MD_PAD[md_pad_sel] &= ~md_pad_rapid ;
		}

		rapid_ct[md_pad_sel]++ ;
	}
}

void md_setpaddata2( PSP_PAD_DATA* pPad, EMU_CONFIG* emc )
{
	md_pad_sel = 1 ;

	md_setpaddata( pPad, emc );

	md_pad_sel = 0 ;
}

void md_select_paddata( int sel )
{
	md_pad_sel = sel ;
}

void md_setpad ( int pad, int sel ){ MD_PAD[sel] = pad ; }
int  md_getpad ( int sel ) { return MD_PAD[sel] ; }

void md_cheat_exec( unsigned long addr, unsigned short val )
{
	if( addr & 1 )
	{
		mdcore->write_byte( addr  , val >> 8   ) ;
		mdcore->write_byte( addr+1, val & 0xFF ) ;
	}
	else
	{
		mdcore->write_word( addr, val ) ;
	}
}

int md_run( int z80_async )
{
	if( !is_md_loadrom ){ return 0 ; }

	if( z80_async )
	{
		mdcore->do_frame(0);
	}
	else
	{
		mdcore->do_frame_68k(0) ;
	}

	return 1 ;
}

int md_run_skip( int z80_async )
{
	if( !is_md_loadrom ){ return 0 ; }

	if( z80_async )
	{
		mdcore->do_frame(1) ;
	}
	else
	{
		mdcore->do_frame_68k(1) ;
	}

	return 1 ;
}

void md_sound_update( int z80_async, short* p, short len )
{
	if( !z80_async ){ mdcore->do_frame_z80() ; }
	mdcore->make_sound( (unsigned short*)p, len ) ;
}

void md_sound_update2( int z80_async, short* p1, short* p2, short len )
{
	if( !z80_async ){ mdcore->do_frame_z80() ; }
	mdcore->dac_make( (unsigned short*)p2, len ) ;
	mdcore->fm_make ( (unsigned short*)p1, len ) ;
}

#ifdef __cplusplus
}
#endif
