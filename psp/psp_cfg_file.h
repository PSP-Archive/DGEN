///////////////////////////////////////////////////////////////////////////
/// psp_cfg_file.h
///////////////////////////////////////////////////////////////////////////
#ifndef _PSP_CFG_FILE_H
#define _PSP_CFG_FILE_H
///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
extern void PSP_CfgFile_Init    ( const char* header ) ;
extern int  PSP_CfgFile_FindKey ( const char* key    ) ;
extern int  PSP_CfgFile_GetInt  ( int pos ) ;
extern int  PSP_CfgFile_GetHex  ( int pos ) ;
extern int  PSP_CfgFile_GetStr  ( int pos, char* str, int max ) ;
extern int  PSP_CfgFile_MakeInt ( const char* key, int val ) ;
extern int  PSP_CfgFile_MakeHex ( const char* key, int val ) ;
extern int  PSP_CfgFile_MakeStr ( const char* key, char* str ) ;
extern int  PSP_CfgFile_Read    ( const char* path ) ;
extern int  PSP_CfgFile_Write   ( const char* path ) ;
///////////////////////////////////////////////////////////////////////////
#endif //_PSP_CFG_FILE_H
///////////////////////////////////////////////////////////////////////////
