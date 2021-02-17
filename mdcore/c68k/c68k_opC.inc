
// ANDaD
_OP(0xC000)
{
	u32 adr, res;
	u32 src, dst;

	src = (u8)CPU->D[(Opcode >> 0) & 7];
	res = (u8)CPU->D[(Opcode >> 9) & 7];
	res &= src;
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res;
	*(u8*)(&CPU->D[(Opcode >> 9) & 7]) = res;
}
RET(4)

// ANDaD
_OP(0xC010)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7];
	PRE_IO
	READ_BYTE_F(adr, src)
	res = (u8)CPU->D[(Opcode >> 9) & 7];
	res &= src;
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res;
	*(u8*)(&CPU->D[(Opcode >> 9) & 7]) = res;
	POST_IO
}
RET(8)

// ANDaD
_OP(0xC018)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7];
	CPU->A[(Opcode >> 0) & 7] += 1;
	PRE_IO
	READ_BYTE_F(adr, src)
	res = (u8)CPU->D[(Opcode >> 9) & 7];
	res &= src;
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res;
	*(u8*)(&CPU->D[(Opcode >> 9) & 7]) = res;
	POST_IO
}
RET(8)

// ANDaD
_OP(0xC020)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7] - 1;
	CPU->A[(Opcode >> 0) & 7] = adr;
	PRE_IO
	READ_BYTE_F(adr, src)
	res = (u8)CPU->D[(Opcode >> 9) & 7];
	res &= src;
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res;
	*(u8*)(&CPU->D[(Opcode >> 9) & 7]) = res;
	POST_IO
}
RET(10)

// ANDaD
_OP(0xC028)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7] + (s32)(s16)FETCH_WORD;
	PC += 2;
	PRE_IO
	READ_BYTE_F(adr, src)
	res = (u8)CPU->D[(Opcode >> 9) & 7];
	res &= src;
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res;
	*(u8*)(&CPU->D[(Opcode >> 9) & 7]) = res;
	POST_IO
}
RET(12)

// ANDaD
_OP(0xC030)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7];
	DECODE_EXT_WORD
	PRE_IO
	READ_BYTE_F(adr, src)
	res = (u8)CPU->D[(Opcode >> 9) & 7];
	res &= src;
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res;
	*(u8*)(&CPU->D[(Opcode >> 9) & 7]) = res;
	POST_IO
}
RET(14)

// ANDaD
_OP(0xC038)
{
	u32 adr, res;
	u32 src, dst;

	adr = (s32)(s16)FETCH_WORD;
	PC += 2;
	PRE_IO
	READ_BYTE_F(adr, src)
	res = (u8)CPU->D[(Opcode >> 9) & 7];
	res &= src;
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res;
	*(u8*)(&CPU->D[(Opcode >> 9) & 7]) = res;
	POST_IO
}
RET(12)

// ANDaD
_OP(0xC039)
{
	u32 adr, res;
	u32 src, dst;

	adr = (s32)FETCH_LONG;
	PC += 4;
	PRE_IO
	READ_BYTE_F(adr, src)
	res = (u8)CPU->D[(Opcode >> 9) & 7];
	res &= src;
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res;
	*(u8*)(&CPU->D[(Opcode >> 9) & 7]) = res;
	POST_IO
}
RET(16)

// ANDaD
_OP(0xC03A)
{
	u32 adr, res;
	u32 src, dst;

	adr = (PC - CPU->BasePC) + (s32)(s16)FETCH_WORD;
	PC += 2;
	PRE_IO
	READ_BYTE_F(adr, src)
	res = (u8)CPU->D[(Opcode >> 9) & 7];
	res &= src;
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res;
	*(u8*)(&CPU->D[(Opcode >> 9) & 7]) = res;
	POST_IO
}
RET(12)

// ANDaD
_OP(0xC03B)
{
	u32 adr, res;
	u32 src, dst;

	adr = PC - CPU->BasePC;
	DECODE_EXT_WORD
	PRE_IO
	READ_BYTE_F(adr, src)
	res = (u8)CPU->D[(Opcode >> 9) & 7];
	res &= src;
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res;
	*(u8*)(&CPU->D[(Opcode >> 9) & 7]) = res;
	POST_IO
}
RET(14)

// ANDaD
_OP(0xC03C)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_BYTE;
	PC += 2;
	res = (u8)CPU->D[(Opcode >> 9) & 7];
	res &= src;
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res;
	*(u8*)(&CPU->D[(Opcode >> 9) & 7]) = res;
}
RET(8)

// ANDaD
_OP(0xC01F)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[7];
	CPU->A[7] += 2;
	PRE_IO
	READ_BYTE_F(adr, src)
	res = (u8)CPU->D[(Opcode >> 9) & 7];
	res &= src;
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res;
	*(u8*)(&CPU->D[(Opcode >> 9) & 7]) = res;
	POST_IO
}
RET(8)

// ANDaD
_OP(0xC027)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[7] - 2;
	CPU->A[7] = adr;
	PRE_IO
	READ_BYTE_F(adr, src)
	res = (u8)CPU->D[(Opcode >> 9) & 7];
	res &= src;
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res;
	*(u8*)(&CPU->D[(Opcode >> 9) & 7]) = res;
	POST_IO
}
RET(10)

