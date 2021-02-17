/********************************************************************************/
/*                                                                              */
/* CZ80 macro file                                                              */
/* C Z80 emulator version 0.9                                                   */
/* Copyright 2004-2005 Stéphane Dallongeville                                   */
/*                                                                              */
/********************************************************************************/

#if CZ80_USE_JUMPTABLE
#define _SSOP(A,B)	A##B
#define OP(A)		_SSOP(OP, A)
#define OPCB(A)		_SSOP(OPCB, A)
#define OPED(A)		_SSOP(OPED, A)
#define OPXY(A)		_SSOP(OPXY, A)
#define OPXYCB(A)	_SSOP(OPXYCB, A)
#else
#define OP(A)		case A
#define OPCB(A)		case A
#define OPED(A)		case A
#define OPXY(A)		case A
#define OPXYCB(A)	case A
#endif

#define GET_BYTE                \
    (*(u8*)zPC)

#define GET_BYTE_S              \
    (*(s8*)zPC)

#if CZ80_LITTLE_ENDIAN
#define GET_WORD                \
    ((*(u8*)(zPC + 0)) | ((*(u8*)(zPC + 1)) << 8))
#else
#define GET_WORD                \
    ((*(u8*)(zPC + 1)) | ((*(u8*)(zPC + 0)) << 8))
#endif

#define FETCH_BYTE		(*(u8 *)zPC++)
#define FETCH_BYTE_S	(*(s8 *)zPC++)
#define FETCH_WORD(A)	{ A = GET_WORD; zPC += 2; }

#define NEXT			{ cpu->ICount -= cc[Opcode]; goto Cz80_Exec; }
#define NEXT_EX			{ cpu->ICount -= cc[Opcode] + cc_ex[Opcode]; goto Cz80_Exec; }

#define SET_PC(A)               \
    cpu->BasePC = (u32) cpu->Fetch[(A) >> CZ80_FETCH_SFT];  \
    zPC = (A) + cpu->BasePC;

#define READ_BYTE(A, D)                 \
    D = cpu->Read_Byte(A);

#if CZ80_USE_WORD_HANDLER
#define READ_WORD(A, D)                 \
    D = cpu->Read_Word(A);
#elif CZ80_LITTLE_ENDIAN
#define READ_WORD(A, D)                 \
    D = cpu->Read_Byte(A) | (cpu->Read_Byte((A) + 1) << 8);
#else
#define READ_WORD(A, D)                 \
    D = (cpu->Read_Byte(A) << 8) | cpu->Read_Byte((A) + 1);
#endif

#define READSX_BYTE(A, D)               \
    D = (s32)(s8)cpu->Read_Byte(A);

#define WRITE_BYTE(A, D)                \
    cpu->Write_Byte(A, D);

#if CZ80_USE_WORD_HANDLER
#define WRITE_WORD(A, D)                \
    cpu->Write_Word(A, D);
#elif CZ80_LITTLE_ENDIAN
#define WRITE_WORD(A, D)                \
    cpu->Write_Byte(A, D);              \
    cpu->Write_Byte((A) + 1, (D) >> 8);
#else
#define WRITE_WORD(A, D)                \
    cpu->Write_Byte(A, D);              \
    cpu->Write_Byte((A) + 1, (D) >> 8);
#endif

#define PUSH_16(A)          \
    zSP -= 2;               \
    WRITE_WORD(zSP, A);

#define POP_16(A)           \
    READ_WORD(zSP, A)       \
    zSP += 2;

#define IN(A, D)            \
    D = cpu->IN_Port(A);

#define OUT(A, D)           \
    cpu->OUT_Port(A, D);
