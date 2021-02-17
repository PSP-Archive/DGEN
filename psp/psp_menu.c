///////////////////////////////////////////////////////////////////////////
/// psp_menu.c
///////////////////////////////////////////////////////////////////////////
#include <string.h>
#include "psp_main.h"
#include "psp_screen.h"
#include "psp_menu.h"
#include "emu_core.h"
#include "psp_bitmap.h"

///////////////////////////////////////////////////////////////////////////
#ifdef _NON_KERNEL ///////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////

#define PSP_Comm_IsConnect()  (0)
#define PSP_Comm_Term() 

#else /////////////////////////////////////////////////////////////////////

#include "psp_comm.h"

///////////////////////////////////////////////////////////////////////////
#endif //_NON_KERNEL //////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////

//#define _EMU_CLOCK_ADJ

///////////////////////////////////////////////////////////////////////////
#define MENU_FRAME_SPACE  (8)
#define MENU_TEXT_SPACE   (10)
#define MAX_FILE_ENTRY    (1024)
#define MAX_ROM_PAGE      (19)
#define MISC_TITLE_TOP    (1)
#define MISC_TITLE_LR     (30)
#define MISC_TITLE_CX     (70)
#define MISC_TITLE_CY     (14)
#define MISC_TITLE_NUM    (6)
#define MISC_BATT_TOP     (MISC_TITLE_TOP + MISC_TITLE_CY*2)
#define MISC_BATT_CX      (10)
#define MISC_BATT_CY      (18)
#define MISC_BATT2_TOP    (MISC_BATT_TOP + MISC_BATT_CY)
#define MISC_BATT2_CX     (10)
#define MISC_BATT2_CY     (8)
#define MISC_CLOCK_TOP    (MISC_BATT2_TOP + MISC_BATT2_CY)
#define MISC_CLOCK_CX     (10)
#define MISC_CLOCK_CY     (8)
#define MISC_ICON_TOP     (MISC_CLOCK_TOP + MISC_CLOCK_CY)
#define MISC_ICON_CX      (10)
#define MISC_ICON_CY      (8)
#define MISC_CURSOR_TOP   (MISC_ICON_TOP + MISC_ICON_CY*3)
#define MISC_CURSOR_CX    (10)
#define MISC_CURSOR_CY    (8)
#define MISC_MSGBOX_CX    (300)
#define MISC_MSGBOX_CY    (100)
#define MISC_MSGBOX_LEFT  (PSP_SCREEN_WIDTH - MISC_MSGBOX_CX)
#define MISC_MSGBOX1_TOP  (MISC_TITLE_TOP + MISC_TITLE_CY*2)
#define MISC_MSGBOX2_TOP  (MISC_MSGBOX1_TOP + MISC_MSGBOX_CY)

#define MISC_TR_BG        (0)
#define MISC_TR_TITLE     (1)
#define MISC_TR_BATT      (2)
#define MISC_TR_BATT2     (3)
#define MISC_TR_CLOCK     (4)
#define MISC_TR_ICON      (5)
#define MISC_TR_CURSOR    (6)
#define MISC_TR_MSGBOX1   (7)
#define MISC_TR_MSGBOX2   (8)

#define NAME_IMAGE_BG     "BG.PNG"
#define NAME_IMAGE_MENU   "MENU.PNG"
#define NAME_IMAGE_MISC   "SKIN.PNG"
#define COLOR_TITLE       emsc->color_title   //PSP_RGB(  0,255,255)
#define COLOR_MSGBOX      emsc->color_msgbox  //PSP_RGB(192,255,192)
#define COLOR_MSG         emsc->color_msg     //PSP_RGB(192,255,192)
#define COLOR_SEL         emsc->color_select  //PSP_RGB(255,255,128)
#define COLOR_NORMAL      emsc->color_text    //PSP_RGB(192,192,192)
#define COLOR_SHADOW      emsc->color_shadow  //PSP_RGB( 96, 96, 96)
#define MENU_ITEM_LEFT    (MENU_FRAME_SPACE * 4)
#define MENU_ITEM_TOP     (MENU_FRAME_SPACE * 4 + 4)
#define MENU_MESSAGE_LEFT (MENU_FRAME_SPACE*2+5)
#define MENU_MESSAGE_TOP  (PSP_SCREEN_HEIGHT - (MENU_FRAME_SPACE*3+1))
#define MAX_SHOW_CHEAT    (10)

enum
{ 
	MENU_MODE_MD_LOAD    ,
	MENU_MODE_MD_STATE   ,
	MENU_MODE_MD_CHEAT   ,
	MENU_MODE_MD_CONFIG  ,
	MENU_MODE_MD_KEY     ,
	MENU_MODE_MD_SYSTEM  ,
	MAX_MENU_MODE        
};

enum
{
	MENU_CUR_CHEAT_ON                  ,
	MENU_CUR_CHEAT_MIN                 , 
	MENU_CUR_CHEAT_MAX = MAX_CHEAT_NUM , 
	MAX_MENU_CUR_CHEAT 
} ;

enum //for CONFIG
{
	MENU_CUR_CPU        ,

#ifdef _EMU_CLOCK_ADJ
	MENU_CUR_EMU_CLOCK  ,
	MENU_CUR_Z80_CLOCK  ,
#endif // _EMU_CLOCK_ADJ

	MENU_CUR_CORE       ,
	MENU_CUR_Z80ASYNC   ,
	MENU_CUR_AUTOSRAM   ,
	MENU_CUR_COUNTRY    ,
	MENU_CUR_CHECKSUM   ,
	MENU_CUR_2CON       ,
	MENU_CUR_MAX_FPS    ,
	MENU_CUR_SKIP       ,
	MENU_CUR_SKIP_MODE  ,
	MENU_CUR_SKIP_T     ,
	MENU_CUR_SCREEN     ,
	MENU_CUR_SHOW_FPS   ,
	MENU_CUR_SHOW_BG    ,
	MENU_CUR_VSYNC      ,
	MENU_CUR_PALETTE    ,
	MENU_CUR_SOUND_ON   ,
	MENU_CUR_SOUND_VOL  ,
	MENU_CUR_SOUND_YMCH ,
	MENU_CUR_SOUND_PSG  ,
	MENU_CUR_SOUND_DAC  ,
	MENU_CUR_SOUND_TB   ,
	MAX_MENU_CUR_CONFIG 
};

enum // for Key Config
{
	MENU_CUR_6PAD       ,

	MENU_CUR_CIRCLE     ,
	MENU_CUR_CROSS      ,
	MENU_CUR_TRIANGLE   ,
	MENU_CUR_SQUARE     ,

	MENU_CUR_R_CIRCLE   ,
	MENU_CUR_R_CROSS    ,
	MENU_CUR_R_TRIANGLE ,
	MENU_CUR_R_SQUARE   ,

	MENU_CUR_SELECT     ,
	MENU_CUR_START      ,
	MENU_CUR_R_SELECT   ,
	MENU_CUR_R_START    ,

	MENU_CUR_DPAD_T     ,
	MENU_CUR_DPAD_B     ,
	MENU_CUR_DPAD_L     ,
	MENU_CUR_DPAD_R     ,

	MENU_CUR_R_DPAD_T   ,
	MENU_CUR_R_DPAD_B   ,
	MENU_CUR_R_DPAD_L   ,
	MENU_CUR_R_DPAD_R   ,

	MENU_CUR_L          ,
	MENU_CUR_R          ,

	MENU_CUR_SWAP_ANALOG,

	MAX_MENU_CUR_KEY
} ;

enum // state
{
	MENU_CUR_STATE_ROM   ,
	MENU_CUR_STATE_SLOT0 ,
	MENU_CUR_STATE_SLOT1 ,
	MENU_CUR_STATE_SLOT2 ,
	MENU_CUR_STATE_SLOT3 ,
	MENU_CUR_STATE_SLOT4 ,
	MENU_CUR_STATE_SLOT5 ,
	MENU_CUR_STATE_SLOT6 ,
	MENU_CUR_STATE_SLOT7 ,
	MENU_CUR_STATE_SLOT8 ,
	MENU_CUR_STATE_SLOT9 ,
	MENU_CUR_STATE_MODE1 ,
	MENU_CUR_STATE_MODE2 ,
	MENU_CUR_STATE_LOOP  ,
	MENU_CUR_STATE_INC   ,
	MENU_CUR_STATE_SEL   ,
	MAX_MENU_CUR_STATE   
} ;

enum // system
{
	MENU_CUR_SYS_KEY_MENU ,
	MENU_CUR_SYS_KEY_R    ,
	MENU_CUR_SYS_CLOCK    ,
	MENU_CUR_SYS_BATT1    ,
	MENU_CUR_SYS_BATT2    ,
	MENU_CUR_SYS_STATE_D  ,
	MENU_CUR_SYS_STATE_L  ,
	MENU_CUR_SYS_STATE_S  ,
	MENU_CUR_SYS_SHADOW   ,
	MENU_CUR_SYS_BGMODE   ,
	MENU_CUR_SYS_BG_R     ,
	MENU_CUR_SYS_BG_G     ,
	MENU_CUR_SYS_BG_B     ,
	MENU_CUR_SYS_GRAD     ,
	MENU_CUR_SYS_TITLE_R  ,
	MENU_CUR_SYS_TITLE_G  ,
	MENU_CUR_SYS_TITLE_B  ,
	MENU_CUR_SYS_SEL_R    ,
	MENU_CUR_SYS_SEL_G    ,
	MENU_CUR_SYS_SEL_B    ,
	MENU_CUR_SYS_MSGBOX_R ,
	MENU_CUR_SYS_MSGBOX_G ,
	MENU_CUR_SYS_MSGBOX_B ,
	MENU_CUR_SYS_MSG_R    ,
	MENU_CUR_SYS_MSG_G    ,
	MENU_CUR_SYS_MSG_B    ,
	MENU_CUR_SYS_TEXT_R   ,
	MENU_CUR_SYS_TEXT_G   ,
	MENU_CUR_SYS_TEXT_B   ,
	MENU_CUR_SYS_SHADOW_R ,
	MENU_CUR_SYS_SHADOW_G ,
	MENU_CUR_SYS_SHADOW_B ,
	MAX_MENU_CUR_SYS      
} ;

/*
const char SZ_MAIN_TITLE[] = "DGEN for PSP" ;

const char* SZ_MENU_TITLE[] =
{
	"[ ROM List ]"  ,
	"[State List]"  ,
	"[ Settings ]"  ,
	"[key Config]"  ,
	"[  System  ]"  ,
} ;
*/

const char* SZ_MENU_MESSAGE_ROM[] = {
	"Åõ:Load ROM  Å~:Return to game  Å®:Select State"               ,
	"Åõ:Load ROM  Å~:Return to game  Å®:Select State  Å¢:Save SRAM" ,
	"Åõ:Select Folder  Å~:Return to game  Å©Å®:Select Folder"       
} ;

const char* SZ_MENU_MESSAGE_STATE[] =
{
	"Å©Å®:Select ROM  Å~:Return to game"                           ,
	"Åõ:Load State  Å~:Return to game  Å¢:Save State  Å†Flush MS"  ,
	"Åõ:Load ROM and State  Å~:Return to game"                     ,
	"Å©Å®:Change value  Å~:Return to game  Å¢:Save Default"        ,
	"Å†:Delete State  Å~:Return to game"                     
};

const char* SZ_MENU_MESSAGE_CHEAT[] = {
	"Å©Å®:Change value  Å~:Return to game" ,
	"Å©Å®:ON/OFF  Å~:Return to game  Åõ:Input Patch code  Å¢:Input GG code" ,
	"Å†:Input Cheat Description"
} ;

const char SZ_MENU_MESSAGE_CFG[] =
	"Å©Å®:Change value  Å~:Return to game  Å¢:Save Default" ;

const char SZ_MENU_MESSAGE_KEY[] =
	"Å©Å®:Change value  Å~:Return to game  Å¢:Save Default" ;

const char SZ_MENU_MESSAGE_SYS[] =
	"Å©Å®:Change value  Å~:Return to game" ;

const char SZ_MSGBOX_OK_CANCEL[] = "ÅõÅFOK    Å~ÅFCancel" ;
const char SZ_MSGBOX_CHEAT    [] = "Å©Å®ÅFUnit  Å™Å´ÅFValue   ÅõÅFOK  Å~ÅFCancel" ;
const char SZ_MSGBOX_CHEAT_PT [] = "Input patch code" ;
const char SZ_MSGBOX_CHEAT_GG [] = "Input game genie code" ;

char* SZ_MESSAGE_SAVEDEFAULT  [] = { "åªç›ÇÃê›íËÇÉfÉtÉHÉãÉgÇ∆ÇµÇƒï€ë∂ÇµÇ‹Ç∑ÅB"  , "( This settings are saved to default. )" } ;
char* SZ_MESSAGE_SAVESRAM     [] = { "SRAM ÇÃì‡óeÇÉÅÉÇÉäÉJÅ[ÉhÇ…èëÇ´èoÇµÇ‹Ç∑ÅB" , "( The content of SRAM is written to the memory card. )" } ; 
char* SZ_MESSAGE_SAVESTATE    [] = { "SLOT0 Ç…ï€ë∂ÇµÇ‹Ç∑ÅB"                      , "( Current state is saved in the SLOT. )" } ;
char* SZ_MESSAGE_SAVESTATE_MSD[] = { "SLOT0 ÇÉÅÉÇÉäÉJÅ[ÉhÇ…èëÇ´èoÇµÇ‹Ç∑ÅB"      , "( The SLOT is written to the memory card. )" } ;
char* SZ_MESSAGE_DELETESTATE  [] = { "SLOT0 ÇçÌèúÇµÇ‹Ç∑ÅB"                      , "( The SLOT is deleted. )" } ;
char* SZ_MESSAGE_LOADSTATE    [] = { "SLOT0 ÇÉçÅ[ÉhÇµÇ‹Ç∑ÅB"                    , "( The SLOT is loaded. )" } ;
char* SZ_MESSAGE_ON_SAVE      [] = { "ÉÅÉÇÉäÉJÅ[ÉhÇ…ï€ë∂íÜÇ≈Ç∑ÅB", "ÇµÇŒÇÁÇ≠Ç®ë“Çøâ∫Ç≥Ç¢ÅB" , "( Now saving to memory card..., Please wait. )" } ;
char* SZ_MESSAGE_CHEAT_ERROR  [] = { "ì¸óÕílÇ™ê≥ÇµÇ≠Ç†ÇËÇ‹ÇπÇÒÅBì¸óÕÇµíºÇµÇ‹Ç∑Ç©ÅH", "( The input value is not correct.", "         Do you try to input it? )" } ;
char* SZ_MESSAGE_LOADING      [] = { "Now Loading ..." } ;
char* SZ_MESSAGE_EXITGAME     [] = { "Are you sure to EXIT to PSP menu ?" } ;
char* SZ_MESSAGE_ON_EXITGAME  [] = { "Good bye ..." } ;
char* SZ_MESSAGE_DISCONNECT   [] = { "Are you sure to disconnect Adhoc ?" } ;

const char SZ_LTRIGGER[] = "[L] < " ;
const char SZ_RTRIGGER[] = " > [R]" ;

const char* SZ_CPU_FREQ[] = 
{
	"         PSP CPU Freq : 222MHz",
	"         PSP CPU Freq : 266MHz",
	"         PSP CPU Freq : 333MHz"
} ;

#ifdef _EMU_CLOCK_ADJ ///////////////////

char SZ_EMU_CLOCK[] =
{
	"      Emulation Clock : x1.000" 
} ;

char SZ_Z80_CLOCK[] =
{
	"            Z80 Clock : x1.000" 
} ;

#endif // _EMU_CLOCK_ADJ

const char* SZ_CORE[] =
{
	"   CPU Core Emulation : Normal" ,
	"   CPU Core Emulation : Performance" 
} ;

const char* SZ_Z80ASYNC[] =
{
	"        BGM Speed Fix : ON"  ,
	"        BGM Speed Fix : OFF"  
} ;

const char* SZ_AUTOSRAM[] =
{
	"       Auto SRAM Save : OFF" ,
	"       Auto SRAM Save : ON" 
} ;

const char* SZ_COUNTRY[] =
{
	"     Country priority : JPN - USA - EUR" , 
	"     Country priority : USA - EUR - JPN" ,
	"     Country priority : EUR - USA - JPN" 
} ;

const char* SZ_CHECKSUM[] =
{
	"         Fix Checksum : OFF" ,
	"         Fix Checksum : ON"  
} ;

const char* SZ_2CON[] =
{
	"            Comm mode : Disable"    , 
	"            Comm mode : 1P (adhoc)" , 
	"            Comm mode : 2P (adhoc)"  
} ;

char SZ_MAX_FPS[] = 
    "              Max FPS : 0 " ;

char SZ_SKIP[] = 
    "       Max Frame Skip : 0" ;

char SZ_SKIP_T[] = 
    "  Frame Skip on Turbo : 00" ;

const char* SZ_SKIP_MODE[] =
{
	"            Skip type : Type A" ,
	"            Skip type : Type F" ,
	"            Skip type : Type R" 
} ;

const char* SZ_SCREEN[] =
{
	"          Screen Mode : Normal" ,
	"          Screen Mode : Fit"    ,
	"          Screen Mode : Full"   ,
	"          Screen Mode : 4:3" 
} ;

const char* SZ_VSYNC[] =
{
	"                vsync : OFF",
	"                vsync : ON" 
} ;

const char* SZ_SHOW_FPS[] =
{
	"             Show FPS : OFF"        ,
	"             Show FPS : LeftTop"    ,
	"             Show FPS : RightTop"   ,
	"             Show FPS : LeftBottom" ,
	"             Show FPS : RightBottom" 
} ;

const char* SZ_SHOW_BG[] = 
{
	"Show Background Image : OFF" ,
	"Show Background Image : ON" 
} ;

const char* SZ_PALETTE_MODE[] =
{
	"         Palette Mode : Normal"           ,
	"         Palette Mode : High Contrast"    ,
	"         Palette Mode : High Brightness"   
} ;

const char* SZ_SOUND_ON[] =
{
	"           Sound Mode : OFF",
	"           Sound Mode : 11025 KHz" ,
	"           Sound Mode : 22050 KHz" ,
	"           Sound Mode : 44100 KHz" 
} ;

const char* SZ_SOUND_VOL[] =
{
	"         Sound Volume : ÅÙ"         ,
	"         Sound Volume : ÅÙÅÙ"       ,
	"         Sound Volume : ÅÙÅÙÅÙ"     ,
	"         Sound Volume : ÅÙÅÙÅÙÅÙ"   ,
	"         Sound Volume : ÅÙÅÙÅÙÅÙÅÙ"
} ;

const char* SZ_SOUND_PSG[] =
{
	"           PSG Enable : ON"  ,
	"           PSG Enable : OFF" 
} ;

const char* SZ_SOUND_DAC[] =
{
	"           DAC Enable : ON"  ,
	"           DAC Enable : OFF" 
} ;

const char* SZ_SOUND_YM[] =
{
	"        YM2612 Enable : ON"  ,
	"        YM2612 Enable : OFF" 
} ;

const char* SZ_SOUND_TURBO[] =
{
	"     Turbo Mode Sound : ON"  ,
	"     Turbo Mode Sound : OFF" 
} ;

const char* SZ_KEY_MENU[] =
{
	"          Åõ : " ,
	"          Å~ : " ,
	"          Å¢ : " ,
	"          Å† : " ,
	"     ÇqÅ{ Åõ : " ,
	"     ÇqÅ{ Å~ : " ,
	"     ÇqÅ{ Å¢ : " ,
	"     ÇqÅ{ Å† : " ,

	"      SELECT : " ,
	"       START : " ,
	" ÇqÅ{ SELECT : " ,
	" ÇqÅ{  START : " ,

	"      Analog Å™ : " ,
	"      Analog Å´ : " ,
	"      Analog Å© : " ,
	"      Analog Å® : " ,
	" ÇqÅ{ Analog Å™ : " ,
	" ÇqÅ{ Analog Å´ : " ,
	" ÇqÅ{ Analog Å© : " ,
	" ÇqÅ{ Analog Å® : " ,

	"             Çk : " ,
	"             Çq : " 
} ;

const char* SZ_SWAP_ANALOG[] =
{
	"Analog Pad Mode : Use key config" ,
	"Analog Pad Mode : Å™Å´Å©Å®"       ,
	"Analog Pad Mode : Swap D-Pad"     
} ;

const char* SZ_6PAD[] =
{
	"Use 6button pad : OFF" ,
	"Use 6button pad : ON"
} ;

