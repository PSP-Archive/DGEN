/********************************************************************************/
/*                                                                              */
/* CZ80 include file                                                            */
/* C Z80 emulator version 0.1                                                   */
/* Copyright 2004 Stéphane Dallongeville                                        */
/*                                                                              */
/********************************************************************************/

#ifndef _CZ80_H_
#define _CZ80_H_

#ifdef __cplusplus
extern "C" {
#endif


/******************************/
/* Compiler dependant defines */
/******************************/

#ifndef u8
#define u8	unsigned char
#endif

#ifndef s8
#define s8	char
#endif

#ifndef u16
#define u16	unsigned short
#endif

#ifndef s16
#define s16	short
#endif

#ifndef u32
#define u32	unsigned int
#endif

#ifndef s32
#define s32	int
#endif


/*************************************/
/* Z80 core Structures & definitions */
/*************************************/

#define CZ80_FETCH_BITS         4   // [4-12]   default = 8

#define CZ80_FETCH_SFT          (16 - CZ80_FETCH_BITS)
#define CZ80_FETCH_BANK         (1 << CZ80_FETCH_BITS)

#define CZ80_LITTLE_ENDIAN      1
#define CZ80_USE_JUMPTABLE      1
#define CZ80_USE_WORD_HANDLER   0
#define CZ80_DEBUG              0

#define zR8(A)          (*pzR8[A])
#define zR16(A)         (pzR16[A]->W)

#define pzAF            &(cpu->AF)
#define zAF             cpu->AF.W
#define zlAF            cpu->AF.B.L
#define zhAF            cpu->AF.B.H
#define zA              zhAF
#define zF              zlAF

#define pzBC            &(cpu->BC)
#define zBC             cpu->BC.W
#define zlBC            cpu->BC.B.L
#define zhBC            cpu->BC.B.H
#define zB              zhBC
#define zC              zlBC

#define pzDE            &(cpu->DE)
#define zDE             cpu->DE.W
#define zlDE            cpu->DE.B.L
#define zhDE            cpu->DE.B.H
#define zD              zhDE
#define zE              zlDE

#define pzHL            &(cpu->HL)
#define zHL             cpu->HL.W
#define zlHL            cpu->HL.B.L
#define zhHL            cpu->HL.B.H
#define zH              zhHL
#define zL              zlHL

#define zAF2            cpu->AF2.W
#define zlAF2           cpu->AF2.B.L
#define zhAF2           cpu->AF2.B.H
#define zA2             zhAF2
#define zF2             zlAF2

#define zBC2            cpu->BC2.W
#define zDE2            cpu->DE2.W
#define zHL2            cpu->HL2.W

#define pzIX            &(cpu->IX)
#define zIX             cpu->IX.W
#define zlIX            cpu->IX.B.L
#define zhIX            cpu->IX.B.H

#define pzIY            &(cpu->IY)
#define zIY             cpu->IY.W
#define zlIY            cpu->IY.B.L
#define zhIY            cpu->IY.B.H

#define pzSP            &(cpu->SP)
#define zSP             cpu->SP.W
#define zlSP            cpu->SP.B.L
#define zhSP            cpu->SP.B.H

#define zRealPC         (cpu->PC - cpu->BasePC)
#define zPC             cpu->PC

#define zI              cpu->I
#define zIM             cpu->IM

#define zwR             cpu->R.W
#define zR1             cpu->R.B.L
#define zR2             cpu->R.B.H
#define zR              zR1

#define zIFF            cpu->IFF.W
#define zIFF1           cpu->IFF.B.L
#define zIFF2           cpu->IFF.B.H

#define CZ80_SF_SFT     7
#define CZ80_ZF_SFT     6
#define CZ80_YF_SFT     5
#define CZ80_HF_SFT     4
#define CZ80_XF_SFT     3
#define CZ80_PF_SFT     2
#define CZ80_VF_SFT     2
#define CZ80_NF_SFT     1
#define CZ80_CF_SFT     0

#define CZ80_SF         (1 << CZ80_SF_SFT)
#define CZ80_ZF         (1 << CZ80_ZF_SFT)
#define CZ80_YF         (1 << CZ80_YF_SFT)
#define CZ80_HF         (1 << CZ80_HF_SFT)
#define CZ80_XF         (1 << CZ80_XF_SFT)
#define CZ80_PF         (1 << CZ80_PF_SFT)
#define CZ80_VF         (1 << CZ80_VF_SFT)
#define CZ80_NF         (1 << CZ80_NF_SFT)
#define CZ80_CF         (1 << CZ80_CF_SFT)

#define CZ80_IFF_SFT    CZ80_PF_SFT
#define CZ80_IFF        CZ80_PF


typedef union
{
	struct
	{
#if CZ80_LITTLE_ENDIAN
		u8 L;
		u8 H;
#else
		u8 H;
		u8 L;
#endif
	} B;
	u16 W;
} union16;

typedef struct
{
    union16 BC;         // 32 bytes aligned
    union16 DE;
    union16 HL;
    union16 AF;

    union16 IX;
    union16 IY;
    union16 SP;
    u32     PC;

    union16 BC2;
    union16 DE2;
    union16 HL2;
    union16 AF2;

	union16 R;
	union16 IFF;

	u8 I;
	u8 IM;
	u8 IRQVector;
	u8 IRQLine;
	u8 NMIState;
	u8 afterEI;
	u8 Halted;
    u8 filler;

	u32 BasePC;

	int ICount;
	int InitialICount;
	int ExtraCycles;

	u32 (*Read_Byte)(u32 adr);
	void (*Write_Byte)(u32 adr, u32 data);
#if CZ80_USE_WORD_HANDLER
	u32 (*Read_Word)(u32 adr);
	void (*Write_Word)(u32 adr, u32 data);
#endif

	u32 (*IN_Port)(u32 port);
	void (*OUT_Port)(u32 port, u32 value);

	int (*Interrupt_Callback)(int line);
	void (*RETI_Callback)(void);

	u8 *Fetch[CZ80_FETCH_BANK];
} cz80_struc;


/*************************/
/* Publics Z80 variables */
/*************************/

extern cz80_struc CZ80;


/*************************/
/* Publics Z80 functions */
/*************************/

void Cz80_Init(cz80_struc *cpu);
void Cz80_Reset(cz80_struc *cpu);

void Cz80_Set_Fetch(cz80_struc *cpu, u32 low_adr, u32 high_adr, u32 fetch_adr);

void Cz80_Set_ReadB(cz80_struc *cpu, u32 (*func)(u32 adr));
void Cz80_Set_WriteB(cz80_struc *cpu, void (*func)(u32 adr, u32 data));
#if CZ80_USE_WORD_HANDLER
void Cz80_Set_ReadW(cz80_struc *cpu, u32 (*func)(u32 adr));
void Cz80_Set_WriteW(cz80_struc *cpu, void (*func)(u32 adr, u32 data));
#endif

void Cz80_Set_INPort(cz80_struc *cpu, u32 (*func)(u32 port));
void Cz80_Set_OUTPort(cz80_struc *cpu, void (*func)(u32 port, u32 value));

void Cz80_Set_IRQ_Callback(cz80_struc *cpu, int (*callback)(int line));
void Cz80_Set_RETI_Callback(cz80_struc *cpu, void (*callback)(void));

u32  Cz80_Read_Byte(cz80_struc *cpu, u32 adr);
u32  Cz80_Read_Word(cz80_struc *cpu, u32 adr);
void Cz80_Write_Byte(cz80_struc *cpu, u32 adr, u32 data);
void Cz80_Write_Word(cz80_struc *cpu, u32 adr, u32 data);

int  Cz80_Exec(cz80_struc *cpu, int cycles);

void Cz80_Set_IRQ(cz80_struc *cpu);
void Cz80_Set_NMI(cz80_struc *cpu);
void Cz80_Clear_IRQ(cz80_struc *cpu);

int  Cz80_Get_CycleToDo(cz80_struc *cpu);
int  Cz80_Get_CycleRemaining(cz80_struc *cpu);
int  Cz80_Get_CycleDone(cz80_struc *cpu);
void Cz80_Release_Cycle(cz80_struc *cpu);
void Cz80_Add_Cycle(cz80_struc *cpu, int cycle);

u32  Cz80_Get_BC(cz80_struc *cpu);
u32  Cz80_Get_DE(cz80_struc *cpu);
u32  Cz80_Get_HL(cz80_struc *cpu);
u32  Cz80_Get_AF(cz80_struc *cpu);

u32  Cz80_Get_BC2(cz80_struc *cpu);
u32  Cz80_Get_DE2(cz80_struc *cpu);
u32  Cz80_Get_HL2(cz80_struc *cpu);
u32  Cz80_Get_AF2(cz80_struc *cpu);

u32  Cz80_Get_IX(cz80_struc *cpu);
u32  Cz80_Get_IY(cz80_struc *cpu);
u32  Cz80_Get_SP(cz80_struc *cpu);
u32  Cz80_Get_PC(cz80_struc *cpu);

u32  Cz80_Get_R(cz80_struc *cpu);
u32  Cz80_Get_IFF(cz80_struc *cpu);
u32  Cz80_Get_IM(cz80_struc *cpu);
u32  Cz80_Get_I(cz80_struc *cpu);

void Cz80_Set_BC(cz80_struc *cpu, u32 value);
void Cz80_Set_DE(cz80_struc *cpu, u32 value);
void Cz80_Set_HL(cz80_struc *cpu, u32 value);
void Cz80_Set_AF(cz80_struc *cpu, u32 value);

void Cz80_Set_BC2(cz80_struc *cpu, u32 value);
void Cz80_Set_DE2(cz80_struc *cpu, u32 value);
void Cz80_Set_HL2(cz80_struc *cpu, u32 value);
void Cz80_Set_AF2(cz80_struc *cpu, u32 value);

void Cz80_Set_IX(cz80_struc *cpu, u32 value);
void Cz80_Set_IY(cz80_struc *cpu, u32 value);
void Cz80_Set_SP(cz80_struc *cpu, u32 value);
void Cz80_Set_PC(cz80_struc *cpu, u32 value);

void Cz80_Set_R(cz80_struc *cpu, u32 value);
void Cz80_Set_IFF(cz80_struc *cpu, u32 value);
void Cz80_Set_IM(cz80_struc *cpu, u32 value);
void Cz80_Set_I(cz80_struc *cpu, u32 value);

#ifdef __cplusplus
};
#endif

#endif  // _CZ80_H_

