
// ORI
//_OP(0x0000)
_OP(0x0000)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_BYTE;
	PC += 2;
	res = (u8)CPU->D[(Opcode >> 0) & 7];
	res |= src;
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res;
	*(u8*)(&CPU->D[(Opcode >> 0) & 7]) = res;
}
RET(8)

// ORI
_OP(0x0010)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_BYTE;
	PC += 2;
	adr = CPU->A[(Opcode >> 0) & 7];
	PRE_IO
	READ_BYTE_F(adr, res)
	res |= src;
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res;
	WRITE_BYTE_F(adr, res)
	POST_IO
}
RET(16)

// ORI
_OP(0x0018)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_BYTE;
	PC += 2;
	adr = CPU->A[(Opcode >> 0) & 7];
	CPU->A[(Opcode >> 0) & 7] += 1;
	PRE_IO
	READ_BYTE_F(adr, res)
	res |= src;
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res;
	WRITE_BYTE_F(adr, res)
	POST_IO
}
RET(16)

// ORI
_OP(0x0020)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_BYTE;
	PC += 2;
	adr = CPU->A[(Opcode >> 0) & 7] - 1;
	CPU->A[(Opcode >> 0) & 7] = adr;
	PRE_IO
	READ_BYTE_F(adr, res)
	res |= src;
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res;
	WRITE_BYTE_F(adr, res)
	POST_IO
}
RET(18)

// ORI
_OP(0x0028)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_BYTE;
	PC += 2;
	adr = CPU->A[(Opcode >> 0) & 7] + (s32)(s16)FETCH_WORD;
	PC += 2;
	PRE_IO
	READ_BYTE_F(adr, res)
	res |= src;
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res;
	WRITE_BYTE_F(adr, res)
	POST_IO
}
RET(20)

// ORI
_OP(0x0030)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_BYTE;
	PC += 2;
	adr = CPU->A[(Opcode >> 0) & 7];
	DECODE_EXT_WORD
	PRE_IO
	READ_BYTE_F(adr, res)
	res |= src;
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res;
	WRITE_BYTE_F(adr, res)
	POST_IO
}
RET(22)

// ORI
_OP(0x0038)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_BYTE;
	PC += 2;
	adr = (s32)(s16)FETCH_WORD;
	PC += 2;
	PRE_IO
	READ_BYTE_F(adr, res)
	res |= src;
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res;
	WRITE_BYTE_F(adr, res)
	POST_IO
}
RET(20)

// ORI
_OP(0x0039)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_BYTE;
	PC += 2;
	adr = (s32)FETCH_LONG;
	PC += 4;
	PRE_IO
	READ_BYTE_F(adr, res)
	res |= src;
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res;
	WRITE_BYTE_F(adr, res)
	POST_IO
}
RET(24)

// ORI
_OP(0x001F)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_BYTE;
	PC += 2;
	adr = CPU->A[7];
	CPU->A[7] += 2;
	PRE_IO
	READ_BYTE_F(adr, res)
	res |= src;
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res;
	WRITE_BYTE_F(adr, res)
	POST_IO
}
RET(16)

// ORI
_OP(0x0027)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_BYTE;
	PC += 2;
	adr = CPU->A[7] - 2;
	CPU->A[7] = adr;
	PRE_IO
	READ_BYTE_F(adr, res)
	res |= src;
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res;
	WRITE_BYTE_F(adr, res)
	POST_IO
}
RET(18)

// ORI
_OP(0x0040)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_WORD;
	PC += 2;
	res = (u16)CPU->D[(Opcode >> 0) & 7];
	res |= src;
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	*(u16*)(&CPU->D[(Opcode >> 0) & 7]) = res;
}
RET(8)

// ORI
_OP(0x0050)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_WORD;
	PC += 2;
	adr = CPU->A[(Opcode >> 0) & 7];
	PRE_IO
	READ_WORD_F(adr, res)
	res |= src;
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(16)

// ORI
_OP(0x0058)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_WORD;
	PC += 2;
	adr = CPU->A[(Opcode >> 0) & 7];
	CPU->A[(Opcode >> 0) & 7] += 2;
	PRE_IO
	READ_WORD_F(adr, res)
	res |= src;
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(16)

// ORI
_OP(0x0060)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_WORD;
	PC += 2;
	adr = CPU->A[(Opcode >> 0) & 7] - 2;
	CPU->A[(Opcode >> 0) & 7] = adr;
	PRE_IO
	READ_WORD_F(adr, res)
	res |= src;
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(18)

// ORI
_OP(0x0068)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_WORD;
	PC += 2;
	adr = CPU->A[(Opcode >> 0) & 7] + (s32)(s16)FETCH_WORD;
	PC += 2;
	PRE_IO
	READ_WORD_F(adr, res)
	res |= src;
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(20)

// ORI
_OP(0x0070)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_WORD;
	PC += 2;
	adr = CPU->A[(Opcode >> 0) & 7];
	DECODE_EXT_WORD
	PRE_IO
	READ_WORD_F(adr, res)
	res |= src;
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(22)

// ORI
_OP(0x0078)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_WORD;
	PC += 2;
	adr = (s32)(s16)FETCH_WORD;
	PC += 2;
	PRE_IO
	READ_WORD_F(adr, res)
	res |= src;
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(20)

// ORI
_OP(0x0079)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_WORD;
	PC += 2;
	adr = (s32)FETCH_LONG;
	PC += 4;
	PRE_IO
	READ_WORD_F(adr, res)
	res |= src;
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(24)

// ORI
_OP(0x005F)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_WORD;
	PC += 2;
	adr = CPU->A[7];
	CPU->A[7] += 2;
	PRE_IO
	READ_WORD_F(adr, res)
	res |= src;
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(16)

// ORI
_OP(0x0067)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_WORD;
	PC += 2;
	adr = CPU->A[7] - 2;
	CPU->A[7] = adr;
	PRE_IO
	READ_WORD_F(adr, res)
	res |= src;
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(18)

// ORI
_OP(0x0080)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_LONG;
	PC += 4;
	res = (u32)CPU->D[(Opcode >> 0) & 7];
	res |= src;
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 24;
	*(u32*)(&CPU->D[(Opcode >> 0) & 7]) = res;
}
RET(16)

// ORI
_OP(0x0090)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_LONG;
	PC += 4;
	adr = CPU->A[(Opcode >> 0) & 7];
	PRE_IO
	READ_LONG_F(adr, res)
	res |= src;
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 24;
	WRITE_LONG_F(adr, res)
	POST_IO
}
RET(28)

// ORI
_OP(0x0098)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_LONG;
	PC += 4;
	adr = CPU->A[(Opcode >> 0) & 7];
	CPU->A[(Opcode >> 0) & 7] += 4;
	PRE_IO
	READ_LONG_F(adr, res)
	res |= src;
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 24;
	WRITE_LONG_F(adr, res)
	POST_IO
}
RET(28)

// ORI
_OP(0x00A0)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_LONG;
	PC += 4;
	adr = CPU->A[(Opcode >> 0) & 7] - 4;
	CPU->A[(Opcode >> 0) & 7] = adr;
	PRE_IO
	READ_LONG_F(adr, res)
	res |= src;
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 24;
	WRITE_LONG_F(adr, res)
	POST_IO
}
RET(30)

// ORI
_OP(0x00A8)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_LONG;
	PC += 4;
	adr = CPU->A[(Opcode >> 0) & 7] + (s32)(s16)FETCH_WORD;
	PC += 2;
	PRE_IO
	READ_LONG_F(adr, res)
	res |= src;
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 24;
	WRITE_LONG_F(adr, res)
	POST_IO
}
RET(32)

// ORI
_OP(0x00B0)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_LONG;
	PC += 4;
	adr = CPU->A[(Opcode >> 0) & 7];
	DECODE_EXT_WORD
	PRE_IO
	READ_LONG_F(adr, res)
	res |= src;
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 24;
	WRITE_LONG_F(adr, res)
	POST_IO
}
RET(34)

// ORI
_OP(0x00B8)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_LONG;
	PC += 4;
	adr = (s32)(s16)FETCH_WORD;
	PC += 2;
	PRE_IO
	READ_LONG_F(adr, res)
	res |= src;
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 24;
	WRITE_LONG_F(adr, res)
	POST_IO
}
RET(32)

// ORI
_OP(0x00B9)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_LONG;
	PC += 4;
	adr = (s32)FETCH_LONG;
	PC += 4;
	PRE_IO
	READ_LONG_F(adr, res)
	res |= src;
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 24;
	WRITE_LONG_F(adr, res)
	POST_IO
}
RET(36)

// ORI
_OP(0x009F)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_LONG;
	PC += 4;
	adr = CPU->A[7];
	CPU->A[7] += 4;
	PRE_IO
	READ_LONG_F(adr, res)
	res |= src;
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 24;
	WRITE_LONG_F(adr, res)
	POST_IO
}
RET(28)

// ORI
_OP(0x00A7)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_LONG;
	PC += 4;
	adr = CPU->A[7] - 4;
	CPU->A[7] = adr;
	PRE_IO
	READ_LONG_F(adr, res)
	res |= src;
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 24;
	WRITE_LONG_F(adr, res)
	POST_IO
}
RET(30)

// ORICCR
_OP(0x003C)
{
	u32 adr, res;
	u32 src, dst;

	res = FETCH_BYTE & C68K_CCR_MASK;
	PC += 2;
	res |= GET_CCR;
	SET_CCR(res)
}
RET(20)

// ORISR
_OP(0x007C)
{
	u32 adr, res;
	u32 src, dst;

	if (CPU->flag_S)
	{
		res = FETCH_WORD & C68K_SR_MASK;
		PC += 2;
		res |= GET_SR;
		SET_SR(res)
	}
	else
	{
		res = CPU->USP;
		CPU->USP = CPU->A[7];
		CPU->A[7] = res;
		res = C68K_PRIVILEGE_VIOLATION_EX;
	CCnt -= c68k_exception_cycle_table[res];
	PRE_IO
		PUSH_32_F(PC - CPU->BasePC)
		PUSH_16_F(GET_SR)
		CPU->flag_S = C68K_SR_S;
		READ_LONG_F(res * 4, PC)
		SET_PC(PC)
	}

	POST_IO //<-
}
RET(20)
// warning!!
//POST_IO
//CCnt -= 20;
//goto C68k_Exec_End;

// ANDI
_OP(0x0200)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_BYTE;
	PC += 2;
	res = (u8)CPU->D[(Opcode >> 0) & 7];
	res &= src;
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res;
	*(u8*)(&CPU->D[(Opcode >> 0) & 7]) = res;
}
RET(8)

// ANDI
_OP(0x0210)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_BYTE;
	PC += 2;
	adr = CPU->A[(Opcode >> 0) & 7];
	PRE_IO
	READ_BYTE_F(adr, res)
	res &= src;
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res;
	WRITE_BYTE_F(adr, res)
	POST_IO
}
RET(16)

// ANDI
_OP(0x0218)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_BYTE;
	PC += 2;
	adr = CPU->A[(Opcode >> 0) & 7];
	CPU->A[(Opcode >> 0) & 7] += 1;
	PRE_IO
	READ_BYTE_F(adr, res)
	res &= src;
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res;
	WRITE_BYTE_F(adr, res)
	POST_IO
}
RET(16)

// ANDI
_OP(0x0220)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_BYTE;
	PC += 2;
	adr = CPU->A[(Opcode >> 0) & 7] - 1;
	CPU->A[(Opcode >> 0) & 7] = adr;
	PRE_IO
	READ_BYTE_F(adr, res)
	res &= src;
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res;
	WRITE_BYTE_F(adr, res)
	POST_IO
}
RET(18)

// ANDI
_OP(0x0228)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_BYTE;
	PC += 2;
	adr = CPU->A[(Opcode >> 0) & 7] + (s32)(s16)FETCH_WORD;
	PC += 2;
	PRE_IO
	READ_BYTE_F(adr, res)
	res &= src;
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res;
	WRITE_BYTE_F(adr, res)
	POST_IO
}
RET(20)

// ANDI
_OP(0x0230)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_BYTE;
	PC += 2;
	adr = CPU->A[(Opcode >> 0) & 7];
	DECODE_EXT_WORD
	PRE_IO
	READ_BYTE_F(adr, res)
	res &= src;
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res;
	WRITE_BYTE_F(adr, res)
	POST_IO
}
RET(22)

// ANDI
_OP(0x0238)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_BYTE;
	PC += 2;
	adr = (s32)(s16)FETCH_WORD;
	PC += 2;
	PRE_IO
	READ_BYTE_F(adr, res)
	res &= src;
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res;
	WRITE_BYTE_F(adr, res)
	POST_IO
}
RET(20)

// ANDI
_OP(0x0239)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_BYTE;
	PC += 2;
	adr = (s32)FETCH_LONG;
	PC += 4;
	PRE_IO
	READ_BYTE_F(adr, res)
	res &= src;
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res;
	WRITE_BYTE_F(adr, res)
	POST_IO
}
RET(24)

// ANDI
_OP(0x021F)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_BYTE;
	PC += 2;
	adr = CPU->A[7];
	CPU->A[7] += 2;
	PRE_IO
	READ_BYTE_F(adr, res)
	res &= src;
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res;
	WRITE_BYTE_F(adr, res)
	POST_IO
}
RET(16)

// ANDI
_OP(0x0227)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_BYTE;
	PC += 2;
	adr = CPU->A[7] - 2;
	CPU->A[7] = adr;
	PRE_IO
	READ_BYTE_F(adr, res)
	res &= src;
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res;
	WRITE_BYTE_F(adr, res)
	POST_IO
}
RET(18)

// ANDI
_OP(0x0240)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_WORD;
	PC += 2;
	res = (u16)CPU->D[(Opcode >> 0) & 7];
	res &= src;
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	*(u16*)(&CPU->D[(Opcode >> 0) & 7]) = res;
}
RET(8)

// ANDI
_OP(0x0250)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_WORD;
	PC += 2;
	adr = CPU->A[(Opcode >> 0) & 7];
	PRE_IO
	READ_WORD_F(adr, res)
	res &= src;
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(16)

// ANDI
_OP(0x0258)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_WORD;
	PC += 2;
	adr = CPU->A[(Opcode >> 0) & 7];
	CPU->A[(Opcode >> 0) & 7] += 2;
	PRE_IO
	READ_WORD_F(adr, res)
	res &= src;
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(16)

// ANDI
_OP(0x0260)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_WORD;
	PC += 2;
	adr = CPU->A[(Opcode >> 0) & 7] - 2;
	CPU->A[(Opcode >> 0) & 7] = adr;
	PRE_IO
	READ_WORD_F(adr, res)
	res &= src;
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(18)

