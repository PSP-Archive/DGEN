///////////////////////////////////////////////////////////////////////////
/// emu_config.h
///////////////////////////////////////////////////////////////////////////
#ifndef _EMU_CONFIG_H
#define _EMU_CONFIG_H
///////////////////////////////////////////////////////////////////////////

enum{ CPU_FREQ_222, CPU_FREQ_266, CPU_FREQ_333 } ;

#define MAX_EMU_CLOCK  ( 500)
#define MIN_EMU_CLOCK  (-500)

#define MAX_Z80_CLOCK  ( 1000)
#define MIN_Z80_CLOCK  (-1000)

#define MAX_FRAME_SKIP (9)
#define MIN_FRAME_SKIP (0)

#define MAX_FRAME_SKIP_T (20)
#define MIN_FRAME_SKIP_T (0)

#define MIN_MAX_FPS  (20)
#define MAX_MAX_FPS  (98)

enum{ SKIP_MODE_A , SKIP_MODE_F, SKIP_MODE_R } ;

enum{ SHOW_FPS_NON, SHOW_FPS_LT, SHOW_FPS_RT, SHOW_FPS_LB, SHOW_FPS_RB } ;

enum{ SCREEN_1X, SCREEN_FIT, SCREEN_FULL, SCREEN_FIT43 } ;

enum{ VOL_08, VOL_10, VOL_12, VOL_14, VOL_16 } ;

enum{ PALETTE_NORMAL, PALETTE_H_CNT, PALETTE_H_BLT } ;

enum{ COMM_NON, COMM_1P, COMM_2P } ;

#define SHOW_BG       (0x01)
#define SHOW_VSYNC    (0x10)
#define FLAG_Z80ASYNC (0x04)
#define FLAG_CORE     (0x40)
#define FLAG_CHECKSUM (0x80)

enum
{ 
	// sound flags
	MD_SND_YMCH  = 0x01 ,
	MD_SND_PSG   = 0x40 ,
	MD_SND_DAC   = 0x80 ,
	MD_SND_TURBO = 0x08 ,

	// language
	MD_LANG_JPN = 0 ,
	MD_LANG_USA     ,
	MD_LANG_EUR     ,

	// sample rate
	MD_SOUND_OFF = 0 ,
	MD_SOUND_11K     ,
	MD_SOUND_22K     ,
	MD_SOUND_44K     

} ;


enum
{
	PSP_ANALOG_CFG  = 0 ,
	PSP_ANALOG_MOVE ,
	PSP_ANALOG_SWAP , // �\���L�[ �| �A�i���O�L�[ �؂�ւ�

	PAD_OPT_6PAD    = 0x01 ,

	MD_PAD_NONE     = 0 ,

	MD_PAD_A        ,
	MD_PAD_B        ,
	MD_PAD_C        ,
	MD_PAD_X        ,
	MD_PAD_Y        ,
	MD_PAD_Z        ,
	MD_PAD_MODE     ,
	MD_PAD_START    ,

	MD_PAD_RAPID_A  ,
	MD_PAD_RAPID_B  ,
	MD_PAD_RAPID_C  ,
	MD_PAD_RAPID_X  ,
	MD_PAD_RAPID_Y  ,
	MD_PAD_RAPID_Z  ,

	MD_PAD_SKIP_D   ,
	MD_PAD_SKIP_U   ,
	MD_PAD_SCREEN   ,
	MD_PAD_FPS      ,
	MD_PAD_VSYNC    ,
	MD_PAD_SOUND_D  ,
	MD_PAD_SOUND_U  ,
	MD_PAD_SOUND_V_D,
	MD_PAD_SOUND_V_U,
	MD_PAD_STATE_SUB, // select state slot 
	MD_PAD_STATE_ADD, // select state slot 
	MD_PAD_STATE_LD , // load state
	MD_PAD_STATE_SV , // save state
	MD_PAD_STATE_LDL, // load last slot
	MD_PAD_STATE_SVL, // save slot+
	MD_PAD_CHEAT_ON , // toggle cheat on/off
	MD_PAD_CORE     , // change core
	MD_PAD_RESET    , // game reset
	MD_PAD_TURBO    , // toggle turbo on/off

