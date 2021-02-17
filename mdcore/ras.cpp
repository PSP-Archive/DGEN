//////////////////////////////////////////////////////
/// ras.cpp
//////////////////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include "md.h"
#ifdef _DEBUG
#ifndef _NO_USE_MFC
#include "../fmtest/stdafx.h"
#endif //
#endif //
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
#define  _USE_REVERS_ENDIAN
#ifdef _USE_REVERS_ENDIAN
#else // _USE_REVERS_ENDIAN
#endif // _USE_REVERS_ENDIAN
//////////////////////////////////////////////////////
#ifdef _USE_REVERS_ENDIAN
#define get_word_vdp(p)  *((unsigned short*)(p))
//#define get_word_vdp  get_word
#else // _USE_REVERS_ENDIAN
#define get_word_vdp  get_word
#endif // _USE_REVERS_ENDIAN
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
#define DRAW_START  (16)
//////////////////////////////////////////////////////

#ifdef _USE_REVERS_ENDIAN
//*
#define PIXEL0 (0x0000f000)
#define PIXEL1 (0x00000f00)
#define PIXEL2 (0x000000f0)
#define PIXEL3 (0x0000000f)
#define PIXEL4 (0xf0000000)
#define PIXEL5 (0x0f000000)
#define PIXEL6 (0x00f00000)
#define PIXEL7 (0x000f0000)
#define SHIFT0 (12)
#define SHIFT1 ( 8)
#define SHIFT2 ( 4)
#define SHIFT3 ( 0)
#define SHIFT4 (28)
#define SHIFT5 (24)
#define SHIFT6 (20)
#define SHIFT7 (16)
//*/
#else // _USE_REVERS_ENDIAN
#define PIXEL0 (0x000000f0)
#define PIXEL1 (0x0000000f)
#define PIXEL2 (0x0000f000)
#define PIXEL3 (0x00000f00)
#define PIXEL4 (0x00f00000)
#define PIXEL5 (0x000f0000)
#define PIXEL6 (0xf0000000)
#define PIXEL7 (0x0f000000)
#define SHIFT0 ( 4)
#define SHIFT1 ( 0)
#define SHIFT2 (12)
#define SHIFT3 ( 8)
#define SHIFT4 (20)
#define SHIFT5 (16)
#define SHIFT6 (28)
#define SHIFT7 (24)
#endif // _USE_REVERS_ENDIAN
/*
#define PIXEL0 (0xf0000000)
#define PIXEL1 (0x0f000000)
#define PIXEL2 (0x00f00000)
#define PIXEL3 (0x000f0000)
#define PIXEL4 (0x0000f000)
#define PIXEL5 (0x00000f00)
#define PIXEL6 (0x000000f0)
#define PIXEL7 (0x0000000f)
#define SHIFT0 (28)
#define SHIFT1 (24)
#define SHIFT2 (20)
#define SHIFT3 (16)
#define SHIFT4 (12)
#define SHIFT5 ( 8)
#define SHIFT6 ( 4)
#define SHIFT7 ( 0)
*/
//////////////////////////////////////////////////////
//*
//#define _PLANE_A0_OFF
//#define _PLANE_A1_OFF
//#define _PLANE_B0_OFF
//#define _PLANE_B1_OFF
//#define _WND_0_OFF
//#define _WND_1_OFF
//#define _SPRITE_0_OFF
//#define _SPRITE_1_OFF
//*/
//////////////////////////////////////////////////////


#define set_pixel(pixel,shift, which, tile, palnum) \
	{ \
	tmp = (unsigned char)( (tile >> shift) & mask) ; \
	if( tmp ){ palnum = (unsigned char)( tmp | (unsigned char)which ); } \
	}


/*
#define set_pixel(pixel,shift, which, tile, palnum) \
	{ \
	tmp = (unsigned char)( (tile & pixel) >> shift ) ; \
	if( tmp ){ palnum = (unsigned char)( tmp | (unsigned char)which ); } \
	}
*/

#define draw_lay_fast1( which, tile, palnum ) \
	if( tile )\
	{\
		unsigned char tmp ;\
		if( which & 0x0800 )\
		{\
			which = ((which >> 9) & 0x0030) ;\
			set_pixel( PIXEL7, SHIFT7, which, tile, palnum[0] ) ;  \
			set_pixel( PIXEL6, SHIFT6, which, tile, palnum[1] ) ;  \
			set_pixel( PIXEL5, SHIFT5, which, tile, palnum[2] ) ;  \
			set_pixel( PIXEL4, SHIFT4, which, tile, palnum[3] ) ;  \
			set_pixel( PIXEL3, SHIFT3, which, tile, palnum[4] ) ;  \
			set_pixel( PIXEL2, SHIFT2, which, tile, palnum[5] ) ;  \
			set_pixel( PIXEL1, SHIFT1, which, tile, palnum[6] ) ;  \
			set_pixel( PIXEL0, SHIFT0, which, tile, palnum[7] ) ;  \
		}else{\
			which = ((which >> 9) & 0x0030) ;\
			set_pixel( PIXEL0, SHIFT0, which, tile, palnum[0] ) ;  \
			set_pixel( PIXEL1, SHIFT1, which, tile, palnum[1] ) ;  \
			set_pixel( PIXEL2, SHIFT2, which, tile, palnum[2] ) ;  \
			set_pixel( PIXEL3, SHIFT3, which, tile, palnum[3] ) ;  \
			set_pixel( PIXEL4, SHIFT4, which, tile, palnum[4] ) ;  \
			set_pixel( PIXEL5, SHIFT5, which, tile, palnum[5] ) ;  \
			set_pixel( PIXEL6, SHIFT6, which, tile, palnum[6] ) ;  \
			set_pixel( PIXEL7, SHIFT7, which, tile, palnum[7] ) ;  \
		}\
	}

//*
#define draw_lay_fast2( which, tile, palnum ) \
	palnum -= 8 ; \
	if( tile )\
	{\
		unsigned char tmp ;\
		set_pixel( PIXEL7, SHIFT7, which, tile, palnum[0] ) ;  \
		set_pixel( PIXEL6, SHIFT6, which, tile, palnum[1] ) ;  \
		set_pixel( PIXEL5, SHIFT5, which, tile, palnum[2] ) ;  \
		set_pixel( PIXEL4, SHIFT4, which, tile, palnum[3] ) ;  \
		set_pixel( PIXEL3, SHIFT3, which, tile, palnum[4] ) ;  \
		set_pixel( PIXEL2, SHIFT2, which, tile, palnum[5] ) ;  \
		set_pixel( PIXEL1, SHIFT1, which, tile, palnum[6] ) ;  \
		set_pixel( PIXEL0, SHIFT0, which, tile, palnum[7] ) ;  \
	}


#define draw_lay_fast3( which, tile, palnum ) \
	if( tile )\
	{\
		unsigned char tmp ;\
		set_pixel( PIXEL0, SHIFT0, which, tile, palnum[0] ) ;  \
		set_pixel( PIXEL1, SHIFT1, which, tile, palnum[1] ) ;  \
		set_pixel( PIXEL2, SHIFT2, which, tile, palnum[2] ) ;  \
		set_pixel( PIXEL3, SHIFT3, which, tile, palnum[3] ) ;  \
		set_pixel( PIXEL4, SHIFT4, which, tile, palnum[4] ) ;  \
		set_pixel( PIXEL5, SHIFT5, which, tile, palnum[5] ) ;  \
		set_pixel( PIXEL6, SHIFT6, which, tile, palnum[6] ) ;  \
		set_pixel( PIXEL7, SHIFT7, which, tile, palnum[7] ) ;  \
	}\
	palnum +=8 ;
//*/


/*
#define draw_lay_fast2( which, tile, palnum ) \
	if( tile )\
	{\
		unsigned char tmp ; \
		set_pixel( PIXEL0, SHIFT0, which, tile, *palnum ) ; palnum--; \
		set_pixel( PIXEL1, SHIFT1, which, tile, *palnum ) ; palnum--; \
		set_pixel( PIXEL2, SHIFT2, which, tile, *palnum ) ; palnum--; \
		set_pixel( PIXEL3, SHIFT3, which, tile, *palnum ) ; palnum--; \
		set_pixel( PIXEL4, SHIFT4, which, tile, *palnum ) ; palnum--; \
		set_pixel( PIXEL5, SHIFT5, which, tile, *palnum ) ; palnum--; \
		set_pixel( PIXEL6, SHIFT6, which, tile, *palnum ) ; palnum--; \
		set_pixel( PIXEL7, SHIFT7, which, tile, *palnum ) ; palnum--; \
	}\
	else{ palnum -= 8 ; }


#define draw_lay_fast3( which, tile, palnum ) \
	if( tile )\
	{\
		unsigned char tmp ; \
		set_pixel( PIXEL0, SHIFT0, which, tile, *palnum ) ; palnum++; \
		set_pixel( PIXEL1, SHIFT1, which, tile, *palnum ) ; palnum++; \
		set_pixel( PIXEL2, SHIFT2, which, tile, *palnum ) ; palnum++; \
		set_pixel( PIXEL3, SHIFT3, which, tile, *palnum ) ; palnum++; \
		set_pixel( PIXEL4, SHIFT4, which, tile, *palnum ) ; palnum++; \
		set_pixel( PIXEL5, SHIFT5, which, tile, *palnum ) ; palnum++; \
		set_pixel( PIXEL6, SHIFT6, which, tile, *palnum ) ; palnum++; \
		set_pixel( PIXEL7, SHIFT7, which, tile, *palnum ) ; palnum++; \
	}\
	else{ palnum +=8 ; }
	
*/

/// for Shadow/Hilight
#define set_pixel_sh0(pixel,shift, which, tile, palnum) \
	tmp = (unsigned char)( (tile >> shift) & mask) ; \
	if( tmp ){ palnum = (unsigned char)( tmp | (unsigned char)which ); } 