// ANDI
_OP(0x0268)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_WORD;
	PC += 2;
	adr = CPU->A[(Opcode >> 0) & 7] + (s32)(s16)FETCH_WORD;
	PC += 2;
	PRE_IO
	READ_WORD_F(adr, res)
	res &= src;
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(20)

// ANDI
_OP(0x0270)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_WORD;
	PC += 2;
	adr = CPU->A[(Opcode >> 0) & 7];
	DECODE_EXT_WORD
	PRE_IO
	READ_WORD_F(adr, res)
	res &= src;
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(22)

// ANDI
_OP(0x0278)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_WORD;
	PC += 2;
	adr = (s32)(s16)FETCH_WORD;
	PC += 2;
	PRE_IO
	READ_WORD_F(adr, res)
	res &= src;
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(20)

// ANDI
_OP(0x0279)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_WORD;
	PC += 2;
	adr = (s32)FETCH_LONG;
	PC += 4;
	PRE_IO
	READ_WORD_F(adr, res)
	res &= src;
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(24)

// ANDI
_OP(0x025F)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_WORD;
	PC += 2;
	adr = CPU->A[7];
	CPU->A[7] += 2;
	PRE_IO
	READ_WORD_F(adr, res)
	res &= src;
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(16)

// ANDI
_OP(0x0267)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_WORD;
	PC += 2;
	adr = CPU->A[7] - 2;
	CPU->A[7] = adr;
	PRE_IO
	READ_WORD_F(adr, res)
	res &= src;
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(18)

// ANDI
_OP(0x0280)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_LONG;
	PC += 4;
	res = (u32)CPU->D[(Opcode >> 0) & 7];
	res &= src;
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 24;
	*(u32*)(&CPU->D[(Opcode >> 0) & 7]) = res;
}
RET(16)

// ANDI
_OP(0x0290)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_LONG;
	PC += 4;
	adr = CPU->A[(Opcode >> 0) & 7];
	PRE_IO
	READ_LONG_F(adr, res)
	res &= src;
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 24;
	WRITE_LONG_F(adr, res)
	POST_IO
}
RET(28)

// ANDI
_OP(0x0298)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_LONG;
	PC += 4;
	adr = CPU->A[(Opcode >> 0) & 7];
	CPU->A[(Opcode >> 0) & 7] += 4;
	PRE_IO
	READ_LONG_F(adr, res)
	res &= src;
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 24;
	WRITE_LONG_F(adr, res)
	POST_IO
}
RET(28)

// ANDI
_OP(0x02A0)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_LONG;
	PC += 4;
	adr = CPU->A[(Opcode >> 0) & 7] - 4;
	CPU->A[(Opcode >> 0) & 7] = adr;
	PRE_IO
	READ_LONG_F(adr, res)
	res &= src;
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 24;
	WRITE_LONG_F(adr, res)
	POST_IO
}
RET(30)

// ANDI
_OP(0x02A8)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_LONG;
	PC += 4;
	adr = CPU->A[(Opcode >> 0) & 7] + (s32)(s16)FETCH_WORD;
	PC += 2;
	PRE_IO
	READ_LONG_F(adr, res)
	res &= src;
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 24;
	WRITE_LONG_F(adr, res)
	POST_IO
}
RET(32)

// ANDI
_OP(0x02B0)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_LONG;
	PC += 4;
	adr = CPU->A[(Opcode >> 0) & 7];
	DECODE_EXT_WORD
	PRE_IO
	READ_LONG_F(adr, res)
	res &= src;
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 24;
	WRITE_LONG_F(adr, res)
	POST_IO
}
RET(34)

// ANDI
_OP(0x02B8)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_LONG;
	PC += 4;
	adr = (s32)(s16)FETCH_WORD;
	PC += 2;
	PRE_IO
	READ_LONG_F(adr, res)
	res &= src;
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 24;
	WRITE_LONG_F(adr, res)
	POST_IO
}
RET(32)

// ANDI
_OP(0x02B9)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_LONG;
	PC += 4;
	adr = (s32)FETCH_LONG;
	PC += 4;
	PRE_IO
	READ_LONG_F(adr, res)
	res &= src;
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 24;
	WRITE_LONG_F(adr, res)
	POST_IO
}
RET(36)

// ANDI
_OP(0x029F)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_LONG;
	PC += 4;
	adr = CPU->A[7];
	CPU->A[7] += 4;
	PRE_IO
	READ_LONG_F(adr, res)
	res &= src;
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 24;
	WRITE_LONG_F(adr, res)
	POST_IO
}
RET(28)

// ANDI
_OP(0x02A7)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_LONG;
	PC += 4;
	adr = CPU->A[7] - 4;
	CPU->A[7] = adr;
	PRE_IO
	READ_LONG_F(adr, res)
	res &= src;
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 24;
	WRITE_LONG_F(adr, res)
	POST_IO
}
RET(30)

// ANDICCR
_OP(0x023C)
{
	u32 adr, res;
	u32 src, dst;

	res = FETCH_BYTE & C68K_CCR_MASK;
	PC += 2;
	res &= GET_CCR;
	SET_CCR(res)
}
RET(20)

// ANDISR
_OP(0x027C)
{
	u32 adr, res;
	u32 src, dst;

	if (CPU->flag_S)
	{
		res = FETCH_WORD & C68K_SR_MASK;
		PC += 2;
		res &= GET_SR;
		SET_SR(res)
		SWAP_USP() ;
	}
	else
	{
		res = CPU->USP;
		CPU->USP = CPU->A[7];
		CPU->A[7] = res;
		res = C68K_PRIVILEGE_VIOLATION_EX;
	CCnt -= c68k_exception_cycle_table[res];
	PRE_IO
		PUSH_32_F(PC - CPU->BasePC)
		PUSH_16_F(GET_SR)
		CPU->flag_S = C68K_SR_S;
		READ_LONG_F(res * 4, PC)
		SET_PC(PC)
	}

	POST_IO //<-
}
RET(20)
// warning!!
//POST_IO
//CCnt -= 20;
//goto C68k_Exec_End;

// EORI
_OP(0x0A00)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_BYTE;
	PC += 2;
	res = (u8)CPU->D[(Opcode >> 0) & 7];
	res ^= src;
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res;
	*(u8*)(&CPU->D[(Opcode >> 0) & 7]) = res;
}
RET(8)

// EORI
_OP(0x0A10)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_BYTE;
	PC += 2;
	adr = CPU->A[(Opcode >> 0) & 7];
	PRE_IO
	READ_BYTE_F(adr, res)
	res ^= src;
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res;
	WRITE_BYTE_F(adr, res)
	POST_IO
}
RET(16)

// EORI
_OP(0x0A18)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_BYTE;
	PC += 2;
	adr = CPU->A[(Opcode >> 0) & 7];
	CPU->A[(Opcode >> 0) & 7] += 1;
	PRE_IO
	READ_BYTE_F(adr, res)
	res ^= src;
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res;
	WRITE_BYTE_F(adr, res)
	POST_IO
}
RET(16)

// EORI
_OP(0x0A20)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_BYTE;
	PC += 2;
	adr = CPU->A[(Opcode >> 0) & 7] - 1;
	CPU->A[(Opcode >> 0) & 7] = adr;
	PRE_IO
	READ_BYTE_F(adr, res)
	res ^= src;
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res;
	WRITE_BYTE_F(adr, res)
	POST_IO
}
RET(18)

// EORI
_OP(0x0A28)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_BYTE;
	PC += 2;
	adr = CPU->A[(Opcode >> 0) & 7] + (s32)(s16)FETCH_WORD;
	PC += 2;
	PRE_IO
	READ_BYTE_F(adr, res)
	res ^= src;
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res;
	WRITE_BYTE_F(adr, res)
	POST_IO
}
RET(20)

// EORI
_OP(0x0A30)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_BYTE;
	PC += 2;
	adr = CPU->A[(Opcode >> 0) & 7];
	DECODE_EXT_WORD
	PRE_IO
	READ_BYTE_F(adr, res)
	res ^= src;
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res;
	WRITE_BYTE_F(adr, res)
	POST_IO
}
RET(22)

// EORI
_OP(0x0A38)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_BYTE;
	PC += 2;
	adr = (s32)(s16)FETCH_WORD;
	PC += 2;
	PRE_IO
	READ_BYTE_F(adr, res)
	res ^= src;
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res;
	WRITE_BYTE_F(adr, res)
	POST_IO
}
RET(20)

// EORI
_OP(0x0A39)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_BYTE;
	PC += 2;
	adr = (s32)FETCH_LONG;
	PC += 4;
	PRE_IO
	READ_BYTE_F(adr, res)
	res ^= src;
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res;
	WRITE_BYTE_F(adr, res)
	POST_IO
}
RET(24)

// EORI
_OP(0x0A1F)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_BYTE;
	PC += 2;
	adr = CPU->A[7];
	CPU->A[7] += 2;
	PRE_IO
	READ_BYTE_F(adr, res)
	res ^= src;
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res;
	WRITE_BYTE_F(adr, res)
	POST_IO
}
RET(16)

// EORI
_OP(0x0A27)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_BYTE;
	PC += 2;
	adr = CPU->A[7] - 2;
	CPU->A[7] = adr;
	PRE_IO
	READ_BYTE_F(adr, res)
	res ^= src;
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res;
	WRITE_BYTE_F(adr, res)
	POST_IO
}
RET(18)

// EORI
_OP(0x0A40)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_WORD;
	PC += 2;
	res = (u16)CPU->D[(Opcode >> 0) & 7];
	res ^= src;
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	*(u16*)(&CPU->D[(Opcode >> 0) & 7]) = res;
}
RET(8)

// EORI
_OP(0x0A50)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_WORD;
	PC += 2;
	adr = CPU->A[(Opcode >> 0) & 7];
	PRE_IO
	READ_WORD_F(adr, res)
	res ^= src;
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(16)

// EORI
_OP(0x0A58)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_WORD;
	PC += 2;
	adr = CPU->A[(Opcode >> 0) & 7];
	CPU->A[(Opcode >> 0) & 7] += 2;
	PRE_IO
	READ_WORD_F(adr, res)
	res ^= src;
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(16)

// EORI
_OP(0x0A60)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_WORD;
	PC += 2;
	adr = CPU->A[(Opcode >> 0) & 7] - 2;
	CPU->A[(Opcode >> 0) & 7] = adr;
	PRE_IO
	READ_WORD_F(adr, res)
	res ^= src;
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(18)

// EORI
_OP(0x0A68)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_WORD;
	PC += 2;
	adr = CPU->A[(Opcode >> 0) & 7] + (s32)(s16)FETCH_WORD;
	PC += 2;
	PRE_IO
	READ_WORD_F(adr, res)
	res ^= src;
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(20)

// EORI
_OP(0x0A70)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_WORD;
	PC += 2;
	adr = CPU->A[(Opcode >> 0) & 7];
	DECODE_EXT_WORD
	PRE_IO
	READ_WORD_F(adr, res)
	res ^= src;
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(22)

// EORI
_OP(0x0A78)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_WORD;
	PC += 2;
	adr = (s32)(s16)FETCH_WORD;
	PC += 2;
	PRE_IO
	READ_WORD_F(adr, res)
	res ^= src;
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(20)

// EORI
_OP(0x0A79)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_WORD;
	PC += 2;
	adr = (s32)FETCH_LONG;
	PC += 4;
	PRE_IO
	READ_WORD_F(adr, res)
	res ^= src;
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(24)

// EORI
_OP(0x0A5F)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_WORD;
	PC += 2;
	adr = CPU->A[7];
	CPU->A[7] += 2;
	PRE_IO
	READ_WORD_F(adr, res)
	res ^= src;
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(16)

// EORI
_OP(0x0A67)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_WORD;
	PC += 2;
	adr = CPU->A[7] - 2;
	CPU->A[7] = adr;
	PRE_IO
	READ_WORD_F(adr, res)
	res ^= src;
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(18)

// EORI
_OP(0x0A80)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_LONG;
	PC += 4;
	res = (u32)CPU->D[(Opcode >> 0) & 7];
	res ^= src;
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 24;
	*(u32*)(&CPU->D[(Opcode >> 0) & 7]) = res;
}
RET(16)

// EORI
_OP(0x0A90)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_LONG;
	PC += 4;
	adr = CPU->A[(Opcode >> 0) & 7];
	PRE_IO
	READ_LONG_F(adr, res)
	res ^= src;
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 24;
	WRITE_LONG_F(adr, res)
	POST_IO
}
RET(28)

// EORI
_OP(0x0A98)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_LONG;
	PC += 4;
	adr = CPU->A[(Opcode >> 0) & 7];
	CPU->A[(Opcode >> 0) & 7] += 4;
	PRE_IO
	READ_LONG_F(adr, res)
	res ^= src;
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 24;
	WRITE_LONG_F(adr, res)
	POST_IO
}
RET(28)

// EORI
_OP(0x0AA0)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_LONG;
	PC += 4;
	adr = CPU->A[(Opcode >> 0) & 7] - 4;
	CPU->A[(Opcode >> 0) & 7] = adr;
	PRE_IO
	READ_LONG_F(adr, res)
	res ^= src;
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 24;
	WRITE_LONG_F(adr, res)
	POST_IO
}
RET(30)

// EORI
_OP(0x0AA8)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_LONG;
	PC += 4;
	adr = CPU->A[(Opcode >> 0) & 7] + (s32)(s16)FETCH_WORD;
	PC += 2;
	PRE_IO
	READ_LONG_F(adr, res)
	res ^= src;
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 24;
	WRITE_LONG_F(adr, res)
	POST_IO
}
RET(32)

// EORI
_OP(0x0AB0)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_LONG;
	PC += 4;
	adr = CPU->A[(Opcode >> 0) & 7];
	DECODE_EXT_WORD
	PRE_IO
	READ_LONG_F(adr, res)
	res ^= src;
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 24;
	WRITE_LONG_F(adr, res)
	POST_IO
}
RET(34)

// EORI
_OP(0x0AB8)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_LONG;
	PC += 4;
	adr = (s32)(s16)FETCH_WORD;
	PC += 2;
	PRE_IO
	READ_LONG_F(adr, res)
	res ^= src;
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 24;
	WRITE_LONG_F(adr, res)
	POST_IO
}
RET(32)

// EORI
_OP(0x0AB9)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_LONG;
	PC += 4;
	adr = (s32)FETCH_LONG;
	PC += 4;
	PRE_IO
	READ_LONG_F(adr, res)
	res ^= src;
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 24;
	WRITE_LONG_F(adr, res)
	POST_IO
}
RET(36)

