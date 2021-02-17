///////////////////////////////////////////////////////////////////////////
/// emu_core.h
///////////////////////////////////////////////////////////////////////////
#ifndef _EMU_CORE_H
#define _EMU_CORE_H
///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
enum{ EMU_MD, EMU_PCE, MAX_EMU } ;
#define MAX_EMU_CORE_BUFF  (10 * 1024 * 1024)
///////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
extern "C"
{
#endif
///////////////////////////////////////////////////////////////////////////
#include "emu_config.h"
#include "emu_state.h"
#include "emu_cheat.h"
#include "psp_pad.h"
///////////////////////////////////////////////////////////////////////////
//extern unsigned char    sEmuCoreBuff[ MAX_EMU_CORE_BUFF ] ;
extern int              emu_mode                          ;
extern EMU_CONFIG       emu_config[MAX_EMU]               ;
extern EMU_STATE        emu_state                         ;
extern EMU_CHEAT        emu_cheat                         ;
///////////////////////////////////////////////////////////////////////////

unsigned char* EmuCore_GetBuffer( unsigned long size ) ;

int EmuCore_IsSaveSRAM () ;
int EmuCore_SaveSRAM   ( const char* path ) ;

//////////////////////////////////////////////////////////////////////
/// MD core
//////////////////////////////////////////////////////////////////////
extern void md_init_memory( unsigned char* pCoreBuff, unsigned long nBuffSize ) ;
extern void md_init() ;
extern void md_reset() ;
extern void md_setconfig( EMU_CONFIG* emc ) ;
extern int  md_load_rom( const char* path, int fix_checksum ) ;
extern void md_fix_checksum() ;
extern int  md_get_checksum() ;
extern int  md_is_save_sram () ;
extern int  md_load_sram( const char* path ) ;
extern int  md_save_sram( const char* path ) ;
extern void md_setframe( unsigned char* p, int width ) ;
extern void md_set_bpp( unsigned char bpp ) ;
extern int  md_get_vline() ;
extern void md_sound_update ( int z80_async, short* p, short len ) ;
extern void md_sound_update2( int z80_async, short* p1, short* p2, short len ) ;
extern void md_setpaddata ( PSP_PAD_DATA* pPad, EMU_CONFIG* emc ) ;
extern void md_setpaddata2( PSP_PAD_DATA* pPad, EMU_CONFIG* emc ) ;
extern void md_select_paddata( int sel ) ;
extern void md_setpad     ( int pad, int sel ) ;
extern int  md_getpad     ( int sel ) ;
extern int  md_get_state( EMU_STATE_SLOT* ess ) ;
extern int  md_set_state( EMU_STATE_SLOT* ess ) ;
extern void md_cheat_exec( unsigned long addr, unsigned short val );
extern int  md_select_core( int n ) ;
extern void md_set_adjust_clock( int sysclk, int z80clk ) ;
extern int  md_run     ( int z80_async ) ;
extern int  md_run_skip( int z80_async ) ;
//////////////////////////////////////////////////////////////////////
/// PCE core
//////////////////////////////////////////////////////////////////////
extern void pce_init_memory( unsigned char* pCoreBuff, unsigned long nBuffSize ) ;
extern void pce_init() ;
extern void pce_setconfig( EMU_CONFIG* emc ) ;
extern int  pce_load_rom( const char* path ) ;
extern void pce_setframe( unsigned char* p, int width ) ;
extern int  pce_sound_update( short* p, short len ) ;
extern void pce_setpaddata ( unsigned long buttons ) ;
extern int  pce_run() ;
extern int  pce_run_skip() ;

//////////////////////////////////////////////////////////////////////
/// GB core
//////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
}
#endif
///////////////////////////////////////////////////////////////////////////
#endif // _EMU_CORE_H
///////////////////////////////////////////////////////////////////////////