#define set_pixel_sh1(pixel,shift, which, tile, palnum) \
	tmp = (unsigned char)( (tile >> shift) & mask) ; \
	if( tmp ){ palnum = (unsigned char)( tmp | (unsigned char)which ); } 
//	else     { *(palnum) &= 0x7F ; }

#define set_pixel_shs(pixel,shift, which, tile, palnum) \
	tmp = (unsigned char)( (tile >> shift) & mask) ; \
	if( tmp )\
	{\
		tmp = (unsigned char)( tmp | (unsigned char)which ) ; \
		if(      tmp == 0x3F ){ palnum |= 0x80 ; } \
		else if( tmp == 0x3E ){ palnum |= 0x40 ; } \
		else                  { palnum  = tmp  ; } \
	}

/*
/// for Shadow/Hilight
#define set_pixel_sh0(pixel,shift, which, tile, palnum) \
	tmp = (unsigned char)( (tile & pixel) >> shift ) ; \
	if( tmp ){ palnum = (unsigned char)( tmp | (unsigned char)which ); } 

#define set_pixel_sh1(pixel,shift, which, tile, palnum) \
	tmp = (unsigned char)( (tile & pixel) >> shift ) ; \
	if( tmp ){ palnum = (unsigned char)( tmp | (unsigned char)which ); } 
//	else     { *(palnum) &= 0x7F ; }

#define set_pixel_shs(pixel,shift, which, tile, palnum) \
	tmp = (unsigned char)( (tile & pixel) >> shift ) ; \
	if( tmp )\
	{\
		tmp = (unsigned char)( tmp | (unsigned char)which ) ; \
		if(      tmp == 0x3F ){ palnum |= 0x80 ; } \
		else if( tmp == 0x3E ){ palnum |= 0x40 ; } \
		else                  { palnum  = tmp  ; } \
	}
*/

#define set_shadow_on( palnum ) \
			palnum[0] |= 0x80 ; \
			palnum[1] |= 0x80 ; \
			palnum[2] |= 0x80 ; \
			palnum[3] |= 0x80 ; \
			palnum[4] |= 0x80 ; \
			palnum[5] |= 0x80 ; \
			palnum[6] |= 0x80 ; \
			palnum[7] |= 0x80 ;

#define set_shadow_cancel( palnum ) \
			palnum[0] &= 0x7F ; \
			palnum[1] &= 0x7F ; \
			palnum[2] &= 0x7F ; \
			palnum[3] &= 0x7F ; \
			palnum[4] &= 0x7F ; \
			palnum[5] &= 0x7F ; \
			palnum[6] &= 0x7F ; \
			palnum[7] &= 0x7F ;

#define draw_lay_sh1( which, tile, palnum ) \
	if( tile )\
	{\
		unsigned char tmp ; \
		if( which & 0x0800 )\
		{\
			which = ((which >> 9) & 0x0030) | shadow ;\
			set_pixel_sh0( PIXEL7, SHIFT7, which, tile, palnum[0] ) ;  \
			set_pixel_sh0( PIXEL6, SHIFT6, which, tile, palnum[1] ) ;  \
			set_pixel_sh0( PIXEL5, SHIFT5, which, tile, palnum[2] ) ;  \
			set_pixel_sh0( PIXEL4, SHIFT4, which, tile, palnum[3] ) ;  \
			set_pixel_sh0( PIXEL3, SHIFT3, which, tile, palnum[4] ) ;  \
			set_pixel_sh0( PIXEL2, SHIFT2, which, tile, palnum[5] ) ;  \
			set_pixel_sh0( PIXEL1, SHIFT1, which, tile, palnum[6] ) ;  \
			set_pixel_sh0( PIXEL0, SHIFT0, which, tile, palnum[7] ) ;  \
		}\
		else\
		{\
			which = ((which >> 9) & 0x0030) | shadow ;\
			set_pixel_sh0( PIXEL0, SHIFT0, which, tile, palnum[0] ) ;  \
			set_pixel_sh0( PIXEL1, SHIFT1, which, tile, palnum[1] ) ;  \
			set_pixel_sh0( PIXEL2, SHIFT2, which, tile, palnum[2] ) ;  \
			set_pixel_sh0( PIXEL3, SHIFT3, which, tile, palnum[3] ) ;  \
			set_pixel_sh0( PIXEL4, SHIFT4, which, tile, palnum[4] ) ;  \
			set_pixel_sh0( PIXEL5, SHIFT5, which, tile, palnum[5] ) ;  \
			set_pixel_sh0( PIXEL6, SHIFT6, which, tile, palnum[6] ) ;  \
			set_pixel_sh0( PIXEL7, SHIFT7, which, tile, palnum[7] ) ;  \
		}\
	}

#define draw_lay_sh2( which, tile, palnum ) \
	if( tile )\
	{\
		unsigned char tmp ; \
		if( which & 0x0800 )\
		{\
			which = ((which >> 9) & 0x0030) | 0x80 ;\
			set_pixel_sh0( PIXEL7, SHIFT7, which, tile, palnum[0] ) ;  \
			set_pixel_sh0( PIXEL6, SHIFT6, which, tile, palnum[1] ) ;  \
			set_pixel_sh0( PIXEL5, SHIFT5, which, tile, palnum[2] ) ;  \
			set_pixel_sh0( PIXEL4, SHIFT4, which, tile, palnum[3] ) ;  \
			set_pixel_sh0( PIXEL3, SHIFT3, which, tile, palnum[4] ) ;  \
			set_pixel_sh0( PIXEL2, SHIFT2, which, tile, palnum[5] ) ;  \
			set_pixel_sh0( PIXEL1, SHIFT1, which, tile, palnum[6] ) ;  \
			set_pixel_sh0( PIXEL0, SHIFT0, which, tile, palnum[7] ) ;  \
		}\
		else\
		{\
			which = ((which >> 9) & 0x0030) | 0x80 ;\
			set_pixel_sh0( PIXEL0, SHIFT0, which, tile, palnum[0] ) ;  \
			set_pixel_sh0( PIXEL1, SHIFT1, which, tile, palnum[1] ) ;  \
			set_pixel_sh0( PIXEL2, SHIFT2, which, tile, palnum[2] ) ;  \
			set_pixel_sh0( PIXEL3, SHIFT3, which, tile, palnum[3] ) ;  \
			set_pixel_sh0( PIXEL4, SHIFT4, which, tile, palnum[4] ) ;  \
			set_pixel_sh0( PIXEL5, SHIFT5, which, tile, palnum[5] ) ;  \
			set_pixel_sh0( PIXEL6, SHIFT6, which, tile, palnum[6] ) ;  \
			set_pixel_sh0( PIXEL7, SHIFT7, which, tile, palnum[7] ) ;  \
		}\
	}\
	palnum += 8;

#define draw_lay_sh3( which, tile, palnum ) \
	if( tile )\
	{\
		unsigned char tmp ; \
		if( which & 0x0800 )\
		{\
			which = ((which >> 9) & 0x0030) ;\
			set_pixel_sh1( PIXEL7, SHIFT7, which, tile, palnum[0] ) ; \
			set_pixel_sh1( PIXEL6, SHIFT6, which, tile, palnum[1] ) ; \
			set_pixel_sh1( PIXEL5, SHIFT5, which, tile, palnum[2] ) ; \
			set_pixel_sh1( PIXEL4, SHIFT4, which, tile, palnum[3] ) ; \
			set_pixel_sh1( PIXEL3, SHIFT3, which, tile, palnum[4] ) ; \
			set_pixel_sh1( PIXEL2, SHIFT2, which, tile, palnum[5] ) ; \
			set_pixel_sh1( PIXEL1, SHIFT1, which, tile, palnum[6] ) ; \
			set_pixel_sh1( PIXEL0, SHIFT0, which, tile, palnum[7] ) ; \
		}\
		else\
		{\
			which = ((which >> 9) & 0x0030) ;\
			set_pixel_sh1( PIXEL0, SHIFT0, which, tile, palnum[0] ) ; \
			set_pixel_sh1( PIXEL1, SHIFT1, which, tile, palnum[1] ) ; \
			set_pixel_sh1( PIXEL2, SHIFT2, which, tile, palnum[2] ) ; \
			set_pixel_sh1( PIXEL3, SHIFT3, which, tile, palnum[3] ) ; \
			set_pixel_sh1( PIXEL4, SHIFT4, which, tile, palnum[4] ) ; \
			set_pixel_sh1( PIXEL5, SHIFT5, which, tile, palnum[5] ) ; \
			set_pixel_sh1( PIXEL6, SHIFT6, which, tile, palnum[6] ) ; \
			set_pixel_sh1( PIXEL7, SHIFT7, which, tile, palnum[7] ) ; \
		}\
	}\
	else if( shadow )\
	{\
		/* shadow cancel */ \
		set_shadow_cancel( palnum ) ; \
	}

/*
#define draw_lay_shs1( which, tile, palnum ) \
	if( tile )\
	{\
		unsigned char tmp ; \
		set_pixel_shs( PIXEL0, SHIFT0, which, tile, *palnum ) ; palnum--;  \
		set_pixel_shs( PIXEL1, SHIFT1, which, tile, *palnum ) ; palnum--;  \
		set_pixel_shs( PIXEL2, SHIFT2, which, tile, *palnum ) ; palnum--;  \
		set_pixel_shs( PIXEL3, SHIFT3, which, tile, *palnum ) ; palnum--;  \
		set_pixel_shs( PIXEL4, SHIFT4, which, tile, *palnum ) ; palnum--;  \
		set_pixel_shs( PIXEL5, SHIFT5, which, tile, *palnum ) ; palnum--;  \
		set_pixel_shs( PIXEL6, SHIFT6, which, tile, *palnum ) ; palnum--;  \
		set_pixel_shs( PIXEL7, SHIFT7, which, tile, *palnum ) ; palnum--;  \
	}\
	else{ palnum -= 8 ; }
	
*/