// EORI
_OP(0x0A9F)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_LONG;
	PC += 4;
	adr = CPU->A[7];
	CPU->A[7] += 4;
	PRE_IO
	READ_LONG_F(adr, res)
	res ^= src;
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 24;
	WRITE_LONG_F(adr, res)
	POST_IO
}
RET(28)

// EORI
_OP(0x0AA7)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_LONG;
	PC += 4;
	adr = CPU->A[7] - 4;
	CPU->A[7] = adr;
	PRE_IO
	READ_LONG_F(adr, res)
	res ^= src;
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 24;
	WRITE_LONG_F(adr, res)
	POST_IO
}
RET(30)

// EORICCR
_OP(0x0A3C)
{
	u32 adr, res;
	u32 src, dst;

	res = FETCH_BYTE & C68K_CCR_MASK;
	PC += 2;
	res ^= GET_CCR;
	SET_CCR(res)
}
RET(20)

// EORISR
_OP(0x0A7C)
{
	u32 adr, res;
	u32 src, dst;

	if (CPU->flag_S)
	{
		res = FETCH_WORD & C68K_SR_MASK;
		PC += 2;
		res ^= GET_SR;
		SET_SR(res)
		SWAP_USP() ;
	}
	else
	{
		res = CPU->USP;
		CPU->USP = CPU->A[7];
		CPU->A[7] = res;
		res = C68K_PRIVILEGE_VIOLATION_EX;
	CCnt -= c68k_exception_cycle_table[res];
	PRE_IO
		PUSH_32_F(PC - CPU->BasePC)
		PUSH_16_F(GET_SR)
		CPU->flag_S = C68K_SR_S;
		READ_LONG_F(res * 4, PC)
		SET_PC(PC)
	}

	POST_IO //<-
}
RET(20)
//warning!!
//POST_IO
//CCnt -= 20;
//goto C68k_Exec_End;

// SUBI
_OP(0x0400)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_BYTE;
	PC += 2;
	dst = (u8)CPU->D[(Opcode >> 0) & 7];
	res = dst - src;
	CPU->flag_N = CPU->flag_X = CPU->flag_C = res;
	CPU->flag_V = (src ^ dst) & (res ^ dst);
	CPU->flag_notZ = res & 0xFF;
	*(u8*)(&CPU->D[(Opcode >> 0) & 7]) = res;
}
RET(8)

// SUBI
_OP(0x0410)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_BYTE;
	PC += 2;
	adr = CPU->A[(Opcode >> 0) & 7];
	PRE_IO
	READ_BYTE_F(adr, dst)
	res = dst - src;
	CPU->flag_N = CPU->flag_X = CPU->flag_C = res;
	CPU->flag_V = (src ^ dst) & (res ^ dst);
	CPU->flag_notZ = res & 0xFF;
	WRITE_BYTE_F(adr, res)
	POST_IO
}
RET(16)

// SUBI
_OP(0x0418)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_BYTE;
	PC += 2;
	adr = CPU->A[(Opcode >> 0) & 7];
	CPU->A[(Opcode >> 0) & 7] += 1;
	PRE_IO
	READ_BYTE_F(adr, dst)
	res = dst - src;
	CPU->flag_N = CPU->flag_X = CPU->flag_C = res;
	CPU->flag_V = (src ^ dst) & (res ^ dst);
	CPU->flag_notZ = res & 0xFF;
	WRITE_BYTE_F(adr, res)
	POST_IO
}
RET(16)

// SUBI
_OP(0x0420)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_BYTE;
	PC += 2;
	adr = CPU->A[(Opcode >> 0) & 7] - 1;
	CPU->A[(Opcode >> 0) & 7] = adr;
	PRE_IO
	READ_BYTE_F(adr, dst)
	res = dst - src;
	CPU->flag_N = CPU->flag_X = CPU->flag_C = res;
	CPU->flag_V = (src ^ dst) & (res ^ dst);
	CPU->flag_notZ = res & 0xFF;
	WRITE_BYTE_F(adr, res)
	POST_IO
}
RET(18)

// SUBI
_OP(0x0428)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_BYTE;
	PC += 2;
	adr = CPU->A[(Opcode >> 0) & 7] + (s32)(s16)FETCH_WORD;
	PC += 2;
	PRE_IO
	READ_BYTE_F(adr, dst)
	res = dst - src;
	CPU->flag_N = CPU->flag_X = CPU->flag_C = res;
	CPU->flag_V = (src ^ dst) & (res ^ dst);
	CPU->flag_notZ = res & 0xFF;
	WRITE_BYTE_F(adr, res)
	POST_IO
}
RET(20)

// SUBI
_OP(0x0430)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_BYTE;
	PC += 2;
	adr = CPU->A[(Opcode >> 0) & 7];
	DECODE_EXT_WORD
	PRE_IO
	READ_BYTE_F(adr, dst)
	res = dst - src;
	CPU->flag_N = CPU->flag_X = CPU->flag_C = res;
	CPU->flag_V = (src ^ dst) & (res ^ dst);
	CPU->flag_notZ = res & 0xFF;
	WRITE_BYTE_F(adr, res)
	POST_IO
}
RET(22)

// SUBI
_OP(0x0438)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_BYTE;
	PC += 2;
	adr = (s32)(s16)FETCH_WORD;
	PC += 2;
	PRE_IO
	READ_BYTE_F(adr, dst)
	res = dst - src;
	CPU->flag_N = CPU->flag_X = CPU->flag_C = res;
	CPU->flag_V = (src ^ dst) & (res ^ dst);
	CPU->flag_notZ = res & 0xFF;
	WRITE_BYTE_F(adr, res)
	POST_IO
}
RET(20)

// SUBI
_OP(0x0439)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_BYTE;
	PC += 2;
	adr = (s32)FETCH_LONG;
	PC += 4;
	PRE_IO
	READ_BYTE_F(adr, dst)
	res = dst - src;
	CPU->flag_N = CPU->flag_X = CPU->flag_C = res;
	CPU->flag_V = (src ^ dst) & (res ^ dst);
	CPU->flag_notZ = res & 0xFF;
	WRITE_BYTE_F(adr, res)
	POST_IO
}
RET(24)

// SUBI
_OP(0x041F)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_BYTE;
	PC += 2;
	adr = CPU->A[7];
	CPU->A[7] += 2;
	PRE_IO
	READ_BYTE_F(adr, dst)
	res = dst - src;
	CPU->flag_N = CPU->flag_X = CPU->flag_C = res;
	CPU->flag_V = (src ^ dst) & (res ^ dst);
	CPU->flag_notZ = res & 0xFF;
	WRITE_BYTE_F(adr, res)
	POST_IO
}
RET(16)

// SUBI
_OP(0x0427)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_BYTE;
	PC += 2;
	adr = CPU->A[7] - 2;
	CPU->A[7] = adr;
	PRE_IO
	READ_BYTE_F(adr, dst)
	res = dst - src;
	CPU->flag_N = CPU->flag_X = CPU->flag_C = res;
	CPU->flag_V = (src ^ dst) & (res ^ dst);
	CPU->flag_notZ = res & 0xFF;
	WRITE_BYTE_F(adr, res)
	POST_IO
}
RET(18)

// SUBI
_OP(0x0440)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_WORD;
	PC += 2;
	dst = (u16)CPU->D[(Opcode >> 0) & 7];
	res = dst - src;
	CPU->flag_V = ((src ^ dst) & (res ^ dst)) >> 8;
	CPU->flag_N = CPU->flag_X = CPU->flag_C = res >> 8;
	CPU->flag_notZ = res & 0xFFFF;
	*(u16*)(&CPU->D[(Opcode >> 0) & 7]) = res;
}
RET(8)

// SUBI
_OP(0x0450)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_WORD;
	PC += 2;
	adr = CPU->A[(Opcode >> 0) & 7];
	PRE_IO
	READ_WORD_F(adr, dst)
	res = dst - src;
	CPU->flag_V = ((src ^ dst) & (res ^ dst)) >> 8;
	CPU->flag_N = CPU->flag_X = CPU->flag_C = res >> 8;
	CPU->flag_notZ = res & 0xFFFF;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(16)

// SUBI
_OP(0x0458)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_WORD;
	PC += 2;
	adr = CPU->A[(Opcode >> 0) & 7];
	CPU->A[(Opcode >> 0) & 7] += 2;
	PRE_IO
	READ_WORD_F(adr, dst)
	res = dst - src;
	CPU->flag_V = ((src ^ dst) & (res ^ dst)) >> 8;
	CPU->flag_N = CPU->flag_X = CPU->flag_C = res >> 8;
	CPU->flag_notZ = res & 0xFFFF;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(16)

// SUBI
_OP(0x0460)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_WORD;
	PC += 2;
	adr = CPU->A[(Opcode >> 0) & 7] - 2;
	CPU->A[(Opcode >> 0) & 7] = adr;
	PRE_IO
	READ_WORD_F(adr, dst)
	res = dst - src;
	CPU->flag_V = ((src ^ dst) & (res ^ dst)) >> 8;
	CPU->flag_N = CPU->flag_X = CPU->flag_C = res >> 8;
	CPU->flag_notZ = res & 0xFFFF;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(18)

// SUBI
_OP(0x0468)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_WORD;
	PC += 2;
	adr = CPU->A[(Opcode >> 0) & 7] + (s32)(s16)FETCH_WORD;
	PC += 2;
	PRE_IO
	READ_WORD_F(adr, dst)
	res = dst - src;
	CPU->flag_V = ((src ^ dst) & (res ^ dst)) >> 8;
	CPU->flag_N = CPU->flag_X = CPU->flag_C = res >> 8;
	CPU->flag_notZ = res & 0xFFFF;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(20)

// SUBI
_OP(0x0470)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_WORD;
	PC += 2;
	adr = CPU->A[(Opcode >> 0) & 7];
	DECODE_EXT_WORD
	PRE_IO
	READ_WORD_F(adr, dst)
	res = dst - src;
	CPU->flag_V = ((src ^ dst) & (res ^ dst)) >> 8;
	CPU->flag_N = CPU->flag_X = CPU->flag_C = res >> 8;
	CPU->flag_notZ = res & 0xFFFF;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(22)

// SUBI
_OP(0x0478)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_WORD;
	PC += 2;
	adr = (s32)(s16)FETCH_WORD;
	PC += 2;
	PRE_IO
	READ_WORD_F(adr, dst)
	res = dst - src;
	CPU->flag_V = ((src ^ dst) & (res ^ dst)) >> 8;
	CPU->flag_N = CPU->flag_X = CPU->flag_C = res >> 8;
	CPU->flag_notZ = res & 0xFFFF;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(20)

// SUBI
_OP(0x0479)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_WORD;
	PC += 2;
	adr = (s32)FETCH_LONG;
	PC += 4;
	PRE_IO
	READ_WORD_F(adr, dst)
	res = dst - src;
	CPU->flag_V = ((src ^ dst) & (res ^ dst)) >> 8;
	CPU->flag_N = CPU->flag_X = CPU->flag_C = res >> 8;
	CPU->flag_notZ = res & 0xFFFF;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(24)

// SUBI
_OP(0x045F)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_WORD;
	PC += 2;
	adr = CPU->A[7];
	CPU->A[7] += 2;
	PRE_IO
	READ_WORD_F(adr, dst)
	res = dst - src;
	CPU->flag_V = ((src ^ dst) & (res ^ dst)) >> 8;
	CPU->flag_N = CPU->flag_X = CPU->flag_C = res >> 8;
	CPU->flag_notZ = res & 0xFFFF;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(16)

// SUBI
_OP(0x0467)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_WORD;
	PC += 2;
	adr = CPU->A[7] - 2;
	CPU->A[7] = adr;
	PRE_IO
	READ_WORD_F(adr, dst)
	res = dst - src;
	CPU->flag_V = ((src ^ dst) & (res ^ dst)) >> 8;
	CPU->flag_N = CPU->flag_X = CPU->flag_C = res >> 8;
	CPU->flag_notZ = res & 0xFFFF;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(18)

// SUBI
_OP(0x0480)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_LONG;
	PC += 4;
	dst = (u32)CPU->D[(Opcode >> 0) & 7];
	res = dst - src;
	CPU->flag_notZ = res;
	CPU->flag_X = CPU->flag_C = ((src & res & 1) + (src >> 1) + (res >> 1)) >> 23;
	CPU->flag_V = ((src ^ dst) & (res ^ dst)) >> 24;
	CPU->flag_N = res >> 24;
	*(u32*)(&CPU->D[(Opcode >> 0) & 7]) = res;
}
RET(16)

// SUBI
_OP(0x0490)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_LONG;
	PC += 4;
	adr = CPU->A[(Opcode >> 0) & 7];
	PRE_IO
	READ_LONG_F(adr, dst)
	res = dst - src;
	CPU->flag_notZ = res;
	CPU->flag_X = CPU->flag_C = ((src & res & 1) + (src >> 1) + (res >> 1)) >> 23;
	CPU->flag_V = ((src ^ dst) & (res ^ dst)) >> 24;
	CPU->flag_N = res >> 24;
	WRITE_LONG_F(adr, res)
	POST_IO
}
RET(28)

// SUBI
_OP(0x0498)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_LONG;
	PC += 4;
	adr = CPU->A[(Opcode >> 0) & 7];
	CPU->A[(Opcode >> 0) & 7] += 4;
	PRE_IO
	READ_LONG_F(adr, dst)
	res = dst - src;
	CPU->flag_notZ = res;
	CPU->flag_X = CPU->flag_C = ((src & res & 1) + (src >> 1) + (res >> 1)) >> 23;
	CPU->flag_V = ((src ^ dst) & (res ^ dst)) >> 24;
	CPU->flag_N = res >> 24;
	WRITE_LONG_F(adr, res)
	POST_IO
}
RET(28)

// SUBI
_OP(0x04A0)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_LONG;
	PC += 4;
	adr = CPU->A[(Opcode >> 0) & 7] - 4;
	CPU->A[(Opcode >> 0) & 7] = adr;
	PRE_IO
	READ_LONG_F(adr, dst)
	res = dst - src;
	CPU->flag_notZ = res;
	CPU->flag_X = CPU->flag_C = ((src & res & 1) + (src >> 1) + (res >> 1)) >> 23;
	CPU->flag_V = ((src ^ dst) & (res ^ dst)) >> 24;
	CPU->flag_N = res >> 24;
	WRITE_LONG_F(adr, res)
	POST_IO
}
RET(30)

