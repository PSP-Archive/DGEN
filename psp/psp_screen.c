///////////////////////////////////////////////////////////////////////////
/// psp_screen.c
///////////////////////////////////////////////////////////////////////////
#include <string.h>
#include "psp_screen.h"
#include "font.c"
#include "fontNaga10.c"
////////////////////////////////////////////////////////
//#define PSP_SCREEN_PTRTOP  (0x04000000)
//#define PSP_SCREEN_TMPTOP  (0x04140000)
#define PSP_SCREEN_PTRTOP  (0x44000000)
#define PSP_SCREEN_TMPTOP  (0x44140000)
#define PSP_VBLANK_INT     (30)
#define MAX_SCREEN_NUM     (4)
#define VSYNC_MODE_NORMAL  (0)
#define VSYNC_MODE_CB      (1)

int   psp_screen_bk_color  = -1 ;
int   psp_screen_bltid     = -1 ;
int   psp_screen_vsyncmode = VSYNC_MODE_NORMAL ;
int   psp_screen_intnum    = 0 ;
void* psp_screen_int[ MAX_SCREEN_NUM ] ;

static void* psp_screen_ptr = ((void*)PSP_SCREEN_PTRTOP) ;
static void* psp_screen_tmp = ((void*)PSP_SCREEN_TMPTOP) ;

void PSP_Screen_InitGe    () ;
void PSP_Screen_VBlankInt ( void *parg ) ;

int PSP_Screen_Init( void )
{
	int   no  = 0 ;
	void *arg = 0 ;

	PSP_Screen_InitGe() ;

	sceDisplaySetMode( 1, PSP_SCREEN_WIDTH, PSP_SCREEN_HEIGHT );
	sceDisplaySetFrameBuf(0,0,0,1);

	memset( psp_screen_ptr, 0, PSP_FRAMESIZE * 2 ) ;

	sceDisplaySetFrameBuf( psp_screen_ptr, PSP_LINESIZE,1,1 ) ;

	//sceKernelRegisterSubIntrHandler( PSP_VBLANK_INT, no, PSP_Screen_VBlankInt, arg ) ;

	return 0 ;
}

void  PSP_Screen_ClearAll( unsigned char* src )
{
	int n, y ;
	unsigned short* pBuf = ((unsigned short*)PSP_SCREEN_PTRTOP) ;

	if( src )
	{
		for( n=0; n < MAX_SCREEN_NUM ; n++ )
		{
			unsigned short* pSrc = (unsigned short*)src ;
			for( y=0; y < PSP_SCREEN_HEIGHT ; y++ )
			{
				memcpy( pBuf, pSrc, PSP_SCREEN_WIDTH * sizeof(short) ) ;
				pBuf += PSP_LINESIZE     ;
				pSrc += PSP_SCREEN_WIDTH ;
			}
		}
	}
	else
	{
		for( n=0; n < MAX_SCREEN_NUM ; n++ )
		{
			for( y=0; y < PSP_SCREEN_HEIGHT ; y++ )
			{
				memset( pBuf, 0, PSP_SCREEN_WIDTH * sizeof(short) ) ;
				pBuf += PSP_LINESIZE ;
			}
		}
	}
}

inline void* PSP_Screen_GetBackBuffer( int x, int y )
{
	return psp_screen_ptr + y * (PSP_LINESIZE * 2) + x * 2 ;
}

void PSP_Screen_Flip( void )
{
	if( psp_screen_vsyncmode == VSYNC_MODE_NORMAL )
	{
		sceKernelDcacheWritebackAll();
		sceDisplaySetFrameBuf( psp_screen_ptr, PSP_LINESIZE,1,0 ) ;
		psp_screen_ptr = (void*)( (int)psp_screen_ptr == PSP_SCREEN_PTRTOP ? (PSP_SCREEN_PTRTOP + PSP_FRAMESIZE) : PSP_SCREEN_PTRTOP ) ;
	}
	else //if( psp_screen_vsyncmode == VSYNC_MODE_CB )
	{
		if( psp_screen_intnum < MAX_SCREEN_NUM )
		{
			psp_screen_int[psp_screen_intnum] = psp_screen_ptr ;
			psp_screen_intnum++ ;

			int n, m, find, num = psp_screen_intnum ;
			for( n=0; n < MAX_SCREEN_NUM ; n++ )
			{
				find = 0 ;
				for( m=0; m < num ; m++ )
				{
					if( psp_screen_int[m] == (void*)(PSP_SCREEN_PTRTOP + PSP_FRAMESIZE*n) )
					{
						find = 1 ; break ;
					}
				}

				if( !find ){ psp_screen_ptr = (void*)(PSP_SCREEN_PTRTOP + PSP_FRAMESIZE*n) ; return ; }
			}

			//sceKernelDcacheWritebackAll() ;
			//psp_screen_intnum++ ;
		}
	}
}

