///////////////////////////////////////////////////////////////////////////
/// psp_bitmap.cpp
///////////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include "psp_std.h"
#include "psp_screen.h"
#include "psp_bitmap.h"
#include "../lib/png.h"
///////////////////////////////////////////////////////////////////////////
#ifndef WORD
#define WORD unsigned short
#endif //

#ifndef DWORD
#define DWORD unsigned long
#endif //

#ifndef LONG
#define LONG long
#endif //

typedef struct tagBITMAPFILEHEADER {
        WORD    bfType;
        DWORD   bfSize;
        WORD    bfReserved1;
        WORD    bfReserved2;
        DWORD   bfOffBits;
} BITMAPFILEHEADER, *LPBITMAPFILEHEADER, *PBITMAPFILEHEADER;

typedef struct tagBITMAPINFOHEADER{
        DWORD      biSize;
        LONG       biWidth;
        LONG       biHeight;
        WORD       biPlanes;
        WORD       biBitCount;
        DWORD      biCompression;
        DWORD      biSizeImage;
        LONG       biXPelsPerMeter;
        LONG       biYPelsPerMeter;
        DWORD      biClrUsed;
        DWORD      biClrImportant;
} BITMAPINFOHEADER, *LPBITMAPINFOHEADER, *PBITMAPINFOHEADER;

#define  DIB_HEADER_MARKER ((WORD) ('M' << 8) | 'B')

#define  CALCWIDTHBYTES(bits)    ((DWORD)(((bits) + 31) / 32) * 4)
///////////////////////////////////////////////////////////////////////////

int debug_bmp[10];

unsigned short PSP_Bitmap_GetBitColor( PSP_BITMAP* pBmp, int px, int py )
{
	// worning! px, py is no check .
//	unsigned char* p = (unsigned char*)( pBmp->pBuff + ((MAX_PSP_BMP_WIDTH * py + px) * sizeof(short)) ) ;
//	return *((unsigned short*)( p )) ; 
	return *((unsigned short*)( pBmp->pBuff + ((MAX_PSP_BMP_WIDTH * py + px) * sizeof(short)) )) ; 
}

int PSP_Bitmap_FileRead( char* pPath, PSP_BITMAP* pBmp ) 
{
	BITMAPFILEHEADER bmf ;
	BITMAPINFOHEADER bmi ;
	unsigned char buff[ CALCWIDTHBYTES( MAX_PSP_BMP_WIDTH * 24 ) ] ;
	unsigned short* dest ;
	int width ;
	int x, y ;
	//FILE* pFile = fopen( pPath, "rb" ) ;
	int fd = sceIoOpen( pPath, PSP_O_RDONLY, 0644 );

	//if( pFile )
	if( fd )
	{
		//if( fread( &bmf, sizeof(bmf), 1, pFile) != sizeof(bmf) ){ return -1; }
		//if( fread( &bmf, 14, 1, pFile) != 14 ){ return -1; }
		if( sceIoRead( fd, &bmf, 14 ) != 14 ){ return -1; }
		if( bmf.bfType != DIB_HEADER_MARKER ){ return -2; }

		if( sceIoRead( fd, &bmi, sizeof(bmi) ) != sizeof(bmi) ){ return -3; }
		//if( fread( &bmi, sizeof(bmi), 1, pFile) != sizeof(bmi) ){ return -3; }

		width = CALCWIDTHBYTES( bmi.biWidth * bmi.biBitCount ) ;

		if( width > sizeof(buff) ){ return -4 ; }
		if( bmi.biHeight > MAX_PSP_BMP_HEIGHT ){ bmi.biHeight = MAX_PSP_BMP_HEIGHT ; }
		if( bmi.biWidth  > MAX_PSP_BMP_WIDTH  ){ bmi.biWidth  = MAX_PSP_BMP_WIDTH  ; }

		pBmp->nCX = bmi.biWidth  ;
		pBmp->nCY = bmi.biHeight ;

		dest = (unsigned short*)( pBmp->pBuff ) ;
		dest += ( MAX_PSP_BMP_WIDTH * (MAX_PSP_BMP_HEIGHT -1) ) ;

		debug_bmp[2] = (int)pBmp->pBuff ;
		debug_bmp[3] = (int)dest        ;

		if( bmi.biBitCount == 24 )
		{
			for( y=0; y < bmi.biHeight ; y++ )
			{
				//dest = (unsigned short*)( pBmp->pBuff ) ;
				//dest += ( MAX_PSP_BMP_WIDTH * (MAX_PSP_BMP_HEIGHT - y -1) ) ;

				//fread( buff, width, 1, pFile ) ;
				sceIoRead( fd, buff, width ) ;

				for( x=0; x < width ; x+=3 )
				{
					dest[x/3] = PSP_RGB( buff[x+2], buff[x+1], buff[x+0] ) ;
				}

				//for( ; x < PSP_BMP_WIDTH ; x++ ){ dest[n] = 0 ; }

				//dest += MAX_PSP_BMP_WIDTH ;
				dest -= MAX_PSP_BMP_WIDTH ;
			}
		}
		else if( bmi.biBitCount == 16 )
		{
			//*
			for( y=0; y < bmi.biHeight ; y++ )
			{
				//fread( dest, width, 1, pFile ) ;
				sceIoRead( fd, dest, width ) ;

				//for( x=m_nCX; x < PSP_BMP_WIDTH ; x++ ){ dest[n] = 0 ; }

				dest -= MAX_PSP_BMP_WIDTH ;
			}
			//*/
		}
		else if( bmi.biBitCount == 8 )
		{
		}

		//fclose( pFile ) ;
		sceIoClose( fd ) ;

		return 1 ;
	}

	return 0 ;
}