//*
#define draw_lay_shs1( which, tile, palnum ) \
	palnum -= 8 ; \
	if( tile )\
	{\
		unsigned char tmp ; \
		set_pixel_shs( PIXEL7, SHIFT7, which, tile, palnum[0] ) ;  \
		set_pixel_shs( PIXEL6, SHIFT6, which, tile, palnum[1] ) ;  \
		set_pixel_shs( PIXEL5, SHIFT5, which, tile, palnum[2] ) ;  \
		set_pixel_shs( PIXEL4, SHIFT4, which, tile, palnum[3] ) ;  \
		set_pixel_shs( PIXEL3, SHIFT3, which, tile, palnum[4] ) ;  \
		set_pixel_shs( PIXEL2, SHIFT2, which, tile, palnum[5] ) ;  \
		set_pixel_shs( PIXEL1, SHIFT1, which, tile, palnum[6] ) ;  \
		set_pixel_shs( PIXEL0, SHIFT0, which, tile, palnum[7] ) ;  \
	}
//*/

#define draw_lay_shs2( which, tile, palnum ) \
	if( tile )\
	{\
		unsigned char tmp ; \
		set_pixel_shs( PIXEL0, SHIFT0, which, tile, palnum[0] ) ;  \
		set_pixel_shs( PIXEL1, SHIFT1, which, tile, palnum[1] ) ;  \
		set_pixel_shs( PIXEL2, SHIFT2, which, tile, palnum[2] ) ;  \
		set_pixel_shs( PIXEL3, SHIFT3, which, tile, palnum[3] ) ;  \
		set_pixel_shs( PIXEL4, SHIFT4, which, tile, palnum[4] ) ;  \
		set_pixel_shs( PIXEL5, SHIFT5, which, tile, palnum[5] ) ;  \
		set_pixel_shs( PIXEL6, SHIFT6, which, tile, palnum[6] ) ;  \
		set_pixel_shs( PIXEL7, SHIFT7, which, tile, palnum[7] ) ;  \
	}\
	palnum +=8 ;

//////////////////////////////////////////////////////
//*
//#define _PLANE_A0_OFF
//#define _PLANE_A1_OFF
//#define _PLANE_B0_OFF
//#define _PLANE_B1_OFF
//#define _WND_0_OFF
//#define _WND_1_OFF
//#define _SPRITE_0_OFF
//#define _SPRITE_1_OFF
//*/
#define draw_none( which, tile, palnum ) 

#define draw_splite_norm0  draw_lay_fast2
#define draw_splite_norm1  draw_lay_fast3
#define draw_splite_high0  draw_lay_shs1
#define draw_splite_high1  draw_lay_shs2

#define draw_wnd_norm0     draw_lay_fast1
#define draw_wnd_norm1     draw_lay_fast1

#define draw_lay_norm0     draw_lay_sh1
#define draw_lay_norm1     draw_lay_fast1
#define draw_lay_high1     draw_lay_sh3

#define draw_lay_vscr0     draw_lay_fast1
#define draw_lay_vscr1     draw_lay_fast1
#define draw_lay_vscr0h    draw_lay_sh2
#define draw_lay_vscr1h    draw_lay_sh3
#define draw_lay_vscr1ha   draw_lay_sh3




//////////////////////////////////////////////////////
#define get_tile( which, sh, scan_a, scan ) \
	*(unsigned*)( ((which & 0x1000) ? scan_a : scan) + ((which & 0x07FF) << sh) );

//////////////////////////////////////////////////////
inline short md_core::vdp_get_xsize()
{
	switch( vdp_reg[16] & 3 )
	{
	//default :
	//case 0 : xsize =  32 << 1 ; break ;
	case 1 : 
	case 2 : return  64 << 1 ; 
	case 3 : return 128 << 1 ; 
	}

	return 32 << 1 ;
}

inline short md_core::vdp_get_ysize()
{
	switch( (vdp_reg[16] >> 4) & 3 )
	{
	//default :
	//case 0 : ysize =  32 ; break ;
	case 1 : return  64 ; 
	case 2 : return  64 ; 
	case 3 : return 128 ; 
	}
	return 32 ;
}

inline unsigned char*  md_core::vdp_get_xscrolls( int line )
{
	unsigned char* xscrolls = vdp_vram + ((vdp_reg[13]<<10) & 0xFC00);

	switch(vdp_reg[11] & 3)
	{
	case 1: xscrolls += ((line &  7) << 2) ; break; // per tile
	case 2: xscrolls += ((line & ~7) << 2) ; break; // per tile
	case 3: xscrolls += ( line       << 2) ; break; // per line
	default: break;
	}

	return xscrolls ;
}

inline unsigned char  md_core::vdp_is_plane_b ( int line )
{
	if( vdp_reg[18] & 0x80 )
	{
		return ( (line >> 3) < (vdp_reg[18]&0x1f) ) ;
	}

	return ( (line >> 3) >= (vdp_reg[18]&0x1f) ) ;
}

//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
inline void md_core::vdp_update_sprite_order( int line )
{
	unsigned short which ;
	int x, y, xend, ysize, yoff, n ;
	unsigned char *sprite;
	unsigned next = 0;
	unsigned char* sprite_base = vdp_vram + ( vdp_reg[5] << 9 ) ;
	signed short sc0 = -1, sc1 = -1 ;

	n =  0 ;
	sprite = sprite_base + (0 << 3) ;
	for( ; ; )
	{
		//if( (which >> 15) == front )
		{
			y     = get_word_vdp(sprite)          ;

#ifdef _USE_REVERS_ENDIAN
			xend  = sprite[3]                    ;
#else // _USE_REVERS_ENDIAN
			xend  = sprite[2]                    ;
#endif // _USE_REVERS_ENDIAN

			x     = get_word_vdp(sprite + 6) & 0x01FF ;
			ysize = xend & 0x03                   ;
			xend  = ((xend << 1) & 0x18) + x      ;

			if(vdp_reg[12] & 2){ y = (y & 0x3fe) >> 1 ; }else{ y &= 0x1ff ; }
			y    -= 0x80                          ; 
			yoff  = (line - y)                    ;

			if( (0 <= yoff) && (yoff <= ((ysize<<3)+7)) )
			{
				if( (x == 0) && (xend == 0) )
				{
					// Find Sprite cancel !!
					//if( front == 0      ){ break ; }
					//if( which & (1<<15) ){ break ; }
					break ; 
				}

				which  = get_word_vdp(sprite + 4);

				{
					if( which & 0x8000 )
					{
						sc1++ ;
						sprite_order_1[sc1] = next ; 
					}
					else
					{
						sc0++ ;
						sprite_order_0[sc0] = next ; 
					}
				}
			}
		}

#ifdef _USE_REVERS_ENDIAN
		next = sprite[2] ;
#else // _USE_REVERS_ENDIAN
		next = sprite[3] ;
#endif // _USE_REVERS_ENDIAN

		if( next == 0 ){ break ; }

		n++ ;
		if( n >= MAX_SPRITES ){ break ; }
		//if( n >= 40 ){ break ; }

		sprite = sprite_base + (next << 3) ;
	}

	sprite_count_0 = sc0 ;
	sprite_count_1 = sc1 ;

//	TRACE( "SPR : %d - %d - %d, %d\n", line_count, n, sprite_count_0, sprite_count_1 );
}

