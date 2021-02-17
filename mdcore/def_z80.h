/////////////////////////////////////////////////////////////
/// def_z80.h
/////////////////////////////////////////////////////////////
#ifndef _DEF_Z80_H
#define _DEF_Z80_H
/////////////////////////////////////////////////////////////

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

/////////////////////////////////////////////////////////////
#if _USE_Z80==1
/////////////////////////////////////////////////////////////
#include "mz80.h"
/////////////////////////////////////////////////////////////

#define Z80_CONTEXT        mz80context
#define Z80_RESET()        mz80reset()
#define Z80_CLEAR_CLOCK()  mz80GetElapsedTicks(1)
#define Z80_IRQ(v)         mz80int(v)

extern UINT8  mz80_read      (UINT32 a,         struct MemoryReadByte  *huh);
extern void   mz80_write     (UINT32 a,UINT8 d, struct MemoryWriteByte *huh);
extern UINT16 mz80_port_read (UINT16 a,         struct z80PortRead     *huh);
extern void   mz80_port_write(UINT16 a,UINT8 d, struct z80PortWrite    *huh);

/////////////////////////////////////////////////////////////
#elif _USE_Z80==2
/////////////////////////////////////////////////////////////
#include "cz80/cz80.h"
/////////////////////////////////////////////////////////////

#define Z80_CONTEXT        cz80_struc
#define Z80_RESET()        Cz80_Reset(&z80)
#define Z80_CLEAR_CLOCK()  Cz80_Release_Cycle(&z80);
//#define Z80_IRQ(v)         Cz80_Set_IRQ(&z80,v)
#define Z80_IRQ(v)         Cz80_Set_IRQ(&z80)

extern unsigned int cz80_read      ( unsigned int a                 ) ;
extern void         cz80_write     ( unsigned int a, unsigned int d ) ;
extern unsigned int cz80_port_read ( unsigned int a                 ) ;
extern void         cz80_port_write( unsigned int a, unsigned int d ) ;
/////////////////////////////////////////////////////////////
#elif _USE_Z80==3
/////////////////////////////////////////////////////////////
#ifdef __cplusplus
extern "C" {
#endif
/////////////////////////////////////////////////////////////
#include "pz80/pz80.h"
/////////////////////////////////////////////////////////////
extern int pz80_clocks ;

#define Z80_CONTEXT        Z80_STATE
#define Z80_RESET()        ::z80_reset((void*)0)
#define Z80_CLEAR_CLOCK()  pz80_clocks=0;
#define Z80_IRQ(v)         ::set_irq_line(0,ASSERT_LINE)

extern unsigned int pz80_read      (unsigned int a                );
extern void         pz80_write     (unsigned int a, unsigned int d);
extern unsigned int pz80_port_read (unsigned int a                );
extern void         pz80_port_write(unsigned int a, unsigned int d);
/////////////////////////////////////////////////////////////
#ifdef __cplusplus
}
#endif
/////////////////////////////////////////////////////////////
#endif // _USE_Z80
/////////////////////////////////////////////////////////////
#endif //_DEF_Z80_H
/////////////////////////////////////////////////////////////
