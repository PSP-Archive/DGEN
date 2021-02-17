// internals core macros
/////////////////////////

#define LSL(A, C)       ((A) << (C))
#define LSR(A, C)       ((A) >> (C))

#define LSR_32(A, C)    ((C) < 32 ? (A) >> (C) : 0)
#define LSL_32(A, C)    ((C) < 32 ? (A) << (C) : 0)

#define ROL_8(A, C)     (LSL(A, C) | LSR(A, 8-(C)))
#define ROL_9(A, C)     (LSL(A, C) | LSR(A, 9-(C)))
#define ROL_16(A, C)    (LSL(A, C) | LSR(A, 16-(C)))
#define ROL_17(A, C)    (LSL(A, C) | LSR(A, 17-(C)))
#define ROL_32(A, C)    (LSL_32(A, C) | LSR_32(A, 32-(C)))
#define ROL_33(A, C)    (LSL_32(A, C) | LSR_32(A, 33-(C)))

#define ROR_8(A, C)     (LSR(A, C) | LSL(A, 8-(C)))
#define ROR_9(A, C)     (LSR(A, C) | LSL(A, 9-(C)))
#define ROR_16(A, C)    (LSR(A, C) | LSL(A, 16-(C)))
#define ROR_17(A, C)    (LSR(A, C) | LSL(A, 17-(C)))
#define ROR_32(A, C)    (LSR_32(A, C) | LSL_32(A, 32-(C)))
#define ROR_33(A, C)    (LSR_32(A, C) | LSL_32(A, 33-(C)))

#define PUSH_REG  (7)

#define _OP(a)  static void OP_##a(void){

#define _TAG_OP(a)  OP_##a

#define NEXT                    \
    Opcode = FETCH_WORD;        \
	PC += 2;			        \
	JumpTable[Opcode]() ;    \
	if( (CCnt <= 0) || bStop ){ bStop=0; goto C68k_Exec_End ;} \
	else                       goto C68k_Exec ;

#define RET(A)  CCnt -= (A); return ;}
//#define RET2(A) POST_IO CCnt -= (A); bStop=1; return ;}
//#define RET2(A) CCnt -= (A); bStop=1; return ;}
#define RET2(A) POST_IO CCnt -= (A); return ;}
#define RET3(A) CCnt -= (A); return ;

//#define RET(A)  return ;}
//#define RET2(A) POST_IO return ;}
//#define RET3(A) return ;

#define FAULTED()  return ;


/*
#define _OP(a)  OP_##a:

#define NEXT                    \
    Opcode = FETCH_WORD;        \
	PC += 2;			        \
    goto *JumpTable[Opcode] ;

#define RET(A)                  \
    CCnt -= (A);                \
    if (CCnt <= 0) goto C68k_Exec_End;	\
    NEXT

#define RET2(A)                 \
	POST_IO                     \
    CCnt -= (A);                \
    goto C68k_Exec_End;

#define FAULTED()  goto C68k_Exec_Really_End;
*/

#define SET_PC(A)               \
    CPU->BasePC = CPU->Fetch[((A & 0x00FFFFFF) >> C68K_FETCH_SFT) & C68K_FETCH_MASK];    \
    CPU->BasePC -= (A) & 0xFF000000;    \
    PC = (A) + CPU->BasePC;

///*
#define PRE_IO                  \
    CPU->CycleIO = CCnt;

#define POST_IO                 \
    CCnt = CPU->CycleIO;
//*/
//#define PRE_IO                  
//#define POST_IO                 

#define READ_BYTE_F(A, D)           \
    D = (u8)CPU->Read_Byte(A) ;//& 0xFF;

#define READ_WORD_F(A, D)           \
    D = (u16)CPU->Read_Word(A) ;//& 0xFFFF;

#ifdef C68K_FUNC_32
    #define READ_LONG_F(A, D)               \
    D = CPU->Read_Long((A)) ;

    #define READ_LONG_DEC_F(A, D)           \
    D = CPU->Read_Long((A)) ;