const char* SZ_KEY_ITEMS[] =
{
	" " ,
	"A", "B", "C", "X", "Y", "Z", "MODE", "START",
	"A(Rapid)"       , 
	"B(Rapid)"       , 
	"C(Rapid)"       , 
	"X(Rapid)"       , 
	"Y(Rapid)"       , 
	"Z(Rapid)"       , 
	"Frame Skip -"   , 
	"Frame Skip +"   , 
	"Screen Mode"    , 
	"Show FPS"       , 
	"vsync ON/OFF"   , 
	"Sound Mode -"   , 
	"Sound Mode +"   , 
	"Sound Volume -" , 
	"Sound Volume +" ,
	"Manual State Slot-"   ,
	"Manual State Slot+"   ,
	"Manual State Load"    ,
	"Manual State Save"    ,
	"Increment State Load" ,
	"Increment State Save" ,
	"All Cheat ON/OFF"     ,
	"Change CPU Emulation" ,
	"Game Reset"           ,
	"Turbo mode ON/OFF"    ,
	" "
} ;

const char SZ_NOCONTROL[] = "Cannot be used" ;

const char SZ_NOTUSE_R_JPN[] = "ÇqÉ{É^ÉìäÑÇËìñÇƒéûÇÕ ÇqÅ{(*) É{É^ÉìÇÕégópÇ≈Ç´Ç‹ÇπÇÒÅB" ;
const char SZ_NOTUSE_R_ENG[] = "( When R button is allocated, R+(*) button cannot be used. )" ;

const char* SZ_STATE_MODE1 [] =
{
	"         State Slot 0-4 : On Memory and MS" ,
	"         State Slot 0-4 : On MS"            ,
	"         State Slot 0-4 : On Memory Only"    
} ;

const char* SZ_STATE_MODE2 [] =
{
	"         State Slot 5-9 : On Memory and MS" ,
	"         State Slot 5-9 : On MS"            ,
	"         State Slot 5-9 : On Memory Only"    
} ;

const char* SZ_STATE_LOOP [] =
{
	"   Increment State Save : Slot 0-9" ,
	"   Increment State Save : Slot 0-4" ,
	"   Increment State Save : Slot 5-9" 
} ;

char SZ_STATE_SLOT_I [] =
	"Current Increment State : Slot 0" ;

char SZ_STATE_SLOT_M [] =
	"      Manual State Save : Slot 0" ;

char SZ_STATE_SLOT[] = "*SLOT0" ;


const char* SZ_MENU_KEYS[] =
{
	"Çk"         ,
	"ÇkÅ{START"  ,
	"ÇkÅ{SELECT" ,
	"Çq"         ,
	"ÇqÅ{START"  ,
	"ÇqÅ{SELECT" ,
	"START"      ,
	"SELECT"      
} ;

char SZ_MENU_KEY[] = 
	"Menu display key  : " ;

const char* SZ_KEY_REPEAT [] =
{
	"Key repeat speed  : FAST" ,
	"Key repeat speed  : MID"  ,
	"Key repeat speed  : SLOW" 
} ;

const char* SZ_SHOW_CLOCK[] =
{
	"Show clock        : OFF" ,
	"Show clock        : ON"  
} ;

const char* SZ_SHOW_BATT1[] =
{
	"Show battery life : OFF" ,
	"Show battery life : ON" 
} ;

const char* SZ_SHOW_BATT2[] =
{
	"Show battery temp : OFF" ,
	"Show battery temp : ON" 
} ;

const char* SZ_SHOW_STATE_DATE[] =
{
	"Show state date   : OFF" ,
	"Show state date   : ON"  
} ;

const char* SZ_SHOW_STATE_SAVE[] =
{
	"Show state saving message  : OFF" ,
	"Show state saving message  : ON"  
} ;

const char* SZ_SHOW_STATE_LOAD[] =
{
	"Show state loading message : OFF" ,
	"Show state loading message : ON"  
} ;

const char* SZ_SHADOW_ON[] =
{
	"Shadow color      : OFF" ,
	"Shadow color      : ON"  
} ;

const char* SZ_BG_MODE[] =
{
	"Background mode   : Color scroll" ,
	"Background mode   : Simple"       ,
	"Background mode   : Gradation"    ,
	"Background mode   : Image only"   
} ;

char SZ_COLOR_GRAD[] = 
	"    Gradation mode color : " ;

char SZ_COLOR_SIMPLE[] = "    Simple mode" ;

char SZ_COLOR_TITLE [] = "      Title" ; 
char SZ_COLOR_SELECT[] = "   Selected" ; 
char SZ_COLOR_MSGBOX[] = " MessageBox" ; 
char SZ_COLOR_MSG   [] = "    Message" ; 
char SZ_COLOR_NORMAL[] = "     Normal" ; 
char SZ_COLOR_SHADOW[] = "     Shadow" ; 

const char SZ_CFG_FOLDER  [] = "CFG/"   ;
const char SZ_CHEAT_FOLDER[] = "CHEAT/" ;
const char SZ_STATE_FOLDER[] = "STATE/" ;
const char SZ_SRAM_FOLDER [] = "SAVE/"  ;
const char SZ_CFG_EXT     [] = "INI"    ;
const char SZ_CHEAT_EXT   [] = "CHT"    ;
const char SZ_SRAM_EXT    [] = "SRM"    ;

char SZ_DATE       [] = "00/00 00:00" ;
char SZ_BATT_LIFE_P[] = "000%"        ;
char SZ_BATT_LIFE_C[] = "(Chrg.)"     ;
char SZ_BATT_LIFE_A[] = "(AC)"        ;
char SZ_BATT_LIFE_X[] = "(--:--)"     ;
char SZ_BATT_LIFE_T[] = "(00:00)"     ;
char SZ_BATT_CHARG [] = "(00:00)"     ;
char SZ_BATT_TEMP  [] = " 00Åé"       ;

const char* SZ_CHEAT_ON[] = 
{
	"All Cheat Enable : OFF" ,
	"All Cheat Enable : ON" 
} ;
   
const char* SZ_TURBO_ON[] = 
{
	"Turbo mode : OFF" ,
	"Turbo mode : ON" 
} ;

                    //0         1         2
                       //0123456789012345678901234567890
char SZ_CHEAT_HEAD [] = "  No  Address  Data  GG Code    Description" ;

char* SZ_CHEAT_FLAG[] = { "Å†", "Å°" };

//                      0         1         2         3
//                      0123456789012345678901234567890
char SZ_CHEAT_PT  [] = "Address - Data : 000000 - 0000" ;
char SZ_CHEAT_GG  [] = "Game Genie Code : 0000 - 0000"  ;
char SZ_CHEAT_NAME[] = "Input Cheat Description"        ;
char SZ_CHEAT_GG_DEFAULT[] = "0000-0000" ;
char SZ_CHEAT_NAME_T    [] = "--------------------------------" ;

///////////////////////////////////////////////////////////////////////////
// 
typedef struct _MENU_FILE_ITEM
{
    unsigned long type           ;
	int           ext_id         ;
    char          name[MAX_PATH] ;

} MENU_FILE_ITEM ;

///////////////////////////////////////////////////////////////////////////
// member

int menu_mode = MENU_MODE_MD_LOAD ;
int menu_cmd  = MENU_CMD_NONE ;
int menu_cursol    [ MAX_MENU_MODE ] ;
int menu_cursol_max[ MAX_MENU_MODE ] ;
int menu_key_repeat      = 0 ;
int menu_key_repeat_mode = 0 ;

int menu_files_sel   = -1 ;
int menu_files_sel_s = 0  ;
int menu_files_num   = 0  ;
MENU_FILE_ITEM menu_files [ MAX_FILE_ENTRY ] ;
EMU_STATE menu_emu_state ;
int       is_menu_emu_state_load = 0 ;

char path_exec   [MAX_PATH] ;
char path_last   [MAX_EMU][MAX_PATH] ;
char path_rom    [MAX_PATH] ;
char path_cfg    [MAX_PATH] ;
char path_state  [MAX_PATH] ;
char path_state_s[MAX_PATH] ;
char path_sram   [MAX_PATH] ;
char path_cheat  [MAX_PATH] ;
int  menu_load_state_slot = 0 ;

unsigned long menu_cheat_addr ;
unsigned long menu_cheat_data ;
	     char menu_cheat_gg   [ MAX_CHEAT_GG+1   ] ;
	     char menu_cheat_name [ MAX_CHEAT_NAME+1 ] ;

PSP_BITMAP bmpBG   ;
PSP_BITMAP bmpMENU ;
PSP_BITMAP bmpMISC ;

EMU_CONFIG*     emc  = 0 ;
EMU_SYS_CONFIG* emsc = 0 ;
EMU_CHEAT*      emct = 0 ;

///////////////////////////////////////////////////////////////////////////
// functions
/*
PSP_BITMAP bmpCAP  ;
char path_cap[128] ;
int  capfile ;

void make_cap()
{
	int y ;

	for( y=0; y < PSP_SCREEN_HEIGHT ; y++ )
	{
		memcpy( bmpCAP.pBuff + PSP_SCREEN_WIDTH * 2 * y, PSP_Screen_GetBackBuffer(0,y), PSP_SCREEN_WIDTH*2 ) ;
	}

	strcpy( path_cap, path_exec ) ;
	strcat( path_cap, "cap.dat" ) ;
	capfile = sceIoOpen( path_cap, 0x0200 | 0x0400 | 0x0002, 0644 ) ;
	if( capfile )
	{
		make_cap() ;

		sceIoWrite( capfile, bmpCAP.pBuff, MAX_PSP_BMP_WIDTH * 2 * MAX_PSP_BMP_HEIGHT ) ;
		sceIoClose( capfile ) ;
	}
}
*/

void make_cfg_path( char* name )
{
	char* p ;

	strcpy( path_cfg, path_exec     ) ;
	strcat( path_cfg, SZ_CFG_FOLDER ) ;
	strcat( path_cfg, name          ) ;
	p = strrchr( path_cfg, '.' ) ;
	if( p )
	{ 
		p++ ;
		*(p++) = SZ_CFG_EXT[0] ;
		*(p++) = SZ_CFG_EXT[1] ;
		*(p++) = SZ_CFG_EXT[2] ;
		*(p) = 0   ;
	}
}

void make_cheat_path( char* dst, char* name )
{
	char* p ;
	strcpy( dst, path_exec       ) ;
	strcat( dst, SZ_CHEAT_FOLDER ) ;
	strcat( dst, name            ) ;
	p = strrchr( dst, '.'  ) ;
	if( p )
	{ 
		p++ ;
		*(p++) = SZ_CHEAT_EXT[0] ;
		*(p++) = SZ_CHEAT_EXT[1] ;
		*(p++) = SZ_CHEAT_EXT[2] ;
		*p = 0 ;
	}
}

void make_state_path( char* dst, char* name )
{
	char* p ;
	strcpy( dst, path_exec       ) ;
	strcat( dst, SZ_STATE_FOLDER ) ;
	strcat( dst, name            ) ;
	p = strrchr( dst, '.'  ) ;
	*p = 0 ;
}

void make_sram_path( char* dst, char* name )
{
	char* p ;
	strcpy( dst, path_exec       ) ;
	strcat( dst, SZ_SRAM_FOLDER  ) ;
	strcat( dst, name            ) ;
	p = strrchr( dst, '.'  ) ;
	if( p )
	{ 
		p++ ;
		*(p++) = SZ_SRAM_EXT[0] ;
		*(p++) = SZ_SRAM_EXT[1] ;
		*(p++) = SZ_SRAM_EXT[2] ;
		*p = 0 ;
	}
}

void calc_color( unsigned short* color, int rgb, int add )
{
	int sh  = rgb * 5 ;
	int col = ( (*color >> sh) & 0x1F ) ;
	col += add ;
	if( col < 0 ){ col = 0x1F ; }else if( col > 0x1F ){ col = 0 ; }

	*color = (*color & ~(0x1F << sh)) | (col << sh) ;
}

void calc_unit( unsigned long* val, int unit, int add )
{
	int n = ((*val >> unit) & 0x0F);
	n += add ;
	if( n < 0 ){ n = 0x0F ; }else if( n > 0x0F ){ n = 0 ; }

	*val= (*val & ~(0x0F << unit)) | (n << unit) ;
}

void calc_char( char* ch, int add, int sp )
{
	*ch += add ;

	if( sp )
	{
		if( add > 0 )
		{
			if( *ch > 'Z' ){ *ch = ' ' ; }
			else if( (*ch > ' ') && (*ch < '0') ){ *ch = '0' ; }
			else if( (*ch > '9') && (*ch < 'A') ){ *ch = 'A' ; }
			else if( *ch < ' ' ){ *ch = ' ' ; }
		}
		else
		{
			if( *ch < ' ' ){ *ch = 'Z' ; }
			else if( (*ch > ' ') && (*ch < '0') ){ *ch = ' ' ; }
			else if( (*ch > '9') && (*ch < 'A') ){ *ch = '9' ; }
			else if( *ch > 'Z' ){ *ch = 'Z' ; }
		}
	}
	else
	{
		if( add > 0 )
		{
			if( *ch > 'Z' ){ *ch = '0' ; }else if( (*ch > '9') && (*ch < 'A') ){ *ch = 'A' ; }
		}
		else
		{
			if( *ch < '0' ){ *ch = 'Z' ; }else if( (*ch > '9') && (*ch < 'A') ){ *ch = '9' ; }
		}
	}
}



////////////////////////////////////////////////////////////////////////
// ÉNÉCÉbÉNÉ\Å[Ég  by RIN. thanks for MIRAKICHI. and othors.
// AC add start
void SJISCopy(MENU_FILE_ITEM *a, unsigned char *file)
{
	unsigned char ca;
	int i;

	for(i=0;i<=strlen(a->name);i++)
	{
		ca = a->name[i];
		if( ((0x81 <= ca)&&(ca <= 0x9f)) || ((0xe0 <= ca)&&(ca <= 0xef)) )
		{
			file[i++] = ca ;
			file[i  ] = a->name[i];
		}
		else
		{
			if(ca>='a' && ca<='z') ca -= 0x20 ;
			file[i] = ca;
		}
	}
}

int cmpFile(MENU_FILE_ITEM *a, MENU_FILE_ITEM *b)
{
    unsigned char file1[0x108];
    unsigned char file2[0x108];
	unsigned char ca, cb;
	int i, n, ret;

	if( (a->type & 0x30) == (b->type & 0x30) )
	{
		SJISCopy(a, file1);
		SJISCopy(b, file2);
		n = strlen(file1);
		for(i=0; i<=n; i++)
		{
			ca = file1[i] ; 
			cb = file2[i] ;
			ret = ca - cb ;
			if( ret != 0 ) return ret;
		}
		return 0;
	}
	
	if( a->type & PSP_FILE_TYPE_DIR ) return -1 ;
	else					          return  1 ;
}
// AC add end

/*
void sort(PSP_FILE_ATTRIBUTE *a, int left, int right) 
{
	PSP_FILE_ATTRIBUTE tmp, pivot;
	int i, p;
	
	if (left < right) 
	{
		pivot = a[left] ;
		p     = left    ;
		for (i=left+1; i<=right; i++) 
		{
			if( cmpFile( &a[i], &pivot ) < 0 )
			{
				p    = p+1  ;
				tmp  = a[p] ;
				a[p] = a[i] ;
				a[i] = tmp  ;
			}
		}
		a[left] = a[p];
		a[p   ] = pivot;
		sort(a, left, p-1  );
		sort(a, p+1 , right);
	}
}
*/

void sort_b( MENU_FILE_ITEM *list, int left, int right )
{
	MENU_FILE_ITEM tmp ;
	int n, m ;

	for( n=left ; n < right -1; n++ )
	{
		for( m=n+1; m < right; m++ )
		{
			if( cmpFile( &list[n], &list[m] ) > 0 )
			{
				tmp     = list[n] ;
				list[n] = list[m] ;
				list[m] = tmp     ;
			}
		}
	}
}

enum{ EXT_UNKNOWN, EXT_DIR, EXT_ZIP, EXT_SMD } ;

// ägí£éqä«óùóp
const struct { char *szExt; int nExtId ; } stExtentions[] = 
{
	// "gb" , EXT_GB  ,
	// "gbc", EXT_GB  ,
	// "pce", EXT_PCE ,
	"zip", EXT_ZIP ,
	"gz" , EXT_ZIP ,
	"tgz", EXT_ZIP ,
	"bin", EXT_SMD ,
	"smd", EXT_SMD ,
	NULL , EXT_UNKNOWN
};

int getExtId(const char *szFilePath) 
{
	char *pszExt;
	int i;
	if((pszExt = strrchr(szFilePath, '.'))) 
	{
		pszExt++;
		for (i = 0; stExtentions[i].nExtId != EXT_UNKNOWN; i++) 
		{
			if (!stricmp(stExtentions[i].szExt,pszExt)) 
			{
				return stExtentions[i].nExtId;
			}
		}
	}
	return EXT_UNKNOWN;
}

PSP_FILE_ATTRIBUTE attr ;

void Menu_UpdateFileList( const char* path )
{
	int fd, b=0, ext_id ;
	char *p;
	
	menu_files_num = 0;

	if( strcmp(path,"ms0:/") != 0 )
	{
		strcpy( menu_files[menu_files_num].name, "..");
		menu_files[menu_files_num].type   = PSP_FILE_TYPE_DIR ;
		menu_files[menu_files_num].ext_id = EXT_DIR           ;
		menu_files_num++;
		b = 1 ;
	}

	fd = sceIoDopen( path );

	while( menu_files_num < MAX_FILE_ENTRY )
	{
		if( sceIoDread(fd, &attr) <=0 ) break;
		if( attr.name[0] == '.') continue;
		if( attr.type & PSP_FILE_TYPE_DIR )
		{
			strcpy( menu_files[menu_files_num].name, attr.name ) ; 
			strcat( menu_files[menu_files_num].name, "/");
			menu_files[menu_files_num].type   = attr.type ;
			menu_files[menu_files_num].ext_id = EXT_DIR   ;
			menu_files_num++ ;
			continue;
		}

		ext_id = getExtId( attr.name ) ;

		if( ext_id != EXT_UNKNOWN )
		{
			strcpy( menu_files[menu_files_num].name, attr.name ) ; 
			menu_files[menu_files_num].type   = attr.type ;
			menu_files[menu_files_num].ext_id = ext_id    ;
			menu_files_num++ ;
		}
	}
	sceIoDclose( fd );

	if(b){ sort_b( menu_files, 1, menu_files_num ) ; }
	else { sort_b( menu_files, 0, menu_files_num ) ; }

	if( menu_cursol[ MENU_MODE_MD_LOAD ] >= menu_files_num )
	{
		menu_cursol[ MENU_MODE_MD_LOAD ] = menu_files_num -1 ;
	}
	menu_cursol_max[ menu_mode ] = menu_files_num ;
}

////////////////////////////////////////////////////////////////////////

void DrawMenuText ( short x, short y, const char* p, short color, short shadow, int shadow_on )
{
	if( shadow_on ){ PSP_Screen_DrawText ( x+1, y+1, p, shadow ) ; }
	PSP_Screen_DrawText ( x, y, p, color ) ;
}

int DrawMenu( int x, int y, const char* p, int n, int sel )
{
	static unsigned short tp = 0 ;

	if( emsc->shadow_on ){ PSP_Screen_DrawText( x+1, y+1, p, COLOR_SHADOW ) ; }

	if( n == sel )
	{
		PSP_Screen_DrawText( x, y, p, COLOR_SEL ) ; 
//		if( emsc->shadow_on ){ PSP_Screen_DrawText( x-MENU_TEXT_SPACE  , y+1, ">", COLOR_SHADOW ) ; }
//		PSP_Screen_DrawText( x-MENU_TEXT_SPACE-1, y  , ">", COLOR_SEL    ) ; 

		tp++ ;
		PSP_Bitmap_BltEx( &bmpMISC, x-MISC_CURSOR_CX-3, y+1, MISC_CURSOR_CX, MISC_CURSOR_CY, 0, 
			MISC_CURSOR_TOP + ((tp/4)&7) * MISC_CURSOR_CY , PSP_Bitmap_GetBitColor( &bmpMISC, MISC_TR_CURSOR, 0 ) ) ;
	}
	else
	{
		PSP_Screen_DrawText( x, y, p, COLOR_NORMAL ) ; 
	}
	
	return y + MENU_TEXT_SPACE ;
}