inline void md_core::vdp_draw_sprites ( int line, int front )
{
	unsigned short which ;
	int tx, ty, x, y, xend, ysize, yoff, i ;
	unsigned char *sprite;
	unsigned long tile ;
	int sh1, sh2 ;
	unsigned char *va; //, *vb ;
	unsigned char* palnum_t ;
	unsigned char* sprite_order ;
	unsigned char* sprite_base = vdp_vram + ( vdp_reg[5] << 9 ) ;
	unsigned char mask = 0x0F ;

	if( front )
	{
		sprite_order = sprite_order_1 ;
		i = sprite_count_1 ;
	}
	else
	{
		sprite_order = sprite_order_0 ;
		i = sprite_count_0 ;
	}

	// interlace
	if(vdp_reg[12] & 2){ sh1 = 6 ; sh2 = 3 ; }else{ sh1 = 5 ; sh2 = 2 ; }

	for( ; i >= 0; --i )
	{
		sprite = sprite_base + (sprite_order[i] << 3) ;

		//which = get_word_vdp(sprite + 4);

		//if( (which >> 15) == front )
		{
			y     = get_word_vdp(sprite)         ;
#ifdef _USE_REVERS_ENDIAN
			xend  = sprite[3]                    ;
#else // _USE_REVERS_ENDIAN
			xend  = sprite[2]                    ;
#endif // _USE_REVERS_ENDIAN
			x     = get_word_vdp(sprite + 6) & 0x1ff ;
			ysize = xend & 0x03                  ;
			xend  = ((xend << 1) & 0x18)         ;
			if(vdp_reg[12] & 2){ y = (y & 0x3fe) >> 1; }else{ y &= 0x1ff; }
			y    -= 0x80                         ; 
			yoff  = (line - y)                   ;

			//if( (0 <= yoff) && (yoff <= ((ysize<<3)+7)) )
			{
				// Narrow mode?
				//if( vdp_reg[12] & 1 ){ x -= 0x80 ; }else{ x += 32 - 0x80 ; }
				x -= 0x80 ;

				if( x < 320 )
				{
					xend += x ;

					// Render if this sprite's on this line
					if( xend > -8 ) //&& x < 320 && yoff >= 0 && yoff <= (ysize<<3)+7)
					{
						which = get_word_vdp(sprite + 4);

						// y flipped?
						if( which & 0x1000 )
						{
							va = (vdp_vram + (((yoff & 7)^7) << sh2));
							ty = (which & 0x7ff) - (yoff >> 3) + ysize ;
						}
						else
						{
							va = (vdp_vram + (((yoff & 7)  ) << sh2));
							ty = (which & 0x7ff) + (yoff >> 3)         ;
						}

						++ysize;

						// x flipped?
						if( which & 0x0800 )
						{
							//palnum_t = draw_palnum + 8 + xend ;
							palnum_t = draw_cashe + DRAW_START + 8 + xend ;

							which = ((which >> 9) & 0x0030) ;

							//*
							for( ; xend > 320 ; xend -= 8)
							{
								ty       += ysize ;
								palnum_t -= 8     ;
							}
							//*/

							if( x < -7 ){ x = -7 ; }

							//TRACE( "SP = %d,%d, %d, %d\n", x, xend, xend - x, (xend - x)/8 );
							//*
							if( (vdp_reg[12] & 8) == 0 )
							{
								for(tx = xend; tx >= x ; tx -= 8)
								{
									tile = *(unsigned*)(va + ((ty & 0x7ff) << sh1) );
									draw_splite_norm0( which, tile, palnum_t ) ;
									ty       += ysize ;
								}
							}
							else
							{
								for(tx = xend; tx >= x ; tx -= 8)
								{
									tile = *(unsigned*)(va + ((ty & 0x7ff) << sh1) );
									draw_splite_high0( which, tile, palnum_t ) ;
									ty       += ysize ;
								}
							}
							//*/
						} 
						else 
						{
							palnum_t = draw_cashe + DRAW_START + x ;

							which = ((which >> 9) & 0x0030) ;
							
							if( xend > (320) ){ xend = 320 ; }

							//*
							for( ; x < -7 ; x += 8)
							{
								ty       += ysize ;
								palnum_t += 8     ;
							}
							//*/

							//*
							if( (vdp_reg[12] & 8) == 0 )
							{
								for(tx = x; tx <= xend ; tx += 8)
								{
									tile = *(unsigned*)(va + ((ty & 0x7ff) << sh1) );
									draw_splite_norm1( which, tile, palnum_t ) ;
									ty       += ysize ;
								}
							}
							else
							{
								for(tx = x; tx <= xend ; tx += 8)
								{
									tile = *(unsigned*)(va + ((ty & 0x7ff) << sh1) );
									draw_splite_high1( which, tile, palnum_t ) ;
									ty       += ysize ;
								}
							}
							//*/
						}

					} // ( xend > -8 )
				}
			}
		}
	}
}

inline void md_core::vdp_draw_bg ( )
{
	unsigned long tile ;
	tile = vdp_reg[7] & 0x3F ; //backcolor
	tile = (tile << 24) | (tile << 16) | (tile << 8) | tile ;
	for( int x=0 ; x < MAX_PAL_CACHE/4 ; x++ ){ ((unsigned long*)draw_cashe)[x] = tile ; }

	//TRACE( "vdp_draw_bg - %08X, %d\n", draw_cashe, line_count );
}


#if 1
void md_core::vdp_draw_line_norm( int line )
{
	short xsize, ysize;
	short x, w, xpos, ed ;
	unsigned char* xscrolls ;
	unsigned char *tile0, *tile1, *tile2 ;
	int xoff0, xoff1, yoff0, yoff1, xoff_mask;
	int xoff0a, xoff1a ;
	int xscroll0, xscroll1, yscroll0, yscroll1;
	unsigned char p0, sh1, sh2 ;
	unsigned char shadow ;
	unsigned char *scan0, *scan1, *scan0a, *scan1a, *scan2, *scan2a ;
	unsigned short which, laynum ;
	unsigned long  tile ;
	unsigned char* palnum_t ;
	register unsigned char mask = 0x0F ;


//for( ; line <= line_count ; line++ )
{
	vdp_update_sprite_order( line ) ;


	// config
	xsize     = vdp_get_xsize    () ;
	ysize     = vdp_get_ysize    () ;
	xoff_mask = xsize - 1;
	shadow    = ( vdp_reg[12] & 8 ) << 4 ;
	//if(vdp_reg[12] & 1){ w = 40; xstart = -8; } else { w = 32; xstart = 24; }
	if(vdp_reg[12] & 1){ w = 40; } else { w = 32; }
	xpos = ((vdp_reg[17] & 0x1f) << 1) ; if( xpos > 40 ){ xpos = 40 ; }

	// for X
	xscrolls   = vdp_get_xscrolls ( line ) ;
	xscroll0   = (short)get_word_vdp(xscrolls  ) ; //& 0x03FF ;
	xscroll1   = (short)get_word_vdp(xscrolls+2) ; //& 0x03FF ;
	xoff0      = ((-(xscroll0>>3) - 1)<<1) & xoff_mask;
	xoff1      = ((-(xscroll1>>3) - 1)<<1) & xoff_mask;
	xscroll0   = (xscroll0 & 7) ;
	xscroll1   = (xscroll1 & 7) ;

	// for Y
	p0       = vdp_is_plane_b   ( line ) ;
	yscroll0 = get_word_vdp(vdp_vsram  );
	yscroll1 = get_word_vdp(vdp_vsram+2);

	switch( vdp_reg[12] & 6 )
	{
	// Interlace
	case 2 : 
	case 6 : 
		yscroll0 >>= 1; 
		yscroll1 >>= 1; 
		sh1 = 6 ; sh2 = 3 ;
		break ;

	// Interlace Double
	//case 6 : 
	//	sh1 = 6 ; sh2 = 3 ;
	//	break ;

	// Normal
	default :
		sh1 = 5 ; sh2 = 2 ;
		//sh1 = 6 ; sh2 = 3 ;
		break ;
	}

	// Offset for the line
	yscroll0 += line;
	yscroll1 += line;

	yoff0  = (yscroll0>>3) & (ysize - 1);
	yoff1  = (yscroll1>>3) & (ysize - 1);
	tile0  = vdp_vram + (vdp_reg[ 2]<<10) + xsize * yoff0 ;
	tile1  = vdp_vram + (vdp_reg[ 4]<<13) + xsize * yoff1 ;
	tile2  = vdp_vram + (vdp_reg[ 3]<<10) + ( ((line >> 3) & 0x3F) * ( (vdp_reg[12] & 1) ? 128 : 64 ) );
	scan0  = (vdp_vram + (( yscroll0 & 7)    << sh2) );
	scan1  = (vdp_vram + (( yscroll1 & 7)    << sh2) );
	scan0a = (vdp_vram + (((yscroll0 & 7)^7) << sh2) );
	scan1a = (vdp_vram + (((yscroll1 & 7)^7) << sh2) );
	scan2  = (vdp_vram + (( line     & 7)    << sh2) );
	scan2a = (vdp_vram + (((line     & 7)^7) << sh2) );
	laynum = 0 ;

	vdp_draw_bg() ;

_DoDRAW :
	//* Plane A - 0
	xoff1a = xoff1 ;
	palnum_t = draw_cashe + 8 + xscroll1 ;
	//palnum_t = draw_cashe + 8 ;

	for( x=0; x <= w; x++ )
	{
		which = get_word_vdp( tile1 + xoff1a );
		xoff1a = (xoff1a + 2) & xoff_mask ;
		
		if( (which >> 15) == laynum )
		{
			tile = get_tile( which, sh1, scan1a, scan1 ) ;
			if( laynum == 0 )
				{ draw_lay_norm0( which, tile, palnum_t ) ; }
			else{ draw_lay_high1( which, tile, palnum_t ) ; }
		}
		palnum_t += 8 ;
	}


	if( p0 )
	{
		//* Plane B - 0
		palnum_t = draw_cashe + 8 + xscroll0 ;
		//palnum_t = draw_cashe + 8 + xscroll0 + xscroll1 ;
		if( vdp_reg[17] & 0x80 ) // Don't draw where the window will be
		{
			xoff0a = xoff0 ;
			x  = 0 ;
			ed = xpos ;
		}
		else
		{
			xoff0a = (xoff0 + 2 * xpos) & xoff_mask ;
			palnum_t += 8 * xpos ;
			x  = xpos ;
			ed = w    ;
		}

		if( laynum == 0 )
		{
			for( ; x <= ed ; x++ )
			{
				which = get_word_vdp( tile0 + xoff0a ); 
				xoff0a = (xoff0a + 2) & xoff_mask ;

				if( (which >> 15) == laynum )
				{
					tile = get_tile( which, sh1, scan0a, scan0 ) ;
					draw_lay_norm0( which, tile, palnum_t ) ;
				}
				else if( shadow ){ set_shadow_cancel( palnum_t ) ; }
				palnum_t += 8 ;
			}
		}
		else
		{
			for( ; x <= ed ; x++ )
			{
				which = get_word_vdp( tile0 + xoff0a ); 
				xoff0a = (xoff0a + 2) & xoff_mask ;

				if( (which >> 15) == laynum )
				{
					tile = get_tile( which, sh1, scan0a, scan0 ) ;
					draw_lay_norm1( which, tile, palnum_t ) ;
				}
				palnum_t += 8 ;
			}
		}

		if( vdp_reg[17] & 0x80 ) // Don't draw where the window will be
		{
			x        = xpos ;
			ed       = w    ;
			xoff0a   = (2 * xpos) & xoff_mask ;
			palnum_t = draw_cashe + DRAW_START + 8 * xpos ;
		}
		else
		{
			x        = 0     ;
			ed       = xpos  ;
			xoff0a   = 0     ;
			palnum_t = draw_cashe + DRAW_START ;
		}
	}
	else
	{
		x        = 0   ;
		ed       = w   ;
		xoff0a   = 0   ;
		palnum_t = draw_cashe + DRAW_START ;
	}

	// window
	//*
	for( ; x < ed ; x++ )
	{
		which = get_word_vdp( tile2 + xoff0a );
		xoff0a = (xoff0a + 2) & xoff_mask ;
		
		if( (which >> 15) == laynum )
		{
			tile = get_tile( which, sh1, scan2a, scan2 ) ;
			draw_wnd_norm0( which, tile, palnum_t ) ;
		}
		palnum_t += 8 ;
	}
	//*/

	vdp_draw_sprites ( line, laynum ); 

	if( laynum == 0 )
	{
		laynum = 1 ;
		goto _DoDRAW ;
	}

	vdp_draw_pal2dest( line ) ;

}// for

}
#endif //

