///////////////////////////////////////////////////////////////////////////
/// emu_config.c
///////////////////////////////////////////////////////////////////////////
#include "emu_config.h"
#include "psp_cfg_file.h"
///////////////////////////////////////////////////////////////////////////

#define FILENAME_DEF_CFG "CFG/DEFAULT.INI"
#define FILENAME_SYS_CFG "CFG/SYSTEM.INI"

EMU_CONFIG      def_cfg       ;
EMU_SYS_CONFIG  sys_cfg       ;
EMU_SYS_CONFIG  sys_cfg_old   ;
char            path_old[128] ;

///////////////////////////////////////////////////////////////
#define EMU_SYSCFG_FILEVER  (1)
const char SZ_SYSCFG_TITLE       [] = "[DGEN for PSP SysConfig]" ;
const char SZ_SYSCFG_KEY_VERSION [] = "VERSION =" ;
const char SZ_SYSCFG_KEY_ROMPATH [] = "ROMPATH =" ;
const char SZ_SYSCFG_KEY_MENUKEY [] = "MENUKEY =" ;
const char SZ_SYSCFG_KEY_KEY_REP [] = "KEY_REP =" ;
const char SZ_SYSCFG_KEY_OPTION  [] = "OPTION  =" ;
const char SZ_SYSCFG_KEY_SHADOW  [] = "SHADOW  =" ;
const char SZ_SYSCFG_KEY_BG_MODE [] = "BG_MODE =" ;
const char SZ_SYSCFG_KEY_COL_BG  [] = "COL_BG  =" ;
const char SZ_SYSCFG_KEY_COL_GRAD[] = "COL_GRAD=" ;
const char SZ_SYSCFG_KEY_COL_TITL[] = "COL_TITL=" ;
const char SZ_SYSCFG_KEY_COL_SEL [] = "COL_SEL =" ;
const char SZ_SYSCFG_KEY_COL_MSGB[] = "COL_MSGB=" ;
const char SZ_SYSCFG_KEY_COL_MSG [] = "COL_MSG =" ;
const char SZ_SYSCFG_KEY_COL_TEXT[] = "COL_TEXT=" ;
const char SZ_SYSCFG_KEY_COL_SHAD[] = "COL_SHAD=" ;

EMU_SYS_CONFIG* EmuSysCfg_Get() { return &sys_cfg ; }

#define PSP_RGB(r,g,b) ((((b>>3) & 0x1F)<<10)|(((g>>3) & 0x1F)<<5)|(((r>>3) & 0x1F)<<0))

void EmuSysCfg_Copy( EMU_SYS_CONFIG* pDst, EMU_SYS_CONFIG* pSrc )
{
	if( pSrc->rom_path && pDst->rom_path )
	{
		strcpy( pDst->rom_path, pSrc->rom_path ) ;
	}

	pDst->key_menu     = pSrc->key_menu     ; 
	pDst->key_repeat   = pSrc->key_repeat   ; 

	pDst->option_flags = pSrc->option_flags ; 

	pDst->shadow_on    = pSrc->shadow_on    ; 

	pDst->bg_mode      = pSrc->bg_mode      ; 

	pDst->color_bg     = pSrc->color_bg     ; 
	pDst->color_grad   = pSrc->color_grad   ; 

	pDst->color_title  = pSrc->color_title  ; 
	pDst->color_select = pSrc->color_select ; 
	pDst->color_msgbox = pSrc->color_msgbox ; 
	pDst->color_msg    = pSrc->color_msg    ; 
	pDst->color_text   = pSrc->color_text   ; 
	pDst->color_shadow = pSrc->color_shadow ; 
}

int EmuSysCfg_Compare( EMU_SYS_CONFIG* pDst, EMU_SYS_CONFIG* pSrc )
{
	if( pDst->key_menu     != pSrc->key_menu     ){ return 1 ; } 
	if( pDst->key_repeat   != pSrc->key_repeat   ){ return 1 ; }

	if( pDst->option_flags != pSrc->option_flags ){ return 1 ; }

	if( pDst->shadow_on    != pSrc->shadow_on    ){ return 1 ; }

	if( pDst->bg_mode      != pSrc->bg_mode      ){ return 1 ; }

	if( pDst->color_bg     != pSrc->color_bg     ){ return 1 ; }
	if( pDst->color_grad   != pSrc->color_grad   ){ return 1 ; }

	if( pDst->color_title  != pSrc->color_title  ){ return 1 ; }
	if( pDst->color_select != pSrc->color_select ){ return 1 ; }
	if( pDst->color_msgbox != pSrc->color_msgbox ){ return 1 ; }
	if( pDst->color_msg    != pSrc->color_msg    ){ return 1 ; }
	if( pDst->color_text   != pSrc->color_text   ){ return 1 ; }
	if( pDst->color_shadow != pSrc->color_shadow ){ return 1 ; }

	if( pSrc->rom_path && pDst->rom_path )
	{
		return strcmp( pDst->rom_path, pSrc->rom_path ) ;
	}

	return 0 ;
}