#else
#ifdef C68K_BIG_ENDIAN
    #define READ_LONG_F(A, D)           \
    D = CPU->Read_Word((A)) & 0xFFFF;   \
    D |= CPU->Read_Word((A) + 2) << 16;

    #define READ_LONG_DEC_F(A, D)       \
    D = CPU->Read_Word((A) + 2) << 16;  \
    D |= CPU->Read_Word((A)) & 0xFFFF;
#else

    #define READ_LONG_F(A, D)               \
    D = CPU->Read_Word((A)) << 16;          \
    D |= CPU->Read_Word((A) + 2) & 0xFFFF;

    #define READ_LONG_DEC_F(A, D)           \
    D = CPU->Read_Word((A) + 2) & 0xFFFF;   \
    D |= CPU->Read_Word((A)) << 16;
#endif
#endif //C68K_FUNC_32

#define READSX_BYTE_F(A, D)             \
    D = (s32)((s8)CPU->Read_Byte(A));

#define READSX_WORD_F(A, D)             \
    D = (s32)((s16)CPU->Read_Word(A));
    
#ifdef C68K_FUNC_32
    #define READSX_LONG_F(A, D)             \
    D = CPU->Read_Long((A)) ;

    #define READSX_LONG_DEC_F(A, D)         \
    D = CPU->Read_Long((A)) ;
#else
#ifdef C68K_BIG_ENDIAN
    #define READSX_LONG_F(A, D)         \
    D = CPU->Read_Word((A)) & 0xFFFF;   \
    D |= CPU->Read_Word((A) + 2) << 16;

    #define READSX_LONG_DEC_F(A, D)     \
    D = CPU->Read_Word((A) + 2) << 16;  \
    D |= CPU->Read_Word((A)) & 0xFFFF;
#else
    #define READSX_LONG_F(A, D)             \
    D = CPU->Read_Word((A)) << 16;          \
    D |= CPU->Read_Word((A) + 2) & 0xFFFF;

    #define READSX_LONG_DEC_F(A, D)         \
    D = CPU->Read_Word((A) + 2) & 0xFFFF;   \
    D |= CPU->Read_Word((A)) << 16;
#endif
#endif //C68K_FUNC_32

#define WRITE_BYTE_F(A, D)      \
    CPU->Write_Byte(A, D);

#define WRITE_WORD_F(A, D)      \
    CPU->Write_Word(A, D);

#ifdef C68K_FUNC_32
    #define WRITE_LONG_F(A, D)              \
    CPU->Write_Long(A,D);

#ifdef C68K_BIG_ENDIAN
    #define WRITE_LONG_DEC_F(A, D)          \
    CPU->Write_Word((A) + 2, (D) >> 16);    \
    CPU->Write_Word((A), (D) & 0xFFFF);
#else
    #define WRITE_LONG_DEC_F(A, D)          \
    CPU->Write_Word((A) + 2, (D) & 0xFFFF); \
    CPU->Write_Word((A), (D) >> 16);
#endif

#else // C68K_FUNC_32

#ifdef C68K_BIG_ENDIAN
    #define WRITE_LONG_F(A, D)              \
    CPU->Write_Word((A), (D) & 0xFFFF);     \
    CPU->Write_Word((A) + 2, (D) >> 16);

    #define WRITE_LONG_DEC_F(A, D)          \
    CPU->Write_Word((A) + 2, (D) >> 16);    \
    CPU->Write_Word((A), (D) & 0xFFFF);
#else
    #define WRITE_LONG_F(A, D)              \
    CPU->Write_Word((A), (D) >> 16);        \
    CPU->Write_Word((A) + 2, (D) & 0xFFFF);

    #define WRITE_LONG_DEC_F(A, D)          \
    CPU->Write_Word((A) + 2, (D) & 0xFFFF); \
    CPU->Write_Word((A), (D) >> 16);