// SUBI
_OP(0x04A8)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_LONG;
	PC += 4;
	adr = CPU->A[(Opcode >> 0) & 7] + (s32)(s16)FETCH_WORD;
	PC += 2;
	PRE_IO
	READ_LONG_F(adr, dst)
	res = dst - src;
	CPU->flag_notZ = res;
	CPU->flag_X = CPU->flag_C = ((src & res & 1) + (src >> 1) + (res >> 1)) >> 23;
	CPU->flag_V = ((src ^ dst) & (res ^ dst)) >> 24;
	CPU->flag_N = res >> 24;
	WRITE_LONG_F(adr, res)
	POST_IO
}
RET(32)

// SUBI
_OP(0x04B0)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_LONG;
	PC += 4;
	adr = CPU->A[(Opcode >> 0) & 7];
	DECODE_EXT_WORD
	PRE_IO
	READ_LONG_F(adr, dst)
	res = dst - src;
	CPU->flag_notZ = res;
	CPU->flag_X = CPU->flag_C = ((src & res & 1) + (src >> 1) + (res >> 1)) >> 23;
	CPU->flag_V = ((src ^ dst) & (res ^ dst)) >> 24;
	CPU->flag_N = res >> 24;
	WRITE_LONG_F(adr, res)
	POST_IO
}
RET(34)

// SUBI
_OP(0x04B8)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_LONG;
	PC += 4;
	adr = (s32)(s16)FETCH_WORD;
	PC += 2;
	PRE_IO
	READ_LONG_F(adr, dst)
	res = dst - src;
	CPU->flag_notZ = res;
	CPU->flag_X = CPU->flag_C = ((src & res & 1) + (src >> 1) + (res >> 1)) >> 23;
	CPU->flag_V = ((src ^ dst) & (res ^ dst)) >> 24;
	CPU->flag_N = res >> 24;
	WRITE_LONG_F(adr, res)
	POST_IO
}
RET(32)

// SUBI
_OP(0x04B9)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_LONG;
	PC += 4;
	adr = (s32)FETCH_LONG;
	PC += 4;
	PRE_IO
	READ_LONG_F(adr, dst)
	res = dst - src;
	CPU->flag_notZ = res;
	CPU->flag_X = CPU->flag_C = ((src & res & 1) + (src >> 1) + (res >> 1)) >> 23;
	CPU->flag_V = ((src ^ dst) & (res ^ dst)) >> 24;
	CPU->flag_N = res >> 24;
	WRITE_LONG_F(adr, res)
	POST_IO
}
RET(36)

// SUBI
_OP(0x049F)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_LONG;
	PC += 4;
	adr = CPU->A[7];
	CPU->A[7] += 4;
	PRE_IO
	READ_LONG_F(adr, dst)
	res = dst - src;
	CPU->flag_notZ = res;
	CPU->flag_X = CPU->flag_C = ((src & res & 1) + (src >> 1) + (res >> 1)) >> 23;
	CPU->flag_V = ((src ^ dst) & (res ^ dst)) >> 24;
	CPU->flag_N = res >> 24;
	WRITE_LONG_F(adr, res)
	POST_IO
}
RET(28)

// SUBI
_OP(0x04A7)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_LONG;
	PC += 4;
	adr = CPU->A[7] - 4;
	CPU->A[7] = adr;
	PRE_IO
	READ_LONG_F(adr, dst)
	res = dst - src;
	CPU->flag_notZ = res;
	CPU->flag_X = CPU->flag_C = ((src & res & 1) + (src >> 1) + (res >> 1)) >> 23;
	CPU->flag_V = ((src ^ dst) & (res ^ dst)) >> 24;
	CPU->flag_N = res >> 24;
	WRITE_LONG_F(adr, res)
	POST_IO
}
RET(30)

// ADDI
_OP(0x0600)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_BYTE;
	PC += 2;
	dst = (u8)CPU->D[(Opcode >> 0) & 7];
	res = dst + src;
	CPU->flag_N = CPU->flag_X = CPU->flag_C = res;
	CPU->flag_V = (src ^ res) & (dst ^ res);
	CPU->flag_notZ = res & 0xFF;
	*(u8*)(&CPU->D[(Opcode >> 0) & 7]) = res;
}
RET(8)

// ADDI
_OP(0x0610)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_BYTE;
	PC += 2;
	adr = CPU->A[(Opcode >> 0) & 7];
	PRE_IO
	READ_BYTE_F(adr, dst)
	res = dst + src;
	CPU->flag_N = CPU->flag_X = CPU->flag_C = res;
	CPU->flag_V = (src ^ res) & (dst ^ res);
	CPU->flag_notZ = res & 0xFF;
	WRITE_BYTE_F(adr, res)
	POST_IO
}
RET(16)

// ADDI
_OP(0x0618)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_BYTE;
	PC += 2;
	adr = CPU->A[(Opcode >> 0) & 7];
	CPU->A[(Opcode >> 0) & 7] += 1;
	PRE_IO
	READ_BYTE_F(adr, dst)
	res = dst + src;
	CPU->flag_N = CPU->flag_X = CPU->flag_C = res;
	CPU->flag_V = (src ^ res) & (dst ^ res);
	CPU->flag_notZ = res & 0xFF;
	WRITE_BYTE_F(adr, res)
	POST_IO
}
RET(16)

// ADDI
_OP(0x0620)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_BYTE;
	PC += 2;
	adr = CPU->A[(Opcode >> 0) & 7] - 1;
	CPU->A[(Opcode >> 0) & 7] = adr;
	PRE_IO
	READ_BYTE_F(adr, dst)
	res = dst + src;
	CPU->flag_N = CPU->flag_X = CPU->flag_C = res;
	CPU->flag_V = (src ^ res) & (dst ^ res);
	CPU->flag_notZ = res & 0xFF;
	WRITE_BYTE_F(adr, res)
	POST_IO
}
RET(18)

// ADDI
_OP(0x0628)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_BYTE;
	PC += 2;
	adr = CPU->A[(Opcode >> 0) & 7] + (s32)(s16)FETCH_WORD;
	PC += 2;
	PRE_IO
	READ_BYTE_F(adr, dst)
	res = dst + src;
	CPU->flag_N = CPU->flag_X = CPU->flag_C = res;
	CPU->flag_V = (src ^ res) & (dst ^ res);
	CPU->flag_notZ = res & 0xFF;
	WRITE_BYTE_F(adr, res)
	POST_IO
}
RET(20)

// ADDI
_OP(0x0630)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_BYTE;
	PC += 2;
	adr = CPU->A[(Opcode >> 0) & 7];
	DECODE_EXT_WORD
	PRE_IO
	READ_BYTE_F(adr, dst)
	res = dst + src;
	CPU->flag_N = CPU->flag_X = CPU->flag_C = res;
	CPU->flag_V = (src ^ res) & (dst ^ res);
	CPU->flag_notZ = res & 0xFF;
	WRITE_BYTE_F(adr, res)
	POST_IO
}
RET(22)

// ADDI
_OP(0x0638)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_BYTE;
	PC += 2;
	adr = (s32)(s16)FETCH_WORD;
	PC += 2;
	PRE_IO
	READ_BYTE_F(adr, dst)
	res = dst + src;
	CPU->flag_N = CPU->flag_X = CPU->flag_C = res;
	CPU->flag_V = (src ^ res) & (dst ^ res);
	CPU->flag_notZ = res & 0xFF;
	WRITE_BYTE_F(adr, res)
	POST_IO
}
RET(20)

// ADDI
_OP(0x0639)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_BYTE;
	PC += 2;
	adr = (s32)FETCH_LONG;
	PC += 4;
	PRE_IO
	READ_BYTE_F(adr, dst)
	res = dst + src;
	CPU->flag_N = CPU->flag_X = CPU->flag_C = res;
	CPU->flag_V = (src ^ res) & (dst ^ res);
	CPU->flag_notZ = res & 0xFF;
	WRITE_BYTE_F(adr, res)
	POST_IO
}
RET(24)

// ADDI
_OP(0x061F)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_BYTE;
	PC += 2;
	adr = CPU->A[7];
	CPU->A[7] += 2;
	PRE_IO
	READ_BYTE_F(adr, dst)
	res = dst + src;
	CPU->flag_N = CPU->flag_X = CPU->flag_C = res;
	CPU->flag_V = (src ^ res) & (dst ^ res);
	CPU->flag_notZ = res & 0xFF;
	WRITE_BYTE_F(adr, res)
	POST_IO
}
RET(16)

// ADDI
_OP(0x0627)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_BYTE;
	PC += 2;
	adr = CPU->A[7] - 2;
	CPU->A[7] = adr;
	PRE_IO
	READ_BYTE_F(adr, dst)
	res = dst + src;
	CPU->flag_N = CPU->flag_X = CPU->flag_C = res;
	CPU->flag_V = (src ^ res) & (dst ^ res);
	CPU->flag_notZ = res & 0xFF;
	WRITE_BYTE_F(adr, res)
	POST_IO
}
RET(18)

// ADDI
_OP(0x0640)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_WORD;
	PC += 2;
	dst = (u16)CPU->D[(Opcode >> 0) & 7];
	res = dst + src;
	CPU->flag_V = ((src ^ res) & (dst ^ res)) >> 8;
	CPU->flag_N = CPU->flag_X = CPU->flag_C = res >> 8;
	CPU->flag_notZ = res & 0xFFFF;
	*(u16*)(&CPU->D[(Opcode >> 0) & 7]) = res;
}
RET(8)

// ADDI
_OP(0x0650)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_WORD;
	PC += 2;
	adr = CPU->A[(Opcode >> 0) & 7];
	PRE_IO
	READ_WORD_F(adr, dst)
	res = dst + src;
	CPU->flag_V = ((src ^ res) & (dst ^ res)) >> 8;
	CPU->flag_N = CPU->flag_X = CPU->flag_C = res >> 8;
	CPU->flag_notZ = res & 0xFFFF;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(16)

// ADDI
_OP(0x0658)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_WORD;
	PC += 2;
	adr = CPU->A[(Opcode >> 0) & 7];
	CPU->A[(Opcode >> 0) & 7] += 2;
	PRE_IO
	READ_WORD_F(adr, dst)
	res = dst + src;
	CPU->flag_V = ((src ^ res) & (dst ^ res)) >> 8;
	CPU->flag_N = CPU->flag_X = CPU->flag_C = res >> 8;
	CPU->flag_notZ = res & 0xFFFF;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(16)

// ADDI
_OP(0x0660)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_WORD;
	PC += 2;
	adr = CPU->A[(Opcode >> 0) & 7] - 2;
	CPU->A[(Opcode >> 0) & 7] = adr;
	PRE_IO
	READ_WORD_F(adr, dst)
	res = dst + src;
	CPU->flag_V = ((src ^ res) & (dst ^ res)) >> 8;
	CPU->flag_N = CPU->flag_X = CPU->flag_C = res >> 8;
	CPU->flag_notZ = res & 0xFFFF;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(18)

// ADDI
_OP(0x0668)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_WORD;
	PC += 2;
	adr = CPU->A[(Opcode >> 0) & 7] + (s32)(s16)FETCH_WORD;
	PC += 2;
	PRE_IO
	READ_WORD_F(adr, dst)
	res = dst + src;
	CPU->flag_V = ((src ^ res) & (dst ^ res)) >> 8;
	CPU->flag_N = CPU->flag_X = CPU->flag_C = res >> 8;
	CPU->flag_notZ = res & 0xFFFF;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(20)

// ADDI
_OP(0x0670)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_WORD;
	PC += 2;
	adr = CPU->A[(Opcode >> 0) & 7];
	DECODE_EXT_WORD
	PRE_IO
	READ_WORD_F(adr, dst)
	res = dst + src;
	CPU->flag_V = ((src ^ res) & (dst ^ res)) >> 8;
	CPU->flag_N = CPU->flag_X = CPU->flag_C = res >> 8;
	CPU->flag_notZ = res & 0xFFFF;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(22)

// ADDI
_OP(0x0678)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_WORD;
	PC += 2;
	adr = (s32)(s16)FETCH_WORD;
	PC += 2;
	PRE_IO
	READ_WORD_F(adr, dst)
	res = dst + src;
	CPU->flag_V = ((src ^ res) & (dst ^ res)) >> 8;
	CPU->flag_N = CPU->flag_X = CPU->flag_C = res >> 8;
	CPU->flag_notZ = res & 0xFFFF;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(20)

// ADDI
_OP(0x0679)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_WORD;
	PC += 2;
	adr = (s32)FETCH_LONG;
	PC += 4;
	PRE_IO
	READ_WORD_F(adr, dst)
	res = dst + src;
	CPU->flag_V = ((src ^ res) & (dst ^ res)) >> 8;
	CPU->flag_N = CPU->flag_X = CPU->flag_C = res >> 8;
	CPU->flag_notZ = res & 0xFFFF;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(24)

// ADDI
_OP(0x065F)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_WORD;
	PC += 2;
	adr = CPU->A[7];
	CPU->A[7] += 2;
	PRE_IO
	READ_WORD_F(adr, dst)
	res = dst + src;
	CPU->flag_V = ((src ^ res) & (dst ^ res)) >> 8;
	CPU->flag_N = CPU->flag_X = CPU->flag_C = res >> 8;
	CPU->flag_notZ = res & 0xFFFF;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(16)

// ADDI
_OP(0x0667)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_WORD;
	PC += 2;
	adr = CPU->A[7] - 2;
	CPU->A[7] = adr;
	PRE_IO
	READ_WORD_F(adr, dst)
	res = dst + src;
	CPU->flag_V = ((src ^ res) & (dst ^ res)) >> 8;
	CPU->flag_N = CPU->flag_X = CPU->flag_C = res >> 8;
	CPU->flag_notZ = res & 0xFFFF;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(18)

// ADDI
_OP(0x0680)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_LONG;
	PC += 4;
	dst = (u32)CPU->D[(Opcode >> 0) & 7];
	res = dst + src;
	CPU->flag_notZ = res;
	CPU->flag_X = CPU->flag_C = ((src & dst & 1) + (src >> 1) + (dst >> 1)) >> 23;
	CPU->flag_V = ((src ^ res) & (dst ^ res)) >> 24;
	CPU->flag_N = res >> 24;
	*(u32*)(&CPU->D[(Opcode >> 0) & 7]) = res;
}
RET(16)

// ADDI
_OP(0x0690)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_LONG;
	PC += 4;
	adr = CPU->A[(Opcode >> 0) & 7];
	PRE_IO
	READ_LONG_F(adr, dst)
	res = dst + src;
	CPU->flag_notZ = res;
	CPU->flag_X = CPU->flag_C = ((src & dst & 1) + (src >> 1) + (dst >> 1)) >> 23;
	CPU->flag_V = ((src ^ res) & (dst ^ res)) >> 24;
	CPU->flag_N = res >> 24;
	WRITE_LONG_F(adr, res)
	POST_IO
}
RET(28)