inline void* PSP_Screen_GetTempBuffer( int x, int y )
{
	return psp_screen_tmp + y * (PSP_LINESIZE * 2) + x * 2 ;
}

inline void PSP_Screen_TempFlip( void )
{
	psp_screen_tmp = (void*)( (int)psp_screen_tmp == PSP_SCREEN_TMPTOP ? (PSP_SCREEN_TMPTOP + PSP_FRAMESIZE) : PSP_SCREEN_TMPTOP ) ;
}

inline void  PSP_Screen_WaitVsync ( void ) 
{ 
	sceDisplayWaitVblankStart() ; 
}

inline void  PSP_Screen_WaitVsyncFast ( void ) 
{ 
	sceDisplayWaitVblankStart() ; 
}

void  PSP_Screen_WaitVsyncTM ( int tm )
{
	unsigned long ts = sceKernelLibcClock() ;

	while( !sceDisplayIsVblank() )
	{
		if( (sceKernelLibcClock() - ts) > tm ){ break ; }
	}
}

void PSP_Screen_VBlankInt ( void *parg )
{
	int n ;

	if( (psp_screen_vsyncmode == VSYNC_MODE_CB) )
	{
		if(psp_screen_intnum > 0)
		{
			//sceKernelDcacheWritebackAll();

			if(psp_screen_intnum > 1)
			{
				sceDisplaySetFrameBuf( psp_screen_int[1], PSP_LINESIZE,1,0 ) ;

				for( n=1; n < psp_screen_intnum ; n++ )
				{
					psp_screen_int[n-1] = psp_screen_int[n] ;
				}
				psp_screen_intnum-- ;
			}
			else
			{
				sceDisplaySetFrameBuf( psp_screen_int[0], PSP_LINESIZE,1,0 ) ;
			}
		}
	}

	/*
	int n ;

	if( (psp_screen_vsyncmode == VSYNC_MODE_CB) && (psp_screen_intnum > 0) )
	{
		//sceKernelDcacheWritebackAll();
		sceDisplaySetFrameBuf( psp_screen_int[0], PSP_LINESIZE,1,0 ) ;

		for( n=1; n < psp_screen_intnum ; n++ )
		{
			psp_screen_int[n-1] = psp_screen_int[n] ;
		}
		psp_screen_intnum-- ;
	}
	*/
}

void  PSP_Screen_SetVBlankInt( int nON )
{
	int   no  = 0, n ;
	void *arg = 0 ;

	if( nON && (psp_screen_vsyncmode != VSYNC_MODE_CB) )
	{
		psp_screen_intnum = 0 ;
		for( n=0; n < MAX_SCREEN_NUM ; n++ ){ psp_screen_int[n] = ((void*)PSP_SCREEN_PTRTOP) ; }
		psp_screen_vsyncmode = VSYNC_MODE_CB ;
		sceKernelRegisterSubIntrHandler( PSP_VBLANK_INT, no, PSP_Screen_VBlankInt, arg ) ;
		sceKernelEnableSubIntr         ( PSP_VBLANK_INT, no ) ;
	}
	else if( psp_screen_vsyncmode != VSYNC_MODE_NORMAL )
	{
		psp_screen_vsyncmode = VSYNC_MODE_NORMAL ;
		sceKernelDisableSubIntr       ( PSP_VBLANK_INT, no ) ;
		sceKernelReleaseSubIntrHandler( PSP_VBLANK_INT, no ) ;
	}
}

void PSP_Screen_SetBkColor( int color )
{
	psp_screen_bk_color = color ;
}

void PSP_Screen_Rectangle( short l, short t, short r, short b, short color )
{
	short x, y ;
	unsigned char* pSC1 = (unsigned char*)PSP_Screen_GetBackBuffer( l  , t ) ;
	unsigned char* pSC2 = (unsigned char*)PSP_Screen_GetBackBuffer( l  , b ) ;
	unsigned char* pSC3 = (unsigned char*)PSP_Screen_GetBackBuffer( r-1, t ) ;
	unsigned char* pSC4 = pSC1 ;

	for( x=0; x < r - l; x++ )
	{
		*((short*)pSC1) = color ;
		*((short*)pSC2) = color ;
		pSC1 += 2    ;
		pSC2 += 2    ;
	}

	for( y=0; y < b - t; y++ )
	{
		*((short*)pSC3) = color   ;
		*((short*)pSC4) = color   ;
		pSC3  += PSP_LINESIZE * 2 ;
		pSC4  += PSP_LINESIZE * 2 ;
	}
}