int PSP_Bitmap_FileRead_PNG( char* pPath, PSP_BITMAP* pBmp ) 
{
	unsigned short* out    = (unsigned short*)pBmp->pBuff ;
	size_t          outlen = sizeof(pBmp->pBuff)          ;

	FILE *fp = fopen(pPath,"rb");
	if(!fp) return 1;

	const size_t nSigSize = 8;
	unsigned char signature[nSigSize];
	if (sceIoRead(fileno(fp), signature, sizeof(unsigned char)*nSigSize) != nSigSize)
	{
		fclose(fp);
		return 2 ;
	}

	if (!png_check_sig( signature, nSigSize ))
	{	
		fclose(fp);
		return 3 ;
	}

	png_struct *pPngStruct = png_create_read_struct( PNG_LIBPNG_VER_STRING,
													 NULL, NULL, NULL );
	if(!pPngStruct)
	{
		fclose(fp);
		return 4 ;
	}

	png_info *pPngInfo = png_create_info_struct(pPngStruct);
	if(!pPngInfo)
	{
		png_destroy_read_struct( &pPngStruct, NULL, NULL );
		fclose(fp);
		return 5 ;
	}

	if (setjmp( pPngStruct->jmpbuf ))
	{
		png_destroy_read_struct( &pPngStruct, NULL, NULL );
		fclose(fp);
		return 6 ;
	}

	png_init_io( pPngStruct, fp );
	png_set_sig_bytes( pPngStruct, nSigSize );
	png_read_png( pPngStruct, pPngInfo,
			PNG_TRANSFORM_STRIP_16 | PNG_TRANSFORM_PACKING |
			PNG_TRANSFORM_EXPAND | PNG_TRANSFORM_BGR , NULL);

	png_uint_32 width      = pPngInfo->width      ;
	png_uint_32 height     = pPngInfo->height     ;
	int         color_type = pPngInfo->color_type ;

	debug_bmp[5] = width  ;
	debug_bmp[6] = height ;
	debug_bmp[7] = outlen ;
	debug_bmp[8] = width * height * sizeof(unsigned short) ;

	if (outlen != width * height * sizeof(unsigned short))
	{
		png_destroy_read_struct( &pPngStruct, &pPngInfo, NULL );
		fclose(fp);
		return 7 ;
	}
	
	png_byte **pRowTable = pPngInfo->row_pointers;
	unsigned int x, y;
	unsigned char r, g, b;
	for (y=0; y<height; y++)
	{
		png_byte *pRow = pRowTable[y];
		for (x=0; x<width; x++)
		{
			switch(color_type)
			{
				case PNG_COLOR_TYPE_GRAY:
					r = g = b = *pRow++;
					break;
				case PNG_COLOR_TYPE_GRAY_ALPHA:
					r = g = b = *pRow++;
					pRow++;
					break;
				case PNG_COLOR_TYPE_RGB:
					b = *pRow++;
					g = *pRow++;
					r = *pRow++;
					break;
				case PNG_COLOR_TYPE_RGB_ALPHA:
					b = *pRow++;
					g = *pRow++;
					r = *pRow++;
					pRow++;
					break;
			}
			*out++ = PSP_RGB(r,g,b);
		}
	}
	
	png_destroy_read_struct( &pPngStruct, &pPngInfo, NULL );
	fclose(fp);

	pBmp->nCX = width  ;
	pBmp->nCY = height ;

	return 0 ;
}

void PSP_Bitmap_Blt ( PSP_BITMAP* pBmp, int px, int py, int cx, int cy )
{
	int x, y ;
	unsigned short* pDst = (unsigned short*)PSP_Screen_GetBackBuffer( px, py ) ;
	unsigned short* pSrc = (unsigned short*)pBmp->pBuff ;

	if( cy > pBmp->nCY ){ cy = pBmp->nCY ; }
	if( cx > pBmp->nCX ){ cx = pBmp->nCX ; }

	for( y=0; y < cy ; y++ )
	{
		for( x=0; x < cx ; x++ )
		{
			pDst[x] = pSrc[x] ;
		}

		pDst += PSP_LINESIZE      ;
		pSrc += MAX_PSP_BMP_WIDTH ;
	}
}

