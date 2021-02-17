//////////////////////////////////////////////////////
/// md.h
//////////////////////////////////////////////////////
#ifndef __MD_H__
#define __MD_H__
//////////////////////////////////////////////////////
#include "def_68k.h"
//#include "def_z80.h"
extern "C" 
{
#include "fm.h"
#include "sn76496.h"
}
//////////////////////////////////////////////////////
#define _USE_BANK_CANGE
#define _USE_REVERS_ENDIAN
#define _USE_DRAW_BPP
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
//#include "def_z80.h"
//*
//////////////////////////////////////////////////////
#include "cz80/cz80.h"
#define Z80_CONTEXT        cz80_struc
#define Z80_RESET()        Cz80_Reset(&z80)
#define Z80_CLEAR_CLOCK()  Cz80_Release_Cycle(&z80);clk_z80=0;
#define Z80_IRQ(v)         Cz80_Set_IRQ(&z80,v)
//#define Z80_IRQ(v)         Cz80_Set_IRQ(&z80)
//////////////////////////////////////////////////////

//extern unsigned int cz80_read      ( unsigned int a                 ) ;
//extern void         cz80_write     ( unsigned int a, unsigned int d ) ;
//extern unsigned int cz80_port_read ( unsigned int a                 ) ;
//extern void         cz80_port_write( unsigned int a, unsigned int d ) ;

typedef struct _Z80_STATE
{
	unsigned char *z80Base         ;
	void          *z80MemRead      ;
	void          *z80MemWrite     ;
	void          *z80IoRead       ;
	void          *z80IoWrite      ;
	unsigned long z80clockticks    ;
	unsigned long z80iff           ;
	unsigned long z80interruptMode ;
	unsigned long z80halted        ;
	unsigned long z80af            ;
	unsigned long z80bc            ;
	unsigned long z80de            ;
	unsigned long z80hl            ;
	unsigned long z80afprime       ;
	unsigned long z80bcprime       ;
	unsigned long z80deprime       ;
	unsigned long z80hlprime       ;
	unsigned long z80ix            ;
	unsigned long z80iy            ;
	unsigned long z80sp            ;
	unsigned long z80pc            ;
	unsigned long z80nmiAddr       ;
	unsigned long z80intAddr       ;
	unsigned long z80rCounter      ;
	unsigned char z80i             ;
	unsigned char z80r             ;
	unsigned char z80intPending    ;

} Z80_STATE;
////////////////////////////////////////////////////*/

//////////////////////////////////////////////////////
#ifdef WIN32
#define _SCRATCHPAD(a)    &draw_cashe_t[0][0]
#else
#define _SCRATCHPAD(a)    (0x00010000+(a))
#endif
//////////////////////////////////////////////////////
#define MAX_CORE_RAM   (0x10000)
#define MAX_Z80_RAM    (0x2000)
#define MAX_SAVE_RAM   (0x20000)
#define MAX_SPRITES    (0x50)
#define MAX_VDP_REG    (0x20)
#define MAX_VDP_VRAM   (0x10000)
#define MAX_VDP_CRAM   (0x80)
#define MAX_VDP_VSRAM  (0x80)
#define MAX_PALETTE    (256*4)
#define MAX_PAL_CACHE  (320+32)
#define MAX_PAD        (2)
#define MAX_DAC_BANK   (7)
#define MAX_DAC_DATA   (0x138)
#define MAX_LINE       (256) 
#define DRAW_BPP       (2)

#ifdef _USE_BANK_CANGE
#define MAX_ROM_BANKED (0x00300000)
#define MAX_ROM_SIZE   (0x00A00000)
#else  //
#define MAX_ROM_SIZE   (0x00A00000)
#endif //