void PSP_Screen_FillRect ( short l, short t, short r, short b, short color )
{
	short x, y ;
	unsigned char* pSC = (unsigned char*)PSP_Screen_GetBackBuffer( l, t ) ;
	unsigned char* pSC1 ;

	for( y=0; y < b - t; y++ )
	{
		pSC1 = pSC ;

		for( x=0; x < r - l; x++ )
		{
			*((short*)pSC1) = color ;
			pSC1 += 2 ;
		}

		pSC += PSP_LINESIZE * 2 ;
	}
}

void PSP_Screen_GradRect ( short l, short t, short r, short b, short color )
{

	short x, y ;
	unsigned char* pSC = (unsigned char*)PSP_Screen_GetBackBuffer( l, t ) ;
	unsigned char* pSC1 ;
	unsigned short col ;

	for( y=0; y < b - t; y++ )
	{
		pSC1 = pSC ;

		col = HSV2RGB(color,255,255 - y) ;
		//if( color > 360 ){ color = 0 ; }

		for( x=0; x < r - l; x++ )
		{
			*((unsigned short*)pSC1) = col ;
			pSC1 += 2 ;
		}

		pSC += PSP_LINESIZE * 2 ;
	}

#if 0

	short x, y ;
	unsigned char* pSC = (unsigned char*)PSP_Screen_GetBackBuffer( l, b -1 ) ;
	unsigned char* pSC1 ;
	short red   = (color) & (0x1F    ) ;
	short green = (color) & (0x1F<<10) ;
	short blue  = (color) & (0x1F<<15) ;
	short ctr = 0, ctg = 0, ctb =0 ;
	short cta_r = (b - t) / 0x0F * 11 / 10 ; //(b - t) / ((red   > 0x0F) ? 0x0F : (0x0F - red  )) ;
	short cta_g = (b - t) / 0x0F * 12 / 10 ; //(b - t) / ((green > 0x0F) ? 0x0F : (0x0F - green)) ;
	short cta_b = (b - t) / 0x0F * 13 / 10 ; //(b - t) / ((blue  > 0x0F) ? 0x0F : (0x0F - blue )) ;

//	red = green = blue = 0 ;

	for( y=0; y < b - t; y++ )
	{
		pSC1 = pSC ;

		for( x=0; x < r - l; x++ )
		{
			*((short*)pSC1) = red | green | blue ;
			pSC1 += 2 ;
		}

//		ctr++; ctg++; ctb++;
//		if( ctr > cta_r ){ ctr = 0 ; red   += (1    ); if( red   < (0x0F    ) ){ ; }else{ red   = 0x00; } }
//		if( ctg > cta_g ){ ctg = 0 ; green += (1<< 5); if( green < (0x0F<< 5) ){ ; }else{ green = 0x00; } }
//		if( ctb > cta_b ){ ctb = 0 ; blue  += (1<<10); if( blue  < (0x0F<<10) ){ ; }else{ blue  = 0x00; } }
		ctr++ ;
		if( ctr > cta_r )
		{ 
			ctr = 0 ;
			if( red > 0 ){ red-- ; }else
			{ 
				red = 0 ;
			}
		}
				ctg++ ;
				if( ctg > cta_g )
				{ 
					ctg = 0 ;
//					if( green < (0x0F<< 5) ){ green += (1<< 5); }else
					if( green > (0x01<< 5) ){ green = (green - (1<< 5)) & (0x0F<<10) ; }else
					{
						green = 0 ;
					}
				}
						ctb++ ;
						if( ctb > cta_b )
						{ 
							ctb = 0 ;
				//			if( blue  < (0x0F<<10) ){ blue  += (1<<10); }else
							if( blue > (0x01<<10) ){ blue = (blue - (1<<10)) & (0x0F<<10) ; }else
							{
								blue = 0 ;
							}
						} 
		//			}
		//		}
		//	} 
		//}

		pSC -= PSP_LINESIZE * 2 ;
	}
#endif // 0


}