int DrawMenu2( int x, int y, const char* p, int n, int sel )
{
	static unsigned short tp = 0 ;

	if( emsc->shadow_on ){ PSP_Screen_DrawText( x+1, y+1, p, COLOR_SHADOW ) ; }

	if( n == sel )
	{
		PSP_Screen_DrawText( x, y, p, COLOR_SEL ) ; 
		//if( emsc->shadow_on ){ PSP_Screen_DrawText( x-MENU_TEXT_SPACE-MISC_ICON_CX-1, y+1, ">", COLOR_SHADOW ) ; }
		//PSP_Screen_DrawText( x-MENU_TEXT_SPACE-MISC_ICON_CX-2, y  , ">", COLOR_SEL    ) ; 

		tp++;
		PSP_Bitmap_BltEx( &bmpMISC, x-MISC_CURSOR_CX-MISC_ICON_CX-3, y+1, MISC_CURSOR_CX, MISC_CURSOR_CY, 0, 
			MISC_CURSOR_TOP + ((tp/4)&7) * MISC_CURSOR_CY , PSP_Bitmap_GetBitColor( &bmpMISC, MISC_TR_CURSOR, 0 ) ) ;
	}
	else
	{
		PSP_Screen_DrawText( x, y, p, COLOR_NORMAL ) ; 
	}
	
	return y + MENU_TEXT_SPACE ;
}

int DrawMenuItem( int x, int y, const char* p, const char* item, int n, int sel )
{
	char buff[64] ;

	strcpy( buff, p ) ;
	strcat( buff, item ) ;

	return DrawMenu( x,y, buff, n, sel ) ;
}

int DrawMenuColor( int x, int y, const char* p, unsigned short color, int n, int sel )
{
	char sz_rgb[] = "RGB" ;
	char buff[64] ;
	int  len      ;
	int  rgb      ;
	int  pt       ;

	strcpy( buff, p ) ;
	strcat( buff, " color" ) ;
	len = strlen( buff ) ;

	for( rgb=0; rgb < 3; rgb++ )
	{
		pt  = len ;
		buff[ pt++ ] = '(' ;
		buff[ pt++ ] = sz_rgb[ rgb ] ;
		buff[ pt++ ] = ')' ;
		buff[ pt++ ] = ' ' ;
		buff[ pt++ ] = ':' ;
		buff[ pt++ ] = ' ' ;

		switch( rgb )
		{
		default:
		case 0: buff[ pt++ ] = '0' + ((color >>  0) & 0x1F) / 10 ;
				buff[ pt++ ] = '0' + ((color >>  0) & 0x1F) % 10 ; break ;
		case 1: buff[ pt++ ] = '0' + ((color >>  5) & 0x1F) / 10 ;
				buff[ pt++ ] = '0' + ((color >>  5) & 0x1F) % 10 ; break ;
		case 2: buff[ pt++ ] = '0' + ((color >> 10) & 0x1F) / 10 ;
				buff[ pt++ ] = '0' + ((color >> 10) & 0x1F) % 10 ; break ;
		}

		buff[ pt++ ] = 0   ;

		DrawMenu( x,y, buff, n, sel ) ;
		
		y += MENU_TEXT_SPACE ;

		memset( buff, ' ', len ) ;
		n++ ;
	}

	return y ;
}

void DrawMenuDate ( short x, short y, PSP_DATE_TIME* pDate, short color, short shadow, int shadow_on )
{
	if( (pDate->mon == 0) && (pDate->mday == 0) ){ return ; }

	char SZ_STATE_DATE [16] ;
	SZ_STATE_DATE[ 0] = '0' + pDate->mon  / 10 ;
	SZ_STATE_DATE[ 1] = '0' + pDate->mon  % 10 ;
	SZ_STATE_DATE[ 2] = '/' ;
	SZ_STATE_DATE[ 3] = '0' + pDate->mday / 10 ;
	SZ_STATE_DATE[ 4] = '0' + pDate->mday % 10 ;
	SZ_STATE_DATE[ 5] = ' ' ;
	SZ_STATE_DATE[ 6] = '0' + pDate->hour / 10 ;
	SZ_STATE_DATE[ 7] = '0' + pDate->hour % 10 ;
	SZ_STATE_DATE[ 8] = ':' ;
	SZ_STATE_DATE[ 9] = '0' + pDate->min  / 10 ;
	SZ_STATE_DATE[10] = '0' + pDate->min  % 10 ;
	SZ_STATE_DATE[11] = ':' ;
	SZ_STATE_DATE[12] = '0' + pDate->sec  / 10 ;
	SZ_STATE_DATE[13] = '0' + pDate->sec  % 10 ;
	SZ_STATE_DATE[14] = 0   ;

	DrawMenuText( x,y, SZ_STATE_DATE, color, shadow, shadow_on ) ;
}

void ToHEX( char* p, unsigned long d, int bit, int end_null )
{
	int n, m, v ;

	m = 0 ;
	for( n=bit-4; n >= 0 ; n -= 4 )
	{
		v = ((d >> n) & 0x0F) ;
		p[m++] = (v < 10) ? ( '0' + v ) : ( 'A' + v - 10 ) ;
	}

	if( end_null ){ p[m] = 0 ; }
}

////////////////////////////////////////////////////////////////////////

int ret = 0 ;
extern int debug_bmp[10] ;


void Menu_InitPath ( int argc, char *argv )
{
	if( (argc > 0) && argv && strlen(argv) )
	{
		char* p ;
		strcpy( path_exec, argv ) ;
		p = strrchr( path_exec, '/' ) ;
		if( p ){ *(p+1) = 0 ; }
	}
	else
	{
		strcpy( path_exec, "ms0:/PSP/GAME/" ) ;
	}
}

void Menu_Init ( )
{
	char buff[256] ;
	char* p ;
	int   n ;

	//strcpy( path_exec, argv ) ;
	//p = strrchr( path_exec, '/' ) ;
	//if( p ){ *(p+1) = 0 ; }

	emct = &emu_cheat ;
	EmuCheat_Init( emct ) ;

	EmuCfg_InitDefault( path_exec ) ;
	EmuCfg_GetDefault( &emu_config[ EMU_MD ] ) ;

	EmuStateCfg_InitDefault( path_exec ) ;

	strcpy( path_last[0], path_exec ) ;
	strcpy( path_last[1], path_exec ) ; 

	EmuSysCfg_Get()->rom_path = path_last[0] ;
	EmuSysCfg_Init( path_exec ) ;
	emsc = EmuSysCfg_Get() ;

	menu_cursol_max[ MENU_MODE_MD_CHEAT   ] = MAX_MENU_CUR_CHEAT  ;
	menu_cursol_max[ MENU_MODE_MD_CONFIG  ] = MAX_MENU_CUR_CONFIG ;
	menu_cursol_max[ MENU_MODE_MD_KEY     ] = MAX_MENU_CUR_KEY    ;
	menu_cursol_max[ MENU_MODE_MD_STATE   ] = MAX_MENU_CUR_STATE  ;
	menu_cursol_max[ MENU_MODE_MD_SYSTEM  ] = MAX_MENU_CUR_SYS    ;

	strcpy( buff, path_exec );
	strcat( buff, NAME_IMAGE_BG ) ;
	if( PSP_Bitmap_FileRead_PNG( buff, &bmpBG ) != 0 )
	{
		p = strrchr( buff, '.' ) ;
		p[1] = 'B' ; p[2] = 'M' ; p[3] = 'P' ;
		PSP_Bitmap_FileRead( buff, &bmpBG ) ;
	}

	strcpy( buff, path_exec );
	strcat( buff, NAME_IMAGE_MENU ) ;
	//debug_bmp[4] = PSP_Bitmap_FileRead_PNG( buff, &bmpMENU ) ;
	if( PSP_Bitmap_FileRead_PNG( buff, &bmpMENU ) != 0 )
	{
		p = strrchr( buff, '.' ) ;
		p[1] = 'B' ; p[2] = 'M' ; p[3] = 'P' ;
		PSP_Bitmap_FileRead( buff, &bmpMENU ) ;
	}

	strcpy( buff, path_exec );
	strcat( buff, NAME_IMAGE_MISC ) ;
	if( PSP_Bitmap_FileRead_PNG( buff, &bmpMISC ) != 0 )
	{
		p = strrchr( buff, '.' ) ;
		p[1] = 'B' ; p[2] = 'M' ; p[3] = 'P' ;
		PSP_Bitmap_FileRead( buff, &bmpMISC ) ;
	}

	strcpy( buff, path_exec ) ;
	strcat( buff, "SAVE"    ) ;
	sceIoMkdir( buff, 0777  ) ;

	strcpy( buff, path_exec ) ;
	strcat( buff, "CHEAT"   ) ;
	sceIoMkdir( buff, 0777  ) ;

	menu_mode = MENU_MODE_MD_LOAD ;
	Menu_UpdateFileList( path_last[0] ) ; 
}

enum{ MSGBOX_OKCANCEL, MSGBOX_NOBUTTON, MSGBOX_CHEAT_PATCH, MSGBOX_CHEAT_GG, MSGBOX_CHEAT_NAME } ;

int Menu_MessageBox( char* pMessage[], int num, int type )
{
	unsigned long buttons_new, buttons_old = 0 ;
	int n, l, t, r, b, w, h, len, grad=240, blank=1 ;
	int unit = 0 ;
	int is_key_r = 0 ;
	unsigned long tm_key_repeat ;

	sceKernelDcacheWritebackAll();

	tm_key_repeat = sceKernelLibcClock() ;

	while(1)
	{
		l = PSP_SCREEN_WIDTH      / 5 ;
		r = PSP_SCREEN_WIDTH  * 4 / 5 ;
		t = PSP_SCREEN_HEIGHT     / 3 ;
		b = PSP_SCREEN_HEIGHT * 2 / 3 ;
		w = r - l ;
		h = b - t ;

		if( type == MSGBOX_OKCANCEL )
		{
			PSP_Bitmap_BltEx( &bmpMISC, l, t, MISC_MSGBOX_CX, MISC_MSGBOX_CY, 
				MISC_MSGBOX_LEFT, MISC_MSGBOX1_TOP , 
					PSP_Bitmap_GetBitColor( &bmpMISC, MISC_TR_MSGBOX1, 0 ) ) ;
		}
		else //if( type == MSGBOX_NOBUTTON )
		{
			PSP_Bitmap_BltEx( &bmpMISC, l, t, MISC_MSGBOX_CX, MISC_MSGBOX_CY, 
				MISC_MSGBOX_LEFT, MISC_MSGBOX2_TOP , 
					PSP_Bitmap_GetBitColor( &bmpMISC, MISC_TR_MSGBOX2, 0 ) ) ;
		}

		if(      blank < 15 ){ blank++   ; }
		else if( blank < 30 ){ blank++   ; }
		else                 { blank = 0 ; }

		t += MENU_TEXT_SPACE / 2 ;
		len = strlen( menu_files[menu_files_sel].name ) * 5 ;
		l = PSP_SCREEN_WIDTH / 5 + (w - len) / 2 ;
		DrawMenuText( l,t, menu_files[menu_files_sel].name, COLOR_TITLE, COLOR_SHADOW, emsc->shadow_on ) ;

		PSP_Bitmap_BltEx( &bmpMISC, l - MISC_ICON_CX -2, t+1, MISC_ICON_CX, MISC_ICON_CY, 0, 
			(menu_files[menu_files_sel].ext_id -1) * MISC_ICON_CY + MISC_ICON_TOP , 
				PSP_Bitmap_GetBitColor( &bmpMISC, MISC_TR_ICON, 0 ) ) ;

		t = PSP_SCREEN_HEIGHT / 2 - MENU_TEXT_SPACE * num / 2 ;
		for( n=0; n < num ; n++ )
		{
			len = strlen( pMessage[n] ) * 5 ;
			l = PSP_SCREEN_WIDTH / 5 + (w - len) / 2 ;
			DrawMenuText( l,t, pMessage[n], COLOR_MSGBOX, COLOR_SHADOW, emsc->shadow_on ) ;
			t += MENU_TEXT_SPACE + 2 ;
		}

		PSP_Ctrlpad_Update() ;
		buttons_new = PSP_Ctrlpad_GetButtons() ;

		if( buttons_new & (CTRL_UP | CTRL_DOWN | CTRL_LEFT | CTRL_RIGHT) )
		{
			if( is_key_r )
			{
				if( (sceKernelLibcClock() - tm_key_repeat) > (TM_1FRAME*2) * emsc->key_repeat )
				{
					buttons_new &= ~(CTRL_UP | CTRL_DOWN | CTRL_LEFT | CTRL_RIGHT) ;
				}
			}
			else if( (sceKernelLibcClock() - tm_key_repeat) > TM_1FRAME * 10 )
			{
				if( buttons_old ){ is_key_r = 1 ; }
			}
		}
		else{  is_key_r = 0 ; }

		switch( type )
		{
		case MSGBOX_OKCANCEL :
			l = r - ( sizeof(SZ_MSGBOX_OK_CANCEL) + 2) * 5 ;
			t =  b - MENU_TEXT_SPACE * 2 ;
			DrawMenuText( l,t, SZ_MSGBOX_OK_CANCEL, COLOR_MSGBOX, COLOR_SHADOW, emsc->shadow_on ) ;

			if(      (buttons_old & CTRL_CIRCLE) && ((buttons_new & CTRL_CIRCLE)==0) ){ return 1 ; }
			else if( (buttons_old & CTRL_CROSS ) && ((buttons_new & CTRL_CROSS )==0) ){ return 0 ; }
			break ;

		case MSGBOX_NOBUTTON :
			if( blank > 3 ){ return 1 ; }
			break ;

		case MSGBOX_CHEAT_PATCH :
			l = PSP_SCREEN_WIDTH  / 5 + MENU_TEXT_SPACE * 3 / 2 ;
			t = PSP_SCREEN_HEIGHT / 3 + MENU_TEXT_SPACE * 5 / 2 ;
			DrawMenuText( l,t, SZ_MSGBOX_CHEAT_PT, COLOR_MSGBOX, COLOR_SHADOW, emsc->shadow_on ) ;

			len = sizeof( SZ_CHEAT_PT ) * 5 ;
			l = PSP_SCREEN_WIDTH / 5 + (w - len) / 2 ;
			t = PSP_SCREEN_HEIGHT / 2 ;//- MENU_TEXT_SPACE / 2 ;
			ToHEX( &SZ_CHEAT_PT[17], menu_cheat_addr , 24, 0 ) ;
			ToHEX( &SZ_CHEAT_PT[26], menu_cheat_data , 16, 0 ) ;
			DrawMenuText( l,t, SZ_CHEAT_PT, COLOR_MSGBOX, COLOR_SHADOW, emsc->shadow_on ) ;

			l = (unit < 6) ? l+17*5+unit*5-2 : l+20*5+unit*5-2 ;
			DrawMenuText( l, t-MENU_TEXT_SPACE, "Å•", COLOR_SEL, COLOR_SHADOW, emsc->shadow_on ) ;
			DrawMenuText( l, t+MENU_TEXT_SPACE, "Å£", COLOR_SEL, COLOR_SHADOW, emsc->shadow_on ) ;

			l = r - ( sizeof(SZ_MSGBOX_CHEAT) + 2) * 5 ;
			t =  b - MENU_TEXT_SPACE * 2 ;
			DrawMenuText( l,t, SZ_MSGBOX_CHEAT, COLOR_MSGBOX, COLOR_SHADOW, emsc->shadow_on ) ;

			if(      (buttons_old & CTRL_CIRCLE) && ((buttons_new & CTRL_CIRCLE)==0) ){ return 1 ; }
			else if( (buttons_old & CTRL_CROSS ) && ((buttons_new & CTRL_CROSS )==0) ){ return 0 ; }
			else if( (buttons_old & CTRL_LEFT  ) && ((buttons_new & CTRL_LEFT  )==0) ){ if( unit > 0 ){ unit--; } }
			else if( (buttons_old & CTRL_RIGHT ) && ((buttons_new & CTRL_RIGHT )==0) ){ if( unit < 9 ){ unit++; } }
			else if( (buttons_old & CTRL_UP    ) && ((buttons_new & CTRL_UP    )==0) )
			{
				if(      unit <  6 ){ calc_unit( &menu_cheat_addr , (5 - unit)*4, 1 ) ; }
				else                { calc_unit( &menu_cheat_data , (9 - unit)*4, 1 ) ; }
			}
			else if( (buttons_old & CTRL_DOWN  ) && ((buttons_new & CTRL_DOWN  )==0) )
			{  
				if(      unit <  6 ){ calc_unit( &menu_cheat_addr , (5 - unit)*4, -1 ) ; }
				else                { calc_unit( &menu_cheat_data , (9 - unit)*4, -1 ) ; }
			}
			break ;

		case MSGBOX_CHEAT_GG :
			l = PSP_SCREEN_WIDTH  / 5 + MENU_TEXT_SPACE * 3 / 2 ;
			t = PSP_SCREEN_HEIGHT / 3 + MENU_TEXT_SPACE * 5 / 2 ;
			DrawMenuText( l,t, SZ_MSGBOX_CHEAT_GG, COLOR_MSGBOX, COLOR_SHADOW, emsc->shadow_on ) ;

			len = sizeof( SZ_CHEAT_GG ) * 5 ;
			l = PSP_SCREEN_WIDTH / 5 + (w - len) / 2 ;
			t = PSP_SCREEN_HEIGHT / 2 ;//- MENU_TEXT_SPACE / 2 ;
			memcpy( &SZ_CHEAT_GG[18], &menu_cheat_gg[0], 4 ) ;
			memcpy( &SZ_CHEAT_GG[25], &menu_cheat_gg[5], 4 ) ;
			DrawMenuText( l,t, SZ_CHEAT_GG, COLOR_MSGBOX, COLOR_SHADOW, emsc->shadow_on ) ;

			l = (unit < 4) ? l+18*5+unit*5-2 : l+21*5+unit*5-2 ;
			DrawMenuText( l, t-MENU_TEXT_SPACE, "Å•", COLOR_SEL, COLOR_SHADOW, emsc->shadow_on ) ;
			DrawMenuText( l, t+MENU_TEXT_SPACE, "Å£", COLOR_SEL, COLOR_SHADOW, emsc->shadow_on ) ;

			l = r - ( sizeof(SZ_MSGBOX_CHEAT) + 2) * 5 ;
			t =  b - MENU_TEXT_SPACE * 2 ;
			DrawMenuText( l,t, SZ_MSGBOX_CHEAT, COLOR_MSGBOX, COLOR_SHADOW, emsc->shadow_on ) ;

			if(      (buttons_old & CTRL_CIRCLE) && ((buttons_new & CTRL_CIRCLE)==0) ){ return 1 ; }
			else if( (buttons_old & CTRL_CROSS ) && ((buttons_new & CTRL_CROSS )==0) ){ return 0 ; }
			else if( (buttons_old & CTRL_LEFT  ) && ((buttons_new & CTRL_LEFT  )==0) ){ if( unit > 0 ){ unit--; } }
			else if( (buttons_old & CTRL_RIGHT ) && ((buttons_new & CTRL_RIGHT )==0) ){ if( unit < 7 ){ unit++; } }
			else if( (buttons_old & CTRL_UP    ) && ((buttons_new & CTRL_UP    )==0) )
			{
				if( unit < 4 ){ calc_char( &menu_cheat_gg[unit  ], 1, 0 ) ; }
				else          { calc_char( &menu_cheat_gg[unit+1], 1, 0 ) ; }
			}
			else if( (buttons_old & CTRL_DOWN  ) && ((buttons_new & CTRL_DOWN  )==0) )
			{  
				if( unit < 4 ){ calc_char( &menu_cheat_gg[unit  ], -1, 0 ) ; }
				else          { calc_char( &menu_cheat_gg[unit+1], -1, 0 ) ; }
			}
			break ;

		case MSGBOX_CHEAT_NAME :
			l = PSP_SCREEN_WIDTH  / 5 + MENU_TEXT_SPACE * 3 / 2 ;
			t = PSP_SCREEN_HEIGHT / 3 + MENU_TEXT_SPACE * 5 / 2 ;
			DrawMenuText( l,t, SZ_CHEAT_NAME, COLOR_MSGBOX, COLOR_SHADOW, emsc->shadow_on ) ;

			len = MAX_CHEAT_NAME * 5 ;
			l = PSP_SCREEN_WIDTH / 5 + (w - len) / 2 ;
			t = PSP_SCREEN_HEIGHT / 2 ;//- MENU_TEXT_SPACE / 2 ;
			DrawMenuText( l,t, menu_cheat_name, COLOR_MSGBOX, COLOR_SHADOW, emsc->shadow_on ) ;
			DrawMenuText( l, t+MENU_TEXT_SPACE/2, SZ_CHEAT_NAME_T, COLOR_SEL, COLOR_SHADOW, emsc->shadow_on ) ;

			l = l + unit*5 -2 ;
			DrawMenuText( l, t-MENU_TEXT_SPACE  , "Å•", COLOR_SEL, COLOR_SHADOW, emsc->shadow_on ) ;
			DrawMenuText( l, t+MENU_TEXT_SPACE+1, "Å£", COLOR_SEL, COLOR_SHADOW, emsc->shadow_on ) ;

			l = r - ( sizeof(SZ_MSGBOX_CHEAT) + 2) * 5 ;
			t =  b - MENU_TEXT_SPACE * 2 ;
			DrawMenuText( l,t, SZ_MSGBOX_CHEAT, COLOR_MSGBOX, COLOR_SHADOW, emsc->shadow_on ) ;

			if(      (buttons_old & CTRL_CIRCLE) && ((buttons_new & CTRL_CIRCLE)==0) ){ return 1 ; }
			else if( (buttons_old & CTRL_CROSS ) && ((buttons_new & CTRL_CROSS )==0) ){ return 0 ; }
			else if( (buttons_old & CTRL_LEFT  ) && ((buttons_new & CTRL_LEFT  )==0) ){ if( unit > 0 ){ unit--; } }
			else if( (buttons_old & CTRL_RIGHT ) && ((buttons_new & CTRL_RIGHT )==0) ){ if( unit < MAX_CHEAT_NAME-1 ){ unit++; } }
			else if( (buttons_old & CTRL_UP    ) && ((buttons_new & CTRL_UP    )==0) )
			{
				calc_char( &menu_cheat_name[unit], 1, 1 ) ;
			}
			else if( (buttons_old & CTRL_DOWN  ) && ((buttons_new & CTRL_DOWN  )==0) )
			{  
				calc_char( &menu_cheat_name[unit], -1, 1 ) ;
			}
			break ;

		default : return 0 ;
		}

		if( buttons_new != buttons_old )
		{
			buttons_old   = buttons_new ;
			tm_key_repeat = sceKernelLibcClock() ;
		}

		PSP_Screen_WaitVsync() ;
		PSP_Screen_WaitVsync() ;
		PSP_Screen_Flip() ;
	}

	return 0 ;
}

