//////////////////////////////////////////////////////
/// vdp.cpp
//////////////////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "md.h"
#ifdef _DEBUG
#ifndef _NO_USE_MFC
#include "../fmtest/stdafx.h"
#endif //
//#define DEBUG_VDP
#endif //
//////////////////////////////////////////////////////
#ifdef WIN32
#define  sceDmacMemcpy  memcpy
#else //WIN32
extern "C" int sceDmacMemcpy(void *pDst, const void *pSrc, unsigned int uiSize);
#endif //WIN32
//////////////////////////////////////////////////////
#define  DMA_BYTES_MEM_A   ( 18)
#define  DMA_BYTES_MEM_V   (205)
#define  DMA_BYTES_FILL_A  ( 17)
#define  DMA_BYTES_FILL_V  (204)
#define  DMA_BYTES_COPY_A  (  9)
#define  DMA_BYTES_COPY_V  (102)
//////////////////////////////////////////////////////
#define dma_len()  ((vdp_reg[0x14]<<8) | vdp_reg[0x13])
#define dma_addr() ( ((vdp_reg[0x17]&0x7f)<<17) | (vdp_reg[0x16]<<9) | (vdp_reg[0x15]<<1) )
#define MASK_VRAM  ((unsigned short)(0xFFFF))
#define MASK_CRAM  ((unsigned short)(0x007F))
#define MASK_VSRAM ((unsigned short)(0x007F))
//////////////////////////////////////////////////////
#define  _USE_REVERS_ENDIAN
//////////////////////////////////////////////////////

unsigned char md_core::vdp_read_byte()
{
	unsigned char result;

	switch( vdp_mode )
	{
#ifdef _USE_REVERS_ENDIAN
	case 0x00: result = vdp_vram [ (vdp_addr ^ 1) & MASK_VRAM  ] ; break ;
	case 0x20: result = vdp_cram [ (vdp_addr ^ 1) & MASK_CRAM  ] ; break ;
	case 0x10: result = vdp_vsram[ (vdp_addr ^ 1) & MASK_VSRAM ] ; break ;
#else // _USE_REVERS_ENDIAN
	case 0x00: result = vdp_vram [ vdp_addr & MASK_VRAM  ] ; break ;
	case 0x20: result = vdp_cram [ vdp_addr & MASK_CRAM  ] ; break ;
	case 0x10: result = vdp_vsram[ vdp_addr & MASK_VSRAM ] ; break ;
#endif // _USE_REVERS_ENDIAN
	default  : result = 0                              ; break ;
	}


	vdp_addr += vdp_reg[15];
	vdp_pending = 0 ;

#ifdef DEBUG_VDP
		TRACE( "VDP_READ_B[ %04X ] = %02X, %d, %d\n", vdp_addr, result, result, line_count );
#endif // _DEBUG_VDP

	return result ;
}

unsigned short md_core::vdp_read_word()
{
	unsigned short result;

	switch( vdp_mode )
	{
#ifdef _USE_REVERS_ENDIAN
	case 0x00: result =(vdp_vram [ vdp_addr & MASK_VRAM  ]) | (vdp_vram [(vdp_addr+1) & MASK_VRAM  ]<<8); break;
	case 0x20: result =(vdp_cram [ vdp_addr & MASK_CRAM  ]) | (vdp_cram [(vdp_addr+1) & MASK_CRAM  ]<<8); break;
	case 0x10: result =(vdp_vsram[ vdp_addr & MASK_VSRAM ]) | (vdp_vsram[(vdp_addr+1) & MASK_VSRAM ]<<8); break;
#else // _USE_REVERS_ENDIAN
	case 0x00: result =(vdp_vram [ vdp_addr & MASK_VRAM  ]<<8) | vdp_vram [(vdp_addr+1) & MASK_VRAM  ]; break;
	case 0x20: result =(vdp_cram [ vdp_addr & MASK_CRAM  ]<<8) | vdp_cram [(vdp_addr+1) & MASK_CRAM  ]; break;
	case 0x10: result =(vdp_vsram[ vdp_addr & MASK_VSRAM ]<<8) | vdp_vsram[(vdp_addr+1) & MASK_VSRAM ]; break;
#endif // _USE_REVERS_ENDIAN
	default  : result = 0 ; break ;
	}

	
	vdp_addr += vdp_reg[15] ;
	vdp_pending = 0 ;

#ifdef DEBUG_VDP
		TRACE( "VDP_READ_W[ %04X ] = %04X, %d, %d\n", vdp_addr, (unsigned short)result, (unsigned short)result, line_count );
#endif // _DEBUG_VDP

	return result ;
}

