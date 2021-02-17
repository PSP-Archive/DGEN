///////////////////////////////////////////////////////////////////////////
/// psp_menu.h
///////////////////////////////////////////////////////////////////////////
#ifndef _PSP_MENU_H
#define _PSP_MENU_H
////////////////////////////////////////////////////////
#include "psp_pad.h"
////////////////////////////////////////////////////////

enum
{
	MENU_CMD_NONE          ,
	MENU_CMD_OK            ,
	MENU_CMD_CANCEL        ,
	MENU_CMD_LOAD_MD       ,
	MENU_CMD_LOAD_MD_STATE ,
	MENU_CMD_LOAD_MD_ROM_S ,
	MENU_CMD_LOAD_PCE 
};

#define MAX_PATH  (128)

extern int  menu_cmd             ;
extern char path_exec [MAX_PATH] ;
extern char path_rom  [MAX_PATH] ;
extern char path_cfg  [MAX_PATH] ;
extern char path_state[MAX_PATH] ;
extern char path_sram [MAX_PATH] ;
extern char path_cheat[MAX_PATH] ;
extern int  menu_load_state_slot ;

void Menu_InitPath         ( int argc, char *argv ) ;
void Menu_Init             ( ) ;
void Menu_DrawBG           ( ) ;
void Menu_DrawBG_1X        ( ) ;
void Menu_GetMsg_Skip      ( char* buff ) ;
void Menu_GetMsg_Screen    ( char* buff ) ;
void Menu_GetMsg_FPS       ( char* buff ) ;
void Menu_GetMsg_Vsync     ( char* buff ) ;
void Menu_GetMsg_SoundMode ( char* buff ) ;
void Menu_GetMsg_SoundVol  ( char* buff ) ;
void Menu_GetMsg_State     ( char* buff, int nFlag, int nSlot ) ;
void Menu_GetMsg_CheatOn   ( char* buff, int on ) ;
void Menu_GetMsg_Core      ( char* buff ) ;
void Menu_GetMsg_Reset     ( char* buff ) ;
void Menu_GetMsg_Turbo     ( char* buff, int on ) ;
int  Menu_MainMenu         ( ) ;

/////////////////////////////////////////////////////////
#endif // _PSP_MENU_H
//////////////////////////////////////////////////////////////////////////