// ADDI
_OP(0x0698)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_LONG;
	PC += 4;
	adr = CPU->A[(Opcode >> 0) & 7];
	CPU->A[(Opcode >> 0) & 7] += 4;
	PRE_IO
	READ_LONG_F(adr, dst)
	res = dst + src;
	CPU->flag_notZ = res;
	CPU->flag_X = CPU->flag_C = ((src & dst & 1) + (src >> 1) + (dst >> 1)) >> 23;
	CPU->flag_V = ((src ^ res) & (dst ^ res)) >> 24;
	CPU->flag_N = res >> 24;
	WRITE_LONG_F(adr, res)
	POST_IO
}
RET(28)

// ADDI
_OP(0x06A0)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_LONG;
	PC += 4;
	adr = CPU->A[(Opcode >> 0) & 7] - 4;
	CPU->A[(Opcode >> 0) & 7] = adr;
	PRE_IO
	READ_LONG_F(adr, dst)
	res = dst + src;
	CPU->flag_notZ = res;
	CPU->flag_X = CPU->flag_C = ((src & dst & 1) + (src >> 1) + (dst >> 1)) >> 23;
	CPU->flag_V = ((src ^ res) & (dst ^ res)) >> 24;
	CPU->flag_N = res >> 24;
	WRITE_LONG_F(adr, res)
	POST_IO
}
RET(30)

// ADDI
_OP(0x06A8)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_LONG;
	PC += 4;
	adr = CPU->A[(Opcode >> 0) & 7] + (s32)(s16)FETCH_WORD;
	PC += 2;
	PRE_IO
	READ_LONG_F(adr, dst)
	res = dst + src;
	CPU->flag_notZ = res;
	CPU->flag_X = CPU->flag_C = ((src & dst & 1) + (src >> 1) + (dst >> 1)) >> 23;
	CPU->flag_V = ((src ^ res) & (dst ^ res)) >> 24;
	CPU->flag_N = res >> 24;
	WRITE_LONG_F(adr, res)
	POST_IO
}
RET(32)

// ADDI
_OP(0x06B0)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_LONG;
	PC += 4;
	adr = CPU->A[(Opcode >> 0) & 7];
	DECODE_EXT_WORD
	PRE_IO
	READ_LONG_F(adr, dst)
	res = dst + src;
	CPU->flag_notZ = res;
	CPU->flag_X = CPU->flag_C = ((src & dst & 1) + (src >> 1) + (dst >> 1)) >> 23;
	CPU->flag_V = ((src ^ res) & (dst ^ res)) >> 24;
	CPU->flag_N = res >> 24;
	WRITE_LONG_F(adr, res)
	POST_IO
}
RET(34)

// ADDI
_OP(0x06B8)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_LONG;
	PC += 4;
	adr = (s32)(s16)FETCH_WORD;
	PC += 2;
	PRE_IO
	READ_LONG_F(adr, dst)
	res = dst + src;
	CPU->flag_notZ = res;
	CPU->flag_X = CPU->flag_C = ((src & dst & 1) + (src >> 1) + (dst >> 1)) >> 23;
	CPU->flag_V = ((src ^ res) & (dst ^ res)) >> 24;
	CPU->flag_N = res >> 24;
	WRITE_LONG_F(adr, res)
	POST_IO
}
RET(32)

// ADDI
_OP(0x06B9)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_LONG;
	PC += 4;
	adr = (s32)FETCH_LONG;
	PC += 4;
	PRE_IO
	READ_LONG_F(adr, dst)
	res = dst + src;
	CPU->flag_notZ = res;
	CPU->flag_X = CPU->flag_C = ((src & dst & 1) + (src >> 1) + (dst >> 1)) >> 23;
	CPU->flag_V = ((src ^ res) & (dst ^ res)) >> 24;
	CPU->flag_N = res >> 24;
	WRITE_LONG_F(adr, res)
	POST_IO
}
RET(36)

// ADDI
_OP(0x069F)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_LONG;
	PC += 4;
	adr = CPU->A[7];
	CPU->A[7] += 4;
	PRE_IO
	READ_LONG_F(adr, dst)
	res = dst + src;
	CPU->flag_notZ = res;
	CPU->flag_X = CPU->flag_C = ((src & dst & 1) + (src >> 1) + (dst >> 1)) >> 23;
	CPU->flag_V = ((src ^ res) & (dst ^ res)) >> 24;
	CPU->flag_N = res >> 24;
	WRITE_LONG_F(adr, res)
	POST_IO
}
RET(28)

// ADDI
_OP(0x06A7)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_LONG;
	PC += 4;
	adr = CPU->A[7] - 4;
	CPU->A[7] = adr;
	PRE_IO
	READ_LONG_F(adr, dst)
	res = dst + src;
	CPU->flag_notZ = res;
	CPU->flag_X = CPU->flag_C = ((src & dst & 1) + (src >> 1) + (dst >> 1)) >> 23;
	CPU->flag_V = ((src ^ res) & (dst ^ res)) >> 24;
	CPU->flag_N = res >> 24;
	WRITE_LONG_F(adr, res)
	POST_IO
}
RET(30)

// CMPI
_OP(0x0C00)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_BYTE;
	PC += 2;
	dst = (u8)CPU->D[(Opcode >> 0) & 7];
	res = dst - src;
	CPU->flag_N = CPU->flag_C = res;
	CPU->flag_V = (src ^ dst) & (res ^ dst);
	CPU->flag_notZ = res & 0xFF;
}
RET(8)

// CMPI
_OP(0x0C10)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_BYTE;
	PC += 2;
	adr = CPU->A[(Opcode >> 0) & 7];
	PRE_IO
	READ_BYTE_F(adr, dst)
	res = dst - src;
	CPU->flag_N = CPU->flag_C = res;
	CPU->flag_V = (src ^ dst) & (res ^ dst);
	CPU->flag_notZ = res & 0xFF;
	POST_IO
}
RET(12)

// CMPI
_OP(0x0C18)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_BYTE;
	PC += 2;
	adr = CPU->A[(Opcode >> 0) & 7];
	CPU->A[(Opcode >> 0) & 7] += 1;
	PRE_IO
	READ_BYTE_F(adr, dst)
	res = dst - src;
	CPU->flag_N = CPU->flag_C = res;
	CPU->flag_V = (src ^ dst) & (res ^ dst);
	CPU->flag_notZ = res & 0xFF;
	POST_IO
}
RET(12)

// CMPI
_OP(0x0C20)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_BYTE;
	PC += 2;
	adr = CPU->A[(Opcode >> 0) & 7] - 1;
	CPU->A[(Opcode >> 0) & 7] = adr;
	PRE_IO
	READ_BYTE_F(adr, dst)
	res = dst - src;
	CPU->flag_N = CPU->flag_C = res;
	CPU->flag_V = (src ^ dst) & (res ^ dst);
	CPU->flag_notZ = res & 0xFF;
	POST_IO
}
RET(14)

// CMPI
_OP(0x0C28)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_BYTE;
	PC += 2;
	adr = CPU->A[(Opcode >> 0) & 7] + (s32)(s16)FETCH_WORD;
	PC += 2;
	PRE_IO
	READ_BYTE_F(adr, dst)
	res = dst - src;
	CPU->flag_N = CPU->flag_C = res;
	CPU->flag_V = (src ^ dst) & (res ^ dst);
	CPU->flag_notZ = res & 0xFF;
	POST_IO
}
RET(16)

// CMPI
_OP(0x0C30)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_BYTE;
	PC += 2;
	adr = CPU->A[(Opcode >> 0) & 7];
	DECODE_EXT_WORD
	PRE_IO
	READ_BYTE_F(adr, dst)
	res = dst - src;
	CPU->flag_N = CPU->flag_C = res;
	CPU->flag_V = (src ^ dst) & (res ^ dst);
	CPU->flag_notZ = res & 0xFF;
	POST_IO
}
RET(18)

// CMPI
_OP(0x0C38)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_BYTE;
	PC += 2;
	adr = (s32)(s16)FETCH_WORD;
	PC += 2;
	PRE_IO
	READ_BYTE_F(adr, dst)
	res = dst - src;
	CPU->flag_N = CPU->flag_C = res;
	CPU->flag_V = (src ^ dst) & (res ^ dst);
	CPU->flag_notZ = res & 0xFF;
	POST_IO
}
RET(16)

// CMPI
_OP(0x0C39)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_BYTE;
	PC += 2;
	adr = (s32)FETCH_LONG;
	PC += 4;
	PRE_IO
	READ_BYTE_F(adr, dst)
	res = dst - src;
	CPU->flag_N = CPU->flag_C = res;
	CPU->flag_V = (src ^ dst) & (res ^ dst);
	CPU->flag_notZ = res & 0xFF;
	POST_IO
}
RET(20)

// CMPI
_OP(0x0C1F)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_BYTE;
	PC += 2;
	adr = CPU->A[7];
	CPU->A[7] += 2;
	PRE_IO
	READ_BYTE_F(adr, dst)
	res = dst - src;
	CPU->flag_N = CPU->flag_C = res;
	CPU->flag_V = (src ^ dst) & (res ^ dst);
	CPU->flag_notZ = res & 0xFF;
	POST_IO
}
RET(12)

// CMPI
_OP(0x0C27)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_BYTE;
	PC += 2;
	adr = CPU->A[7] - 2;
	CPU->A[7] = adr;
	PRE_IO
	READ_BYTE_F(adr, dst)
	res = dst - src;
	CPU->flag_N = CPU->flag_C = res;
	CPU->flag_V = (src ^ dst) & (res ^ dst);
	CPU->flag_notZ = res & 0xFF;
	POST_IO
}
RET(14)

// CMPI
_OP(0x0C40)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_WORD;
	PC += 2;
	dst = (u16)CPU->D[(Opcode >> 0) & 7];
	res = dst - src;
	CPU->flag_V = ((src ^ dst) & (res ^ dst)) >> 8;
	CPU->flag_N = CPU->flag_C = res >> 8;
	CPU->flag_notZ = res & 0xFFFF;
}
RET(8)

// CMPI
_OP(0x0C50)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_WORD;
	PC += 2;
	adr = CPU->A[(Opcode >> 0) & 7];
	PRE_IO
	READ_WORD_F(adr, dst)
	res = dst - src;
	CPU->flag_V = ((src ^ dst) & (res ^ dst)) >> 8;
	CPU->flag_N = CPU->flag_C = res >> 8;
	CPU->flag_notZ = res & 0xFFFF;
	POST_IO
}
RET(12)

// CMPI
_OP(0x0C58)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_WORD;
	PC += 2;
	adr = CPU->A[(Opcode >> 0) & 7];
	CPU->A[(Opcode >> 0) & 7] += 2;
	PRE_IO
	READ_WORD_F(adr, dst)
	res = dst - src;
	CPU->flag_V = ((src ^ dst) & (res ^ dst)) >> 8;
	CPU->flag_N = CPU->flag_C = res >> 8;
	CPU->flag_notZ = res & 0xFFFF;
	POST_IO
}
RET(12)

// CMPI
_OP(0x0C60)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_WORD;
	PC += 2;
	adr = CPU->A[(Opcode >> 0) & 7] - 2;
	CPU->A[(Opcode >> 0) & 7] = adr;
	PRE_IO
	READ_WORD_F(adr, dst)
	res = dst - src;
	CPU->flag_V = ((src ^ dst) & (res ^ dst)) >> 8;
	CPU->flag_N = CPU->flag_C = res >> 8;
	CPU->flag_notZ = res & 0xFFFF;
	POST_IO
}
RET(14)

// CMPI
_OP(0x0C68)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_WORD;
	PC += 2;
	adr = CPU->A[(Opcode >> 0) & 7] + (s32)(s16)FETCH_WORD;
	PC += 2;
	PRE_IO
	READ_WORD_F(adr, dst)
	res = dst - src;
	CPU->flag_V = ((src ^ dst) & (res ^ dst)) >> 8;
	CPU->flag_N = CPU->flag_C = res >> 8;
	CPU->flag_notZ = res & 0xFFFF;
	POST_IO
}
RET(16)

// CMPI
_OP(0x0C70)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_WORD;
	PC += 2;
	adr = CPU->A[(Opcode >> 0) & 7];
	DECODE_EXT_WORD
	PRE_IO
	READ_WORD_F(adr, dst)
	res = dst - src;
	CPU->flag_V = ((src ^ dst) & (res ^ dst)) >> 8;
	CPU->flag_N = CPU->flag_C = res >> 8;
	CPU->flag_notZ = res & 0xFFFF;
	POST_IO
}
RET(18)

// CMPI
_OP(0x0C78)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_WORD;
	PC += 2;
	adr = (s32)(s16)FETCH_WORD;
	PC += 2;
	PRE_IO
	READ_WORD_F(adr, dst)
	res = dst - src;
	CPU->flag_V = ((src ^ dst) & (res ^ dst)) >> 8;
	CPU->flag_N = CPU->flag_C = res >> 8;
	CPU->flag_notZ = res & 0xFFFF;
	POST_IO
}
RET(16)

// CMPI
_OP(0x0C79)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_WORD;
	PC += 2;
	adr = (s32)FETCH_LONG;
	PC += 4;
	PRE_IO
	READ_WORD_F(adr, dst)
	res = dst - src;
	CPU->flag_V = ((src ^ dst) & (res ^ dst)) >> 8;
	CPU->flag_N = CPU->flag_C = res >> 8;
	CPU->flag_notZ = res & 0xFFFF;
	POST_IO
}
RET(20)

// CMPI
_OP(0x0C5F)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_WORD;
	PC += 2;
	adr = CPU->A[7];
	CPU->A[7] += 2;
	PRE_IO
	READ_WORD_F(adr, dst)
	res = dst - src;
	CPU->flag_V = ((src ^ dst) & (res ^ dst)) >> 8;
	CPU->flag_N = CPU->flag_C = res >> 8;
	CPU->flag_notZ = res & 0xFFFF;
	POST_IO
}
RET(12)

// CMPI
_OP(0x0C67)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_WORD;
	PC += 2;
	adr = CPU->A[7] - 2;
	CPU->A[7] = adr;
	PRE_IO
	READ_WORD_F(adr, dst)
	res = dst - src;
	CPU->flag_V = ((src ^ dst) & (res ^ dst)) >> 8;
	CPU->flag_N = CPU->flag_C = res >> 8;
	CPU->flag_notZ = res & 0xFFFF;
	POST_IO
}
RET(14)