unsigned long md_core::vdp_read_long()
{
	unsigned long result;

	switch( vdp_mode )
	{
#ifdef _USE_REVERS_ENDIAN
	case 0x00: 
		result = (vdp_vram [ vdp_addr                ] << 16)
			   | (vdp_vram [(vdp_addr+1) & MASK_VRAM ] << 24) ; 

		vdp_addr += vdp_reg[15] ;

		result |= (vdp_vram [ vdp_addr                ]      )
			   |  (vdp_vram [(vdp_addr+1) & MASK_VRAM ] << 8 ) ; 
		break;

	case 0x20: 
		result = (vdp_cram [ vdp_addr    & MASK_CRAM ] << 16)
			   | (vdp_cram [(vdp_addr+1) & MASK_CRAM ] << 24) ; 

		vdp_addr += vdp_reg[15] ;

		result |= (vdp_cram [ vdp_addr    & MASK_CRAM ] << 0)
			   |  (vdp_cram [(vdp_addr+1) & MASK_CRAM ] << 8) ; 
		break;

	case 0x10: 
		result = (vdp_vsram [ vdp_addr    & MASK_VSRAM ] << 16)
			   | (vdp_vsram [(vdp_addr+1) & MASK_VSRAM ] << 24) ; 

		vdp_addr += vdp_reg[15] ;

		result |= (vdp_vsram [ vdp_addr    & MASK_VSRAM ] << 0)
			   |  (vdp_vsram [(vdp_addr+1) & MASK_VSRAM ] << 8) ; 
		break;

#else // _USE_REVERS_ENDIAN
	case 0x00: 
		result = (vdp_vram [ vdp_addr                ] << 24)
			   | (vdp_vram [(vdp_addr+1) & MASK_VRAM ] << 16) ; 

		vdp_addr += vdp_reg[15] ;

		result |= (vdp_vram [ vdp_addr                ] << 8)
			   |  (vdp_vram [(vdp_addr+1) & MASK_VRAM ]     ) ; 
		break;

	case 0x20: 
		result = (vdp_cram [ vdp_addr    & MASK_CRAM ] << 24)
			   |  vdp_cram [(vdp_addr+1) & MASK_CRAM ] << 16) ; 

		vdp_addr += vdp_reg[15] ;

		result |= (vdp_cram [ vdp_addr    & MASK_CRAM ] << 8)
			   |  (vdp_cram [(vdp_addr+1) & MASK_CRAM ]     ) ; 
		break;

	case 0x10: 
		result = (vdp_vsram [ vdp_addr    & MASK_VSRAM ] << 24)
			   | (vdp_vsram [(vdp_addr+1) & MASK_VSRAM ] << 16) ; 

		vdp_addr += vdp_reg[15] ;

		result |= (vdp_vsram [ vdp_addr    & MASK_VSRAM ] << 8)
			   |  (vdp_vsram [(vdp_addr+1) & MASK_VSRAM ]     ) ; 
		break;

#endif // _USE_REVERS_ENDIAN

	default  : result = 0 ; break ;
	}

	
	vdp_addr += vdp_reg[15] ;
	vdp_pending = 0 ;

#ifdef DEBUG_VDP
		TRACE( "VDP_READ_L[ %04X ] = %08X, %d, %d\n", vdp_addr, (unsigned long)result, (unsigned long)result, line_count );