#define DIRT_ALL        (0xFF)
#define DIRT_VRAM       (0x01)
#define DIRT_CRAM       (0x02)
#define DIRT_VSRAM      (0x04)
#define DIRT_REG        (0x08)
#define DIRT_REG_DRAW   (0x10)
#define ADDR_MASK_VRAM  (0xFFFF)
#define ADDR_MASK_CRAM  (0x007F)
#define ADDR_MASK_VSRAM (0x007F)
/////////////////////////////////////////////////////
/*
static inline int get_word(unsigned char *where) 
{
	unsigned short t = *((unsigned short*)where) ;
	t = (t << 8) | (t >> 8) ;
	return t ;
}
*/

#define  get_word(a)  (( (a)[0] << 8) | (a)[1])

//////////////////////////////////////////////////////

typedef struct _MD_DATA
{
	// for VDP
	unsigned char  draw_cashe_t   [MAX_LINE][ MAX_PAL_CACHE ] ; // 352 x 256 bytes
	unsigned char  draw_palette   [ MAX_PALETTE   ] ; // 512 (256 x2) bytes

	unsigned char  vdp_reg        [ MAX_VDP_REG   ] ; //  32 bytes
	unsigned char  vdp_vram       [ MAX_VDP_VRAM  ] ; //  64 KB
	unsigned char  vdp_cram       [ MAX_VDP_CRAM  ] ; // 128 bytes
	unsigned char  vdp_vsram      [ MAX_VDP_VSRAM ] ; // 128 bytes

	unsigned char  sprite_order_0 [ MAX_SPRITES   ] ; //  80 bytes 
	unsigned char  sprite_order_1 [ MAX_SPRITES   ] ; //  80 bytes

	//unsigned short* draw_palette   ;
	unsigned char* draw_cashe      ;

	  signed short sprite_count_0 ;                   //   2 bytes
	  signed short sprite_count_1 ;                   //   2 bytes
	unsigned short line_count     ;                   //   2 bytes
	unsigned short dmy            ;                   //   2 bytes

	unsigned short vdp_cmd1       ;                   //   2 bytes
	unsigned short vdp_cmd2       ;                   //   2 bytes

	unsigned short vdp_addr       ;                   //   2 bytes
	unsigned char  vdp_mode       ;                   //   1 bytes
	unsigned char  vdp_dma        ;                   //   1 bytes

	unsigned char  vdp_pending    ;                   //   1 bytes
	unsigned char  vdp_pal        ;                   //   1 bytes
	unsigned char  vdp_status1    ; // coo4                1 bytes
	unsigned char  vdp_status2    ; // coo5                1 bytes

	unsigned char* vdp_write      ;                   //   4 bytes
	unsigned char* vdp_read       ;                   //   4 bytes
	unsigned short vdp_addr_mask  ;                   //   2 bytes
	unsigned char  dirt_flag      ;                   //   1 bytes
	unsigned char  dirt_bit       ;                   //   1 bytes
	unsigned char  vdp_render_skip ;                  //   1
	unsigned char  vdp_dmy_buff   [3] ;               //   3 bytes


	// for Core
#if _USE_68K != 1 ///
	c68k_struc     C68K           ;
#endif ///_USE_68K != 1 //////////////////////

	unsigned char  core_ram       [ MAX_CORE_RAM  ] ; //  64 KB
	unsigned char  z80_ram        [ MAX_Z80_RAM   ] ; //   8 KB

	unsigned char  cpu_emu        ;                   //   1 bytes
	unsigned char  draw_bpp       ;                   //   1 bytes
	  signed char  palette_mode   ;                   //   1 bytes 
	unsigned char  irq_req        ;

	unsigned char* draw_ptr       ;                   //   4 bytes

	int            clk_sys        ;                   //   4 bytes
	int            clk_68k        ;                   //   4 bytes
	int            clk_z80        ;                   //   4 bytes
	int            clk_line       ;                   //   4 bytes

	unsigned short draw_pitch     ;                   //   2 bytes
	unsigned char  dmy3[2]        ;                   //   2 bytes

	// for PAD
	unsigned long  pad            [ MAX_PAD ] ;       //   8 bytes
	unsigned char  pad_toggle     [ MAX_PAD ] ;       //   2 bytes
	unsigned char  pad_six_tm     [ MAX_PAD ] ;       //   2 bytes
	unsigned char  pad_six_ct     [ MAX_PAD ] ;       //   2 bytes
	unsigned char  pad_use_six    ;                   //   1 bytes
	unsigned char  dmy4           ; // for Align      //   1 bytes

	// for DAC
	  signed short dac_data       [ MAX_DAC_BANK+1 ][ MAX_DAC_DATA ] ; // 4992 (312x8x2) bytes
	  signed short dac_last       ;                                    // 2 bytes
	  signed short dac_frame      ;                                    // 2 bytes
	  signed short dac_play       ;                                    // 2 bytes
	  signed short dac_enabled    ;                                    // 2 bytes
	//unsigned char  dac_rep        ;

	// for FM
	signed short   fm_reg         [ 2 ][ 0x100 ] ;    //  1024 (256x2x2) bytes
	int            fm_timer       [ 4 ] ;             // 16 bytes
	int            fm_amax        ;
	int            fm_bmax        ;
	unsigned char  fm_sel         [ 2 ] ;             //  2 bytes
	unsigned char  fm_status      [ 2 ] ;             //  2 bytes
	unsigned char  fm_status_sel  ; 
	unsigned char  sound_flag     ;                   //  1 bytes
	unsigned char  z80_irq        ;
	unsigned char  z80_in         ;

	// for Z80
	Z80_CONTEXT    z80            ;
	unsigned short z80_bank68k    ; // 9 bits
	unsigned short z80_online     ;
	unsigned short z80_reset      ;

	// for ROM
	unsigned long  rom_len        ;
	unsigned long  save_start     ;
	unsigned long  save_end       ;
	unsigned long  save_len       ;
	unsigned char  save_active    ;
	unsigned char  save_prot      ;
	unsigned char  save_update    ;
	unsigned char  dmy6           ; // for Align
	unsigned short country_ver    ;	// for mem
	unsigned char  dmy7           ; // for Align
	unsigned char  rom_bank       ;
	unsigned char  save_ram       [ MAX_SAVE_RAM ] ;
	unsigned char  rom_data       [ MAX_ROM_SIZE ] ;

} MD_DATA ;
//////////////////////////////////////////////////////