// CMPI
_OP(0x0C80)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_LONG;
	PC += 4;
	dst = (u32)CPU->D[(Opcode >> 0) & 7];
	res = dst - src;
	CPU->flag_notZ = res;
	CPU->flag_C = ((src & res & 1) + (src >> 1) + (res >> 1)) >> 23;
	CPU->flag_V = ((src ^ dst) & (res ^ dst)) >> 24;
	CPU->flag_N = res >> 24;
}
RET(14)

// CMPI
_OP(0x0C90)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_LONG;
	PC += 4;
	adr = CPU->A[(Opcode >> 0) & 7];
	PRE_IO
	READ_LONG_F(adr, dst)
	res = dst - src;
	CPU->flag_notZ = res;
	CPU->flag_C = ((src & res & 1) + (src >> 1) + (res >> 1)) >> 23;
	CPU->flag_V = ((src ^ dst) & (res ^ dst)) >> 24;
	CPU->flag_N = res >> 24;
	POST_IO
}
RET(20)

// CMPI
_OP(0x0C98)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_LONG;
	PC += 4;
	adr = CPU->A[(Opcode >> 0) & 7];
	CPU->A[(Opcode >> 0) & 7] += 4;
	PRE_IO
	READ_LONG_F(adr, dst)
	res = dst - src;
	CPU->flag_notZ = res;
	CPU->flag_C = ((src & res & 1) + (src >> 1) + (res >> 1)) >> 23;
	CPU->flag_V = ((src ^ dst) & (res ^ dst)) >> 24;
	CPU->flag_N = res >> 24;
	POST_IO
}
RET(20)

// CMPI
_OP(0x0CA0)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_LONG;
	PC += 4;
	adr = CPU->A[(Opcode >> 0) & 7] - 4;
	CPU->A[(Opcode >> 0) & 7] = adr;
	PRE_IO
	READ_LONG_F(adr, dst)
	res = dst - src;
	CPU->flag_notZ = res;
	CPU->flag_C = ((src & res & 1) + (src >> 1) + (res >> 1)) >> 23;
	CPU->flag_V = ((src ^ dst) & (res ^ dst)) >> 24;
	CPU->flag_N = res >> 24;
	POST_IO
}
RET(22)

// CMPI
_OP(0x0CA8)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_LONG;
	PC += 4;
	adr = CPU->A[(Opcode >> 0) & 7] + (s32)(s16)FETCH_WORD;
	PC += 2;
	PRE_IO
	READ_LONG_F(adr, dst)
	res = dst - src;
	CPU->flag_notZ = res;
	CPU->flag_C = ((src & res & 1) + (src >> 1) + (res >> 1)) >> 23;
	CPU->flag_V = ((src ^ dst) & (res ^ dst)) >> 24;
	CPU->flag_N = res >> 24;
	POST_IO
}
RET(24)

// CMPI
_OP(0x0CB0)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_LONG;
	PC += 4;
	adr = CPU->A[(Opcode >> 0) & 7];
	DECODE_EXT_WORD
	PRE_IO
	READ_LONG_F(adr, dst)
	res = dst - src;
	CPU->flag_notZ = res;
	CPU->flag_C = ((src & res & 1) + (src >> 1) + (res >> 1)) >> 23;
	CPU->flag_V = ((src ^ dst) & (res ^ dst)) >> 24;
	CPU->flag_N = res >> 24;
	POST_IO
}
RET(26)

// CMPI
_OP(0x0CB8)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_LONG;
	PC += 4;
	adr = (s32)(s16)FETCH_WORD;
	PC += 2;
	PRE_IO
	READ_LONG_F(adr, dst)
	res = dst - src;
	CPU->flag_notZ = res;
	CPU->flag_C = ((src & res & 1) + (src >> 1) + (res >> 1)) >> 23;
	CPU->flag_V = ((src ^ dst) & (res ^ dst)) >> 24;
	CPU->flag_N = res >> 24;
	POST_IO
}
RET(24)

// CMPI
_OP(0x0CB9)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_LONG;
	PC += 4;
	adr = (s32)FETCH_LONG;
	PC += 4;
	PRE_IO
	READ_LONG_F(adr, dst)
	res = dst - src;
	CPU->flag_notZ = res;
	CPU->flag_C = ((src & res & 1) + (src >> 1) + (res >> 1)) >> 23;
	CPU->flag_V = ((src ^ dst) & (res ^ dst)) >> 24;
	CPU->flag_N = res >> 24;
	POST_IO
}
RET(28)

// CMPI
_OP(0x0C9F)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_LONG;
	PC += 4;
	adr = CPU->A[7];
	CPU->A[7] += 4;
	PRE_IO
	READ_LONG_F(adr, dst)
	res = dst - src;
	CPU->flag_notZ = res;
	CPU->flag_C = ((src & res & 1) + (src >> 1) + (res >> 1)) >> 23;
	CPU->flag_V = ((src ^ dst) & (res ^ dst)) >> 24;
	CPU->flag_N = res >> 24;
	POST_IO
}
RET(20)

// CMPI
_OP(0x0CA7)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_LONG;
	PC += 4;
	adr = CPU->A[7] - 4;
	CPU->A[7] = adr;
	PRE_IO
	READ_LONG_F(adr, dst)
	res = dst - src;
	CPU->flag_notZ = res;
	CPU->flag_C = ((src & res & 1) + (src >> 1) + (res >> 1)) >> 23;
	CPU->flag_V = ((src ^ dst) & (res ^ dst)) >> 24;
	CPU->flag_N = res >> 24;
	POST_IO
}
RET(22)

// BTSTn
_OP(0x0800)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_BYTE;
	PC += 2;
	src = 1 << (src & 31);
	res = (u32)CPU->D[(Opcode >> 0) & 7];
	CPU->flag_notZ = res & src;
}
RET(10)

// BTSTn
_OP(0x0810)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_BYTE;
	PC += 2;
	src = 1 << (src & 7);
	adr = CPU->A[(Opcode >> 0) & 7];
	PRE_IO
	READ_BYTE_F(adr, res)
	CPU->flag_notZ = res & src;
	POST_IO
}
RET(12)

// BTSTn
_OP(0x0818)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_BYTE;
	PC += 2;
	src = 1 << (src & 7);
	adr = CPU->A[(Opcode >> 0) & 7];
	CPU->A[(Opcode >> 0) & 7] += 1;
	PRE_IO
	READ_BYTE_F(adr, res)
	CPU->flag_notZ = res & src;
	POST_IO
}
RET(12)

// BTSTn
_OP(0x0820)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_BYTE;
	PC += 2;
	src = 1 << (src & 7);
	adr = CPU->A[(Opcode >> 0) & 7] - 1;
	CPU->A[(Opcode >> 0) & 7] = adr;
	PRE_IO
	READ_BYTE_F(adr, res)
	CPU->flag_notZ = res & src;
	POST_IO
}
RET(14)

// BTSTn
_OP(0x0828)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_BYTE;
	PC += 2;
	src = 1 << (src & 7);
	adr = CPU->A[(Opcode >> 0) & 7] + (s32)(s16)FETCH_WORD;
	PC += 2;
	PRE_IO
	READ_BYTE_F(adr, res)
	CPU->flag_notZ = res & src;
	POST_IO
}
RET(16)

// BTSTn
_OP(0x0830)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_BYTE;
	PC += 2;
	src = 1 << (src & 7);
	adr = CPU->A[(Opcode >> 0) & 7];
	DECODE_EXT_WORD
	PRE_IO
	READ_BYTE_F(adr, res)
	CPU->flag_notZ = res & src;
	POST_IO
}
RET(18)

// BTSTn
_OP(0x0838)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_BYTE;
	PC += 2;
	src = 1 << (src & 7);
	adr = (s32)(s16)FETCH_WORD;
	PC += 2;
	PRE_IO
	READ_BYTE_F(adr, res)
	CPU->flag_notZ = res & src;
	POST_IO
}
RET(16)

// BTSTn
_OP(0x0839)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_BYTE;
	PC += 2;
	src = 1 << (src & 7);
	adr = (s32)FETCH_LONG;
	PC += 4;
	PRE_IO
	READ_BYTE_F(adr, res)
	CPU->flag_notZ = res & src;
	POST_IO
}
RET(20)

// BTSTn
_OP(0x083A)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_BYTE;
	PC += 2;
	src = 1 << (src & 7);
	adr = (PC - CPU->BasePC) + (s32)(s16)FETCH_WORD;
	PC += 2;
	PRE_IO
	READ_BYTE_F(adr, res)
	CPU->flag_notZ = res & src;
	POST_IO
}
RET(16)

// BTSTn
_OP(0x083B)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_BYTE;
	PC += 2;
	src = 1 << (src & 7);
	adr = PC - CPU->BasePC;
	DECODE_EXT_WORD
	PRE_IO
	READ_BYTE_F(adr, res)
	CPU->flag_notZ = res & src;
	POST_IO
}
RET(18)

// BTSTn
_OP(0x081F)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_BYTE;
	PC += 2;
	src = 1 << (src & 7);
	adr = CPU->A[7];
	CPU->A[7] += 2;
	PRE_IO
	READ_BYTE_F(adr, res)
	CPU->flag_notZ = res & src;
	POST_IO
}
RET(12)

// BTSTn
_OP(0x0827)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_BYTE;
	PC += 2;
	src = 1 << (src & 7);
	adr = CPU->A[7] - 2;
	CPU->A[7] = adr;
	PRE_IO
	READ_BYTE_F(adr, res)
	CPU->flag_notZ = res & src;
	POST_IO
}
RET(14)

// BCHGn
_OP(0x0840)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_BYTE;
	PC += 2;
	src = 1 << (src & 31);
	res = (u32)CPU->D[(Opcode >> 0) & 7];
	CPU->flag_notZ = res & src;
	res ^= src;
	*(u32*)(&CPU->D[(Opcode >> 0) & 7]) = res;
}
RET(12)

// BCHGn
_OP(0x0850)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_BYTE;
	PC += 2;
	src = 1 << (src & 7);
	adr = CPU->A[(Opcode >> 0) & 7];
	PRE_IO
	READ_BYTE_F(adr, res)
	CPU->flag_notZ = res & src;
	res ^= src;
	WRITE_BYTE_F(adr, res)
	POST_IO
}
RET(16)

// BCHGn
_OP(0x0858)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_BYTE;
	PC += 2;
	src = 1 << (src & 7);
	adr = CPU->A[(Opcode >> 0) & 7];
	CPU->A[(Opcode >> 0) & 7] += 1;
	PRE_IO
	READ_BYTE_F(adr, res)
	CPU->flag_notZ = res & src;
	res ^= src;
	WRITE_BYTE_F(adr, res)
	POST_IO
}
RET(16)

// BCHGn
_OP(0x0860)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_BYTE;
	PC += 2;
	src = 1 << (src & 7);
	adr = CPU->A[(Opcode >> 0) & 7] - 1;
	CPU->A[(Opcode >> 0) & 7] = adr;
	PRE_IO
	READ_BYTE_F(adr, res)
	CPU->flag_notZ = res & src;
	res ^= src;
	WRITE_BYTE_F(adr, res)
	POST_IO
}
RET(18)

// BCHGn
_OP(0x0868)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_BYTE;
	PC += 2;
	src = 1 << (src & 7);
	adr = CPU->A[(Opcode >> 0) & 7] + (s32)(s16)FETCH_WORD;
	PC += 2;
	PRE_IO
	READ_BYTE_F(adr, res)
	CPU->flag_notZ = res & src;
	res ^= src;
	WRITE_BYTE_F(adr, res)
	POST_IO
}
RET(20)

// BCHGn
_OP(0x0870)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_BYTE;
	PC += 2;
	src = 1 << (src & 7);
	adr = CPU->A[(Opcode >> 0) & 7];
	DECODE_EXT_WORD
	PRE_IO
	READ_BYTE_F(adr, res)
	CPU->flag_notZ = res & src;
	res ^= src;
	WRITE_BYTE_F(adr, res)
	POST_IO
}
RET(22)

// BCHGn
_OP(0x0878)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_BYTE;
	PC += 2;
	src = 1 << (src & 7);
	adr = (s32)(s16)FETCH_WORD;
	PC += 2;
	PRE_IO
	READ_BYTE_F(adr, res)
	CPU->flag_notZ = res & src;
	res ^= src;
	WRITE_BYTE_F(adr, res)
	POST_IO
}
RET(20)

// BCHGn
_OP(0x0879)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_BYTE;
	PC += 2;
	src = 1 << (src & 7);
	adr = (s32)FETCH_LONG;
	PC += 4;
	PRE_IO
	READ_BYTE_F(adr, res)
	CPU->flag_notZ = res & src;
	res ^= src;
	WRITE_BYTE_F(adr, res)
	POST_IO
}
RET(24)

// BCHGn
_OP(0x085F)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_BYTE;
	PC += 2;
	src = 1 << (src & 7);
	adr = CPU->A[7];
	CPU->A[7] += 2;
	PRE_IO
	READ_BYTE_F(adr, res)
	CPU->flag_notZ = res & src;
	res ^= src;
	WRITE_BYTE_F(adr, res)
	POST_IO
}
RET(16)

// BCHGn
_OP(0x0867)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_BYTE;
	PC += 2;
	src = 1 << (src & 7);
	adr = CPU->A[7] - 2;
	CPU->A[7] = adr;
	PRE_IO
	READ_BYTE_F(adr, res)
	CPU->flag_notZ = res & src;
	res ^= src;
	WRITE_BYTE_F(adr, res)
	POST_IO
}
RET(18)

// BCLRn
_OP(0x0880)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_BYTE;
	PC += 2;
	src = 1 << (src & 31);
	res = (u32)CPU->D[(Opcode >> 0) & 7];
	CPU->flag_notZ = res & src;
	res &= ~src;
	*(u32*)(&CPU->D[(Opcode >> 0) & 7]) = res;
}
RET(14)

// BCLRn
_OP(0x0890)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_BYTE;
	PC += 2;
	src = 1 << (src & 7);
	adr = CPU->A[(Opcode >> 0) & 7];
	PRE_IO
	READ_BYTE_F(adr, res)
	CPU->flag_notZ = res & src;
	res &= ~src;
	WRITE_BYTE_F(adr, res)
	POST_IO
}
RET(16)