// ANDaD
_OP(0xC040)
{
	u32 adr, res;
	u32 src, dst;

	src = (u16)CPU->D[(Opcode >> 0) & 7];
	res = (u16)CPU->D[(Opcode >> 9) & 7];
	res &= src;
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	*(u16*)(&CPU->D[(Opcode >> 9) & 7]) = res;
}
RET(4)

// ANDaD
_OP(0xC050)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7];
	PRE_IO
	READ_WORD_F(adr, src)
	res = (u16)CPU->D[(Opcode >> 9) & 7];
	res &= src;
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	*(u16*)(&CPU->D[(Opcode >> 9) & 7]) = res;
	POST_IO
}
RET(8)

// ANDaD
_OP(0xC058)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7];
	CPU->A[(Opcode >> 0) & 7] += 2;
	PRE_IO
	READ_WORD_F(adr, src)
	res = (u16)CPU->D[(Opcode >> 9) & 7];
	res &= src;
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	*(u16*)(&CPU->D[(Opcode >> 9) & 7]) = res;
	POST_IO
}
RET(8)

// ANDaD
_OP(0xC060)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7] - 2;
	CPU->A[(Opcode >> 0) & 7] = adr;
	PRE_IO
	READ_WORD_F(adr, src)
	res = (u16)CPU->D[(Opcode >> 9) & 7];
	res &= src;
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	*(u16*)(&CPU->D[(Opcode >> 9) & 7]) = res;
	POST_IO
}
RET(10)

// ANDaD
_OP(0xC068)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7] + (s32)(s16)FETCH_WORD;
	PC += 2;
	PRE_IO
	READ_WORD_F(adr, src)
	res = (u16)CPU->D[(Opcode >> 9) & 7];
	res &= src;
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	*(u16*)(&CPU->D[(Opcode >> 9) & 7]) = res;
	POST_IO
}
RET(12)

// ANDaD
_OP(0xC070)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7];
	DECODE_EXT_WORD
	PRE_IO
	READ_WORD_F(adr, src)
	res = (u16)CPU->D[(Opcode >> 9) & 7];
	res &= src;
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	*(u16*)(&CPU->D[(Opcode >> 9) & 7]) = res;
	POST_IO
}
RET(14)

// ANDaD
_OP(0xC078)
{
	u32 adr, res;
	u32 src, dst;

	adr = (s32)(s16)FETCH_WORD;
	PC += 2;
	PRE_IO
	READ_WORD_F(adr, src)
	res = (u16)CPU->D[(Opcode >> 9) & 7];
	res &= src;
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	*(u16*)(&CPU->D[(Opcode >> 9) & 7]) = res;
	POST_IO
}
RET(12)

// ANDaD
_OP(0xC079)
{
	u32 adr, res;
	u32 src, dst;

	adr = (s32)FETCH_LONG;
	PC += 4;
	PRE_IO
	READ_WORD_F(adr, src)
	res = (u16)CPU->D[(Opcode >> 9) & 7];
	res &= src;
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	*(u16*)(&CPU->D[(Opcode >> 9) & 7]) = res;
	POST_IO
}
RET(16)

// ANDaD
_OP(0xC07A)
{
	u32 adr, res;
	u32 src, dst;

	adr = (PC - CPU->BasePC) + (s32)(s16)FETCH_WORD;
	PC += 2;
	PRE_IO
	READ_WORD_F(adr, src)
	res = (u16)CPU->D[(Opcode >> 9) & 7];
	res &= src;
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	*(u16*)(&CPU->D[(Opcode >> 9) & 7]) = res;
	POST_IO
}
RET(12)

// ANDaD
_OP(0xC07B)
{
	u32 adr, res;
	u32 src, dst;

	adr = PC - CPU->BasePC;
	DECODE_EXT_WORD
	PRE_IO
	READ_WORD_F(adr, src)
	res = (u16)CPU->D[(Opcode >> 9) & 7];
	res &= src;
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	*(u16*)(&CPU->D[(Opcode >> 9) & 7]) = res;
	POST_IO
}
RET(14)

// ANDaD
_OP(0xC07C)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_WORD;
	PC += 2;
	res = (u16)CPU->D[(Opcode >> 9) & 7];
	res &= src;
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	*(u16*)(&CPU->D[(Opcode >> 9) & 7]) = res;
}
RET(8)

// ANDaD
_OP(0xC05F)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[7];
	CPU->A[7] += 2;
	PRE_IO
	READ_WORD_F(adr, src)
	res = (u16)CPU->D[(Opcode >> 9) & 7];
	res &= src;
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	*(u16*)(&CPU->D[(Opcode >> 9) & 7]) = res;
	POST_IO
}
RET(8)

// ANDaD
_OP(0xC067)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[7] - 2;
	CPU->A[7] = adr;
	PRE_IO
	READ_WORD_F(adr, src)
	res = (u16)CPU->D[(Opcode >> 9) & 7];
	res &= src;
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	*(u16*)(&CPU->D[(Opcode >> 9) & 7]) = res;
	POST_IO
}
RET(10)

