/***************************************************************************

	z80.c

	Handling CZ80 core.

**************************************************************************/

#include "ncdz.h"
#include "cz80.h"


int cpu_enable;


void z80_init(void)
{
	Cz80_Init(&CZ80);
	Cz80_Set_Fetch(&CZ80, 0x0000, 0xffff, (u32)neogeo_z80_memory);
	Cz80_Set_ReadB(&CZ80, &z80_read_memory_8);
	Cz80_Set_WriteB(&CZ80, &z80_write_memory_8);
	Cz80_Set_INPort(&CZ80, &neogeo_z80_port_r);
	Cz80_Set_OUTPort(&CZ80, &neogeo_z80_port_w);
	Cz80_Reset(&CZ80);
	cpu_enable = 0;
}


void z80_reset(int param)
{
	Cz80_Reset(&CZ80);
	cpu_enable = param;
}


void z80_exit(void)
{
	/* nothing to do ? */
}


int z80_execute(int cycles)
{
	if (cpu_enable)
		return Cz80_Exec(&CZ80, cycles);
	else
		return cycles;
}


INLINE void z80_assert_irq(int irqline)
{
	if (irqline == IRQ_LINE_NMI)
		Cz80_Set_NMI(&CZ80);
	else
		Cz80_Set_IRQ(&CZ80);
}


INLINE void z80_clear_irq(int irqline)
{
	Cz80_Clear_IRQ(&CZ80);
}


void z80_set_irq_line(int irqline, int state)
{
	switch (state)
	{
	case CLEAR_LINE:
		z80_clear_irq(irqline);
		return;

	case ASSERT_LINE:
		z80_assert_irq(irqline);
		return;

	default:
		z80_assert_irq(irqline);
		return;
	}
}


void z80_enable(int enable)
{
	cpu_enable = enable;
}

#ifdef SAVE_STATE
void state_save_z80(u8 *state_buf)
{
	u8 *in = state_buf;
	u32 pc = Cz80_Get_PC(&CZ80);

	state_save_word(&CZ80.BC.W, 1);
	state_save_word(&CZ80.DE.W, 1);
	state_save_word(&CZ80.HL.W, 1);
	state_save_word(&CZ80.AF.W, 1);
	state_save_word(&CZ80.IX.W, 1);
	state_save_word(&CZ80.IY.W, 1);
	state_save_word(&CZ80.SP.W, 1);
	state_save_long(&pc, 1);
	state_save_word(&CZ80.BC2.W, 1);
	state_save_word(&CZ80.DE2.W, 1);
	state_save_word(&CZ80.HL2.W, 1);
	state_save_word(&CZ80.AF2.W, 1);
	state_save_word(&CZ80.R.W, 1);
	state_save_word(&CZ80.IFF.W, 1);
	state_save_byte(&CZ80.I, 1);
	state_save_byte(&CZ80.IM, 1);
	state_save_byte(&CZ80.IRQVector, 1);
	state_save_byte(&CZ80.IRQLine, 1);
	state_save_byte(&CZ80.NMIState, 1);
	state_save_byte(&CZ80.afterEI, 1);
	state_save_byte(&CZ80.Halted, 1);
	state_save_long(&CZ80.ICount, 1);
	state_save_long(&CZ80.InitialICount, 1);
	state_save_long(&CZ80.ExtraCycles, 1);
}

void state_load_z80(u8 *state_buf)
{
	u8 *out = state_buf;
	u32 pc;

	state_load_word(&CZ80.BC.W, 1);
	state_load_word(&CZ80.DE.W, 1);
	state_load_word(&CZ80.HL.W, 1);
	state_load_word(&CZ80.AF.W, 1);
	state_load_word(&CZ80.IX.W, 1);
	state_load_word(&CZ80.IY.W, 1);
	state_load_word(&CZ80.SP.W, 1);
	state_load_long(&pc, 1);
	state_load_word(&CZ80.BC2.W, 1);
	state_load_word(&CZ80.DE2.W, 1);
	state_load_word(&CZ80.HL2.W, 1);
	state_load_word(&CZ80.AF2.W, 1);
	state_load_word(&CZ80.R.W, 1);
	state_load_word(&CZ80.IFF.W, 1);
	state_load_byte(&CZ80.I, 1);
	state_load_byte(&CZ80.IM, 1);
	state_load_byte(&CZ80.IRQVector, 1);
	state_load_byte(&CZ80.IRQLine, 1);
	state_load_byte(&CZ80.NMIState, 1);
	state_load_byte(&CZ80.afterEI, 1);
	state_load_byte(&CZ80.Halted, 1);
	state_load_long(&CZ80.ICount, 1);
	state_load_long(&CZ80.InitialICount, 1);
	state_load_long(&CZ80.ExtraCycles, 1);

	Cz80_Set_PC(&CZ80, pc);
}
#endif
