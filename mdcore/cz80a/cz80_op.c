/********************************************************************************/
/*                                                                              */
/* CZ80 opcode include source file                                              */
/* C Z80 emulator version 0.9                                                   */
/* Copyright 2004-2005 Stéphane Dallongeville                                   */
/*                                                                              */
/********************************************************************************/

#if CZ80_USE_JUMPTABLE
	goto *JumpTable[Opcode];
#else
MAIN_OP:
switch (Opcode)
{
#endif

/*-----------------------------------------
 NOP
-----------------------------------------*/
	OP(0x00):   // NOP


/*-----------------------------------------
 LD r8 (same register)
-----------------------------------------*/

	OP(0x40):   // LD   B,B
	OP(0x49):   // LD   C,C
	OP(0x52):   // LD   D,D
	OP(0x5b):   // LD   E,E
	OP(0x64):   // LD   H,H
	OP(0x6d):   // LD   L,L
	OP(0x7f):   // LD   A,A
OP_NOP:
		NEXT


/*-----------------------------------------
 LD r8
-----------------------------------------*/

	OP(0x41):   // LD   B,C
	OP(0x42):   // LD   B,D
	OP(0x43):   // LD   B,E
	OP(0x44):   // LD   B,H
	OP(0x45):   // LD   B,L
	OP(0x47):   // LD   B,A

	OP(0x48):   // LD   C,B
	OP(0x4a):   // LD   C,D
	OP(0x4b):   // LD   C,E
	OP(0x4c):   // LD   C,H
	OP(0x4d):   // LD   C,L
	OP(0x4f):   // LD   C,A

	OP(0x50):   // LD   D,B
	OP(0x51):   // LD   D,C
	OP(0x53):   // LD   D,E
	OP(0x54):   // LD   D,H
	OP(0x55):   // LD   D,L
	OP(0x57):   // LD   D,A

	OP(0x58):   // LD   E,B
	OP(0x59):   // LD   E,C
	OP(0x5a):   // LD   E,D
	OP(0x5c):   // LD   E,H
	OP(0x5d):   // LD   E,L
	OP(0x5f):   // LD   E,A

	OP(0x60):   // LD   H,B
	OP(0x61):   // LD   H,C
	OP(0x62):   // LD   H,D
	OP(0x63):   // LD   H,E
	OP(0x65):   // LD   H,L
	OP(0x67):   // LD   H,A

	OP(0x68):   // LD   L,B
	OP(0x69):   // LD   L,C
	OP(0x6a):   // LD   L,D
	OP(0x6b):   // LD   L,E
	OP(0x6c):   // LD   L,H
	OP(0x6f):   // LD   L,A

	OP(0x78):   // LD   A,B
	OP(0x79):   // LD   A,C
	OP(0x7a):   // LD   A,D
	OP(0x7b):   // LD   A,E
	OP(0x7c):   // LD   A,H
	OP(0x7d):   // LD   A,L
OP_LD_R_R:
		zR8((Opcode >> 3) & 7) = zR8(Opcode & 7);
		NEXT

	OP(0x06):   // LD   B,#imm
	OP(0x0e):   // LD   C,#imm
	OP(0x16):   // LD   D,#imm
	OP(0x1e):   // LD   E,#imm
	OP(0x26):   // LD   H,#imm
	OP(0x2e):   // LD   L,#imm
	OP(0x3e):   // LD   A,#imm
OP_LD_R_imm:
		zR8(Opcode >> 3) = FETCH_BYTE;
		NEXT

	OP(0x46):   // LD   B,(HL)
	OP(0x4e):   // LD   C,(HL)
	OP(0x56):   // LD   D,(HL)
	OP(0x5e):   // LD   E,(HL)
	OP(0x66):   // LD   H,(HL)
	OP(0x6e):   // LD   L,(HL)
	OP(0x7e):   // LD   A,(HL)
OP_LD_R_mHL:
		READ_BYTE(zHL, zR8((Opcode >> 3) & 7))
		NEXT

	OP(0x70):   // LD   (HL),B
	OP(0x71):   // LD   (HL),C
	OP(0x72):   // LD   (HL),D
	OP(0x73):   // LD   (HL),E
	OP(0x74):   // LD   (HL),H
	OP(0x75):   // LD   (HL),L
	OP(0x77):   // LD   (HL),A
OP_LD_mHL_R:
		WRITE_BYTE(zHL, zR8(Opcode & 7))
		NEXT

	OP(0x36):   // LD (HL), #imm
OP_LD_mHL_imm:
		WRITE_BYTE(zHL, FETCH_BYTE)
		NEXT

	OP(0x0a):   // LD   A,(BC)
OP_LOAD_A_mBC:
		EA = zBC;
		goto OP_LOAD_A_mxx;

	OP(0x1a):   // LD   A,(DE)
OP_LOAD_A_mDE:
		EA = zDE;

OP_LOAD_A_mxx:
		READ_BYTE(EA, zA)
		NEXT

	OP(0x3a):   // LD   A,(nn)
OP_LOAD_A_mNN:
		FETCH_WORD(EA)
		READ_BYTE(EA, zA)
		NEXT

	OP(0x02):   // LD   (BC),A
OP_LOAD_mBC_A:
		EA = zBC;
		goto OP_LOAD_mxx_A;

	OP(0x12):   // LD   (DE),A
OP_LOAD_mDE_A:
		EA = zDE;

OP_LOAD_mxx_A:
		WRITE_BYTE(EA, zA)
		NEXT

	OP(0x32):   // LD   (nn),A
OP_LOAD_mNN_A:
		FETCH_WORD(EA)
		WRITE_BYTE(EA, zA)
		NEXT


/*-----------------------------------------
 LD r16
-----------------------------------------*/

	OP(0x01):   // LD   BC,nn
	OP(0x11):   // LD   DE,nn
	OP(0x21):   // LD   HL,nn
OP_LOAD_RR_imm16:
		FETCH_WORD(zR16(Opcode >> 4))
		NEXT

	OP(0x31):   // LD   SP,nn
OP_LOAD_SP_imm16:
		FETCH_WORD(zSP)
		NEXT

	OP(0xf9):   // LD   SP,HL
OP_LD_SP_xx:
		zSP = data->W;
		NEXT

	OP(0x2a):   // LD   HL,(nn)
OP_LD_xx_mNN:
		FETCH_WORD(EA)
		READ_WORD(EA, data->W)
		NEXT

	OP(0x22):   // LD   (nn),HL
OP_LD_mNN_xx:
		FETCH_WORD(EA)
		WRITE_WORD(EA, data->W)
		NEXT


/*-----------------------------------------
 POP
-----------------------------------------*/

	OP(0xc1):   // POP  BC
	OP(0xd1):   // POP  DE
	OP(0xf1):   // POP  AF
OP_POP_RR:
		data = pzR16[(Opcode >> 4) & 3];

	OP(0xe1):   // POP  HL
OP_POP:
		POP_16(data->W)
		NEXT


/*-----------------------------------------
 PUSH
-----------------------------------------*/

	OP(0xc5):   // PUSH BC
	OP(0xd5):   // PUSH DE
	OP(0xf5):   // PUSH AF
OP_PUSH_RR:
		data = pzR16[(Opcode >> 4) & 3];

	OP(0xe5):   // PUSH HL
OP_PUSH:
		PUSH_16(data->W);
		NEXT


/*-----------------------------------------
 EX
-----------------------------------------*/

	OP(0x08):   // EX   AF,AF'
OP_EX_AF_AF2:
		EX(zAF, zAF2)
		NEXT

	OP(0xeb):   // EX   DE,HL
OP_EX_DE_HL:
		EX(zDE, zHL)
		NEXT

	OP(0xd9):   // EXX
OP_EXX:
		EX(zBC, zBC2)
		EX(zDE, zDE2)
		EX(zHL, zHL2)
		NEXT

	OP(0xe3):   // EX   HL,(SP)
OP_EX_xx_mSP:
		val = data->W;
		READ_WORD(zSP, data->W)
		WRITE_WORD(zSP, val)
		NEXT


/*-----------------------------------------
 INC r8
-----------------------------------------*/

	OP(0x04):   // INC  B
	OP(0x0c):   // INC  C
	OP(0x14):   // INC  D
	OP(0x1c):   // INC  E
	OP(0x24):   // INC  H
	OP(0x2c):   // INC  L
	OP(0x3c):   // INC  A
OP_INC_R:
		zR8(Opcode >> 3)++;
		zF = (zF & CF) | SZHV_inc[zR8(Opcode >> 3)];
		NEXT

OP_INC_mIx:
		EA = data->W + FETCH_BYTE_S;
		goto OP_INC_m;

	OP(0x34):   // INC  (HL)
		EA = zHL;

OP_INC_m:
		READ_BYTE(EA, val)
		val = (val + 1) & 0xff;
		WRITE_BYTE(EA, val)
		zF = (zF & CF) | SZHV_inc[val];
		NEXT


/*-----------------------------------------
 DEC r8
-----------------------------------------*/

	OP(0x05):   // DEC  B
	OP(0x0d):   // DEC  C
	OP(0x15):   // DEC  D
	OP(0x1d):   // DEC  E
	OP(0x25):   // DEC  H
	OP(0x2d):   // DEC  L
	OP(0x3d):   // DEC  A
OP_DEC_R:
		zR8(Opcode >> 3)--;
		zF = (zF & CF) | SZHV_dec[zR8(Opcode >> 3)];
		NEXT

OP_DEC_mIx:
		EA = data->W + FETCH_BYTE_S;
		goto OP_DEC_m;

	OP(0x35):   // DEC  (HL)
		EA = zHL;

OP_DEC_m:
		READ_BYTE(EA, val)
		val = (val - 1) & 0xff;
		WRITE_BYTE(EA, val)
		zF = (zF & CF) | SZHV_dec[val];
		NEXT


/*-----------------------------------------
 ADD r8
-----------------------------------------*/

OP_ADD_mIx:
		READ_BYTE(data->W + FETCH_BYTE_S, val)
		goto OP_ADD;

	OP(0xc6):   // ADD  A,n
OP_ADD_imm:
		val = FETCH_BYTE;
		goto OP_ADD;

	OP(0x86):   // ADD  A,(HL)
OP_ADD_mHL:
		READ_BYTE(zHL, val)
		goto OP_ADD;

OP_ADD_IxH:
		val = data->B.H;
		goto OP_ADD;

OP_ADD_IxL:
		val = data->B.L;
		goto OP_ADD;

	OP(0x80):   // ADD  A,B
	OP(0x81):   // ADD  A,C
	OP(0x82):   // ADD  A,D
	OP(0x83):   // ADD  A,E
	OP(0x84):   // ADD  A,H
	OP(0x85):   // ADD  A,L
	OP(0x87):   // ADD  A,A
OP_ADD_R:
		val = zR8(Opcode & 7);

OP_ADD:
		ADD(val)
		NEXT


/*-----------------------------------------
 ADC r8
-----------------------------------------*/

OP_ADC_mIx:
		READ_BYTE(data->W + FETCH_BYTE_S, val)
		goto OP_ADC;

	OP(0xce):   // ADC  A,n
OP_ADC_imm:
		val = FETCH_BYTE;
		goto OP_ADC;

	OP(0x8e):   // ADC  A,(HL)
OP_ADC_mHL:
		READ_BYTE(zHL, val)
		goto OP_ADC;

OP_ADC_IxH:
		val = data->B.H;
		goto OP_ADC;

OP_ADC_IxL:
		val = data->B.L;
		goto OP_ADC;

	OP(0x88):   // ADC  A,B
	OP(0x89):   // ADC  A,C
	OP(0x8a):   // ADC  A,D
	OP(0x8b):   // ADC  A,E
	OP(0x8c):   // ADC  A,H
	OP(0x8d):   // ADC  A,L
	OP(0x8f):   // ADC  A,A
OP_ADC_R:
		val = zR8(Opcode & 7);

OP_ADC:
		ADC(val)
		NEXT


/*-----------------------------------------
 SUB r8
-----------------------------------------*/

OP_SUB_mIx:
		READ_BYTE(data->W + FETCH_BYTE_S, val)
		goto OP_SUB;

	OP(0xd6):   // SUB  A,n
OP_SUB_imm:
		val = FETCH_BYTE;
		goto OP_SUB;

	OP(0x96):   // SUB  (HL)
OP_SUB_mHL:
		READ_BYTE(zHL, val)
		goto OP_SUB;

OP_SUB_IxH:
		val = data->B.H;
		goto OP_SUB;

OP_SUB_IxL:
		val = data->B.L;
		goto OP_SUB;

	OP(0x90):   // SUB  B
	OP(0x91):   // SUB  C
	OP(0x92):   // SUB  D
	OP(0x93):   // SUB  E
	OP(0x94):   // SUB  H
	OP(0x95):   // SUB  L
	OP(0x97):   // SUB  A
OP_SUB_R:
		val = zR8(Opcode & 7);

OP_SUB:
		SUB(val)
		NEXT


/*-----------------------------------------
 SBC r8
-----------------------------------------*/

OP_SBC_mIx:
		READ_BYTE(data->W + FETCH_BYTE_S, val)
		goto OP_SBC;

	OP(0xde):   // SBC  A,n
OP_SBC_imm:
		val = FETCH_BYTE;
		goto OP_SBC;

	OP(0x9e):   // SBC  A,(HL)
OP_SBC_mHL:
		READ_BYTE(zHL, val)
		goto OP_SBC;

OP_SBC_IxH:
		val = data->B.H;
		goto OP_SBC;

OP_SBC_IxL:
		val = data->B.L;
		goto OP_SBC;

	OP(0x98):   // SBC  A,B
	OP(0x99):   // SBC  A,C
	OP(0x9a):   // SBC  A,D
	OP(0x9b):   // SBC  A,E
	OP(0x9c):   // SBC  A,H
	OP(0x9d):   // SBC  A,L
	OP(0x9f):   // SBC  A,A
OP_SBC_R:
		val = zR8(Opcode & 7);

OP_SBC:
		SBC(val)
		NEXT


/*-----------------------------------------
 CP r8
-----------------------------------------*/

OP_CP_mIx:
		READ_BYTE(data->W + FETCH_BYTE_S, val)
		goto OP_CP;

	OP(0xfe):   // CP   n
OP_CP_imm:
		val = FETCH_BYTE;
		goto OP_CP;

	OP(0xbe):   // CP   (HL)
OP_CP_mHL:
		READ_BYTE(zHL, val)
		goto OP_CP;

OP_CP_IxH:
		val = data->B.H;
		goto OP_CP;

OP_CP_IxL:
		val = data->B.L;
		goto OP_CP;

	OP(0xb8):   // CP   B
	OP(0xb9):   // CP   C
	OP(0xba):   // CP   D
	OP(0xbb):   // CP   E
	OP(0xbc):   // CP   H
	OP(0xbd):   // CP   L
	OP(0xbf):   // CP   A
OP_CP_R:
		val = zR8(Opcode & 7);

OP_CP:
		CP(val)
		NEXT


/*-----------------------------------------
 AND r8
-----------------------------------------*/

	OP(0xa6):   // AND  (HL)
OP_AND_mHL:
		READ_BYTE(zHL, val)
		goto OP_AND;

	OP(0xe6):   // AND  A,n
OP_AND_imm:
		val = FETCH_BYTE;
		goto OP_AND;

OP_AND_IxL:
		val = data->B.L;
		goto OP_AND;

OP_AND_IxH:
		val = data->B.H;
		goto OP_AND;

	OP(0xa0):   // AND  B
	OP(0xa1):   // AND  C
	OP(0xa2):   // AND  D
	OP(0xa3):   // AND  E
	OP(0xa4):   // AND  H
	OP(0xa5):   // AND  L
	OP(0xa7):   // AND  A
OP_AND_R:
		val = zR8(Opcode & 7);

OP_AND:
		AND(val)
		NEXT


/*-----------------------------------------
 XOR r8
-----------------------------------------*/

	OP(0xae):   // XOR  (HL)
OP_XOR_mHL:
		READ_BYTE(zHL, val)
		goto OP_XOR;

	OP(0xee):   // XOR  A,n
OP_XOR_imm:
		val = FETCH_BYTE;
		goto OP_XOR;

OP_XOR_IxL:
		val = data->B.L;
		goto OP_XOR;

OP_XOR_IxH:
		val = data->B.H;
		goto OP_XOR;

	OP(0xa8):   // XOR  B
	OP(0xa9):   // XOR  C
	OP(0xaa):   // XOR  D
	OP(0xab):   // XOR  E
	OP(0xac):   // XOR  H
	OP(0xad):   // XOR  L
	OP(0xaf):   // XOR  A
OP_XOR_R:
		val = zR8(Opcode & 7);

OP_XOR:
		XOR(val)
		NEXT


/*-----------------------------------------
 OR r8
-----------------------------------------*/

	OP(0xb6):   // OR   (HL)
OP_OR_mHL:
		READ_BYTE(zHL, val)
		goto OP_OR;

	OP(0xf6):   // OR   A,n
OP_OR_imm:
		val = FETCH_BYTE;
		goto OP_OR;

OP_OR_IxL:
		val = data->B.L;
		goto OP_OR;

OP_OR_IxH:
		val = data->B.H;
		goto OP_OR;

	OP(0xb0):   // OR   B
	OP(0xb1):   // OR   C
	OP(0xb2):   // OR   D
	OP(0xb3):   // OR   E
	OP(0xb4):   // OR   H
	OP(0xb5):   // OR   L
	OP(0xb7):   // OR   A
OP_OR_R:
		val = zR8(Opcode & 7);

OP_OR:
		OR(val)
		NEXT


/*-----------------------------------------
 MISC ARITHMETIC & cpu CONTROL
-----------------------------------------*/

	OP(0x27):   // DAA
OP_DAA:
		DAA
		NEXT

	OP(0x2f):   // CPL
OP_CPL:
		CPL
		NEXT

	OP(0x37):   // SCF
OP_SCF:
		SCF
		NEXT

	OP(0x3f):   // CCF
OP_CCF:
		CCF
		NEXT

	OP(0x76):   // HALT
OP_HALT:
		cpu->Halted = 1;
		cpu->ICount = 0;
		goto Cz80_Exec_Really_End;

	OP(0xf3):   // DI
OP_DI:
		zIFF1 = zIFF2 = 0;
		NEXT

	OP(0xfb):   // EI
OP_EI:
		EI
		goto Cz80_Exec;


/*-----------------------------------------
 INC r16
-----------------------------------------*/

	OP(0x03):   // INC  BC
OP_INC_BC:
		zBC++;
		NEXT

	OP(0x13):   // INC  DE
OP_INC_DE:
		zDE++;
		NEXT

	OP(0x23):   // INC  HL
OP_INC_xx:
		data->W++;
		NEXT

	OP(0x33):   // INC  SP
OP_INC_SP:
		zSP++;
		NEXT


/*-----------------------------------------
 DEC r16
-----------------------------------------*/

	OP(0x0b):   // DEC  BC
OP_DEC_BC:
		zBC--;
		NEXT

	OP(0x1b):   // DEC  DE
OP_DEC_DE:
		zDE--;
		NEXT

	OP(0x2b):   // DEC  HL
OP_DEC_xx:
		data->W--;
		NEXT

	OP(0x3b):   // DEC  SP
OP_DEC_SP:
		zSP--;
		NEXT


/*-----------------------------------------
 ADD r16
-----------------------------------------*/

	OP(0x39):   // ADD  xx,SP
OP_ADD16_xx_SP:
		val = zSP;
		goto OP_ADD16;

	OP(0x29):   // ADD  xx,xx
OP_ADD16_xx_xx:
		val = data->W;
		goto OP_ADD16;

	OP(0x09):   // ADD  xx,BC
OP_ADD16_xx_BC:
		val = zBC;
		goto OP_ADD16;

	OP(0x19):   // ADD  xx,DE
OP_ADD16_xx_DE:
		val = zDE;

OP_ADD16:
		ADD16(data->W, val)
		NEXT


/*-----------------------------------------
 ROTATE
-----------------------------------------*/

	OP(0x07):   // RLCA
OP_RLCA:
		RLCA
		NEXT

	OP(0x0f):   // RRCA
OP_RRCA:
		RRCA
		NEXT

	OP(0x17):   // RLA
OP_RLA:
		RLA
		NEXT

	OP(0x1f):   // RRA
OP_RRA:
		RRA
		NEXT


/*-----------------------------------------
 JP
-----------------------------------------*/

	OP(0xd2):   // JP   NC,nn
OP_JP_NC:
		if (!(zF & CF)) goto OP_JP;
		zPC += 2;
		NEXT

	OP(0xda):   // JP   C,nn
OP_JP_C:
		if (zF & CF) goto OP_JP;
		zPC += 2;
		NEXT

	OP(0xe2):   // JP   PO,nn
OP_JP_PO:
		if (!(zF & VF)) goto OP_JP;
		zPC += 2;
		NEXT

	OP(0xea):   // JP   PE,nn
OP_JP_PE:
		if (zF & VF) goto OP_JP;
		zPC += 2;
		NEXT

	OP(0xf2):   // JP   P,nn
OP_JP_P:
		if (!(zF & SF)) goto OP_JP;
		zPC += 2;
		NEXT

	OP(0xfa):   // JP   M,nn
OP_JP_M:
		if (zF & SF) goto OP_JP;
		zPC += 2;
		NEXT

	OP(0xca):   // JP   Z,nn
OP_JP_Z:
		if (zF & ZF) goto OP_JP;
		zPC += 2;
		NEXT

	OP(0xc2):   // JP   NZ,nn
OP_JP_NZ:
		if (!(zF & ZF)) goto OP_JP;
		zPC += 2;
		NEXT

	OP(0xc3):   // JP   nn
OP_JP:
		PC = GET_WORD;
		SET_PC(PC);
		NEXT

	OP(0xe9):   // JP   (xx)
OP_JP_xx:
		PC = data->W;
		SET_PC(PC);
		NEXT_EX


/*-----------------------------------------
 JR
-----------------------------------------*/

	OP(0x38):   // JR   C,n
OP_JR_C:
		if (zF & CF) goto OP_JR;
		zPC++;
		NEXT

	OP(0x30):   // JR   NC,n
OP_JR_NC:
		if (!(zF & CF)) goto OP_JR;
		zPC++;
		NEXT

	OP(0x28):   // JR   Z,n
OP_JR_Z:
		if (zF & ZF) goto OP_JR;
		zPC++;
		NEXT

	OP(0x20):   // JR   NZ,n
OP_JR_NZ:
		if (!(zF & ZF)) goto OP_JR;
		zPC++;
		NEXT

	OP(0x10):   // DJNZ n
OP_DJNZ:
		cpu->ICount--;
		if (--zB) goto OP_JR;
		zPC++;
		NEXT

	OP(0x18):   // JR   n
OP_JR:
		EA = FETCH_BYTE_S;
		zPC += EA;
		NEXT_EX


/*-----------------------------------------
 CALL
-----------------------------------------*/

	OP(0xd4):   // CALL NC,nn
OP_CALL_NC:
		if (!(zF & CF)) goto OP_CALL;
		zPC += 2;
		NEXT

	OP(0xdc):   // CALL C,nn
OP_CALL_C:
		if (zF & CF) goto OP_CALL;
		zPC += 2;
		NEXT

	OP(0xe4):   // CALL PO,nn
OP_CALL_PO:
		if (!(zF & VF)) goto OP_CALL;
		zPC += 2;
		NEXT

	OP(0xec):   // CALL PE,nn
OP_CALL_PE:
		if (zF & VF) goto OP_CALL;
		zPC += 2;
		NEXT

	OP(0xf4):   // CALL P,nn
OP_CALL_P:
		if (!(zF & SF)) goto OP_CALL;
		zPC += 2;
		NEXT

	OP(0xfc):   // CALL M,nn
OP_CALL_M:
		if (zF & SF) goto OP_CALL;
		zPC += 2;
		NEXT

	OP(0xcc):   // CALL Z,nn
OP_CALL_Z:
		if (zF & ZF) goto OP_CALL;
		zPC += 2;
		NEXT

	OP(0xc4):   // CALL NZ,nn
OP_CALL_NZ:
		if (!(zF & ZF)) goto OP_CALL;
		zPC += 2;
		NEXT

	OP(0xcd):   // CALL nn
OP_CALL:
	{
		u32 oldRPC;

		FETCH_WORD(PC);
		oldRPC = zRealPC;
		PUSH_16(oldRPC);
		SET_PC(PC);
		NEXT_EX
	}


/*-----------------------------------------
 RET
-----------------------------------------*/

	OP(0xd0):   // RET  NC
OP_RET_NC:
		if (!(zF & CF)) goto OP_RET;
		NEXT

	OP(0xd8):   // RET  C
OP_RET_C:
		if (zF & CF) goto OP_RET;
		NEXT

	OP(0xe0):   // RET  PO
OP_RET_PO:
		if (!(zF & VF)) goto OP_RET;
		NEXT

	OP(0xe8):   // RET  PE
OP_RET_PE:
		if (zF & VF) goto OP_RET;
		NEXT

	OP(0xf0):   // RET  P
OP_RET_P:
		if (!(zF & SF)) goto OP_RET;
		NEXT

	OP(0xf8):   // RET  M
OP_RET_M:
		if (zF & SF) goto OP_RET;
		NEXT

	OP(0xc0):   // RET  NZ
OP_RET_NZ:
		if (!(zF & ZF)) goto OP_RET;
		NEXT

	OP(0xc8):   // RET  Z
OP_RET_Z:
		if (zF & ZF) goto OP_RET;
		NEXT

	OP(0xc9):   // RET
OP_RET:
		POP_16(PC);
		SET_PC(PC);
		NEXT_EX


/*-----------------------------------------
 RST
-----------------------------------------*/

	OP(0xc7):   // RST  0
	OP(0xcf):   // RST  1
	OP(0xd7):   // RST  2
	OP(0xdf):   // RST  3
	OP(0xe7):   // RST  4
	OP(0xef):   // RST  5
	OP(0xf7):   // RST  6
	OP(0xff):   // RST  7
OP_RST:
		PC = zRealPC;
		PUSH_16(PC);
		PC = Opcode & 0x38;
		SET_PC(PC);
		NEXT


/*-----------------------------------------
 OUT
-----------------------------------------*/

	OP(0xd3):   // OUT  (n),A
OP_OUT_mN_A:
		EA = (zA << 8) | FETCH_BYTE;
		OUT(EA, zA)
		NEXT


/*-----------------------------------------
 IN
-----------------------------------------*/

	OP(0xdb):   // IN   A,(n)
OP_IN_A_mN:
		EA = (zA << 8) | FETCH_BYTE;
		IN(EA, zA)
		NEXT


/*-----------------------------------------
 PREFIX
-----------------------------------------*/

	OP(0xcb):   // CB prefix (BIT & SHIFT INSTRUCTIONS)
CB_PREFIX:
		zR++;
		Opcode = FETCH_BYTE;
		cc = cc_cb;
		#include "cz80_opCB.c"

	OP(0xed):   // ED prefix
ED_PREFIX:
		zR++;
		Opcode = FETCH_BYTE;
		cc = cc_ed;
		#include "cz80_opED.c"

	OP(0xdd):   // DD prefix (IX)
DD_PREFIX:
		data = pzIX;
		goto XY_PREFIX;

	OP(0xfd):   // FD prefix (IY)
FD_PREFIX:
		data = pzIY;

XY_PREFIX:
		zR++;
		Opcode = FETCH_BYTE;
		cc = cc_xy;
		#include "cz80_opXY.c"

#if (CZ80_USE_JUMPTABLE == 0)
}
#endif