// BCLRn
_OP(0x0898)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_BYTE;
	PC += 2;
	src = 1 << (src & 7);
	adr = CPU->A[(Opcode >> 0) & 7];
	CPU->A[(Opcode >> 0) & 7] += 1;
	PRE_IO
	READ_BYTE_F(adr, res)
	CPU->flag_notZ = res & src;
	res &= ~src;
	WRITE_BYTE_F(adr, res)
	POST_IO
}
RET(16)

// BCLRn
_OP(0x08A0)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_BYTE;
	PC += 2;
	src = 1 << (src & 7);
	adr = CPU->A[(Opcode >> 0) & 7] - 1;
	CPU->A[(Opcode >> 0) & 7] = adr;
	PRE_IO
	READ_BYTE_F(adr, res)
	CPU->flag_notZ = res & src;
	res &= ~src;
	WRITE_BYTE_F(adr, res)
	POST_IO
}
RET(18)

// BCLRn
_OP(0x08A8)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_BYTE;
	PC += 2;
	src = 1 << (src & 7);
	adr = CPU->A[(Opcode >> 0) & 7] + (s32)(s16)FETCH_WORD;
	PC += 2;
	PRE_IO
	READ_BYTE_F(adr, res)
	CPU->flag_notZ = res & src;
	res &= ~src;
	WRITE_BYTE_F(adr, res)
	POST_IO
}
RET(20)

// BCLRn
_OP(0x08B0)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_BYTE;
	PC += 2;
	src = 1 << (src & 7);
	adr = CPU->A[(Opcode >> 0) & 7];
	DECODE_EXT_WORD
	PRE_IO
	READ_BYTE_F(adr, res)
	CPU->flag_notZ = res & src;
	res &= ~src;
	WRITE_BYTE_F(adr, res)
	POST_IO
}
RET(22)

// BCLRn
_OP(0x08B8)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_BYTE;
	PC += 2;
	src = 1 << (src & 7);
	adr = (s32)(s16)FETCH_WORD;
	PC += 2;
	PRE_IO
	READ_BYTE_F(adr, res)
	CPU->flag_notZ = res & src;
	res &= ~src;
	WRITE_BYTE_F(adr, res)
	POST_IO
}
RET(20)

// BCLRn
_OP(0x08B9)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_BYTE;
	PC += 2;
	src = 1 << (src & 7);
	adr = (s32)FETCH_LONG;
	PC += 4;
	PRE_IO
	READ_BYTE_F(adr, res)
	CPU->flag_notZ = res & src;
	res &= ~src;
	WRITE_BYTE_F(adr, res)
	POST_IO
}
RET(24)

// BCLRn
_OP(0x089F)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_BYTE;
	PC += 2;
	src = 1 << (src & 7);
	adr = CPU->A[7];
	CPU->A[7] += 2;
	PRE_IO
	READ_BYTE_F(adr, res)
	CPU->flag_notZ = res & src;
	res &= ~src;
	WRITE_BYTE_F(adr, res)
	POST_IO
}
RET(16)

// BCLRn
_OP(0x08A7)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_BYTE;
	PC += 2;
	src = 1 << (src & 7);
	adr = CPU->A[7] - 2;
	CPU->A[7] = adr;
	PRE_IO
	READ_BYTE_F(adr, res)
	CPU->flag_notZ = res & src;
	res &= ~src;
	WRITE_BYTE_F(adr, res)
	POST_IO
}
RET(18)

// BSETn
_OP(0x08C0)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_BYTE;
	PC += 2;
	src = 1 << (src & 31);
	res = (u32)CPU->D[(Opcode >> 0) & 7];
	CPU->flag_notZ = res & src;
	res |= src;
	*(u32*)(&CPU->D[(Opcode >> 0) & 7]) = res;
}
RET(12)

// BSETn
_OP(0x08D0)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_BYTE;
	PC += 2;
	src = 1 << (src & 7);
	adr = CPU->A[(Opcode >> 0) & 7];
	PRE_IO
	READ_BYTE_F(adr, res)
	CPU->flag_notZ = res & src;
	res |= src;
	WRITE_BYTE_F(adr, res)
	POST_IO
}
RET(16)

// BSETn
_OP(0x08D8)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_BYTE;
	PC += 2;
	src = 1 << (src & 7);
	adr = CPU->A[(Opcode >> 0) & 7];
	CPU->A[(Opcode >> 0) & 7] += 1;
	PRE_IO
	READ_BYTE_F(adr, res)
	CPU->flag_notZ = res & src;
	res |= src;
	WRITE_BYTE_F(adr, res)
	POST_IO
}
RET(16)

// BSETn
_OP(0x08E0)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_BYTE;
	PC += 2;
	src = 1 << (src & 7);
	adr = CPU->A[(Opcode >> 0) & 7] - 1;
	CPU->A[(Opcode >> 0) & 7] = adr;
	PRE_IO
	READ_BYTE_F(adr, res)
	CPU->flag_notZ = res & src;
	res |= src;
	WRITE_BYTE_F(adr, res)
	POST_IO
}
RET(18)

// BSETn
_OP(0x08E8)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_BYTE;
	PC += 2;
	src = 1 << (src & 7);
	adr = CPU->A[(Opcode >> 0) & 7] + (s32)(s16)FETCH_WORD;
	PC += 2;
	PRE_IO
	READ_BYTE_F(adr, res)
	CPU->flag_notZ = res & src;
	res |= src;
	WRITE_BYTE_F(adr, res)
	POST_IO
}
RET(20)

// BSETn
_OP(0x08F0)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_BYTE;
	PC += 2;
	src = 1 << (src & 7);
	adr = CPU->A[(Opcode >> 0) & 7];
	DECODE_EXT_WORD
	PRE_IO
	READ_BYTE_F(adr, res)
	CPU->flag_notZ = res & src;
	res |= src;
	WRITE_BYTE_F(adr, res)
	POST_IO
}
RET(22)

// BSETn
_OP(0x08F8)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_BYTE;
	PC += 2;
	src = 1 << (src & 7);
	adr = (s32)(s16)FETCH_WORD;
	PC += 2;
	PRE_IO
	READ_BYTE_F(adr, res)
	CPU->flag_notZ = res & src;
	res |= src;
	WRITE_BYTE_F(adr, res)
	POST_IO
}
RET(20)

// BSETn
_OP(0x08F9)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_BYTE;
	PC += 2;
	src = 1 << (src & 7);
	adr = (s32)FETCH_LONG;
	PC += 4;
	PRE_IO
	READ_BYTE_F(adr, res)
	CPU->flag_notZ = res & src;
	res |= src;
	WRITE_BYTE_F(adr, res)
	POST_IO
}
RET(24)

// BSETn
_OP(0x08DF)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_BYTE;
	PC += 2;
	src = 1 << (src & 7);
	adr = CPU->A[7];
	CPU->A[7] += 2;
	PRE_IO
	READ_BYTE_F(adr, res)
	CPU->flag_notZ = res & src;
	res |= src;
	WRITE_BYTE_F(adr, res)
	POST_IO
}
RET(16)

// BSETn
_OP(0x08E7)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_BYTE;
	PC += 2;
	src = 1 << (src & 7);
	adr = CPU->A[7] - 2;
	CPU->A[7] = adr;
	PRE_IO
	READ_BYTE_F(adr, res)
	CPU->flag_notZ = res & src;
	res |= src;
	WRITE_BYTE_F(adr, res)
	POST_IO
}
RET(18)

// BTST
_OP(0x0100)
{
	u32 adr, res;
	u32 src, dst;

	src = (u32)CPU->D[(Opcode >> 9) & 7];
	src = 1 << (src & 31);
	res = (u32)CPU->D[(Opcode >> 0) & 7];
	CPU->flag_notZ = res & src;
}
RET(6)

// BTST
_OP(0x0110)
{
	u32 adr, res;
	u32 src, dst;

	src = (u8)CPU->D[(Opcode >> 9) & 7];
	src = 1 << (src & 7);
	adr = CPU->A[(Opcode >> 0) & 7];
	PRE_IO
	READ_BYTE_F(adr, res)
	CPU->flag_notZ = res & src;
	POST_IO
}
RET(8)

// BTST
_OP(0x0118)
{
	u32 adr, res;
	u32 src, dst;

	src = (u8)CPU->D[(Opcode >> 9) & 7];
	src = 1 << (src & 7);
	adr = CPU->A[(Opcode >> 0) & 7];
	CPU->A[(Opcode >> 0) & 7] += 1;
	PRE_IO
	READ_BYTE_F(adr, res)
	CPU->flag_notZ = res & src;
	POST_IO
}
RET(8)

// BTST
_OP(0x0120)
{
	u32 adr, res;
	u32 src, dst;

	src = (u8)CPU->D[(Opcode >> 9) & 7];
	src = 1 << (src & 7);
	adr = CPU->A[(Opcode >> 0) & 7] - 1;
	CPU->A[(Opcode >> 0) & 7] = adr;
	PRE_IO
	READ_BYTE_F(adr, res)
	CPU->flag_notZ = res & src;
	POST_IO
}
RET(10)

// BTST
_OP(0x0128)
{
	u32 adr, res;
	u32 src, dst;

	src = (u8)CPU->D[(Opcode >> 9) & 7];
	src = 1 << (src & 7);
	adr = CPU->A[(Opcode >> 0) & 7] + (s32)(s16)FETCH_WORD;
	PC += 2;
	PRE_IO
	READ_BYTE_F(adr, res)
	CPU->flag_notZ = res & src;
	POST_IO
}
RET(12)

// BTST
_OP(0x0130)
{
	u32 adr, res;
	u32 src, dst;

	src = (u8)CPU->D[(Opcode >> 9) & 7];
	src = 1 << (src & 7);
	adr = CPU->A[(Opcode >> 0) & 7];
	DECODE_EXT_WORD
	PRE_IO
	READ_BYTE_F(adr, res)
	CPU->flag_notZ = res & src;
	POST_IO
}
RET(14)

// BTST
_OP(0x0138)
{
	u32 adr, res;
	u32 src, dst;

	src = (u8)CPU->D[(Opcode >> 9) & 7];
	src = 1 << (src & 7);
	adr = (s32)(s16)FETCH_WORD;
	PC += 2;
	PRE_IO
	READ_BYTE_F(adr, res)
	CPU->flag_notZ = res & src;
	POST_IO
}
RET(12)

// BTST
_OP(0x0139)
{
	u32 adr, res;
	u32 src, dst;

	src = (u8)CPU->D[(Opcode >> 9) & 7];
	src = 1 << (src & 7);
	adr = (s32)FETCH_LONG;
	PC += 4;
	PRE_IO
	READ_BYTE_F(adr, res)
	CPU->flag_notZ = res & src;
	POST_IO
}
RET(16)

// BTST
_OP(0x013A)
{
	u32 adr, res;
	u32 src, dst;

	src = (u8)CPU->D[(Opcode >> 9) & 7];
	src = 1 << (src & 7);
	adr = (PC - CPU->BasePC) + (s32)(s16)FETCH_WORD;
	PC += 2;
	PRE_IO
	READ_BYTE_F(adr, res)
	CPU->flag_notZ = res & src;
	POST_IO
}
RET(12)

// BTST
_OP(0x013B)
{
	u32 adr, res;
	u32 src, dst;

	src = (u8)CPU->D[(Opcode >> 9) & 7];
	src = 1 << (src & 7);
	adr = PC - CPU->BasePC;
	DECODE_EXT_WORD
	PRE_IO
	READ_BYTE_F(adr, res)
	CPU->flag_notZ = res & src;
	POST_IO
}
RET(14)

// BTST
_OP(0x013C)
{
	u32 adr, res;
	u32 src, dst;

	src = (u8)CPU->D[(Opcode >> 9) & 7];
	src = 1 << (src & 7);
	res = FETCH_BYTE;
	PC += 2;
	CPU->flag_notZ = res & src;
}
RET(8)

// BTST
_OP(0x011F)
{
	u32 adr, res;
	u32 src, dst;

	src = (u8)CPU->D[(Opcode >> 9) & 7];
	src = 1 << (src & 7);
	adr = CPU->A[7];
	CPU->A[7] += 2;
	PRE_IO
	READ_BYTE_F(adr, res)
	CPU->flag_notZ = res & src;
	POST_IO
}
RET(8)

// BTST
_OP(0x0127)
{
	u32 adr, res;
	u32 src, dst;

	src = (u8)CPU->D[(Opcode >> 9) & 7];
	src = 1 << (src & 7);
	adr = CPU->A[7] - 2;
	CPU->A[7] = adr;
	PRE_IO
	READ_BYTE_F(adr, res)
	CPU->flag_notZ = res & src;
	POST_IO
}
RET(10)

// BCHG
_OP(0x0140)
{
	u32 adr, res;
	u32 src, dst;

	src = (u32)CPU->D[(Opcode >> 9) & 7];
	src = 1 << (src & 31);
	res = (u32)CPU->D[(Opcode >> 0) & 7];
	CPU->flag_notZ = res & src;
	res ^= src;
	*(u32*)(&CPU->D[(Opcode >> 0) & 7]) = res;
}
RET(8)

// BCHG
_OP(0x0150)
{
	u32 adr, res;
	u32 src, dst;

	src = (u8)CPU->D[(Opcode >> 9) & 7];
	src = 1 << (src & 7);
	adr = CPU->A[(Opcode >> 0) & 7];
	PRE_IO
	READ_BYTE_F(adr, res)
	CPU->flag_notZ = res & src;
	res ^= src;
	WRITE_BYTE_F(adr, res)
	POST_IO
}
RET(12)

// BCHG
_OP(0x0158)
{
	u32 adr, res;
	u32 src, dst;

	src = (u8)CPU->D[(Opcode >> 9) & 7];
	src = 1 << (src & 7);
	adr = CPU->A[(Opcode >> 0) & 7];
	CPU->A[(Opcode >> 0) & 7] += 1;
	PRE_IO
	READ_BYTE_F(adr, res)
	CPU->flag_notZ = res & src;
	res ^= src;
	WRITE_BYTE_F(adr, res)
	POST_IO
}
RET(12)

// BCHG
_OP(0x0160)
{
	u32 adr, res;
	u32 src, dst;

	src = (u8)CPU->D[(Opcode >> 9) & 7];
	src = 1 << (src & 7);
	adr = CPU->A[(Opcode >> 0) & 7] - 1;
	CPU->A[(Opcode >> 0) & 7] = adr;
	PRE_IO
	READ_BYTE_F(adr, res)
	CPU->flag_notZ = res & src;
	res ^= src;
	WRITE_BYTE_F(adr, res)
	POST_IO
}
RET(14)