int EmuSysCfg_Init( char* path )
{
	int  pos, ver, n ;
	char buff[128] ;

	sys_cfg.key_menu     = KEY_MENU_L           ;
	sys_cfg.key_repeat   = KEY_REPEAT_MID       ;
	sys_cfg.bg_mode      = BG_MODE_ROLL         ;
	sys_cfg.option_flags = OPTION_SHOW_CLOCK | OPTION_SHOW_BATT1 | OPTION_SHOW_BATT2 | OPTION_LOWBATT ;
	sys_cfg.shadow_on    = 1                    ;
	sys_cfg.color_bg     = PSP_RGB(  0, 96,128) ;
	sys_cfg.color_grad   = 240                  ;
	sys_cfg.color_title  = PSP_RGB(  0,255,255) ;
	sys_cfg.color_msg    = PSP_RGB(192,255,192) ;
	sys_cfg.color_msgbox = PSP_RGB(255,255,255) ;
	sys_cfg.color_select = PSP_RGB(255,255,128) ;
	sys_cfg.color_text   = PSP_RGB(192,192,192) ;
	sys_cfg.color_shadow = PSP_RGB( 96, 96, 96) ;

	strcpy( buff, path ) ;
	strcat( buff, FILENAME_SYS_CFG ) ;

	if( PSP_CfgFile_Read( buff ) )
	{
		pos = PSP_CfgFile_FindKey( SZ_SYSCFG_TITLE ) ;
		if( pos < 0 ){ return -1 ; }

		pos = PSP_CfgFile_FindKey( SZ_SYSCFG_KEY_VERSION ) ;
		if( pos > 0 ){ ver = PSP_CfgFile_GetInt( pos ) ; }

		if( sys_cfg.rom_path )
		{
			pos = PSP_CfgFile_FindKey( SZ_SYSCFG_KEY_ROMPATH ) ;
			if( pos > 0 ){ n = PSP_CfgFile_GetStr( pos, sys_cfg.rom_path, 127 ) ; }
		}

		pos = PSP_CfgFile_FindKey( SZ_SYSCFG_KEY_MENUKEY  ) ;
		if( pos > 0 ){ sys_cfg.key_menu = PSP_CfgFile_GetInt( pos ) ; }

		pos = PSP_CfgFile_FindKey( SZ_SYSCFG_KEY_KEY_REP  ) ;
		if( pos > 0 ){ sys_cfg.key_repeat = PSP_CfgFile_GetInt( pos ) ; }

		pos = PSP_CfgFile_FindKey( SZ_SYSCFG_KEY_OPTION   ) ;
		if( pos > 0 ){ sys_cfg.option_flags = PSP_CfgFile_GetHex( pos ) ; }

		pos = PSP_CfgFile_FindKey( SZ_SYSCFG_KEY_SHADOW   ) ;
		if( pos > 0 ){ sys_cfg.shadow_on = PSP_CfgFile_GetInt( pos ) ; }

		pos = PSP_CfgFile_FindKey( SZ_SYSCFG_KEY_BG_MODE  ) ;
		if( pos > 0 ){ sys_cfg.bg_mode = PSP_CfgFile_GetInt( pos ) ; }

		pos = PSP_CfgFile_FindKey( SZ_SYSCFG_KEY_COL_BG   ) ;
		if( pos > 0 ){ sys_cfg.color_bg = PSP_CfgFile_GetHex( pos ) ; }

		pos = PSP_CfgFile_FindKey( SZ_SYSCFG_KEY_COL_GRAD ) ;
		if( pos > 0 ){ sys_cfg.color_grad = PSP_CfgFile_GetInt( pos ) ; }

		pos = PSP_CfgFile_FindKey( SZ_SYSCFG_KEY_COL_TITL ) ;
		if( pos > 0 ){ sys_cfg.color_title = PSP_CfgFile_GetHex( pos ) ; }

		pos = PSP_CfgFile_FindKey( SZ_SYSCFG_KEY_COL_SEL  ) ;
		if( pos > 0 ){ sys_cfg.color_select = PSP_CfgFile_GetHex( pos ) ; }

		pos = PSP_CfgFile_FindKey( SZ_SYSCFG_KEY_COL_MSGB ) ;
		if( pos > 0 ){ sys_cfg.color_msgbox = PSP_CfgFile_GetHex( pos ) ; }

		pos = PSP_CfgFile_FindKey( SZ_SYSCFG_KEY_COL_MSG  ) ;
		if( pos > 0 ){ sys_cfg.color_msg = PSP_CfgFile_GetHex( pos ) ; }

		pos = PSP_CfgFile_FindKey( SZ_SYSCFG_KEY_COL_TEXT ) ;
		if( pos > 0 ){ sys_cfg.color_text = PSP_CfgFile_GetHex( pos ) ; }

		pos = PSP_CfgFile_FindKey( SZ_SYSCFG_KEY_COL_SHAD ) ;
		if( pos > 0 ){ sys_cfg.color_shadow = PSP_CfgFile_GetHex( pos ) ; }
	}

	sys_cfg_old.rom_path = path_old ;
	EmuSysCfg_Copy( &sys_cfg_old, &sys_cfg ) ;

	return 0 ;
}