#endif // _DEBUG_VDP

	return result ;
}

void md_core::vdp_write_word ( unsigned short d )
{
#ifdef DEBUG_VDP
		TRACE( "VDP_WRITE[ %04X ] = %02X, %04X, %d, %d\n", vdp_addr, vdp_mode, (unsigned short)d, (unsigned short)d, line_count );
#endif // _DEBUG_VDP

#ifdef _USE_REVERS_ENDIAN
	if( (vdp_addr & 1) == 1 ){ d = (d >> 8) | (d << 8) ; }
#else // _USE_REVERS_ENDIAN
	if( (vdp_addr & 1) == 0 ){ d = (d >> 8) | (d << 8) ; }
#endif // _USE_REVERS_ENDIAN

	switch( vdp_mode )
	{
	//case 0x00 :
	case 0x04 : 
				*((unsigned short*)&vdp_vram [ vdp_addr & (MASK_VRAM-1) ]) = d ; 
				dirt_flag |= DIRT_VRAM ; 
				break;

	case 0x0c : 
				*((unsigned short*)&vdp_cram [ vdp_addr & (MASK_CRAM-1) ]) = d ; 
				dirt_flag |= DIRT_CRAM ; 
				break;

	case 0x14 : 
				*((unsigned short*)&vdp_vsram [ vdp_addr & (MASK_VSRAM-1) ]) = d ; 
				dirt_flag |= DIRT_VSRAM ; 
				break;
	}
	vdp_addr += vdp_reg[15] ;
	vdp_pending = 0 ;

	if( vdp_dma )
	{
		vdp_dma = 0 ;

		if( ((vdp_reg[0x17]>>6)&3) == 2 )
		{
			unsigned short i, len ;

			len = dma_len();

#ifdef _USE_REVERS_ENDIAN
#else // _USE_REVERS_ENDIAN
			d = d >> 8 ;
#endif // _USE_REVERS_ENDIAN
			for( i=0; i < len ; i++ )
			{

#ifdef _USE_REVERS_ENDIAN
				vdp_vram[ (vdp_addr    ) & MASK_VRAM ] = (unsigned char)d ; 
#else // _USE_REVERS_ENDIAN
				vdp_vram[ (vdp_addr ^ 1) & MASK_VRAM ] = (unsigned char)d ; 
#endif // _USE_REVERS_ENDIAN

				vdp_addr += vdp_reg[15] ;
			}

			dirt_flag |= DIRT_VRAM ;
		}
	}
}

void md_core::vdp_write_long ( unsigned long d )
{
	vdp_write_word( (unsigned short)(d >> 16   ) ) ;
	vdp_write_word( (unsigned short)(d & 0xFFFF) ) ;
}

