///////////////////////////////////////////////////////////////////////////
/// psp_bitmap.h
///////////////////////////////////////////////////////////////////////////
#ifndef _PSP_BITMAP_H
#define _PSP_BITMAP_H
////////////////////////////////////////////////////////
#define MAX_PSP_BMP_WIDTH     (480)
#define MAX_PSP_BMP_HEIGHT    (272)
#define MAX_PSP_BMP_BUFF  (MAX_PSP_BMP_WIDTH*MAX_PSP_BMP_HEIGHT*sizeof(short))

typedef struct _PSP_BITMAP
{
	long           nCX                       ;
	long           nCY                       ;
	unsigned char  pBuff[ MAX_PSP_BMP_BUFF ] ;
} PSP_BITMAP ;

int            PSP_Bitmap_FileRead          ( char* pPath, PSP_BITMAP* pBmp ) ;
int            PSP_Bitmap_FileRead_PNG      ( char* pPath, PSP_BITMAP* pBmp ) ;
unsigned short PSP_Bitmap_GetBitColor       ( PSP_BITMAP* pBmp, int px, int py ) ;
void           PSP_Bitmap_Blt               ( PSP_BITMAP* pBmp, int px, int py, int cx, int cy ) ;
void           PSP_Bitmap_BltMask           ( PSP_BITMAP* pBmp, int px, int py, int cx, int cy, unsigned short colMask ) ;
void           PSP_Bitmap_BltEx             ( PSP_BITMAP* pBmp, int px, int py, int cx, int cy, int sx, int sy, unsigned short colMask ) ;
void           PSP_Bitmap_BltTransparent    ( PSP_BITMAP* pBmp, int px, int py, int cx, int cy, int tr ) ;
void           PSP_Bitmap_BltTransparentMask( PSP_BITMAP* pBmp, int px, int py, int cx, int cy, int tr, unsigned short colMask ) ;

/////////////////////////////////////////////////////////
#endif // _PSP_BITMAP_H
//////////////////////////////////////////////////////////////////////////
