//////////////////////////////////////////////////////
/// md.cpp
//////////////////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//////////////////////////////////////////////////////
#include "md.h"
//#include "cz80/cz80.c"
//#include "ras.cpp"
//#include "vdp.cpp"
#ifdef _DEBUG
#ifndef _NO_USE_MFC
#include "../fmtest/stdafx.h"
//#define DEBUG_MAIN 
#define _DEBUG_MEM
#define _DEBUG_MEM2
#ifdef _DEBUG_MEM
LARGE_INTEGER  li1 , li2 ;
int debug_mem_r  = 0 ;
int debug_mem_w  = 0 ;
unsigned int debug_bp     = 0 ;
unsigned int debug_bp_m   = 0 ;
#endif // _NO_USE_MFC
#endif 
#endif // _DEBUG
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
#define LINES_PER_FRAME_NTSC 0x106 // Number of scanlines in NTSC (w/ vblank)
#define LINES_PER_FRAME_PAL  0x138 // Number of scanlines in PAL  (w/ vblank)
#define LINES_PER_FRAME (vdp_pal? LINES_PER_FRAME_PAL : LINES_PER_FRAME_NTSC)
#define VBLANK_LINE_NTSC 0xE0 // (224) vblank location for NTSC (and PAL 28-cel mode)
#define VBLANK_LINE_PAL  0xF0 // (240) vblank location for PAL 30-cel mode

#define VBLANK_LINE ((vdp_pal && (vdp_reg[1] & 8)) ? VBLANK_LINE_PAL : VBLANK_LINE_NTSC)

#define _scanlength (vdp_pal ? ((7189547/50/0x138)) : (      487)) //(8000000/60/0x106))
#define _scanlen_f  (vdp_pal ? ((7189547/50      )) : (487*0x106)) //(8000000/60/0x106))
//#define _scanlength (vdp_pal ? ((7189547/50/0x138)) : (      455)) //(8000000/60/0x106))
//#define _scanlen_f  (vdp_pal ? ((7189547/50      )) : (455*0x106)) //(8000000/60/0x106))
//////////////////////////////////////////////////////
#define PAD_TIMEOUT  (23)
#define PAD_TIMER(a) if ( pad_six_tm[a] > PAD_TIMEOUT ){ pad_six_ct[a] = 0 ; }else{ pad_six_tm[a]++ ; }
#define PAD_TIMERS() {PAD_TIMER(0);PAD_TIMER(1);}
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
#define ZeroMem(a)  memset(a,0,sizeof(a))
//////////////////////////////////////////////////////
static md_core*       mdcore_ptr ;
static unsigned char* mdcore_ram ;
static unsigned char* mdcore_rom ;
#ifdef _USE_BANK_CANGE
static unsigned char* mdcore_rom_b ;
#endif // _USE_BANK_CANGE
//////////////////////////////////////////////////////
#ifdef WIN32
void sceKernelDelayThread(unsigned long delay){;}
#else // WIN32
extern "C" 
{
//extern unsigned long  psp_sound_thid    ;
extern void sceKernelDelayThread(unsigned long delay);
}
#endif //WIN32
//////////////////////////////////////////////////////
md_core::md_core()
{
}

md_core::~md_core()
{
}

//////////////////////////////////////////////////////
// for PSP Sub CPU
//////////////////////////////////////////////////////
void Start_Sub() ;
void Wait_Sub () ;


//////////////////////////////////////////////////////
// for Core
//////////////////////////////////////////////////////
void md_core::init( void )
{
	MD_DATA* p = (MD_DATA*)this ;
	memset( p, 0, sizeof(MD_DATA) ) ;

	mdcore_ptr = this ;
	mdcore_ram = mdcore_ptr->core_ram ;

#ifdef WIN32
	mdcore_rom = mdcore_ptr->rom_data ;
#else
	//mdcore_rom = (unsigned char*)( (unsigned long)mdcore_ptr->rom_data | 0x40000000 ) ; // uncached
	mdcore_rom = mdcore_ptr->rom_data ;
#endif

#ifdef MEMORY_EXT
	read_byte  = m68k_read_memory_8   ;
	read_word  = m68k_read_memory_16  ;
	read_long  = m68k_read_memory_32  ;
	write_byte = m68k_write_memory_8  ;
	write_word = m68k_write_memory_16 ;
	write_long = m68k_write_memory_32 ;
#endif // MEMORY_EXT

	draw_bpp = 2 ;

	z80_init() ;
}

void md_core::reset( void )
{
	int n ;

	ZeroMem( core_ram  ) ;
	ZeroMem( z80_ram   ) ;

#ifdef _USE_68K

	if( (core_ram != NULL) && (rom_data != NULL) )
	{ 
		_68K_INIT ( rom_data, core_ram ) ;
		_68K_RESET(); 
	}

#endif // _USE_68K

#ifdef _USE_BANK_CANGE
	mdcore_rom_b = &mdcore_rom[ MAX_ROM_BANKED ] ;
#endif // _USE_BANK_CANGE

	Z80_RESET();

	ZeroMem( vdp_vram  ) ;
	ZeroMem( vdp_vsram ) ;
	ZeroMem( vdp_cram  ) ;
	ZeroMem( vdp_reg   ) ;

	//draw_cashe     [ MAX_DRAW_CASHE ] ;
	//palette        [ MAX_PALETTE   ] ;

	//sprite_order_0 [ MAX_SPRITES+1 ] ;
	//sprite_order_1 [ MAX_SPRITES+1 ] ;
	sprite_count_0 = 0 ;
	sprite_count_1 = 0 ;

	vdp_cmd1       = 0 ;
	vdp_cmd2       = 0 ;
	vdp_addr       = 0 ;
	vdp_write      = vdp_dmy_buff ;
	vdp_read       = vdp_dmy_buff ;
	vdp_addr_mask  = 0            ;
	dirt_flag      = DIRT_ALL     ;
	dirt_bit       = 0            ;
	ZeroMem( vdp_dmy_buff )    ;

	vdp_mode       = 0 ;
	vdp_dma        = 0 ;
	vdp_pending    = 0 ;
	vdp_pal        = 0 ;

	vdp_status1    = 0 ; // coo4
	vdp_status2    = 0 ; // coo5
	
	//cpu_emu        ;
	line_count     = 0 ;
	clk_sys        = 0 ;
	clk_68k        = 0 ;
	clk_z80        = 0 ;
	clk_line       = 0 ;
	palette_mode   = 1 ;
	irq_req        = 0 ;
	z80_irq        = 0 ;
	z80_in         = 0 ;

	for( n=0; n < MAX_PAD ; n++ )
	{
		pad            [ n ] = 0xf303f ;
		pad_toggle     [ n ] = 0 ;
		pad_six_tm     [ n ] = 0 ;
		pad_six_ct     [ n ] = 0 ;
	}
	//pad_use_six    = 0 ;
	
	/*
	dac_data       [ MAX_DAC_BANK+1 ][ MAX_DAC_DATA ] ;
	dac_last       ;
	dac_frame      ;
	dac_play       ;
	dac_enabled    ;
	dac_rep        ;
	*/
	dac_init() ;

	ZeroMem( fm_sel        ) ;
	ZeroMem( fm_status     ) ;
	ZeroMem( fm_timer      ) ;
	ZeroMem( fm_reg        ) ;

	//sound_flag     ;

	//z80            ;
	z80_bank68k    = 0 ; // 9 bits
	z80_online     = 0 ;
	z80_reset      = 0 ;

	//rom_len        ;
	//rom_data       [ MAX_ROM_SIZE ] ;
	//save_ram       [ MAX_SAVE_RAM ] ;
	//save_start     = 0 ;
	//save_end       = 0 ;
	//save_active    = 0 ;
	//save_prot      = 0 ;
	save_update    = 0 ;
	country_ver    = 0x0FF0 ;

	SN76496_reset   () ;
	YM2612ResetChip () ;
}


int md_core::change_cpu_emu(int to)
{
	if( cpu_emu != to )
	{

#if _USE_68K == 3

		M68K_STATE state ; memset( &state, 0, sizeof(M68K_STATE) ) ;
		//M68K_STATE state2 ;

		if( to == 1 )
		{
			_68K_INIT2( rom_data, core_ram ) ;
			_68K_GET_STATE1( &state ) ;

			//C68k_InitTable(1) ;

			//state.pc = state.pc & 0x00FFFFFF ;
			//TRACE( "PC = %08X\n", state.pc ) ;

			_68K_SET_STATE2( &state ) ;
			//_68K_GET_STATE2( &state2 ) ;

		}
		else
		{
			_68K_INIT1( rom_data, core_ram ) ;
			_68K_GET_STATE2( &state ) ;

			//m68ki_build_opcode_table() ;

			_68K_SET_STATE1( &state ) ;
			//_68K_GET_STATE1( &state2 ) ;
		}

#endif //

		cpu_emu=to;
		return 0;
	}

	return 0 ;
}

void md_core::do_irq1( short b )
{
	if(      (vdp_reg[1] & 0x20) && (irq_req & 0x08) ){ _68K_SET_IRQ(6); }
	else if( (vdp_reg[0] & 0x10) && (irq_req & 0x04) ){ _68K_SET_IRQ(4); }
	else if(b){ _68K_CLEAR_IRQ(0) ; }
}

void md_core::do_irq2( short b )
{
#if _USE_68K == 3 ///
	if(      (vdp_reg[1] & 0x20) && (irq_req & 0x08) ){ _68K_SET_IRQ2(6); }
	else if( (vdp_reg[0] & 0x10) && (irq_req & 0x04) ){ _68K_SET_IRQ2(4); }
	else if( b ){ _68K_CLEAR_IRQ2(0) ; }
#endif ///_USE_68K == 3
}

extern "C"
{

int vdp_int_ack_callback(int int_level)
{
	/*
	if(      (mdcore_ptr->vdp_reg[1] & 0x20) && (mdcore_ptr->irq_req & 0x08) )
	{ 
		mdcore_ptr->irq_req  =     0 ; 
	}
	else if( (mdcore_ptr->vdp_reg[0] & 0x10)                                 )
	{ 
		mdcore_ptr->irq_req &= ~0x08 ;
	}
	*/

	switch( int_level )
	{
	case 4 :
		mdcore_ptr->irq_req &= ~0x04 ;
		break ;

	case 6 :
		mdcore_ptr->vdp_status2 &= ~0x80 ;
		mdcore_ptr->irq_req     &= ~0x08 ;
		break ;
	}

	//if(      int_level == 4 ){ irq_req &= ~0x04 ; }
	//else if( int_level == 6 ){ irq_req &= ~0x08 ; }
	return -1 ;//M68K_INT_ACK_AUTOVECTOR;
}

} // extern "C"