void md_core::vdp_dma_copy( unsigned char* src, int len )
{
	//*
	int reg15 ;
	unsigned char* dst ;

	reg15 = vdp_reg[15] ;

	switch( vdp_mode )
	{
	//case 0x00 :
	case 0x04 : 
		dst = &vdp_vram [ vdp_addr & MASK_VRAM  ] ; 
		dirt_flag |= DIRT_VRAM  ; 
		if( (vdp_addr + reg15 * len) > ADDR_MASK_VRAM ){ len = (ADDR_MASK_VRAM - vdp_addr +1) / reg15 ; }
		break ;
	case 0x0c : 
		dst = &vdp_cram [ vdp_addr & MASK_CRAM  ] ; 
		dirt_flag |= DIRT_CRAM  ; 
		if( (vdp_addr + reg15 * len) > ADDR_MASK_CRAM ){ len = (ADDR_MASK_CRAM - vdp_addr +1) / reg15 ; }
		break ;
	case 0x14 : 
		dst = &vdp_vsram[ vdp_addr & MASK_VSRAM ] ; 
		dirt_flag |= DIRT_VSRAM ; 
		if( (vdp_addr + reg15 * len) > ADDR_MASK_VSRAM ){ len = (ADDR_MASK_VSRAM - vdp_addr +1) / reg15 ; }
		break ;
	default : vdp_addr += reg15 * len ; return ;
	}

#ifdef DEBUG_VDP
		TRACE( "VDP_DMA[ %04X, %02X ] = %04X, %08X, %d\n", vdp_addr, vdp_mode, (unsigned long)src, len, line_count );
#endif // _DEBUG_VDP

	//*
#ifdef _USE_REVERS_ENDIAN
	//if( vdp_mode == 0x04)
	{
		if( reg15 == 2 )
		{
			vdp_addr += reg15 * len ;

			if( (unsigned long)dst & 1 ){ dst++ ; } //return ; } // alignment check

			//*
			for( ; len > 0 ; len-- ) //n=0; n < len ; n++ )
			{
				*((unsigned short*) dst ) = *((unsigned short*) src ) ; 
				dst += 2 ;
				src += 2 ;
			}
			//*/
			//sceDmacMemcpy( dst, src, len * 2 ) ;
		}
		else
		if( reg15 == 1 )
		{
			vdp_addr += reg15 * len ;

			for( ; len > 0 ; len-- ) //n=0; n < len ; n++ )
			{
				*dst = *src ; 
				dst ++      ;
				src += 2    ;
			}
		}
		else
		//*/
		{
			vdp_addr += reg15 * len ;

			if( (unsigned long)dst & 1 ){ dst++ ; } //return ; } // alignment check

			for( ; len > 0 ; len-- ) //n=0; n < len ; n++ )
			{
				//*((unsigned short*)dst) = get_word( src ) ; 
				*((unsigned short*) dst ) = *((unsigned short*) src ) ; 
				dst += reg15 ;
				src += 2     ;
			}
		}
		return ;
	}
//#endif // _USE_REVERS_ENDIAN
#else // _USE_REVERS_ENDIAN
	if( reg15 == 2 )
	{
		vdp_addr += reg15 * len ;

		if( (unsigned long)dst & 1 ){ dst++ ; } //return ; } // alignment check

		for( ; len > 0 ; len-- ) //n=0; n < len ; n++ )
		{
			//*((unsigned short*) dst   ) = get_word( src     ) ; 
			dst[0] = src[1] ;
			dst[1] = src[0] ;
			dst += 2 ;
			src += 2 ;
		}
	}
	else
	if( reg15 == 1 )
	{
		vdp_addr += reg15 * len ;

		for( ; len > 0 ; len-- ) //n=0; n < len ; n++ )
		{
			*dst = *src ; 
			dst ++      ;
			src += 2    ;
		}
	}
	else
	//*/
	{
		vdp_addr += reg15 * len ;

		if( (unsigned long)dst & 1 ){ dst++ ; } //return ; } // alignment check

		for( ; len > 0 ; len-- ) //n=0; n < len ; n++ )
		{
			//*((unsigned short*)dst) = get_word( src ) ; 
			dst[0] = src[1] ;
			dst[1] = src[0] ;
			dst += reg15 ;
			src += 2     ;
		}
	}
#endif // _USE_REVERS_ENDIAN
}

//unsigned short vdp_addr_old       ;