#if 1
#define set_tile_info( tiles, xoff, yscr, tile, scan ) \
	{\
			unsigned char* p = (tiles + xoff);\
			which = get_word_vdp( p ) ; \
			xoff  = (xoff + 2) & xoff_mask ;\
			tile  = which ;\
			scan  = ((which & 0x1000) ? (((yscr & 7)^7) << sh2) : ((yscr & 7) << sh2) );\
	}

#define set_tile_info_2x( tiles, xoff, yscr, tile1, scan1, tile2, scan2 ) \
		{\
			unsigned char* p1 = (tiles + xoff);\
			xoff  = (xoff + 2) & xoff_mask ;\
			which1 = get_word_vdp( p1 ) ; \
			tile1  = which1 ;\
			scan1  = ((which1 & 0x1000) ? (((yscr & 7)^7) << sh2) : ((yscr & 7) << sh2) );\
			\
			unsigned char* p2 = (tiles + xoff);\
			xoff  = (xoff + 2) & xoff_mask ;\
			which2 = get_word_vdp( p2 ) ; \
			tile2  = which2 ;\
			scan2  = ((which2 & 0x1000) ? (((yscr & 7)^7) << sh2) : ((yscr & 7) << sh2) );\
		}


void md_core::vdp_draw_line_vscr( int line )
{
	short xsize, ysize;
	short x, w, xpos, ed ;
	unsigned char *xscrolls ;
	unsigned char *tiles0  , *tiles1, *tile2, *scan2, *scan2a  ;
	int xoff0, xoff1, yoff0, yoff1, xoff_mask;
	int xoff0a, xoff1a ;
	int xscroll0, xscroll1, yscroll0, yscroll1;
	int p0, sh1, sh2 ;
	unsigned char shadow ;
	unsigned short which , laynum ;
	unsigned short which1, which2 ;
	unsigned long  tile, which2x  ;
	unsigned char* palnum_t ;
	unsigned short  scan0[42],  scan1[42] ;
	unsigned short  tile0[42],  tile1[42] ;
	register unsigned char mask = 0x0F ;

//for( ; line <= line_count ; line++ )
{
	vdp_update_sprite_order( line ) ;

	vdp_draw_bg() ;

	// config
	p0       = vdp_is_plane_b   ( line ) ;
	xsize    = vdp_get_xsize    () ;
	ysize    = vdp_get_ysize    () ;
	shadow   = ( vdp_reg[12] & 8 ) << 4 ;
	if(vdp_reg[12] & 1){ w = 40; } else { w = 32; }
	xpos = ((vdp_reg[17] & 0x1f) << 1) ; if( xpos > 40 ){ xpos = 40 ; }

	// for X
	xscrolls   = vdp_get_xscrolls ( line ) ;
	xoff_mask  = xsize - 1;
	xscroll0   = get_word_vdp(xscrolls  ) ;//& 0x3FFF ;
	xscroll1   = get_word_vdp(xscrolls+2) ;//& 0x3FFF ;
	xoff0      = ((-(xscroll0>>3) - 1)<<1) & xoff_mask;
	xoff1      = ((-(xscroll1>>3) - 1)<<1) & xoff_mask;
	xscroll0   = (xscroll0 & 7) ;
	xscroll1   = (xscroll1 & 7) ;

	tiles0   = vdp_vram +  (vdp_reg[ 2]<<10);
	tiles1   = vdp_vram +  (vdp_reg[ 4]<<13);

	switch( vdp_reg[12] & 6 )
	{
	// Interlace
	case 2 : 
	case 6 : 
		sh1 = 6 ; sh2 = 3 ;
		break ;

	// Interlace Double
	//case 6 : 
	//	sh1 = 6 ; sh2 = 3 ;
	//	break ;

	// Normal
	default :
		sh1 = 5 ; sh2 = 2 ;
		//sh1 = 6 ; sh2 = 3 ;
		break ;
	}

	{
		unsigned char* vsram0 = vdp_vsram     ;
		unsigned char* vsram1 = vdp_vsram + 2 ;
		int            shs    = (vdp_reg[12] & 2) >> 1 ;
		int            xs     ;
		switch( vdp_reg[16] & 3 )
		{
		default : xs = 6 ; break ;
		case 1  : 
		case 2  : xs = 7 ; break ;
		case 3  : xs = 8 ; break ;
		}

		xoff0a = xoff0 ;
		xoff1a = xoff1 ;

		x = 0 ;
		yscroll1 = get_word_vdp( vsram1 ); //vsram1 += 4 ;
		yscroll0 = get_word_vdp( vsram0 ); //vsram0 += 4 ;
		yscroll1 >>= shs ; 
		yscroll0 >>= shs ; 
		yscroll1 += line ;
		yscroll0 += line ;
		yoff1 = ((yscroll1>>3) & (ysize - 1)) << xs ;
		yoff0 = ((yscroll0>>3) & (ysize - 1)) << xs ;
		
		if( (vdp_reg[12] & 1) == 0 )
		{ 
			set_tile_info_2x( tiles1 + yoff1, xoff1a, yscroll1, tile1[x], scan1[x], tile1[x+1], scan1[x+1] ) ;
			set_tile_info_2x( tiles0 + yoff0, xoff0a, yscroll0, tile0[x], scan0[x], tile0[x+1], scan0[x+1] ) ;
			x += 2 ; 
		}
		else
		{
			set_tile_info( tiles1 + yoff1, xoff1a, yscroll1, tile1[x+0], scan1[x+0] ) ;
			set_tile_info( tiles0 + yoff0, xoff0a, yscroll0, tile0[x+0], scan0[x+0] ) ;
			x += 1 ; 
		}

		for( ; x <= w ; x += 2 )
		{
			yscroll1 = get_word_vdp( vsram1 ); vsram1 += 4 ;
			yscroll0 = get_word_vdp( vsram0 ); vsram0 += 4 ;
			yscroll1 >>= shs ; 
			yscroll0 >>= shs ; 
			yscroll1 += line ;
			yscroll0 += line ;
			yoff1 = ((yscroll1>>3) & (ysize - 1)) << xs ;
			yoff0 = ((yscroll0>>3) & (ysize - 1)) << xs ;
			set_tile_info_2x( tiles1 + yoff1, xoff1a, yscroll1, tile1[x], scan1[x], tile1[x+1], scan1[x+1] ) ;
			set_tile_info_2x( tiles0 + yoff0, xoff0a, yscroll0, tile0[x], scan0[x], tile0[x+1], scan0[x+1] ) ;
		}
	}

	// window
	tile2  = vdp_vram + (vdp_reg[ 3]<<10) + ( ((line >> 3) & 0x3F) * ( (vdp_reg[12] & 1) ? 128 : 64 ) );
	scan2  = (vdp_vram + (( line     & 7)    << sh2) );
	scan2a = (vdp_vram + (((line     & 7)^7) << sh2) );
	laynum = 0 ;

_DoDRAW :

	//* Plane A - 0
	palnum_t = draw_cashe + 8 + xscroll1 ;

	/*
	for( x=0; x <= w ; x++ )
	{
		which1 = tile1[x] ;
		if( (which1 >> 15) == laynum )
		{
			tile = *(unsigned*)( vdp_vram + scan1[x] + ((which1 & 0x7ff) << sh1) ) ;

			if( laynum == 0 )
				{ draw_lay_norm0( which1, tile, palnum_t ) ; }
			else{ draw_lay_high1( which1, tile, palnum_t ) ; }
		}
		palnum_t += 8 ;
	}
	//*/

	//*
	for( x=0; x < w ; x+=2 )
	{
		//which2x = ((unsigned long*)tile1)[x >> 1] ;
		which2x = *((unsigned long*)&tile1[x]) ;
		which1  = (unsigned short)which2x ;
		which2  = (unsigned short)(which2x >> 16) ;

		if( (which1 >> 15) == laynum )
		{
			tile = *(unsigned*)( vdp_vram + scan1[x] + ((which1 & 0x7ff) << sh1) ) ;

			if( laynum == 0 )
				{ draw_lay_norm0( which1, tile, palnum_t ) ; }
			else{ draw_lay_high1( which1, tile, palnum_t ) ; }
		}
		//else if( (laynum == 1) && shadow ){ set_shadow_on( palnum_t ) ; }
		palnum_t += 8 ;

		if( (which2 >> 15) == laynum )
		{
			tile = *(unsigned*)( vdp_vram + scan1[x+1] + ((which2 & 0x7ff) << sh1) ) ;

			if( laynum == 0 )
				{ draw_lay_norm0( which2, tile, palnum_t ) ; }
			else{ draw_lay_high1( which2, tile, palnum_t ) ; }
		}
		//else if( (laynum == 1) && shadow ){ set_shadow_on( palnum_t ) ; }
		palnum_t += 8 ;
	}

	{
		which1 = tile1[w] ;
		if( (which1 >> 15) == laynum )
		{
			tile = *(unsigned*)( vdp_vram + scan1[w] + ((which1 & 0x7ff) << sh1) ) ;

			if( laynum == 0 )
				{ draw_lay_norm0( which1, tile, palnum_t ) ; }
			else{ draw_lay_high1( which1, tile, palnum_t ) ; }
		}
		//else if( (laynum == 1) && shadow ){ set_shadow_on( palnum_t ) ; }
		palnum_t += 8 ;
	}
	//*/

	if( p0 )
	{
		//* Plane B - 0
		palnum_t = draw_cashe + 8 + xscroll0 ;
		if( vdp_reg[17] & 0x80 ) // Don't draw where the window will be
		{
			x  = 0 ;
			ed = xpos ;
		}
		else
		{
			palnum_t += 8 * xpos ;
			x  = xpos ;
			ed = w    ;
		}

		if( laynum == 0 )
		{
			for( ; x <= ed ; x++ )
			{
				which = tile0[x] ;

				if( (which >> 15) == laynum )
				{
					tile = *(unsigned*)( vdp_vram + scan0[x] + ((which & 0x7ff) << sh1) ) ;
					draw_lay_norm0( which, tile, palnum_t ) ;
				}
				else if( shadow ){ set_shadow_cancel( palnum_t ) ; }
				palnum_t += 8 ; 
			}
		}
		else
		{
			for( ; x <= ed ; x++ )
			{
				which = tile0[x] ;

				if( (which >> 15) == laynum )
				{
					tile = *(unsigned*)( vdp_vram + scan0[x] + ((which & 0x7ff) << sh1) ) ;
					draw_lay_norm1( which, tile, palnum_t ) ;
				}
				//else if( shadow ){ set_shadow_on( palnum_t ) ; }
				palnum_t += 8 ; 
			}
		}

		if( vdp_reg[17] & 0x80 ) // Don't draw where the window will be
		{
			x        = xpos ;
			ed       = w    ;
			xoff0a   = (2 * (xpos)) & xoff_mask ;
			palnum_t = draw_cashe + DRAW_START + 8 * xpos ;
		}
		else
		{
			x        = 0     ;
			ed       = xpos  ;
			xoff0a   = 0     ;
			palnum_t = draw_cashe + DRAW_START ;
		}
	}
	else
	{
		x        = 0  ;
		ed       = w  ;
		xoff0a   = 0  ;
		palnum_t = draw_cashe + DRAW_START ;
	}

	for( ; x < ed ; x++ )
	{
		which = get_word_vdp( tile2 + xoff0a );
		xoff0a = (xoff0a + 2) & xoff_mask ;
		
		if( (which >> 15) == laynum )
		{
			tile = get_tile( which, sh1, scan2a, scan2 ) ;
			draw_wnd_norm0( which, tile, palnum_t ) ;
		}
		palnum_t += 8 ;
	}

	vdp_draw_sprites ( line, laynum ); 

	if( laynum == 0 )
	{
		laynum = 1 ;
		goto _DoDRAW ;
	}

	vdp_draw_pal2dest( line ) ;

}// for

}

