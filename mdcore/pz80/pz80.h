/******************************************************************************/
/* "z80ex.h" */
/******************************************************************************/
#ifndef _Z80_EX
#define _Z80_EX
/******************************************************************************/
#define LSB_FIRST
#include "osd_cpu.h"
/******************************************************************************/
extern unsigned int pz80_read      (unsigned int a                );
extern void         pz80_write     (unsigned int a, unsigned int d);
extern unsigned int pz80_port_read (unsigned int a                );
extern void         pz80_port_write(unsigned int a, unsigned int d);
/******************************************************************************/
extern void z80_init(void) ;
extern void z80_reset(void *param) ;
extern int  z80_execute(int cycles);
extern void set_irq_line(int irqline, int state) ;
extern void z80_get_context (void *dst);
extern void z80_set_context (void *src);
/******************************************************************************/

#ifndef INLINE
#define INLINE
#endif //INLINE

#ifndef STATIC
#define STATIC
#endif //STATIC

#ifndef CALL_MAME_DEBUG
#define CALL_MAME_DEBUG
#endif //CALL_MAME_DEBUG

#ifndef NULL
#define NULL  (0)
#endif //

#define state_save_register_UINT16(a,b,c,d,e)
#define state_save_register_UINT8(a,b,c,d,e)
#define state_save_register_INT8(a,b,c,d,e)
#define logerror(a,b,c,d)
#define change_pc(a)
#define cpu_getactivecpu()  (0)
#define osd_die(a)

#if 1
#define program_read_byte_8(a)    1
#define program_write_byte_8(a,b) 
#define cpu_readop(a)             1
#define cpu_readop_arg(a)         1
#define io_read_byte_8(a)         1
#define io_write_byte_8(a,b)      
#else
#define program_read_byte_8(a)    pz80_read(a)
#define program_write_byte_8(a,b) pz80_write(a,b)
#define cpu_readop(a)             pz80_read(a)
#define cpu_readop_arg(a)         pz80_read(a)
#define io_read_byte_8(a)         pz80_port_read(a)
#define io_write_byte_8(a,b)      pz80_port_write(a,b)
#endif

/*************************************************************************************/
/* from "z80.h" */
/*************************************************************************************/
enum {
	Z80_PC=1, Z80_SP, Z80_AF, Z80_BC, Z80_DE, Z80_HL,
	Z80_IX, Z80_IY,	Z80_AF2, Z80_BC2, Z80_DE2, Z80_HL2,
	Z80_R, Z80_I, Z80_IM, Z80_IFF1, Z80_IFF2, Z80_HALT,
	Z80_DC0, Z80_DC1, Z80_DC2, Z80_DC3
};

enum {
	Z80_TABLE_op,
	Z80_TABLE_cb,
	Z80_TABLE_ed,
	Z80_TABLE_xy,
	Z80_TABLE_xycb,
	Z80_TABLE_ex	/* cycles counts for taken jr/jp/call and interrupt latency (rst opcodes) */
};
/*************************************************************************************/


/*************************************************************************************/
/* from "cpuintrf.h" */
/*************************************
 *
 *  Interrupt line constants
 *
 *************************************/

enum
{
	/* line states */
	CLEAR_LINE = 0,				/* clear (a fired, held or pulsed) line */
	ASSERT_LINE,				/* assert an interrupt immediately */
	HOLD_LINE,					/* hold interrupt line until acknowledged */
	PULSE_LINE,					/* pulse interrupt line for one instruction */

	/* internal flags (not for use by drivers!) */
	INTERNAL_CLEAR_LINE = 100 + CLEAR_LINE,
	INTERNAL_ASSERT_LINE = 100 + ASSERT_LINE,

	/* input lines */
	MAX_INPUT_LINES = 32+3,
	INPUT_LINE_IRQ0 = 0,
	INPUT_LINE_IRQ1 = 1,
	INPUT_LINE_IRQ2 = 2,
	INPUT_LINE_IRQ3 = 3,
	INPUT_LINE_IRQ4 = 4,
	INPUT_LINE_IRQ5 = 5,
	INPUT_LINE_IRQ6 = 6,
	INPUT_LINE_IRQ7 = 7,
	INPUT_LINE_IRQ8 = 8,
	INPUT_LINE_IRQ9 = 9,
	INPUT_LINE_NMI = MAX_INPUT_LINES - 3,

	/* special input lines that are implemented in the core */
	INPUT_LINE_RESET = MAX_INPUT_LINES - 2,
	INPUT_LINE_HALT = MAX_INPUT_LINES - 1,

	/* output lines */
	MAX_OUTPUT_LINES = 32
};

/*************************************************************************************/

/******************************************************************************/
/*  from "cpuexec.h" */
/*************************************
 *
 *  Z80 daisy chain
 *
 *************************************/

/* fix me - where should this stuff go? */

/* daisy-chain link */
typedef struct
{
	void (*reset)(int); 			/* reset callback     */
	int  (*interrupt_entry)(int);	/* entry callback     */
	void (*interrupt_reti)(int);	/* reti callback      */
	int irq_param;					/* callback paramater */
} Z80_DaisyChain;

#define Z80_MAXDAISY	4		/* maximum of daisy chan device */

#define Z80_INT_REQ 	0x01	/* interrupt request mask       */
#define Z80_INT_IEO 	0x02	/* interrupt disable mask(IEO)  */

#define Z80_VECTOR(device,state) (((device)<<8)|(state))
/******************************************************************************/

/******************************************************************************/
#endif //_Z80_EX
/******************************************************************************/