void md_core::do_frame ( unsigned char skip )
{
	short hints, hint0 ;
	unsigned short vline ;
	int hblank1, hblank2, scanlength1 ;

//	vdp_update_palette() ;

	//odo = odom = odoz = 0 ;
	z80_irq = 1 ;
	Z80_CLEAR_CLOCK();
	//dac_clear();

	vdp_status1 = 0x37; // Init status register

	if(  vdp_reg[12] & 0x02 ) vdp_status2 ^=  0x10 ; // Toggle odd/even for interlace
	if(  vdp_reg[ 1] & 0x40 ) vdp_status2 &= ~0x88 ; // Clear vblank and vint
	if(!(vdp_reg[ 1] & 0x20)) vdp_status2 |=  0x80 ; // If vint disabled, vint happened

	hints       = vdp_reg[10]        ; // Set hint counter
	vline       = VBLANK_LINE        ;
	clk_line    = _scanlength        ;
	hblank1     = clk_line - 404     ; //(scanlength * 36/209)      ;
	hblank2     = clk_line - 360     ; //(scanlength * 36/209) +44  ;

	//clk_line     = _scanlength  *2    ;
	//hblank1     = (clk_line * 36/209)      ;
	//hblank2     = (clk_line * 36/209) +44  ;

	scanlength1 = clk_line - hblank1 ;
	hint0 = hints ;

#if _USE_68K == 3 ///
	if( cpu_emu == 0 )
	{
#endif ///_USE_68K == 3

		for( line_count = 0; line_count < vline ; line_count++ )
		{
			//fm_timer_update2() ;
			PAD_TIMERS() ;

			{
				vdp_status2 |= 4 ;
				clk_68k += hblank1 ;
				clk_sys += _68K_EXEC( clk_68k - clk_sys ) ;
				vdp_status2 &= ~4 ;
			}

			{
				if( (--hints < 0) )
				{
					hints    = vdp_reg[10] ; 
					irq_req |= 0x04 ;
					do_irq1(0) ;
				}
			}

			if( !skip ){ vdp_update_line( line_count ) ; }

			{
				clk_68k += scanlength1 ;
				clk_sys += _68K_EXEC( clk_68k - clk_sys ) ;
			}
		}

		if( --hints < 0 )
		{
			irq_req |= 0x04 ;
			do_irq1(0) ;
		}

		//fm_timer_update2();

		vdp_status2 |= 0x0C ;
		clk_68k += hblank2 ;
		clk_sys += _68K_EXEC( clk_68k - clk_sys );
		//z80_do_clock( clk_sys );
		vdp_status2 &= ~4;
		vdp_status2 |= 0x80 ;
		irq_req |= 0x08 ;
		do_irq1(0) ;

		//if( z80_online ) //&& z80_reset)
		{
			//Z80_IRQ(0); 
		}

		clk_68k += clk_line - hblank2 ;
		clk_sys += _68K_EXEC( clk_68k - clk_sys );
		//z80_do_clock( clk_sys );
		line_count++ ;

		for( ; line_count < LINES_PER_FRAME; line_count++ )
		{
			//fm_timer_update2();
			PAD_TIMERS() ;

			vdp_status2 |= 4; 
			clk_68k += hblank1 ;
			clk_sys += _68K_EXEC( clk_68k - clk_sys );
			vdp_status2 &= ~4;
			clk_68k += scanlength1  ;
			clk_sys += _68K_EXEC( clk_68k - clk_sys );
			//z80_do_clock( clk_sys ) ;
		}

#if _USE_68K == 3 ///
	}
	else
	{
		for( line_count = 0; line_count < vline ; line_count++ )
		{
			//fm_timer_update2() ;
			PAD_TIMERS() ;

			{
				vdp_status2 |= 4 ;
				clk_68k += hblank1 ;
				clk_sys += _68K_EXEC2( clk_68k - clk_sys ) ;
				vdp_status2 &= ~4 ;
			}

			{
				if( (--hints < 0) )
				{
					hints    = vdp_reg[10] ; 
					irq_req |= 0x04 ;
					do_irq2(0) ;
				}
			}

			if( !skip ){ vdp_update_line( line_count ) ; }

			{
				clk_68k += scanlength1 ;
				clk_sys += _68K_EXEC2( clk_68k - clk_sys ) ;
			}
		}

		if( --hints < 0 )
		{
			irq_req |= 0x04 ;
			do_irq2(0) ;
		}

		//fm_timer_update2();

		vdp_status2 |= 0x0C ;
		clk_68k += hblank2 ;
		clk_sys += _68K_EXEC2( clk_68k - clk_sys );
		//z80_do_clock( clk_sys );
		vdp_status2 &= ~4;
		vdp_status2 |= 0x80 ;
		irq_req |= 0x08 ;
		do_irq2(0) ;

		//if( z80_online ) //&& z80_reset)
		{
			//Z80_IRQ(0); 
		}

		clk_68k += clk_line - hblank2 ;
		clk_sys += _68K_EXEC2( clk_68k - clk_sys );
		//z80_do_clock( clk_sys );
		line_count++ ;

		for( ; line_count < LINES_PER_FRAME; line_count++ )
		{
			//fm_timer_update2();
			PAD_TIMERS() ;

			vdp_status2 |= 4; 
			clk_68k += hblank1 ;
			clk_sys += _68K_EXEC2( clk_68k - clk_sys );
			vdp_status2 &= ~4;
			clk_68k += scanlength1  ;
			clk_sys += _68K_EXEC2( clk_68k - clk_sys );
			//z80_do_clock( clk_sys ) ;
		}

	}
#endif ///_USE_68K == 3 //////////////////////

	{
		//Z80_IRQ(0); 
		z80_do_sync( clk_sys ) ;
	}

	clk_68k = clk_z80 = 0 ;
	//clk_sys -= ( _scanlen_f ) ;
	clk_sys = 0 ;

	//TRACE( "%d\n", clk_sys );

	dac_frame ++ ;

//	vdp_draw_interace( vline ) ;
}

void md_core::do_frame_68k ( unsigned char skip )
{
	short hints, hint0 ;
	unsigned short vline, dline = 0 ;
	int hblank1, hblank2, scanlength1 ;

//	vdp_update_palette() ;

	//odo = odom = odoz = 0 ;
	//Z80_CLEAR_CLOCK();
	//dac_clear();

	vdp_status1 = 0x37; // Init status register

	if(  vdp_reg[12] & 0x02 ) vdp_status2 ^=  0x10 ; // Toggle odd/even for interlace
	if(  vdp_reg[ 1] & 0x40 ) vdp_status2 &= ~0x88 ; // Clear vblank and vint
	if(!(vdp_reg[ 1] & 0x20)) vdp_status2 |=  0x80 ; // If vint disabled, vint happened

	hints       = vdp_reg[10]        ; // Set hint counter
	vline       = VBLANK_LINE        ;
	clk_line    = _scanlength        ;
	hblank1     = clk_line - 404     ; //(scanlength * 36/209)      ;
	hblank2     = clk_line - 360     ; //(scanlength * 36/209) +44  ;

	//clk_line     = _scanlength  *2    ;
	//hblank1     = (clk_line * 36/209)      ;
	//hblank2     = (clk_line * 36/209) +44  ;

	scanlength1 = clk_line - hblank1 ;
	hint0 = hints ;

#if _USE_68K == 3 ///
	if( cpu_emu == 0 )
	{
#endif ///_USE_68K == 3

		for( line_count = 0; line_count < vline ; line_count++ )
		{
			//fm_timer_update2();
			PAD_TIMERS() ;

			{
				vdp_status2 |= 4;
				clk_68k += hblank1 ;
				clk_sys += _68K_EXEC( clk_68k - clk_sys );
				vdp_status2 &= ~4;
			}

			if( (--hints < 0) )
			{
				hints    = vdp_reg[10]; 
				irq_req |= 0x04 ;
				do_irq1(0) ;
			} 

			if( !skip )
			{ 
				vdp_update_line( line_count ) ; 
			}

			{
				clk_68k += scanlength1 ;
				clk_sys += _68K_EXEC( clk_68k - clk_sys );
				//z80_do_clock( clk_sys );// Do Z80
			}
		}

		if( --hints < 0 )
		{
			irq_req |= 0x04 ;
			do_irq1(0) ;
		}

		//fm_timer_update();

		vdp_status2 |= 0x0C ;
		clk_68k += hblank2 ;
		clk_sys += _68K_EXEC( clk_68k - clk_sys );
		//z80_do_clock( clk_sys );
		vdp_status2 &= ~4;
		vdp_status2 |= 0x80 ;

		irq_req |= 0x08 ;
		do_irq1(0) ;

		//if( z80_online ) //&& z80_reset)
		{
			//Z80_IRQ(0); 
		}

		clk_68k += clk_line - hblank2 ;
		clk_sys += _68K_EXEC( clk_68k - clk_sys );
		//z80_do_clock( clk_sys );
		line_count++ ;

		for(; line_count < LINES_PER_FRAME ; line_count++ )
		{
			//fm_timer_update2();
			PAD_TIMERS() ;

			vdp_status2 |= 4; 
			clk_68k += hblank1 ;
			clk_sys += _68K_EXEC( clk_68k - clk_sys );
			vdp_status2 &= ~4;
			clk_68k += scanlength1  ;
			clk_sys += _68K_EXEC( clk_68k - clk_sys );
			//z80_do_clock( clk_sys );
		}

#if _USE_68K == 3 ///
	}
	else
	{
		for( line_count = 0; line_count < vline ; line_count++ )
		{
			//fm_timer_update2();
			PAD_TIMERS() ;

			{
				vdp_status2 |= 4;
				clk_68k += hblank1 ;
				clk_sys += _68K_EXEC2( clk_68k - clk_sys );
				vdp_status2 &= ~4;
			}

			{
				if( (--hints < 0) )
				{
					hints    = vdp_reg[10]; 
					irq_req |= 0x04 ;
					do_irq2(0) ;
				} 
			}

			if( !skip ){ vdp_update_line( line_count ) ; }

			{
				clk_68k += scanlength1 ;
				clk_sys += _68K_EXEC2( clk_68k - clk_sys );
				//z80_do_clock( clk_sys );
			}
		}

		if( --hints < 0 )
		{
			irq_req |= 0x04 ;
			do_irq2(0) ;
		}

		//fm_timer_update();

		vdp_status2 |= 0x0C ;
		clk_68k += hblank2 ;
		clk_sys += _68K_EXEC2( clk_68k - clk_sys );
		//z80_do_clock( clk_sys );
		vdp_status2 &= ~4;
		vdp_status2 |= 0x80 ;
		irq_req |= 0x08 ;
		do_irq2(0) ;

		//if( z80_online ) //&& z80_reset)
		{
			//Z80_IRQ(0); 
		}

		clk_68k += clk_line - hblank2 ;
		clk_sys += _68K_EXEC2( clk_68k - clk_sys );
		//z80_do_clock( clk_sys );
		line_count++ ;

		for( ; line_count < LINES_PER_FRAME ; line_count++ )
		{
			//fm_timer_update2();
			PAD_TIMERS() ;

			vdp_status2 |= 4; 
			clk_68k += hblank1 ;
			clk_sys += _68K_EXEC2( clk_68k - clk_sys );
			vdp_status2 &= ~4;
			clk_68k += scanlength1  ;
			clk_sys += _68K_EXEC2( clk_68k - clk_sys );
			//z80_do_clock( clk_sys ) ;
		}
	}

#endif ///_USE_68K == 3 //////////////////////

	clk_68k = 0 ;
	//clk_sys -= ( _scanlen_f ) ;
	clk_sys = 0 ;

	//TRACE( "%d\n", clk_sys );

//	dac_frame ++ ;

}

void md_core::do_frame_z80 ( )
{
//Z80_CLEAR_CLOCK();
	//if( z80_in ){ return ; }
	while( z80_in ){ sceKernelDelayThread(1000); }

	z80_in = 1 ;
	//z80_in = 0 ;
	int vline   = VBLANK_LINE    ;
	int line    = line_count     ;
	int clk     = (clk_z80 << 1) ;
	int add     = _scanlength    ;
	int do_clk  = _scanlen_f     ;

	line_count = clk / add  ;

	//for(  ; clk < (do_clk - add) ; clk += add )
	for( ; clk < do_clk ; clk += add )
	{
		fm_timer_update();
		if( (line_count == vline) && z80_irq ){ z80_irq = 0 ; Z80_IRQ(0); }
		z80_do_clock( clk ) ;
		line_count++ ;
	}

	//if( clk < do_clk ){ z80_do_clock( do_clk ) ; }

	line_count = line ;
	dac_frame ++ ;
	Z80_CLEAR_CLOCK();
	z80_irq = 1 ;
	z80_in  = 0 ;
}

void md_core::make_sound ( unsigned short* p, short len )
{
	unsigned short sound_buff[ 1024 ] ; 

	if( (sound_flag & 0x40) == 0 )
	{
		dac_make( sound_buff, len ) ;

		for( int i=0 ; i < len  ; i++ )
		{
			//p[i*2  ] = sound_buff[i] ;
			//p[i*2+1] = sound_buff[i] ;
			((unsigned long*)p)[i] = sound_buff[i] | (sound_buff[i] << 16) ;
		}
	}
	else
	{
		for( int i=0 ; i < len ; i++ )
		{
			((unsigned long*)p)[i] = 0 ;
		}
	}

	fm_make ( p, len ) ;

	/*
	int i ;

	if( (sound_flag & 0x40) == 0 )
	{
		// Get the PSG
		SN76496Update_16( 0, sound_buff, len );

		// We bring in the dac, but stretch it out to fit the real length.
		dac_make( sound_buff, len ) ;

		// Copy mono signal to center channel
		for( i=0 ; i < len  ; i++ )
		{
			//((unsigned long*)p)[i] = (((unsigned long)sound_buff[i] << 16) & 0xFFFF0000) | ((unsigned long)sound_buff[i] & 0x0000FFFF) ;
			p[i*2  ] = sound_buff[i] ;
			p[i*2+1] = sound_buff[i] ;
		}
	}
	else
	{
//	  for( i=0 ; i < len /2 ; i++ )
		for( i=0 ; i < len ; i++ )
		{
			((unsigned long*)p)[i] = 0 ;
			//((unsigned long*)sndi->l)[i] = 0 ;
		}
	}

	// Add in the stereo FM buffer
	FMSAMPLE *buf[2];
	buf[0] = (FMSAMPLE*) p ;
	buf[1] = (FMSAMPLE*) p ;

	//if( (snd_mute & 0x3F) == 0 )
	if( (sound_flag & 0x01) == 0 )
	{
		YM2612UpdateOne( buf, len, sound_flag );
	}
	*/

}