#define MEMORY_EXT

typedef unsigned int MEM_READ (unsigned int a                 );
typedef void         MEM_WRITE(unsigned int a, unsigned int d);

class md_core : public MD_DATA
{
public :
	md_core ();
	~md_core();

public :
	// for Core
	void          init             ( void ) ;
	void          reset            ( void ) ;
	void          set_frame_info   ( unsigned char* p, unsigned short pitch ){ draw_ptr = p ; draw_pitch = pitch ; }
	void          set_bpp          ( unsigned char bpp ){ draw_bpp = bpp ; }
	void          do_frame         ( unsigned char skip ) ;
	void          do_frame_68k     ( unsigned char skip ) ;
	void          do_frame_z80     ( ) ;
	void          do_irq1          ( short b ) ;
	void          do_irq2          ( short b ) ;
	void          make_sound       ( unsigned short* p, short len ) ;
	int           change_cpu_emu   ( int to ) ;
	void          set_palette_mode ( signed char mode ){ palette_mode = mode ; dirt_flag |= DIRT_CRAM ; }
	unsigned char is_width320      ( ){ return (vdp_reg[12] & 1) ; }
	int           get_state        ( unsigned char* buff ) ;
	int           set_state        ( unsigned char* buff, int buff_len, int ver ) ;

#ifndef MEMORY_EXT
	// for MEM
	static unsigned long  read_byte  ( unsigned long a ) ;
	static unsigned long  read_word  ( unsigned long a ) ;
	static unsigned long  read_long  ( unsigned long a ) ;
	static void           write_byte ( unsigned long a, unsigned long d ) ;
	static void           write_word ( unsigned long a, unsigned long d ) ;
	static void           write_long ( unsigned long a, unsigned long d ) ;
#else  //
	MEM_READ*  read_byte  ;
	MEM_READ*  read_word  ;
	MEM_READ*  read_long  ;
	MEM_WRITE* write_byte ;
	MEM_WRITE* write_word ;
	MEM_WRITE* write_long ;
#endif //MEMORY_EXT

