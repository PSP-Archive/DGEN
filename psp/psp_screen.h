///////////////////////////////////////////////////////////////////////////
/// psp_screen.h
///////////////////////////////////////////////////////////////////////////
#ifndef _PSP_SCREEN_H
#define _PSP_SCREEN_H
////////////////////////////////////////////////////////
#ifdef __cplusplus
extern "C" {
#endif
////////////////////////////////////////////////////////
#define PSP_SCREEN_WIDTH  (480)
#define PSP_SCREEN_HEIGHT (272)
#define	PSP_LINESIZE	  (512)
#define PSP_PIXELSIZE     (1) 
#define	PSP_FRAMESIZE	  (0x00044000)
#define PSP_FONT_WIDTH    (5)
#define PSP_FONT_HEIGHT   (10)
////////////////////////////////////////////////////////
#define PSP_RGB(r,g,b) ((((b>>3) & 0x1F)<<10)|(((g>>3) & 0x1F)<<5)|(((r>>3) & 0x1F)<<0))
////////////////////////////////////////////////////////

int   PSP_Screen_Init          ( void ) ;
void* PSP_Screen_GetBackBuffer ( int x, int y ) ;
void  PSP_Screen_ClearAll      ( unsigned char* src ) ;
void  PSP_Screen_Flip          ( void ) ; 
void  PSP_Screen_WaitVsync     ( void ) ; // { sceDisplayWaitVblankStart() ; }
void  PSP_Screen_WaitVsyncFast ( void ) ; 
void  PSP_Screen_WaitVsyncTM   ( int tm ) ; 
void  PSP_Screen_SetVBlankInt  ( int nON ) ;
void* PSP_Screen_GetTempBuffer ( int x, int y ) ;
void  PSP_Screen_TempFlip      ( void ) ;
void  PSP_Screen_BitBltGe      ( unsigned char* src, int srcW, int srcH, int dstX, int dstY, int dstW, int dstH ) ;
void  PSP_Screen_WaitBlt       ( void ) ;

void  PSP_Screen_SetBkColor ( int color ) ;
void  PSP_Screen_Rectangle  ( short l, short t, short r, short b , short color ) ;
void  PSP_Screen_FillRect   ( short l, short t, short r, short b , short color ) ;
void  PSP_Screen_GradRect   ( short l, short t, short r, short b , short color ) ;
void  PSP_Screen_DrawText   ( short l, short t, const char* p    , short color ) ;
void  PSP_Screen_DrawTextN  ( short l, short t, const char* p    , int num, short color ) ;
void  PSP_Screen_DrawInt2   ( short l, short t, unsigned char val, short color ) ; //‚QŒ…ŒÀ’è
void  PSP_Screen_DrawInt3   ( short l, short t, unsigned char val, short color ) ; //‚RŒ…ŒÀ’è
void  PSP_Screen_DrawHex2   ( short l, short t, unsigned char val, short color ) ; //‚QŒ…ŒÀ’è
void  PSP_Screen_DrawHex8   ( short l, short t, unsigned long val, short color ) ; //‚WŒ…ŒÀ’è
void  PSP_Screen_DrawIntN   ( short l, short t, unsigned long val, int num, short color ) ; //Œ…”ŒÀ’è
void  PSP_Screen_DrawSIntN  ( short l, short t,   signed long val, int num, short color ) ; //Œ…”ŒÀ’è

unsigned short HSV2RGB( int iH, int iS, int iV ) ;

////////////////////////////////////////////////////////
#ifdef __cplusplus
}
#endif
/////////////////////////////////////////////////////////
#endif // _PSP_SCREEN_H
//////////////////////////////////////////////////////////////////////////
