/********************************************************************************/
/*                                                                              */
/* CZ80 (Z80 cpu emulator) version 0.9                                          */
/* Compiled with Dev-C++                                                        */
/* Copyright 2004-2005 Stéphane Dallongeville                                   */
/*                                                                              */
/********************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cz80.h"

#define CF	0x01
#define NF	0x02
#define PF	0x04
#define VF	PF
#define XF	0x08
#define HF	0x10
#define YF	0x20
#define ZF	0x40
#define SF	0x80

// include macro file
//////////////////////

#include "cz80_inc.c"

// shared global variable
//////////////////////////

cz80_struc CZ80;

static u8 *pzR8[8];
static union16 *pzR16[4];

static u8 SZ[256];            // zero and sign flags
static u8 SZP[256];           // zero, sign and parity flags
static u8 SZ_BIT[256];        // zero, sign and parity/overflow (=zero) flags for BIT opcode
static u8 SZHV_inc[256];      // zero, sign, half carry and overflow flags INC R8
static u8 SZHV_dec[256];      // zero, sign, half carry and overflow flags DEC R8
static u32 PC, EA;

// prototype
/////////////

u32 Cz80_Read_Dummy(u32 adr);
void Cz80_Write_Dummy(u32 adr, u32 data);

int Cz80_Interrupt_Ack_Dummy(int param);

// core main functions
///////////////////////

void Cz80_Init(cz80_struc *cpu)
{
    u32 i, j, p;

    memset(cpu, 0, sizeof(cz80_struc));

    // flags tables initialisation
    for (i = 0; i < 256; i++)
    {
		p = 0;
		if (i & 0x01) p++;
		if (i & 0x02) p++;
		if (i & 0x04) p++;
		if (i & 0x08) p++;
		if (i & 0x10) p++;
		if (i & 0x20) p++;
		if (i & 0x40) p++;
		if (i & 0x80) p++;
		SZ[i] = i ? i & SF : ZF;
		SZ[i] |= (i & (YF | XF));		/* undocumented flag bits 5+3 */
		SZ_BIT[i] = i ? i & SF : ZF | PF;
		SZ_BIT[i] |= (i & (YF | XF));	/* undocumented flag bits 5+3 */
		SZP[i] = SZ[i] | ((p & 1) ? 0 : PF);
		SZHV_inc[i] = SZ[i];
		if (i == 0x80) SZHV_inc[i] |= VF;
		if ((i & 0x0f) == 0x00) SZHV_inc[i] |= HF;
		SZHV_dec[i] = SZ[i] | NF;
		if (i == 0x7f) SZHV_dec[i] |= VF;
		if ((i & 0x0f) == 0x0f) SZHV_dec[i] |= HF;
    }

    Cz80_Set_Fetch(cpu, 0x0000, 0xffff, (u32)NULL);

    Cz80_Set_ReadB(cpu, Cz80_Read_Dummy);
    Cz80_Set_WriteB(cpu, Cz80_Write_Dummy);

    cpu->Interrupt_Callback = Cz80_Interrupt_Ack_Dummy;
    cpu->RETI_Callback = NULL;

    zIX = zIY = 0xffff;
    zF = ZF;

	pzR8[0] = &zB;
	pzR8[1] = &zC;
	pzR8[2] = &zD;
	pzR8[3] = &zE;
	pzR8[4] = &zH;
	pzR8[5] = &zL;
	pzR8[6] = &zF;	// ˆ—‚Ì“s‡ãAA‚Æ“ü‚ê‘Ö‚¦
	pzR8[7] = &zA;	// ˆ—‚Ì“s‡ãAF‚Æ“ü‚ê‘Ö‚¦

	pzR16[0] = pzBC;
	pzR16[1] = pzDE;
	pzR16[2] = pzHL;
	pzR16[3] = pzAF;
}

void Cz80_Reset(cz80_struc *cpu)
{
    memset(cpu, 0, (u32)(&(cpu->ExtraCycles)) - (u32)(&(cpu->BC)));

    Cz80_Set_PC(cpu, 0);
}

/////////////////////////////////

#include "cz80exec.c"

/////////////////////////////////