#endif

#endif // C68K_FUNC_32

#define PUSH_16_F(D)                      \
	CPU->A[PUSH_REG] -= 2 ;               \
    CPU->Write_Word(CPU->A[PUSH_REG], D); 

#define POP_16_F(D)                     \
    D = (u16)CPU->Read_Word(CPU->A[PUSH_REG]); \
    CPU->A[PUSH_REG] += 2;

#ifdef C68K_FUNC_32
    #define PUSH_32_F(D)                                 \
    CPU->A[PUSH_REG] -= 4;                               \
    CPU->Write_Long(CPU->A[PUSH_REG], (D));

    #define POP_32_F(D)                            \
    D =  CPU->Read_Long(CPU->A[PUSH_REG]) ;        \
    CPU->A[PUSH_REG] += 4;
#else 
#ifdef C68K_BIG_ENDIAN
    #define PUSH_32_F(D)                               \
    CPU->A[PUSH_REG] -= 4;                             \
    CPU->Write_Word(CPU->A[PUSH_REG] + 2, (D) >> 16);  \
    CPU->Write_Word(CPU->A[PUSH_REG], (D) & 0xFFFF);
    
    #define POP_32_F(D)                              \
    D =  CPU->Read_Word(CPU->A[PUSH_REG]) & 0xFFFF;  \
    D |= CPU->Read_Word(CPU->A[PUSH_REG] + 2) << 16; \
    CPU->A[PUSH_REG] += 4;
#else
    #define PUSH_32_F(D)                                 \
    CPU->A[PUSH_REG] -= 4;                               \
    CPU->Write_Word(CPU->A[PUSH_REG] + 2, (D) & 0xFFFF); \
    CPU->Write_Word(CPU->A[PUSH_REG], (D) >> 16);

    #define POP_32_F(D)                                 \
    D =  CPU->Read_Word(CPU->A[PUSH_REG]) << 16;        \
    D |= CPU->Read_Word(CPU->A[PUSH_REG] + 2) & 0xFFFF; \
    CPU->A[PUSH_REG] += 4;
#endif
#endif // C68K_FUNC_32

#define FETCH_BYTE          \
((*(u16*)PC) & 0xFF)

#define FETCH_WORD          \
(*(u16*)PC)

#define FETCH_LONG          \
(*(u32*)PC)

#define DECODE_EXT_WORD     \
{                           \
    u32 ext;                \
                            \
    ext = (*(u16*)PC);      \
    PC += 2;                \
                            \
    adr += (s32)((s8)(ext));                            \
    if (ext & 0x0800) adr += (s32) CPU->D[ext >> 12];   \
    else adr += (s32)((s16)(CPU->D[ext >> 12]));        \
}

#ifndef C68K_BIG_ENDIAN
#ifdef C68K_BYTE_SWAP_OPT

    #undef FETCH_LONG
    #define FETCH_LONG          \
    ((((u32)(*(u16*)PC)) << 16) | ((u32)(*(u16*)(PC + 2))))
//    ((((u32)(*(u8*)(PC + 2))) | (((u32)(*(u8*)(PC + 3))) << 8) | (((u32)(*(u8*)PC)) << 16) | (((u32)(*(u8*)(PC + 1))) << 24)))

#else
    #undef FETCH_BYTE
    #define FETCH_BYTE          \
    (*(u16*)PC) >> 8)

    #undef FETCH_WORD
    #define FETCH_WORD          \
    ((((u16)(*(u8*)PC)) << 8) | ((u16)(*(u8*)(PC + 1))))
//    ((((u16)(*(u8*)(PC + 1))) | (((u16)(*(u8*)PC)) << 8)))

    #undef FETCH_LONG
    #define FETCH_LONG          \
    ((((u32)(*(u8*)PC)) << 24) | (((u32)(*(u8*)(PC + 1))) << 16) | (((u32)(*(u8*)(PC + 2))) << 8) | ((u32)(*(u8*)(PC + 3))))