void pgPutChar(unsigned long x,unsigned long y,unsigned long color,unsigned long bgcolor,unsigned char ch,char drawfg,char drawbg,char mag)
{
	unsigned char *vptr0;		//pointer to vram
	unsigned char *vptr;		//pointer to vram
	const unsigned char *cfont;		//pointer to font
	unsigned long cx,cy;
	unsigned long b;
	char mx,my;

	//if (ch>255) return;
	cfont=font+ch*8;
	vptr0=(unsigned char*)PSP_Screen_GetBackBuffer(x,y);
	for (cy=0; cy<8; cy++) 
	{
		for (my=0; my<mag; my++) 
		{
			vptr=vptr0;
			b=0x80;
			for (cx=0; cx<8; cx++) 
			{
				for (mx=0; mx<mag; mx++) 
				{
					if ((*cfont&b)!=0) 
					{
						if (drawfg) *(unsigned short *)vptr=color;
					}
					else 
					{
						if (drawbg) *(unsigned short *)vptr=bgcolor;
					}
					vptr+=PSP_PIXELSIZE*2;
				}
				b=b>>1;
			}
			vptr0+=PSP_LINESIZE*2;
		}
		cfont++;
	}
}


// by kwn
void Draw_Char_Hankaku(int x,int y,const unsigned char c,int col) 
{
	unsigned short *vr;
	unsigned char  *fnt;
	unsigned char  pt;
	unsigned char ch;
	int x1,y1, mx,my;

	ch = c;

	// mapping
	if (ch<0x20)
		ch = 0;
	else if (ch<0x80)
		ch -= 0x20;
	else if (ch<0xa0)
		ch = 0;
	else
		ch -= 0x40;

	fnt = (unsigned char *)&hankaku_font10[ch*10];

	// draw

	vr = (unsigned short *)PSP_Screen_GetBackBuffer(x,y);
	for(y1=0;y1<10;y1++) 
	{
		pt = *fnt++;
		for(x1=0;x1<5;x1++) 
		{
			if (pt & 1)
			{
				*vr = col;
			}
			else if( psp_screen_bk_color >= 0 )
			{
				*vr = (unsigned short)psp_screen_bk_color ;
			}
			vr++;
			pt = pt >> 1;
		}
		vr += PSP_LINESIZE-5;
	}
}

// by kwn
void Draw_Char_Zenkaku(int x,int y,const unsigned char u,unsigned char d,int col) 
{
	// ELISA100.FNTに存在しない文字
	const unsigned short font404[] = {
		0xA2AF, 11,
		0xA2C2, 8,
		0xA2D1, 11,
		0xA2EB, 7,
		0xA2FA, 4,
		0xA3A1, 15,
		0xA3BA, 7,
		0xA3DB, 6,
		0xA3FB, 4,
		0xA4F4, 11,
		0xA5F7, 8,
		0xA6B9, 8,
		0xA6D9, 38,
		0xA7C2, 15,
		0xA7F2, 13,
		0xA8C1, 720,
		0xCFD4, 43,
		0xF4A5, 1030,
		0,0
	};
	unsigned short *vr;
	unsigned short *fnt;
	unsigned short pt;
	int x1,y1, mx,my;

	unsigned long n;
	unsigned short code;
	int i, j;

	// SJISコードの生成
	code = u;
	code = (code<<8) + d;

	// SJISからEUCに変換
	if(code >= 0xE000) code-=0x4000;
	code = ((((code>>8)&0xFF)-0x81)<<9) + (code&0x00FF);
	if((code & 0x00FF) >= 0x80) code--;
	if((code & 0x00FF) >= 0x9E) code+=0x62;
	else code-=0x40;
	code += 0x2121 + 0x8080;

	// EUCから恵梨沙フォントの番号を生成
	n = (((code>>8)&0xFF)-0xA1)*(0xFF-0xA1)
		+ (code&0xFF)-0xA1;
	j=0;
	while(font404[j]) {
		if(code >= font404[j]) {
			if(code <= font404[j]+font404[j+1]-1) {
				n = -1;
				break;
			} else {
				n-=font404[j+1];
			}
		}
		j+=2;
	}
	fnt = (unsigned short *)&zenkaku_font10[n*10];

	// draw
	vr = (unsigned short *)PSP_Screen_GetBackBuffer(x,y);
	for(y1=0;y1<10;y1++) 
	{
		pt = *fnt++;
		for(x1=0;x1<10;x1++) 
		{
			if (pt & 1)
			{
				*vr = col;
			}
			else if( psp_screen_bk_color >= 0 )
			{
				*vr = (unsigned short)psp_screen_bk_color ;
			}
			vr++;
			pt = pt >> 1;
		}
		vr += PSP_LINESIZE-10;
	}
}

void PSP_Screen_DrawText ( short x, short y, const char* str , short col )
{
	// by kwn
	unsigned char ch = 0,bef = 0;

	while(*str != 0) 
	{
		ch = *str++;
		if (bef!=0) 
		{
			Draw_Char_Zenkaku(x,y,bef,ch,col);
			x+=10 ;
			bef=0;
		} 
		else 
		{
			if (((ch>=0x80) && (ch<0xa0)) || (ch>=0xe0)) 
			{
				bef = ch;
			} 
			else 
			{
				Draw_Char_Hankaku(x,y,ch,col);
				x += 5 ;
			}
		}
	}
}