// ANDaD
_OP(0xC080)
{
	u32 adr, res;
	u32 src, dst;

	src = (u32)CPU->D[(Opcode >> 0) & 7];
	res = (u32)CPU->D[(Opcode >> 9) & 7];
	res &= src;
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 24;
	*(u32*)(&CPU->D[(Opcode >> 9) & 7]) = res;
}
RET(8)

// ANDaD
_OP(0xC090)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7];
	PRE_IO
	READ_LONG_F(adr, src)
	res = (u32)CPU->D[(Opcode >> 9) & 7];
	res &= src;
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 24;
	*(u32*)(&CPU->D[(Opcode >> 9) & 7]) = res;
	POST_IO
}
RET(14)

// ANDaD
_OP(0xC098)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7];
	CPU->A[(Opcode >> 0) & 7] += 4;
	PRE_IO
	READ_LONG_F(adr, src)
	res = (u32)CPU->D[(Opcode >> 9) & 7];
	res &= src;
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 24;
	*(u32*)(&CPU->D[(Opcode >> 9) & 7]) = res;
	POST_IO
}
RET(14)

// ANDaD
_OP(0xC0A0)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7] - 4;
	CPU->A[(Opcode >> 0) & 7] = adr;
	PRE_IO
	READ_LONG_F(adr, src)
	res = (u32)CPU->D[(Opcode >> 9) & 7];
	res &= src;
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 24;
	*(u32*)(&CPU->D[(Opcode >> 9) & 7]) = res;
	POST_IO
}
RET(16)

// ANDaD
_OP(0xC0A8)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7] + (s32)(s16)FETCH_WORD;
	PC += 2;
	PRE_IO
	READ_LONG_F(adr, src)
	res = (u32)CPU->D[(Opcode >> 9) & 7];
	res &= src;
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 24;
	*(u32*)(&CPU->D[(Opcode >> 9) & 7]) = res;
	POST_IO
}
RET(18)

// ANDaD
_OP(0xC0B0)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7];
	DECODE_EXT_WORD
	PRE_IO
	READ_LONG_F(adr, src)
	res = (u32)CPU->D[(Opcode >> 9) & 7];
	res &= src;
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 24;
	*(u32*)(&CPU->D[(Opcode >> 9) & 7]) = res;
	POST_IO
}
RET(20)

// ANDaD
_OP(0xC0B8)
{
	u32 adr, res;
	u32 src, dst;

	adr = (s32)(s16)FETCH_WORD;
	PC += 2;
	PRE_IO
	READ_LONG_F(adr, src)
	res = (u32)CPU->D[(Opcode >> 9) & 7];
	res &= src;
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 24;
	*(u32*)(&CPU->D[(Opcode >> 9) & 7]) = res;
	POST_IO
}
RET(18)

// ANDaD
_OP(0xC0B9)
{
	u32 adr, res;
	u32 src, dst;

	adr = (s32)FETCH_LONG;
	PC += 4;
	PRE_IO
	READ_LONG_F(adr, src)
	res = (u32)CPU->D[(Opcode >> 9) & 7];
	res &= src;
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 24;
	*(u32*)(&CPU->D[(Opcode >> 9) & 7]) = res;
	POST_IO
}
RET(22)

// ANDaD
_OP(0xC0BA)
{
	u32 adr, res;
	u32 src, dst;

	adr = (PC - CPU->BasePC) + (s32)(s16)FETCH_WORD;
	PC += 2;
	PRE_IO
	READ_LONG_F(adr, src)
	res = (u32)CPU->D[(Opcode >> 9) & 7];
	res &= src;
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 24;
	*(u32*)(&CPU->D[(Opcode >> 9) & 7]) = res;
	POST_IO
}
RET(18)

// ANDaD
_OP(0xC0BB)
{
	u32 adr, res;
	u32 src, dst;

	adr = PC - CPU->BasePC;
	DECODE_EXT_WORD
	PRE_IO
	READ_LONG_F(adr, src)
	res = (u32)CPU->D[(Opcode >> 9) & 7];
	res &= src;
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 24;
	*(u32*)(&CPU->D[(Opcode >> 9) & 7]) = res;
	POST_IO
}
RET(20)

// ANDaD
_OP(0xC0BC)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_LONG;
	PC += 4;
	res = (u32)CPU->D[(Opcode >> 9) & 7];
	res &= src;
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 24;
	*(u32*)(&CPU->D[(Opcode >> 9) & 7]) = res;
}
RET(16)

// ANDaD
_OP(0xC09F)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[7];
	CPU->A[7] += 4;
	PRE_IO
	READ_LONG_F(adr, src)
	res = (u32)CPU->D[(Opcode >> 9) & 7];
	res &= src;
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 24;
	*(u32*)(&CPU->D[(Opcode >> 9) & 7]) = res;
	POST_IO
}
RET(14)

// ANDaD
_OP(0xC0A7)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[7] - 4;
	CPU->A[7] = adr;
	PRE_IO
	READ_LONG_F(adr, src)
	res = (u32)CPU->D[(Opcode >> 9) & 7];
	res &= src;
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 24;
	*(u32*)(&CPU->D[(Opcode >> 9) & 7]) = res;
	POST_IO
}
RET(16)

