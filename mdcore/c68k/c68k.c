/*********************************************************************************
 *
 * C68K (68000 CPU emulator) version 0.80
 * Compiled with Dev-C++
 * Copyright 2003-2004 Stephane Dallongeville
 *
 ********************************************************************************/

#include <stdio.h>
#include <string.h>

#include "types.h"
#include "c68k.h"

// shared global variable
//////////////////////////

//c68k_struc C68K;

// include macro file
//////////////////////

#include "c68kmac.inc"

// prototype
/////////////

u32 FASTCALL C68k_Read_Dummy(const u32 adr);
void FASTCALL C68k_Write_Dummy(const u32 adr, u32 data);

u32 C68k_Read_Byte(c68k_struc *cpu, u32 adr);
u32 C68k_Read_Word(c68k_struc *cpu, u32 adr);
u32 C68k_Read_Long(c68k_struc *cpu, u32 adr);
void C68k_Write_Byte(c68k_struc *cpu, u32 adr, u32 data);
void C68k_Write_Word(c68k_struc *cpu, u32 adr, u32 data);
void C68k_Write_Long(c68k_struc *cpu, u32 adr, u32 data);

s32  FASTCALL C68k_Interrupt_Ack_Dummy(s32 level);
void FASTCALL C68k_Reset_Dummy(void);

// core main functions
///////////////////////

void C68k_Init(c68k_struc *cpu, C68K_INT_CALLBACK *int_cb)
{
    u32 i;
    
    memset(cpu, 0, sizeof(c68k_struc));

    C68k_Set_ReadB(cpu, C68k_Read_Dummy);
    C68k_Set_ReadW(cpu, C68k_Read_Dummy);

    C68k_Set_WriteB(cpu, C68k_Write_Dummy);
    C68k_Set_WriteW(cpu, C68k_Write_Dummy);

    if (int_cb) cpu->Interrupt_CallBack = int_cb;
    else cpu->Interrupt_CallBack = C68k_Interrupt_Ack_Dummy;
    cpu->Reset_CallBack = C68k_Reset_Dummy;

    // used to init JumpTable
    cpu->Status |= C68K_DISABLE;
    C68k_Exec(cpu, 0);
    
    cpu->Status &= ~C68K_DISABLE;
}

s32 FASTCALL C68k_Reset(c68k_struc *cpu)
{
    memset(cpu, 0, (u32)(&(cpu->dirty1)) - (u32)(&(cpu->D[0])));
    
    cpu->flag_I = 7;
    cpu->flag_S = C68K_SR_S;

    cpu->A[7] = C68k_Read_Long(cpu, 0);
    C68k_Set_PC(cpu, C68k_Read_Long(cpu, 4));

    return cpu->Status;
}

/////////////////////////////////

void FASTCALL C68k_Set_IRQ(c68k_struc *cpu, s32 level)
{
	cpu->IRQLine = level;

	if( level )
	{
		if (cpu->Status & C68K_RUNNING)
		{
			cpu->CycleSup = cpu->CycleIO;
			cpu->CycleIO  = 0 ;
		}

		cpu->Status &= ~(C68K_HALTED | C68K_WAITING);
	}
}

/////////////////////////////////

s32 FASTCALL C68k_Get_CycleToDo(c68k_struc *cpu)
{
    if (!(cpu->Status & C68K_RUNNING)) return -1;
    
    return cpu->CycleToDo;
}

s32 FASTCALL C68k_Get_CycleRemaining(c68k_struc *cpu)
{
    if (!(cpu->Status & C68K_RUNNING)) return -1;

    return (cpu->CycleIO + cpu->CycleSup);
}

s32 FASTCALL C68k_Get_CycleDone(c68k_struc *cpu)
{
    if (!(cpu->Status & C68K_RUNNING)) return -1;

    return (cpu->CycleToDo - (cpu->CycleIO + cpu->CycleSup));
}

void FASTCALL C68k_Release_Cycle(c68k_struc *cpu)
{
    if (cpu->Status & C68K_RUNNING) cpu->CycleIO = cpu->CycleSup = 0;
}

void FASTCALL C68k_Add_Cycle(c68k_struc *cpu, s32 cycle)
{
    if (cpu->Status & C68K_RUNNING) cpu->CycleIO -= cycle;
}

// Read / Write dummy functions
////////////////////////////////

u32 FASTCALL C68k_Read_Dummy(const u32 adr)
{
    return 0;
}

void FASTCALL C68k_Write_Dummy(const u32 adr, u32 data)
{

}

s32 FASTCALL C68k_Interrupt_Ack_Dummy(s32 level)
{
    // return vector
    return (C68K_INTERRUPT_AUTOVECTOR_EX + level);
}

void FASTCALL C68k_Reset_Dummy(void)
{

}

// Read / Write core functions
///////////////////////////////