#endif


#if 0
#define set_tile_info( tiles, xoff, yscr, tile, scan ) \
	{\
			unsigned char* p = (tiles + xoff);\
			which = get_word_vdp( p ) ; \
			xoff  = (xoff + 2) & xoff_mask ;\
			tile  = which ;\
			scan  = vdp_vram + ((which & 0x1000) ? (((yscr & 7)^7) << sh2) : ((yscr & 7) << sh2) );\
	}

#define set_tile_info_2x( tiles, xoff, yscr, tile1, scan1, tile2, scan2 ) \
		{\
			unsigned char* p1 = (tiles + xoff);\
			xoff  = (xoff + 2) & xoff_mask ;\
			which1 = get_word_vdp( p1 ) ; \
			tile1  = which1 ;\
			scan1  = vdp_vram + ((which1 & 0x1000) ? (((yscr & 7)^7) << sh2) : ((yscr & 7) << sh2) );\
			\
			unsigned char* p2 = (tiles + xoff);\
			xoff  = (xoff + 2) & xoff_mask ;\
			which2 = get_word_vdp( p2 ) ; \
			tile2  = which2 ;\
			scan2  = vdp_vram + ((which2 & 0x1000) ? (((yscr & 7)^7) << sh2) : ((yscr & 7) << sh2) );\
		}


void md_core::vdp_draw_line_vscr( int line )
{
	short xsize, ysize;
	short x, w, xpos, ed ;
	unsigned char *xscrolls ;
	unsigned char *tiles0  , *tiles1, *tile2, *scan2, *scan2a  ;
	int xoff0, xoff1, yoff0, yoff1, xoff_mask;
	int xoff0a, xoff1a ;
	int xscroll0, xscroll1, yscroll0, yscroll1;
	int p0, sh1, sh2 ;
	unsigned char shadow ;
	unsigned short which , laynum ;
	unsigned short which1, which2 ;
	unsigned long  tile, which2x  ;
	unsigned char* palnum_t ;
	unsigned char  *scan0[42], *scan1[42] ;
	unsigned short  tile0[42],  tile1[42] ;
	register unsigned char mask = 0x0F ;

//for( ; line <= line_count ; line++ )
{
	vdp_update_sprite_order( line ) ;

	vdp_draw_bg() ;

	// config
	p0       = vdp_is_plane_b   ( line ) ;
	xsize    = vdp_get_xsize    () ;
	ysize    = vdp_get_ysize    () ;
	shadow   = ( vdp_reg[12] & 8 ) << 4 ;
	if(vdp_reg[12] & 1){ w = 40; } else { w = 32; }
	xpos = ((vdp_reg[17] & 0x1f) << 1) ; if( xpos > 40 ){ xpos = 40 ; }

	// for X
	xscrolls   = vdp_get_xscrolls ( line ) ;
	xoff_mask  = xsize - 1;
	xscroll0   = get_word_vdp(xscrolls  ) ;//& 0x3FFF ;
	xscroll1   = get_word_vdp(xscrolls+2) ;//& 0x3FFF ;
	xoff0      = ((-(xscroll0>>3) - 1)<<1) & xoff_mask;
	xoff1      = ((-(xscroll1>>3) - 1)<<1) & xoff_mask;
	xscroll0   = (xscroll0 & 7) ;
	xscroll1   = (xscroll1 & 7) ;

	tiles0   = vdp_vram +  (vdp_reg[ 2]<<10);
	tiles1   = vdp_vram +  (vdp_reg[ 4]<<13);

	switch( vdp_reg[12] & 6 )
	{
	// Interlace
	case 2 : 
	case 6 : 
		sh1 = 6 ; sh2 = 3 ;
		break ;

	// Interlace Double
	//case 6 : 
	//	sh1 = 6 ; sh2 = 3 ;
	//	break ;

	// Normal
	default :
		sh1 = 5 ; sh2 = 2 ;
		//sh1 = 6 ; sh2 = 3 ;
		break ;
	}

	{
		unsigned char* vsram0 = vdp_vsram     ;
		unsigned char* vsram1 = vdp_vsram + 2 ;
		int            shs    = (vdp_reg[12] & 2) >> 1 ;
		xoff0a = xoff0 ;
		xoff1a = xoff1 ;

		x = 0 ;
		yscroll1 = get_word_vdp( vsram1 ); //vsram1 += 4 ;
		yscroll0 = get_word_vdp( vsram0 ); //vsram0 += 4 ;
		yscroll1 >>= shs ; 
		yscroll0 >>= shs ; 
		yscroll1 += line ;
		yscroll0 += line ;
		yoff1 = (yscroll1>>3) & (ysize - 1);
		yoff0 = (yscroll0>>3) & (ysize - 1);

		set_tile_info( tiles1 + xsize * yoff1, xoff1a, yscroll1, tile1[x+0], scan1[x+0] ) ;
		set_tile_info( tiles0 + xsize * yoff0, xoff0a, yscroll0, tile0[x+0], scan0[x+0] ) ;
		x += 1 ; 
		if( (vdp_reg[12] & 1) == 0 )
		{ 
			set_tile_info( tiles1 + xsize * yoff1, xoff1a, yscroll1, tile1[x+0], scan1[x+0] ) ;
			set_tile_info( tiles0 + xsize * yoff0, xoff0a, yscroll0, tile0[x+0], scan0[x+0] ) ;
			x += 1 ; 
		}

		ed = w ;
		for( ; x <= ed ; x += 2 )
		{
			yscroll1 = get_word_vdp( vsram1 ); vsram1 += 4 ;
			yscroll0 = get_word_vdp( vsram0 ); vsram0 += 4 ;
			yscroll1 >>= shs ; 
			yscroll0 >>= shs ; 
			yscroll1 += line ;
			yscroll0 += line ;
			yoff1 = (yscroll1>>3) & (ysize - 1);
			yoff0 = (yscroll0>>3) & (ysize - 1);
			set_tile_info_2x( tiles1 + xsize * yoff1, xoff1a, yscroll1, tile1[x], scan1[x], tile1[x+1], scan1[x+1] ) ;
			set_tile_info_2x( tiles0 + xsize * yoff0, xoff0a, yscroll0, tile0[x], scan0[x], tile0[x+1], scan0[x+1] ) ;
		}
	}

	// window
	tile2  = vdp_vram + (vdp_reg[ 3]<<10) + ( ((line >> 3) & 0x3F) * ( (vdp_reg[12] & 1) ? 128 : 64 ) );
	scan2  = (vdp_vram + (( line     & 7)    << sh2) );
	scan2a = (vdp_vram + (((line     & 7)^7) << sh2) );
	laynum = 0 ;

_DoDRAW :

	//* Plane A - 0
	palnum_t = draw_cashe + 8 + xscroll1 ;

	for( x=0; x < w ; x+=2 )
	{
		//which2x = ((unsigned long*)tile1)[x >> 1] ;
		which2x = *((unsigned long*)&tile1[x]) ;
		which1  = (unsigned short)which2x ;
		which2  = (unsigned short)(which2x >> 16) ;

		if( (which1 >> 15) == laynum )
		{
			tile = *(unsigned*)( scan1[x] + ((which1 & 0x7ff) << sh1) ) ;

			if( laynum == 0 )
				{ draw_lay_norm0( which1, tile, palnum_t ) ; }
			else{ draw_lay_high1( which1, tile, palnum_t ) ; }
		}
		//else if( (laynum == 1) && shadow ){ set_shadow_on( palnum_t ) ; }
		palnum_t += 8 ;

		if( (which2 >> 15) == laynum )
		{
			tile = *(unsigned*)( scan1[x+1] + ((which2 & 0x7ff) << sh1) ) ;

			if( laynum == 0 )
				{ draw_lay_norm0( which2, tile, palnum_t ) ; }
			else{ draw_lay_high1( which2, tile, palnum_t ) ; }
		}
		//else if( (laynum == 1) && shadow ){ set_shadow_on( palnum_t ) ; }
		palnum_t += 8 ;
	}

	{
		which1 = tile1[w] ;
		if( (which1 >> 15) == laynum )
		{
			tile = *(unsigned*)( scan1[w] + ((which1 & 0x7ff) << sh1) ) ;

			if( laynum == 0 )
				{ draw_lay_norm0( which1, tile, palnum_t ) ; }
			else{ draw_lay_high1( which1, tile, palnum_t ) ; }
		}
		//else if( (laynum == 1) && shadow ){ set_shadow_on( palnum_t ) ; }
		palnum_t += 8 ;
	}

	if( p0 )
	{
		//* Plane B - 0
		palnum_t = draw_cashe + 8 + xscroll0 ;
		if( vdp_reg[17] & 0x80 ) // Don't draw where the window will be
		{
			x  = 0 ;
			ed = xpos ;
		}
		else
		{
			palnum_t += 8 * xpos ;
			x  = xpos ;
			ed = w    ;
		}

		if( laynum == 0 )
		{
			for( ; x <= ed ; x++ )
			{
				which = tile0[x] ;

				if( (which >> 15) == laynum )
				{
					tile = *(unsigned*)( scan0[x] + ((which & 0x7ff) << sh1) ) ;
					draw_lay_norm0( which, tile, palnum_t ) ;
				}
				else if( shadow ){ set_shadow_cancel( palnum_t ) ; }
				palnum_t += 8 ; 
			}
		}
		else
		{
			for( ; x <= ed ; x++ )
			{
				which = tile0[x] ;

				if( (which >> 15) == laynum )
				{
					tile = *(unsigned*)( scan0[x] + ((which & 0x7ff) << sh1) ) ;
					draw_lay_norm1( which, tile, palnum_t ) ;
				}
				//else if( shadow ){ set_shadow_on( palnum_t ) ; }
				palnum_t += 8 ; 
			}
		}

		if( vdp_reg[17] & 0x80 ) // Don't draw where the window will be
		{
			x        = xpos ;
			ed       = w    ;
			xoff0a   = (2 * (xpos)) & xoff_mask ;
			palnum_t = draw_cashe + DRAW_START + 8 * xpos ;
		}
		else
		{
			x        = 0     ;
			ed       = xpos  ;
			xoff0a   = 0     ;
			palnum_t = draw_cashe + DRAW_START ;
		}
	}
	else
	{
		x        = 0  ;
		ed       = w  ;
		xoff0a   = 0  ;
		palnum_t = draw_cashe + DRAW_START ;
	}

	for( ; x < ed ; x++ )
	{
		which = get_word_vdp( tile2 + xoff0a );
		xoff0a = (xoff0a + 2) & xoff_mask ;
		
		if( (which >> 15) == laynum )
		{
			tile = get_tile( which, sh1, scan2a, scan2 ) ;
			draw_wnd_norm0( which, tile, palnum_t ) ;
		}
		palnum_t += 8 ;
	}

	vdp_draw_sprites ( line, laynum ); 

	if( laynum == 0 )
	{
		laynum = 1 ;
		goto _DoDRAW ;
	}

	vdp_draw_pal2dest( line ) ;

}// for

}