void PSP_Screen_DrawTextN ( short x, short y, const char* str, int num, short col )
{
	// by kwn
	unsigned char ch = 0,bef = 0;

	for( ; num > 0 ; num-- ) 
	{
		ch = *str++;
		if (bef!=0) 
		{
			Draw_Char_Zenkaku(x,y,bef,ch,col);
			x+=10 ;
			bef=0;
		} 
		else 
		{
			if (((ch>=0x80) && (ch<0xa0)) || (ch>=0xe0)) 
			{
				bef = ch;
			} 
			else 
			{
				Draw_Char_Hankaku(x,y,ch,col);
				x += 5 ;
			}
		}
	}
}

void  PSP_Screen_DrawInt2 ( short x, short y, unsigned char val, short col )
{
	Draw_Char_Hankaku(x  ,y,'0' + (val/10),col) ;
	Draw_Char_Hankaku(x+5,y,'0' + (val%10),col) ;
}

void  PSP_Screen_DrawInt3 ( short x, short y, unsigned char val, short col )
{
	Draw_Char_Hankaku(x,y,'0' + (val/100)  ,col) ; x+=5 ;
	Draw_Char_Hankaku(x,y,'0' + (val/10)%10,col) ; x+=5 ;
	Draw_Char_Hankaku(x,y,'0' + (val%10)   ,col) ;
}

void  PSP_Screen_DrawHex2 ( short x, short y, unsigned char val, short col )
{
	int tt1, tt2 ;
	
	tt1 = ((val >> 4) & 0x0F) ;
	tt2 = ((val     ) & 0x0F) ;
	Draw_Char_Hankaku( x  , y, (tt1 < 10) ? ('0' + tt1) : ('A' + tt1 - 10), col ) ;
	Draw_Char_Hankaku( x+5, y, (tt2 < 10) ? ('0' + tt2) : ('A' + tt2 - 10), col ) ;
}

void  PSP_Screen_DrawHex8 ( short x, short y, unsigned long val, short col )
{
	int n, tt ;

	for( n=28; n >= 0 ; n -= 4 )
	{
		tt = ((val >> n) & 0x0F) ;
		Draw_Char_Hankaku( x  , y, (tt < 10) ? ('0' + tt) : ('A' + tt - 10), col ) ;
		x += 5 ;
	}
}

void  PSP_Screen_DrawIntN ( short x, short y, unsigned long val, int num, short col ) 
{
	unsigned long IntN[] = { 1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000, 1000000000 } ;

	if( num > 9 ){ num = 9 ; }

	for( ; num > 0 ; num-- )
	{
		Draw_Char_Hankaku(x,y,'0' + (val/IntN[num]),col);
		val = val % IntN[num] ;
		x+=5;
	}

	Draw_Char_Hankaku(x,y,'0' + (val%10),col);
}

void  PSP_Screen_DrawSIntN ( short x, short y, signed long val, int num, short col ) 
{
	unsigned long IntN[] = { 1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000, 1000000000 } ;

	if( num > 9 ){ num = 9 ; }

	if( val < 0 )
	{
		Draw_Char_Hankaku(x,y,'-',col);
		x+=5 ;
		val = -val ;
	}

	for( ; num > 0 ; num-- )
	{
		Draw_Char_Hankaku(x,y,'0' + (val/IntN[num]),col);
		val = val % IntN[num] ;
		x+=5;
	}

	Draw_Char_Hankaku(x,y,'0' + (val%10),col);
}

unsigned short HSV2RGB( int iH, int iS, int iV )
{
	int iF, iM, iN, iK, iR, iG, iB;

	//引数が範囲外の時は最大・最小に変換
	if(iH > 360) iH = 360;
//	if(iS > 200) iS = 200;
//	if(iV > 200) iV = 200;
	if(iH < 0) iH = 0;
	if(iS < 0) iS = 0;
	if(iV < 0) iV = 0;

	// SとVの値を0-200から0-255に変換
//	dS = (double)iS / 200.0 * 255.0;
//	dV = (double)iV / 200.0 * 255.0;

	// 色相が360度の場合は、0度として扱う
	if (iH == 360) iH = 0;

	if (iS == 0)
	{
		iR = iV;
		iG = iV;
		iB = iV;
	}
	else
	{
    	iF = iH;
		while(iF >= 60){iF = iF - 60;}
		iM = (iV * (255 - iS) / 255);
		iN = (iV * (255 - iS * iF / 60) / 255);
		iK = (iV * (255 - iS * (60 - iF) / 60) / 255);

		// 色相によってRGBの生成を分岐
		if(  0<=iH && iH< 60){iR = iV; iG = iK; iB = iM;}
		if( 60<=iH && iH<120){iR = iN; iG = iV; iB = iM;}
		if(120<=iH && iH<180){iR = iM; iG = iV; iB = iK;}
		if(180<=iH && iH<240){iR = iM; iG = iN; iB = iV;}
		if(240<=iH && iH<300){iR = iK; iG = iM; iB = iV;}
		if(300<=iH && iH<360){iR = iV; iG = iM; iB = iN;}
	}

	return PSP_RGB( iR, iG, iB );
}

