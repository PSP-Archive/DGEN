///////////////////////////////////////////////////////////////////////////
/// emu_state.h
///////////////////////////////////////////////////////////////////////////
#ifndef _EMU_STATE_H
#define _EMU_STATE_H
///////////////////////////////////////////////////////////////////////////
#include "psp_main.h"
///////////////////////////////////////////////////////////////////////////

#define MAX_STATE_SLOT  (10)
#define MAX_STATE_BUFF  ((0x30000) - sizeof(PSP_DATE_TIME))
#define STATE_IMAGE_CX  (80)
#define STATE_IMAGE_CY  (56)

extern unsigned short state_temp_image[ STATE_IMAGE_CY ][ STATE_IMAGE_CX ] ;
extern char emu_state_path[ 128 ] ;
extern char emu_state_ext [] ;

enum{ STATE_LOOP_0_9, STATE_LOOP_0_4, STATE_LOOP_5_9 } ;

enum{ STATE_MODE_BOTH, STATE_MODE_MSD, STATE_MODE_MEM } ;

enum{ STATE_FLAG_ACTIVE = 0x01, STATE_FLAG_UPDATE=0x10 } ;

typedef struct _EMU_STATE_SLOT
{
	char title[4]  ;
	int  version   ;
	int  flag      ;
	int  buff_len  ;

	unsigned short image[ STATE_IMAGE_CY ][ STATE_IMAGE_CX ] ;
	unsigned char  buff [ MAX_STATE_BUFF ] ;

	PSP_DATE_TIME date ;

} EMU_STATE_SLOT ;

typedef struct _EMU_STATE_CFG
{
	signed   char loop_mode   ; // インクリメントステートセーブ
	signed   char save_mode1  ; // ステート保存モード（SLOT0-4）
	signed   char save_mode2  ; // ステート保存モード（SLOT5-9）
	signed   char dmy1        ;

	signed   char slot_sel    ;
	signed   char slot_inc    ;
	signed   char dmy2        ;
	signed   char dmy3        ;

} EMU_STATE_CFG ;

typedef struct _EMU_STATE
{
	EMU_STATE_CFG   cfg ;

	EMU_STATE_SLOT  slot[ MAX_STATE_SLOT ] ;

} EMU_STATE ;

void EmuStateCfg_CheckValue ( EMU_STATE_CFG* esc ) ;

void EmuStateCfg_InitDefault ( const char* path ) ;
void EmuStateCfg_SaveDefault ( const char* path ) ;
void EmuStateCfg_SetDefault  ( EMU_STATE_CFG* esc ) ;

void EmuState_Init           ( EMU_STATE* es ) ;
void EmuState_CalcInc        ( EMU_STATE* es ) ;
void EmuState_Load           ( EMU_STATE* es, const char* path ) ;
void EmuState_Save           ( EMU_STATE* es, const char* path ) ;
int  EmuState_SaveSlot       ( EMU_STATE* es, int slot, unsigned short* image, const char* path ) ;
int  EmuState_DeleteSlot     ( EMU_STATE* es, int slot, const char* path ) ;
int  EmuState_GetSaveCount   ( EMU_STATE* es ) ;
void EmuState_Copy           ( EMU_STATE* dst, EMU_STATE* src ) ;

void EmuStateSlot_Init       ( EMU_STATE_SLOT* ess ) ;
int  EmuStateSlot_Load_MEM   ( EMU_STATE_SLOT* ess ) ;
int  EmuStateSlot_Save_MEM   ( EMU_STATE_SLOT* ess, unsigned short* image ) ;
int  EmuStateSlot_Load_MSD   ( EMU_STATE_SLOT* ess, const char* path );
int  EmuStateSlot_Save_MSD   ( EMU_STATE_SLOT* ess, const char* path );
void EmuStateSlot_Copy       ( EMU_STATE_SLOT* dst, EMU_STATE_SLOT* src ) ;

unsigned char* EmuState_GetBuffer() ;
int            EmuState_Compress  ( void* buff, int len, int offset ) ;
int            EmuState_Uncompress( void* buff, int len ) ;

///////////////////////////////////////////////////////////////////////////
#endif //_EMU_STATE
///////////////////////////////////////////////////////////////////////////