u32 C68k_Read_Byte(c68k_struc *cpu, u32 adr)
{
    return cpu->Read_Byte(adr);
}

u32 C68k_Read_Word(c68k_struc *cpu, u32 adr)
{
    return cpu->Read_Word(adr);
}

u32 C68k_Read_Long(c68k_struc *cpu, u32 adr)
{
#ifdef C68K_FUNC_32

    return cpu->Read_Long(adr);

#else //
#ifdef C68K_BIG_ENDIAN
    return (cpu->Read_Word(adr) & 0xFFFF) | (cpu->Read_Word(adr + 2) << 16);
#else
    return (cpu->Read_Word(adr) << 16) | (cpu->Read_Word(adr + 2) & 0xFFFF);
#endif
#endif //
}

void C68k_Write_Byte(c68k_struc *cpu, u32 adr, u32 data)
{
    cpu->Write_Byte(adr, data);
}

void C68k_Write_Word(c68k_struc *cpu, u32 adr, u32 data)
{
    cpu->Write_Word(adr, data);
}

void C68k_Write_Long(c68k_struc *cpu, u32 adr, u32 data)
{
#ifdef C68K_FUNC_32

    cpu->Write_Long(adr, data);

#else //
#ifdef C68K_BIG_ENDIAN
    cpu->Write_Word(adr, data & 0xFFFF);
    cpu->Write_Word(adr + 2, data >> 16);
#else
    cpu->Write_Word(adr, data >> 16);
    cpu->Write_Word(adr + 2, data & 0xFFFF);
#endif
#endif //
}

// setting core functions
//////////////////////////

void C68k_Set_Fetch(c68k_struc *cpu, u32 low_adr, u32 high_adr, u32 fetch_adr)
{
    u32 i, j;

    i = (low_adr  >> C68K_FETCH_SFT) & C68K_FETCH_MASK;
    j = (high_adr >> C68K_FETCH_SFT) & C68K_FETCH_MASK;
    fetch_adr -= i << C68K_FETCH_SFT;
    while (i <= j) cpu->Fetch[i++] = fetch_adr;
}

void C68k_Set_ReadB(c68k_struc *cpu, C68K_READ *Func)
{
    cpu->Read_Byte = Func;
}

void C68k_Set_ReadW(c68k_struc *cpu, C68K_READ *Func)
{
    cpu->Read_Word = Func;
}

void C68k_Set_WriteB(c68k_struc *cpu, C68K_WRITE *Func)
{
    cpu->Write_Byte = Func;
}

void C68k_Set_WriteW(c68k_struc *cpu, C68K_WRITE *Func)
{
    cpu->Write_Word = Func;
}

#ifdef C68K_FUNC_32

void C68k_Set_ReadL(c68k_struc *cpu, C68K_READ *Func)
{
    cpu->Read_Long = Func;
}

void C68k_Set_WriteL(c68k_struc *cpu, C68K_WRITE *Func)
{
    cpu->Write_Long = Func;
}

#endif //C68K_FUNC_32

// externals main functions
////////////////////////////

u32 C68k_Get_DReg(c68k_struc *cpu, u32 num)
{
    return cpu->D[num];
}

u32 C68k_Get_AReg(c68k_struc *cpu, u32 num)
{
    return cpu->A[num];
}

u32 C68k_Get_PC(c68k_struc *cpu)
{
    return (cpu->PC - cpu->BasePC);
}

u32 C68k_Get_SR(c68k_struc *cpu)
{
    c68k_struc *CPU = cpu;
    return GET_SR;
}

u32 C68k_Get_USP(c68k_struc *cpu)
{
    if (cpu->flag_S) return cpu->USP;
    else return cpu->A[7];
}

u32 C68k_Get_MSP(c68k_struc *cpu)
{
    if (cpu->flag_S) return cpu->A[7];
    else return cpu->USP;
}

void C68k_Set_DReg(c68k_struc *cpu, u32 num, u32 val)
{
    cpu->D[num] = val;
}

void C68k_Set_AReg(c68k_struc *cpu, u32 num, u32 val)
{
    cpu->A[num] = val;
}

void C68k_Set_PC(c68k_struc *cpu, u32 val)
{
    cpu->BasePC = cpu->Fetch[(val >> C68K_FETCH_SFT) & C68K_FETCH_MASK];
    cpu->PC = val + cpu->BasePC;
}

void C68k_Set_SR(c68k_struc *cpu, u32 val)
{
    c68k_struc *CPU = cpu;
    SET_SR(val);
}

void C68k_Set_USP(c68k_struc *cpu, u32 val)
{
    if (cpu->flag_S) cpu->USP = val;
    else cpu->A[7] = val;
}

void C68k_Set_MSP(c68k_struc *cpu, u32 val)
{
    if (cpu->flag_S) cpu->A[7] = val;
    else cpu->USP = val;
}