// ANDDa
_OP(0xC110)
{
	u32 adr, res;
	u32 src, dst;

	src = (u8)CPU->D[(Opcode >> 9) & 7];
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
RET(12)

// ANDDa
_OP(0xC118)
{
	u32 adr, res;
	u32 src, dst;

	src = (u8)CPU->D[(Opcode >> 9) & 7];
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
RET(12)

// ANDDa
_OP(0xC120)
{
	u32 adr, res;
	u32 src, dst;

	src = (u8)CPU->D[(Opcode >> 9) & 7];
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
RET(14)

// ANDDa
_OP(0xC128)
{
	u32 adr, res;
	u32 src, dst;

	src = (u8)CPU->D[(Opcode >> 9) & 7];
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
RET(16)

// ANDDa
_OP(0xC130)
{
	u32 adr, res;
	u32 src, dst;

	src = (u8)CPU->D[(Opcode >> 9) & 7];
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
RET(18)

// ANDDa
_OP(0xC138)
{
	u32 adr, res;
	u32 src, dst;

	src = (u8)CPU->D[(Opcode >> 9) & 7];
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
RET(16)

// ANDDa
_OP(0xC139)
{
	u32 adr, res;
	u32 src, dst;

	src = (u8)CPU->D[(Opcode >> 9) & 7];
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
RET(20)

// ANDDa
_OP(0xC11F)
{
	u32 adr, res;
	u32 src, dst;

	src = (u8)CPU->D[(Opcode >> 9) & 7];
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
RET(12)

// ANDDa
_OP(0xC127)
{
	u32 adr, res;
	u32 src, dst;

	src = (u8)CPU->D[(Opcode >> 9) & 7];
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
RET(14)

// ANDDa
_OP(0xC150)
{
	u32 adr, res;
	u32 src, dst;

	src = (u16)CPU->D[(Opcode >> 9) & 7];
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
RET(12)

// ANDDa
_OP(0xC158)
{
	u32 adr, res;
	u32 src, dst;

	src = (u16)CPU->D[(Opcode >> 9) & 7];
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
RET(12)

// ANDDa
_OP(0xC160)
{
	u32 adr, res;
	u32 src, dst;

	src = (u16)CPU->D[(Opcode >> 9) & 7];
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
RET(14)

// ANDDa
_OP(0xC168)
{
	u32 adr, res;
	u32 src, dst;

	src = (u16)CPU->D[(Opcode >> 9) & 7];
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
RET(16)

// ANDDa
_OP(0xC170)
{
	u32 adr, res;
	u32 src, dst;

	src = (u16)CPU->D[(Opcode >> 9) & 7];
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
RET(18)

// ANDDa
_OP(0xC178)
{
	u32 adr, res;
	u32 src, dst;

	src = (u16)CPU->D[(Opcode >> 9) & 7];
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
RET(16)

// ANDDa
_OP(0xC179)
{
	u32 adr, res;
	u32 src, dst;

	src = (u16)CPU->D[(Opcode >> 9) & 7];
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
RET(20)

// ANDDa
_OP(0xC15F)
{
	u32 adr, res;
	u32 src, dst;

	src = (u16)CPU->D[(Opcode >> 9) & 7];
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
RET(12)

// ANDDa
_OP(0xC167)
{
	u32 adr, res;
	u32 src, dst;

	src = (u16)CPU->D[(Opcode >> 9) & 7];
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
RET(14)

// ANDDa
_OP(0xC190)
{
	u32 adr, res;
	u32 src, dst;

	src = (u32)CPU->D[(Opcode >> 9) & 7];
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
RET(20)

// ANDDa
_OP(0xC198)
{
	u32 adr, res;
	u32 src, dst;

	src = (u32)CPU->D[(Opcode >> 9) & 7];
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
RET(20)

// ANDDa
_OP(0xC1A0)
{
	u32 adr, res;
	u32 src, dst;

	src = (u32)CPU->D[(Opcode >> 9) & 7];
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
RET(22)

// ANDDa
_OP(0xC1A8)
{
	u32 adr, res;
	u32 src, dst;

	src = (u32)CPU->D[(Opcode >> 9) & 7];
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
RET(24)

// ANDDa
_OP(0xC1B0)
{
	u32 adr, res;
	u32 src, dst;

	src = (u32)CPU->D[(Opcode >> 9) & 7];
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
RET(26)

// ANDDa
_OP(0xC1B8)
{
	u32 adr, res;
	u32 src, dst;

	src = (u32)CPU->D[(Opcode >> 9) & 7];
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
RET(24)

// ANDDa
_OP(0xC1B9)
{
	u32 adr, res;
	u32 src, dst;

	src = (u32)CPU->D[(Opcode >> 9) & 7];
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
RET(28)

// ANDDa
_OP(0xC19F)
{
	u32 adr, res;
	u32 src, dst;

	src = (u32)CPU->D[(Opcode >> 9) & 7];
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
RET(20)

// ANDDa
_OP(0xC1A7)
{
	u32 adr, res;
	u32 src, dst;

	src = (u32)CPU->D[(Opcode >> 9) & 7];
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
RET(22)

// ABCD
_OP(0xC100)
{
	u32 adr, res;
	u32 src, dst;

	src = (u8)CPU->D[(Opcode >> 0) & 7];
	dst = (u8)CPU->D[(Opcode >> 9) & 7];
	res = (dst & 0xF) + (src & 0xF) + ((CPU->flag_X >> C68K_SR_X_SFT) & 1);
	if (res > 9) res += 6;
	res += (dst & 0xF0) + (src & 0xF0);
	if (res > 0x99)
	{
		res -= 0xA0;
		CPU->flag_X = CPU->flag_C = C68K_SR_C;
	}
	else CPU->flag_X = CPU->flag_C = 0;
	CPU->flag_notZ |= res & 0xFF;
	CPU->flag_N = res;
	*(u8*)(&CPU->D[(Opcode >> 9) & 7]) = res;
}
RET(6)

// ABCDM
_OP(0xC108)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7] - 1;
	CPU->A[(Opcode >> 0) & 7] = adr;
	PRE_IO
	READ_BYTE_F(adr, src)
	adr = CPU->A[(Opcode >> 9) & 7] - 1;
	CPU->A[(Opcode >> 9) & 7] = adr;
	READ_BYTE_F(adr, dst)
	res = (dst & 0xF) + (src & 0xF) + ((CPU->flag_X >> C68K_SR_X_SFT) & 1);
	if (res > 9) res += 6;
	res += (dst & 0xF0) + (src & 0xF0);
	if (res > 0x99)
	{
		res -= 0xA0;
		CPU->flag_X = CPU->flag_C = C68K_SR_C;
	}
	else CPU->flag_X = CPU->flag_C = 0;
	CPU->flag_notZ |= res & 0xFF;
	CPU->flag_N = res;
	WRITE_BYTE_F(adr, res)
	POST_IO
}
RET(18)

// ABCD7M
_OP(0xC10F)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[7] - 2;
	CPU->A[7] = adr;
	PRE_IO
	READ_BYTE_F(adr, src)
	adr = CPU->A[(Opcode >> 9) & 7] - 1;
	CPU->A[(Opcode >> 9) & 7] = adr;
	READ_BYTE_F(adr, dst)
	res = (dst & 0xF) + (src & 0xF) + ((CPU->flag_X >> C68K_SR_X_SFT) & 1);
	if (res > 9) res += 6;
	res += (dst & 0xF0) + (src & 0xF0);
	if (res > 0x99)
	{
		res -= 0xA0;
		CPU->flag_X = CPU->flag_C = C68K_SR_C;
	}
	else CPU->flag_X = CPU->flag_C = 0;
	CPU->flag_notZ |= res & 0xFF;
	CPU->flag_N = res;
	WRITE_BYTE_F(adr, res)
	POST_IO
}
RET(18)

// ABCDM7
_OP(0xCF08)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7] - 1;
	CPU->A[(Opcode >> 0) & 7] = adr;
	PRE_IO
	READ_BYTE_F(adr, src)
	adr = CPU->A[7] - 2;
	CPU->A[7] = adr;
	READ_BYTE_F(adr, dst)
	res = (dst & 0xF) + (src & 0xF) + ((CPU->flag_X >> C68K_SR_X_SFT) & 1);
	if (res > 9) res += 6;
	res += (dst & 0xF0) + (src & 0xF0);
	if (res > 0x99)
	{
		res -= 0xA0;
		CPU->flag_X = CPU->flag_C = C68K_SR_C;
	}
	else CPU->flag_X = CPU->flag_C = 0;
	CPU->flag_notZ |= res & 0xFF;
	CPU->flag_N = res;
	WRITE_BYTE_F(adr, res)
	POST_IO
}
RET(18)

// ABCD7M7
_OP(0xCF0F)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[7] - 2;
	CPU->A[7] = adr;
	PRE_IO
	READ_BYTE_F(adr, src)
	adr = CPU->A[7] - 2;
	CPU->A[7] = adr;
	READ_BYTE_F(adr, dst)
	res = (dst & 0xF) + (src & 0xF) + ((CPU->flag_X >> C68K_SR_X_SFT) & 1);
	if (res > 9) res += 6;
	res += (dst & 0xF0) + (src & 0xF0);
	if (res > 0x99)
	{
		res -= 0xA0;
		CPU->flag_X = CPU->flag_C = C68K_SR_C;
	}
	else CPU->flag_X = CPU->flag_C = 0;
	CPU->flag_notZ |= res & 0xFF;
	CPU->flag_N = res;
	WRITE_BYTE_F(adr, res)
	POST_IO
}
RET(18)

// MULU
_OP(0xC0C0)
{
	u32 adr, res;
	u32 src, dst;

	src = (u16)CPU->D[(Opcode >> 0) & 7];
	res = (u16)CPU->D[(Opcode >> 9) & 7];
	res *= src;
	CPU->flag_N = res >> 24;
	CPU->flag_notZ = res;
	CPU->flag_V = CPU->flag_C = 0;
	*(u32*)(&CPU->D[(Opcode >> 9) & 7]) = res;
}
RET(50)

// MULU
_OP(0xC0D0)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7];
	PRE_IO
	READ_WORD_F(adr, src)
	res = (u16)CPU->D[(Opcode >> 9) & 7];
	res *= src;
	CPU->flag_N = res >> 24;
	CPU->flag_notZ = res;
	CPU->flag_V = CPU->flag_C = 0;
	*(u32*)(&CPU->D[(Opcode >> 9) & 7]) = res;
	POST_IO
}
RET(54)

// MULU
_OP(0xC0D8)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7];
	CPU->A[(Opcode >> 0) & 7] += 2;
	PRE_IO
	READ_WORD_F(adr, src)
	res = (u16)CPU->D[(Opcode >> 9) & 7];
	res *= src;
	CPU->flag_N = res >> 24;
	CPU->flag_notZ = res;
	CPU->flag_V = CPU->flag_C = 0;
	*(u32*)(&CPU->D[(Opcode >> 9) & 7]) = res;
	POST_IO
}
RET(54)

// MULU
_OP(0xC0E0)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7] - 2;
	CPU->A[(Opcode >> 0) & 7] = adr;
	PRE_IO
	READ_WORD_F(adr, src)
	res = (u16)CPU->D[(Opcode >> 9) & 7];
	res *= src;
	CPU->flag_N = res >> 24;
	CPU->flag_notZ = res;
	CPU->flag_V = CPU->flag_C = 0;
	*(u32*)(&CPU->D[(Opcode >> 9) & 7]) = res;
	POST_IO
}
RET(56)

// MULU
_OP(0xC0E8)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7] + (s32)(s16)FETCH_WORD;
	PC += 2;
	PRE_IO
	READ_WORD_F(adr, src)
	res = (u16)CPU->D[(Opcode >> 9) & 7];
	res *= src;
	CPU->flag_N = res >> 24;
	CPU->flag_notZ = res;
	CPU->flag_V = CPU->flag_C = 0;
	*(u32*)(&CPU->D[(Opcode >> 9) & 7]) = res;
	POST_IO
}
RET(58)

// MULU
_OP(0xC0F0)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7];
	DECODE_EXT_WORD
	PRE_IO
	READ_WORD_F(adr, src)
	res = (u16)CPU->D[(Opcode >> 9) & 7];
	res *= src;
	CPU->flag_N = res >> 24;
	CPU->flag_notZ = res;
	CPU->flag_V = CPU->flag_C = 0;
	*(u32*)(&CPU->D[(Opcode >> 9) & 7]) = res;
	POST_IO
}
RET(60)

// MULU
_OP(0xC0F8)
{
	u32 adr, res;
	u32 src, dst;

	adr = (s32)(s16)FETCH_WORD;
	PC += 2;
	PRE_IO
	READ_WORD_F(adr, src)
	res = (u16)CPU->D[(Opcode >> 9) & 7];
	res *= src;
	CPU->flag_N = res >> 24;
	CPU->flag_notZ = res;
	CPU->flag_V = CPU->flag_C = 0;
	*(u32*)(&CPU->D[(Opcode >> 9) & 7]) = res;
	POST_IO
}
RET(58)

// MULU
_OP(0xC0F9)
{
	u32 adr, res;
	u32 src, dst;

	adr = (s32)FETCH_LONG;
	PC += 4;
	PRE_IO
	READ_WORD_F(adr, src)
	res = (u16)CPU->D[(Opcode >> 9) & 7];
	res *= src;
	CPU->flag_N = res >> 24;
	CPU->flag_notZ = res;
	CPU->flag_V = CPU->flag_C = 0;
	*(u32*)(&CPU->D[(Opcode >> 9) & 7]) = res;
	POST_IO
}
RET(62)

// MULU
_OP(0xC0FA)
{
	u32 adr, res;
	u32 src, dst;

	adr = (PC - CPU->BasePC) + (s32)(s16)FETCH_WORD;
	PC += 2;
	PRE_IO
	READ_WORD_F(adr, src)
	res = (u16)CPU->D[(Opcode >> 9) & 7];
	res *= src;
	CPU->flag_N = res >> 24;
	CPU->flag_notZ = res;
	CPU->flag_V = CPU->flag_C = 0;
	*(u32*)(&CPU->D[(Opcode >> 9) & 7]) = res;
	POST_IO
}
RET(58)

// MULU
_OP(0xC0FB)
{
	u32 adr, res;
	u32 src, dst;

	adr = PC - CPU->BasePC;
	DECODE_EXT_WORD
	PRE_IO
	READ_WORD_F(adr, src)
	res = (u16)CPU->D[(Opcode >> 9) & 7];
	res *= src;
	CPU->flag_N = res >> 24;
	CPU->flag_notZ = res;
	CPU->flag_V = CPU->flag_C = 0;
	*(u32*)(&CPU->D[(Opcode >> 9) & 7]) = res;
	POST_IO
}
RET(60)

// MULU
_OP(0xC0FC)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_WORD;
	PC += 2;
	res = (u16)CPU->D[(Opcode >> 9) & 7];
	res *= src;
	CPU->flag_N = res >> 24;
	CPU->flag_notZ = res;
	CPU->flag_V = CPU->flag_C = 0;
	*(u32*)(&CPU->D[(Opcode >> 9) & 7]) = res;
}
RET(54)

// MULU
_OP(0xC0DF)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[7];
	CPU->A[7] += 2;
	PRE_IO
	READ_WORD_F(adr, src)
	res = (u16)CPU->D[(Opcode >> 9) & 7];
	res *= src;
	CPU->flag_N = res >> 24;
	CPU->flag_notZ = res;
	CPU->flag_V = CPU->flag_C = 0;
	*(u32*)(&CPU->D[(Opcode >> 9) & 7]) = res;
	POST_IO
}
RET(54)

// MULU
_OP(0xC0E7)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[7] - 2;
	CPU->A[7] = adr;
	PRE_IO
	READ_WORD_F(adr, src)
	res = (u16)CPU->D[(Opcode >> 9) & 7];
	res *= src;
	CPU->flag_N = res >> 24;
	CPU->flag_notZ = res;
	CPU->flag_V = CPU->flag_C = 0;
	*(u32*)(&CPU->D[(Opcode >> 9) & 7]) = res;
	POST_IO
}
RET(56)

// MULS
_OP(0xC1C0)
{
	u32 adr, res;
	u32 src, dst;

	src = (s32)(s16)CPU->D[(Opcode >> 0) & 7];
	res = (s32)(s16)CPU->D[(Opcode >> 9) & 7];
	//(s32)res *= (s32)src;	
	res = (s32)res * (s32)src;	
	CPU->flag_N = res >> 24;
	CPU->flag_notZ = res;
	CPU->flag_V = CPU->flag_C = 0;
	*(u32*)(&CPU->D[(Opcode >> 9) & 7]) = res;
}
RET(50)

// MULS
_OP(0xC1D0)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7];
	PRE_IO
	READSX_WORD_F(adr, src)
	res = (s32)(s16)CPU->D[(Opcode >> 9) & 7];
	//(s32)res *= (s32)src;
	res = (s32)res * (s32)src;
	CPU->flag_N = res >> 24;
	CPU->flag_notZ = res;
	CPU->flag_V = CPU->flag_C = 0;
	*(u32*)(&CPU->D[(Opcode >> 9) & 7]) = res;
	POST_IO
}
RET(54)

// MULS
_OP(0xC1D8)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7];
	CPU->A[(Opcode >> 0) & 7] += 2;
	PRE_IO
	READSX_WORD_F(adr, src)
	res = (s32)(s16)CPU->D[(Opcode >> 9) & 7];
	//(s32)res *= (s32)src;
	res = (s32)res * (s32)src;
	CPU->flag_N = res >> 24;
	CPU->flag_notZ = res;
	CPU->flag_V = CPU->flag_C = 0;
	*(u32*)(&CPU->D[(Opcode >> 9) & 7]) = res;
	POST_IO
}
RET(54)

// MULS
_OP(0xC1E0)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7] - 2;
	CPU->A[(Opcode >> 0) & 7] = adr;
	PRE_IO
	READSX_WORD_F(adr, src)
	res = (s32)(s16)CPU->D[(Opcode >> 9) & 7];
	//(s32)res *= (s32)src;
	res = (s32)res * (s32)src;
	CPU->flag_N = res >> 24;
	CPU->flag_notZ = res;
	CPU->flag_V = CPU->flag_C = 0;
	*(u32*)(&CPU->D[(Opcode >> 9) & 7]) = res;
	POST_IO
}
RET(56)

// MULS
_OP(0xC1E8)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7] + (s32)(s16)FETCH_WORD;
	PC += 2;
	PRE_IO
	READSX_WORD_F(adr, src)
	res = (s32)(s16)CPU->D[(Opcode >> 9) & 7];
	//(s32)res *= (s32)src;
	res = (s32)res * (s32)src;
	CPU->flag_N = res >> 24;
	CPU->flag_notZ = res;
	CPU->flag_V = CPU->flag_C = 0;
	*(u32*)(&CPU->D[(Opcode >> 9) & 7]) = res;
	POST_IO
}
RET(58)