void show_clock( int x, int y )
{
	PSP_DATE_TIME tm ;

	sceRtcGetCurrentClockLocalTime( &tm ) ;
	SZ_DATE[ 0] = (tm.mon / 10) ? '0' + tm.mon  / 10 : ' ' ;
	SZ_DATE[ 1] = '0' + tm.mon  % 10 ;
	SZ_DATE[ 3] = '0' + tm.mday / 10 ;
	SZ_DATE[ 4] = '0' + tm.mday % 10 ;
	SZ_DATE[ 6] = '0' + tm.hour / 10 ;
	SZ_DATE[ 7] = '0' + tm.hour % 10 ;
	SZ_DATE[ 9] = '0' + tm.min  / 10 ;
	SZ_DATE[10] = '0' + tm.min  % 10 ;
	PSP_Bitmap_BltEx( &bmpMISC, x-MISC_CLOCK_CX-1,y+1, MISC_CLOCK_CX, MISC_CLOCK_CY, 0, MISC_CLOCK_TOP , 
		PSP_Bitmap_GetBitColor( &bmpMISC, MISC_TR_CLOCK, 0 ) ) ;
	DrawMenuText( x,y, SZ_DATE, COLOR_TITLE, COLOR_SHADOW, emsc->shadow_on ) ;
}

void show_batt_info( int x, int y, int ct )
{
	int n, top, cy ;
	unsigned short misc_color_t ;
	unsigned char flag ;
	char buff[16] ;

	flag = (emsc->option_flags & (OPTION_SHOW_BATT1|OPTION_SHOW_BATT2)) ;

	if( flag && scePowerIsBatteryExist() )
	{

		if( (flag == (OPTION_SHOW_BATT1|OPTION_SHOW_BATT2)) && (menu_mode != MENU_MODE_MD_STATE) )
		{
			misc_color_t = PSP_Bitmap_GetBitColor( &bmpMISC, MISC_TR_BATT, 0 ) ;
			top = MISC_BATT_TOP ;
			cy  = MISC_BATT_CY  ;
		}
		else
		{
			misc_color_t = PSP_Bitmap_GetBitColor( &bmpMISC, MISC_TR_BATT2, 0 ) ;
			top = MISC_BATT2_TOP ;
			cy  = MISC_BATT2_CY  ;
		}

		n = scePowerGetBatteryLifePercent() ;
		SZ_BATT_LIFE_P[0] = (n / 100) ? '1' : ' ' ;
		SZ_BATT_LIFE_P[1] = (n /  10) ? '0' + ((n / 10) % 10) : ' ' ;
		SZ_BATT_LIFE_P[2] = '0' + (n % 10) ;
		strcpy( buff, SZ_BATT_LIFE_P ) ;

		if( scePowerIsPowerOnline() )
		{
			if( scePowerIsBatteryCharging() )
			{
				strcat( buff, SZ_BATT_LIFE_C ) ;

				if( (ct % 60) < 30 )
				{
					if( n > 0 )
					{
						PSP_Bitmap_BltEx( &bmpMISC, x-MISC_BATT_CX-1,y+1, MISC_BATT_CX, cy, 
							((n-1) / 20) * MISC_BATT_CX , top , misc_color_t ) ;
					}
					else
					{
						PSP_Bitmap_BltEx( &bmpMISC, x-MISC_BATT_CX-1,y+1, MISC_BATT_CX, cy, 
							0 , top , misc_color_t ) ;
					}
				}
				else
				{
				}
			}
			else
			{
				strcat( buff, SZ_BATT_LIFE_A ) ;

				PSP_Bitmap_BltEx( &bmpMISC, x-MISC_BATT_CX-1,y+1, MISC_BATT_CX, cy, 
					MISC_BATT_CX * 5, top , misc_color_t ) ;
			}
		}
		else
		{
			if( n > 0 )
			{
				if( n < 15 )
				{
					if( (ct % 40) < 20 )
					{
						PSP_Bitmap_BltEx( &bmpMISC, x-MISC_BATT_CX-1,y+1, MISC_BATT_CX, cy, 
							0 , top , misc_color_t ) ;
					}
				}
				else
				{
					PSP_Bitmap_BltEx( &bmpMISC, x-MISC_BATT_CX-1,y+1, MISC_BATT_CX, cy, 
						((n-1) / 20) * MISC_BATT_CX , top , misc_color_t ) ;
				}
			}

			n = scePowerGetBatteryLifeTime() ;
			if( n > 0 )
			{
				SZ_BATT_LIFE_T[1] = '0' + (n / 60) / 10 ;
				SZ_BATT_LIFE_T[2] = '0' + (n / 60) % 10 ;
				SZ_BATT_LIFE_T[4] = '0' + (n % 60) / 10 ;
				SZ_BATT_LIFE_T[5] = '0' + (n % 60) % 10 ;
				strcat( buff, SZ_BATT_LIFE_T ) ;
			}
			else
			{
				strcat( buff, SZ_BATT_LIFE_X ) ;
			}
		}

		if( emsc->option_flags & OPTION_SHOW_BATT1 )
		{
			DrawMenuText( x,y, buff, COLOR_TITLE, COLOR_SHADOW, emsc->shadow_on ) ;
			y += MENU_TEXT_SPACE ;
		}

		if( (emsc->option_flags & OPTION_SHOW_BATT2) && (menu_mode != MENU_MODE_MD_STATE) )
		{
			n = scePowerGetBatteryTemp() ;
			if( n >= 0 )
			{
				SZ_BATT_TEMP[1] = '0' + n / 10 ;
				SZ_BATT_TEMP[2] = '0' + n % 10 ;
				DrawMenuText( x,y, SZ_BATT_TEMP, COLOR_TITLE, COLOR_SHADOW, emsc->shadow_on ) ;
			}
		}
	}
}

void Menu_DrawFrame()
{
	static unsigned short ct    = 0 ;

	int n, x, y ;
	unsigned short misc_color_t ;

	ct++ ;
	if( ct > 360 ){ ct = 0 ; }

	switch( emsc->bg_mode )
	{
	case BG_MODE_ROLL      :
		PSP_Screen_GradRect( 0,0, PSP_SCREEN_WIDTH, PSP_SCREEN_HEIGHT, ct ) ;
		PSP_Bitmap_BltMask( &bmpMENU, 0,0, PSP_SCREEN_WIDTH, PSP_SCREEN_HEIGHT, 
			PSP_Bitmap_GetBitColor( &bmpMISC, MISC_TR_BG, 0 ) ) ;
		break ;
	case BG_MODE_NORMAL    : 
		PSP_Screen_FillRect( 0,0, PSP_SCREEN_WIDTH, PSP_SCREEN_HEIGHT, emsc->color_bg ) ;
		PSP_Bitmap_BltMask( &bmpMENU, 0,0, PSP_SCREEN_WIDTH, PSP_SCREEN_HEIGHT, 
			PSP_Bitmap_GetBitColor( &bmpMISC, MISC_TR_BG, 0 ) ) ;
		break ;
	case BG_MODE_GRADATION :
		PSP_Screen_GradRect( 0,0, PSP_SCREEN_WIDTH, PSP_SCREEN_HEIGHT, emsc->color_grad ) ;
		PSP_Bitmap_BltMask( &bmpMENU, 0,0, PSP_SCREEN_WIDTH, PSP_SCREEN_HEIGHT, 
			PSP_Bitmap_GetBitColor( &bmpMISC, MISC_TR_BG, 0 ) ) ;
		break ;
	case BG_MODE_IMAGE     :
		PSP_Bitmap_Blt( &bmpMENU, 0,0, PSP_SCREEN_WIDTH, PSP_SCREEN_HEIGHT ) ;
		break ;
	}

	{
		x = PSP_SCREEN_WIDTH - MENU_TEXT_SPACE * 4 - MENU_FRAME_SPACE * 4 ;
		y = MENU_ITEM_TOP ;

		if( emsc->option_flags & OPTION_SHOW_CLOCK )
		{
			show_clock( x, y ) ;
			y += MENU_TEXT_SPACE ;

			if( menu_mode != MENU_MODE_MD_STATE ){ show_batt_info( x, y, ct ) ; }
		}
		else
		{
			show_batt_info( x, y, ct ) ;
		}
	}

	misc_color_t = PSP_Bitmap_GetBitColor( &bmpMISC, MISC_TR_TITLE, 0 ) ;
	x = 0 ;
	y = MENU_FRAME_SPACE*2 ;

	PSP_Bitmap_BltEx( &bmpMISC, x, y, MISC_TITLE_LR, MISC_TITLE_CY, x, MISC_TITLE_TOP, misc_color_t ) ;
	x += MISC_TITLE_LR ;

	for( n=0; n < MISC_TITLE_NUM ; n++ )
	{
		if( n == menu_mode )
		{
			PSP_Bitmap_BltEx( &bmpMISC, x, y, MISC_TITLE_CX, MISC_TITLE_CY, x, 
				MISC_TITLE_TOP + MISC_TITLE_CY, misc_color_t ) ;
		}
		else
		{
			PSP_Bitmap_BltEx( &bmpMISC, x, y, MISC_TITLE_CX, MISC_TITLE_CY, x, MISC_TITLE_TOP, misc_color_t ) ;
		}

		//if( n == 1 ){ x += MISC_TITLE_CX * 2 ; }else{ x += MISC_TITLE_CX ; }
		x += MISC_TITLE_CX ;
	}

	PSP_Bitmap_BltEx( &bmpMISC, x, y, MISC_TITLE_LR, MISC_TITLE_CY, x, MISC_TITLE_TOP, misc_color_t ) ;

	/*
	debug_bmp[0] = bmpMENU.nCX ;
	debug_bmp[1] = bmpMENU.nCY ;
	PSP_Screen_DrawIntN( 0, 30, debug_bmp[0], 5, 0xFFFF ) ;
	PSP_Screen_DrawIntN( 0, 40, debug_bmp[1], 5, 0xFFFF ) ;
	PSP_Screen_DrawIntN( 0, 50, debug_bmp[2],10, 0xFFFF ) ;
	PSP_Screen_DrawIntN( 0, 60, debug_bmp[3],10, 0xFFFF ) ;
	PSP_Screen_DrawIntN( 0, 70, debug_bmp[4],10, 0xFFFF ) ;
	PSP_Screen_DrawIntN( 0, 80, debug_bmp[5],10, 0xFFFF ) ;
	PSP_Screen_DrawIntN( 0, 90, debug_bmp[6],10, 0xFFFF ) ;
	PSP_Screen_DrawIntN( 0,100, debug_bmp[7],10, 0xFFFF ) ;
	PSP_Screen_DrawIntN( 0,110, debug_bmp[8],10, 0xFFFF ) ;
	//*/
}

void Menu_DrawMD_Load()
{
	int x, y, n, st ;
	unsigned short misc_icon_t ;

	misc_icon_t = PSP_Bitmap_GetBitColor( &bmpMISC, MISC_TR_ICON, 0 ) ;

	x = MENU_ITEM_LEFT ;
	y = MENU_ITEM_TOP  ;

	PSP_Bitmap_BltEx( &bmpMISC, x-MISC_ICON_CX-1, y+1, MISC_ICON_CX, MISC_ICON_CY, 0, MISC_ICON_TOP , misc_icon_t ) ;

	DrawMenuText( x+1,y, path_last[0], COLOR_TITLE, COLOR_SHADOW, emsc->shadow_on ) ;

	y += MENU_TEXT_SPACE + 2 ;
	if( menu_files_num > MAX_ROM_PAGE )
	{
		st = menu_cursol[ menu_mode ] - MAX_ROM_PAGE / 2 ;
		if( st < 0 ){ st = 0 ; }
		else if( st > (menu_files_num - MAX_ROM_PAGE) ){ st = (menu_files_num - MAX_ROM_PAGE) ; }

		for( n=st; n < (st + MAX_ROM_PAGE) ; n++ )
		{
			PSP_Bitmap_BltEx( &bmpMISC, x,y+1, MISC_ICON_CX, MISC_ICON_CY, 0, 
				(menu_files[n].ext_id -1) * MISC_ICON_CY + MISC_ICON_TOP , misc_icon_t ) ;

			y = DrawMenu2( x+MISC_ICON_CX+1,y, menu_files[n].name, n, menu_cursol[ menu_mode ] ) ;
		}
	}
	else
	{
		for( n=0; n < menu_files_num ; n++ )
		{
			PSP_Bitmap_BltEx( &bmpMISC, x,y+1, MISC_ICON_CX, MISC_ICON_CY, 0, 
				(menu_files[n].ext_id -1) * MISC_ICON_CY + MISC_ICON_TOP , misc_icon_t ) ;

			y = DrawMenu2( x+MISC_ICON_CX+1,y, menu_files[n].name, n, menu_cursol[ menu_mode ] ) ;
		}
	}

	x = MENU_MESSAGE_LEFT ;
	y = MENU_MESSAGE_TOP  ;

	if( menu_files[ menu_cursol[ menu_mode ] ].type & PSP_FILE_TYPE_FILE )
	{
		if( EmuCore_IsSaveSRAM() )
		{
			DrawMenuText( x,y, SZ_MENU_MESSAGE_ROM[1], COLOR_MSG, COLOR_SHADOW, emsc->shadow_on ) ;
		}
		else
		{
			DrawMenuText( x,y, SZ_MENU_MESSAGE_ROM[0], COLOR_MSG, COLOR_SHADOW, emsc->shadow_on ) ;
		}
	}
	else
	{
		DrawMenuText( x,y, SZ_MENU_MESSAGE_ROM[2], COLOR_MSG, COLOR_SHADOW, emsc->shadow_on ) ;
	}
}