#endif

inline void md_core::vdp_draw_pal2dest ( int line )
{
	int x, w ;
	unsigned long  pal  ;
	unsigned char* dest_t ;
#ifdef WIN32
	unsigned char* dest = ( draw_ptr + (draw_pitch * (239 - line )) ) ;
#else // WIN32
	unsigned char* dest = ( draw_ptr + (draw_pitch * (line )) ) ;
#endif // WIN32

#ifdef _USE_DRAW_BPP
	unsigned short* dp = (unsigned short*)draw_palette ;
	switch( draw_bpp )
	{
	case 1 :
		break ;

	case 3 :
		{
			unsigned long* dp = (unsigned long*)draw_palette ;

		}
		break ;

	case 4 :
		{
			unsigned long* dp = (unsigned long*)draw_palette ;

			if( (vdp_reg[12] & 1) != 0 )
			{
				dest_t = dest ; 
				w = ( 40 * 8 + DRAW_START ) ;
			}
			else
			{
				unsigned char* dest_t1 = dest ;
				unsigned char* dest_t2 = dest + ( 40 - 4 ) * 8 * 4 ;
				for( x=0; x < 16 ; x++ )
				{ 
					*((unsigned long*)(dest_t1+ 0)) = 0 ;
					*((unsigned long*)(dest_t1+ 4)) = 0 ;
					*((unsigned long*)(dest_t2+ 0)) = 0 ;
					*((unsigned long*)(dest_t2+ 4)) = 0 ;
					dest_t1 += 8 ;
					dest_t2 += 8 ;
				}

				dest_t = dest + (8 * 4 * 4) ;
				w = ( 32 * 8 + DRAW_START ) ;
			}

			for( x = DRAW_START ; x < w ; x+=4 )
			{ 
				pal = *((unsigned long*)&draw_cashe[x]) ;
				*((unsigned long*)(dest_t+ 0)) = dp[ (unsigned char)(pal >>  0) ] ; 
				*((unsigned long*)(dest_t+ 4)) = dp[ (unsigned char)(pal >>  8) ] ; 
				*((unsigned long*)(dest_t+ 8)) = dp[ (unsigned char)(pal >> 16) ] ; 
				*((unsigned long*)(dest_t+12)) = dp[ (unsigned char)(pal >> 24) ] ; 
				dest_t += 16 ;
			}		
		}
		break ;

	case 0 :
	case 2 :
		{

#endif // _USE_DRAW_BPP

	unsigned short* dp = (unsigned short*)draw_palette ;

	if( (vdp_reg[12] & 1) != 0 )
	{
		dest_t = dest ; 
		w = ( 40 * 8 + DRAW_START ) ;
	}
	else
	{
		unsigned char* dest_t1 = dest ;
		unsigned char* dest_t2 = dest + ( 40 - 4 ) * 8 * DRAW_BPP ;
		for( x=0; x < 8 ; x++ )
		{ 
			*((unsigned long*)(dest_t1+ 0)) = 0 ;
			*((unsigned long*)(dest_t1+ 4)) = 0 ;
			*((unsigned long*)(dest_t2+ 0)) = 0 ;
			*((unsigned long*)(dest_t2+ 4)) = 0 ;
			dest_t1 += 8 ;
			dest_t2 += 8 ;
		}

		dest_t = dest + (8 * 4 * DRAW_BPP) ;
		w = ( 32 * 8 + DRAW_START ) ;
	}

	for( x = DRAW_START ; x < w ; x+=4 )
	{ 
		pal = *((unsigned long*)&draw_cashe[x]) ;
		*((unsigned short*)(dest_t+0)) = dp[ (unsigned char)(pal >>  0) ] ; 
		*((unsigned short*)(dest_t+2)) = dp[ (unsigned char)(pal >>  8) ] ; 
		*((unsigned short*)(dest_t+4)) = dp[ (unsigned char)(pal >> 16) ] ; 
		*((unsigned short*)(dest_t+6)) = dp[ (unsigned char)(pal >> 24) ] ; 
		dest_t += 8 ;
	}

#ifdef _USE_DRAW_BPP
		}
		break ;
	}
#endif // _USE_DRAW_BPP
}