typedef struct _MD_CORE_STATE1
{
	unsigned char  core_ram       [ MAX_CORE_RAM  ] ;
	unsigned char  vdp_reg        [ MAX_VDP_REG   ] ;
	unsigned char  vdp_vsram      [ MAX_VDP_VSRAM ] ;
	unsigned char  vdp_cram       [ MAX_VDP_CRAM  ] ;
	unsigned char  vdp_vram       [ MAX_VDP_VRAM  ] ;
	unsigned char  z80_ram        [ MAX_Z80_RAM   ] ;
	Z80_STATE      z80_state      ;
	long           z80_bank68k    ;
	long           z80_online     ;
	long           dac_data       [0x138] ;
	long           dac_enabled    ;
	long           dac_last       ;
	long           vdp_pal        ;
	long           pad_toggle_1   ;
	long           pad_six_ct1    ;
	long           pad_six_tm1    ;
	long           pad_toggle_2   ;
	long           pad_six_ct2    ;
	long           pad_six_tm2    ;
	long           vdp_pending    ;
	unsigned long  vdp_cmd        ;
	long           vdp_mode       ;
	long           vdp_addr       ;
	long           vdp_dma        ;

} MD_CORE_STATE1 ;// __attribute__((aligned(32))) ;

typedef struct _MD_CORE_STATE2
{
	int          fm_sel        [2]        ;
	int          fm_status     [2]        ;
	int          fm_timer      [4]        ;
	signed short fm_reg        [2][0x100] ;

} MD_CORE_STATE2  ; //__attribute__((aligned(32))) ;

inline void set_long( unsigned char* dest, int data )
{
	unsigned char* p = (unsigned char*)&data ;
	dest[0] = p[0] ;
	dest[1] = p[1] ;
	dest[2] = p[2] ;
	dest[3] = p[3] ;
}

inline unsigned long get_long( unsigned char* src )
{
	unsigned long data ;
	unsigned char* p = (unsigned char*)&data ;
	p[0] = src[0] ;
	p[1] = src[1] ;
	p[2] = src[2] ;
	p[3] = src[3] ;
	return data ;
}


int md_core::get_state ( unsigned char* buff )
{
	/*
	int n, ret = 0 ;

	MD_CORE_STATE1  s1a ;
	MD_CORE_STATE2  s2a ;
	MD_CORE_STATE1* s1  ;
	MD_CORE_STATE2* s2  ;

	ret += _68K_GET_STATE( (M68K_STATE*)&buff[ret] ) ;

	s1 = &s1a ; //(MD_CORE_STATE1*)&buff[ret] ;

	memcpy( s1->core_ram   , core_ram , sizeof(core_ram ) ) ;
	memcpy( s1->vdp_reg    , vdp_reg  , sizeof(vdp_reg  ) ) ;
	memcpy( s1->vdp_vsram  , vdp_vsram, 0x50 ) ; //sizeof(vdp_vsram) ) ;
	memcpy( s1->vdp_cram   , vdp_cram , sizeof(vdp_cram ) ) ;
	memcpy( s1->vdp_vram   , vdp_vram , sizeof(vdp_vram ) ) ;
	memcpy( s1->z80_ram    , z80_ram  , sizeof(z80_ram  ) ) ;
	memcpy( &s1->z80_state , &z80     , sizeof(z80      ) ) ;
	s1->z80_bank68k  = z80_bank68k               ;
	s1->z80_online   = z80_online                ;
	//dac_data       [0x138] ;
	s1->dac_enabled  = dac_enabled               ;
	s1->dac_last     = dac_last                  ;
	s1->vdp_pal      = vdp_pal                   ;
	s1->pad_toggle_1 = pad_toggle[0]             ;
	s1->pad_six_ct1  = pad_six_ct[0]             ;
	s1->pad_six_tm1  = pad_six_tm[0]             ;
	s1->pad_toggle_2 = pad_toggle[1]             ;
	s1->pad_six_ct2  = pad_six_ct[1]             ;
	s1->pad_six_tm2  = pad_six_tm[1]             ;
	s1->vdp_pending  = vdp_pending               ;
	s1->vdp_cmd      = vdp_cmd1 << 16 | vdp_cmd2 ;
	s1->vdp_mode     = vdp_mode                  ;
	s1->vdp_addr     = vdp_addr                  ;
	s1->vdp_dma      = vdp_dma                   ;
	
	memcpy( &buff[ret], s1, sizeof(MD_CORE_STATE1) ) ;
	ret += sizeof(MD_CORE_STATE1) ; 

	ret += SN76496_get_state( &buff[ret] ) ;

	s2 = &s2a ; //(MD_CORE_STATE2*)&buff[ret] ;
	
	s2->fm_sel  [0] = fm_sel  [0] ;
	s2->fm_sel  [1] = fm_sel  [1] ;
	s2->fm_tover[0] = fm_tover[0] ;
	s2->fm_tover[0] = fm_tover[1] ;
	for( n=0; n < 4     ; n++ ){ s2->ras_fm_ticker[n] = fm_ras_ticker[n] ; }
	for( n=0; n < 0x100 ; n++ ){ s2->fm_reg[0][n] = fm_reg[0][n] ; s2->fm_reg[1][n] = fm_reg[1][n] ; }

	memcpy( &buff[ret], s2, sizeof(MD_CORE_STATE2) ) ;
	ret += sizeof(MD_CORE_STATE2) ; 

	ret += YM2612GetState( &buff[ret] ) ;

	return ret ;
	*/

	int ret = 0 ;
	Z80_STATE z80st ;

	ret += _68K_GET_STATE( (M68K_STATE*)&buff[ret] ) ;

	memcpy( &buff[ret], core_ram , 0x10000 ) ; ret += 0x10000 ;
	memcpy( &buff[ret], vdp_reg  , 0x00020 ) ; ret += 0x00020 ;
	memcpy( &buff[ret], vdp_vsram, 0x00050 ) ; ret += 0x00050 ;
	memcpy( &buff[ret], vdp_cram , 0x00080 ) ; ret += 0x00080 ;
	memcpy( &buff[ret], vdp_vram , 0x10000 ) ; ret += 0x10000 ;
#ifdef _USE_REVERS_ENDIAN
	{
		unsigned char* p = &buff[ret - (0x10000 + 0x00080 + 0x00050)] ;
		for( int n=0; n < (0x10000 + 0x00080 + 0x00050) ; n+=2 )
		{
			unsigned char tmp = p[n] ;
			p[n  ] = p[n+1] ;
			p[n+1] = tmp ;
		}
	}
#endif // _USE_REVERS_ENDIAN
	
	memcpy( &buff[ret], z80_ram  , 0x02000 ) ; ret += 0x02000 ;

	//memcpy( &buff[ret], &z80 , sizeof(z80) ) ; ret += sizeof(z80) ;
	z80_get_state( &z80st ) ;
	memcpy( &buff[ret], &z80st , sizeof(z80st) ) ; ret += sizeof(z80st) ;
//	set_long( &buff[ret], z80_int_pending  ) ; ret += 4 ;
	set_long( &buff[ret], z80_bank68k      ) ; ret += 4 ;
	set_long( &buff[ret], z80_online       ) ; ret += 4 ;
//	set_long( &buff[ret], z80_extra_cycles ) ; ret += 4 ;

//	memcpy( &buff[ret], dac_data , sizeof(dac_data) ) ; ret += sizeof(dac_data) ;
	ret += sizeof( int[0x138] ) ; // * 0x138 ;
	set_long( &buff[ret], dac_enabled      ) ; ret += 4 ;
	set_long( &buff[ret], dac_last         ) ; ret += 4 ;
	set_long( &buff[ret], vdp_pal          ) ; ret += 4 ;

	set_long( &buff[ret], pad_toggle[0] ) ; ret += 4 ;
	set_long( &buff[ret], pad_six_ct[0] ) ; ret += 4 ;
	set_long( &buff[ret], pad_six_tm[0] ) ; ret += 4 ;
	set_long( &buff[ret], pad_toggle[1] ) ; ret += 4 ;
	set_long( &buff[ret], pad_six_ct[1] ) ; ret += 4 ;
	set_long( &buff[ret], pad_six_tm[1] ) ; ret += 4 ;

	set_long( &buff[ret], vdp_pending               ) ; ret += 4 ;
	set_long( &buff[ret], vdp_cmd1 << 16 | vdp_cmd2 ) ; ret += 4 ;

	set_long( &buff[ret], vdp_mode      ) ; ret += 4 ;
	set_long( &buff[ret], vdp_addr      ) ; ret += 4 ;
	set_long( &buff[ret], vdp_dma       ) ; ret += 4 ;

	ret += SN76496_get_state( &buff[ret] ) ;

	{
		int n ;
		MD_CORE_STATE2* s2 = (MD_CORE_STATE2*)&buff[ret] ;
		s2->fm_sel   [0] = fm_sel   [0] ;
		s2->fm_sel   [1] = fm_sel   [1] ;
		s2->fm_status[0] = fm_status[0] ;
		s2->fm_status[0] = fm_status[1] ;
		for( n=0; n < 4     ; n++ ){ s2->fm_timer[n] = fm_timer[n] ; }
		for( n=0; n < 0x100 ; n++ ){ s2->fm_reg[0][n] = fm_reg[0][n] ; s2->fm_reg[1][n] = fm_reg[1][n] ; }
		ret += sizeof(MD_CORE_STATE2) ; 
	}

	ret += YM2612GetState( &buff[ret] ) ;

	set_long( &buff[ret], rom_bank ) ; ret += 4 ;

	return ret ;

	/*
	int ret = 0 ;
	Z80_STATE z80st ;

	ret += _68K_GET_STATE( (M68K_STATE*)&buff[ret] ) ;

	memcpy( &buff[ret], ram      , 0x10000 ) ; ret += 0x10000 ;
	memcpy( &buff[ret], vdp.reg  , 0x00020 ) ; ret += 0x00020 ;
	memcpy( &buff[ret], vdp.vsram, 0x00050 ) ; ret += 0x00050 ;
	memcpy( &buff[ret], vdp.cram , 0x00080 ) ; ret += 0x00080 ;
	memcpy( &buff[ret], vdp.vram , 0x10000 ) ; ret += 0x10000 ;
	memcpy( &buff[ret], z80ram   , 0x02000 ) ; ret += 0x02000 ;

	//memcpy( &buff[ret], &z80 , sizeof(z80) ) ; ret += sizeof(z80) ;
	z80_get_state( &z80st ) ;
	memcpy( &buff[ret], &z80st , sizeof(z80st) ) ; ret += sizeof(z80st) ;
//	memcpy4byte( &buff[ret], &z80_int_pending  ) ; ret += 4 ;
	memcpy4byte( &buff[ret], &z80_bank68k      ) ; ret += 4 ;
	memcpy4byte( &buff[ret], &z80_online       ) ; ret += 4 ;
//	memcpy4byte( &buff[ret], &z80_extra_cycles ) ; ret += 4 ;

//	memcpy( &buff[ret], dac_data , sizeof(dac_data) ) ; ret += sizeof(dac_data) ;
	ret += sizeof( int[0x138] ) ; // * 0x138 ;
	memcpy4byte( &buff[ret], &dac_enabled      ) ; ret += 4 ;
	memcpy4byte( &buff[ret], &dac_last         ) ; ret += 4 ;
	memcpy4byte( &buff[ret], &pal              ) ; ret += 4 ;

	memcpy4byte( &buff[ret], &aoo3_toggle      ) ; ret += 4 ;
	memcpy4byte( &buff[ret], &aoo3_six         ) ; ret += 4 ;
	memcpy4byte( &buff[ret], &aoo3_six_timeout ) ; ret += 4 ;
	memcpy4byte( &buff[ret], &aoo5_toggle      ) ; ret += 4 ;
	memcpy4byte( &buff[ret], &aoo5_six         ) ; ret += 4 ;
	memcpy4byte( &buff[ret], &aoo5_six_timeout ) ; ret += 4 ;

	memcpy4byte( &buff[ret], &coo_waiting      ) ; ret += 4 ;
	memcpy4byte( &buff[ret], &coo_cmd          ) ; ret += 4 ;

	memcpy4byte( &buff[ret], &vdp.rw_mode      ) ; ret += 4 ;
	memcpy4byte( &buff[ret], &vdp.rw_addr      ) ; ret += 4 ;
	memcpy4byte( &buff[ret], &vdp.rw_dma       ) ; ret += 4 ;

	ret += SN76496_get_state( &buff[ret] ) ;

	memcpy( &buff[ret], fm_sel        , sizeof(fm_sel       ) ) ; ret += sizeof(fm_sel       ) ;
	memcpy( &buff[ret], fm_tover      , sizeof(fm_tover     ) ) ; ret += sizeof(fm_tover     ) ;
	memcpy( &buff[ret], ras_fm_ticker , sizeof(ras_fm_ticker) ) ; ret += sizeof(ras_fm_ticker) ;
	memcpy( &buff[ret], fm_reg        , sizeof(fm_reg       ) ) ; ret += sizeof(fm_reg       ) ;

	ret += YM2612GetState( &buff[ret] ) ;

	return ret ;
	//*/
}