void Menu_DrawMD_State()
{
	unsigned short* pDisp ;
	int x, y, n, m, sel = 0 ;
	int cursol = menu_cursol[ menu_mode ] ;
	EMU_STATE* es ;

	if( menu_files_sel_s >= menu_files_num )
	{
		menu_files_sel_s = menu_files_num -1 ;
	}

	if( menu_files_sel == menu_files_sel_s )
	{
		es = &emu_state ;
	}
	else
	{ 
		es = &menu_emu_state ;

		if( (is_menu_emu_state_load == 0) && (menu_files[ menu_files_sel_s ].type & PSP_FILE_TYPE_FILE) )
		{
			is_menu_emu_state_load = 1 ;
			make_state_path( path_state_s, menu_files[ menu_files_sel_s ].name ) ;
			EmuState_Load( &menu_emu_state, path_state_s ) ;
		}
	}

	emc = &emu_config[EMU_MD] ;
	x = PSP_SCREEN_WIDTH * 25 / 100 ; //MENU_FRAME_SPACE * 4 ;
	y = MENU_ITEM_TOP ;

	if( (menu_files_num > 0) && (menu_files[ menu_files_sel_s ].type & PSP_FILE_TYPE_FILE) )
	{
		x = (PSP_SCREEN_WIDTH - strlen( menu_files[menu_files_sel_s].name ) * 5) /2 ;
		DrawMenu( x, y+MENU_CUR_STATE_ROM*MENU_TEXT_SPACE, menu_files[menu_files_sel_s].name, sel++, cursol ) ;
	}
	else
	{
		x = (PSP_SCREEN_WIDTH - strlen( "---" ) * 5) /2 ;
		DrawMenu( x, y+MENU_CUR_STATE_ROM*MENU_TEXT_SPACE, "---", sel++, cursol ) ;
	}

	y += MENU_TEXT_SPACE + 3 ;
	for( n=0; n < 5 ; n++ )
	{
		if( emsc->shadow_on )
		{
			PSP_Screen_Rectangle( 
				(PSP_SCREEN_WIDTH - (STATE_IMAGE_CX +10) * 5) /2 + (n) * (STATE_IMAGE_CX +10) +5                 +1, y                  +1,
				(PSP_SCREEN_WIDTH - (STATE_IMAGE_CX +10) * 5) /2 + (n) * (STATE_IMAGE_CX +10) +5 + STATE_IMAGE_CX+1, y + STATE_IMAGE_CY   ,
				COLOR_SHADOW
				) ;
		}

		pDisp = PSP_Screen_GetBackBuffer( 
			(PSP_SCREEN_WIDTH - (STATE_IMAGE_CX +10) * 5) /2 + n * (STATE_IMAGE_CX +10) +5, y ) ;

		if( es->slot[n].flag & STATE_FLAG_ACTIVE )
		{
			for( m=0; m < STATE_IMAGE_CY ; m++ )
			{
				for( x=0; x < STATE_IMAGE_CX ; x++ )
				{
					pDisp[x] = es->slot[n].image[m][x] ;
				}

				pDisp += PSP_LINESIZE ;
			}

			if( (emsc->option_flags & OPTION_SHOW_DATE) == 0 )
			{
				DrawMenuDate( 
					(PSP_SCREEN_WIDTH - (STATE_IMAGE_CX +10) * 5) /2 
					+ n * (STATE_IMAGE_CX +10) + (STATE_IMAGE_CX - (13)*5) /2 +2,
					y + STATE_IMAGE_CY - 2 - MENU_TEXT_SPACE ,
					&es->slot[n].date, 
					COLOR_NORMAL, COLOR_SHADOW, emsc->shadow_on ) ;

			}
		}
		else
		{
			PSP_Screen_Rectangle( 
				(PSP_SCREEN_WIDTH - (STATE_IMAGE_CX +10) * 5) /2 + (n) * (STATE_IMAGE_CX +10) +5, y ,
				(PSP_SCREEN_WIDTH - (STATE_IMAGE_CX +10) * 5) /2 + (n) * (STATE_IMAGE_CX +10) +5 + STATE_IMAGE_CX, y + STATE_IMAGE_CY -1,
				COLOR_NORMAL
				) ;
		}

		if( sel == cursol )
		{
			PSP_Screen_Rectangle( 
				(PSP_SCREEN_WIDTH - (STATE_IMAGE_CX +10) * 5) /2 + (n) * (STATE_IMAGE_CX +10) +5, y ,
				(PSP_SCREEN_WIDTH - (STATE_IMAGE_CX +10) * 5) /2 + (n) * (STATE_IMAGE_CX +10) +5 + STATE_IMAGE_CX, y + STATE_IMAGE_CY -1,
				COLOR_SEL
				) ;
		}

		SZ_STATE_SLOT[ 0 ] = es->cfg.slot_inc == n ? '*' : ' ' ;
		SZ_STATE_SLOT[ sizeof(SZ_STATE_SLOT) - 2 ] = '0' + n ;

		DrawMenu( 
			(PSP_SCREEN_WIDTH - (STATE_IMAGE_CX +10) * 5) /2 
			+ n * (STATE_IMAGE_CX +10) + (STATE_IMAGE_CX - (sizeof(SZ_STATE_SLOT)-1)*5) /2 ,
			y + STATE_IMAGE_CY + 1,
			SZ_STATE_SLOT, sel++, cursol ) ;
	}

	y += STATE_IMAGE_CY + MENU_TEXT_SPACE + 3 ;
	for( n=5; n < 10 ; n++ )
	{
		if( emsc->shadow_on )
		{
			PSP_Screen_Rectangle( 
				(PSP_SCREEN_WIDTH - (STATE_IMAGE_CX +10) * 5) /2 + (n-5) * (STATE_IMAGE_CX +10) +5                 +1, y                  +1,
				(PSP_SCREEN_WIDTH - (STATE_IMAGE_CX +10) * 5) /2 + (n-5) * (STATE_IMAGE_CX +10) +5 + STATE_IMAGE_CX+1, y + STATE_IMAGE_CY   ,
				COLOR_SHADOW
				) ;
		}

		pDisp = PSP_Screen_GetBackBuffer( 
			(PSP_SCREEN_WIDTH - (STATE_IMAGE_CX +10) * 5) /2 + (n-5) * (STATE_IMAGE_CX +10) +5, y ) ;

		if( es->slot[n].flag & STATE_FLAG_ACTIVE )
		{
			for( m=0; m < STATE_IMAGE_CY ; m++ )
			{
				for( x=0; x < STATE_IMAGE_CX ; x++ )
				{
					pDisp[x] = es->slot[n].image[m][x] ;
				}

				pDisp += PSP_LINESIZE ;
			}

			if( (emsc->option_flags & OPTION_SHOW_DATE) == 0 )
			{
				DrawMenuDate( 
					(PSP_SCREEN_WIDTH - (STATE_IMAGE_CX +10) * 5) /2 
					+ (n-5) * (STATE_IMAGE_CX +10) + (STATE_IMAGE_CX - (13)*5) /2 +2 ,
					y + STATE_IMAGE_CY - 2 - MENU_TEXT_SPACE ,
					&es->slot[n].date, 
					COLOR_NORMAL, COLOR_SHADOW, emsc->shadow_on ) ;

			}
		}
		else
		{
			PSP_Screen_Rectangle( 
				(PSP_SCREEN_WIDTH - (STATE_IMAGE_CX +10) * 5) /2 + (n-5) * (STATE_IMAGE_CX +10) +5, y ,
				(PSP_SCREEN_WIDTH - (STATE_IMAGE_CX +10) * 5) /2 + (n-5) * (STATE_IMAGE_CX +10) +5 + STATE_IMAGE_CX, y + STATE_IMAGE_CY -1,
				COLOR_NORMAL
				) ;
		}

		if( sel == cursol )
		{
			PSP_Screen_Rectangle( 
				(PSP_SCREEN_WIDTH - (STATE_IMAGE_CX +10) * 5) /2 + (n-5) * (STATE_IMAGE_CX +10) +5, y ,
				(PSP_SCREEN_WIDTH - (STATE_IMAGE_CX +10) * 5) /2 + (n-5) * (STATE_IMAGE_CX +10) +5 + STATE_IMAGE_CX, y + STATE_IMAGE_CY -1,
				COLOR_SEL
				) ;
		}

		SZ_STATE_SLOT[ 0 ] = es->cfg.slot_inc == n ? '*' : ' ' ;
		SZ_STATE_SLOT[ sizeof(SZ_STATE_SLOT) - 2 ] = '0' + n ;

		DrawMenu( 
			(PSP_SCREEN_WIDTH - (STATE_IMAGE_CX +10) * 5) /2 
			+ (n-5) * (STATE_IMAGE_CX +10) + (STATE_IMAGE_CX - (sizeof(SZ_STATE_SLOT)-1)*5) /2 ,
			y + STATE_IMAGE_CY +1,
			SZ_STATE_SLOT, sel++, cursol ) ;
	}

	x = PSP_SCREEN_WIDTH  * 25 / 100 ; //MENU_FRAME_SPACE * 4 ;
	y = MENU_FRAME_SPACE * 4 + 4 + MENU_TEXT_SPACE * 4 ;
	DrawMenu( x, y+MENU_CUR_STATE_MODE1*MENU_TEXT_SPACE, SZ_STATE_MODE1[ es->cfg.save_mode1 ], sel++, cursol ) ;
	DrawMenu( x, y+MENU_CUR_STATE_MODE2*MENU_TEXT_SPACE, SZ_STATE_MODE2[ es->cfg.save_mode2 ], sel++, cursol ) ;
	DrawMenu( x, y+MENU_CUR_STATE_LOOP *MENU_TEXT_SPACE, SZ_STATE_LOOP [ es->cfg.loop_mode  ], sel++, cursol ) ;

	SZ_STATE_SLOT_I[ sizeof(SZ_STATE_SLOT_M) -2 ] = '0' + es->cfg.slot_inc ;
	DrawMenu( x, y+MENU_CUR_STATE_INC*MENU_TEXT_SPACE, SZ_STATE_SLOT_I, sel++, cursol ) ;

	SZ_STATE_SLOT_M[ sizeof(SZ_STATE_SLOT_M) -2 ] = '0' + es->cfg.slot_sel ;
	DrawMenu( x, y+MENU_CUR_STATE_SEL*MENU_TEXT_SPACE, SZ_STATE_SLOT_M, sel++, cursol ) ;

	x = MENU_MESSAGE_LEFT ;
	y = MENU_MESSAGE_TOP  ;
	if( cursol == 0 )
	{
		DrawMenuText( x,y, SZ_MENU_MESSAGE_STATE[0], COLOR_MSG, COLOR_SHADOW, emsc->shadow_on ) ;
	}
	else if( cursol >= MENU_CUR_STATE_MODE1 )
	{
		DrawMenuText( x,y, SZ_MENU_MESSAGE_STATE[3], COLOR_MSG, COLOR_SHADOW, emsc->shadow_on ) ;
	}
	else
	{
		if( menu_files_sel == menu_files_sel_s )
		{
			if( PSP_Ctrlpad_GetButtons() & CTRL_SELECT )
			{
				DrawMenuText( x,y, SZ_MENU_MESSAGE_STATE[4], COLOR_MSG, COLOR_SHADOW, emsc->shadow_on ) ;
			}
			else
			{
				DrawMenuText( x,y, SZ_MENU_MESSAGE_STATE[1], COLOR_MSG, COLOR_SHADOW, emsc->shadow_on ) ;
			}
		}
		else
		{
			DrawMenuText( x,y, SZ_MENU_MESSAGE_STATE[2], COLOR_MSG, COLOR_SHADOW, emsc->shadow_on ) ;
		}
	}
}

void Menu_DrawMD_Cheat()
{
	int x, y, n, m, st, sel = 0 ;
	int cursol = menu_cursol[ menu_mode ] ;
	char buff[ 128 ] ;

	x = MENU_ITEM_LEFT ;
	y = MENU_ITEM_TOP  + MENU_TEXT_SPACE / 2 ;

	DrawMenu( x, y, SZ_CHEAT_ON[ emct->all_on ], sel++, cursol ) ;

//	if( emct->all_on )
	{
		y += MENU_TEXT_SPACE * 2 ;

		DrawMenuText( x+MENU_TEXT_SPACE,y, SZ_CHEAT_HEAD, COLOR_TITLE, COLOR_SHADOW, emsc->shadow_on ) ;
		
		if( emsc->shadow_on ){ 	PSP_Screen_Rectangle( x+MENU_TEXT_SPACE/2+1, y-MENU_TEXT_SPACE*3/8+1, PSP_SCREEN_WIDTH*3/4-15+1, y+MENU_TEXT_SPACE*9/8+1+1, COLOR_SHADOW ) ; }
		PSP_Screen_Rectangle( x+MENU_TEXT_SPACE/2, y-MENU_TEXT_SPACE*3/8, PSP_SCREEN_WIDTH*3/4-15, y+MENU_TEXT_SPACE*9/8+1, COLOR_NORMAL ) ;

		y += MENU_TEXT_SPACE * 3 / 2 ;

		if(      cursol < MENU_CUR_CHEAT_MIN+MAX_SHOW_CHEAT/2 ){ st = 0                              ; }
		else if( cursol > MENU_CUR_CHEAT_MAX-MAX_SHOW_CHEAT/2 ){ st = MAX_CHEAT_NUM - MAX_SHOW_CHEAT ; }
		else                                                   { st = cursol - MENU_CUR_CHEAT_MIN - MAX_SHOW_CHEAT/2 ; }

		for( n=st; n < st+MAX_SHOW_CHEAT ; n++ )
		{
			if( emsc->shadow_on ){ 	PSP_Screen_Rectangle( x+MENU_TEXT_SPACE/2+1, y-MENU_TEXT_SPACE*3/8+1, PSP_SCREEN_WIDTH*3/4-15+1, y+MENU_TEXT_SPACE*9/8+1+1, COLOR_SHADOW ) ; }
			PSP_Screen_Rectangle( x+MENU_TEXT_SPACE/2, y-MENU_TEXT_SPACE*3/8, PSP_SCREEN_WIDTH*3/4-15, y+MENU_TEXT_SPACE*9/8+1, COLOR_NORMAL ) ;

			m = 0 ;
			strcpy( &buff[m], SZ_CHEAT_FLAG[ emct->flag[n] ] ) ;
			m += strlen( SZ_CHEAT_FLAG[ emct->flag[n] ] ) ;

			buff[m++] = ( (n+1) < 10 ) ? ' ' : ('0' + ((n+1) /10)) ;
			buff[m++] = ('0' + ((n+1) % 10)) ;

			buff[m++] = ' ' ; buff[m++] = ' ' ;

			ToHEX( &buff[ m ], emct->addr[n], 24, 0 ) ;
			m += 6 ;

			buff[m++] = ' ' ; buff[m++] = ' ' ; buff[m++] = ' ' ;

			ToHEX( &buff[ m ], emct->data[n], 16, 0 ) ;
			m += 4 ;

			buff[m++] = ' ' ; buff[m++] = ' ' ;

			if( strlen( emct->code[n] ) == MAX_CHEAT_GG  )
			{
				strcpy( &buff[m], emct->code[n] ) ;
			}
			else{ memset( &buff[m], '-', MAX_CHEAT_GG ) ; }
			m += MAX_CHEAT_GG ;

			buff[m++] = ' ' ; buff[m++] = ' ' ;

			strcpy( &buff[m], emct->name[n] ) ;

			DrawMenu2( x+MENU_TEXT_SPACE, y, buff, sel+n, cursol ) ;
			y += MENU_TEXT_SPACE * 3 / 2 ;
		}

		menu_cursol_max[ MENU_MODE_MD_CHEAT ] = MAX_MENU_CUR_CHEAT  ;
	}
	//else{ menu_cursol_max[ MENU_MODE_MD_CHEAT ] = 1 ; }

	x = MENU_MESSAGE_LEFT ;
	y = MENU_MESSAGE_TOP  ;
	if( cursol == 0 )
	{
		DrawMenuText( x,y, SZ_MENU_MESSAGE_CHEAT[0], COLOR_MSG, COLOR_SHADOW, emsc->shadow_on ) ;
	}
	else
	{
		DrawMenuText( x,y, SZ_MENU_MESSAGE_CHEAT[1], COLOR_MSG, COLOR_SHADOW, emsc->shadow_on ) ;
		DrawMenuText( x+32*5,y-MENU_TEXT_SPACE*3/2, SZ_MENU_MESSAGE_CHEAT[2], COLOR_MSG, COLOR_SHADOW, emsc->shadow_on ) ;
	}
}

void Menu_DrawMD_Config()
{
	int x, y, i, sel = 0 ;
	int cursol = menu_cursol[ menu_mode ] ;

	emc = &emu_config[EMU_MD] ;
	//x = PSP_SCREEN_WIDTH * 25 / 100 ; //MENU_FRAME_SPACE * 4 ;
	x = PSP_SCREEN_WIDTH * 8 / 100 ; //MENU_FRAME_SPACE * 4 ;
	y = MENU_FRAME_SPACE * 4 + MENU_TEXT_SPACE ;

	SZ_SKIP  [ sizeof(SZ_SKIP  ) -2 ] = '0' + emc->skip_count ;
	SZ_SKIP_T[ sizeof(SZ_SKIP_T) -3 ] = '0' + emc->skip_turbo / 10 ;
	SZ_SKIP_T[ sizeof(SZ_SKIP_T) -2 ] = '0' + emc->skip_turbo % 10 ;

	SZ_MAX_FPS[ sizeof(SZ_MAX_FPS) -3 ] = '0' + emc->max_fps / 10 ;
	SZ_MAX_FPS[ sizeof(SZ_MAX_FPS) -2 ] = '0' + emc->max_fps % 10 ;

#ifdef _EMU_CLOCK_ADJ  ////////////////////
	int emu_clk , z80_clk ;
	emu_clk = emc->emu_clock + 1000 ;
	z80_clk = emc->z80_clock + 1000 ;
	SZ_EMU_CLOCK[ sizeof(SZ_EMU_CLOCK) - 6 ] = '0' + emu_clk / 1000 ; emu_clk = emu_clk % 1000 ;
	SZ_EMU_CLOCK[ sizeof(SZ_EMU_CLOCK) - 4 ] = '0' + emu_clk /  100 ; emu_clk = emu_clk %  100 ;
	SZ_EMU_CLOCK[ sizeof(SZ_EMU_CLOCK) - 3 ] = '0' + emu_clk /   10 ; emu_clk = emu_clk %   10 ;
	SZ_EMU_CLOCK[ sizeof(SZ_EMU_CLOCK) - 2 ] = '0' + emu_clk        ; 
	SZ_Z80_CLOCK[ sizeof(SZ_Z80_CLOCK) - 6 ] = '0' + z80_clk / 1000 ; z80_clk = z80_clk % 1000 ;
	SZ_Z80_CLOCK[ sizeof(SZ_Z80_CLOCK) - 4 ] = '0' + z80_clk /  100 ; z80_clk = z80_clk %  100 ;
	SZ_Z80_CLOCK[ sizeof(SZ_Z80_CLOCK) - 3 ] = '0' + z80_clk /   10 ; z80_clk = z80_clk %   10 ;
	SZ_Z80_CLOCK[ sizeof(SZ_Z80_CLOCK) - 2 ] = '0' + z80_clk        ; 
#endif // _EMU_CLOCK_ADJ  //////////////////

	DrawMenu( x, y+MENU_CUR_CPU      *MENU_TEXT_SPACE, SZ_CPU_FREQ [ emc->cpu_freq    ], sel++, cursol ) ;
	y += MENU_TEXT_SPACE/2 ;

#ifdef _EMU_CLOCK_ADJ  ////////////////////
	DrawMenu( x, y+MENU_CUR_EMU_CLOCK *MENU_TEXT_SPACE, SZ_EMU_CLOCK , sel++, cursol ) ;
	DrawMenu( x, y+MENU_CUR_Z80_CLOCK *MENU_TEXT_SPACE, SZ_Z80_CLOCK , sel++, cursol ) ;
	y += MENU_TEXT_SPACE/2 ;
#endif // _EMU_CLOCK_ADJ  //////////////////

	DrawMenu( x, y+MENU_CUR_CORE     *MENU_TEXT_SPACE, SZ_CORE     [ (emc->show_flags & FLAG_CORE    ) ? 1 : 0 ], sel++, cursol ) ;
	DrawMenu( x, y+MENU_CUR_Z80ASYNC *MENU_TEXT_SPACE, SZ_Z80ASYNC [ (emc->show_flags & FLAG_Z80ASYNC) ? 1 : 0 ], sel++, cursol ) ;
	DrawMenu( x, y+MENU_CUR_AUTOSRAM *MENU_TEXT_SPACE, SZ_AUTOSRAM [ emc->autosram    ], sel++, cursol ) ;
	DrawMenu( x, y+MENU_CUR_COUNTRY  *MENU_TEXT_SPACE, SZ_COUNTRY  [ emc->country     ], sel++, cursol ) ;
	DrawMenu( x, y+MENU_CUR_CHECKSUM *MENU_TEXT_SPACE, SZ_CHECKSUM [ (emc->show_flags & FLAG_CHECKSUM) ? 1 : 0 ], sel++, cursol ) ;
	y += MENU_TEXT_SPACE/2 ;
	DrawMenu( x, y+MENU_CUR_2CON     *MENU_TEXT_SPACE, SZ_2CON     [ emc->comm_mode   ], sel++, cursol ) ;
	y += MENU_TEXT_SPACE/2 ;

	DrawMenu( x, y+MENU_CUR_MAX_FPS  *MENU_TEXT_SPACE, SZ_MAX_FPS                      , sel++, cursol ) ;
	DrawMenu( x, y+MENU_CUR_SKIP     *MENU_TEXT_SPACE, SZ_SKIP                         , sel++, cursol ) ;
	DrawMenu( x, y+MENU_CUR_SKIP_MODE*MENU_TEXT_SPACE, SZ_SKIP_MODE[ emc->skip_mode   ], sel++, cursol ) ;
	DrawMenu( x, y+MENU_CUR_SKIP_T   *MENU_TEXT_SPACE, SZ_SKIP_T                       , sel++, cursol ) ;
	//y += MENU_TEXT_SPACE/2 ;

	x = PSP_SCREEN_WIDTH  * 50 / 100 ; //MENU_FRAME_SPACE * 4 ;
	y = PSP_SCREEN_HEIGHT * 30 / 100 - MENU_CUR_SCREEN * MENU_TEXT_SPACE ;

	DrawMenu( x, y+MENU_CUR_SCREEN   *MENU_TEXT_SPACE, SZ_SCREEN   [ emc->screen_mode ], sel++, cursol ) ;
	DrawMenu( x, y+MENU_CUR_SHOW_FPS *MENU_TEXT_SPACE, SZ_SHOW_FPS [ emc->show_fps    ], sel++, cursol ) ;
	DrawMenu( x, y+MENU_CUR_SHOW_BG  *MENU_TEXT_SPACE, SZ_SHOW_BG  [ emc->show_flags & SHOW_BG    ? 1 : 0 ], sel++, cursol ) ;
	DrawMenu( x, y+MENU_CUR_VSYNC    *MENU_TEXT_SPACE, SZ_VSYNC    [ emc->show_flags & SHOW_VSYNC ? 1 : 0 ], sel++, cursol ) ;
	DrawMenu( x, y+MENU_CUR_PALETTE  *MENU_TEXT_SPACE, SZ_PALETTE_MODE[ emc->palette_mode ], sel++, cursol ) ;
	y += MENU_TEXT_SPACE/2 ;

	DrawMenu( x, y+MENU_CUR_SOUND_ON *MENU_TEXT_SPACE, SZ_SOUND_ON [ emc->sound_on    ], sel++, cursol ) ;

	if( emc->sound_on )
	{
		DrawMenu( x, y+MENU_CUR_SOUND_VOL *MENU_TEXT_SPACE, SZ_SOUND_VOL  [ emc->sound_vol                         ], sel++, cursol ) ;
		DrawMenu( x, y+MENU_CUR_SOUND_YMCH*MENU_TEXT_SPACE, SZ_SOUND_YM   [ emc->sound_flag & MD_SND_YMCH  ? 1 : 0 ], sel++, cursol ) ;
		DrawMenu( x, y+MENU_CUR_SOUND_PSG *MENU_TEXT_SPACE, SZ_SOUND_PSG  [ emc->sound_flag & MD_SND_PSG   ? 1 : 0 ], sel++, cursol ) ;
		DrawMenu( x, y+MENU_CUR_SOUND_DAC *MENU_TEXT_SPACE, SZ_SOUND_DAC  [ emc->sound_flag & MD_SND_DAC   ? 1 : 0 ], sel++, cursol ) ;
		DrawMenu( x, y+MENU_CUR_SOUND_TB  *MENU_TEXT_SPACE, SZ_SOUND_TURBO[ emc->sound_flag & MD_SND_TURBO ? 1 : 0 ], sel++, cursol ) ;

		menu_cursol_max[ MENU_MODE_MD_CONFIG  ] = MAX_MENU_CUR_CONFIG ;
	}
	else
	{
		menu_cursol_max[ MENU_MODE_MD_CONFIG  ] = MAX_MENU_CUR_CONFIG - 5 ;
	}

	x = MENU_MESSAGE_LEFT ;
	y = MENU_MESSAGE_TOP  ;
	DrawMenuText( x,y, SZ_MENU_MESSAGE_CFG, COLOR_MSG, COLOR_SHADOW, emsc->shadow_on ) ;
}

