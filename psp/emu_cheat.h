///////////////////////////////////////////////////////////////////////////
/// emu_cheat.h
///////////////////////////////////////////////////////////////////////////
#ifndef _EMU_CHEAT_H
#define _EMU_CHEAT_H
///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
#define MAX_CHEAT_NUM   (30)
#define MAX_CHEAT_NAME  (32)
#define MAX_CHEAT_GG    (9)
#define MIN_CHEAT_ADDR  (0x00000000)
#define MAX_CHEAT_ADDR  (0x00FFFFFF)
///////////////////////////////////////////////////////////////////////////

typedef struct _EMU_CHEAT
{
	          int  modifyed               ;
	          int  all_on                 ;
	          int  all_on_old             ;
	  signed char  flag [ MAX_CHEAT_NUM ] ;
	unsigned long  addr [ MAX_CHEAT_NUM ] ;
	unsigned short data [ MAX_CHEAT_NUM ] ;
	         char  code [ MAX_CHEAT_NUM ][ MAX_CHEAT_GG  +1 ] ;
	         char  name [ MAX_CHEAT_NUM ][ MAX_CHEAT_NAME+1 ] ;

} EMU_CHEAT ;

///////////////////////////////////////////////////////////////////////////

int  EmuCheat_Init      ( EMU_CHEAT* emct ) ;
void EmuCheat_CheckValue( EMU_CHEAT* emct ) ;
int  EmuCheat_Load      ( EMU_CHEAT* emct, const char* path ) ;
int  EmuCheat_Save      ( EMU_CHEAT* emct, const char* path ) ;
void EmuCheat_Exec      ( EMU_CHEAT* emct );
int  EmuCheat_CalcGG    ( const char* code_p, unsigned long* addr_p, unsigned long* data_p ) ;

///////////////////////////////////////////////////////////////////////////
#endif // _EMU_CHEAT_H
///////////////////////////////////////////////////////////////////////////