void PSP_Bitmap_BltMask( PSP_BITMAP* pBmp, int px, int py, int cx, int cy, unsigned short colMask )
{
	int x, y ;
	unsigned short* pDst = (unsigned short*)PSP_Screen_GetBackBuffer( px, py ) ;
	unsigned short* pSrc = (unsigned short*)pBmp->pBuff ;

	if( cy > pBmp->nCY ){ cy = pBmp->nCY ; }
	if( cx > pBmp->nCX ){ cx = pBmp->nCX ; }

	for( y=0; y < cy ; y++ )
	{
		for( x=0; x < cx ; x++ )
		{
			if( pSrc[x] != colMask )
			{
				pDst[x] = pSrc[x] ;
			}
		}

		pDst += PSP_LINESIZE      ;
		pSrc += MAX_PSP_BMP_WIDTH ;
	}
}

void PSP_Bitmap_BltEx( PSP_BITMAP* pBmp, int px, int py, int cx, int cy, int sx, int sy, unsigned short colMask )
{
	int x, y ;
	unsigned short* pDst = (unsigned short*)PSP_Screen_GetBackBuffer( px, py ) ;
	unsigned short* pSrc = (unsigned short*)( pBmp->pBuff + (pBmp->nCX * sy + sx) * 2 ) ;

	if( cy > pBmp->nCY ){ cy = pBmp->nCY ; }
	if( cx > pBmp->nCX ){ cx = pBmp->nCX ; }

	for( y=0; y < cy ; y++ )
	{
		for( x=0; x < cx ; x++ )
		{
			if( pSrc[x] != colMask )
			{
				pDst[x] = pSrc[x] ;
			}
		}

		pDst += PSP_LINESIZE      ;
		pSrc += MAX_PSP_BMP_WIDTH ;
	}
}

void PSP_Bitmap_BltTransparent( PSP_BITMAP* pBmp, int px, int py, int cx, int cy, int tr )
{
	int x, y ;
	unsigned short* pDst = (unsigned short*)PSP_Screen_GetBackBuffer( px, py ) ;
	unsigned short* pSrc = (unsigned short*)pBmp->pBuff ;
	unsigned long tr_mask ;
	unsigned long tt ;

	switch( tr )
	{
	default : tr_mask = 0xFFFF ; break ;
	case 1  : tr_mask = 0xFBDE ; break ;
	case 2  : tr_mask = 0xF39C ; break ;
	case 3  : tr_mask = 0xE318 ; break ;
	case 4  : tr_mask = 0xC210 ; break ;
	case 5  : tr_mask = 0x0000 ; break ;
	}

	if( cy > pBmp->nCY ){ cy = pBmp->nCY ; }
	if( cx > pBmp->nCX ){ cx = pBmp->nCX ; }

	for( y=0; y < cy ; y++ )
	{
		for( x=0; x < cx ; x++ )
		{
			tt = pSrc[x] ;
			pDst[x] = ((tt & tr_mask) >> tr) ;
//			pDst[x] = (unsigned short)(( (int)pSrc[x] & tr_mask ) >> tr ) ;
		}

		pDst += PSP_LINESIZE      ;
		pSrc += MAX_PSP_BMP_WIDTH ;
	}
}

void PSP_Bitmap_BltTransparentMask( PSP_BITMAP* pBmp, int px, int py, int cx, int cy, int tr, unsigned short colMask )
{
	int x, y ;
	unsigned short* pDst = (unsigned short*)PSP_Screen_GetBackBuffer( px, py ) ;
	unsigned short* pSrc = (unsigned short*)pBmp->pBuff ;
	unsigned short tr_mask ;

	switch( tr )
	{
	default : tr_mask = 0xFFFF ; break ;
	case 1  : tr_mask = 0xFBDE ; break ;
	case 2  : tr_mask = 0xF39C ; break ;
	case 3  : tr_mask = 0xE318 ; break ;
	case 4  : tr_mask = 0xC210 ; break ;
	case 5  : tr_mask = 0x0000 ; break ;
	}

	if( cy > pBmp->nCY ){ cy = pBmp->nCY ; }
	if( cx > pBmp->nCX ){ cx = pBmp->nCX ; }

	for( y=0; y < cy ; y++ )
	{
		for( x=0; x < cx ; x++ )
		{
			if( pSrc[x] != colMask )
			{
				pDst[x] = ( pSrc[x] & tr_mask ) >> tr ;
			}
		}

		pDst += PSP_LINESIZE      ;
		pSrc += MAX_PSP_BMP_WIDTH ;
	}
}