void md_core::vdp_update_palette( ) 
{
	int i;
	// Set the destination in the bmap
	//dest = bits->data + (bits->pitch * (line + 8) ) ; //+ 16);
	// If bytes per pixel hasn't yet been set, do it
	//Bpp = 2;

	// If the palette's been changed, update it
	if( dirt_flag & DIRT_CRAM )
	{
		dirt_flag &= ~DIRT_CRAM ;


#ifdef WIN32 ////////////////////////////////////////////////////////////
#ifdef _USE_DRAW_BPP

#ifdef _USE_REVERS_ENDIAN
		switch( draw_bpp )
		{
		case 1 : break ;
		case 0 :
		case 2 :
			{
				unsigned short* ptr = (unsigned short*)draw_palette ;
				for(i = 0; i < 128; i += 2)
				{
					*ptr  = ((vdp_cram[i+1]&0x0e) << 1 ) |
							((vdp_cram[i+0]&0xe0) << 2 ) |
							((vdp_cram[i+0]&0x0e) << 11);

					*ptr++ ;
				}
			}
			break ;
		case 3 :
		case 4 :
			{
				unsigned long* ptr = (unsigned long*)draw_palette ;
				for(i = 0; i < 128; i += 2)
				{
					*ptr  = ((vdp_cram[i+1]&0x0e) << 4 ) |
							((vdp_cram[i+0]&0xe0) << 8 ) |
							((vdp_cram[i+0]&0x0e) << 20) ;

					*ptr++ ;
				}
			}
			break ;
		}
#else // _USE_REVERS_ENDIAN
		for(i = 0; i < 128; i += 2)
		{
			*ptr  = ((vdp_cram[i  ]&0x0e) << 1 ) |
					((vdp_cram[i+1]&0xe0) << 2 ) |
					((vdp_cram[i+1]&0x0e) << 11);

			*ptr++ ;
		}
#endif // _USE_REVERS_ENDIAN

#else // _USE_DRAW_BPP
		unsigned short* ptr = (unsigned short*)draw_palette ;
#ifdef _USE_REVERS_ENDIAN
		for(i = 0; i < 128; i += 2)
		{
			*ptr  = ((vdp_cram[i+1]&0x0e) << 1 ) |
					((vdp_cram[i+0]&0xe0) << 2 ) |
					((vdp_cram[i+0]&0x0e) << 11);

			*ptr++ ;
		}
#else // _USE_REVERS_ENDIAN
		for(i = 0; i < 128; i += 2)
		{
			*ptr  = ((vdp_cram[i  ]&0x0e) << 1 ) |
					((vdp_cram[i+1]&0xe0) << 2 ) |
					((vdp_cram[i+1]&0x0e) << 11);

			*ptr++ ;
		}
#endif // _USE_REVERS_ENDIAN
#endif // _USE_DRAW_BPP
#else // WIN32 //////////////////////////////////////////////////////////
		unsigned short* ptr = (unsigned short*)draw_palette ;
#ifdef _USE_REVERS_ENDIAN
		for(i = 0; i < 128; i += 2)
		{
			*ptr  = ((vdp_cram[i+0]&0x0e) << 1 ) |
					((vdp_cram[i+0]&0xe0) << 2 ) |
					((vdp_cram[i+1]&0x0e) << 11);

			*ptr++ ;
		}
#else // _USE_REVERS_ENDIAN
		for(i = 0; i < 128; i += 2)
		{
			*ptr  = ((vdp_cram[i+1]&0x0e) << 1 ) |
					((vdp_cram[i+1]&0xe0) << 2 ) |
					((vdp_cram[i  ]&0x0e) << 11);

			*ptr++ ;
		}
#endif // _USE_REVERS_ENDIAN
#endif // WIN32 /////////////////////////////////////////////////////////

#ifdef _USE_DRAW_BPP
	switch( draw_bpp )
	{
		case 1 : break ;

		case 3 :
		case 4 :
			{
				unsigned long* ptr = (unsigned long*)draw_palette ;

				if( palette_mode == 1 )
				{
					for( i = 0; i < 64; i++ )
					{
						ptr[i     ] = ptr[i] | (ptr[i] >> 2) ;
						ptr[i+0x40] = ptr[i+0xC0] = ptr[i] | 0x1C1C1C ;
						ptr[i+0x80] = (ptr[i] >> 1) & 0x7F7F7F ;
					}
				}
				else if( palette_mode ==2 )
				{
					for( i = 0; i < 64; i++ )
					{
						ptr[i     ] = ptr[i] | 0x0C0C0C ;
						ptr[i+0x40] = ptr[i+0xC0] = ptr[i] | 0x1C1C1C ;
						ptr[i+0x80] = ((ptr[i] >> 1) | 0x0C0C0C) & 0x7F7F7F ;
					}
				}
				else
				{
					for( i = 0; i < 64; i++ )
					{
						ptr[i+0x40] = ptr[i+0xC0] = ptr[i] | 0x1C1C1C ;
						ptr[i+0x80] = (ptr[i] >> 1) & 0x7F7F7F ;
					}
				}
			}
			break ;

		case 0 :
		case 2 :
		{
#endif // _USE_DRAW_BPP

		unsigned short* ptr = (unsigned short*)draw_palette ;

		if( palette_mode == 1 )
		{
			for( i = 0; i < 64; i++ )
			{
				ptr[i     ] = ptr[i] | (ptr[i] >> 2) ;
				ptr[i+0x40] = ptr[i+0xC0] = ptr[i] | 0x5EF7 ;
				ptr[i+0x80] = (ptr[i] >> 1) & 0x3DEF ;
			}
		}
		else if( palette_mode ==2 )
		{
			for( i = 0; i < 64; i++ )
			{
				ptr[i     ] = ptr[i] | 0x0C63 ;
				ptr[i+0x40] = ptr[i+0xC0] = ptr[i] | 0x5EF7 ;
				ptr[i+0x80] = ((ptr[i] >> 1) | 0x0C63) & 0x3DEF ;
			}
		}
		else
		{
			for( i = 0; i < 64; i++ )
			{
				ptr[i+0x40] = ptr[i+0xC0] = ptr[i] | 0x5EF7 ;
				ptr[i+0x80] = (ptr[i] >> 1) & 0x3DEF ;
			}
		}

#ifdef _USE_DRAW_BPP
		}
		break ;
	}
#endif // _USE_DRAW_BPP

	}
}

void md_core::vdp_draw_line_clear ( int line  )
{
	int i ;
	unsigned *dest_t = (unsigned*)( draw_ptr + (draw_pitch * (line)) ) ;


#ifdef _USE_DRAW_BPP
	switch( draw_bpp )
	{
		case 1 : break ;

		case 3 :
		case 4 :
			{
				if( palette_mode == 2 )
				{
					for(i = 0; i < (80 * draw_bpp); ++i){ dest_t[i] = 0x0C0C0C ; }
				}
				else
				{
					for(i = 0; i < (80 * draw_bpp); ++i){ dest_t[i] = 0 ; }
				}
			}
			break ;

		case 0 :
		case 2 :
		{
#endif // _USE_DRAW_BPP

		if( palette_mode == 2 )
		{
			for(i = 0; i < (80 * DRAW_BPP); ++i){ dest_t[i] = 0x0C630C63 ; }
		}
		else
		{
			for(i = 0; i < (80 * DRAW_BPP); ++i){ dest_t[i] = 0 ; }
		}

#ifdef _USE_DRAW_BPP
		}
		break ;
	}
#endif // _USE_DRAW_BPP

}

void md_core::vdp_draw_interace( int v_line ) 
{
	/*
	int n, i = vdp_render_skip ? 1 : 0 ;
	unsigned short *src1   = (unsigned short*)( draw_ptr + (draw_pitch * (i-1 + 8)) ) ;
	unsigned short *src2   = (unsigned short*)( draw_ptr + (draw_pitch * (i+1 + 8)) ) ;
	unsigned short *dest_t = (unsigned short*)( draw_ptr + (draw_pitch * (i   + 8)) ) ;
	unsigned long s11, s12, s13, s21, s22, s23 ;

	for( n=0; n < v_line / 2 ; n++ )
	{
		s11 = ( src1[0] >> 1 ) & 0x3DEF ;
		s21 = ( src2[0] >> 1 ) & 0x3DEF ;

		s13 = (( s11 + s21 ) >> 1 ) & 0x3DEF ;

		for( i=1; i < (80 * DRAW_BPP *2) ; ++i)
		{
			s12 = ( src1[i] >> 1 ) & 0x3DEF ;
			s22 = ( src2[i] >> 1 ) & 0x3DEF ;

			s23 = (( s12 + s22 ) >> 1 ) & 0x3DEF ;

			dest_t[i] = ( s13 + s23 )  ; 

			s13 = s23 ;
			//s11 = s12 ;
			//s21 = s22 ;
			//s12 = s13 ;
			//s22 = s23 ;
		}

		//dest_t[0] = dest_t[1] ;

		src1   += draw_pitch  ;
		src2   += draw_pitch  ;
		dest_t += draw_pitch  ;
	}

	vdp_render_skip = !vdp_render_skip ;
	*/

	/*
	int n, i = vdp_render_skip ? 1 : 0 ;
	unsigned *src1   = (unsigned*)( draw_ptr + (draw_pitch * (i-1 + 8)) ) ;
	unsigned *src2   = (unsigned*)( draw_ptr + (draw_pitch * (i+1 + 8)) ) ;
	unsigned *dest_t = (unsigned*)( draw_ptr + (draw_pitch * (i   + 8)) ) ;

	for( n=0; n < v_line / 2 ; n++ )
	{
		for( i=0; i < (80 * DRAW_BPP); ++i)
		{
			unsigned long s1 = (src1[i] >> 1) & 0x3DEF3DEF ;
			unsigned long s2 = (src2[i] >> 1) & 0x3DEF3DEF ;
			dest_t[i] = s1 + s2 ; 
		}

		src1   += draw_pitch / 2 ;
		src2   += draw_pitch / 2 ;
		dest_t += draw_pitch / 2 ;
	}

	vdp_render_skip = !vdp_render_skip ;
	//*/
}

void md_core::vdp_update_line( int line )
{
	if( vdp_reg[1] & 0x40 )
	{
		//if( vdp_render_skip || (line == 0) )
		{
			vdp_update_palette() ;

			//draw_cashe = &draw_cashe_t[ line_count ][0] ;
			draw_cashe = (unsigned char*)_SCRATCHPAD(0); //(unsigned char*)0x00010000 ; // &draw_cashe_t[ 0 ][0] ;


			if( (vdp_reg[11] & 4) == 0 )
			{
				vdp_draw_line_norm ( line ) ;
			}
			else
			{
				vdp_draw_line_vscr ( line ) ;
			}
		}

		/*
		else
		{
			int i ;
			unsigned *src    = (unsigned*)( draw_ptr + (draw_pitch * (line + 7)) ) ;
			unsigned *dest_t = (unsigned*)( draw_ptr + (draw_pitch * (line + 8)) ) ;
			for(i = 0; i < (80 * DRAW_BPP); ++i){ dest_t[i] = src[i] ; }
			
			//vdp_draw_line_clear( line ) ;
		}

		if( line != 0 )
			vdp_render_skip = !vdp_render_skip ;
		//*/
	} 
	else 
	{
		vdp_draw_line_clear( line ) ;
	}

}


//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