void Menu_DrawMD_KeyCfg()
{
	int x, y, n, m, num = 0, pad = 0 ;
	int cursol = menu_cursol[ menu_mode ] ;

	emc = &emu_config[EMU_MD] ;
	//x = MENU_FRAME_SPACE * 4 ;
	x = PSP_SCREEN_WIDTH  * 10 / 100 ;
	//y = PSP_SCREEN_HEIGHT * 18 / 100 ; // MENU_FRAME_SPACE * 4 + 4 ;
	y = MENU_ITEM_TOP + MENU_TEXT_SPACE ;

	DrawMenu( x,y, SZ_6PAD[ (emc->pad_option & PAD_OPT_6PAD) ? 1 : 0 ], num++, menu_cursol[ menu_mode ] ) ;
	y += MENU_TEXT_SPACE * 2 ;

	for( m=0; m < 4; m++ )
	{
		y = DrawMenuItem( x,y, 
			SZ_KEY_MENU [pad], 
			SZ_KEY_ITEMS[ emc->pad_cfg[pad] ],
			num, menu_cursol[ menu_mode ] ) ;
		num++ ;
		pad++ ;
	}

	if( emc->pad_cfg[MD_PAD_CFG_R] )
	{
		for( m=0; m < 4; m++ )
		{
			y = DrawMenuItem( x,y, SZ_KEY_MENU[pad], SZ_NOCONTROL, num, menu_cursol[ menu_mode ] ) ;
			num++ ;
			pad++ ;
		}
		y += MENU_TEXT_SPACE ;

		for( m=0; m < 2; m++ )
		{
			y = DrawMenuItem( x,y, SZ_KEY_MENU[pad], 
					SZ_KEY_ITEMS[ emc->pad_cfg[pad] ], num, menu_cursol[ menu_mode ] ) ;
			num++ ;
			pad++ ;
		}

		for( m=0; m < 2; m++ )
		{
			y = DrawMenuItem( x,y, SZ_KEY_MENU[pad], SZ_NOCONTROL, num, menu_cursol[ menu_mode ] ) ;
			num++ ;
			pad++ ;
		}

		x = PSP_SCREEN_WIDTH  * 55 / 100 ;
		//y = PSP_SCREEN_HEIGHT * 18 / 100 + MENU_TEXT_SPACE * 2 ; // MENU_FRAME_SPACE * 4 + 4 ;
		y = MENU_ITEM_TOP + MENU_TEXT_SPACE * 3 ;

		for( m=0; m < 4; m++ )
		{
			y = DrawMenuItem( x,y, SZ_KEY_MENU[pad], 
					SZ_KEY_ITEMS[ emc->pad_cfg[pad] ], num, menu_cursol[ menu_mode ] ) ;
			num++ ;
			pad++ ;
		}

		for( m=0; m < 4; m++ )
		{
			y = DrawMenuItem( x,y, SZ_KEY_MENU[pad], SZ_NOCONTROL, num, menu_cursol[ menu_mode ] ) ;
			num++ ;
			pad++ ;
		}
	}
	else
	{
		for( m=0; m < 4; m++ )
		{
			y = DrawMenuItem( x,y, 
				SZ_KEY_MENU [pad], 
				SZ_KEY_ITEMS[ emc->pad_cfg[pad] ],
				num, menu_cursol[ menu_mode ] ) ;
			num++ ;
			pad++ ;
		}
		y += MENU_TEXT_SPACE ;

		for( m=0; m < 4; m++ )
		{
			y = DrawMenuItem( x,y, SZ_KEY_MENU[pad], 
					SZ_KEY_ITEMS[ emc->pad_cfg[pad] ], num, menu_cursol[ menu_mode ] ) ;
			num++ ;
			pad++ ;
		}

		x = PSP_SCREEN_WIDTH  * 55 / 100 ;
		//y = PSP_SCREEN_HEIGHT * 18 / 100 + MENU_TEXT_SPACE * 2 ; // MENU_FRAME_SPACE * 4 + 4 ;
		y = MENU_ITEM_TOP + MENU_TEXT_SPACE * 3 ;

		for( m=0; m < 8; m++ )
		{
			y = DrawMenuItem( x,y, SZ_KEY_MENU[pad], 
					SZ_KEY_ITEMS[ emc->pad_cfg[pad] ], num, menu_cursol[ menu_mode ] ) ;
			num++ ;
			pad++ ;
		}
	}

	y += MENU_TEXT_SPACE ;

	for( m=0; m < 2; m++ )
	{
		y = DrawMenuItem( x,y, SZ_KEY_MENU[pad], 
				SZ_KEY_ITEMS[ emc->pad_cfg[pad] ], num, menu_cursol[ menu_mode ] ) ;
		num++ ;
		pad++ ;
	}

	y += MENU_TEXT_SPACE ;

	//x = MENU_FRAME_SPACE * 4 ;
	DrawMenu( x,y, SZ_SWAP_ANALOG[ emc->analog_mode ], num, menu_cursol[ menu_mode ] ) ;

	if( emc->pad_cfg[MD_PAD_CFG_R] )
	{
		x = MENU_MESSAGE_LEFT ;
		y = MENU_MESSAGE_TOP - MENU_TEXT_SPACE * 3 ;
		DrawMenuText( x,y, SZ_NOTUSE_R_JPN, COLOR_MSG, COLOR_SHADOW, emsc->shadow_on ) ;
		y += MENU_TEXT_SPACE ;
		DrawMenuText( x,y, SZ_NOTUSE_R_ENG, COLOR_MSG, COLOR_SHADOW, emsc->shadow_on ) ;
	}

	x = MENU_MESSAGE_LEFT ;
	y = MENU_MESSAGE_TOP  ;
	DrawMenuText( x,y, SZ_MENU_MESSAGE_KEY, COLOR_MSG, COLOR_SHADOW, emsc->shadow_on ) ;
}

void Menu_DrawMD_System()
{
	int x, y, n, m, sel = 0 ;
	int cursol = menu_cursol[ menu_mode ] ;
	char buff[4] ;

	buff[0] = '0' +  emsc->color_grad / 100      ;
	buff[1] = '0' + (emsc->color_grad / 10) % 10 ; 
	buff[2] = '0' +  emsc->color_grad % 10       ;
	buff[3] = 0 ;

	emc = &emu_config[EMU_MD] ;
	x = MENU_ITEM_LEFT ;
	y = MENU_ITEM_TOP  ;

	DrawMenuItem( x,y+MENU_TEXT_SPACE*MENU_CUR_SYS_KEY_MENU, SZ_MENU_KEY, SZ_MENU_KEYS[ emsc->key_menu ], sel++, menu_cursol[ menu_mode ] ) ; 
	y += MENU_TEXT_SPACE ;

	DrawMenu( x,y+MENU_TEXT_SPACE*MENU_CUR_SYS_KEY_R, SZ_KEY_REPEAT[ emsc->key_repeat - KEY_REPEAT_FAST ], sel++, menu_cursol[ menu_mode ] ) ;
	y += MENU_TEXT_SPACE ;

	DrawMenu( x,y+MENU_TEXT_SPACE*MENU_CUR_SYS_CLOCK, SZ_SHOW_CLOCK[ (emsc->option_flags & OPTION_SHOW_CLOCK) ? 1 : 0 ], sel++, menu_cursol[ menu_mode ] ) ;
	DrawMenu( x,y+MENU_TEXT_SPACE*MENU_CUR_SYS_BATT1, SZ_SHOW_BATT1[ (emsc->option_flags & OPTION_SHOW_BATT1) ? 1 : 0 ], sel++, menu_cursol[ menu_mode ] ) ;
	DrawMenu( x,y+MENU_TEXT_SPACE*MENU_CUR_SYS_BATT2, SZ_SHOW_BATT2[ (emsc->option_flags & OPTION_SHOW_BATT2) ? 1 : 0 ], sel++, menu_cursol[ menu_mode ] ) ;
	y += MENU_TEXT_SPACE ;

	DrawMenu( x,y+MENU_TEXT_SPACE*MENU_CUR_SYS_STATE_D, SZ_SHOW_STATE_DATE[ (emsc->option_flags & OPTION_SHOW_DATE ) ? 0 : 1 ], sel++, menu_cursol[ menu_mode ] ) ;
	DrawMenu( x,y+MENU_TEXT_SPACE*MENU_CUR_SYS_STATE_L, SZ_SHOW_STATE_LOAD[ (emsc->option_flags & OPTION_STATE_LOAD) ? 1 : 0 ], sel++, menu_cursol[ menu_mode ] ) ;
	DrawMenu( x,y+MENU_TEXT_SPACE*MENU_CUR_SYS_STATE_S, SZ_SHOW_STATE_SAVE[ (emsc->option_flags & OPTION_STATE_SAVE) ? 0 : 1 ], sel++, menu_cursol[ menu_mode ] ) ;
	y += MENU_TEXT_SPACE ;

	DrawMenu ( x,y+MENU_TEXT_SPACE*MENU_CUR_SYS_SHADOW, SZ_SHADOW_ON[ emsc->shadow_on ], sel++, menu_cursol[ menu_mode ] ) ;
	//y += MENU_TEXT_SPACE ;

	y = DrawMenu     ( x,y+MENU_TEXT_SPACE*MENU_CUR_SYS_BGMODE, SZ_BG_MODE[ emsc->bg_mode ], sel++, menu_cursol[ menu_mode ] ) ;
	y += MENU_TEXT_SPACE/2 ;
	y = DrawMenuColor( x,y, SZ_COLOR_SIMPLE, emsc->color_bg    , sel, menu_cursol[ menu_mode ] ) ; sel+=3 ;
	y += MENU_TEXT_SPACE/2 ;
	y = DrawMenuItem ( x,y, SZ_COLOR_GRAD, buff, sel++, menu_cursol[ menu_mode ] ) ;

	x = PSP_SCREEN_WIDTH / 2  - MENU_TEXT_SPACE * 3 ;
	y = MENU_ITEM_TOP ;
	y = DrawMenuColor( x,y, SZ_COLOR_TITLE , emsc->color_title , sel, menu_cursol[ menu_mode ] ) ; sel+=3 ; y += 4 ;
	y = DrawMenuColor( x,y, SZ_COLOR_SELECT, emsc->color_select, sel, menu_cursol[ menu_mode ] ) ; sel+=3 ; y += 4 ;
	y = DrawMenuColor( x,y, SZ_COLOR_MSGBOX, emsc->color_msgbox, sel, menu_cursol[ menu_mode ] ) ; sel+=3 ; y += 4 ;

	y = DrawMenuColor( x,y, SZ_COLOR_MSG   , emsc->color_msg   , sel, menu_cursol[ menu_mode ] ) ; sel+=3 ; y += 4 ;
	y = DrawMenuColor( x,y, SZ_COLOR_NORMAL, emsc->color_text  , sel, menu_cursol[ menu_mode ] ) ; sel+=3 ; y += 4 ;
	y = DrawMenuColor( x,y, SZ_COLOR_SHADOW, emsc->color_shadow, sel, menu_cursol[ menu_mode ] ) ; sel+=3 ;

	x = MENU_MESSAGE_LEFT ;
	y = MENU_MESSAGE_TOP  ;
	DrawMenuText( x,y, SZ_MENU_MESSAGE_SYS, COLOR_MSG, COLOR_SHADOW, emsc->shadow_on ) ;
}

inline void menu_update_file_path( int sel )
{
	strcpy( path_rom, path_last[emu_mode] ) ;
	strcat( path_rom, menu_files[ sel ].name ) ;

	if( strlen( path_cfg ) > 0 ){ EmuCfg_Save( emc, path_cfg ) ; }
	make_cfg_path( menu_files[ sel ].name ) ;

	if( strlen( path_cheat ) > 0 ){ EmuCheat_Save( emct, path_cheat ) ; }
	make_cheat_path( path_cheat, menu_files[ sel ].name ) ;

	if( strlen( path_sram ) > 0 && emc->autosram )
	{ 
		if( EmuCore_IsSaveSRAM() )
		{
			Menu_MessageBox( SZ_MESSAGE_ON_SAVE, 3, MSGBOX_NOBUTTON ) ;
			EmuCore_SaveSRAM ( path_sram ) ; 
		}
	}
	make_sram_path( path_sram, menu_files[ sel ].name ) ;

	if( strlen( path_state ) > 0 )
	{ 
		if( EmuState_GetSaveCount( &emu_state ) > 0 )
		{
			Menu_MessageBox( SZ_MESSAGE_ON_SAVE, 3, MSGBOX_NOBUTTON ) ;
		}
		EmuState_Save( &emu_state, path_state ) ; 
	}
	make_state_path( path_state, menu_files[ sel ].name ) ;
}