void Cz80_Set_IRQ(cz80_struc *cpu)
{
    cpu->IRQLine = 1;
    CHECK_INT
}

void Cz80_Set_NMI(cz80_struc *cpu)
{
    zIFF1 = 0;
    cpu->ExtraCycles += 11;
    zR++;
    cpu->Halted = 0;

    PUSH_16(cpu->PC - cpu->BasePC)
    Cz80_Set_PC(cpu, 0x66);
}

void Cz80_Clear_IRQ(cz80_struc *cpu)
{
    cpu->IRQLine = 0;
}

/////////////////////////////////

int Cz80_Get_CycleToDo(cz80_struc *cpu)
{
    return cpu->InitialICount;
}

int Cz80_Get_CycleRemaining(cz80_struc *cpu)
{
    return cpu->InitialICount - (cpu->ICount + cpu->ExtraCycles);
}

int Cz80_Get_CycleDone(cz80_struc *cpu)
{
    return cpu->ICount + cpu->ExtraCycles;
}

void Cz80_Release_Cycle(cz80_struc *cpu)
{
    cpu->InitialICount = 0;
    cpu->ICount = 0;
    cpu->ExtraCycles = 0;
}

void Cz80_Add_Cycle(cz80_struc *cpu, int cycle)
{
    cpu->InitialICount += cycle;
}

// Read / Write dummy functions
////////////////////////////////

u32 Cz80_Read_Dummy(u32 adr)
{
    return 0;
}

void Cz80_Write_Dummy(u32 adr, u32 data)
{

}

int Cz80_Interrupt_Ack_Dummy(int param)
{
    // return vector
    return 0xff;
}


// Read / Write core functions
///////////////////////////////

u32 Cz80_Read_Byte(cz80_struc *cpu, u32 adr)
{
    return cpu->Read_Byte(adr);
}

u32 Cz80_Read_Word(cz80_struc *cpu, u32 adr)
{
#if CZ80_USE_WORD_HANDLER
    return cpu->Read_Word(adr);
#elif CZ80_LITTLE_ENDIAN
    return cpu->Read_Byte(adr) | (cpu->Read_Byte(adr + 1) << 8);
#else
    return (cpu->Read_Byte(adr) << 8) | cpu->Read_Byte(adr + 1);
#endif
}

void Cz80_Write_Byte(cz80_struc *cpu, u32 adr, u32 data)
{
    cpu->Write_Byte(adr, data);
}

void Cz80_Write_Word(cz80_struc *cpu, u32 adr, u32 data)
{
#if CZ80_USE_WORD_HANDLER
    cpu->Write_Word(adr, data);
#elif CZ80_LITTLE_ENDIAN
    cpu->Write_Byte(adr, data & 0xff);
    cpu->Write_Byte(adr + 1, data >> 8);
#else
    cpu->Write_Byte(adr, data >> 8);
    cpu->Write_Byte(adr + 1, data & 0xff);
#endif
}

// setting core functions
//////////////////////////

void Cz80_Set_Fetch(cz80_struc *cpu, u32 low_adr, u32 high_adr, u32 fetch_adr)
{
    u32 i, j;

    i = low_adr >> CZ80_FETCH_SFT;
    j = high_adr >> CZ80_FETCH_SFT;
    fetch_adr -= i << CZ80_FETCH_SFT;
    while (i <= j) cpu->Fetch[i++] = (u8 *)fetch_adr;
}

void Cz80_Set_ReadB(cz80_struc *cpu, u32 (*func)(u32 adr))
{
    cpu->Read_Byte = func;
}

void Cz80_Set_WriteB(cz80_struc *cpu, void (*func)(u32 adr, u32 data))
{
    cpu->Write_Byte = func;
}

#if CZ80_USE_WORD_HANDLER
void Cz80_Set_ReadW(cz80_struc *cpu, u32 (*func)(u32 adr))
{
    cpu->Read_Word = func;
}

void Cz80_Set_WriteW(cz80_struc *cpu, void (*func)(u32 adr, u32 data))
{
    cpu->Write_Word = func;
}
#endif

void Cz80_Set_INPort(cz80_struc *cpu, u32 (*func)(u32 adr))
{
    cpu->IN_Port = func;
}