	MAX_MD_PAD      ,

	MD_PAD_CFG_CIRCLE     = 0,
	MD_PAD_CFG_CROSS      ,
	MD_PAD_CFG_TRIANGLE   ,
	MD_PAD_CFG_SQUARE     ,

	MD_PAD_CFG_R_CIRCLE   ,
	MD_PAD_CFG_R_CROSS    ,
	MD_PAD_CFG_R_TRIANGLE ,
	MD_PAD_CFG_R_SQUARE   ,

	MD_PAD_CFG_SELECT     ,
	MD_PAD_CFG_START      ,
	MD_PAD_CFG_R_SELECT   ,
	MD_PAD_CFG_R_START    ,

	MD_PAD_CFG_DPAD_T     ,
	MD_PAD_CFG_DPAD_B     ,
	MD_PAD_CFG_DPAD_L     ,
	MD_PAD_CFG_DPAD_R     ,

	MD_PAD_CFG_R_DPAD_T   ,
	MD_PAD_CFG_R_DPAD_B   ,
	MD_PAD_CFG_R_DPAD_L   ,
	MD_PAD_CFG_R_DPAD_R   ,

	MD_PAD_CFG_L          ,
	MD_PAD_CFG_R          ,

	MAX_MD_PAD_CFG     
} ;

#define MD_PAD_DATA_UP    (0x00000001)
#define MD_PAD_DATA_DOWN  (0x00000002)
#define MD_PAD_DATA_LEFT  (0x00000004)
#define MD_PAD_DATA_RIGHT (0x00000008)
#define MD_PAD_DATA_A     (0x00001000)
#define MD_PAD_DATA_B     (0x00000010)
#define MD_PAD_DATA_C     (0x00000020)
#define MD_PAD_DATA_X     (0x00040000)
#define MD_PAD_DATA_Y     (0x00020000)
#define MD_PAD_DATA_Z     (0x00010000)
#define MD_PAD_DATA_MODE  (0x00080000)
#define MD_PAD_DATA_START (0x00002000)

typedef struct _EMU_CONFIG
{
	//char           emu_name[4] ; // MD,PCE,etc...

#ifdef _EMU_CLOCK_ADJ ///////////////////
	signed  short  emu_clock   ;
	signed  short  z80_clock   ;
#endif // _EMU_CLOCK_ADJ

	signed   char  comm_mode    ; // �ʐM�@�\ 0:�g�p���Ȃ� 1:�P�o 2:�Q�o
	signed   char  cpu_freq     ; // CPU ���g�� 0:222,1:266,2,333
	signed   char  country      ; // ���[�W�����R�[�h
	signed   char  autosram     ; // SRAM �����ۑ�

	signed   char  max_fps      ; // �ő� FPS
	signed   char  skip_count   ; // �t���[���X�L�b�v
	signed   char  skip_mode    ; // �t���[���X�L�b�v���[�h
	signed   char  skip_turbo   ; // �^�[�{���[�h���̃t���[���X�L�b�v

	signed   char  screen_mode  ; // �X�N���[�����[�h  0:NORMAL, 1:FIT, 2:FULL, 3:FIT43
	signed   char  show_fps     ; // FPS �\��
	unsigned char  show_flags   ; // �\���t���O
	signed   char  palette_mode ; // �p���b�g���[�h

	signed   char  sound_on     ; // �T�E���h : off/11025/22050/44100
	signed   char  sound_vol    ; // �T�E���h����
	unsigned char  sound_flag   ; // �T�E���h�t���O

	signed   char  analog_mode  ; // �A�i���O�p�b�h���[�h
	signed   char  pad_option   ; // 
	signed   char  pad_cfg[MAX_MD_PAD_CFG] ; // �L�[�R���t�B�O

} EMU_CONFIG ;