void Menu_UpdateButtons( unsigned long buttons_new, unsigned long buttons_old )
{
	if( buttons_new & (CTRL_UP | CTRL_DOWN | CTRL_LEFT | CTRL_RIGHT) )
	{
		if( menu_key_repeat_mode )
		{
			if( (sceKernelLibcClock() - menu_key_repeat) > (TM_1FRAME*2) * emsc->key_repeat )
			{ 
				buttons_new &= ~(CTRL_UP | CTRL_DOWN | CTRL_LEFT | CTRL_RIGHT) ;
			}
		}
		else if( (sceKernelLibcClock() - menu_key_repeat) > TM_1FRAME * 10 )
		{
			if( buttons_old ){ menu_key_repeat_mode = 1 ; }
		}
	}
	else
	{
		menu_key_repeat_mode = 0 ;
	}

	if( buttons_new != buttons_old )
	{
		menu_key_repeat = sceKernelLibcClock() ;

		if( (buttons_old & CTRL_CIRCLE) && ((buttons_new & CTRL_CIRCLE)==0) )
		{
			// Åõ
			switch( menu_mode )
			{
			case MENU_MODE_MD_LOAD :
				if( strcmp( menu_files[ menu_cursol[ menu_mode ] ].name, ".." ) == 0 )
				{
					char* p = strrchr( path_last[0], '/' ) ;
					if( p )
					{
						*p = 0 ;
						p = strrchr( path_last[0], '/' ) ;
						if( p ){ *(p+1) = 0 ; }
					}

					Menu_UpdateFileList( path_last[0] ) ; 
				}
				else if( menu_files[ menu_cursol[ menu_mode ] ].type & PSP_FILE_TYPE_DIR )
				{
					strcat( path_last[0], menu_files[ menu_cursol[ menu_mode ] ].name ) ;
					Menu_UpdateFileList( path_last[0] ) ; 
				}
				else
				{
					menu_update_file_path( menu_cursol[ menu_mode ] ) ;

					menu_files_sel   = menu_cursol[ menu_mode ] ;
					menu_files_sel_s = menu_files_sel ;

					Menu_MessageBox( SZ_MESSAGE_LOADING, 1, MSGBOX_NOBUTTON ) ;

					EmuState_Load( &emu_state, path_state ) ;

					menu_cmd = MENU_CMD_LOAD_MD ;
				}
				break ;
			case MENU_MODE_MD_STATE :
				if( (menu_cursol[ menu_mode ] >= MENU_CUR_STATE_SLOT0)
				  &&(menu_cursol[ menu_mode ] <= MENU_CUR_STATE_SLOT9)
				  &&( menu_files[ menu_files_sel_s ].type & PSP_FILE_TYPE_FILE )
				  )
				{
					menu_load_state_slot = menu_cursol[ menu_mode ] - MENU_CUR_STATE_SLOT0 ;

					if( menu_files_sel == menu_files_sel_s )
					{
						if( emu_state.slot[menu_load_state_slot].flag & STATE_FLAG_ACTIVE )
						{
							if( (emsc->option_flags & OPTION_STATE_LOAD) != 0 )
							{
								SZ_MESSAGE_LOADSTATE[0][4] = '0' + menu_load_state_slot ;

								if( Menu_MessageBox( SZ_MESSAGE_LOADSTATE, 2, MSGBOX_OKCANCEL ) )
								{
									menu_cmd = MENU_CMD_LOAD_MD_STATE ;
								}
							}
							else
							{
								menu_cmd = MENU_CMD_LOAD_MD_STATE ;
							}
						}
					}
					else
					{
						menu_update_file_path( menu_files_sel_s ) ;

						menu_files_sel                   = menu_files_sel_s ;
						menu_cursol[ MENU_MODE_MD_LOAD ] = menu_files_sel_s ;

						EmuState_Copy( &emu_state, &menu_emu_state ) ;

						menu_cmd = MENU_CMD_LOAD_MD_ROM_S ;
					}
				}
				break ;
			case MENU_MODE_MD_CHEAT :
				if( menu_cursol[ menu_mode ] != MENU_CUR_CHEAT_ON )
				{
					int num = menu_cursol[ menu_mode ] - MENU_CUR_CHEAT_MIN ;
					menu_cheat_addr = emct->addr[ num ] ;
					menu_cheat_data = emct->data[ num ] ;
					strcpy( menu_cheat_name, emct->name[ num ] );

					if( Menu_MessageBox( NULL, 0, MSGBOX_CHEAT_PATCH ) )
					{
						emct->modifyed    = 1 ;
						emct->flag[ num ] = 1 ;
						emct->addr[ num ] = menu_cheat_addr ;
						emct->data[ num ] = menu_cheat_data ;
						memset( emct->code[ num ], 0, MAX_CHEAT_GG ) ;
					}
				}
				break ;
			}
		}
		else if( (buttons_old & CTRL_LTRIGGER) && ((buttons_new & CTRL_LTRIGGER)==0) )
		{
			// Çk
			if( (menu_mode == MENU_MODE_MD_STATE) && (menu_files_sel >= 0) ){ menu_files_sel_s = menu_files_sel ; }

			menu_mode-- ;
			if( menu_mode < 0 ){ menu_mode = MAX_MENU_MODE -1 ; }

			if( menu_mode == MENU_MODE_MD_LOAD )
			{
				Menu_UpdateFileList( path_last[0] ) ; 
			}
		}
		else if( (buttons_old & CTRL_RTRIGGER) && ((buttons_new & CTRL_RTRIGGER)==0) )
		{
			// Çq
			if( (menu_mode == MENU_MODE_MD_STATE) && (menu_files_sel >= 0) ){ menu_files_sel_s = menu_files_sel ; }

			menu_mode++ ;
			if( menu_mode > (MAX_MENU_MODE-1) ){ menu_mode = 0 ; }
			
			if( menu_mode == MENU_MODE_MD_LOAD )
			{
				Menu_UpdateFileList( path_last[0] ) ; 
			}
		}
		else if( (buttons_old & CTRL_CROSS) && ((buttons_new & CTRL_CROSS)==0) )
		{
			// Å~
			menu_files_sel_s = menu_files_sel ;
			menu_cmd  = MENU_CMD_CANCEL ;
		}
		else if( (buttons_old & CTRL_TRIANGLE) && ((buttons_new & CTRL_TRIANGLE)==0) )
		{
			// Å¢
			switch( menu_mode )
			{
			case MENU_MODE_MD_LOAD  :
				if( EmuCore_IsSaveSRAM() )
				{
					if( Menu_MessageBox( SZ_MESSAGE_SAVESRAM, 2, MSGBOX_OKCANCEL ) )
					{
						Menu_MessageBox( SZ_MESSAGE_ON_SAVE, 3, MSGBOX_NOBUTTON ) ;
						EmuCore_SaveSRAM ( path_sram ) ; 
					}
				}
				break ;
			case MENU_MODE_MD_CONFIG  : 
				if( Menu_MessageBox( SZ_MESSAGE_SAVEDEFAULT, 2, MSGBOX_OKCANCEL ) )
				{
					EmuCfg_SetDefault_Std( emc ) ;
					EmuCfg_SaveDefault( path_exec ) ;
				}
				break ;
			case MENU_MODE_MD_KEY     :
				if( Menu_MessageBox( SZ_MESSAGE_SAVEDEFAULT, 2, MSGBOX_OKCANCEL ) )
				{
					EmuCfg_SetDefault_Key( emc ) ;
					EmuCfg_SaveDefault( path_exec ) ;
				}
				break ;
			case MENU_MODE_MD_STATE :
				if( (menu_cursol[ menu_mode ] >= MENU_CUR_STATE_SLOT0) 
				 && (menu_cursol[ menu_mode ] <= MENU_CUR_STATE_SLOT9) )
				{
					if( menu_files_sel == menu_files_sel_s )
					{
						SZ_MESSAGE_SAVESTATE[0][  4 ] = '0' + menu_cursol[ menu_mode ] - MENU_CUR_STATE_SLOT0 ;
						//SZ_MESSAGE_SAVESTATE[1][ 20 ] = '0' + menu_cursol[ menu_mode ] - MENU_CUR_STATE_SLOT0 ;

						if( ((emsc->option_flags & OPTION_STATE_SAVE) != 0)
							|| Menu_MessageBox( SZ_MESSAGE_SAVESTATE, 2, MSGBOX_OKCANCEL ) )
						{
							EmuState_SaveSlot( &emu_state, 
								menu_cursol[ menu_mode ] - MENU_CUR_STATE_SLOT0, 
								&state_temp_image[0][0],
								path_state ) ;
						}
					}
				}
				else if( menu_cursol[ menu_mode ] != MENU_CUR_STATE_ROM )
				{
					if( Menu_MessageBox( SZ_MESSAGE_SAVEDEFAULT, 2, MSGBOX_OKCANCEL ) )
					{
						EmuStateCfg_SetDefault ( &emu_state.cfg ) ;
						EmuStateCfg_SaveDefault( path_exec ) ;
					}
				}
				break ;
			case MENU_MODE_MD_CHEAT :
				if( menu_cursol[ menu_mode ] != MENU_CUR_CHEAT_ON )
				{
					int num = menu_cursol[ menu_mode ] - MENU_CUR_CHEAT_MIN ;
					menu_cheat_addr = emct->addr[ num ] ;
					menu_cheat_data = emct->data[ num ] ;
					strcpy( menu_cheat_name, emct->name[ num ] );
					if( strlen( emct->code[ num ] ) == MAX_CHEAT_GG )
						{ strcpy( menu_cheat_gg, emct->code[ num ] ); }
					else{ strcpy( menu_cheat_gg, SZ_CHEAT_GG_DEFAULT  ); }

				_Retry :
					if( Menu_MessageBox( NULL, 0, MSGBOX_CHEAT_GG ) )
					{
						menu_cheat_gg[4] = '-' ;

						if( EmuCheat_CalcGG( menu_cheat_gg, &menu_cheat_addr, &menu_cheat_data ) )
						{
							emct->modifyed    = 1 ;
							emct->flag[ num ] = 1 ;
							emct->addr[ num ] = menu_cheat_addr ;
							emct->data[ num ] = menu_cheat_data ;
							strcpy( emct->code[ num ], menu_cheat_gg );
							emct->code[ num ][4] = '-' ;
							//EmuCheat_CheckValue( emct ) ;
						}
						else
						{
							if( Menu_MessageBox( SZ_MESSAGE_CHEAT_ERROR, 3, MSGBOX_OKCANCEL ) )
							{
								goto _Retry ;
							}
						}
					}
				}
				break ;
			}
		}
		else if( (buttons_old & CTRL_SQUARE) && ((buttons_new & CTRL_SQUARE)==0) )
		{
			// Å†
			switch( menu_mode )
			{
			case MENU_MODE_MD_LOAD :
				if( strcmp( menu_files[ menu_cursol[ menu_mode ] ].name, ".." ) == 0 )
				{
				}
				else if( menu_files[ menu_cursol[ menu_mode ] ].type & PSP_FILE_TYPE_DIR )
				{
				}
				else
				{
				}
				break ;

			case MENU_MODE_MD_STATE :
				if( menu_files_sel == menu_files_sel_s )
				{
					if( (menu_cursol[ menu_mode ] >= MENU_CUR_STATE_SLOT0) 
					 && (menu_cursol[ menu_mode ] <= MENU_CUR_STATE_SLOT9) )
					{
						if( buttons_new & CTRL_SELECT )
						{
							SZ_MESSAGE_DELETESTATE[0][4] = '0' + menu_cursol[ menu_mode ] - MENU_CUR_STATE_SLOT0 ;

							if( Menu_MessageBox( SZ_MESSAGE_DELETESTATE, 2, MSGBOX_OKCANCEL ) )
							{
								EmuState_DeleteSlot( &emu_state, 
									menu_cursol[ menu_mode ] - MENU_CUR_STATE_SLOT0, 
									path_state ) ;
							}
						}
						else
						{
							SZ_MESSAGE_SAVESTATE_MSD[0][ 4 ] = '0' + menu_cursol[ menu_mode ] - MENU_CUR_STATE_SLOT0 ;
							//SZ_MESSAGE_SAVESTATE_MSD[1][ 6 ] = '0' + menu_cursol[ menu_mode ] - MENU_CUR_STATE_SLOT0 ;

							if( Menu_MessageBox( SZ_MESSAGE_SAVESTATE_MSD, 2, MSGBOX_OKCANCEL ) )
							{
								Menu_MessageBox( SZ_MESSAGE_ON_SAVE, 3, MSGBOX_NOBUTTON ) ;

								emu_state_ext[3] = '0' + menu_cursol[ menu_mode ] - MENU_CUR_STATE_SLOT0 ;
								strcpy( emu_state_path, path_state    ) ;
								strcat( emu_state_path, emu_state_ext ) ;

								EmuStateSlot_Save_MSD( 
									&emu_state.slot[ menu_cursol[ menu_mode ] - MENU_CUR_STATE_SLOT0 ], 
									emu_state_path ) ;
							}
						}
					}
				}
				break ;
			case MENU_MODE_MD_CHEAT :
				if( menu_cursol[ menu_mode ] != MENU_CUR_CHEAT_ON )
				{
					int num = menu_cursol[ menu_mode ] - MENU_CUR_CHEAT_MIN ;
					strcpy( menu_cheat_name, emct->name[ num ] );

					if( Menu_MessageBox( NULL, 0, MSGBOX_CHEAT_NAME ) )
					{
						strcpy( emct->name[ num ], menu_cheat_name );
						emct->modifyed = 1 ;
					}
					break ; 
				}
			}
		}
		else if( (buttons_old & CTRL_UP) && ((buttons_new & CTRL_UP)==0) )
		{
			// Å™
			if( menu_mode == MENU_MODE_MD_STATE )
			{
				if( (menu_cursol[ menu_mode ] >= MENU_CUR_STATE_SLOT0) 
				 && (menu_cursol[ menu_mode ] <= MENU_CUR_STATE_SLOT4) )
				{
					menu_cursol[ menu_mode ] = MENU_CUR_STATE_ROM ;
				}
				else if( (menu_cursol[ menu_mode ] >= MENU_CUR_STATE_SLOT5) 
					  && (menu_cursol[ menu_mode ] <= MENU_CUR_STATE_SLOT9) )
				{
					menu_cursol[ menu_mode ] -= 5 ;
				}
				else if( menu_cursol[ menu_mode ] == MENU_CUR_STATE_MODE1 )
				{
					menu_cursol[ menu_mode ] = MENU_CUR_STATE_SLOT7 ;
				}
				else
				{
					menu_cursol[ menu_mode ] -- ;
					if( menu_cursol[ menu_mode ] < 0 ){ menu_cursol[ menu_mode ] = menu_cursol_max[ menu_mode ] -1 ; } //0 ; }
				}
			}
			else
			{
				menu_cursol[ menu_mode ] -- ;
				if( menu_cursol[ menu_mode ] < 0 ){ menu_cursol[ menu_mode ] = menu_cursol_max[ menu_mode ] -1 ; } //0 ; }
			}
		}
		else if( (buttons_old & CTRL_DOWN) && ((buttons_new & CTRL_DOWN)==0) )
		{
			// Å´
			if( menu_mode == MENU_MODE_MD_STATE )
			{
				if( (menu_cursol[ menu_mode ] >= MENU_CUR_STATE_SLOT0) 
				 && (menu_cursol[ menu_mode ] <= MENU_CUR_STATE_SLOT4) )
				{
					menu_cursol[ menu_mode ] += 5 ;
				}
				else if( (menu_cursol[ menu_mode ] >= MENU_CUR_STATE_SLOT5) 
					  && (menu_cursol[ menu_mode ] <= MENU_CUR_STATE_SLOT9) )
				{
					menu_cursol[ menu_mode ] = MENU_CUR_STATE_MODE1 ;
				}
				else if( menu_cursol[ menu_mode ] == MENU_CUR_STATE_ROM )
				{
					menu_cursol[ menu_mode ] = MENU_CUR_STATE_SLOT2 ;
				}
				else
				{
					menu_cursol[ menu_mode ] ++ ;
					if( menu_cursol[ menu_mode ] >= menu_cursol_max[ menu_mode ] ){ menu_cursol[ menu_mode ] = 0 ; }
				}
			}
			else
			{
				menu_cursol[ menu_mode ] ++ ;
				if( menu_cursol[ menu_mode ] >= menu_cursol_max[ menu_mode ] ){ menu_cursol[ menu_mode ] = 0 ; }
			}
		}
		else if( (buttons_old & CTRL_LEFT) && ((buttons_new & CTRL_LEFT)==0) )
		{
			// Å©
			switch( menu_mode )
			{
			case MENU_MODE_MD_LOAD :
				//if( strcmp( menu_files[ menu_cursol[ menu_mode ] ].name, ".." ) == 0 )
				if( strcmp( path_last[0], "ms0:/" ) != 0 )
				{
					char* p = strrchr( path_last[0], '/' ) ;
					if( p )
					{
						*p = 0 ;
						p = strrchr( path_last[0], '/' ) ;
						if( p ){ *(p+1) = 0 ; }
					}

					Menu_UpdateFileList( path_last[0] ) ; 
				}
				break ;
			case MENU_MODE_MD_CHEAT   :
				if( menu_cursol[ menu_mode ] == MENU_CUR_CHEAT_ON )
				{
					emct->all_on = emct->all_on ? 0 : 1 ;
				}
				else
				{
					emct->modifyed = 1 ;
					emct->flag[ menu_cursol[ menu_mode ] -1 ] = emct->flag[ menu_cursol[ menu_mode ] -1 ] ? 0 : 1 ;
					EmuCheat_CheckValue( emct ) ;
				}
				break ;
			case MENU_MODE_MD_CONFIG  :
				if( emc )
				{
					switch( menu_cursol[ menu_mode ] )
					{
					case MENU_CUR_CPU        : emc->cpu_freq--                       ; break ;

#ifdef _EMU_CLOCK_ADJ  ////////////////////
					case MENU_CUR_EMU_CLOCK  : emc->emu_clock-=2                     ; break ;
					case MENU_CUR_Z80_CLOCK  : emc->z80_clock-=2                     ; break ;
#endif // _EMU_CLOCK_ADJ //////////////////

					case MENU_CUR_CORE       : emc->show_flags ^= FLAG_CORE          ; break ; 
					case MENU_CUR_Z80ASYNC   : emc->show_flags ^= FLAG_Z80ASYNC      ; break ; 
					case MENU_CUR_AUTOSRAM   : emc->autosram = emc->autosram ? 0 : 1 ; break ;
					case MENU_CUR_COUNTRY    : emc->country--                        ; break ;
					case MENU_CUR_CHECKSUM   : emc->show_flags ^= FLAG_CHECKSUM      ; break ; 
					case MENU_CUR_2CON       : emc->comm_mode--                      ; break ; 
					case MENU_CUR_MAX_FPS    : emc->max_fps-=2                       ; break ;
					case MENU_CUR_SKIP       : emc->skip_count--                     ; break ;
					case MENU_CUR_SKIP_MODE  : emc->skip_mode--                      ; break ;
					case MENU_CUR_SKIP_T     : emc->skip_turbo--                     ; break ;
					case MENU_CUR_SCREEN     : emc->screen_mode--                    ; break ;
					case MENU_CUR_VSYNC      : emc->show_flags ^= SHOW_VSYNC         ; break ; 
					case MENU_CUR_SHOW_FPS   : emc->show_fps--                       ; break ; 
					case MENU_CUR_SHOW_BG    : emc->show_flags ^= SHOW_BG            ; break ; 
					case MENU_CUR_PALETTE    : emc->palette_mode--                   ; break ; 
					case MENU_CUR_SOUND_ON   : emc->sound_on--                       ; break ;
					case MENU_CUR_SOUND_VOL  : emc->sound_vol--                      ; break ;
					case MENU_CUR_SOUND_YMCH : emc->sound_flag ^= MD_SND_YMCH        ; break ;
					case MENU_CUR_SOUND_PSG  : emc->sound_flag ^= MD_SND_PSG         ; break ;
					case MENU_CUR_SOUND_DAC  : emc->sound_flag ^= MD_SND_DAC         ; break ;
					case MENU_CUR_SOUND_TB   : emc->sound_flag ^= MD_SND_TURBO       ; break ;
					}
				
					EmuCfg_CheckValue( emc ) ;
				}
				break ;
			case MENU_MODE_MD_KEY :
				if( emc )
				{
					if( menu_cursol[ menu_mode ] == MENU_CUR_SWAP_ANALOG )
					{
						emc->analog_mode-- ;
					}
					else if( menu_cursol[ menu_mode ] == MENU_CUR_6PAD )
					{
						emc->pad_option ^= PAD_OPT_6PAD ;
					}
					else
					{
						emc->pad_cfg[ menu_cursol[ menu_mode ] - MENU_CUR_CIRCLE ] -- ;
					}
					EmuCfg_CheckValue( emc ) ;
				}
				break ;
			case MENU_MODE_MD_STATE :
				{
					if( (menu_cursol[ menu_mode ] >= MENU_CUR_STATE_SLOT0) 
					 && (menu_cursol[ menu_mode ] <= MENU_CUR_STATE_SLOT4) )
					{
						menu_cursol[ menu_mode ]-- ;
						if( menu_cursol[ menu_mode ] < MENU_CUR_STATE_SLOT0 )
						{   menu_cursol[ menu_mode ] = MENU_CUR_STATE_SLOT4 ; }
					}
					else if( (menu_cursol[ menu_mode ] >= MENU_CUR_STATE_SLOT5) 
					      && (menu_cursol[ menu_mode ] <= MENU_CUR_STATE_SLOT9) )
					{
						menu_cursol[ menu_mode ]-- ;
						if( menu_cursol[ menu_mode ] < MENU_CUR_STATE_SLOT5 )
						{   menu_cursol[ menu_mode ] = MENU_CUR_STATE_SLOT9 ; }
					}
					else
					{
						if( menu_files_sel_s == menu_files_sel )
						{
							switch( menu_cursol[ menu_mode ] )
							{
							case MENU_CUR_STATE_ROM   : menu_files_sel_s-- ;
								if( menu_files_sel_s < 0 ){ menu_files_sel_s = menu_files_num -1 ; }
								if( menu_files_sel_s != menu_files_sel ){ is_menu_emu_state_load = 0 ; }
								break ;
							case MENU_CUR_STATE_MODE1 : emu_state.cfg.save_mode1--     ; break ;
							case MENU_CUR_STATE_MODE2 : emu_state.cfg.save_mode2--     ; break ;
							case MENU_CUR_STATE_LOOP  : emu_state.cfg.loop_mode --     ; break ;
							case MENU_CUR_STATE_INC   : emu_state.cfg.slot_inc--       ; break ;
							case MENU_CUR_STATE_SEL   : emu_state.cfg.slot_sel--       ; break ;
							}
							EmuStateCfg_CheckValue( &emu_state.cfg ) ;
						}
						else
						{
							switch( menu_cursol[ menu_mode ] )
							{
							case MENU_CUR_STATE_ROM   : menu_files_sel_s-- ;
								if( menu_files_sel_s < 0 ){ menu_files_sel_s = menu_files_num -1 ; }
								if( menu_files_sel_s != menu_files_sel ){ is_menu_emu_state_load = 0 ; }
								break ;
							case MENU_CUR_STATE_MODE1 : menu_emu_state.cfg.save_mode1--     ; break ;
							case MENU_CUR_STATE_MODE2 : menu_emu_state.cfg.save_mode2--     ; break ;
							case MENU_CUR_STATE_LOOP  : menu_emu_state.cfg.loop_mode --     ; break ;
							case MENU_CUR_STATE_INC   : menu_emu_state.cfg.slot_inc--       ; break ;
							case MENU_CUR_STATE_SEL   : menu_emu_state.cfg.slot_sel--       ; break ;
							}
							EmuStateCfg_CheckValue( &menu_emu_state.cfg ) ;
						}
					}
				}
				break ;
			case MENU_MODE_MD_SYSTEM :
				if( emsc )
				{
					switch( menu_cursol[ menu_mode ] )
					{
					case MENU_CUR_SYS_KEY_MENU : emsc->key_menu--                         ; break ;
					case MENU_CUR_SYS_KEY_R    : emsc->key_repeat++                       ; break ;
					case MENU_CUR_SYS_CLOCK    : emsc->option_flags ^= OPTION_SHOW_CLOCK  ; break ;
					case MENU_CUR_SYS_BATT1    : emsc->option_flags ^= OPTION_SHOW_BATT1  ; break ;
					case MENU_CUR_SYS_BATT2    : emsc->option_flags ^= OPTION_SHOW_BATT2  ; break ;
					case MENU_CUR_SYS_STATE_D  : emsc->option_flags ^= OPTION_SHOW_DATE   ; break ;
					case MENU_CUR_SYS_STATE_L  : emsc->option_flags ^= OPTION_STATE_LOAD  ; break ;
					case MENU_CUR_SYS_STATE_S  : emsc->option_flags ^= OPTION_STATE_SAVE  ; break ;
					case MENU_CUR_SYS_BGMODE   : emsc->bg_mode--                          ; break ;
					case MENU_CUR_SYS_BG_R     : calc_color( &emsc->color_bg    , 0, -1 ) ; break ;
					case MENU_CUR_SYS_BG_G     : calc_color( &emsc->color_bg    , 1, -1 ) ; break ;
					case MENU_CUR_SYS_BG_B     : calc_color( &emsc->color_bg    , 2, -1 ) ; break ;
					case MENU_CUR_SYS_GRAD     : emsc->color_grad--                       ; break ;
					case MENU_CUR_SYS_SHADOW   : emsc->shadow_on = emsc->shadow_on ? 0 : 1; break ;
					case MENU_CUR_SYS_TITLE_R  : calc_color( &emsc->color_title , 0, -1 ) ; break ;
					case MENU_CUR_SYS_TITLE_G  : calc_color( &emsc->color_title , 1, -1 ) ; break ;
					case MENU_CUR_SYS_TITLE_B  : calc_color( &emsc->color_title , 2, -1 ) ; break ;
					case MENU_CUR_SYS_SEL_R    : calc_color( &emsc->color_select, 0, -1 ) ; break ;
					case MENU_CUR_SYS_SEL_G    : calc_color( &emsc->color_select, 1, -1 ) ; break ;
					case MENU_CUR_SYS_SEL_B    : calc_color( &emsc->color_select, 2, -1 ) ; break ;
					case MENU_CUR_SYS_MSGBOX_R : calc_color( &emsc->color_msgbox, 0, -1 ) ; break ;
					case MENU_CUR_SYS_MSGBOX_G : calc_color( &emsc->color_msgbox, 1, -1 ) ; break ;
					case MENU_CUR_SYS_MSGBOX_B : calc_color( &emsc->color_msgbox, 2, -1 ) ; break ;
					case MENU_CUR_SYS_MSG_R    : calc_color( &emsc->color_msg   , 0, -1 ) ; break ;
					case MENU_CUR_SYS_MSG_G    : calc_color( &emsc->color_msg   , 1, -1 ) ; break ;
					case MENU_CUR_SYS_MSG_B    : calc_color( &emsc->color_msg   , 2, -1 ) ; break ;
					case MENU_CUR_SYS_TEXT_R   : calc_color( &emsc->color_text  , 0, -1 ) ; break ;
					case MENU_CUR_SYS_TEXT_G   : calc_color( &emsc->color_text  , 1, -1 ) ; break ;
					case MENU_CUR_SYS_TEXT_B   : calc_color( &emsc->color_text  , 2, -1 ) ; break ;
					case MENU_CUR_SYS_SHADOW_R : calc_color( &emsc->color_shadow, 0, -1 ) ; break ;
					case MENU_CUR_SYS_SHADOW_G : calc_color( &emsc->color_shadow, 1, -1 ) ; break ;
					case MENU_CUR_SYS_SHADOW_B : calc_color( &emsc->color_shadow, 2, -1 ) ; break ;
					}

					EmuSysCfg_CheckValue() ;
				}
				break ;
			}
		}
		else if( (buttons_old & CTRL_RIGHT) && ((buttons_new & CTRL_RIGHT)==0) )
		{
			// Å®
			switch( menu_mode )
			{
			case MENU_MODE_MD_LOAD    :
				if( strcmp( menu_files[ menu_cursol[ menu_mode ] ].name, ".." ) == 0 )
				{
					char* p = strrchr( path_last[0], '/' ) ;
					if( p )
					{
						*p = 0 ;
						p = strrchr( path_last[0], '/' ) ;
						if( p ){ *(p+1) = 0 ; }
					}

					Menu_UpdateFileList( path_last[0] ) ; 
				}
				else if( menu_files[ menu_cursol[ menu_mode ] ].type & PSP_FILE_TYPE_DIR )
				{
					strcat( path_last[0], menu_files[ menu_cursol[ menu_mode ] ].name ) ;
					Menu_UpdateFileList( path_last[0] ) ; 
				}
				else
				{
					menu_files_sel_s = menu_cursol[ menu_mode ] ;

					if( menu_files_sel_s != menu_files_sel ){ is_menu_emu_state_load = 0 ; }

					menu_mode = MENU_MODE_MD_STATE ;
				}
				break ;
			case MENU_MODE_MD_CHEAT   :
				if( menu_cursol[ menu_mode ] == MENU_CUR_CHEAT_ON )
				{
					emct->all_on = emct->all_on ? 0 : 1 ;
				}
				else
				{
					emct->modifyed = 1 ;
					emct->flag[ menu_cursol[ menu_mode ] -1 ] = emct->flag[ menu_cursol[ menu_mode ] -1 ] ? 0 : 1 ;
					EmuCheat_CheckValue( emct ) ;
				}
				break ;
			case MENU_MODE_MD_CONFIG  :
				if( emc )
				{
					switch( menu_cursol[ menu_mode ] )
					{
					case MENU_CUR_CPU        : emc->cpu_freq++                       ; break ;

#ifdef _EMU_CLOCK_ADJ  ////////////////////
					case MENU_CUR_EMU_CLOCK  : emc->emu_clock+=2                     ; break ;
					case MENU_CUR_Z80_CLOCK  : emc->z80_clock+=2                     ; break ;
#endif // _EMU_CLOCK_ADJ  /////////////////

					case MENU_CUR_CORE       : emc->show_flags ^= FLAG_CORE          ; break ; 
					case MENU_CUR_Z80ASYNC   : emc->show_flags ^= FLAG_Z80ASYNC      ; break ; 
					case MENU_CUR_AUTOSRAM   : emc->autosram = emc->autosram ? 0 : 1 ; break ;
					case MENU_CUR_COUNTRY    : emc->country++                        ; break ;
					case MENU_CUR_CHECKSUM   : emc->show_flags ^= FLAG_CHECKSUM      ; break ; 
					case MENU_CUR_2CON       : emc->comm_mode++                      ; break ; 
					case MENU_CUR_MAX_FPS    : emc->max_fps+=2                       ; break ;
					case MENU_CUR_SKIP       : emc->skip_count++                     ; break ;
					case MENU_CUR_SKIP_MODE  : emc->skip_mode++                      ; break ;
					case MENU_CUR_SKIP_T     : emc->skip_turbo++                     ; break ;
					case MENU_CUR_SCREEN     : emc->screen_mode++                    ; break ;
					case MENU_CUR_VSYNC      : emc->show_flags ^= SHOW_VSYNC         ; break ; 
					case MENU_CUR_SHOW_FPS   : emc->show_fps++                       ; break ; 
					case MENU_CUR_SHOW_BG    : emc->show_flags ^= SHOW_BG            ; break ; 
					case MENU_CUR_PALETTE    : emc->palette_mode++                   ; break ; 
					case MENU_CUR_SOUND_ON   : emc->sound_on++                       ; break ;
					case MENU_CUR_SOUND_VOL  : emc->sound_vol++                      ; break ;
					case MENU_CUR_SOUND_YMCH : emc->sound_flag ^= MD_SND_YMCH        ; break ;
					case MENU_CUR_SOUND_PSG  : emc->sound_flag ^= MD_SND_PSG         ; break ;
					case MENU_CUR_SOUND_DAC  : emc->sound_flag ^= MD_SND_DAC         ; break ;
					case MENU_CUR_SOUND_TB   : emc->sound_flag ^= MD_SND_TURBO       ; break ;
					}

					EmuCfg_CheckValue( emc ) ;
				}
				break ;
			case MENU_MODE_MD_KEY :
				if( emc )
				{
					if( menu_cursol[ menu_mode ] == MENU_CUR_SWAP_ANALOG )
					{
						emc->analog_mode++ ;
					}
					else if( menu_cursol[ menu_mode ] == MENU_CUR_6PAD )
					{
						emc->pad_option ^= PAD_OPT_6PAD ;
					}
					else
					{
						emc->pad_cfg[ menu_cursol[ menu_mode ] - MENU_CUR_CIRCLE ] ++ ;
					}
					EmuCfg_CheckValue( emc ) ;
				}
				break ;
			case MENU_MODE_MD_STATE :
				{
					if( (menu_cursol[ menu_mode ] >= MENU_CUR_STATE_SLOT0) 
					 && (menu_cursol[ menu_mode ] <= MENU_CUR_STATE_SLOT4) )
					{
						menu_cursol[ menu_mode ]++ ;
						if( menu_cursol[ menu_mode ] > MENU_CUR_STATE_SLOT4 )
						{   menu_cursol[ menu_mode ] = MENU_CUR_STATE_SLOT0 ; }
					}
					else if( (menu_cursol[ menu_mode ] >= MENU_CUR_STATE_SLOT5) 
					      && (menu_cursol[ menu_mode ] <= MENU_CUR_STATE_SLOT9) )
					{
						menu_cursol[ menu_mode ]++ ;
						if( menu_cursol[ menu_mode ] > MENU_CUR_STATE_SLOT9 )
						{   menu_cursol[ menu_mode ] = MENU_CUR_STATE_SLOT5 ; }
					}
					else
					{
						if( menu_files_sel_s == menu_files_sel )
						{
							switch( menu_cursol[ menu_mode ] )
							{
							case MENU_CUR_STATE_ROM   : menu_files_sel_s++ ;
								if( menu_files_sel_s >= menu_files_num ){ menu_files_sel_s = 0 ; }
								if( menu_files_sel_s != menu_files_sel ){ is_menu_emu_state_load = 0 ; }
								break ;
							case MENU_CUR_STATE_MODE1 : emu_state.cfg.save_mode1++     ; break ;
							case MENU_CUR_STATE_MODE2 : emu_state.cfg.save_mode2++     ; break ;
							case MENU_CUR_STATE_LOOP  : emu_state.cfg.loop_mode ++     ; break ;
							case MENU_CUR_STATE_INC   : emu_state.cfg.slot_inc++       ; break ;
							case MENU_CUR_STATE_SEL   : emu_state.cfg.slot_sel++       ; break ;
							}
							EmuStateCfg_CheckValue( &emu_state.cfg ) ;
						}
						else
						{
							switch( menu_cursol[ menu_mode ] )
							{
							case MENU_CUR_STATE_ROM   : menu_files_sel_s++ ;
								if( menu_files_sel_s >= menu_files_num ){ menu_files_sel_s = 0 ; }
								if( menu_files_sel_s != menu_files_sel ){ is_menu_emu_state_load = 0 ; }
								break ;
							case MENU_CUR_STATE_MODE1 : menu_emu_state.cfg.save_mode1++ ; break ;
							case MENU_CUR_STATE_MODE2 : menu_emu_state.cfg.save_mode2++ ; break ;
							case MENU_CUR_STATE_LOOP  : menu_emu_state.cfg.loop_mode ++ ; break ;
							case MENU_CUR_STATE_INC   : menu_emu_state.cfg.slot_inc++   ; break ;
							case MENU_CUR_STATE_SEL   : menu_emu_state.cfg.slot_sel++   ; break ;
							}
							EmuStateCfg_CheckValue( &menu_emu_state.cfg ) ;
						}
					}
				}
				break ;
			case MENU_MODE_MD_SYSTEM :
				if( emsc )
				{
					switch( menu_cursol[ menu_mode ] )
					{
					case MENU_CUR_SYS_KEY_MENU : emsc->key_menu++                         ; break ;
					case MENU_CUR_SYS_KEY_R    : emsc->key_repeat--                       ; break ;
					case MENU_CUR_SYS_CLOCK    : emsc->option_flags ^= OPTION_SHOW_CLOCK  ; break ;
					case MENU_CUR_SYS_BATT1    : emsc->option_flags ^= OPTION_SHOW_BATT1  ; break ;
					case MENU_CUR_SYS_BATT2    : emsc->option_flags ^= OPTION_SHOW_BATT2  ; break ;
					case MENU_CUR_SYS_STATE_D  : emsc->option_flags ^= OPTION_SHOW_DATE   ; break ;
					case MENU_CUR_SYS_STATE_L  : emsc->option_flags ^= OPTION_STATE_LOAD  ; break ;
					case MENU_CUR_SYS_STATE_S  : emsc->option_flags ^= OPTION_STATE_SAVE  ; break ;
					case MENU_CUR_SYS_BGMODE   : emsc->bg_mode++                          ; break ;
					case MENU_CUR_SYS_BG_R     : calc_color( &emsc->color_bg    , 0,  1 ) ; break ;
					case MENU_CUR_SYS_BG_G     : calc_color( &emsc->color_bg    , 1,  1 ) ; break ;
					case MENU_CUR_SYS_BG_B     : calc_color( &emsc->color_bg    , 2,  1 ) ; break ;
					case MENU_CUR_SYS_GRAD     : emsc->color_grad++                       ; break ;
					case MENU_CUR_SYS_SHADOW   : emsc->shadow_on = emsc->shadow_on ? 0:1  ; break ;
					case MENU_CUR_SYS_TITLE_R  : calc_color( &emsc->color_title , 0,  1 ) ; break ;
					case MENU_CUR_SYS_TITLE_G  : calc_color( &emsc->color_title , 1,  1 ) ; break ;
					case MENU_CUR_SYS_TITLE_B  : calc_color( &emsc->color_title , 2,  1 ) ; break ;
					case MENU_CUR_SYS_SEL_R    : calc_color( &emsc->color_select, 0,  1 ) ; break ;
					case MENU_CUR_SYS_SEL_G    : calc_color( &emsc->color_select, 1,  1 ) ; break ;
					case MENU_CUR_SYS_SEL_B    : calc_color( &emsc->color_select, 2,  1 ) ; break ;
					case MENU_CUR_SYS_MSGBOX_R : calc_color( &emsc->color_msgbox, 0,  1 ) ; break ;
					case MENU_CUR_SYS_MSGBOX_G : calc_color( &emsc->color_msgbox, 1,  1 ) ; break ;
					case MENU_CUR_SYS_MSGBOX_B : calc_color( &emsc->color_msgbox, 2,  1 ) ; break ;
					case MENU_CUR_SYS_MSG_R    : calc_color( &emsc->color_msg   , 0,  1 ) ; break ;
					case MENU_CUR_SYS_MSG_G    : calc_color( &emsc->color_msg   , 1,  1 ) ; break ;
					case MENU_CUR_SYS_MSG_B    : calc_color( &emsc->color_msg   , 2,  1 ) ; break ;
					case MENU_CUR_SYS_TEXT_R   : calc_color( &emsc->color_text  , 0,  1 ) ; break ;
					case MENU_CUR_SYS_TEXT_G   : calc_color( &emsc->color_text  , 1,  1 ) ; break ;
					case MENU_CUR_SYS_TEXT_B   : calc_color( &emsc->color_text  , 2,  1 ) ; break ;
					case MENU_CUR_SYS_SHADOW_R : calc_color( &emsc->color_shadow, 0,  1 ) ; break ;
					case MENU_CUR_SYS_SHADOW_G : calc_color( &emsc->color_shadow, 1,  1 ) ; break ;
					case MENU_CUR_SYS_SHADOW_B : calc_color( &emsc->color_shadow, 2,  1 ) ; break ;
					}

					EmuSysCfg_CheckValue() ;
				}
				break ;
			}
		}
		else if( (buttons_old & CTRL_START) && ((buttons_new & CTRL_START)==0) )
		{
			if( PSP_Ctrlpad_GetPadData()->analog[CTRL_ANALOG_X] < LOWER_THRESHOLD2 )
			{
				Menu_DrawFrame() ;
				PSP_Screen_WaitVsync() ;
				PSP_Screen_Flip() ;
				Menu_DrawFrame() ;
				if( Menu_MessageBox( SZ_MESSAGE_EXITGAME, 1, MSGBOX_OKCANCEL ) )
				{
					Menu_DrawFrame() ;
					Menu_MessageBox( SZ_MESSAGE_ON_EXITGAME, 1, MSGBOX_NOBUTTON ) ;
					PSP_Screen_WaitVsync() ;
					PSP_Screen_Flip() ;

					PSP_Main_OnExitGame() ;
					sceKernelExitGame() ;
					for(;;){ sceKernelDelayThread(1000*1000); }
				}
			}
			else if( PSP_Ctrlpad_GetPadData()->analog[CTRL_ANALOG_X] > UPPER_THRESHOLD2 )
			{
				if( PSP_Comm_IsConnect() )
				{
					Menu_DrawFrame() ;
					PSP_Screen_WaitVsync() ;
					PSP_Screen_Flip() ;
					Menu_DrawFrame() ;
					if( Menu_MessageBox( SZ_MESSAGE_DISCONNECT, 1, MSGBOX_OKCANCEL ) )
					{
						PSP_Comm_Term() ;
					}
				}
			}
		}
	}
	else
	{
//		menu_key_repeat++ ;
	}
}

