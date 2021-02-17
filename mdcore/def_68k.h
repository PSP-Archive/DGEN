/////////////////////////////////////////////////////////////
/// def_68k.h
/////////////////////////////////////////////////////////////
#ifndef _DEF_68K_H
#define _DEF_68K_H
/////////////////////////////////////////////////////////////
// _USE_68K == 1 : Use MUSASHI
// _USE_68K == 2 : Use C68k by NeoCDPSP
// _USE_68K == 3 : Use MUSASHI and C68K
/////////////////////////////////////////////////////////////
typedef struct _M68K_STATE
{
	unsigned int dr[8] ;
	unsigned int ar[8] ;
	unsigned int pc    ;
	unsigned int sr    ;
	unsigned int ir    ;
	unsigned int t1    ;
	unsigned int t0    ;
	unsigned int sf    ;
	unsigned int mf    ;
	unsigned int im    ;
	unsigned int xf    ;
	unsigned int nf    ;
	unsigned int zf    ;
	unsigned int vf    ;
	unsigned int cf    ;
	unsigned int usp   ;
	unsigned int isp   ;
	unsigned int msp   ;

} M68K_STATE ;
////////////////////////////////////////////////////

#if _USE_68K == 3 ////
#define _68K_INIT(a,b)        if(cpu_emu==0){_68K_INIT1(a,b);}else{_68K_INIT2(a,b);} 
#define _68K_RESET()          if(cpu_emu==0){_68K_RESET1();}else{_68K_RESET2();}
#define _68K_EXEC(clk)        _68K_EXEC1(clk)
#define _68K_ADD_CYCLE(clk)   _68K_ADD_CYCLE1(clk)
#define _68K_GET_CYCLE()      ((cpu_emu==0) ? _68K_GET_CYCLE1() : _68K_GET_CYCLE2())
#define _68K_SET_IRQ(a)       _68K_SET_IRQ1(a)
#define _68K_CLEAR_IRQ(a)     _68K_CLEAR_IRQ1(a)
#define _68K_GET_STATE(a)     ((cpu_emu==0) ? _68K_GET_STATE1(a) : _68K_GET_STATE2(a))
#define _68K_SET_STATE(a)     ((cpu_emu==0) ? _68K_SET_STATE1(a) : _68K_SET_STATE2(a))
#endif // _USE_68K != 3