int EmuSysCfg_Save( char* path )
{
	char buff[128];

	if( EmuSysCfg_Compare( &sys_cfg_old, &sys_cfg ) )
	{
		strcpy( buff, path ) ;
		strcat( buff, FILENAME_SYS_CFG ) ;

		PSP_CfgFile_Init    ( SZ_SYSCFG_TITLE ) ;
		PSP_CfgFile_MakeInt ( SZ_SYSCFG_KEY_VERSION    , EMU_SYSCFG_FILEVER ) ;

		if( sys_cfg.rom_path )
		{
			PSP_CfgFile_MakeStr( SZ_SYSCFG_KEY_ROMPATH , sys_cfg.rom_path ) ;
		}

		PSP_CfgFile_MakeInt( SZ_SYSCFG_KEY_MENUKEY , sys_cfg.key_menu     ) ;
		PSP_CfgFile_MakeInt( SZ_SYSCFG_KEY_KEY_REP , sys_cfg.key_repeat   ) ;
		PSP_CfgFile_MakeHex( SZ_SYSCFG_KEY_OPTION  , sys_cfg.option_flags ) ;
		PSP_CfgFile_MakeInt( SZ_SYSCFG_KEY_SHADOW  , sys_cfg.shadow_on    ) ;
		PSP_CfgFile_MakeInt( SZ_SYSCFG_KEY_BG_MODE , sys_cfg.bg_mode      ) ;
		PSP_CfgFile_MakeHex( SZ_SYSCFG_KEY_COL_BG  , sys_cfg.color_bg     ) ;
		PSP_CfgFile_MakeInt( SZ_SYSCFG_KEY_COL_GRAD, sys_cfg.color_grad   ) ;
		PSP_CfgFile_MakeHex( SZ_SYSCFG_KEY_COL_TITL, sys_cfg.color_title  ) ;
		PSP_CfgFile_MakeHex( SZ_SYSCFG_KEY_COL_SEL , sys_cfg.color_select ) ;
		PSP_CfgFile_MakeHex( SZ_SYSCFG_KEY_COL_MSGB, sys_cfg.color_msgbox ) ;
		PSP_CfgFile_MakeHex( SZ_SYSCFG_KEY_COL_MSG , sys_cfg.color_msg    ) ;
		PSP_CfgFile_MakeHex( SZ_SYSCFG_KEY_COL_TEXT, sys_cfg.color_text   ) ;
		PSP_CfgFile_MakeHex( SZ_SYSCFG_KEY_COL_SHAD, sys_cfg.color_shadow ) ;

		return PSP_CfgFile_Write( buff ) ;
	}

	return 0 ;
}

void EmuSysCfg_CheckValue ()
{
	if(      sys_cfg.key_menu < KEY_MENU_L       ){ sys_cfg.key_menu = (MAX_KEY_MENU-1) ; }
	else if( sys_cfg.key_menu > (MAX_KEY_MENU-1) ){ sys_cfg.key_menu = KEY_MENU_L       ; }

	if(      sys_cfg.key_repeat < KEY_REPEAT_FAST ){ sys_cfg.key_repeat = KEY_REPEAT_FAST ; }
	else if( sys_cfg.key_repeat > KEY_REPEAT_SLOW ){ sys_cfg.key_repeat = KEY_REPEAT_SLOW ; }

	if(      sys_cfg.bg_mode < 0               ){ sys_cfg.bg_mode = MAX_BG_MODE-1 ; }
	else if( sys_cfg.bg_mode > (MAX_BG_MODE-1) ){ sys_cfg.bg_mode = 0             ; }

	if(      sys_cfg.color_grad < 0   ){ sys_cfg.color_grad = 360 ; }
	else if( sys_cfg.color_grad > 360 ){ sys_cfg.color_grad = 0   ; }
}