int md_core::set_state ( unsigned char* buff, int buff_len, int ver )
{
	/*
	int n, ret = 0 ;

	MD_CORE_STATE1  s1a ;
	MD_CORE_STATE2  s2a ;
	MD_CORE_STATE1* s1  ;
	MD_CORE_STATE2* s2  ;

	ret += _68K_SET_STATE( (M68K_STATE*)&buff[ret] ) ;

	s1 = &s1a ; //(MD_CORE_STATE1*)&buff[ret] ;
	memcpy( s1, &buff[ret], sizeof(MD_CORE_STATE1) ) ;

	memcpy( core_ram , s1->core_ram   , sizeof(core_ram ) ) ;
	memcpy( vdp_reg  , s1->vdp_reg    , sizeof(vdp_reg  ) ) ;
	memcpy( vdp_vsram, s1->vdp_vsram  , 0x50 ) ; //sizeof(vdp_vsram) ) ;
	memcpy( vdp_cram , s1->vdp_cram   , sizeof(vdp_cram ) ) ;
	memcpy( vdp_vram , s1->vdp_vram   , sizeof(vdp_vram ) ) ;
	memcpy( z80_ram  , s1->z80_ram    , sizeof(z80_ram  ) ) ;
	memcpy( &z80     , &s1->z80_state , sizeof(z80      ) ) ;
	z80_bank68k   = (unsigned short)s1->z80_bank68k        ;
	z80_online    = (unsigned char )s1->z80_online         ;
	dac_enabled   = (         short)s1->dac_enabled        ;
	dac_last      = (         short)s1->dac_last           ;
	vdp_pal       = (unsigned char )s1->vdp_pal            ;
	pad_toggle[0] = (unsigned char )s1->pad_toggle_1       ;
	pad_six_ct[0] = (unsigned char )s1->pad_six_ct1        ;
	pad_six_tm[0] = (unsigned char )s1->pad_six_tm1        ;
	pad_toggle[1] = (unsigned char )s1->pad_toggle_2       ;
	pad_six_ct[1] = (unsigned char )s1->pad_six_ct2        ;
	pad_six_tm[1] = (unsigned char )s1->pad_six_tm2        ;
	vdp_pending   = (unsigned char )s1->vdp_pending        ;
	vdp_cmd1      = (unsigned short)(s1->vdp_cmd >> 16   ) ;
	vdp_cmd2      = (unsigned short)(s1->vdp_cmd & 0xFFFF) ;
	vdp_mode      = (unsigned char )s1->vdp_mode           ;
	vdp_addr      = (unsigned short)s1->vdp_addr           ;
	vdp_dma       = (unsigned char )s1->vdp_dma            ;

	ret += sizeof(MD_CORE_STATE1) ; 

	ret += SN76496_set_state( &buff[ret], buff_len - ret ) ;

	s2 = &s2a ; //(MD_CORE_STATE2*)&buff[ret] ;
	memcpy( s2, &buff[ret], sizeof(MD_CORE_STATE2) ) ;
	
	fm_sel  [0] = s2->fm_sel  [0] ;
	fm_sel  [1] = s2->fm_sel  [1] ;
	fm_tover[0] = s2->fm_tover[0] ;
	fm_tover[0] = s2->fm_tover[1] ;
	for( n=0; n < 4     ; n++ ){ fm_ras_ticker[n] = s2->ras_fm_ticker[n] ; }
	for( n=0; n < 0x100 ; n++ ){ fm_reg[0][n] = s2->fm_reg[0][n] ; fm_reg[1][n] = s2->fm_reg[1][n] ; }

	ret += sizeof(MD_CORE_STATE2) ; 

	if( ver > 1 )
	{
		ret += YM2612SetState( &buff[ret], buff_len - ret ) ;
	}

	fm_flush_reg() ;
	dirt_flag = 0xFFFF ;

	return ret ;
	*/

	int ret = 0 ;
	unsigned long cmd ;
	Z80_STATE z80st ;

	ret += _68K_SET_STATE( (M68K_STATE*)&buff[ret] ) ;

	memcpy( core_ram , &buff[ret], 0x10000 ) ; ret += 0x10000 ;
	memcpy( vdp_reg  , &buff[ret], 0x00020 ) ; ret += 0x00020 ;
	memcpy( vdp_vsram, &buff[ret], 0x00050 ) ; ret += 0x00050 ;
	memcpy( vdp_cram , &buff[ret], 0x00080 ) ; ret += 0x00080 ;
	memcpy( vdp_vram , &buff[ret], 0x10000 ) ; ret += 0x10000 ;
	memcpy( z80_ram  , &buff[ret], 0x02000 ) ; ret += 0x02000 ;

	memcpy( &z80st , &buff[ret], sizeof(z80st) ) ; ret += sizeof(z80st) ;

	z80_bank68k   = (unsigned short)get_long( &buff[ret] ) ; ret += 4 ;
	z80_online    = (unsigned char )get_long( &buff[ret] ) ; ret += 4 ;
	ret += sizeof( int[0x138] ) ; // * 0x138 ;
	dac_enabled   = (         short)get_long( &buff[ret] ) ; ret += 4 ;
	dac_last      = (         short)get_long( &buff[ret] ) ; ret += 4 ;
	vdp_pal       = (unsigned char )get_long( &buff[ret] ) ; ret += 4 ;
	pad_toggle[0] = (unsigned char )get_long( &buff[ret] ) ; ret += 4 ;
	pad_six_ct[0] = (unsigned char )get_long( &buff[ret] ) ; ret += 4 ;
	pad_six_tm[0] = (unsigned char )get_long( &buff[ret] ) ; ret += 4 ;
	pad_toggle[1] = (unsigned char )get_long( &buff[ret] ) ; ret += 4 ;
	pad_six_ct[1] = (unsigned char )get_long( &buff[ret] ) ; ret += 4 ;
	pad_six_tm[1] = (unsigned char )get_long( &buff[ret] ) ; ret += 4 ;
	vdp_pending   = (unsigned char )get_long( &buff[ret] ) ; ret += 4 ;
	cmd           =                 get_long( &buff[ret] ) ; ret += 4 ;
	vdp_cmd1      = (unsigned short)( cmd >> 16    ) ;
	vdp_cmd2      = (unsigned short)( cmd & 0xFFFF ) ;
	vdp_mode      = (unsigned char )get_long( &buff[ret] ) ; ret += 4 ;
	vdp_addr      = (unsigned short)get_long( &buff[ret] ) ; ret += 4 ;
	vdp_dma       = (unsigned char )get_long( &buff[ret] ) ; ret += 4 ;

	ret += SN76496_set_state( &buff[ret], buff_len - ret ) ;

	int n ;
	MD_CORE_STATE2* s2 = (MD_CORE_STATE2*)&buff[ret] ;
	fm_sel   [0] = s2->fm_sel   [0] ;
	fm_sel   [1] = s2->fm_sel   [1] ;
	fm_status[0] = s2->fm_status[0] ;
	fm_status[0] = s2->fm_status[1] ;
	for( n=0; n < 4     ; n++ ){ fm_timer[n] = s2->fm_timer[n] ; }
	for( n=0; n < 0x100 ; n++ ){ fm_reg[0][n] = s2->fm_reg[0][n] ; fm_reg[1][n] = s2->fm_reg[1][n] ; }

	fm_amax = 18  * (1024 - ( (fm_reg[0][0x24]<<2) + (fm_reg[0][0x25]&3) ) ) ;
	fm_bmax = 288 * (256 - fm_reg[0][0x26]) ;
	if( fm_amax <= 0 ){ fm_amax = 1 ; }
	if( fm_bmax <= 0 ){ fm_bmax = 1 ; }

	ret += sizeof(MD_CORE_STATE2) ; 

	if( ver > 1 )
	{
		ret += YM2612SetState( &buff[ret], buff_len - ret ) ;
	}
	fm_flush_reg() ;

	rom_bank = (unsigned char )get_long( &buff[ret] ) ; ret += 4 ;
	if( rom_bank != 0 )
	{
		mdcore_rom_b = &mdcore_ptr->rom_data[ ((rom_bank & 0x1F) -1) << 19 ] ;
	}

#ifdef _USE_REVERS_ENDIAN

	{
		unsigned char* p = vdp_vram ;
		for( int n=0; n < 0x10000 ; n+=2 )
		{
			unsigned char tmp = p[n] ;
			p[n  ] = p[n+1] ;
			p[n+1] = tmp ;
		}
	}
	{
		unsigned char* p = vdp_vsram ;
		for( int n=0; n < 0x00050 ; n+=2 )
		{
			unsigned char tmp = p[n] ;
			p[n  ] = p[n+1] ;
			p[n+1] = tmp ;
		}
	}
	{
		unsigned char* p = vdp_cram ;
		for( int n=0; n < 0x00080 ; n+=2 )
		{
			unsigned char tmp = p[n] ;
			p[n  ] = p[n+1] ;
			p[n+1] = tmp ;
		}
	}
	
#endif // _USE_REVERS_ENDIAN

	z80_set_state( &z80st ) ;

	dirt_flag = DIRT_ALL ;

	return ret ;

#if 0
	int ret = 0 ;
	//struct mz80context mz80ct ;
	Z80_STATE z80st ;

	/*
 	int i, t ;
   for (i=0;i<8;i++)
	{ 
		memcpy4byte( &t, &buff[ret] ) ; ret += 4 ;
		_68K_SET_DR(i,t); 
	}

    for (i=0;i<8;i++)
	{ 
		memcpy4byte( &t, &buff[ret] ) ; ret += 4 ;
		_68K_SET_AR(i,t);
	}

	memcpy4byte( &t, &buff[ret] ) ; ret += 4 ; _68K_SET_PC  (t);
	memcpy4byte( &t, &buff[ret] ) ; ret += 4 ; _68K_SET_SR  (t);
	memcpy4byte( &t, &buff[ret] ) ; ret += 4 ; _68K_SET_IR  (t);
	memcpy4byte( &t, &buff[ret] ) ; ret += 4 ; _68K_SET_T1  (t);
	memcpy4byte( &t, &buff[ret] ) ; ret += 4 ; _68K_SET_T0  (t);
	memcpy4byte( &t, &buff[ret] ) ; ret += 4 ; _68K_SET_SF  (t);
	memcpy4byte( &t, &buff[ret] ) ; ret += 4 ; _68K_SET_MF  (t);
	memcpy4byte( &t, &buff[ret] ) ; ret += 4 ; _68K_SET_INTM(t);
	memcpy4byte( &t, &buff[ret] ) ; ret += 4 ; _68K_SET_XF  (t);
	memcpy4byte( &t, &buff[ret] ) ; ret += 4 ; _68K_SET_NF  (t);
	memcpy4byte( &t, &buff[ret] ) ; ret += 4 ; _68K_SET_ZF  (t);
	memcpy4byte( &t, &buff[ret] ) ; ret += 4 ; _68K_SET_VF  (t);
	memcpy4byte( &t, &buff[ret] ) ; ret += 4 ; _68K_SET_CF  (t);
	memcpy4byte( &t, &buff[ret] ) ; ret += 4 ; _68K_SET_USP (t);
	memcpy4byte( &t, &buff[ret] ) ; ret += 4 ; _68K_SET_ISP (t);
	memcpy4byte( &t, &buff[ret] ) ; ret += 4 ; _68K_SET_MSP (t);
	//*/

	ret += _68K_SET_STATE( (M68K_STATE*)&buff[ret] ) ;

	memcpy( ram      , &buff[ret], 0x10000 ) ; ret += 0x10000 ;
	memcpy( vdp.reg  , &buff[ret], 0x00020 ) ; ret += 0x00020 ;
	memcpy( vdp.vsram, &buff[ret], 0x00050 ) ; ret += 0x00050 ;
	memcpy( vdp.cram , &buff[ret], 0x00080 ) ; ret += 0x00080 ;
	memcpy( vdp.vram , &buff[ret], 0x10000 ) ; ret += 0x10000 ;
	memcpy( z80ram   , &buff[ret], 0x02000 ) ; ret += 0x02000 ;

	memcpy( &z80st , &buff[ret], sizeof(z80st) ) ; ret += sizeof(z80st) ;
//	memcpy4byte( &z80_int_pending , &buff[ret] ) ; ret += 4 ;
	memcpy4byte( &z80_bank68k     , &buff[ret] ) ; ret += 4 ;
	memcpy4byte( &z80_online      , &buff[ret] ) ; ret += 4 ;
//	memcpy4byte( &z80_extra_cycles, &buff[ret] ) ; ret += 4 ;

//	memcpy( dac_data , &buff[ret], sizeof(dac_data) ) ; ret += sizeof(dac_data) ;
	ret += sizeof( int[0x138] ) ; // * 0x138 ;
	memcpy4byte( &dac_enabled     , &buff[ret] ) ; ret += 4 ;
	memcpy4byte( &dac_last        , &buff[ret] ) ; ret += 4 ;
	memcpy4byte( &pal             , &buff[ret] ) ; ret += 4 ;

	memcpy4byte( &aoo3_toggle     , &buff[ret] ) ; ret += 4 ;
	memcpy4byte( &aoo3_six        , &buff[ret] ) ; ret += 4 ;
	memcpy4byte( &aoo3_six_timeout, &buff[ret] ) ; ret += 4 ;
	memcpy4byte( &aoo5_toggle     , &buff[ret] ) ; ret += 4 ;
	memcpy4byte( &aoo5_six        , &buff[ret] ) ; ret += 4 ;
	memcpy4byte( &aoo5_six_timeout, &buff[ret] ) ; ret += 4 ;

	memcpy4byte( &coo_waiting     , &buff[ret] ) ; ret += 4 ;
	memcpy4byte( &coo_cmd         , &buff[ret] ) ; ret += 4 ;

	memcpy4byte( &vdp.rw_mode     , &buff[ret] ) ; ret += 4 ;
	memcpy4byte( &vdp.rw_addr     , &buff[ret] ) ; ret += 4 ;
	memcpy4byte( &vdp.rw_dma      , &buff[ret] ) ; ret += 4 ;

	ret += SN76496_set_state( &buff[ret], buff_len - ret ) ;

	memcpy( fm_sel        , &buff[ret], sizeof(fm_sel       ) ) ; ret += sizeof(fm_sel       ) ;
	memcpy( fm_status     , &buff[ret], sizeof(fm_status    ) ) ; ret += sizeof(fm_status    ) ;
	memcpy( ras_fm_ticker , &buff[ret], sizeof(ras_fm_ticker) ) ; ret += sizeof(ras_fm_ticker) ;
	memcpy( fm_reg        , &buff[ret], sizeof(fm_reg       ) ) ; ret += sizeof(fm_reg       ) ;

	if( ver > 1 )
	{
		ret += YM2612SetState( &buff[ret], buff_len - ret ) ;
	}
	flush_fm_to_mame() ;
	//YM2612SetRegs( (short*)&fm_reg[0][0] ) ;

	/*
	z80.z80clockticks    = mz80ct.z80clockticks     ;
	z80.z80iff           = mz80ct.z80iff            ;
	z80.z80interruptMode = mz80ct.z80interruptMode  ;
	z80.z80halted        = mz80ct.z80halted         ;
	z80.z80af            = mz80ct.z80af             ;
	z80.z80bc            = mz80ct.z80bc             ;
	z80.z80de            = mz80ct.z80de             ;
	z80.z80hl            = mz80ct.z80hl             ;
	z80.z80afprime       = mz80ct.z80afprime        ;
	z80.z80bcprime       = mz80ct.z80bcprime        ;
	z80.z80deprime       = mz80ct.z80deprime        ;
	z80.z80hlprime       = mz80ct.z80hlprime        ;
	z80.z80ix            = mz80ct.z80ix             ;
	z80.z80iy            = mz80ct.z80iy             ;
	z80.z80sp            = mz80ct.z80sp             ;
	z80.z80pc            = mz80ct.z80pc             ;
	z80.z80nmiAddr       = mz80ct.z80nmiAddr        ;
	z80.z80intAddr       = mz80ct.z80intAddr        ;
	z80.z80rCounter      = mz80ct.z80rCounter       ;
	z80.z80i             = mz80ct.z80i              ;
	z80.z80r             = mz80ct.z80r              ;
	z80.z80intPending    = mz80ct.z80intPending     ;
	*/
	z80_set_state( &z80st ) ;

	//memset(vdp.dirt,0xff,0x35); // mark everything as changed
	vdp.dirt_flag = 0xFFFF ;

	return ret ;

#endif /////////////////////////////////////////////////////////////////

}