void Menu_DrawBG ()
{
	if( (emc->show_flags & SHOW_BG) && (emc->screen_mode == SCREEN_1X) )
	{
		PSP_Screen_ClearAll( bmpBG.pBuff ) ;
	}
	else
	{
		PSP_Screen_ClearAll( 0 ) ;
	}

	/*
	if( (emc->show_flags & SHOW_BG) && (emc->screen_mode == SCREEN_1X) )
	{
		PSP_Bitmap_Blt( &bmpBG, 0,0, PSP_SCREEN_WIDTH, PSP_SCREEN_HEIGHT ) ;
		PSP_Screen_WaitVsync() ;
		PSP_Screen_Flip() ;
		PSP_Bitmap_Blt( &bmpBG, 0,0, PSP_SCREEN_WIDTH, PSP_SCREEN_HEIGHT ) ;
		PSP_Screen_WaitVsync() ;
		PSP_Screen_Flip() ;
	}
	else
	{
		PSP_Screen_FillRect( 0,0, PSP_SCREEN_WIDTH, PSP_SCREEN_HEIGHT, 0 ) ;
		PSP_Screen_WaitVsync() ;
		PSP_Screen_Flip() ;
		PSP_Screen_FillRect( 0,0, PSP_SCREEN_WIDTH, PSP_SCREEN_HEIGHT, 0 ) ;
		PSP_Screen_WaitVsync() ;
		PSP_Screen_Flip() ;
	}
	*/
}

void Menu_DrawBG_1X ( )
{
	PSP_Screen_ClearAll( bmpBG.pBuff ) ;
}

void Menu_DrawLoading()
{
	Menu_DrawFrame() ;

	Menu_MessageBox( SZ_MESSAGE_LOADING, 1, MSGBOX_NOBUTTON ) ;
}

void Menu_GetMsg_Skip     ( char* buff )
{
	int n = 2 ;

	SZ_SKIP[ sizeof(SZ_SKIP) -2 ] = '0' + emc->skip_count ;

	while(1)
	{
		if( SZ_SKIP[n] != 0x20 ){ strcpy( buff, &SZ_SKIP[n] ); return ; }
		n++ ;
	}
}

void Menu_GetMsg_Screen   ( char* buff )
{	
	int n = 2 ;

	while(1)
	{
		if( SZ_SCREEN[emc->screen_mode][n] != 0x20 ){ strcpy( buff, &SZ_SCREEN[emc->screen_mode][n] ); return ; }
		n++ ;
	}
}

void Menu_GetMsg_FPS      ( char* buff )
{
	int n = 2 ;

	while(1)
	{
		if( SZ_SHOW_FPS[emc->show_fps][n] != 0x20 ){ strcpy( buff, &SZ_SHOW_FPS[emc->show_fps][n] ); return ; }
		n++ ;
	}
}

void Menu_GetMsg_Vsync    ( char* buff )
{
	int n = 2 ;

	while(1)
	{
		if( SZ_VSYNC[ emc->show_flags & SHOW_VSYNC ? 1 : 0 ][n] != 0x20 )
		{ 
			strcpy( buff, &SZ_VSYNC[ emc->show_flags & SHOW_VSYNC ? 1 : 0 ][n] ); 
			return ; 
		}
		n++ ;
	}
}

void Menu_GetMsg_SoundMode( char* buff )
{
	int n = 2 ;

	while(1)
	{
		if( SZ_SOUND_ON[emc->sound_on][n] != 0x20 )
		{ 
			strcpy( buff, &SZ_SOUND_ON[emc->sound_on][n] ); 
			return ; 
		}
		n++ ;
	}
}

void Menu_GetMsg_SoundVol ( char* buff )
{
	int n = 2 ;

	while(1)
	{
		if( SZ_SOUND_VOL[emc->sound_vol][n] != 0x20 )
		{ 
			strcpy( buff, &SZ_SOUND_VOL[emc->sound_vol][n] ); 
			return ; 
		}
		n++ ;
	}
}

const char* SZ_STATE_MSG[] = 
{ 
	"Select Manual State", "Manual State Load" , "Manual State Save", 
	"Increment State Load", "Increment State Save"
} ;

void Menu_GetMsg_State ( char* buff, int nFlag, int nSlot )
{
	strcpy( buff, SZ_STATE_MSG[ nFlag ] ) ;
	strcat( buff, " : Slot 0" );
	buff[ strlen(buff) -1 ] = '0' + nSlot ;
}

void Menu_GetMsg_CheatOn   ( char* buff, int on )
{
	strcpy( buff, SZ_CHEAT_ON[on] ) ;
}

void Menu_GetMsg_Core ( char* buff )
{
	int n   = 2 ;
	int sel = (emc->show_flags & FLAG_CORE) ? 1 : 0 ;

	while(1)
	{
		if( SZ_CORE[sel][n] != 0x20 )
		{ 
			strcpy( buff, &SZ_CORE[sel][n] ); 
			return ; 
		}
		n++ ;
	}

}

void Menu_GetMsg_Reset ( char* buff )
{
	strcpy( buff, SZ_KEY_ITEMS[ MD_PAD_RESET ] ) ;
}

void Menu_GetMsg_Turbo ( char* buff, int on )
{
	strcpy( buff, SZ_TURBO_ON[on] ) ;
}

int Menu_MainMenu()
{
	int n, m ;
	unsigned long buttons_old = 0 ;

	menu_cmd = MENU_CMD_NONE ;

	if( emc == NULL ){ emc = &emu_config[ EMU_MD ] ; }

	/*
	for( m=5; m > 0; m-- )
	{
		for( n=0; n < 5 ; n++ )
		{
			PSP_Bitmap_BltTransparent( &bmpBG, 0,0, PSP_SCREEN_WIDTH, PSP_SCREEN_HEIGHT, m ) ;
			PSP_Screen_WaitVsync() ;
			PSP_Screen_Flip() ;
		}
	}
	//*/

	PSP_Screen_SetBkColor( -1 ) ;

	for(;;)
	{
		PSP_Ctrlpad_Update() ;
		if( PSP_Ctrlpad_GetButtons() == 0 ){ break ; }
		Menu_DrawFrame() ;
		PSP_Screen_WaitVsync() ;
		PSP_Screen_Flip() ;
	}

	for(;;)
	{
		PSP_Ctrlpad_Update() ;
		Menu_UpdateButtons( PSP_Ctrlpad_GetButtons(), buttons_old ) ;
		buttons_old = PSP_Ctrlpad_GetButtons() ;

		Menu_DrawFrame() ;

		switch( menu_mode )
		{
		default :
		case MENU_MODE_MD_LOAD    : Menu_DrawMD_Load   () ; break ;
		case MENU_MODE_MD_STATE   : Menu_DrawMD_State  () ; break ;
		case MENU_MODE_MD_CHEAT   : Menu_DrawMD_Cheat  () ; break ;
		case MENU_MODE_MD_CONFIG  : Menu_DrawMD_Config () ; break ;
		case MENU_MODE_MD_KEY     : Menu_DrawMD_KeyCfg () ; break ;
		case MENU_MODE_MD_SYSTEM  : Menu_DrawMD_System () ; break ;
		}

		if( menu_cmd != MENU_CMD_NONE ){ break ; }

		PSP_Screen_WaitVsync() ;
		PSP_Screen_Flip() ;
	}

	if( (menu_cmd == MENU_CMD_LOAD_MD) || (menu_cmd == MENU_CMD_LOAD_MD_ROM_S) )
	{
		Menu_MessageBox( SZ_MESSAGE_LOADING, 1, MSGBOX_NOBUTTON ) ;
	}
	else
	{
		Menu_DrawBG() ;
	}

	PSP_Screen_SetBkColor( 0 ) ;

	return menu_cmd ;
}

///////////////////////////////////////////////////////////////////////////
/// devil_menu.c
///////////////////////////////////////////////////////////////////////////