void EmuCfg_InitDefault    ( char* path ) ;
void EmuCfg_SaveDefault    ( char* path ) ;
void EmuCfg_GetDefault     ( EMU_CONFIG* emc ) ;
void EmuCfg_SetDefault_Std ( EMU_CONFIG* emc ) ;
void EmuCfg_SetDefault_Key ( EMU_CONFIG* emc ) ;
void EmuCfg_init           ( EMU_CONFIG* emc ) ;
void EmuCfg_CheckValue     ( EMU_CONFIG* emc ) ;
int  EmuCfg_Load           ( EMU_CONFIG* emc, char* path ) ;
int  EmuCfg_Save           ( EMU_CONFIG* emc, char* path ) ;

////////////////////////////////////////////////////////////////////

enum
{
	KEY_MENU_L   , // L 
	KEY_MENU_LST , // L + START
	KEY_MENU_LSE , // L + SELECT
	KEY_MENU_R   , // R 
	KEY_MENU_RST , // R + START
	KEY_MENU_RSE , // R + SELECT
	KEY_MENU_ST  , // START
	KEY_MENU_SE  , // SELECT
	MAX_KEY_MENU 
} ;

enum
{
	OPTION_SHOW_CLOCK = 0x0001 , // ���v��\��
	OPTION_SHOW_BATT1 = 0x0002 , // �o�b�e������\���i�P�j
	OPTION_SHOW_BATT2 = 0x0004 , // �o�b�e������\���i�Q�j

	OPTION_LOWBATT    = 0x0010 , // ���[�o�b�e�����ɋ����X���[�v
	OPTION_SHOW_BATT  = 0x0020 , // �Q�[�����Ƀo�b�e������\��

	OPTION_SHOW_DATE  = 0x0100 , // �X�e�[�g�Z�[�u�̓��t�\��
	OPTION_STATE_LOAD = 0x0200 , // �X�e�[�g���[�h���Ɋm�F�_�C�A���O
	OPTION_STATE_SAVE = 0x0400 , // �X�e�[�g�Z�[�u���Ɋm�F�_�C�A���O
} ;

enum
{
	KEY_REPEAT_FAST = 1 ,
	KEY_REPEAT_MID  = 2 ,
	KEY_REPEAT_SLOW = 3  
} ;

enum
{ 
	BG_MODE_ROLL      ,
	BG_MODE_NORMAL    , 
	BG_MODE_GRADATION ,
	BG_MODE_IMAGE     ,
	MAX_BG_MODE
} ;

typedef struct _EMU_SYS_CONFIG
{
	char*          rom_path     ; // �����t�H���_

	signed char    key_menu     ; // ���j���[�\���L�[
	signed char    key_repeat   ; // �L�[���s�[�g���x

	unsigned long  option_flags ; // 

	signed char    shadow_on    ; // ���j���[�e�L�X�g�̉e

	signed char    bg_mode      ; // �w�i�\�����[�h

	unsigned short color_bg     ; // Simple Mode    ���̐F
	         short color_grad   ; // Gradation Mode ���̐F

	unsigned short color_title  ; // �^�C�g���̐F
	unsigned short color_select ; // �I�����̐F
	unsigned short color_msgbox ; // ���b�Z�[�W�{�b�N�X�̕����F
	unsigned short color_msg    ; // ���b�Z�[�W�̐F
	unsigned short color_text   ; // �ʏ�e�L�X�g�̐F
	unsigned short color_shadow ; // �e�̐F

} EMU_SYS_CONFIG ;

int             EmuSysCfg_Init( char* path ) ;
int             EmuSysCfg_Save( char* path ) ;
EMU_SYS_CONFIG* EmuSysCfg_Get () ;
void            EmuSysCfg_CheckValue () ;

///////////////////////////////////////////////////////////////////////////
#endif //_EMU_CONFIG
///////////////////////////////////////////////////////////////////////////