//////////////////////////////////////////////////////
// for MEM
//////////////////////////////////////////////////////

extern "C"
{


#ifdef MEMORY_EXT
unsigned int  m68k_read_memory_8  ( unsigned int a )
#else  //
unsigned int  md_core::read_byte  ( unsigned int a )
#endif //

{
#ifdef _DEBUG_MEM
	if( debug_mem_r )
	{
		if( (debug_bp == 0) || ((debug_bp <= a) && (a <= debug_bp_m)) )
		{
			QueryPerformanceCounter( &li2 ) ;
			TRACE( "%10d:BR:%08X\n", li2.LowPart - li1.LowPart , a );
			::Sleep(1);
			li1 = li2 ;
		}
	}

#endif //_DEBUG_MEM

#ifdef _USE_BANK_CANGE
	if     ( a < MAX_ROM_BANKED ){ return mdcore_rom  [(a^1)]; }
	else if( a < MAX_ROM_SIZE   ){ return mdcore_rom_b[((a - MAX_ROM_BANKED)^1)]; }
#else
	if     ( a < MAX_ROM_SIZE ){ return mdcore_rom[(a^1)]; }
#endif // _USE_BANK_CANGE

	else if( a > 0x00dfffff   ){ return mdcore_ram[(a^1)&0xFFFF]; }
	else if( a < 0x00a04000   ){ return mdcore_ptr->z80_ram[(a)&0x1FFF]; }
	else
	{

	switch(a)
	{
	case 0x00c00000 :
	case 0x00c00001 : return mdcore_ptr->vdp_read_byte() ;

	case 0x00a04000 : return mdcore_ptr->fm_read(0) ;
	case 0x00a04001 : return mdcore_ptr->fm_read(1) ;
	case 0x00a04002 : return mdcore_ptr->fm_read(2) ;
	case 0x00a04003 : return mdcore_ptr->fm_read(3) ;

	case 0x00a10001 : return mdcore_ptr->rom_get_country_ver() ;

	case 0x00a10002 :
	case 0x00a10003 : return mdcore_ptr->pad_read1() ;

	case 0x00a10004 :
	case 0x00a10005 : return mdcore_ptr->pad_read2() ;

	case 0x00a11100 : return mdcore_ptr->z80_online  ;

	case 0x00c00004 : mdcore_ptr->vdp_pending = 0 ;
					  mdcore_ptr->vdp_status1 ^= 0x03 ; 
					  return mdcore_ptr->vdp_status1 ;

	case 0x00c00005 : mdcore_ptr->vdp_pending = 0 ;
					  return mdcore_ptr->vdp_status2 | mdcore_ptr->vdp_pal ;

	case 0x00c00008 : return mdcore_ptr->vdp_get_status() >> 8   ;
	case 0x00c00009 : return mdcore_ptr->vdp_get_status() & 0xFF ;

	case 0x00a11000 :
	case 0x00a11001 :
	case 0x00a11200 :
	case 0x00a11201 : return 0xFF ;

	case 0x00a130f1 : return mdcore_ptr->save_active | mdcore_ptr->save_prot ;

	// for MODEM ->
	case 0x00a1000F :
	case 0x00a10015 :
	case 0x00a1001B : return 0xFF ;
	case 0x00a10007 : return 0x7F ; // <- Important !
	// for MODEM <-

#ifdef _DEBUG_MEM2
	case 0x00a10006 :
	case 0x00a10008 :
	case 0x00a10009 :
	case 0x00a1000A :
	case 0x00a1000B :
	case 0x00A1000C :
	case 0x00A1000D :
	case 0x00a11101 :
		break ;
	default :
		TRACE( "BR:%08X\n", a );
		//ASSERT(FALSE) ;
		break ;
#endif //_DEBUG_MEM
	}

	}

	return 0x00 ;
}

#ifdef MEMORY_EXT
unsigned int  m68k_read_memory_16  ( unsigned int a )
#else  //
unsigned int  md_core::read_word  ( unsigned int a )
#endif //

{
#ifdef _DEBUG_MEM
	if( debug_mem_r )
	{
		//if( (a % 0x1000) == 0 )
		if( (debug_bp == 0) || ((debug_bp <= a) && (a <= debug_bp_m)) )
		{
			QueryPerformanceCounter( &li2 ) ;
			TRACE( "%10d:WR:%08X\n", li2.LowPart - li1.LowPart, a );
			::Sleep(1);
			li1 = li2 ;
		}
	}
#endif //_DEBUG_MEM

#ifdef _USE_BANK_CANGE
	if (     a < MAX_ROM_BANKED ){ return *((unsigned short*)&mdcore_rom  [(a                 ) ]); }
	else if( a < MAX_ROM_SIZE   ){ return *((unsigned short*)&mdcore_rom_b[(a - MAX_ROM_BANKED) ]); }
#else
	if (     a < MAX_ROM_SIZE ){ return *((unsigned short*)&mdcore_rom[(a)       ]); }
#endif // _USE_BANK_CANGE

	else if( a > 0x00dfffff   ){ return *((unsigned short*)&mdcore_ram[(a)&0xffff]); }

	switch( a )
	{
	case 0x00c00000 :
	case 0x00c00002 : return mdcore_ptr->vdp_read_word() ;

	case 0x00c00004 : 
		mdcore_ptr->vdp_pending = 0 ;
		mdcore_ptr->vdp_status1 ^= 0x03 ;
		return (mdcore_ptr->vdp_status1 << 8) | mdcore_ptr->vdp_status2 | mdcore_ptr->vdp_pal ;

	case 0x00c00008 : return mdcore_ptr->vdp_get_status() ;
	}

	return m68k_read_memory_8(a)<<8 | m68k_read_memory_8(a+1) ;
}

#ifdef MEMORY_EXT
unsigned int  m68k_read_memory_32 ( unsigned int a )
#else  //
unsigned int  md_core::read_long  ( unsigned int a )
#endif //

{
#ifdef _DEBUG_MEM
	if( debug_mem_r )
	{
		if( (debug_bp == 0) || ((debug_bp <= a) && (a <= debug_bp_m)) )
		{
			QueryPerformanceCounter( &li2 ) ;
			TRACE( "%10d:DR:%08X\n", li2.LowPart - li1.LowPart, a );
			::Sleep(1);
			li1 = li2 ;
		}
	}
#endif //_DEBUG_MEM

	unsigned long dd ;

	if( a & 3 )
	{
#ifdef _USE_BANK_CANGE
		if ( a < MAX_ROM_BANKED )
		{
			return *((unsigned short*)&mdcore_rom[(a)]) << 16 | *((unsigned short*)&mdcore_rom[(a+2)]);
		}
		else if ( a < MAX_ROM_SIZE )
		{
			return *((unsigned short*)&mdcore_rom_b[(a   - MAX_ROM_BANKED)]) << 16 
				 | *((unsigned short*)&mdcore_rom_b[(a+2 - MAX_ROM_BANKED)]);
		}
#else
		if ( a < MAX_ROM_SIZE )
		{
			return *((unsigned short*)&mdcore_rom[(a)]) << 16 | *((unsigned short*)&mdcore_rom[(a+2)]);
		}
#endif // _USE_BANK_CANGE
		else if( a > 0x00dfffff )
		{ 
			return *((unsigned short*)&mdcore_ram[(a)&0xffff]) << 16 | *((unsigned short*)&mdcore_ram[(a+2)&0xffff] ); 
		}
	}
	else
	{

#ifdef _USE_BANK_CANGE
		if ( a < MAX_ROM_BANKED )
		{
			dd = *((unsigned long*)&mdcore_rom[(a)]) ;
			return (dd << 16) | (dd >> 16) ;
		}
		else if ( a < MAX_ROM_SIZE )
		{
			dd = *((unsigned long*)&mdcore_rom_b[(a - MAX_ROM_BANKED)]) ;
			return (dd << 16) | (dd >> 16) ;
		}
#else
		if ( a < MAX_ROM_SIZE )
		{
			dd = *((unsigned long*)&mdcore_rom[(a)]) ;
			return (dd << 16) | (dd >> 16) ;
		}
#endif // _USE_BANK_CANGE

		else if( a > 0x00dfffff )
		{ 
			dd = *((unsigned long*)&mdcore_ram[(a)&0xffff]) ;
			return (dd << 16) | (dd >> 16) ;	
		}
	}

	if( (a&0xfffffffc) == 0x00C00000 ) // 00 or 02
	{
		return mdcore_ptr->vdp_read_long() ;
	}

	return m68k_read_memory_16(a)<<16 | m68k_read_memory_16(a+2) ;
}

#ifdef MEMORY_EXT
void  m68k_write_memory_8 ( unsigned int a, unsigned int d )
#else // 
void  md_core::write_byte ( unsigned int a, unsigned int d )
#endif //

{
#ifdef _DEBUG_MEM
	if( debug_mem_w ) //|| d > 0xFF )
	{
		if( (debug_bp == 0) || ((debug_bp <= a) && (a <= debug_bp_m)) )
		{
			QueryPerformanceCounter( &li2 ) ;
			TRACE( "%10d:BW:%08X:%08X\n", li2.LowPart - li1.LowPart, a, d );
			::Sleep(1);
			li1 = li2 ;
		}
	}
#endif //_DEBUG_MEM

	if( a > 0x00dfffff ){ mdcore_ram[(a^1)&0xffff] = (unsigned char)d ; return ; }
	else if ( a < MAX_ROM_SIZE ) 
	{
		mdcore_ptr->save_update = 1 ;
		mdcore_rom[(a^1)] = (unsigned char)d ; 
		return ;
	}
	else if( a < 0x00a04000 ){ mdcore_ptr->z80_ram[a&0x1fff] = (unsigned char)d ; return ; }
	else 
	{

	switch(a)
	{
	case 0x00c00000 :
	case 0x00c00001 :
	case 0x00c00002 :
	case 0x00c00003 : mdcore_ptr->vdp_write_word( (unsigned short)(d | (d << 8)) ); break ;

	case 0x00a07f11 :
	case 0x00c00011 :
	case 0x00c00017 : SN76496Write( 0, d ); break ;

	case 0x00a04000 : mdcore_ptr->fm_write( 0, (unsigned char)d ) ; break ;
	case 0x00a04001 : mdcore_ptr->fm_write( 1, (unsigned char)d ) ; break ;
	case 0x00a04002 : mdcore_ptr->fm_write( 2, (unsigned char)d ) ; break ;
	case 0x00a04003 : mdcore_ptr->fm_write( 3, (unsigned char)d ) ; break ;

	case 0x00a06000 : mdcore_ptr->z80_set_bank  ( (unsigned short)d        ) ; break ;
	case 0x00a11100 : mdcore_ptr->z80_set_online( (unsigned short)(d << 8) ) ; break ;
	case 0x00a11200 : mdcore_ptr->z80_set_reset ( (unsigned short)d        ) ; break ;
	//case 0x00a11201 : break ;

	case 0x00a10003 : mdcore_ptr->pad_write1( (unsigned char)d ) ; break ;
	case 0x00a10005 : mdcore_ptr->pad_write2( (unsigned char)d ) ; break ;

	case 0x00a130F1 : mdcore_ptr->write_save_flag( (unsigned char)d ) ; break ;

#ifdef _DEBUG_MEM
	case 0x00a130FD :  
		TRACE( "0x00a130FD : %02X\n", d );
		break ;
#endif //

	case 0x00a130FF :  
#ifdef _USE_BANK_CANGE
#ifdef _DEBUG_MEM
		TRACE( "0x00a130FF : %02X, %08X\n", d, ((d & 0x1F) -1) << 19 ) ; //* 0x80000 );
#endif //
		mdcore_ptr->rom_bank = d ;
		mdcore_rom_b = &mdcore_ptr->rom_data[ ((d & 0x1F) -1) << 19 ] ;
#endif // _USE_BANK_CANGE
		break ;

#ifdef _DEBUG_MEM2
	case 0x00a10006 :
	case 0x00a10007 :
	case 0x00a10008 :
	case 0x00a10009 :
	case 0x00a1000A :
	case 0x00a1000B :
	case 0x00A1000C :
	case 0x00A1000D :
		break ;
	//case 0x00A11101: break ;
	default : TRACE( "BW:%08X:%08X\n", a, d ); /*ASSERT(FALSE);*/ break ;
#endif //_DEBUG_MEM

	}

	}
}

#ifdef MEMORY_EXT
void  m68k_write_memory_16 ( unsigned int a, unsigned int d )
#else // 
void  md_core::write_word  ( unsigned int a, unsigned int d )
#endif //

{
#ifdef _DEBUG_MEM
	if( debug_mem_w ) //|| (d > 0xFFFF) )
	{
		if( (debug_bp == 0) || ((debug_bp <= a) && (a <= debug_bp_m)) )
		{
			QueryPerformanceCounter( &li2 ) ;
			TRACE( "%10d:WW:%08X:%08X\n", li2.LowPart - li1.LowPart, a, d );
			::Sleep(1);
			li1 = li2 ;
		}
	}
#endif //_DEBUG_MEM

	if( a > 0x00dfffff ){ *((unsigned short*)&mdcore_ram[(a)&0xffff]) = (unsigned short)d ; }
	else
	{
		switch( a )
		{
		case 0x00c00000 :
		case 0x00c00001 :
		case 0x00c00002 :
		case 0x00c00003 : mdcore_ptr->vdp_write_word( (unsigned short)d ); break ;

		case 0x00c00004 :
		case 0x00c00005 :
		case 0x00c00006 :
		case 0x00c00007 : mdcore_ptr->vdp_command ( (unsigned short)d ) ; break ;

		case 0x00A11100 : mdcore_ptr->z80_set_online( (unsigned short)d ) ; break ;
		case 0x00a11200 : mdcore_ptr->z80_set_reset ( (unsigned short)d ) ; break ;

		default :
			m68k_write_memory_8( a   , d >> 8    );
			m68k_write_memory_8( a+1 , d &  0xFF );
			break ;
		}
	}
}

#ifdef MEMORY_EXT
void  m68k_write_memory_32 ( unsigned int a, unsigned int d )
#else // 
void  md_core::write_long  ( unsigned int a, unsigned int d )
#endif //

{
#ifdef _DEBUG_MEM
	if( debug_mem_w )
	{
		if( (debug_bp == 0) || ((debug_bp <= a) && (a <= debug_bp_m)) )
		{
			QueryPerformanceCounter( &li2 ) ;
			TRACE( "%10d:DW:%08X:%08X\n", li2.LowPart - li1.LowPart, a, d );
			::Sleep(1);
			li1 = li2 ;
		}
	}
#endif //_DEBUG_MEM

	if( a > 0x00dfffff )
	{ 
		if( a & 0x03 )
		{
			*((unsigned short*)&mdcore_ram[(a  )&0xffff]) = (unsigned short)(d >> 16) ;
			*((unsigned short*)&mdcore_ram[(a+2)&0xffff]) = (unsigned short)(d      ) ;
		}
		else
		{
			*((unsigned long*)&mdcore_ram[(a+0)&0xffff]) = (unsigned long)((d >> 16) | (d << 16)) ;
		}
		return ;
	}

	switch( a )
	{
	case 0x00c00000 :
	case 0x00c00001 :
	case 0x00c00002 :
	case 0x00c00003 : mdcore_ptr->vdp_write_long( d ); return ;

	case 0x00c00004 :
	case 0x00c00005 :
	case 0x00c00006 :
	case 0x00c00007 : mdcore_ptr->vdp_command_l( d ) ; return ;
	}

	m68k_write_memory_16( a  , (d>>16)&0xffff );
	m68k_write_memory_16( a+2,  d     &0xffff );
}

} // extern "C"
//////////////////////////////////////////////////////
// for VDP
//////////////////////////////////////////////////////