void Cz80_Set_OUTPort(cz80_struc *cpu, void (*func)(u32 adr, u32 data))
{
    cpu->OUT_Port = func;
}

void Cz80_Set_IRQ_Callback(cz80_struc *cpu, int (*callback)(int line))
{
    cpu->Interrupt_Callback = callback;
}

void Cz80_Set_RETI_Callback(cz80_struc *cpu, void (*callback)(void))
{
    cpu->RETI_Callback = callback;
}

// externals main functions
////////////////////////////

u32 Cz80_Get_BC(cz80_struc *cpu)
{
    return zBC;
}

u32 Cz80_Get_DE(cz80_struc *cpu)
{
    return zDE;
}

u32 Cz80_Get_HL(cz80_struc *cpu)
{
    return zHL;
}

u32 Cz80_Get_AF(cz80_struc *cpu)
{
    return zAF;
}

u32 Cz80_Get_BC2(cz80_struc *cpu)
{
    return zBC2;
}

u32 Cz80_Get_DE2(cz80_struc *cpu)
{
    return zDE2;
}

u32 Cz80_Get_HL2(cz80_struc *cpu)
{
    return zHL2;
}

u32 Cz80_Get_AF2(cz80_struc *cpu)
{
    return zAF2;
}

u32 Cz80_Get_IX(cz80_struc *cpu)
{
    return zIX;
}

u32 Cz80_Get_IY(cz80_struc *cpu)
{
    return zIY;
}

u32 Cz80_Get_SP(cz80_struc *cpu)
{
    return zSP;
}

u32 Cz80_Get_PC(cz80_struc *cpu)
{
    return cpu->PC - cpu->BasePC;
}

u32 Cz80_Get_R(cz80_struc *cpu)
{
    return zR;
}

u32 Cz80_Get_IFF(cz80_struc *cpu)
{
    u32 value = 0;

    if (zIFF1 & CZ80_IFF) value |= 1;
    if (zIFF2 & CZ80_IFF) value |= 2;
    return value;
}

u32 Cz80_Get_IM(cz80_struc *cpu)
{
    return zIM;
}

u32 Cz80_Get_I(cz80_struc *cpu)
{
    return zI;
}


void Cz80_Set_BC(cz80_struc *cpu, u32 value)
{
    zBC = value;
}

void Cz80_Set_DE(cz80_struc *cpu, u32 value)
{
    zDE = value;
}

void Cz80_Set_HL(cz80_struc *cpu, u32 value)
{
    zHL = value;
}

void Cz80_Set_AF(cz80_struc *cpu, u32 val)
{
    zAF = val;
}

void Cz80_Set_BC2(cz80_struc *cpu, u32 value)
{
    zBC2 = value;
}

void Cz80_Set_DE2(cz80_struc *cpu, u32 value)
{
    zDE2 = value;
}

void Cz80_Set_HL2(cz80_struc *cpu, u32 value)
{
    zHL2 = value;
}

void Cz80_Set_AF2(cz80_struc *cpu, u32 val)
{
    zAF2 = val;
}

void Cz80_Set_IX(cz80_struc *cpu, u32 value)
{
    zIX = value;
}

void Cz80_Set_IY(cz80_struc *cpu, u32 value)
{
    zIY = value;
}

void Cz80_Set_SP(cz80_struc *cpu, u32 value)
{
    zSP = value;
}

void Cz80_Set_PC(cz80_struc *cpu, u32 val)
{
    cpu->BasePC = (u32)cpu->Fetch[val >> CZ80_FETCH_SFT];
    cpu->PC = val + cpu->BasePC;
}


void Cz80_Set_R(cz80_struc *cpu, u32 value)
{
    zR = value & 0xff;
    zR2 = value & 0x80;
}

void Cz80_Set_IFF(cz80_struc *cpu, u32 value)
{
    zIFF = 0;
    if (value & 1) zIFF1 = CZ80_IFF;
    if (value & 2) zIFF2 = CZ80_IFF;
}

void Cz80_Set_IM(cz80_struc *cpu, u32 value)
{
    zIM = value & 3;
}

void Cz80_Set_I(cz80_struc *cpu, u32 value)
{
    zI = value & 0xff;
}