////////////////////////////////////////////////////////////////

void EmuCfg_Copy( EMU_CONFIG* src, EMU_CONFIG* dst )
{
	int n ;

	if( src == dst ){ return ; }

#ifdef _EMU_CLOCK_ADJ ///////////////////
	dst->emu_clock    = src->emu_clock   ;
	dst->z80_clock    = src->z80_clock   ;
#endif //

	dst->cpu_freq     = src->cpu_freq     ;
	dst->country      = src->country      ;
	dst->max_fps      = src->max_fps      ;
	dst->skip_count   = src->skip_count   ;
	dst->skip_mode    = src->skip_mode    ;
	dst->skip_turbo   = src->skip_turbo   ;
	dst->screen_mode  = src->screen_mode  ;
	dst->show_fps     = src->show_fps     ;
	dst->show_flags   = src->show_flags   ;
	dst->palette_mode = src->palette_mode ;
	dst->autosram     = src->autosram     ;
	dst->sound_on     = src->sound_on     ;
	dst->sound_vol    = src->sound_vol    ;
	dst->sound_flag   = src->sound_flag   ;

	dst->comm_mode    = src->comm_mode    ;
	dst->analog_mode  = src->analog_mode  ;

	for( n=0; n < MAX_MD_PAD_CFG ; n++ )
	{
		dst->pad_cfg[n] = src->pad_cfg[n] ;
	}
}

void EmuCfg_InitDefault( char* path )
{
	char buff[128];

	strcpy( buff, path  );
	strcat( buff, "CFG" );

#ifndef WIN32
	sceIoMkdir( buff, 777 );
#endif //WIN32

	strcpy( buff, path ) ;
	strcat( buff, FILENAME_DEF_CFG ) ;

	EmuCfg_init( &def_cfg ) ;

	EmuCfg_Load( &def_cfg, buff ) ;
}

void EmuCfg_SaveDefault( char* path )
{
	char buff[128];
	strcpy( buff, path ) ;
	strcat( buff, FILENAME_DEF_CFG ) ;

	EmuCfg_Save( &def_cfg, buff ) ;
}

void EmuCfg_GetDefault( EMU_CONFIG* emc )
{
	EmuCfg_Copy( &def_cfg, emc ) ;
}

void EmuCfg_SetDefault_Std( EMU_CONFIG* emc )
{
#ifdef _EMU_CLOCK_ADJ ///////////////////
	def_cfg.emu_clock    = emc->emu_clock    ;
	def_cfg.z80_clock    = emc->z80_clock    ;
#endif //

	def_cfg.cpu_freq     = emc->cpu_freq     ;
	def_cfg.country      = emc->country      ;
	def_cfg.max_fps      = emc->max_fps      ;
	def_cfg.skip_count   = emc->skip_count   ;
	def_cfg.skip_mode    = emc->skip_mode    ;
	def_cfg.skip_turbo   = emc->skip_turbo   ;
	def_cfg.screen_mode  = emc->screen_mode  ;
	def_cfg.show_fps     = emc->show_fps     ;
	def_cfg.show_flags   = emc->show_flags   ;
	def_cfg.palette_mode = emc->palette_mode ;
	def_cfg.autosram     = emc->autosram     ;
	def_cfg.sound_on     = emc->sound_on     ;
	def_cfg.sound_vol    = emc->sound_vol    ;
	def_cfg.sound_flag   = emc->sound_flag   ;
	def_cfg.comm_mode    = emc->comm_mode    ;
}

void EmuCfg_SetDefault_Key( EMU_CONFIG* emc )
{
	int n ;

	def_cfg.analog_mode = emc->analog_mode ;
	def_cfg.pad_option  = emc->pad_option  ;

	for( n=0; n < MAX_MD_PAD_CFG ; n++ )
	{
		def_cfg.pad_cfg[n] = emc->pad_cfg[n] ;
	}
}