void md_core::vdp_command ( unsigned short d )
{
	if( vdp_pending )
	{
		vdp_pending  = 0 ; 
		vdp_cmd2     = d ; 

		// mode writes: 04=VRAM 0C=CRAM 14=VSRAM
		// mode reads:  00=VRAM 20=CRAM 10=VSRAM
		vdp_dma  = ( vdp_cmd2 & 0x0080 ) && (vdp_reg[1] & 0x10) ;
		vdp_mode = ( vdp_cmd2 & 0x0030 ) | ((vdp_cmd1 & 0xC000) >> 12) ;
		vdp_addr = ( vdp_cmd1 & 0x3FFF ) | ( vdp_cmd2 << 14 ) ;
		//vdp_addr_old = vdp_addr & 0xC000 ;

#ifdef DEBUG_VDP
		if( vdp_addr == 0xBC00 )
		{
			//ASSERT(FALSE);
			vdp_addr = 0xBC00 ;
		}
		TRACE( "CMD = %02X, %d, %04X, %02X\n", vdp_mode, vdp_dma, vdp_addr, (vdp_reg[0x17]>>6)&3 );

		//if( dma_len () > 0x200 )
		{
		//	TRACE("!!!\n");
		}
#endif // _DEBUG_VDP

		if( vdp_dma )
		{
			int i, s, len   ;

			switch ( (vdp_reg[0x17]>>6)&3 )
			{
			case 0: case 1:
				s   = dma_addr() ; 
				len = dma_len () ;

				if( len == 0 ){ return ; } //len = 0x10000 ; }

				if( s > 0x00dfffff )
				{
					vdp_dma_copy( &core_ram[s&0xffff], len ) ;
				}
				else if( s < MAX_ROM_SIZE )
				{
					vdp_dma_copy( &rom_data[s], len ) ;
				}
				else
				{
				}
				//dma_use_clock( len, vblank_on ? DMA_BYTES_MEM_V : DMA_BYTES_MEM_A ) ;
				//reg[19] = reg[20] = 0 ;
				//rw_dma = 0 ;
				break;

			case 2:
				// Done later on
				break;

			case 3:
				s   = ((vdp_reg[0x16] << 8) | vdp_reg[0x15]) ;

				len = dma_len () ;

				if( len == 0 ){ len = 0x10000 ; }

				for( i=0; i < len ; i++ )
				{
					vdp_vram[ vdp_addr & MASK_VRAM ] = vdp_vram[ s & MASK_VRAM ] ;
					vdp_addr += vdp_reg[15];
					s++;
				}

				dirt_flag |= DIRT_VRAM ;
				//*/
				//reg[19] = reg[20] = 0 ;
				//rw_dma = 0 ;
				break;
			}
		}
	}
	else if( (d & 0xC000) == 0x8000 )
	{
		unsigned char addr ; 
		
		addr  = (d >> 8) & 0x1F ; 
		//d     = d & 0xFF ;

		//if (vdp_reg[addr] != d ) 
		{
			/*
			if( (addr == 0) && ((vdp_reg[0] & 0x10) != (d & 0x10)) )
			{
				addr = 0 ;
			}
			*/

			vdp_reg[addr]  = (unsigned char)d ;
			dirt_flag     |= DIRT_REG ;
		}

#ifdef DEBUG_VDP
		TRACE( "VDP_REG[ %2d ] = %02X, %d, %d\n", addr, (unsigned char)d, (unsigned char)d, line_count );
#endif // _DEBUG_VDP

		//if( (addr == 12) || (addr == 16) || (addr == 17) )
		{
		//	dirt_flag |= DIRT_REG_DRAW ;
		}

		if( (addr == 0) || (addr == 1) )
		{ 
#if _USE_68K == 3 ///
			if( cpu_emu == 0 )
			{
#endif ///_USE_68K == 3

				do_irq1(1) ; 

#if _USE_68K == 3 ///
			}
			else
			{
				do_irq2(1) ; 
			}
#endif ///_USE_68K == 3

		}

		vdp_mode = 0 ;
        //vdp_addr = ((vdp_addr_old & 0xC000) | (d & 0x3FFF)) & 0xFFFF;
		//vdp_mode = vdp_mode | ((d >> 14) & 0x03) ;
		vdp_dma  = 0 ;
	}
	else
	{
		vdp_cmd1    = d ; 
		vdp_pending = 1 ;
	}
}

void md_core::vdp_command_l ( unsigned long  d )
{
	vdp_command( (unsigned short)(d >> 16   ) ) ;
	vdp_command( (unsigned short)(d & 0xFFFF) ) ;
}

//////////////////////////////////////////////////////