//////////////////////////////////////////////////////////////////////////////////////////////////
/// Ge
/******************************************************************************/
/* copy from uo_Snes9x for PSP */
/******************************************************************************/
static unsigned int GeInit[] = 
{
	0x01000000, 0x02000000,
	0x10000000, 0x12000000, 0x13000000, 0x15000000, 0x16000000, 0x17000000,
	0x18000000, 0x19000000, 0x1A000000, 0x1B000000, 0x1C000000, 0x1D000000,
	0x1E000000, 0x1F000000,
	0x20000000, 0x21000000, 0x22000000, 0x23000000, 0x24000000, 0x25000000,
	0x26000000, 0x27000000, 0x28000000, 0x2A000000, 0x2B000000, 0x2C000000,
	0x2D000000, 0x2E000000, 0x2F000000,
	0x30000000, 0x31000000, 0x32000000, 0x33000000, 0x36000000, 0x37000000,
	0x38000000, 0x3A000000, 0x3B000000, 0x3C000000, 0x3D000000, 0x3E000000,
	0x3F000000,
	0x40000000, 0x41000000, 0x42000000, 0x43000000, 0x44000000, 0x45000000,
	0x46000000, 0x47000000, 0x48000000, 0x49000000, 0x4A000000, 0x4B000000,
	0x4C000000, 0x4D000000,
	0x50000000, 0x51000000, 0x53000000, 0x54000000, 0x55000000, 0x56000000,
	0x57000000, 0x58000000, 0x5B000000, 0x5C000000, 0x5D000000, 0x5E000000,
	0x5F000000,
	0x60000000, 0x61000000, 0x62000000, 0x63000000, 0x64000000, 0x65000000,
	0x66000000, 0x67000000, 0x68000000, 0x69000000, 0x6A000000, 0x6B000000,
	0x6C000000, 0x6D000000, 0x6E000000, 0x6F000000,
	0x70000000, 0x71000000, 0x72000000, 0x73000000, 0x74000000, 0x75000000,
	0x76000000, 0x77000000, 0x78000000, 0x79000000, 0x7A000000, 0x7B000000,
	0x7C000000, 0x7D000000, 0x7E000000, 0x7F000000,
	0x80000000, 0x81000000, 0x82000000, 0x83000000, 0x84000000, 0x85000000,
	0x86000000, 0x87000000, 0x88000000, 0x89000000, 0x8A000000, 0x8B000000,
	0x8C000000, 0x8D000000, 0x8E000000, 0x8F000000,
	0x90000000, 0x91000000, 0x92000000, 0x93000000, 0x94000000, 0x95000000,
	0x96000000, 0x97000000, 0x98000000, 0x99000000, 0x9A000000, 0x9B000000,
	0x9C000000, 0x9D000000, 0x9E000000, 0x9F000000,
	0xA0000000, 0xA1000000, 0xA2000000, 0xA3000000, 0xA4000000, 0xA5000000,
	0xA6000000, 0xA7000000, 0xA8000000, 0xA9000000, 0xAA000000, 0xAB000000,
	0xAC000000, 0xAD000000, 0xAE000000, 0xAF000000,
	0xB0000000, 0xB1000000, 0xB2000000, 0xB3000000, 0xB4000000, 0xB5000000,
	0xB8000000, 0xB9000000, 0xBA000000, 0xBB000000, 0xBC000000, 0xBD000000,
	0xBE000000, 0xBF000000,
	0xC0000000, 0xC1000000, 0xC2000000, 0xC3000000, 0xC4000000, 0xC5000000,
	0xC6000000, 0xC7000000, 0xC8000000, 0xC9000000, 0xCA000000, 0xCB000000,
	0xCC000000, 0xCD000000, 0xCE000000, 0xCF000000,
	0xD0000000, 0xD2000000, 0xD3000000, 0xD4000000, 0xD5000000, 0xD6000000,
	0xD7000000, 0xD8000000, 0xD9000000, 0xDA000000, 0xDB000000, 0xDC000000,
	0xDD000000, 0xDE000000, 0xDF000000,
	0xE0000000, 0xE1000000, 0xE2000000, 0xE3000000, 0xE4000000, 0xE5000000,
	0xE6000000, 0xE7000000, 0xE8000000, 0xE9000000, 0xEB000000, 0xEC000000,
	0xEE000000,
	0xF0000000, 0xF1000000, 0xF2000000, 0xF3000000, 0xF4000000, 0xF5000000,
	0xF6000000,	0xF7000000, 0xF8000000, 0xF9000000,
	0x0F000000, 0x0C000000
};