unsigned short md_core::vdp_get_status ( )
{
	unsigned short hv ;

	int clk = _68K_GET_CYCLE()  ;

	/*
	if( (vdp_status2 & 4) == 0 )
	{
		clk += _scanlength - 404 ;
	}
	*/

	clk = (clk*416) / clk_line ;

	hv = line_count ;

	if( clk > 330 ){ hv++ ; }

	if( hv > ( VBLANK_LINE + 0x0A ) ){ hv -= 6 ; }

	hv = hv << 8 ;

	hv |= ( clk & 0xFF ) ;

#ifdef DEBUG_MAIN 
	TRACE( "VDP_STATUS( %02X ), %d\n", hv, line_count );
#endif //

	return hv ;
}

//////////////////////////////////////////////////////
// for PAD
//////////////////////////////////////////////////////

#define MD_PAD_UP    (0x00000001)
#define MD_PAD_DOWN  (0x00000002)
#define MD_PAD_LEFT  (0x00000004)
#define MD_PAD_RIGHT (0x00000008)
#define MD_PAD_A     (0x00001000)
#define MD_PAD_B     (0x00000010)
#define MD_PAD_C     (0x00000020)
#define MD_PAD_X     (0x00040000)
#define MD_PAD_Y     (0x00020000)
#define MD_PAD_Z     (0x00010000)
#define MD_PAD_MODE  (0x00080000)
#define MD_PAD_START (0x00002000)

#define MD_PAD_CBRLDU (MD_PAD_C | MD_PAD_B | MD_PAD_RIGHT | MD_PAD_LEFT | MD_PAD_DOWN | MD_PAD_UP)
#define MD_PAD_0000DU (MD_PAD_DOWN | MD_PAD_UP)
#define MD_PAD_SA0000 (MD_PAD_A | MD_PAD_START)
#define MD_PAD_CB0000 (MD_PAD_C | MD_PAD_B)
#define MD_PAD_00MXYZ (MD_PAD_MODE | MD_PAD_X | MD_PAD_Y | MD_PAD_Z)

#define PAD_1CBRLDU( pad )  (unsigned char)(0x40 | (pad & MD_PAD_CBRLDU))
#define PAD_0SA00DU( pad )  (unsigned char)( ( (pad & MD_PAD_SA0000) >> 8) | (pad & MD_PAD_0000DU) )
#define PAD_0SA0000( pad )  (unsigned char)( (pad & MD_PAD_SA0000) >> 8  )
#define PAD_0SA1111( pad )  (unsigned char)(((pad & MD_PAD_SA0000) >> 8) | 0x0F)
#define PAD_1CBMXYZ( pad )  (unsigned char)(0x40 | (pad & MD_PAD_CB0000) | ( (pad & MD_PAD_00MXYZ) >> 16 ))

unsigned char md_core::pad_read1()
{
	//TRACE( "PAD1 R : %02X, %02X, %02X\n", (pad_use_six & pad_six_ct[0]) | (pad_toggle[0] & 0x40), pad_six_ct[0], pad_toggle[0] );

	switch( (pad_use_six & pad_six_ct[0]) | (pad_toggle[0] & 0x40) )
	{
	case 0x00 : if( pad_six_ct[0] == 4 ){ return PAD_0SA1111( pad[0] ) ; }
	case 0x01 : return PAD_0SA00DU( pad[0] ) ;
	case 0x02 : 
	case 0x03 : return PAD_0SA0000( pad[0] ) ;
	//case 0x04 : return PAD_0SA1111( pad[0] ) ;
	case 0x40 : 
	case 0x41 : return PAD_1CBRLDU( pad[0] ) ;
	case 0x42 : 
	case 0x43 : return PAD_1CBMXYZ( pad[0] ) ;
	}

	return 0xFF ;
}

unsigned char md_core::pad_read2()
{
	//TRACE( "PAD2 : %02X, %02X, %02X\n", (pad_use_six & pad_six_ct[1]) | (pad_toggle[1] & 0x40), pad_six_ct[1], pad_toggle[1] );

	switch( (pad_use_six & pad_six_ct[1]) | (pad_toggle[1] & 0x40) )
	{
	case 0x00 : if( pad_six_ct[1] == 4 ){ return PAD_0SA1111( pad[1] ) ; }
	case 0x01 : return PAD_0SA00DU( pad[1] ) ;
	case 0x02 : 
	case 0x03 : return PAD_0SA0000( pad[1] ) ;
	//case 0x04 : return PAD_0SA1111( pad[1] ) ;
	case 0x40 : 
	case 0x41 : return PAD_1CBRLDU( pad[1] ) ;
	case 0x42 : 
	case 0x43 : return PAD_1CBMXYZ( pad[1] ) ;
	}

	return 0xFF ;
}