// MULS
_OP(0xC1F0)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7];
	DECODE_EXT_WORD
	PRE_IO
	READSX_WORD_F(adr, src)
	res = (s32)(s16)CPU->D[(Opcode >> 9) & 7];
	//(s32)res *= (s32)src;
	res = (s32)res * (s32)src;
	CPU->flag_N = res >> 24;
	CPU->flag_notZ = res;
	CPU->flag_V = CPU->flag_C = 0;
	*(u32*)(&CPU->D[(Opcode >> 9) & 7]) = res;
	POST_IO
}
RET(60)

// MULS
_OP(0xC1F8)
{
	u32 adr, res;
	u32 src, dst;

	adr = (s32)(s16)FETCH_WORD;
	PC += 2;
	PRE_IO
	READSX_WORD_F(adr, src)
	res = (s32)(s16)CPU->D[(Opcode >> 9) & 7];
	//(s32)res *= (s32)src;
	res = (s32)res * (s32)src;
	CPU->flag_N = res >> 24;
	CPU->flag_notZ = res;
	CPU->flag_V = CPU->flag_C = 0;
	*(u32*)(&CPU->D[(Opcode >> 9) & 7]) = res;
	POST_IO
}
RET(58)

// MULS
_OP(0xC1F9)
{
	u32 adr, res;
	u32 src, dst;

	adr = (s32)FETCH_LONG;
	PC += 4;
	PRE_IO
	READSX_WORD_F(adr, src)
	res = (s32)(s16)CPU->D[(Opcode >> 9) & 7];
	//(s32)res *= (s32)src;
	res = (s32)res * (s32)src;
	CPU->flag_N = res >> 24;
	CPU->flag_notZ = res;
	CPU->flag_V = CPU->flag_C = 0;
	*(u32*)(&CPU->D[(Opcode >> 9) & 7]) = res;
	POST_IO
}
RET(62)