	// for VDP core
	unsigned short vdp_get_status ( ) ;
	unsigned char  vdp_read_byte  ( ) ;
	unsigned short vdp_read_word  ( ) ;
	unsigned long  vdp_read_long  ( ) ;
	void           vdp_write_word ( unsigned short d ) ;
	void           vdp_write_long ( unsigned long  d ) ;
	void           vdp_command    ( unsigned short d ) ;
	void           vdp_command_l  ( unsigned long  d ) ;
	void           vdp_dma_copy   ( unsigned char* src, int len ) ;

	// for VDP render
	void vdp_update_palette      ( ) ;
	void vdp_update_line         ( int line  ) ;
	void vdp_update_sprite_order ( int line  ) ;
	void vdp_draw_bg             ( ) ;
	void vdp_draw_sprites        ( int line, int front ) ;
	void vdp_draw_line_clear     ( int line  ) ;
	void vdp_draw_line_norm      ( int line  ) ;
	void vdp_draw_line_vscr      ( int line  ) ;
	void vdp_draw_pal2dest       ( int line  ) ;
	void vdp_draw_pal2dest_v     ( int v_line ) ;
	void vdp_draw_interace       ( int v_line ) ;

	// for VDP sub func
	short           vdp_get_xsize();
	short           vdp_get_ysize();
	unsigned char*  vdp_get_xscrolls( int line ) ;
	unsigned char   vdp_is_plane_b  ( int line ) ;

	// for PAD
	void          pad_set_use_six ( int use ){ pad_use_six = use ? 3 : 0 ; }
	unsigned char pad_read1       () ;
	unsigned char pad_read2       () ;
	void          pad_write1      ( unsigned char d ) ;
	void          pad_write2      ( unsigned char d ) ;

	// for DAC
	void dac_init   () ;
	void dac_clear  () ;
	void dac_enable ( unsigned char d ) ;
	void dac_make   ( unsigned short* p, short len ) ;

	// for FM
	void          fm_write         ( unsigned char a, unsigned char d ) ;
	unsigned char fm_read          ( unsigned char a ) ;
	void          fm_timer_update  () ;
	void          fm_timer_update2 () ;
	void          fm_flush_reg     () ;
	void          fm_make          ( unsigned short* p, short len ) ;

	// for Z80
	void                 z80_init      ( void ) ;
	void                 z80_do_clock  ( int clk ) ;
	void                 z80_do_sync   ( int do_clk ) ;
	void                 z80_get_state ( Z80_STATE* z80st ) ;
	void                 z80_set_state ( Z80_STATE* z80st ) ;
	void                 z80_set_reset ( unsigned short d ) ;
	void                 z80_set_online( unsigned short d ) ;
	void                 z80_set_bank  ( unsigned short d ) ;
#ifndef MEMORY_EXT
	static unsigned int  z80_read      ( unsigned int a );
	static void          z80_write     ( unsigned int a, unsigned int v );
	static unsigned int  z80_port_read ( unsigned int a ){ return 0 ; }
	static void          z80_port_write( unsigned int a, unsigned int d ){ ; }
#endif //MEMORY_EXT

	// for ROM
	int           rom_load            ( char* name ) ;
	void          rom_fix_checksum    () ;
	int           rom_get_checksum    () ;
	int           rom_load_sram       ( const char* path ) ;
	int           rom_save_sram       ( const char* path ) ;
	int           rom_is_save_sram    (){ return (save_len > 0) && (save_update) ; }
	unsigned char rom_get_country_ver () ;
	void          write_save_flag     ( unsigned char d ) ;

} ;

//////////////////////////////////////////////////////
#endif // __MD_H__
//////////////////////////////////////////////////////