void md_core::pad_write1  ( unsigned char d )
{
//	TRACE( "PAD1 W : %02X, %02X, %02X\n", d, pad_toggle[0], pad_six_ct[0] );

	if( (d&0x40)==0 && (pad_toggle[0] & 0x40) ){ pad_six_ct[0]++; }
	pad_toggle[0] = d ;
	pad_six_tm[0] = 0 ;
}

void md_core::pad_write2  ( unsigned char d )
{
	//TRACE( "PAD2 W : %02X, %02X, %02X\n", d, pad_toggle[1], pad_six_ct[1] );
	if( (d&0x40)==0 && (pad_toggle[1] & 0x40) ){ pad_six_ct[1]++; }
	pad_toggle[1] = d ;
	pad_six_tm[1] = 0 ;
}

//////////////////////////////////////////////////////
// for DAC
//////////////////////////////////////////////////////
void md_core::dac_init() 
{ 
	int n ;
	dac_last    = 0 ; 
	dac_enabled = 0 ; 
	dac_play    = 0 ;
	dac_frame   = 2 ;
	//dac_rep     = 0 ;
	//dac_clear(); 
	for( n=0; n < MAX_DAC_BANK; n++ ){ dac_play = n; dac_clear() ; }
	dac_play = 0 ; 
}

void md_core::dac_clear() 
{
	short* dac_p = &dac_data[dac_play & MAX_DAC_BANK][0] ;
	//dac_p[0] = dac_last;
	//for(int i = 1; i < 0x138; ++i){ dac_p[i] = 1; }
	for(int i = 0; i < 0x138; ++i){ dac_p[i] = 1; }
}

/*
void md_core::dac_submit( int d ) 
{ 
	//dac_last = (d - 0x80) << 6; 
	//if(dac_enabled) dac_data[dac_frame & MAX_DAC_BANK][ras] = dac_last;
	if( dac_enabled ){ dac_data[dac_frame & MAX_DAC_BANK][ras] = (d - 0x80) << 6 ; }
}
*/

void md_core::dac_enable( unsigned char d )
{
#if 0
	dac_enabled = d & 0x80 ;
	dac_data[dac_play & MAX_DAC_BANK][line_count] = (dac_enabled ? dac_last : 1);
#else 
	if( d & 0x80 )
	{
		if( dac_enabled == 0 )
		{
			//dac_play    = 0 ; 
			//dac_frame   = 2 ;
			dac_enabled = 1 ;
			//dac_clear()     ;
		}
		else //if(dac_enabled == 2)
		{
			dac_enabled = 1 ;

			//short* dac_p = &dac_data[dac_frame & MAX_DAC_BANK][0] ;
			//for(int i = 0; i < 0x138; ++i){ dac_p[i] = 1; }
			//dac_p[line_count] = dac_last ;
		}
	}
	else
	{
		dac_enabled = dac_frame ;
	}
#endif 
	//dac_enabled = d & 0x80 ;
	//dac_data[dac_frame & MAX_DAC_BANK][ras] = (dac_enabled? dac_last : 1);
}

void md_core::dac_make ( unsigned short* p, short len )
{
	if( (sound_flag & 0x40) == 0 )
	{
		SN76496Update_16( 0, p, len );

		if( ((sound_flag & 0x80) == 0) ) //&& dac_enabled )
		{
			int i, lpf, in_dac, cur_dac = dac_last , acc_dac = len;
			short* pl ;

			{
				lpf = LINES_PER_FRAME ;
				pl  = (short*)p ; //sound_buff ;
				short* dac = dac_data[dac_play & MAX_DAC_BANK] ; // -1 ;

				for(i = 0; i < len; ++i)
				{
					acc_dac += lpf ;
					if(acc_dac >= len)
					{
						acc_dac -= len;
						in_dac = *dac ; *dac = 1; dac++ ;
						if(in_dac != 1){ cur_dac = in_dac ; }
					}

					in_dac = cur_dac + *pl ;
					if(      in_dac >  32767 ){ *pl =  32767 ; }
					else if( in_dac < -32768 ){ *pl = -32768 ; }
					else                      { *pl = in_dac ; }

					pl++ ;
				}
			}

			if( (dac_play) < dac_frame )
			{
				if( (dac_play+2) < dac_frame )
				{
					dac_play = dac_frame - 2 ;
				}
				else
				{
					dac_play++  ;
				}
			}
			
			if( dac_play > 0x3FFF )
			{
				dac_play  = 0 ; 
				dac_frame = 2 ; 
			}

			dac_last = cur_dac ;
		}
	}
	else
	{
		for( int n=0; n < len ; n++ ){ p[n] = 0 ; }
	}
}

//////////////////////////////////////////////////////
// for FM
//////////////////////////////////////////////////////
void md_core::fm_write ( unsigned char a, unsigned char d )
{
	unsigned char sid ;

	a &= 3;

	//sid = ( a & 2 ) ? 1 : 0 ;
	sid = ( a >> 1 ) & 1 ;

	if( a & 1 )
	{ 
		unsigned char sel = fm_sel[sid] ;

		fm_reg[ sid ][ sel & 0xFF ] = d ;

		switch( sel )
		{
		case 0x2A : 
			//dac_submit( d ); 
			if( dac_enabled ){ dac_data[dac_frame & MAX_DAC_BANK][line_count] = (d - 0x80) << 6 ; }
			//if( dac_enabled ){ dac_data[dac_play & MAX_DAC_BANK][line_count] = (d - 0x80) << 6 ; }
			return ;

		case 0x2B : 
			dac_enable( d ); 
			return ;

		case 0x27 : 
			if( d & 0x10 ){ fm_status[0] &= ~1 ; } //fm_status[1] &= ~1 ; }		
			if( d & 0x20 ){ fm_status[0] &= ~2 ; } //fm_status[1] &= ~2 ; }
			break ;

		case 0x24 :
		case 0x25 :
			fm_amax = 18  * (1024 - ( (fm_reg[0][0x24]<<2) + (fm_reg[0][0x25]&3) ) ) ;
			if( fm_amax <= 0 ){ fm_amax = 1 ; }
			return ;

		case 0x26 :
			fm_bmax = 288 * (256 - fm_reg[0][0x26]) ;
			if( fm_bmax <= 0 ){ fm_bmax = 1 ; }
			return ;

		default   : 
			if( (sel & 0xFC) == 0x24 ) 
			{ 
				return ; 
			}
			break ;
		}
	}
	else
	{
		fm_sel[sid] = d ; 
	}

	//if (sound_is_okay)
		YM2612Write( a, d ) ;
}

unsigned char md_core::fm_read ( unsigned char a )
{
	if( a & 1 )
	{
		return YM2612Read( a & 3 );
	}

	return fm_status[0]; //( z80_in ) ? 0 : 1 ] ;
}

void md_core::fm_timer_update ()
{
	int amax = fm_amax ;
	int bmax = fm_bmax ;

	fm_timer[0] += 64 ;
	fm_timer[1] += 64 ;

	if( fm_timer[0] >= amax )
	{
		fm_timer [0] -= amax ; 
		fm_status[0] |= 1    ; 
	}

	if( fm_timer[1] >= bmax )
	{
		fm_timer [1] -= bmax ; 
		fm_status[0] |= 2    ; 
	}
}

void md_core::fm_timer_update2 ()
{
	int amax = fm_amax ;
	int bmax = fm_bmax ;

	fm_timer[2] += 64 ;
	fm_timer[3] += 64 ;

	if( fm_timer[2] >= amax )
	{
		fm_timer [2] -= amax ; 
		fm_status[1] |= 1    ; 
	}

	if( fm_timer[3] >= bmax )
	{
		fm_timer [3] -= bmax ; 
		fm_status[1] |= 2    ; 
	}
}

void md_core::fm_flush_reg()
{
	YM2612SetRegs( (short*)&fm_reg[0][0] ) ;
}

void md_core::fm_make( unsigned short* p, short len )
{
	// Add in the stereo FM buffer
	FMSAMPLE *buf[2];
	buf[0] = (FMSAMPLE*) p ;
	buf[1] = (FMSAMPLE*) p ;

	//if( (snd_mute & 0x3F) == 0 )
	if( (sound_flag & 0x01) == 0 )
	{
		YM2612UpdateOne( buf, len );
	}
	else
	{
		unsigned long* pt = (unsigned long*)p ;
		for( int n=0; n < len ; n++ ){ pt[n] = 0 ; }
	}
}

//////////////////////////////////////////////////////
// for Z80
//////////////////////////////////////////////////////


#ifdef MEMORY_EXT
extern "C" {
#endif //

#ifdef MEMORY_EXT
unsigned int z80_read(unsigned int a)
#else //
unsigned int md_core::z80_read(unsigned int a)
#endif //

{
	if     ( a <  0x2000 ){ return mdcore_ptr->z80_ram[a]        ; }
	else if( a <  0x4000 ){ return mdcore_ptr->z80_ram[a&0x1fff] ; }
	else if( a <  0x6000 ){ return mdcore_ptr->fm_read( a )    ; }
	else if( a >= 0x8000 )
	{ 

#ifdef _DEBUG_MEM3
		TRACE( "ZR:%08X\n", (mdcore_ptr->z80_bank68k << 15) + (a & 0x7fff) );
#endif // _DEBUG_MEM3

		return m68k_read_memory_8( (mdcore_ptr->z80_bank68k << 15) + (a & 0x7fff) ); 
	}

#ifdef _DEBUG_MEM2
	else { ASSERT(FALSE) ; }
#endif //_DEBUG_MEM2

	return 0 ;
}

#ifdef MEMORY_EXT
void z80_write(unsigned int a,unsigned int v)
#else //
void md_core::z80_write(unsigned int a,unsigned int v)
#endif //

{
	if     ( a <  0x2000 ){ mdcore_ptr->z80_ram[a] = v    ; }
	if     ( a <  0x4000 ){ mdcore_ptr->z80_ram[a&0x1fff] = v ; }
	else if( a <  0x6000 ){ mdcore_ptr->fm_write( a, v ); }
	else if( a == 0x6000 )
	{
		mdcore_ptr->z80_bank68k>>=1;
		mdcore_ptr->z80_bank68k+=(v&1)<<8;
		mdcore_ptr->z80_bank68k&=0x1ff; // 9 bits and filled in the new top one
	}
	else if( a == 0x7f11 ){ SN76496Write( 0, v ); }
	else if( a >= 0x8000 )
	{ 
#ifdef _DEBUG_MEM3
		TRACE( "ZW:%08X:%08X\n", (mdcore_ptr->z80_bank68k << 15) + (a & 0x7fff), v );
#endif // _DEBUG_MEM3

		m68k_write_memory_8( (mdcore_ptr->z80_bank68k << 15) + (a & 0x7fff), v ); 
	}

#ifdef _DEBUG_MEM2
	else { TRACE("ZW:%08X:%08X\n", a, v); ASSERT(FALSE) ; }
#endif //_DEBUG_MEM2

}

#ifdef MEMORY_EXT
unsigned int  z80_port_read ( unsigned int a ){ return 0 ; }
void          z80_port_write( unsigned int a, unsigned int d ){ ; }

}
#endif //

void md_core::z80_do_clock ( int clk )
{
	clk >>= 1 ;

	if( z80_online )
	{

#if _USE_Z80==1
#elif _USE_Z80==2

		clk_z80 += Cz80_Exec( &z80, clk - clk_z80 );

#elif _USE_Z80==3
#endif 

	}
	else
	{
		//clk_z80 += clk - clk_z80 ;
		clk_z80 = clk ;
	}
}

#define Z80_SYNC()  z80_do_sync(_68K_GET_CYCLE() + clk_sys)

void md_core::z80_do_sync ( int do_clk )
{
	{
		z80_in = 1 ;
		int vline  = VBLANK_LINE    ;
		int line   = line_count     ;
		int clk    = (clk_z80 << 1) ;	
		int add    = clk_line       ;

		line_count = clk / add  ;

		//TRACE( "%d, %d, %d, %d, %d, %d\n", z80_online, line_count, vline, clk, do_clk, add );

		//for( ; clk < (do_clk - add) ; clk += add )
		for( ; clk < do_clk ; clk += add )
		{
			//if( !z80_in ){ break ; }

			fm_timer_update() ;
			if( (line_count == vline) && z80_irq ){ z80_irq = 0 ; Z80_IRQ(0); }
			z80_do_clock( clk ) ;
			line_count++ ;
		}

		//if( clk < do_clk ){ z80_do_clock( do_clk ) ; }

		line_count = line ;
		z80_in     = 0    ;
	}
}