void EmuCfg_init( EMU_CONFIG* emc )
{
	int n ;

#ifdef _EMU_CLOCK_ADJ ///////////////////
	emc->emu_clock   = 0            ;
	emc->z80_clock   = 0            ;
#endif //

	emc->cpu_freq     = CPU_FREQ_333 ;
	emc->country      = MD_LANG_JPN  ;
	emc->max_fps      = 60           ;
	emc->skip_count   = 1            ;
	emc->skip_mode    = SKIP_MODE_F  ;
	emc->skip_turbo   = 10           ;
	emc->screen_mode  = SCREEN_1X    ;
	emc->show_fps     = SHOW_FPS_NON ;
	emc->show_flags   = 0x1F         ;
	emc->palette_mode = 1            ;
	emc->autosram     = 1            ;
	emc->sound_on     = MD_SOUND_22K ;
	emc->sound_vol    = VOL_14       ;
	emc->sound_flag   = 0            ;
	emc->analog_mode  = 0            ;
	emc->comm_mode    = 0            ;

	emc->pad_cfg[ MD_PAD_CFG_CIRCLE     ] = MD_PAD_C         ;
	emc->pad_cfg[ MD_PAD_CFG_CROSS      ] = MD_PAD_B         ;
	emc->pad_cfg[ MD_PAD_CFG_TRIANGLE   ] = MD_PAD_RAPID_B   ;
	emc->pad_cfg[ MD_PAD_CFG_SQUARE     ] = MD_PAD_A         ;

	emc->pad_cfg[ MD_PAD_CFG_R_CIRCLE   ] = MD_PAD_Z         ;
	emc->pad_cfg[ MD_PAD_CFG_R_CROSS    ] = MD_PAD_RAPID_Y   ;
	emc->pad_cfg[ MD_PAD_CFG_R_TRIANGLE ] = MD_PAD_Y         ;
	emc->pad_cfg[ MD_PAD_CFG_R_SQUARE   ] = MD_PAD_X         ;

	emc->pad_cfg[ MD_PAD_CFG_SELECT     ] = MD_PAD_MODE      ;
	emc->pad_cfg[ MD_PAD_CFG_START      ] = MD_PAD_START     ;
	emc->pad_cfg[ MD_PAD_CFG_R_SELECT   ] = MD_PAD_STATE_SVL ;
	emc->pad_cfg[ MD_PAD_CFG_R_START    ] = MD_PAD_STATE_LDL ;

	emc->pad_cfg[ MD_PAD_CFG_DPAD_T     ] = MD_PAD_NONE      ;
	emc->pad_cfg[ MD_PAD_CFG_DPAD_B     ] = MD_PAD_NONE      ;
	emc->pad_cfg[ MD_PAD_CFG_DPAD_L     ] = MD_PAD_NONE      ;
	emc->pad_cfg[ MD_PAD_CFG_DPAD_R     ] = MD_PAD_NONE      ;

	emc->pad_cfg[ MD_PAD_CFG_R_DPAD_T   ] = MD_PAD_SCREEN    ;
	emc->pad_cfg[ MD_PAD_CFG_R_DPAD_B   ] = MD_PAD_FPS       ;
	emc->pad_cfg[ MD_PAD_CFG_R_DPAD_L   ] = MD_PAD_SOUND_V_D ;
	emc->pad_cfg[ MD_PAD_CFG_R_DPAD_R   ] = MD_PAD_SOUND_V_U ;

	emc->pad_cfg[ MD_PAD_CFG_L          ] = MD_PAD_NONE      ;
	emc->pad_cfg[ MD_PAD_CFG_R          ] = MD_PAD_NONE      ;
}