// MULS
_OP(0xC1FA)
{
	u32 adr, res;
	u32 src, dst;

	adr = (PC - CPU->BasePC) + (s32)(s16)FETCH_WORD;
	PC += 2;
	PRE_IO
	READSX_WORD_F(adr, src)
	res = (s32)(s16)CPU->D[(Opcode >> 9) & 7];
	//(s32)res *= (s32)src;
	res = (s32)res * (s32)src;
	CPU->flag_N = res >> 24;
	CPU->flag_notZ = res;
	CPU->flag_V = CPU->flag_C = 0;
	*(u32*)(&CPU->D[(Opcode >> 9) & 7]) = res;
	POST_IO
}
RET(58)

// MULS
_OP(0xC1FB)
{
	u32 adr, res;
	u32 src, dst;

	adr = PC - CPU->BasePC;
	DECODE_EXT_WORD
	PRE_IO
	READSX_WORD_F(adr, src)
	res = (s32)(s16)CPU->D[(Opcode >> 9) & 7];
	//(s32)res *= (s32)src;
	res = (s32)res * (s32)src;
	CPU->flag_N = res >> 24;
	CPU->flag_notZ = res;
	CPU->flag_V = CPU->flag_C = 0;
	*(u32*)(&CPU->D[(Opcode >> 9) & 7]) = res;
	POST_IO
}
RET(60)