//    ((((u32)(*(u8*)(PC + 3))) | (((u32)(*(u8*)(PC + 2))) << 8) | (((u32)(*(u8*)(PC + 1))) << 16) | (((u32)(*(u8*)PC)) << 24)))

    #undef DECODE_EXT_WORD
    #define DECODE_EXT_WORD     \
    {                           \
        u32 ext;                \
                                \
        ext = (*(u16*)PC);      \
        PC += 2;                \
                                \
        adr += (s32)((s8)(ext >> 8));                                   \
        if (ext & 0x0008) adr += (s32) CPU->D[(ext >> 4) & 0x000F];     \
        else adr += (s32)((s16)(CPU->D[(ext >> 4) & 0x000F]));          \
    }
#endif
#endif

#define GET_CCR                                     \
    (((CPU->flag_C >> (C68K_SR_C_SFT - 0)) & 1) |   \
     ((CPU->flag_V >> (C68K_SR_V_SFT - 1)) & 2) |   \
     (((!CPU->flag_notZ) & 1) << 2) |               \
     ((CPU->flag_N >> (C68K_SR_N_SFT - 3)) & 8) |   \
     ((CPU->flag_X >> (C68K_SR_X_SFT - 4)) & 0x10))

#define GET_SR                  \
    ((CPU->flag_S << 0)  |      \
     (CPU->flag_I << 8)  |      \
     GET_CCR)

#define SET_CCR(A)                              \
    CPU->flag_C = (A) << (C68K_SR_C_SFT - 0);   \
    CPU->flag_V = (A) << (C68K_SR_V_SFT - 1);   \
    CPU->flag_notZ = ~(A) & 4;                  \
    CPU->flag_N = (A) << (C68K_SR_N_SFT - 3);   \
    CPU->flag_X = (A) << (C68K_SR_X_SFT - 4);

#define SET_SR(A)                   \
    SET_CCR(A)                      \
    CPU->flag_I = ((A) >> 8) & 7;   \
    CPU->flag_S = (A) & C68K_SR_S;

//#define SWAP_USP()                
//*
#define SWAP_USP()                \
		if (!CPU->flag_S)         \
		{                         \
			res = CPU->A[7];      \
			CPU->A[7] = CPU->USP; \
			CPU->USP = res;       \
		}
//*/

#define CHECK_INT                                   \
    {                                               \
        s32 line, vect;                             \
                                                    \
        line = CPU->IRQLine;                        \
                                                    \
        if ((line == 7) || (line > CPU->flag_I))    \
        {                                           \
            PRE_IO                                  \
                                                    \
            /* get vector */                                        \
            CPU->IRQLine = 0;                                       \
            vect = CPU->Interrupt_CallBack(line);                   \
            if (vect == C68K_INT_ACK_AUTOVECTOR)                    \
                vect = C68K_INTERRUPT_AUTOVECTOR_EX + (line & 7);   \
                                                                    \
            /* adjust CCnt */                                       \
            CCnt -= c68k_exception_cycle_table[vect];               \
                                                                    \
            /* swap A7 and USP */           \
            if (!CPU->flag_S)               \
            {                               \
                u32 tmpSP;                  \
                                            \
                tmpSP = CPU->USP;           \
                CPU->USP = CPU->A[7];       \
                CPU->A[7] = tmpSP;          \
            }                               \
                                            \
            /* push PC and SR */            \
            PUSH_32_F(PC - CPU->BasePC)     \
            PUSH_16_F(GET_SR)               \
                                            \
            /* adjust SR */                 \
            CPU->flag_S = C68K_SR_S;        \
            CPU->flag_I = line;             \
                                            \
            /* fetch new PC */              \
            READ_LONG_F(vect * 4, PC)       \
            SET_PC(PC)                      \
                                            \
            POST_IO                         \
        }                                   \
    }