void EmuCfg_CheckValue( EMU_CONFIG* emc )
{
	int n ;

#ifdef _EMU_CLOCK_ADJ ///////////////////
	if(      emc->emu_clock   < MIN_EMU_CLOCK  ){ emc->emu_clock   = MIN_EMU_CLOCK  ; }
	else if( emc->emu_clock   > MAX_EMU_CLOCK  ){ emc->emu_clock   = MAX_EMU_CLOCK  ; }

	if(      emc->z80_clock   < MIN_Z80_CLOCK  ){ emc->z80_clock   = MIN_Z80_CLOCK  ; }
	else if( emc->z80_clock   > MAX_Z80_CLOCK  ){ emc->z80_clock   = MAX_Z80_CLOCK  ; }
#endif //

	if(      emc->cpu_freq    < CPU_FREQ_222   ){ emc->cpu_freq    = CPU_FREQ_222   ; }
	else if( emc->cpu_freq    > CPU_FREQ_333   ){ emc->cpu_freq    = CPU_FREQ_333   ; }

	if(      emc->skip_count  < MIN_FRAME_SKIP ){ emc->skip_count  = MIN_FRAME_SKIP ; }
	else if( emc->skip_count  > MAX_FRAME_SKIP ){ emc->skip_count  = MAX_FRAME_SKIP ; }

	if(      emc->skip_mode   < SKIP_MODE_A    ){ emc->skip_mode   = SKIP_MODE_A    ; }
	else if( emc->skip_mode   > SKIP_MODE_R    ){ emc->skip_mode   = SKIP_MODE_R    ; }

	if(      emc->skip_turbo  < MIN_FRAME_SKIP_T ){ emc->skip_turbo  = MIN_FRAME_SKIP_T ; }
	else if( emc->skip_turbo  > MAX_FRAME_SKIP_T ){ emc->skip_turbo  = MAX_FRAME_SKIP_T ; }

	if(      emc->show_fps    < SHOW_FPS_NON   ){ emc->show_fps    = SHOW_FPS_NON   ; }
	else if( emc->show_fps    > SHOW_FPS_RB    ){ emc->show_fps    = SHOW_FPS_RB    ; }

	if(      emc->max_fps     < MIN_MAX_FPS    ){ emc->max_fps     = MIN_MAX_FPS    ; }
	else if( emc->max_fps     > MAX_MAX_FPS    ){ emc->max_fps     = MAX_MAX_FPS    ; }

	if(      emc->screen_mode < SCREEN_1X      ){ emc->screen_mode = SCREEN_1X      ; }
	else if( emc->screen_mode > SCREEN_FIT43   ){ emc->screen_mode = SCREEN_FIT43   ; }

	if(      emc->palette_mode < PALETTE_NORMAL ){ emc->palette_mode = PALETTE_NORMAL ; }
	else if( emc->palette_mode > PALETTE_H_BLT  ){ emc->palette_mode = PALETTE_H_BLT  ; }

	if(      emc->country     < MD_LANG_JPN    ){ emc->country     = MD_LANG_JPN    ; }
	else if( emc->country     > MD_LANG_EUR    ){ emc->country     = MD_LANG_EUR    ; }

	if(      emc->sound_on    < MD_SOUND_OFF   ){ emc->sound_on    = MD_SOUND_OFF   ; }
	else if( emc->sound_on    > MD_SOUND_44K   ){ emc->sound_on    = MD_SOUND_44K   ; }

	if(      emc->sound_vol   < VOL_08         ){ emc->sound_vol   = VOL_08         ; }
	else if( emc->sound_vol   > VOL_16         ){ emc->sound_vol   = VOL_16         ; }

	if(      emc->comm_mode   < COMM_NON       ){ emc->comm_mode   = COMM_NON       ; }
	else if( emc->comm_mode   > COMM_2P        ){ emc->comm_mode   = COMM_2P        ; }

	if(      emc->analog_mode < PSP_ANALOG_CFG  ){ emc->analog_mode = PSP_ANALOG_CFG  ; }
	else if( emc->analog_mode > PSP_ANALOG_SWAP ){ emc->analog_mode = PSP_ANALOG_SWAP ; }

	for( n=0; n < MAX_MD_PAD_CFG ; n++ )
	{
		if     ( emc->pad_cfg[n] < MD_PAD_NONE    ){ emc->pad_cfg[n] = MAX_MD_PAD-1 ; }
		else if( emc->pad_cfg[n] > (MAX_MD_PAD-1) ){ emc->pad_cfg[n] = MD_PAD_NONE  ; }
	}
}

#define EMU_CFG_FILEVER  (1)