void PSP_Screen_InitGe()
{
	int qid;
	sceKernelDcacheWritebackAll();
	qid = sceGeListEnQueue(&GeInit[0], 0, -1, 0);
	sceGeListSync(qid, 0);
	
	unsigned int GEcmd[64];
	// Draw Area
	GEcmd[ 0] = 0x15000000UL | (0 << 10) | 0;
	GEcmd[ 1] = 0x16000000UL | (271 << 10) | 479;
	// Tex Enable
	GEcmd[ 2] = 0x1E000000UL | 1;
	// Viewport
	GEcmd[ 3] = 0x42000000UL | (((int)((float)(480)) >> 8) & 0x00FFFFFF);
	GEcmd[ 4] = 0x43000000UL | (((int)((float)(-272)) >> 8) & 0x00FFFFFF);
	GEcmd[ 5] = 0x44000000UL | (((int)((float)(50000)) >> 8) & 0x00FFFFFF);
	GEcmd[ 6] = 0x45000000UL | (((int)((float)(2048)) >> 8) & 0x00FFFFFF);
	GEcmd[ 7] = 0x46000000UL | (((int)((float)(2048)) >> 8) & 0x00FFFFFF);
	GEcmd[ 8] = 0x47000000UL | (((int)((float)(60000)) >> 8) & 0x00FFFFFF);
	GEcmd[ 9] = 0x4C000000UL | (1024 << 4);
	GEcmd[10] = 0x4D000000UL | (1024 << 4);
	// Model Color
	GEcmd[11] = 0x54000000UL;
	GEcmd[12] = 0x55000000UL | 0xFFFFFF;
	GEcmd[13] = 0x56000000UL | 0xFFFFFF;
	GEcmd[14] = 0x57000000UL | 0xFFFFFF;
	GEcmd[15] = 0x58000000UL | 0xFF;
	// Depth Buffer
	GEcmd[16] = 0x9E000000UL | 0x88000;
	GEcmd[17] = 0x9F000000UL | (0x44 << 16) | 512;
	// Tex
	GEcmd[18] = 0xC2000000UL | (0 << 16) | (0 << 8) | 0;
	GEcmd[19] = 0xC3000000UL | 1;
	GEcmd[20] = 0xC6000000UL | (1 << 8) | 1;
	GEcmd[21] = 0xC7000000UL | (1 << 8) | 1;
	GEcmd[22] = 0xC9000000UL | (0 << 16) | (0 << 8) | 0;
	// Pixel Format
	GEcmd[23] = 0xD2000000UL | 1;
	// Scissor
	GEcmd[24] = 0xD4000000UL | (0 << 10) | 0;
	GEcmd[25] = 0xD5000000UL | (271 << 10) | 479;
	// Depth
	GEcmd[26] = 0xD6000000UL | 10000;
	GEcmd[27] = 0xD7000000UL | 50000;
	// List End
	GEcmd[28] = 0x0F000000UL;
	GEcmd[29] = 0x0C000000UL;
	GEcmd[30] = 0;
	sceKernelDcacheWritebackAll();
	qid = sceGeListEnQueue(&GEcmd[0], &GEcmd[30], -1, 0);
	sceGeListSync(qid, 0);
}
/******************************************************************************/
/* copy from uo_Snes9x for PSP */
/******************************************************************************/

void  PSP_Screen_WaitBlt( void )
{
	if( psp_screen_bltid != -1 )
	{ 
		sceGeListSync( psp_screen_bltid, 0 ); 
		psp_screen_bltid = -1 ;
	}
}