// BCHG
_OP(0x0168)
{
	u32 adr, res;
	u32 src, dst;

	src = (u8)CPU->D[(Opcode >> 9) & 7];
	src = 1 << (src & 7);
	adr = CPU->A[(Opcode >> 0) & 7] + (s32)(s16)FETCH_WORD;
	PC += 2;
	PRE_IO
	READ_BYTE_F(adr, res)
	CPU->flag_notZ = res & src;
	res ^= src;
	WRITE_BYTE_F(adr, res)
	POST_IO
}
RET(16)

// BCHG
_OP(0x0170)
{
	u32 adr, res;
	u32 src, dst;

	src = (u8)CPU->D[(Opcode >> 9) & 7];
	src = 1 << (src & 7);
	adr = CPU->A[(Opcode >> 0) & 7];
	DECODE_EXT_WORD
	PRE_IO
	READ_BYTE_F(adr, res)
	CPU->flag_notZ = res & src;
	res ^= src;
	WRITE_BYTE_F(adr, res)
	POST_IO
}
RET(18)

// BCHG
_OP(0x0178)
{
	u32 adr, res;
	u32 src, dst;

	src = (u8)CPU->D[(Opcode >> 9) & 7];
	src = 1 << (src & 7);
	adr = (s32)(s16)FETCH_WORD;
	PC += 2;
	PRE_IO
	READ_BYTE_F(adr, res)
	CPU->flag_notZ = res & src;
	res ^= src;
	WRITE_BYTE_F(adr, res)
	POST_IO
}
RET(16)

// BCHG
_OP(0x0179)
{
	u32 adr, res;
	u32 src, dst;

	src = (u8)CPU->D[(Opcode >> 9) & 7];
	src = 1 << (src & 7);
	adr = (s32)FETCH_LONG;
	PC += 4;
	PRE_IO
	READ_BYTE_F(adr, res)
	CPU->flag_notZ = res & src;
	res ^= src;
	WRITE_BYTE_F(adr, res)
	POST_IO
}
RET(20)

// BCHG
_OP(0x015F)
{
	u32 adr, res;
	u32 src, dst;

	src = (u8)CPU->D[(Opcode >> 9) & 7];
	src = 1 << (src & 7);
	adr = CPU->A[7];
	CPU->A[7] += 2;
	PRE_IO
	READ_BYTE_F(adr, res)
	CPU->flag_notZ = res & src;
	res ^= src;
	WRITE_BYTE_F(adr, res)
	POST_IO
}
RET(12)

// BCHG
_OP(0x0167)
{
	u32 adr, res;
	u32 src, dst;

	src = (u8)CPU->D[(Opcode >> 9) & 7];
	src = 1 << (src & 7);
	adr = CPU->A[7] - 2;
	CPU->A[7] = adr;
	PRE_IO
	READ_BYTE_F(adr, res)
	CPU->flag_notZ = res & src;
	res ^= src;
	WRITE_BYTE_F(adr, res)
	POST_IO
}
RET(14)

// BCLR
_OP(0x0180)
{
	u32 adr, res;
	u32 src, dst;

	src = (u32)CPU->D[(Opcode >> 9) & 7];
	src = 1 << (src & 31);
	res = (u32)CPU->D[(Opcode >> 0) & 7];
	CPU->flag_notZ = res & src;
	res &= ~src;
	*(u32*)(&CPU->D[(Opcode >> 0) & 7]) = res;
}
RET(10)

// BCLR
_OP(0x0190)
{
	u32 adr, res;
	u32 src, dst;

	src = (u8)CPU->D[(Opcode >> 9) & 7];
	src = 1 << (src & 7);
	adr = CPU->A[(Opcode >> 0) & 7];
	PRE_IO
	READ_BYTE_F(adr, res)
	CPU->flag_notZ = res & src;
	res &= ~src;
	WRITE_BYTE_F(adr, res)
	POST_IO
}
RET(12)

// BCLR
_OP(0x0198)
{
	u32 adr, res;
	u32 src, dst;

	src = (u8)CPU->D[(Opcode >> 9) & 7];
	src = 1 << (src & 7);
	adr = CPU->A[(Opcode >> 0) & 7];
	CPU->A[(Opcode >> 0) & 7] += 1;
	PRE_IO
	READ_BYTE_F(adr, res)
	CPU->flag_notZ = res & src;
	res &= ~src;
	WRITE_BYTE_F(adr, res)
	POST_IO
}
RET(12)

// BCLR
_OP(0x01A0)
{
	u32 adr, res;
	u32 src, dst;

	src = (u8)CPU->D[(Opcode >> 9) & 7];
	src = 1 << (src & 7);
	adr = CPU->A[(Opcode >> 0) & 7] - 1;
	CPU->A[(Opcode >> 0) & 7] = adr;
	PRE_IO
	READ_BYTE_F(adr, res)
	CPU->flag_notZ = res & src;
	res &= ~src;
	WRITE_BYTE_F(adr, res)
	POST_IO
}
RET(14)

// BCLR
_OP(0x01A8)
{
	u32 adr, res;
	u32 src, dst;

	src = (u8)CPU->D[(Opcode >> 9) & 7];
	src = 1 << (src & 7);
	adr = CPU->A[(Opcode >> 0) & 7] + (s32)(s16)FETCH_WORD;
	PC += 2;
	PRE_IO
	READ_BYTE_F(adr, res)
	CPU->flag_notZ = res & src;
	res &= ~src;
	WRITE_BYTE_F(adr, res)
	POST_IO
}
RET(16)

// BCLR
_OP(0x01B0)
{
	u32 adr, res;
	u32 src, dst;

	src = (u8)CPU->D[(Opcode >> 9) & 7];
	src = 1 << (src & 7);
	adr = CPU->A[(Opcode >> 0) & 7];
	DECODE_EXT_WORD
	PRE_IO
	READ_BYTE_F(adr, res)
	CPU->flag_notZ = res & src;
	res &= ~src;
	WRITE_BYTE_F(adr, res)
	POST_IO
}
RET(18)

// BCLR
_OP(0x01B8)
{
	u32 adr, res;
	u32 src, dst;

	src = (u8)CPU->D[(Opcode >> 9) & 7];
	src = 1 << (src & 7);
	adr = (s32)(s16)FETCH_WORD;
	PC += 2;
	PRE_IO
	READ_BYTE_F(adr, res)
	CPU->flag_notZ = res & src;
	res &= ~src;
	WRITE_BYTE_F(adr, res)
	POST_IO
}
RET(16)

// BCLR
_OP(0x01B9)
{
	u32 adr, res;
	u32 src, dst;

	src = (u8)CPU->D[(Opcode >> 9) & 7];
	src = 1 << (src & 7);
	adr = (s32)FETCH_LONG;
	PC += 4;
	PRE_IO
	READ_BYTE_F(adr, res)
	CPU->flag_notZ = res & src;
	res &= ~src;
	WRITE_BYTE_F(adr, res)
	POST_IO
}
RET(20)

// BCLR
_OP(0x019F)
{
	u32 adr, res;
	u32 src, dst;

	src = (u8)CPU->D[(Opcode >> 9) & 7];
	src = 1 << (src & 7);
	adr = CPU->A[7];
	CPU->A[7] += 2;
	PRE_IO
	READ_BYTE_F(adr, res)
	CPU->flag_notZ = res & src;
	res &= ~src;
	WRITE_BYTE_F(adr, res)
	POST_IO
}
RET(12)

// BCLR
_OP(0x01A7)
{
	u32 adr, res;
	u32 src, dst;

	src = (u8)CPU->D[(Opcode >> 9) & 7];
	src = 1 << (src & 7);
	adr = CPU->A[7] - 2;
	CPU->A[7] = adr;
	PRE_IO
	READ_BYTE_F(adr, res)
	CPU->flag_notZ = res & src;
	res &= ~src;
	WRITE_BYTE_F(adr, res)
	POST_IO
}
RET(14)

// BSET
_OP(0x01C0)
{
	u32 adr, res;
	u32 src, dst;

	src = (u32)CPU->D[(Opcode >> 9) & 7];
	src = 1 << (src & 31);
	res = (u32)CPU->D[(Opcode >> 0) & 7];
	CPU->flag_notZ = res & src;
	res |= src;
	*(u32*)(&CPU->D[(Opcode >> 0) & 7]) = res;
}
RET(8)

// BSET
_OP(0x01D0)
{
	u32 adr, res;
	u32 src, dst;

	src = (u8)CPU->D[(Opcode >> 9) & 7];
	src = 1 << (src & 7);
	adr = CPU->A[(Opcode >> 0) & 7];
	PRE_IO
	READ_BYTE_F(adr, res)
	CPU->flag_notZ = res & src;
	res |= src;
	WRITE_BYTE_F(adr, res)
	POST_IO
}
RET(12)

// BSET
_OP(0x01D8)
{
	u32 adr, res;
	u32 src, dst;

	src = (u8)CPU->D[(Opcode >> 9) & 7];
	src = 1 << (src & 7);
	adr = CPU->A[(Opcode >> 0) & 7];
	CPU->A[(Opcode >> 0) & 7] += 1;
	PRE_IO
	READ_BYTE_F(adr, res)
	CPU->flag_notZ = res & src;
	res |= src;
	WRITE_BYTE_F(adr, res)
	POST_IO
}
RET(12)

// BSET
_OP(0x01E0)
{
	u32 adr, res;
	u32 src, dst;

	src = (u8)CPU->D[(Opcode >> 9) & 7];
	src = 1 << (src & 7);
	adr = CPU->A[(Opcode >> 0) & 7] - 1;
	CPU->A[(Opcode >> 0) & 7] = adr;
	PRE_IO
	READ_BYTE_F(adr, res)
	CPU->flag_notZ = res & src;
	res |= src;
	WRITE_BYTE_F(adr, res)
	POST_IO
}
RET(14)

// BSET
_OP(0x01E8)
{
	u32 adr, res;
	u32 src, dst;

	src = (u8)CPU->D[(Opcode >> 9) & 7];
	src = 1 << (src & 7);
	adr = CPU->A[(Opcode >> 0) & 7] + (s32)(s16)FETCH_WORD;
	PC += 2;
	PRE_IO
	READ_BYTE_F(adr, res)
	CPU->flag_notZ = res & src;
	res |= src;
	WRITE_BYTE_F(adr, res)
	POST_IO
}
RET(16)

// BSET
_OP(0x01F0)
{
	u32 adr, res;
	u32 src, dst;

	src = (u8)CPU->D[(Opcode >> 9) & 7];
	src = 1 << (src & 7);
	adr = CPU->A[(Opcode >> 0) & 7];
	DECODE_EXT_WORD
	PRE_IO
	READ_BYTE_F(adr, res)
	CPU->flag_notZ = res & src;
	res |= src;
	WRITE_BYTE_F(adr, res)
	POST_IO
}
RET(18)

// BSET
_OP(0x01F8)
{
	u32 adr, res;
	u32 src, dst;

	src = (u8)CPU->D[(Opcode >> 9) & 7];
	src = 1 << (src & 7);
	adr = (s32)(s16)FETCH_WORD;
	PC += 2;
	PRE_IO
	READ_BYTE_F(adr, res)
	CPU->flag_notZ = res & src;
	res |= src;
	WRITE_BYTE_F(adr, res)
	POST_IO
}
RET(16)

// BSET
_OP(0x01F9)
{
	u32 adr, res;
	u32 src, dst;

	src = (u8)CPU->D[(Opcode >> 9) & 7];
	src = 1 << (src & 7);
	adr = (s32)FETCH_LONG;
	PC += 4;
	PRE_IO
	READ_BYTE_F(adr, res)
	CPU->flag_notZ = res & src;
	res |= src;
	WRITE_BYTE_F(adr, res)
	POST_IO
}
RET(20)

// BSET
_OP(0x01DF)
{
	u32 adr, res;
	u32 src, dst;

	src = (u8)CPU->D[(Opcode >> 9) & 7];
	src = 1 << (src & 7);
	adr = CPU->A[7];
	CPU->A[7] += 2;
	PRE_IO
	READ_BYTE_F(adr, res)
	CPU->flag_notZ = res & src;
	res |= src;
	WRITE_BYTE_F(adr, res)
	POST_IO
}
RET(12)

// BSET
_OP(0x01E7)
{
	u32 adr, res;
	u32 src, dst;

	src = (u8)CPU->D[(Opcode >> 9) & 7];
	src = 1 << (src & 7);
	adr = CPU->A[7] - 2;
	CPU->A[7] = adr;
	PRE_IO
	READ_BYTE_F(adr, res)
	CPU->flag_notZ = res & src;
	res |= src;
	WRITE_BYTE_F(adr, res)
	POST_IO
}
RET(14)

// MOVEPWaD
_OP(0x0108)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7] + (s32)(s16)FETCH_WORD;
	PC += 2;
	PRE_IO
	READ_BYTE_F(adr + 0, res)
	READ_BYTE_F(adr + 2, src)
	*(u16*)(&CPU->D[(Opcode >> 9) & 7]) = (res << 8) | src;
	POST_IO
}
RET(24)

// MOVEPLaD
_OP(0x0148)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7] + (s32)(s16)FETCH_WORD;
	PC += 2;
	PRE_IO
	READ_BYTE_F(adr, res)
	res <<= 24;
	adr += 2;
	READ_BYTE_F(adr, src)
	res |= src << 16;
	adr += 2;
	READ_BYTE_F(adr, src)
	res |= src << 8;
	adr += 2;
	READ_BYTE_F(adr, src)
	CPU->D[(Opcode >> 9) & 7] = res | src;
	POST_IO
}
RET(32)

// MOVEPWDa
_OP(0x0188)
{
	u32 adr, res;
	u32 src, dst;

	res = (u32)CPU->D[(Opcode >> 9) & 7];
	adr = CPU->A[(Opcode >> 0) & 7] + (s32)(s16)FETCH_WORD;
	PC += 2;
	PRE_IO
	WRITE_BYTE_F(adr + 0, res >> 8)
	WRITE_BYTE_F(adr + 2, res >> 0)
	POST_IO
}
RET(24)

// MOVEPLDa
_OP(0x01C8)
{
	u32 adr, res;
	u32 src, dst;

	res = (u32)CPU->D[(Opcode >> 9) & 7];
	adr = CPU->A[(Opcode >> 0) & 7] + (s32)(s16)FETCH_WORD;
	PC += 2;
	PRE_IO
	WRITE_BYTE_F(adr, res >> 24)
	adr += 2;
	WRITE_BYTE_F(adr, res >> 16)
	adr += 2;
	WRITE_BYTE_F(adr, res >> 8)
	adr += 2;
	WRITE_BYTE_F(adr, res >> 0)
	POST_IO
}
RET(32)