const char SZ_CFG_TITLE       [] = "[DGEN for PSP Config]" ;
const char SZ_CFG_KEY_VERSION [] = "VERSION =" ;
const char SZ_CFG_KEY_CPU     [] = "CPUFREQ =" ;
const char SZ_CFG_KEY_COUNTRY [] = "COUNTRY =" ;
const char SZ_CFG_KEY_FPS     [] = "MAX_FPS =" ;
const char SZ_CFG_KEY_SKIP    [] = "MAX_SKIP=" ;
const char SZ_CFG_KEY_SKIP_M  [] = "SKIP_M  =" ;
const char SZ_CFG_KEY_SKIP_T  [] = "SKIP_T  =" ;
const char SZ_CFG_KEY_SCREEN  [] = "SCREEN_M=" ;
const char SZ_CFG_KEY_SHOWFPS [] = "SHOW_FPS=" ;
const char SZ_CFG_KEY_SHOWFLAG[] = "SHOW_FLG=" ;
const char SZ_CFG_KEY_PALETTE [] = "PALETTE =" ;
const char SZ_CFG_KEY_SRAM    [] = "AUTOSRAM=" ;
const char SZ_CFG_KEY_SOUND   [] = "SOUND_M =" ;
const char SZ_CFG_KEY_VOL     [] = "SOUND_V =" ;
const char SZ_CFG_KEY_SOUND_F [] = "SOUND_F =" ;
const char SZ_CFG_KEY_COMM    [] = "COMMMODE=" ;
const char SZ_CFG_KEY_ANALOG  [] = "ANALOG_M=" ;
const char SZ_CFG_KEY_PADOPT  [] = "PAD_OPT =" ;
      char SZ_CFG_KEY_PADCFG  [] = "PADCFG00=" ;
#define NUM_CFG_KEY         (sizeof(SZ_CFG_KEY_VERSION))


int EmuCfg_Load ( EMU_CONFIG* emc, char* path )
{
	int pos, ver, n ;

	EmuCfg_Copy( &def_cfg, emc ) ;

	{
		if( PSP_CfgFile_Read( path ) )
		{
			{
				pos = PSP_CfgFile_FindKey( SZ_CFG_TITLE ) ;
				if( pos < 0 ){ goto _Ret ; }

				pos = PSP_CfgFile_FindKey( SZ_CFG_KEY_VERSION ) ;
				if( pos > 0 ){ ver = PSP_CfgFile_GetInt( pos ) ; }

				pos = PSP_CfgFile_FindKey( SZ_CFG_KEY_CPU ) ;
				if( pos > 0 ){ emc->cpu_freq = PSP_CfgFile_GetInt( pos ) ; }

				pos = PSP_CfgFile_FindKey( SZ_CFG_KEY_COUNTRY  ) ;
				if( pos > 0 ){ emc->country = PSP_CfgFile_GetInt( pos ) ; }

				pos = PSP_CfgFile_FindKey( SZ_CFG_KEY_FPS      ) ;
				if( pos > 0 ){ emc->max_fps = PSP_CfgFile_GetInt( pos ) ; }

				pos = PSP_CfgFile_FindKey( SZ_CFG_KEY_SKIP     ) ;
				if( pos > 0 ){ emc->skip_count = PSP_CfgFile_GetInt( pos ) ; }

				pos = PSP_CfgFile_FindKey( SZ_CFG_KEY_SKIP_M   ) ;
				if( pos > 0 ){ emc->skip_mode = PSP_CfgFile_GetInt( pos ) ; }

				pos = PSP_CfgFile_FindKey( SZ_CFG_KEY_SKIP_T   ) ;
				if( pos > 0 ){ emc->skip_turbo = PSP_CfgFile_GetInt( pos ) ; }

				pos = PSP_CfgFile_FindKey( SZ_CFG_KEY_SCREEN   ) ;
				if( pos > 0 ){ emc->screen_mode = PSP_CfgFile_GetInt( pos ) ; }

				pos = PSP_CfgFile_FindKey( SZ_CFG_KEY_SHOWFPS  ) ;
				if( pos > 0 ){ emc->show_fps = PSP_CfgFile_GetInt( pos ) ; }

				pos = PSP_CfgFile_FindKey( SZ_CFG_KEY_SHOWFLAG ) ;
				if( pos > 0 ){ emc->show_flags = PSP_CfgFile_GetHex( pos ) ; }

				pos = PSP_CfgFile_FindKey( SZ_CFG_KEY_PALETTE   ) ;
				if( pos > 0 ){ emc->palette_mode = PSP_CfgFile_GetInt( pos ) ; }

				pos = PSP_CfgFile_FindKey( SZ_CFG_KEY_SRAM     ) ;
				if( pos > 0 ){ emc->autosram = PSP_CfgFile_GetInt( pos ) ; }

				pos = PSP_CfgFile_FindKey( SZ_CFG_KEY_SOUND    ) ;
				if( pos > 0 ){ emc->sound_on = PSP_CfgFile_GetInt( pos ) ; }

				pos = PSP_CfgFile_FindKey( SZ_CFG_KEY_VOL      ) ;
				if( pos > 0 ){ emc->sound_vol = PSP_CfgFile_GetInt( pos ) ; }

				pos = PSP_CfgFile_FindKey( SZ_CFG_KEY_SOUND_F  ) ;
				if( pos > 0 ){ emc->sound_flag = PSP_CfgFile_GetHex( pos ) ; }

				pos = PSP_CfgFile_FindKey( SZ_CFG_KEY_COMM ) ;
				if( pos > 0 ){ emc->comm_mode = PSP_CfgFile_GetInt( pos ) ; }

				pos = PSP_CfgFile_FindKey( SZ_CFG_KEY_ANALOG   ) ;
				if( pos > 0 ){ emc->analog_mode = PSP_CfgFile_GetInt( pos ) ; }

				pos = PSP_CfgFile_FindKey( SZ_CFG_KEY_PADOPT   ) ;
				if( pos > 0 ){ emc->pad_option = PSP_CfgFile_GetHex( pos ) ; }

				for( n=0; n < MAX_MD_PAD_CFG ; n++ )
				{
					SZ_CFG_KEY_PADCFG[ sizeof(SZ_CFG_KEY_PADCFG) -4 ] = '0' + n / 10 ;
					SZ_CFG_KEY_PADCFG[ sizeof(SZ_CFG_KEY_PADCFG) -3 ] = '0' + n % 10 ;
					pos = PSP_CfgFile_FindKey( SZ_CFG_KEY_PADCFG   ) ;
					if( pos > 0 ){ emc->pad_cfg[n] = PSP_CfgFile_GetInt( pos ) ; }
				}
			}
		}
	}

_Ret :
	EmuCfg_CheckValue( emc ) ;

	return 1 ;
}