////////////////////////////////////////////////////
#if (_USE_68K==1) || (_USE_68K==3)
////////////////////////////////////////////////////
#ifdef __cplusplus
extern "C" {
#endif
//////////////////////////////
#include "m68k/m68k.h"
//#include "musa/m68k.h"
//////////////////////////////
extern int           m68k_clks_left ;
extern void m68ki_build_opcode_table(void) ;

////////////////////////////////////////////////////////
/* for old core
#define _68K_INIT(a,b)            
#define _68K_RESET()          m68k_pulse_reset(NULL)
#define _68K_EXEC(clk)        m68k_execute(clk)
#define _68K_SET_IRQ(a)       m68k_assert_irq(a)
#define _68K_CLEAR_IRQ(a)     m68k_clear_irq(a)

inline int m68k_get_state( M68K_STATE* ms )
{
	int n ;

	for( n=0; n < 8; n++ ){ ms->ar[n] = m68k_peek_ar(n) ; }
	for( n=0; n < 8; n++ ){ ms->dr[n] = m68k_peek_dr(n) ; }
	ms->pc  = m68k_peek_pc()       ;
	ms->sr  = m68k_peek_sr()       ;
	ms->ir  = m68k_peek_ir()       ;
	ms->t1  = m68k_peek_t1_flag()  ;
	ms->t0  = m68k_peek_t0_flag()  ;
	ms->sf  = m68k_peek_s_flag()   ;
	ms->mf  = m68k_peek_m_flag()   ;
	ms->im  = m68k_peek_int_mask() ;
	ms->xf  = m68k_peek_x_flag()   ;
	ms->nf  = m68k_peek_n_flag()   ;
	ms->zf  = m68k_peek_z_flag()   ;
	ms->vf  = m68k_peek_v_flag()   ;
	ms->cf  = m68k_peek_c_flag()   ;
	ms->usp = m68k_peek_usp()      ;
	ms->isp = m68k_peek_isp()      ;
	ms->msp = m68k_peek_msp()      ;

	return sizeof( M68K_STATE ) ;
}

inline int m68k_set_state( M68K_STATE* ms )
{
	int n ;

	//m68k_pulse_reset() ;
	for( n=0; n < 8; n++ ){ m68k_poke_ar( n, ms->ar[n] ) ; }
	for( n=0; n < 8; n++ ){ m68k_poke_dr( n, ms->dr[n] ) ; }
	m68k_poke_pc      ( ms->pc  ) ;
	m68k_poke_sr      ( ms->sr  ) ;
	m68k_poke_ir      ( ms->ir  ) ;
	m68k_poke_t1_flag ( ms->t1  ) ;
	m68k_poke_t0_flag ( ms->t0  ) ;
	m68k_poke_s_flag  ( ms->sf  ) ;
	m68k_poke_m_flag  ( ms->mf  ) ;
	m68k_poke_int_mask( ms->im  ) ;
	m68k_poke_x_flag  ( ms->xf  ) ;
	m68k_poke_n_flag  ( ms->nf  ) ;
	m68k_poke_z_flag  ( ms->zf  ) ;
	m68k_poke_v_flag  ( ms->vf  ) ;
	m68k_poke_c_flag  ( ms->cf  ) ;
	m68k_poke_usp     ( ms->usp ) ;
	m68k_poke_isp     ( ms->isp ) ;
	m68k_poke_msp     ( ms->msp ) ;

	return sizeof( M68K_STATE ) ;
}
///////////////////////////////////////////////////////////*/

//*

#if _USE_68K == 3 ////
#define _68K_INIT1(a,b)            
#define _68K_RESET1()         m68k_pulse_reset()
#define _68K_EXEC1(clk)       m68k_execute(clk)
#define _68K_ADD_CYCLE1(clk)  m68k_use_cycles(clk)
#define _68K_GET_CYCLE1()     m68k_cycles_run()
#define _68K_SET_IRQ1(a)      m68k_set_irq(a)
#define _68K_CLEAR_IRQ1(a)    m68k_set_irq(0)
#define _68K_GET_STATE1(a)    m68k_get_state(a)
#define _68K_SET_STATE1(a)    m68k_set_state(a)
#else // _USE_68K != 3
#define _68K_INIT(a,b)            
#define _68K_RESET()          m68k_pulse_reset()
#define _68K_EXEC(clk)        m68k_execute(clk)
#define _68K_ADD_CYCLE(clk)   m68k_use_cycles(clk)
#define _68K_GET_CYCLE()      m68k_cycles_run()
#define _68K_SET_IRQ(a)       m68k_set_irq(a)
#define _68K_CLEAR_IRQ(a)     m68k_set_irq(0)
#define _68K_GET_STATE(a)     m68k_get_state(a)
#define _68K_SET_STATE(a)     m68k_set_state(a)
#endif // _USE_68K != 3

inline int m68k_get_state( M68K_STATE* ms )
{
	int n ;

	for( n=0; n < 8; n++ ){ ms->ar[n] = m68k_get_reg( NULL, (m68k_register_t)(M68K_REG_A0 + n) ) ; }
	for( n=0; n < 8; n++ ){ ms->dr[n] = m68k_get_reg( NULL, (m68k_register_t)(M68K_REG_D0 + n) ) ; }
	ms->pc  = m68k_get_reg( NULL, M68K_REG_PC  ) ;
	ms->sr  = m68k_get_reg( NULL, M68K_REG_SR  ) ;
	ms->ir  = m68k_get_reg( NULL, M68K_REG_IR  ) ;
	ms->t1  = m68k_get_reg( NULL, M68K_REG_T0  ) ;
	ms->t0  = m68k_get_reg( NULL, M68K_REG_T1  ) ;
	ms->sf  = m68k_get_reg( NULL, M68K_REG_SF  ) >> 2 ;
	ms->mf  = m68k_get_reg( NULL, M68K_REG_MF  ) ;
	ms->im  = m68k_get_reg( NULL, M68K_REG_IM  ) >> 8 ;
	ms->xf  = m68k_get_reg( NULL, M68K_REG_XF  ) >> 8 ;
	ms->nf  = m68k_get_reg( NULL, M68K_REG_NF  ) ;
	ms->zf  = m68k_get_reg( NULL, M68K_REG_ZF  ) ? 0 : 1 ;
	ms->vf  = m68k_get_reg( NULL, M68K_REG_VF  ) ;
	ms->cf  = m68k_get_reg( NULL, M68K_REG_CF  ) ;
	ms->usp = m68k_get_reg( NULL, M68K_REG_USP ) ;
	ms->isp = m68k_get_reg( NULL, M68K_REG_ISP ) ;
	ms->msp = m68k_get_reg( NULL, M68K_REG_MSP ) ;

	return sizeof( M68K_STATE ) ;
}

inline int m68k_set_state( M68K_STATE* ms )
{
	int n ;

	m68k_pulse_reset() ;
	for( n=0; n < 8; n++ ){ m68k_set_reg( (m68k_register_t)(M68K_REG_A0 + n), ms->ar[n] ) ; }
	for( n=0; n < 8; n++ ){ m68k_set_reg( (m68k_register_t)(M68K_REG_D0 + n), ms->dr[n] ) ; }
	m68k_set_reg( M68K_REG_PC , ms->pc  ) ;
	m68k_set_reg( M68K_REG_SR , ms->sr  ) ;
	m68k_set_reg( M68K_REG_IR , ms->ir  ) ;
	m68k_set_reg( M68K_REG_T0 , ms->t1  ) ;
	m68k_set_reg( M68K_REG_T1 , ms->t0  ) ;
	m68k_set_reg( M68K_REG_SF , ms->sf << 2 ) ;
	m68k_set_reg( M68K_REG_MF , ms->mf  ) ;
	m68k_set_reg( M68K_REG_IM , ms->im << 8 ) ;
	m68k_set_reg( M68K_REG_XF , ms->xf << 8 ) ;
	m68k_set_reg( M68K_REG_NF , ms->nf  ) ;
	m68k_set_reg( M68K_REG_ZF , ms->zf ? 0 : 1 ) ;
	m68k_set_reg( M68K_REG_VF , ms->vf  ) ;
	m68k_set_reg( M68K_REG_CF , ms->cf  ) ;
	m68k_set_reg( M68K_REG_USP, ms->usp ) ;
	m68k_set_reg( M68K_REG_ISP, ms->isp ) ;
//	m68k_set_reg( M68K_REG_MSP, ms->msp ) ;

	return sizeof( M68K_STATE ) ;
}
//*/


//////////////////////////////
#ifdef __cplusplus
}
#endif
////////////////////////////////////////////////////
#endif // _USE_68K
////////////////////////////////////////////////////
#if (_USE_68K==2) || (_USE_68K==3)
////////////////////////////////////////////////////
#ifdef __cplusplus
extern "C" {
#endif
//////////////////////////////
//////////////////////////////
#include "c68k/types.h"
#include "c68k/c68k.h"
//////////////////////////////

#define _PTR_C68K  &C68K //(c68k_struc*)(0x11000)

#if _USE_68K == 3 ////
#define _68K_INIT2(a,b)       _C68k_Init(_PTR_C68K,a,b)
#define _68K_RESET2()         C68k_Reset(_PTR_C68K)
#define _68K_CLEAR_CLKS2()    C68k_Release_Cycle(_PTR_C68K)
#define _68K_EXEC2(clk)       C68k_Exec(_PTR_C68K,clk)
#define _68K_ADD_CYCLE2(clk)  C68k_Use_Cycle(_PTR_C68K,clk)
#define _68K_GET_CYCLE2()     C68k_Get_CycleDone(_PTR_C68K)
#define _68K_SET_IRQ2(a)      C68k_Set_IRQ(_PTR_C68K,a)
#define _68K_CLEAR_IRQ2(a)    C68k_Set_IRQ(_PTR_C68K,0)
#define _68K_GET_STATE2(a)    c68k_get_state(_PTR_C68K,a)
#define _68K_SET_STATE2(a)    c68k_set_state(_PTR_C68K,a)
#else //_USE_68K != 3 //
#define _68K_INIT(a,b)        _C68k_Init(_PTR_C68K,a,b)
#define _68K_RESET()          C68k_Reset(_PTR_C68K)
#define _68K_CLEAR_CLKS()     C68k_Release_Cycle(_PTR_C68K)
#define _68K_EXEC(clk)        C68k_Exec(_PTR_C68K,clk)
#define _68K_ADD_CYCLE(clk)   C68k_Use_Cycle(_PTR_C68K,clk)
#define _68K_GET_CYCLE()      C68k_Get_CycleDone(_PTR_C68K)
#define _68K_SET_IRQ(a)       C68k_Set_IRQ(_PTR_C68K,a)
#define _68K_CLEAR_IRQ(a)     C68k_Set_IRQ(_PTR_C68K,0)
#define _68K_GET_STATE(a)     c68k_get_state(_PTR_C68K,a)
#define _68K_SET_STATE(a)     c68k_set_state(_PTR_C68K,a)
#endif //_USE_68K != 3 //

extern unsigned int m68k_read_memory_8  (unsigned int address) ;
extern unsigned int m68k_read_memory_16 (unsigned int address) ;
extern unsigned int m68k_read_memory_32 (unsigned int address) ;
extern void m68k_write_memory_8 (unsigned int address, unsigned int value) ;
extern void m68k_write_memory_16(unsigned int address, unsigned int value) ;
extern void m68k_write_memory_32(unsigned int address, unsigned int value) ;
extern int vdp_int_ack_callback(int int_level) ;

inline void _C68k_Init( c68k_struc* cpu, unsigned char* rom, unsigned char* ram )
{
	unsigned long n ;
	C68k_InitTable (0);
	C68k_Init      ( cpu, vdp_int_ack_callback                                 ) ;
    C68k_Set_ReadB ( cpu, (C68K_READ* )m68k_read_memory_8   ) ;
    C68k_Set_ReadW ( cpu, (C68K_READ* )m68k_read_memory_16  ) ;
    C68k_Set_WriteB( cpu, (C68K_WRITE*)m68k_write_memory_8  ) ;
    C68k_Set_WriteW( cpu, (C68K_WRITE*)m68k_write_memory_16 ) ;
#ifdef  C68K_FUNC_32
    C68k_Set_ReadL ( cpu, (C68K_READ* )m68k_read_memory_32  ) ;
    C68k_Set_WriteL( cpu, (C68K_WRITE*)m68k_write_memory_32 ) ;
#endif // C68K_FUNC_32
	if( rom && ram )
	{
		C68k_Set_Fetch ( cpu, 0x00000000, 0x009FFFFF, (u32)(rom)  ) ;

		//*
		for( n=0xE0; n <= 0xFF ; n++ )
		{
			C68k_Set_Fetch ( cpu, (n << 16), (n << 16) | 0x00FFFF, (u32)(ram)  ) ;
		}
		//*/
			
		//n = 0xFF ;
		//C68k_Set_Fetch ( _PTR_C68K, (n << 16), (n << 16) | 0x00FFFF, (u32)(ram)  ) ;
	}
}

inline int c68k_get_state( c68k_struc* cpu, M68K_STATE* ms )
{
	int n ;

	for( n=0; n < 8; n++ ){ ms->dr[n] = C68k_Get_DReg( cpu, n ) ; }
	for( n=0; n < 8; n++ ){ ms->ar[n] = C68k_Get_AReg( cpu, n ) ; }
	ms->pc  = C68k_Get_PC( cpu ) ;
	ms->sr  = C68k_Get_SR( cpu ) ;
	//*
	ms->ir  = 0              ;
	ms->t1  = 0              ;
	ms->t0  = 0              ;
	ms->sf  = cpu->flag_S    ? 1 : 0 ;
	ms->mf  = 1              ;
	ms->im  = cpu->flag_I    ;
	ms->xf  = cpu->flag_X    ; // ? 1 : 0 ;
	ms->nf  = cpu->flag_N    ; // ? 1 : 0 ;
	ms->zf  = cpu->flag_notZ ? 0 : 1 ;
	ms->vf  = cpu->flag_V    ; // ? 1 : 0 ;
	ms->cf  = cpu->flag_C    ; // ? 1 : 0 ;
	//*/
	ms->usp = C68k_Get_USP( cpu ) ;
	//ms->isp =  ;
	ms->msp = C68k_Get_MSP( cpu ) ;

	return sizeof( M68K_STATE ) ;
}

inline int c68k_set_state( c68k_struc* cpu, M68K_STATE* ms )
{
	int n ;

	C68k_Reset(cpu) ;
	for( n=0; n < 8; n++ ){ C68k_Set_DReg( cpu, n, ms->dr[n] ) ; }
	for( n=0; n < 8; n++ ){ C68k_Set_AReg( cpu, n, ms->ar[n] ) ; }
	C68k_Set_PC( cpu, ms->pc ) ;
	C68k_Set_SR( cpu, ms->sr ) ;
	//*
	//0              = ms->ir ;
	//0              = ms->t1 ;
	//0              = ms->t0 ;
	cpu->flag_S    = ms->sf ? C68K_SR_S : 0 ;
	//0              = ms->mf ;
	cpu->flag_I    = ms->im ;
	cpu->flag_X    = ms->xf ; // ? C68K_SR_X : 0 ;
	cpu->flag_N    = ms->nf ; // ? C68K_SR_N : 0 ;
	cpu->flag_notZ = ms->zf ?         0 : 1 ;
	cpu->flag_V    = ms->vf ; // ? C68K_SR_V : 0 ;
	cpu->flag_C    = ms->cf ; // ? C68K_SR_C : 0 ;
	//*/
	C68k_Set_USP( cpu, ms->usp ) ;
	//ms->isp =  ;
	//C68k_Set_MSP( _PTR_C68K, ms->msp ) ;

	return sizeof( M68K_STATE ) ;
}


//////////////////////////////
#ifdef __cplusplus
}
#endif
////////////////////////////////////////////////////
#else // _USE_68K
////////////////////////////////////////////////////
// Old codes here.

#ifdef COMPILE_WITH_STAR
#ifndef __STARCPU_H__
#include "starcpu.h"
#endif
#endif

#ifdef COMPILE_WITH_M68KEM
#ifndef M68000__HEADER
extern "C" {
#include "m68000.h"
}
#endif
#endif
	
#ifdef COMPILE_WITH_MUSA
#ifndef M68K__HEADER
extern "C"
{
#include "m68k.h"
extern int           m68k_clks_left ;
}
#endif
#endif

////////////////////////////////////////////////////
#endif // _USE_68K
////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
#endif //_DEF_68K_H
/////////////////////////////////////////////////////////////