// MULS
_OP(0xC1FC)
{
	u32 adr, res;
	u32 src, dst;

	src = (s32)(s16)FETCH_WORD;
	PC += 2;
	res = (s32)(s16)CPU->D[(Opcode >> 9) & 7];
	//(s32)res *= (s32)src;
	res = (s32)res * (s32)src;
	CPU->flag_N = res >> 24;
	CPU->flag_notZ = res;
	CPU->flag_V = CPU->flag_C = 0;
	*(u32*)(&CPU->D[(Opcode >> 9) & 7]) = res;
}
RET(54)

// MULS
_OP(0xC1DF)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[7];
	CPU->A[7] += 2;
	PRE_IO
	READSX_WORD_F(adr, src)
	res = (s32)(s16)CPU->D[(Opcode >> 9) & 7];
	//(s32)res *= (s32)src;
	res = (s32)res * (s32)src;
	CPU->flag_N = res >> 24;
	CPU->flag_notZ = res;
	CPU->flag_V = CPU->flag_C = 0;
	*(u32*)(&CPU->D[(Opcode >> 9) & 7]) = res;
	POST_IO
}
RET(54)

// MULS
_OP(0xC1E7)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[7] - 2;
	CPU->A[7] = adr;
	PRE_IO
	READSX_WORD_F(adr, src)
	res = (s32)(s16)CPU->D[(Opcode >> 9) & 7];
	//(s32)res *= (s32)src;
	res = (s32)res * (s32)src;
	CPU->flag_N = res >> 24;
	CPU->flag_notZ = res;
	CPU->flag_V = CPU->flag_C = 0;
	*(u32*)(&CPU->D[(Opcode >> 9) & 7]) = res;
	POST_IO
}
RET(56)

// EXGDD
_OP(0xC140)
{
	u32 adr, res;
	u32 src, dst;

	res = (u32)CPU->D[(Opcode >> 0) & 7];
	src = (u32)CPU->D[(Opcode >> 9) & 7];
	*(u32*)(&CPU->D[(Opcode >> 9) & 7]) = res;
	res = src;
	*(u32*)(&CPU->D[(Opcode >> 0) & 7]) = res;
}
RET(6)

// EXGAA
_OP(0xC148)
{
	u32 adr, res;
	u32 src, dst;

	res = (u32)CPU->A[(Opcode >> 0) & 7];
	src = (u32)CPU->A[(Opcode >> 9) & 7];
	CPU->A[(Opcode >> 9) & 7] = res;
	res = src;
	CPU->A[(Opcode >> 0) & 7] = res;
}
RET(6)

// EXGAD
_OP(0xC188)
{
	u32 adr, res;
	u32 src, dst;

	res = (u32)CPU->A[(Opcode >> 0) & 7];
	src = (u32)CPU->D[(Opcode >> 9) & 7];
	*(u32*)(&CPU->D[(Opcode >> 9) & 7]) = res;
	res = src;
	CPU->A[(Opcode >> 0) & 7] = res;
}
RET(6)