int EmuCfg_Save ( EMU_CONFIG* emc, char* path )
{
	int n ;

	{
		PSP_CfgFile_Init   ( SZ_CFG_TITLE ) ;

		PSP_CfgFile_MakeInt( SZ_CFG_KEY_VERSION , EMU_CFG_FILEVER   ) ;
		PSP_CfgFile_MakeInt( SZ_CFG_KEY_CPU     , emc->cpu_freq     ) ;
		PSP_CfgFile_MakeInt( SZ_CFG_KEY_COUNTRY , emc->country      ) ;
		PSP_CfgFile_MakeInt( SZ_CFG_KEY_FPS     , emc->max_fps      ) ;
		PSP_CfgFile_MakeInt( SZ_CFG_KEY_SKIP    , emc->skip_count   ) ;
		PSP_CfgFile_MakeInt( SZ_CFG_KEY_SKIP_M  , emc->skip_mode    ) ;
		PSP_CfgFile_MakeInt( SZ_CFG_KEY_SKIP_T  , emc->skip_turbo   ) ;
		PSP_CfgFile_MakeInt( SZ_CFG_KEY_SCREEN  , emc->screen_mode  ) ;
		PSP_CfgFile_MakeInt( SZ_CFG_KEY_SHOWFPS , emc->show_fps     ) ;
		PSP_CfgFile_MakeHex( SZ_CFG_KEY_SHOWFLAG, emc->show_flags   ) ;
		PSP_CfgFile_MakeInt( SZ_CFG_KEY_PALETTE , emc->palette_mode ) ;
		PSP_CfgFile_MakeInt( SZ_CFG_KEY_SRAM    , emc->autosram     ) ;
		PSP_CfgFile_MakeInt( SZ_CFG_KEY_SOUND   , emc->sound_on     ) ;
		PSP_CfgFile_MakeInt( SZ_CFG_KEY_VOL     , emc->sound_vol    ) ;
		PSP_CfgFile_MakeHex( SZ_CFG_KEY_SOUND_F , emc->sound_flag   ) ;
		PSP_CfgFile_MakeInt( SZ_CFG_KEY_COMM    , emc->comm_mode    ) ;
		PSP_CfgFile_MakeInt( SZ_CFG_KEY_ANALOG  , emc->analog_mode  ) ;
		PSP_CfgFile_MakeHex( SZ_CFG_KEY_PADOPT  , emc->pad_option   ) ;

		for( n=0; n < MAX_MD_PAD_CFG ; n++ )
		{
			SZ_CFG_KEY_PADCFG[ sizeof(SZ_CFG_KEY_PADCFG) -4 ] = '0' + n / 10 ;
			SZ_CFG_KEY_PADCFG[ sizeof(SZ_CFG_KEY_PADCFG) -3 ] = '0' + n % 10 ;
			PSP_CfgFile_MakeInt( SZ_CFG_KEY_PADCFG, emc->pad_cfg[n] ) ;
		}
	}

	return PSP_CfgFile_Write( path ) ;
}

///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
