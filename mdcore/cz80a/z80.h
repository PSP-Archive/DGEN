/***************************************************************************

	z80.h

	Handling Z80 core.

***************************************************************************/

#ifndef Z80INTF_H
#define Z80INTF_H

#if MAME_Z80
extern int z80_ICount;
#else
#include "cz80.h"
#define z80_ICount  CZ80.ICount
#endif

void z80_init(void);
void z80_reset(int param);
void z80_exit(void);
int  z80_execute(int cycles);
void z80_set_irq_line(int irqline, int state);
void z80_enable(int enable);

#ifdef SAVE_STATE
void state_save_z80(u8 *state_buf);
void state_load_z80(u8 *state_buf);
#endif

#endif /* Z80INTF_H */
