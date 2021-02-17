////////////////////////////////////////////////////////
/// psp_main.h
////////////////////////////////////////////////////////
#ifndef _PSP_MAIN_H
#define _PSP_MAIN_H
////////////////////////////////////////////////////////

#define PSP_SOUND_SAMPLE       (768)

#define TM_1FRAME   (16666)         //  1 frame time
#define TM_60FRAME  (1000000) //(TM_1FRAME*60)  // 60 frame time

#ifndef NULL
#define NULL (0)
#endif //NULL

enum 
{ 
    PSP_FILE_TYPE_DIR  = 0x10 , 
    PSP_FILE_TYPE_FILE = 0x20 
};

typedef struct _PSP_FILE_DATE 
{
	unsigned short unk[2] ; //常にゼロ？
	unsigned short year   ;
	unsigned short mon    ;
	unsigned short mday   ;
	unsigned short hour   ;
	unsigned short min    ;
	unsigned short sec    ;
}PSP_FILE_DATE ;

typedef struct _PSP_FILE_ATTRIBUTE 
{
    unsigned long unk0        ;
    unsigned long type        ;
    unsigned long size        ;
	PSP_FILE_DATE ctime       ; //作成日時
	PSP_FILE_DATE atime       ; //最終アクセス日時
	PSP_FILE_DATE mtime       ; //最終更新日時
	unsigned long unk[7]      ; //常にゼロ？
    char          name[0x108] ;
}PSP_FILE_ATTRIBUTE  ;

typedef struct _PSP_DATE_TIME
{
	unsigned short year ;
	unsigned short mon  ;
	unsigned short mday ;
	unsigned short hour ;
	unsigned short min  ;
	unsigned short sec  ;
	unsigned long  usec ;

}PSP_DATE_TIME ;


/////////////////////////////////////////////
/// PSP Functions 
/////////////////////////////////////////////

void  PSP_Main_OnExitGame( void ) ;

short*        PSP_Sound_GetBackBuffer  ( int nChannel ) ;
void          PSP_Sound_BufferFlip     ( int nChannel ) ;

#include "psp_pad.h"

////////////////////////////////////////////////////////
#endif //_PSP_MAIN_H
////////////////////////////////////////////////////////