void md_core::z80_set_online( unsigned short d )
{
	Z80_SYNC() ;

	if( d & 0x0100 )
	{
		z80_online = 0 ;
	}
	else
	{
		z80_online = 1 ;
	}
}

void md_core::z80_set_reset ( unsigned short d )
{
	Z80_SYNC() ;

	if( d == 0 )
	{
		Z80_RESET(); YM2612ResetChip(); 
	}
	else
	{
	}

	mdcore_ptr->z80_reset = d ;
}

void md_core::z80_set_bank( unsigned short d )
{
	z80_bank68k >>= 1          ;
	z80_bank68k +=  (d&1) << 8 ;
	z80_bank68k &=  0x1FF      ;
}

void md_core::z80_get_state ( Z80_STATE* z80st )
{
#if _USE_Z80==1
#elif _USE_Z80==2
	z80st->z80bc            = Cz80_Get_BC ( &z80 );
	z80st->z80de            = Cz80_Get_DE ( &z80 );
	z80st->z80hl            = Cz80_Get_HL ( &z80 );
	z80st->z80af            = Cz80_Get_AF ( &z80 );

	z80st->z80bcprime       = Cz80_Get_BC2( &z80 );
	z80st->z80deprime       = Cz80_Get_DE2( &z80 );
	z80st->z80hlprime       = Cz80_Get_HL2( &z80 );
	z80st->z80afprime       = Cz80_Get_AF2( &z80 );

	z80st->z80ix            = Cz80_Get_IX ( &z80 );
	z80st->z80iy            = Cz80_Get_IY ( &z80 );
	z80st->z80sp            = Cz80_Get_SP ( &z80 );
	z80st->z80pc            = Cz80_Get_PC ( &z80 );

	z80st->z80r             = Cz80_Get_R  ( &z80 );
	z80st->z80iff           = Cz80_Get_IFF( &z80 );
	z80st->z80interruptMode = Cz80_Get_IM ( &z80 );
	z80st->z80i             = Cz80_Get_I  ( &z80 );
#elif _USE_Z80==3
#endif 

}

void md_core::z80_set_state ( Z80_STATE* z80st )
{
#if _USE_Z80==1
#elif _USE_Z80==2
	Cz80_Set_BC ( &z80, z80st->z80bc            );
	Cz80_Set_DE ( &z80, z80st->z80de            );
	Cz80_Set_HL ( &z80, z80st->z80hl            );
	Cz80_Set_AF ( &z80, z80st->z80af            );

	Cz80_Set_BC2( &z80, z80st->z80bcprime       );
	Cz80_Set_DE2( &z80, z80st->z80deprime       );
	Cz80_Set_HL2( &z80, z80st->z80hlprime       );
	Cz80_Set_AF2( &z80, z80st->z80afprime       );

	Cz80_Set_IX ( &z80, z80st->z80ix            );
	Cz80_Set_IY ( &z80, z80st->z80iy            );
	Cz80_Set_SP ( &z80, z80st->z80sp            );
	Cz80_Set_PC ( &z80, z80st->z80pc            );

	Cz80_Set_R  ( &z80, z80st->z80r             );
	Cz80_Set_IFF( &z80, z80st->z80iff           );
	Cz80_Set_IM ( &z80, z80st->z80interruptMode );
	Cz80_Set_I  ( &z80, z80st->z80i             );
#elif _USE_Z80==3
#endif 
}

void md_core::z80_init()
{
#if _USE_Z80==1
#elif _USE_Z80==2
	Cz80_Init( &z80 ) ;

	Cz80_Set_Fetch       ( &z80, 0x0000,0x1FFF, (u32)((void *)z80_ram  ));
//	Cz80_Set_Fetch       ( &z80, 0x00FF0000 ,0x00FFFFFF, (u32)((void *)core_ram ));
	Cz80_Set_ReadB       ( &z80, z80_read       ) ;
	Cz80_Set_WriteB      ( &z80, z80_write      ) ;
	Cz80_Set_INPort      ( &z80, z80_port_read  ) ;
	Cz80_Set_OUTPort     ( &z80, z80_port_write ) ;
//	Cz80_Set_IRQ_Callback( &z80,cpu_z80_irq_callback);
	Cz80_Reset           ( &z80 );
	//Cz80_Exec            ( &z80, 100000 );
#elif _USE_Z80==3
#endif 

	clk_z80 = 0 ;
}

//////////////////////////////////////////////////////
// for ROM
//////////////////////////////////////////////////////
extern "C" int load_rom_into(char *name,unsigned char *into, int max_size);

#ifdef _PSP10
#include "../psp/psp_std.h"
#endif //

void byteswap_memory( unsigned char *start, int len )
{ 
	int i; unsigned char tmp;

	for ( i=0; i < len ; i+=2 )
	{ 
		tmp        = start[i+0] ; 
		start[i+0] = start[i+1] ; 
		start[i+1] = tmp        ; 
	}
}

int  md_core::rom_load ( char* name )
{
	unsigned char *temp = NULL ;
	int           len   = 0    ;

	temp = rom_data ;
	len  = load_rom_into( name, temp, MAX_ROM_SIZE ) ;
	if( len <= 0 ){ return len ; }

	// Plug in the cartridge specified by the uchar *
	// NB - The megadrive will free() it if unplug() is called, or it exits
	// So it must be a single piece of malloced data
	byteswap_memory( temp, len ) ; // for starscream
	rom_len = len  ;

	unsigned char* rom = temp ;
	// Get saveram start, length (remember byteswapping)
	// First check magic, if there is saveram
	if(rom[0x1b1] == 'R' && rom[0x1b0] == 'A')
	{
		save_start = rom[0x1b5] << 24 | rom[0x1b4] << 16 | rom[0x1b7] << 8 | rom[0x1b6] ;
		save_len   = rom[0x1b9] << 24 | rom[0x1b8] << 16 | rom[0x1bb] << 8 | rom[0x1ba] ;

		if( save_start >= save_len )
		{
			save_len = save_start + (MAX_SAVE_RAM/2) ;
		}

		if( (save_start < save_len) && (save_start < MAX_ROM_SIZE) && (save_len < MAX_ROM_SIZE) ) 
		{
			if(save_start & 1) --save_start;
			if(!(save_len & 1)) ++save_len;

			save_len -= (save_start - 1);

			memset( save_ram, 0, save_len ) ;
			//memcpy( save_ram, &rom[save_start], save_len ) ;

			// If save RAM does not overlap main ROM, set it active by default since
			// a few games can't manage to properly switch it on/off.
			if(save_start >= rom_len)
			{ 
				save_active = 1; 
				memset( &rom[save_start], 0, save_len ) ;
			}
			else
			{
				save_active = 0 ;
			}
		}
		else 
		{
			save_start = save_len = 0;
			//save_ram = NULL;
		}
	}
	else
	{
		save_start = save_len = 0;
		//save_ram = NULL;
	}

	reset() ;

	return 0 ;
}

void md_core::rom_fix_checksum ()
{
	unsigned short checksum = 0 ;
	int i, len ;

	len = rom_len ;

	for( i=512 ; i <= (len-2) ; i+=2 )
	{
		checksum += ( rom_data[i+1] << 8 ) ;
		checksum +=   rom_data[i+0] ;
	}
	
	if( rom_len >= 0x190 )
	{ 
		rom_data[0x18f] = checksum >> 8   ;
		rom_data[0x18e] = checksum & 0xFF ; 
	}
}

int md_core::rom_get_checksum ()
{
	return (int)(rom_data[0x18f] << 8) | (int)rom_data[0x18e] ;
}

int md_core::rom_load_sram( const char* path )
{
	if( save_len > 0 )
	{
		int len ;

#ifdef _SCE_IO

		int fid = sceIoOpen( path, PSP_O_RDONLY, 0644 );

		if( fid >= 0 )
		{
			if( save_active )
			{
				len = sceIoRead( fid, &rom_data[save_start], save_len ) ;
			}
			else
			{
				len = sceIoRead( fid, save_ram             , save_len ) ;
			}

			//if( len != save_len ){ ; }

			sceIoClose( fid ) ;

			save_update = 0 ;

			return len ;
		}

#else  // _SCE_IO

		FILE* fid = fopen( path, "rb" );

		if( fid )
		{
			if( save_active )
			{
				len = fread( &rom_data[save_start], 1, save_len, fid ) ;
			}
			else
			{
				len = fread( save_ram             , 1, save_len, fid ) ;
			}

			//if( len != save_len ){ ; }

			fclose( fid ) ;

			save_update = 0 ;

			return len ;
		}

#endif // _SCE_IO

	}

	return 0 ;
}

int md_core::rom_save_sram( const char* path )
{
	if( (save_len > 0) && save_update )
	{

#ifdef _SCE_IO

		int fid = sceIoOpen( path, PSP_O_WRONLY|PSP_O_TRUNC|PSP_O_CREAT, 0644 );

		if( fid >= 0 )
		{
			if( save_active )
			{
				sceIoWrite( fid, &rom_data[save_start], save_len ) ;
			}
			else
			{
				sceIoWrite( fid, save_ram             , save_len ) ;
			}

			//if( len != save_len ){ ; }

			sceIoClose( fid ) ;

			save_update = 0 ;

			return save_len ;
		}

#else  // _SCE_IO

		FILE* fid = fopen( path, "wb" );

		if( fid )
		{
			if( save_active )
			{
				fwrite( &rom_data[save_start], 1, save_len, fid ) ;
			}
			else
			{
				fwrite( save_ram             , 1, save_len, fid ) ;
			}

			//if( len != save_len ){ ; }

			fclose( fid ) ;

			save_update = 0 ;

			return save_len ;
		}

#endif // _SCE_IO

	}

	return 0 ;
}
unsigned char md_core::rom_get_country_ver()
{
	// autodetect country
	unsigned char headcode = 0x80, avail = 0 ;
	int i;

	for (i=0;i<3;i++)
	{
		int ch = read_byte( 0x1F0 + i );
		if      (ch=='U'){ avail |= 0x01; }
		else if (ch=='E'){ avail |= 0x02; }
		else if (ch=='J'){ avail |= 0x04; }
		else if( ch=='A'){ avail |= 0x10; }
		else if( ch=='B'){ avail |= 0x20; }
	}

	if      (avail & 0x10){ headcode=0x80; vdp_pal = 1 ; }
	else if (avail & 0x20){ headcode=0x00; vdp_pal = 1 ; }
	else 
	{
	  switch( country_ver )
	  {
	  default :
			   if (avail & 0x04){ headcode=0x00; vdp_pal = 0 ; }
		  else if (avail & 0x01){ headcode=0x80; vdp_pal = 0 ; }
		  else if (avail & 0x02){ headcode=0x80; vdp_pal = 1 ; }
		  else                  { headcode=0x00; vdp_pal = 0 ; }
		  break ;

	  case 1:
			   if (avail & 0x01){ headcode=0x80; vdp_pal = 0 ; }
		  else if (avail & 0x02){ headcode=0x80; vdp_pal = 1 ; }
		  else if (avail & 0x04){ headcode=0x00; vdp_pal = 0 ; }
		  else                  { headcode=0x80; vdp_pal = 0 ; }
		  break ;

	  case 2:
			   if (avail & 0x02){ headcode=0x80; vdp_pal = 1 ; }
		  else if (avail & 0x01){ headcode=0x80; vdp_pal = 0 ; }
		  else if (avail & 0x04){ headcode=0x00; vdp_pal = 0 ; }
		  else                  { headcode=0x80; vdp_pal = 1 ; }
		  break ;
	  }
	}

	return headcode | (vdp_pal? 0x40 : 0) ;
}

void md_core::write_save_flag( unsigned char d )
{
	unsigned int n ;

//	TRACE("%d, %d, %d, %d\n", d, save_active, save_prot, save_active );

	if( save_len > 0 )
	{
		if( (save_active && ((d & 1)==0)) || (!save_active && ((d & 1))) )
		{
			for( n=0; n < save_len ; n++ )
			{
				unsigned char buf        = rom_data[save_start + n] ;
				rom_data[save_start + n] = save_ram[n]              ;
				save_ram[n]              = buf                      ;
			}
		}

		save_active = d & 1;
		save_prot   = d & 2;
	}
}

//////////////////////////////////////////////////////