void  PSP_Screen_BitBltGe ( unsigned char* src, int srcW, int srcH, int dstX, int dstY, int dstW, int dstH )
{
	/*
	//static int qid = -1 ;
	unsigned int log2w, log2h;
    static unsigned int GEcmd[16] __attribute__((aligned(64))) = 
	{
		0,0,0,0,0,                                                                             // 0,1,2,3,4,
		0xCB000000UL                                                                         , // 5
		0x12000000UL | (1 << 23) | (0 << 11) | (0 << 9) | (2 << 7) | (0 << 5) | (0 << 2) | 2 , // 6
		0x10000000UL                                                                         , // 7
		0x02000000UL                                                                         , // 8
		0,0,                                                                                   // 9,10,
		0x04000000UL | (6 << 16) | 2                                                         , // 11
		0x0F000000UL                                                                         , // 12
		0x0C000000UL                                                                         , // 13
		0, 0                                                                                   // 14,15
	};

    static short ScreenVertex[10] __attribute__((aligned(64))) =  {
        //0  1  2  3  4    5    6    7    8  9
          0, 0, 0, 0, 0, 256, 224, 480, 272, 0
        };
		*/

    static unsigned int _GEcmd[16] __attribute__((aligned(64))) = 
	{
		0,0,0,0,0,                                                                             // 0,1,2,3,4,
		0xCB000000UL                                                                         , // 5
		0x12000000UL | (1 << 23) | (0 << 11) | (0 << 9) | (2 << 7) | (0 << 5) | (0 << 2) | 2 , // 6
		0x10000000UL                                                                         , // 7
		0x02000000UL                                                                         , // 8
		0,0,                                                                                   // 9,10,
		0x04000000UL | (6 << 16) | 2                                                         , // 11
		0x0F000000UL                                                                         , // 12
		0x0C000000UL                                                                         , // 13
		0, 0                                                                                   // 14,15
	};

    static short ScreenVertex[10] __attribute__((aligned(64))) =  {
        //0  1  2  3  4    5    6    7    8  9
          0, 0, 0, 0, 0, 256, 224, 480, 272, 0
        };

	unsigned int log2w, log2h;

	unsigned int*   GEcmd         = (unsigned int*  )( (unsigned long)_GEcmd        | 0x40000000 ) ;
	unsigned short* _ScreenVertex = (unsigned short*)( (unsigned long) ScreenVertex | 0x40000000 ) ;

	//PSP_Screen_WaitBlt();

    _ScreenVertex[2] = dstX ;
    _ScreenVertex[3] = dstY ;
    _ScreenVertex[5] = srcW ;
    _ScreenVertex[6] = srcH ;
    _ScreenVertex[7] = dstW ; // + dstX 
    _ScreenVertex[8] = dstH ; // + dstY 
	

	// Set Draw Buffer
	GEcmd[ 0] = 0x9C000000UL | ( (unsigned long)PSP_Screen_GetBackBuffer(0,0) & 0x00FFFFFF);
	GEcmd[ 1] = 0x9D000000UL | (((unsigned long)PSP_Screen_GetBackBuffer(0,0) & 0xFF000000) >> 8) | 512;
	// Set Tex Buffer
	GEcmd[ 2] = 0xA0000000UL | ( (unsigned long)src & 0x00FFFFFF);
	GEcmd[ 3] = 0xA8000000UL | (((unsigned long)src & 0xFF000000) >> 8) | 512;
	if (srcW > 256){ log2w = 9; }else{ log2w = 8; }
	if (srcH > 256){ log2h = 9; }else{ log2h = 8; }
	GEcmd[ 4] = 0xB8000000UL | (log2h << 8) | log2w;
	// Tex Flush
//	GEcmd[ 5] = 0xCB000000UL;
	// Set Vertex
//	GEcmd[ 6] = 0x12000000UL | (1 << 23) | (0 << 11) | (0 << 9) | (2 << 7) | (0 << 5) | (0 << 2) | 2;
//	GEcmd[ 7] = 0x10000000UL;
//	GEcmd[ 8] = 0x02000000UL;
	GEcmd[ 9] = 0x10000000UL | (((unsigned long)((void*)&ScreenVertex) & 0xFF000000) >> 8);
	GEcmd[10] = 0x01000000UL | ( (unsigned long)((void*)&ScreenVertex) & 0x00FFFFFF);
	// Draw Vertex
//	GEcmd[11] = 0x04000000UL | (6 << 16) | 2;
	// List End
//	GEcmd[12] = 0x0F000000UL;
//	GEcmd[13] = 0x0C000000UL;
//	GEcmd[14] = 0;

	//sceKernelDcacheWritebackAll();
//	psp_screen_bltid = sceGeListEnQueue(&GEcmd[0], &GEcmd[14], -1, NULL);

	psp_screen_bltid = sceGeListEnQueue(&_GEcmd[0], &_GEcmd[14], -1, NULL);

	//sceGeListSync(psp_screen_bltid, 1);
}


/////////////////////////////////////////////////////////
/// psp_screen.c
//////////////////////////////////////////////////////////////////////////
