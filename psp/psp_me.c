////////////////////////////////////////////////
/// psp_me.c
////////////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
////////////////////////////////////////////////

extern void me_stub(void);
extern void me_stub_end(void);

void _sw(unsigned long val, unsigned long addr) { *(volatile unsigned long*)addr = val; }

void me_start()
{
	sceSysregMeResetEnable();
	sceSysregMeBusClockEnable();
	sceSysregMeResetDisable();
}

void me_stop()
{
	sceSysregMeResetEnable();
	sceSysregMeBusClockDisable();
	sceSysregMeResetDisable();
}

void me_start_proc(unsigned long func, unsigned long param)
{
	memcpy((void *)0xbfc00040, me_stub, (int)(me_stub_end - me_stub));
	_sw(func , 0xbfc00600);
	_sw(param, 0xbfc00604);
	sceKernelDcacheWritebackAll();
	me_start() ;
}

