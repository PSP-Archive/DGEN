////////////////////////////////////////
#ifndef _PSP_COMM_H
#define _PSP_COMM_H
////////////////////////////////////////
#include "emu_config.h"
////////////////////////////////////////

enum
{ 
	COMM_CMD_ERROR   , 
	COMM_CMD_OK      , 
	COMM_CMD_PAD1    , 
	COMM_CMD_PAD2    , 
	COMM_CMD_INIT    , 
	COMM_CMD_INIT_D  , 
	COMM_CMD_INIT_E  , 
	COMM_CMD_MENU    , 
	COMM_CMD_EXECKEY ,
	COMM_CMD_MSG     , 
	COMM_CMD_KILL    , 
	MAX_COMM_CMD    
} ;

enum
{ 
	COMM_ERROR_NON     , 
	COMM_ERROR_OK      , 
	COMM_ERROR_NG      , 
	COMM_ERROR_TIMEOUT , 
	COMM_ERROR_RETRY   , 
	COMM_ERROR_CANCEL  
} ;

enum{ COMM_SHOW_CONNECT, COMM_SHOW_SYNC } ;

#define MAX_COMM_BUFF    (64)
#define MAX_COMM_BUFF_S  (768)

typedef struct _PSP_COMM_HEADER
{
	//unsigned long id  ;
	unsigned short cmd ;
	unsigned short len ;

} PSP_COMM_HEADER ;

typedef struct _PSP_COMM_INIT
{
	PSP_COMM_HEADER hdr     ;
	int             version ;
	long            chksum  ;
	unsigned long   size    ;
	EMU_CONFIG      emc     ;

} PSP_COMM_INIT ;

typedef struct _PSP_COMM_INIT_DATA
{
	PSP_COMM_HEADER hdr     ;
	unsigned char   data[1] ;

} PSP_COMM_INIT_DATA ;

typedef struct _PSP_COMM_DATA
{
	PSP_COMM_HEADER hdr   ;
	//unsigned long   frame ;
	unsigned long   data  ;

} PSP_COMM_DATA ;

#define MAX_PSP_COMM_MSG  (32)

typedef struct _PSP_COMM_MSG
{
	PSP_COMM_HEADER hdr                    ;
	unsigned long   len                    ;
	char            data[MAX_PSP_COMM_MSG] ;
} PSP_COMM_MSG ;

int PSP_Comm_IsConnect() ;
int PSP_Comm_Init( int bServer ) ;
int PSP_Comm_SyncData( void* set, void* get, int b1 ) ;
int PSP_Comm_CheckCmd( void* get ) ;
void PSP_Comm_Term() ;

unsigned long  PSP_Comm_GetSyncData_S() ;
unsigned long  PSP_Comm_GetSyncData_R() ;
unsigned long  PSP_Comm_GetPad_R1    () ;
unsigned long  PSP_Comm_GetPad_R2    () ;
unsigned long  PSP_Comm_GetPad_S     () ;
int            PSP_Comm_GetCmd       () ;
int            PSP_Comm_SetSyncData  ( unsigned long data ) ;

int PSP_Comm_InitThread  () ;

int  PSP_Comm_StartSync   ( unsigned short cmd, unsigned long data, int b1 ) ;
int  PSP_Comm_StartSync2  ( unsigned short cmd, int b1 ) ;
int  PSP_Comm_WaitSync    () ;
int  PSP_Comm_SyncCmd     ( unsigned short cmd, unsigned long data, int b1 ) ;

// Use GUI
int PSP_Comm_Connect   ( int bServer ) ; 
int PSP_Comm_ShowError ( int nError  ) ;  
int PSP_Comm_SyncState ( int bServer, int bCancel ) ; 

////////////////////////////////////////
#endif // _PSP_COMM_H
////////////////////////////////////////
