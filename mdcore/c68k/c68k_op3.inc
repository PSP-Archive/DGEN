
// MOVEW
_OP(0x3000)
{
	u32 adr, res;
	u32 src, dst;

	res = (u16)CPU->D[(Opcode >> 0) & 7];
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	*(u16*)(&CPU->D[(Opcode >> 9) & 7]) = res;
}
RET(4)

// MOVEW
_OP(0x3080)
{
	u32 adr, res;
	u32 src, dst;

	res = (u16)CPU->D[(Opcode >> 0) & 7];
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	adr = CPU->A[(Opcode >> 9) & 7];
	PRE_IO
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(8)

// MOVEW
_OP(0x30C0)
{
	u32 adr, res;
	u32 src, dst;

	res = (u16)CPU->D[(Opcode >> 0) & 7];
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	adr = CPU->A[(Opcode >> 9) & 7];
	CPU->A[(Opcode >> 9) & 7] += 2;
	PRE_IO
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(8)

// MOVEW
_OP(0x3100)
{
	u32 adr, res;
	u32 src, dst;

	res = (u16)CPU->D[(Opcode >> 0) & 7];
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	adr = CPU->A[(Opcode >> 9) & 7] - 2;
	CPU->A[(Opcode >> 9) & 7] = adr;
	PRE_IO
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(8)

// MOVEW
_OP(0x3140)
{
	u32 adr, res;
	u32 src, dst;

	res = (u16)CPU->D[(Opcode >> 0) & 7];
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	adr = CPU->A[(Opcode >> 9) & 7] + (s32)(s16)FETCH_WORD;
	PC += 2;
	PRE_IO
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(12)

// MOVEW
_OP(0x3180)
{
	u32 adr, res;
	u32 src, dst;

	res = (u16)CPU->D[(Opcode >> 0) & 7];
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	adr = CPU->A[(Opcode >> 9) & 7];
	DECODE_EXT_WORD
	PRE_IO
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(14)

// MOVEW
_OP(0x31C0)
{
	u32 adr, res;
	u32 src, dst;

	res = (u16)CPU->D[(Opcode >> 0) & 7];
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	adr = (s32)(s16)FETCH_WORD;
	PC += 2;
	PRE_IO
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(12)

// MOVEW
_OP(0x33C0)
{
	u32 adr, res;
	u32 src, dst;

	res = (u16)CPU->D[(Opcode >> 0) & 7];
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	adr = (s32)FETCH_LONG;
	PC += 4;
	PRE_IO
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(16)

// MOVEW
_OP(0x3EC0)
{
	u32 adr, res;
	u32 src, dst;

	res = (u16)CPU->D[(Opcode >> 0) & 7];
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	adr = CPU->A[7];
	CPU->A[7] += 2;
	PRE_IO
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(8)

// MOVEW
_OP(0x3F00)
{
	u32 adr, res;
	u32 src, dst;

	res = (u16)CPU->D[(Opcode >> 0) & 7];
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	adr = CPU->A[7] - 2;
	CPU->A[7] = adr;
	PRE_IO
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(8)

// MOVEW
_OP(0x3008)
{
	u32 adr, res;
	u32 src, dst;

	res = (u16)CPU->A[(Opcode >> 0) & 7];
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	*(u16*)(&CPU->D[(Opcode >> 9) & 7]) = res;
}
RET(4)

// MOVEW
_OP(0x3088)
{
	u32 adr, res;
	u32 src, dst;

	res = (u16)CPU->A[(Opcode >> 0) & 7];
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	adr = CPU->A[(Opcode >> 9) & 7];
	PRE_IO
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(8)

// MOVEW
_OP(0x30C8)
{
	u32 adr, res;
	u32 src, dst;

	res = (u16)CPU->A[(Opcode >> 0) & 7];
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	adr = CPU->A[(Opcode >> 9) & 7];
	CPU->A[(Opcode >> 9) & 7] += 2;
	PRE_IO
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(8)

// MOVEW
_OP(0x3108)
{
	u32 adr, res;
	u32 src, dst;

	res = (u16)CPU->A[(Opcode >> 0) & 7];
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	adr = CPU->A[(Opcode >> 9) & 7] - 2;
	CPU->A[(Opcode >> 9) & 7] = adr;
	PRE_IO
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(8)

// MOVEW
_OP(0x3148)
{
	u32 adr, res;
	u32 src, dst;

	res = (u16)CPU->A[(Opcode >> 0) & 7];
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	adr = CPU->A[(Opcode >> 9) & 7] + (s32)(s16)FETCH_WORD;
	PC += 2;
	PRE_IO
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(12)

// MOVEW
_OP(0x3188)
{
	u32 adr, res;
	u32 src, dst;

	res = (u16)CPU->A[(Opcode >> 0) & 7];
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	adr = CPU->A[(Opcode >> 9) & 7];
	DECODE_EXT_WORD
	PRE_IO
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(14)

// MOVEW
_OP(0x31C8)
{
	u32 adr, res;
	u32 src, dst;

	res = (u16)CPU->A[(Opcode >> 0) & 7];
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	adr = (s32)(s16)FETCH_WORD;
	PC += 2;
	PRE_IO
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(12)

// MOVEW
_OP(0x33C8)
{
	u32 adr, res;
	u32 src, dst;

	res = (u16)CPU->A[(Opcode >> 0) & 7];
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	adr = (s32)FETCH_LONG;
	PC += 4;
	PRE_IO
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(16)

// MOVEW
_OP(0x3EC8)
{
	u32 adr, res;
	u32 src, dst;

	res = (u16)CPU->A[(Opcode >> 0) & 7];
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	adr = CPU->A[7];
	CPU->A[7] += 2;
	PRE_IO
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(8)

// MOVEW
_OP(0x3F08)
{
	u32 adr, res;
	u32 src, dst;

	res = (u16)CPU->A[(Opcode >> 0) & 7];
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	adr = CPU->A[7] - 2;
	CPU->A[7] = adr;
	PRE_IO
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(8)

// MOVEW
_OP(0x3010)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7];
	PRE_IO
	READ_WORD_F(adr, res)
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	*(u16*)(&CPU->D[(Opcode >> 9) & 7]) = res;
	POST_IO
}
RET(8)

// MOVEW
_OP(0x3090)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7];
	PRE_IO
	READ_WORD_F(adr, res)
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	adr = CPU->A[(Opcode >> 9) & 7];
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(12)

// MOVEW
_OP(0x30D0)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7];
	PRE_IO
	READ_WORD_F(adr, res)
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	adr = CPU->A[(Opcode >> 9) & 7];
	CPU->A[(Opcode >> 9) & 7] += 2;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(12)

// MOVEW
_OP(0x3110)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7];
	PRE_IO
	READ_WORD_F(adr, res)
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	adr = CPU->A[(Opcode >> 9) & 7] - 2;
	CPU->A[(Opcode >> 9) & 7] = adr;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(12)

// MOVEW
_OP(0x3150)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7];
	PRE_IO
	READ_WORD_F(adr, res)
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	adr = CPU->A[(Opcode >> 9) & 7] + (s32)(s16)FETCH_WORD;
	PC += 2;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(16)

// MOVEW
_OP(0x3190)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7];
	PRE_IO
	READ_WORD_F(adr, res)
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	adr = CPU->A[(Opcode >> 9) & 7];
	DECODE_EXT_WORD
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(18)

// MOVEW
_OP(0x31D0)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7];
	PRE_IO
	READ_WORD_F(adr, res)
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	adr = (s32)(s16)FETCH_WORD;
	PC += 2;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(16)

// MOVEW
_OP(0x33D0)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7];
	PRE_IO
	READ_WORD_F(adr, res)
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	adr = (s32)FETCH_LONG;
	PC += 4;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(20)

// MOVEW
_OP(0x3ED0)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7];
	PRE_IO
	READ_WORD_F(adr, res)
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	adr = CPU->A[7];
	CPU->A[7] += 2;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(12)

// MOVEW
_OP(0x3F10)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7];
	PRE_IO
	READ_WORD_F(adr, res)
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	adr = CPU->A[7] - 2;
	CPU->A[7] = adr;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(12)

// MOVEW
_OP(0x3018)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7];
	CPU->A[(Opcode >> 0) & 7] += 2;
	PRE_IO
	READ_WORD_F(adr, res)
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	*(u16*)(&CPU->D[(Opcode >> 9) & 7]) = res;
	POST_IO
}
RET(8)

// MOVEW
_OP(0x3098)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7];
	CPU->A[(Opcode >> 0) & 7] += 2;
	PRE_IO
	READ_WORD_F(adr, res)
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	adr = CPU->A[(Opcode >> 9) & 7];
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(12)

// MOVEW
_OP(0x30D8)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7];
	CPU->A[(Opcode >> 0) & 7] += 2;
	PRE_IO
	READ_WORD_F(adr, res)
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	adr = CPU->A[(Opcode >> 9) & 7];
	CPU->A[(Opcode >> 9) & 7] += 2;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(12)

// MOVEW
_OP(0x3118)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7];
	CPU->A[(Opcode >> 0) & 7] += 2;
	PRE_IO
	READ_WORD_F(adr, res)
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	adr = CPU->A[(Opcode >> 9) & 7] - 2;
	CPU->A[(Opcode >> 9) & 7] = adr;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(12)

// MOVEW
_OP(0x3158)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7];
	CPU->A[(Opcode >> 0) & 7] += 2;
	PRE_IO
	READ_WORD_F(adr, res)
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	adr = CPU->A[(Opcode >> 9) & 7] + (s32)(s16)FETCH_WORD;
	PC += 2;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(16)

// MOVEW
_OP(0x3198)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7];
	CPU->A[(Opcode >> 0) & 7] += 2;
	PRE_IO
	READ_WORD_F(adr, res)
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	adr = CPU->A[(Opcode >> 9) & 7];
	DECODE_EXT_WORD
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(18)

// MOVEW
_OP(0x31D8)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7];
	CPU->A[(Opcode >> 0) & 7] += 2;
	PRE_IO
	READ_WORD_F(adr, res)
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	adr = (s32)(s16)FETCH_WORD;
	PC += 2;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(16)

// MOVEW
_OP(0x33D8)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7];
	CPU->A[(Opcode >> 0) & 7] += 2;
	PRE_IO
	READ_WORD_F(adr, res)
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	adr = (s32)FETCH_LONG;
	PC += 4;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(20)

// MOVEW
_OP(0x3ED8)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7];
	CPU->A[(Opcode >> 0) & 7] += 2;
	PRE_IO
	READ_WORD_F(adr, res)
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	adr = CPU->A[7];
	CPU->A[7] += 2;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(12)

// MOVEW
_OP(0x3F18)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7];
	CPU->A[(Opcode >> 0) & 7] += 2;
	PRE_IO
	READ_WORD_F(adr, res)
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	adr = CPU->A[7] - 2;
	CPU->A[7] = adr;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(12)

// MOVEW
_OP(0x3020)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7] - 2;
	CPU->A[(Opcode >> 0) & 7] = adr;
	PRE_IO
	READ_WORD_F(adr, res)
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	*(u16*)(&CPU->D[(Opcode >> 9) & 7]) = res;
	POST_IO
}
RET(10)

// MOVEW
_OP(0x30A0)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7] - 2;
	CPU->A[(Opcode >> 0) & 7] = adr;
	PRE_IO
	READ_WORD_F(adr, res)
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	adr = CPU->A[(Opcode >> 9) & 7];
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(14)

// MOVEW
_OP(0x30E0)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7] - 2;
	CPU->A[(Opcode >> 0) & 7] = adr;
	PRE_IO
	READ_WORD_F(adr, res)
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	adr = CPU->A[(Opcode >> 9) & 7];
	CPU->A[(Opcode >> 9) & 7] += 2;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(14)

// MOVEW
_OP(0x3120)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7] - 2;
	CPU->A[(Opcode >> 0) & 7] = adr;
	PRE_IO
	READ_WORD_F(adr, res)
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	adr = CPU->A[(Opcode >> 9) & 7] - 2;
	CPU->A[(Opcode >> 9) & 7] = adr;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(14)

// MOVEW
_OP(0x3160)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7] - 2;
	CPU->A[(Opcode >> 0) & 7] = adr;
	PRE_IO
	READ_WORD_F(adr, res)
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	adr = CPU->A[(Opcode >> 9) & 7] + (s32)(s16)FETCH_WORD;
	PC += 2;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(18)

// MOVEW
_OP(0x31A0)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7] - 2;
	CPU->A[(Opcode >> 0) & 7] = adr;
	PRE_IO
	READ_WORD_F(adr, res)
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	adr = CPU->A[(Opcode >> 9) & 7];
	DECODE_EXT_WORD
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(20)

// MOVEW
_OP(0x31E0)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7] - 2;
	CPU->A[(Opcode >> 0) & 7] = adr;
	PRE_IO
	READ_WORD_F(adr, res)
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	adr = (s32)(s16)FETCH_WORD;
	PC += 2;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(18)

// MOVEW
_OP(0x33E0)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7] - 2;
	CPU->A[(Opcode >> 0) & 7] = adr;
	PRE_IO
	READ_WORD_F(adr, res)
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	adr = (s32)FETCH_LONG;
	PC += 4;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(22)

// MOVEW
_OP(0x3EE0)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7] - 2;
	CPU->A[(Opcode >> 0) & 7] = adr;
	PRE_IO
	READ_WORD_F(adr, res)
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	adr = CPU->A[7];
	CPU->A[7] += 2;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(14)

// MOVEW
_OP(0x3F20)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7] - 2;
	CPU->A[(Opcode >> 0) & 7] = adr;
	PRE_IO
	READ_WORD_F(adr, res)
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	adr = CPU->A[7] - 2;
	CPU->A[7] = adr;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(14)

// MOVEW
_OP(0x3028)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7] + (s32)(s16)FETCH_WORD;
	PC += 2;
	PRE_IO
	READ_WORD_F(adr, res)
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	*(u16*)(&CPU->D[(Opcode >> 9) & 7]) = res;
	POST_IO
}
RET(12)

// MOVEW
_OP(0x30A8)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7] + (s32)(s16)FETCH_WORD;
	PC += 2;
	PRE_IO
	READ_WORD_F(adr, res)
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	adr = CPU->A[(Opcode >> 9) & 7];
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(16)

// MOVEW
_OP(0x30E8)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7] + (s32)(s16)FETCH_WORD;
	PC += 2;
	PRE_IO
	READ_WORD_F(adr, res)
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	adr = CPU->A[(Opcode >> 9) & 7];
	CPU->A[(Opcode >> 9) & 7] += 2;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(16)

// MOVEW
_OP(0x3128)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7] + (s32)(s16)FETCH_WORD;
	PC += 2;
	PRE_IO
	READ_WORD_F(adr, res)
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	adr = CPU->A[(Opcode >> 9) & 7] - 2;
	CPU->A[(Opcode >> 9) & 7] = adr;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(16)

// MOVEW
_OP(0x3168)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7] + (s32)(s16)FETCH_WORD;
	PC += 2;
	PRE_IO
	READ_WORD_F(adr, res)
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	adr = CPU->A[(Opcode >> 9) & 7] + (s32)(s16)FETCH_WORD;
	PC += 2;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(20)

// MOVEW
_OP(0x31A8)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7] + (s32)(s16)FETCH_WORD;
	PC += 2;
	PRE_IO
	READ_WORD_F(adr, res)
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	adr = CPU->A[(Opcode >> 9) & 7];
	DECODE_EXT_WORD
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(22)

// MOVEW
_OP(0x31E8)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7] + (s32)(s16)FETCH_WORD;
	PC += 2;
	PRE_IO
	READ_WORD_F(adr, res)
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	adr = (s32)(s16)FETCH_WORD;
	PC += 2;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(20)

// MOVEW
_OP(0x33E8)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7] + (s32)(s16)FETCH_WORD;
	PC += 2;
	PRE_IO
	READ_WORD_F(adr, res)
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	adr = (s32)FETCH_LONG;
	PC += 4;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(24)

// MOVEW
_OP(0x3EE8)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7] + (s32)(s16)FETCH_WORD;
	PC += 2;
	PRE_IO
	READ_WORD_F(adr, res)
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	adr = CPU->A[7];
	CPU->A[7] += 2;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(16)

// MOVEW
_OP(0x3F28)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7] + (s32)(s16)FETCH_WORD;
	PC += 2;
	PRE_IO
	READ_WORD_F(adr, res)
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	adr = CPU->A[7] - 2;
	CPU->A[7] = adr;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(16)

// MOVEW
_OP(0x3030)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7];
	DECODE_EXT_WORD
	PRE_IO
	READ_WORD_F(adr, res)
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	*(u16*)(&CPU->D[(Opcode >> 9) & 7]) = res;
	POST_IO
}
RET(14)

// MOVEW
_OP(0x30B0)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7];
	DECODE_EXT_WORD
	PRE_IO
	READ_WORD_F(adr, res)
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	adr = CPU->A[(Opcode >> 9) & 7];
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(18)

// MOVEW
_OP(0x30F0)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7];
	DECODE_EXT_WORD
	PRE_IO
	READ_WORD_F(adr, res)
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	adr = CPU->A[(Opcode >> 9) & 7];
	CPU->A[(Opcode >> 9) & 7] += 2;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(18)

// MOVEW
_OP(0x3130)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7];
	DECODE_EXT_WORD
	PRE_IO
	READ_WORD_F(adr, res)
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	adr = CPU->A[(Opcode >> 9) & 7] - 2;
	CPU->A[(Opcode >> 9) & 7] = adr;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(18)

// MOVEW
_OP(0x3170)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7];
	DECODE_EXT_WORD
	PRE_IO
	READ_WORD_F(adr, res)
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	adr = CPU->A[(Opcode >> 9) & 7] + (s32)(s16)FETCH_WORD;
	PC += 2;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(22)

// MOVEW
_OP(0x31B0)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7];
	DECODE_EXT_WORD
	PRE_IO
	READ_WORD_F(adr, res)
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	adr = CPU->A[(Opcode >> 9) & 7];
	DECODE_EXT_WORD
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(24)

// MOVEW
_OP(0x31F0)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7];
	DECODE_EXT_WORD
	PRE_IO
	READ_WORD_F(adr, res)
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	adr = (s32)(s16)FETCH_WORD;
	PC += 2;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(22)

// MOVEW
_OP(0x33F0)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7];
	DECODE_EXT_WORD
	PRE_IO
	READ_WORD_F(adr, res)
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	adr = (s32)FETCH_LONG;
	PC += 4;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(26)

// MOVEW
_OP(0x3EF0)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7];
	DECODE_EXT_WORD
	PRE_IO
	READ_WORD_F(adr, res)
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	adr = CPU->A[7];
	CPU->A[7] += 2;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(18)

// MOVEW
_OP(0x3F30)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7];
	DECODE_EXT_WORD
	PRE_IO
	READ_WORD_F(adr, res)
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	adr = CPU->A[7] - 2;
	CPU->A[7] = adr;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(18)

// MOVEW
_OP(0x3038)
{
	u32 adr, res;
	u32 src, dst;

	adr = (s32)(s16)FETCH_WORD;
	PC += 2;
	PRE_IO
	READ_WORD_F(adr, res)
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	*(u16*)(&CPU->D[(Opcode >> 9) & 7]) = res;
	POST_IO
}
RET(12)

// MOVEW
_OP(0x30B8)
{
	u32 adr, res;
	u32 src, dst;

	adr = (s32)(s16)FETCH_WORD;
	PC += 2;
	PRE_IO
	READ_WORD_F(adr, res)
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	adr = CPU->A[(Opcode >> 9) & 7];
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(16)

// MOVEW
_OP(0x30F8)
{
	u32 adr, res;
	u32 src, dst;

	adr = (s32)(s16)FETCH_WORD;
	PC += 2;
	PRE_IO
	READ_WORD_F(adr, res)
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	adr = CPU->A[(Opcode >> 9) & 7];
	CPU->A[(Opcode >> 9) & 7] += 2;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(16)

// MOVEW
_OP(0x3138)
{
	u32 adr, res;
	u32 src, dst;

	adr = (s32)(s16)FETCH_WORD;
	PC += 2;
	PRE_IO
	READ_WORD_F(adr, res)
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	adr = CPU->A[(Opcode >> 9) & 7] - 2;
	CPU->A[(Opcode >> 9) & 7] = adr;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(16)

// MOVEW
_OP(0x3178)
{
	u32 adr, res;
	u32 src, dst;

	adr = (s32)(s16)FETCH_WORD;
	PC += 2;
	PRE_IO
	READ_WORD_F(adr, res)
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	adr = CPU->A[(Opcode >> 9) & 7] + (s32)(s16)FETCH_WORD;
	PC += 2;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(20)

// MOVEW
_OP(0x31B8)
{
	u32 adr, res;
	u32 src, dst;

	adr = (s32)(s16)FETCH_WORD;
	PC += 2;
	PRE_IO
	READ_WORD_F(adr, res)
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	adr = CPU->A[(Opcode >> 9) & 7];
	DECODE_EXT_WORD
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(22)

// MOVEW
_OP(0x31F8)
{
	u32 adr, res;
	u32 src, dst;

	adr = (s32)(s16)FETCH_WORD;
	PC += 2;
	PRE_IO
	READ_WORD_F(adr, res)
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	adr = (s32)(s16)FETCH_WORD;
	PC += 2;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(20)

// MOVEW
_OP(0x33F8)
{
	u32 adr, res;
	u32 src, dst;

	adr = (s32)(s16)FETCH_WORD;
	PC += 2;
	PRE_IO
	READ_WORD_F(adr, res)
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	adr = (s32)FETCH_LONG;
	PC += 4;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(24)

// MOVEW
_OP(0x3EF8)
{
	u32 adr, res;
	u32 src, dst;

	adr = (s32)(s16)FETCH_WORD;
	PC += 2;
	PRE_IO
	READ_WORD_F(adr, res)
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	adr = CPU->A[7];
	CPU->A[7] += 2;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(16)

// MOVEW
_OP(0x3F38)
{
	u32 adr, res;
	u32 src, dst;

	adr = (s32)(s16)FETCH_WORD;
	PC += 2;
	PRE_IO
	READ_WORD_F(adr, res)
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	adr = CPU->A[7] - 2;
	CPU->A[7] = adr;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(16)

// MOVEW
_OP(0x3039)
{
	u32 adr, res;
	u32 src, dst;

	adr = (s32)FETCH_LONG;
	PC += 4;
	PRE_IO
	READ_WORD_F(adr, res)
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	*(u16*)(&CPU->D[(Opcode >> 9) & 7]) = res;
	POST_IO
}
RET(16)

// MOVEW
_OP(0x30B9)
{
	u32 adr, res;
	u32 src, dst;

	adr = (s32)FETCH_LONG;
	PC += 4;
	PRE_IO
	READ_WORD_F(adr, res)
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	adr = CPU->A[(Opcode >> 9) & 7];
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(20)

// MOVEW
_OP(0x30F9)
{
	u32 adr, res;
	u32 src, dst;

	adr = (s32)FETCH_LONG;
	PC += 4;
	PRE_IO
	READ_WORD_F(adr, res)
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	adr = CPU->A[(Opcode >> 9) & 7];
	CPU->A[(Opcode >> 9) & 7] += 2;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(20)

// MOVEW
_OP(0x3139)
{
	u32 adr, res;
	u32 src, dst;

	adr = (s32)FETCH_LONG;
	PC += 4;
	PRE_IO
	READ_WORD_F(adr, res)
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	adr = CPU->A[(Opcode >> 9) & 7] - 2;
	CPU->A[(Opcode >> 9) & 7] = adr;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(20)

// MOVEW
_OP(0x3179)
{
	u32 adr, res;
	u32 src, dst;

	adr = (s32)FETCH_LONG;
	PC += 4;
	PRE_IO
	READ_WORD_F(adr, res)
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	adr = CPU->A[(Opcode >> 9) & 7] + (s32)(s16)FETCH_WORD;
	PC += 2;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(24)

// MOVEW
_OP(0x31B9)
{
	u32 adr, res;
	u32 src, dst;

	adr = (s32)FETCH_LONG;
	PC += 4;
	PRE_IO
	READ_WORD_F(adr, res)
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	adr = CPU->A[(Opcode >> 9) & 7];
	DECODE_EXT_WORD
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(26)

// MOVEW
_OP(0x31F9)
{
	u32 adr, res;
	u32 src, dst;

	adr = (s32)FETCH_LONG;
	PC += 4;
	PRE_IO
	READ_WORD_F(adr, res)
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	adr = (s32)(s16)FETCH_WORD;
	PC += 2;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(24)

// MOVEW
_OP(0x33F9)
{
	u32 adr, res;
	u32 src, dst;

	adr = (s32)FETCH_LONG;
	PC += 4;
	PRE_IO
	READ_WORD_F(adr, res)
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	adr = (s32)FETCH_LONG;
	PC += 4;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(28)

// MOVEW
_OP(0x3EF9)
{
	u32 adr, res;
	u32 src, dst;

	adr = (s32)FETCH_LONG;
	PC += 4;
	PRE_IO
	READ_WORD_F(adr, res)
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	adr = CPU->A[7];
	CPU->A[7] += 2;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(20)

// MOVEW
_OP(0x3F39)
{
	u32 adr, res;
	u32 src, dst;

	adr = (s32)FETCH_LONG;
	PC += 4;
	PRE_IO
	READ_WORD_F(adr, res)
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	adr = CPU->A[7] - 2;
	CPU->A[7] = adr;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(20)

// MOVEW
_OP(0x303A)
{
	u32 adr, res;
	u32 src, dst;

	adr = (PC - CPU->BasePC) + (s32)(s16)FETCH_WORD;
	PC += 2;
	PRE_IO
	READ_WORD_F(adr, res)
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	*(u16*)(&CPU->D[(Opcode >> 9) & 7]) = res;
	POST_IO
}
RET(12)

// MOVEW
_OP(0x30BA)
{
	u32 adr, res;
	u32 src, dst;

	adr = (PC - CPU->BasePC) + (s32)(s16)FETCH_WORD;
	PC += 2;
	PRE_IO
	READ_WORD_F(adr, res)
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	adr = CPU->A[(Opcode >> 9) & 7];
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(16)

// MOVEW
_OP(0x30FA)
{
	u32 adr, res;
	u32 src, dst;

	adr = (PC - CPU->BasePC) + (s32)(s16)FETCH_WORD;
	PC += 2;
	PRE_IO
	READ_WORD_F(adr, res)
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	adr = CPU->A[(Opcode >> 9) & 7];
	CPU->A[(Opcode >> 9) & 7] += 2;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(16)

// MOVEW
_OP(0x313A)
{
	u32 adr, res;
	u32 src, dst;

	adr = (PC - CPU->BasePC) + (s32)(s16)FETCH_WORD;
	PC += 2;
	PRE_IO
	READ_WORD_F(adr, res)
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	adr = CPU->A[(Opcode >> 9) & 7] - 2;
	CPU->A[(Opcode >> 9) & 7] = adr;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(16)

// MOVEW
_OP(0x317A)
{
	u32 adr, res;
	u32 src, dst;

	adr = (PC - CPU->BasePC) + (s32)(s16)FETCH_WORD;
	PC += 2;
	PRE_IO
	READ_WORD_F(adr, res)
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	adr = CPU->A[(Opcode >> 9) & 7] + (s32)(s16)FETCH_WORD;
	PC += 2;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(20)

// MOVEW
_OP(0x31BA)
{
	u32 adr, res;
	u32 src, dst;

	adr = (PC - CPU->BasePC) + (s32)(s16)FETCH_WORD;
	PC += 2;
	PRE_IO
	READ_WORD_F(adr, res)
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	adr = CPU->A[(Opcode >> 9) & 7];
	DECODE_EXT_WORD
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(22)

// MOVEW
_OP(0x31FA)
{
	u32 adr, res;
	u32 src, dst;

	adr = (PC - CPU->BasePC) + (s32)(s16)FETCH_WORD;
	PC += 2;
	PRE_IO
	READ_WORD_F(adr, res)
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	adr = (s32)(s16)FETCH_WORD;
	PC += 2;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(20)

// MOVEW
_OP(0x33FA)
{
	u32 adr, res;
	u32 src, dst;

	adr = (PC - CPU->BasePC) + (s32)(s16)FETCH_WORD;
	PC += 2;
	PRE_IO
	READ_WORD_F(adr, res)
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	adr = (s32)FETCH_LONG;
	PC += 4;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(24)

// MOVEW
_OP(0x3EFA)
{
	u32 adr, res;
	u32 src, dst;

	adr = (PC - CPU->BasePC) + (s32)(s16)FETCH_WORD;
	PC += 2;
	PRE_IO
	READ_WORD_F(adr, res)
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	adr = CPU->A[7];
	CPU->A[7] += 2;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(16)

// MOVEW
_OP(0x3F3A)
{
	u32 adr, res;
	u32 src, dst;

	adr = (PC - CPU->BasePC) + (s32)(s16)FETCH_WORD;
	PC += 2;
	PRE_IO
	READ_WORD_F(adr, res)
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	adr = CPU->A[7] - 2;
	CPU->A[7] = adr;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(16)

// MOVEW
_OP(0x303B)
{
	u32 adr, res;
	u32 src, dst;

	adr = PC - CPU->BasePC;
	DECODE_EXT_WORD
	PRE_IO
	READ_WORD_F(adr, res)
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	*(u16*)(&CPU->D[(Opcode >> 9) & 7]) = res;
	POST_IO
}
RET(14)

// MOVEW
_OP(0x30BB)
{
	u32 adr, res;
	u32 src, dst;

	adr = PC - CPU->BasePC;
	DECODE_EXT_WORD
	PRE_IO
	READ_WORD_F(adr, res)
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	adr = CPU->A[(Opcode >> 9) & 7];
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(18)

// MOVEW
_OP(0x30FB)
{
	u32 adr, res;
	u32 src, dst;

	adr = PC - CPU->BasePC;
	DECODE_EXT_WORD
	PRE_IO
	READ_WORD_F(adr, res)
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	adr = CPU->A[(Opcode >> 9) & 7];
	CPU->A[(Opcode >> 9) & 7] += 2;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(18)

// MOVEW
_OP(0x313B)
{
	u32 adr, res;
	u32 src, dst;

	adr = PC - CPU->BasePC;
	DECODE_EXT_WORD
	PRE_IO
	READ_WORD_F(adr, res)
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	adr = CPU->A[(Opcode >> 9) & 7] - 2;
	CPU->A[(Opcode >> 9) & 7] = adr;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(18)

// MOVEW
_OP(0x317B)
{
	u32 adr, res;
	u32 src, dst;

	adr = PC - CPU->BasePC;
	DECODE_EXT_WORD
	PRE_IO
	READ_WORD_F(adr, res)
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	adr = CPU->A[(Opcode >> 9) & 7] + (s32)(s16)FETCH_WORD;
	PC += 2;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(22)

// MOVEW
_OP(0x31BB)
{
	u32 adr, res;
	u32 src, dst;

	adr = PC - CPU->BasePC;
	DECODE_EXT_WORD
	PRE_IO
	READ_WORD_F(adr, res)
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	adr = CPU->A[(Opcode >> 9) & 7];
	DECODE_EXT_WORD
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(24)

// MOVEW
_OP(0x31FB)
{
	u32 adr, res;
	u32 src, dst;

	adr = PC - CPU->BasePC;
	DECODE_EXT_WORD
	PRE_IO
	READ_WORD_F(adr, res)
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	adr = (s32)(s16)FETCH_WORD;
	PC += 2;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(22)

// MOVEW
_OP(0x33FB)
{
	u32 adr, res;
	u32 src, dst;

	adr = PC - CPU->BasePC;
	DECODE_EXT_WORD
	PRE_IO
	READ_WORD_F(adr, res)
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	adr = (s32)FETCH_LONG;
	PC += 4;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(26)

// MOVEW
_OP(0x3EFB)
{
	u32 adr, res;
	u32 src, dst;

	adr = PC - CPU->BasePC;
	DECODE_EXT_WORD
	PRE_IO
	READ_WORD_F(adr, res)
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	adr = CPU->A[7];
	CPU->A[7] += 2;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(18)

// MOVEW
_OP(0x3F3B)
{
	u32 adr, res;
	u32 src, dst;

	adr = PC - CPU->BasePC;
	DECODE_EXT_WORD
	PRE_IO
	READ_WORD_F(adr, res)
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	adr = CPU->A[7] - 2;
	CPU->A[7] = adr;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(18)

// MOVEW
_OP(0x303C)
{
	u32 adr, res;
	u32 src, dst;

	res = FETCH_WORD;
	PC += 2;
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	*(u16*)(&CPU->D[(Opcode >> 9) & 7]) = res;
}
RET(8)

// MOVEW
_OP(0x30BC)
{
	u32 adr, res;
	u32 src, dst;

	res = FETCH_WORD;
	PC += 2;
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	adr = CPU->A[(Opcode >> 9) & 7];
	PRE_IO
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(12)

// MOVEW
_OP(0x30FC)
{
	u32 adr, res;
	u32 src, dst;

	res = FETCH_WORD;
	PC += 2;
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	adr = CPU->A[(Opcode >> 9) & 7];
	CPU->A[(Opcode >> 9) & 7] += 2;
	PRE_IO
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(12)

// MOVEW
_OP(0x313C)
{
	u32 adr, res;
	u32 src, dst;

	res = FETCH_WORD;
	PC += 2;
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	adr = CPU->A[(Opcode >> 9) & 7] - 2;
	CPU->A[(Opcode >> 9) & 7] = adr;
	PRE_IO
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(12)

// MOVEW
_OP(0x317C)
{
	u32 adr, res;
	u32 src, dst;

	res = FETCH_WORD;
	PC += 2;
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	adr = CPU->A[(Opcode >> 9) & 7] + (s32)(s16)FETCH_WORD;
	PC += 2;
	PRE_IO
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(16)

// MOVEW
_OP(0x31BC)
{
	u32 adr, res;
	u32 src, dst;

	res = FETCH_WORD;
	PC += 2;
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	adr = CPU->A[(Opcode >> 9) & 7];
	DECODE_EXT_WORD
	PRE_IO
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(18)

// MOVEW
_OP(0x31FC)
{
	u32 adr, res;
	u32 src, dst;

	res = FETCH_WORD;
	PC += 2;
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	adr = (s32)(s16)FETCH_WORD;
	PC += 2;
	PRE_IO
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(16)

// MOVEW
_OP(0x33FC)
{
	u32 adr, res;
	u32 src, dst;

	res = FETCH_WORD;
	PC += 2;
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	adr = (s32)FETCH_LONG;
	PC += 4;
	PRE_IO
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(20)

// MOVEW
_OP(0x3EFC)
{
	u32 adr, res;
	u32 src, dst;

	res = FETCH_WORD;
	PC += 2;
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	adr = CPU->A[7];
	CPU->A[7] += 2;
	PRE_IO
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(12)

// MOVEW
_OP(0x3F3C)
{
	u32 adr, res;
	u32 src, dst;

	res = FETCH_WORD;
	PC += 2;
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	adr = CPU->A[7] - 2;
	CPU->A[7] = adr;
	PRE_IO
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(12)

// MOVEW
_OP(0x301F)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[7];
	CPU->A[7] += 2;
	PRE_IO
	READ_WORD_F(adr, res)
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	*(u16*)(&CPU->D[(Opcode >> 9) & 7]) = res;
	POST_IO
}
RET(8)

// MOVEW
_OP(0x309F)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[7];
	CPU->A[7] += 2;
	PRE_IO
	READ_WORD_F(adr, res)
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	adr = CPU->A[(Opcode >> 9) & 7];
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(12)

// MOVEW
_OP(0x30DF)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[7];
	CPU->A[7] += 2;
	PRE_IO
	READ_WORD_F(adr, res)
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	adr = CPU->A[(Opcode >> 9) & 7];
	CPU->A[(Opcode >> 9) & 7] += 2;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(12)

// MOVEW
_OP(0x311F)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[7];
	CPU->A[7] += 2;
	PRE_IO
	READ_WORD_F(adr, res)
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	adr = CPU->A[(Opcode >> 9) & 7] - 2;
	CPU->A[(Opcode >> 9) & 7] = adr;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(12)

// MOVEW
_OP(0x315F)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[7];
	CPU->A[7] += 2;
	PRE_IO
	READ_WORD_F(adr, res)
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	adr = CPU->A[(Opcode >> 9) & 7] + (s32)(s16)FETCH_WORD;
	PC += 2;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(16)

// MOVEW
_OP(0x319F)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[7];
	CPU->A[7] += 2;
	PRE_IO
	READ_WORD_F(adr, res)
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	adr = CPU->A[(Opcode >> 9) & 7];
	DECODE_EXT_WORD
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(18)

// MOVEW
_OP(0x31DF)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[7];
	CPU->A[7] += 2;
	PRE_IO
	READ_WORD_F(adr, res)
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	adr = (s32)(s16)FETCH_WORD;
	PC += 2;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(16)

// MOVEW
_OP(0x33DF)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[7];
	CPU->A[7] += 2;
	PRE_IO
	READ_WORD_F(adr, res)
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	adr = (s32)FETCH_LONG;
	PC += 4;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(20)

// MOVEW
_OP(0x3EDF)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[7];
	CPU->A[7] += 2;
	PRE_IO
	READ_WORD_F(adr, res)
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	adr = CPU->A[7];
	CPU->A[7] += 2;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(12)

// MOVEW
_OP(0x3F1F)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[7];
	CPU->A[7] += 2;
	PRE_IO
	READ_WORD_F(adr, res)
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	adr = CPU->A[7] - 2;
	CPU->A[7] = adr;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(12)

// MOVEW
_OP(0x3027)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[7] - 2;
	CPU->A[7] = adr;
	PRE_IO
	READ_WORD_F(adr, res)
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	*(u16*)(&CPU->D[(Opcode >> 9) & 7]) = res;
	POST_IO
}
RET(10)

// MOVEW
_OP(0x30A7)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[7] - 2;
	CPU->A[7] = adr;
	PRE_IO
	READ_WORD_F(adr, res)
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	adr = CPU->A[(Opcode >> 9) & 7];
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(14)

// MOVEW
_OP(0x30E7)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[7] - 2;
	CPU->A[7] = adr;
	PRE_IO
	READ_WORD_F(adr, res)
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	adr = CPU->A[(Opcode >> 9) & 7];
	CPU->A[(Opcode >> 9) & 7] += 2;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(14)

// MOVEW
_OP(0x3127)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[7] - 2;
	CPU->A[7] = adr;
	PRE_IO
	READ_WORD_F(adr, res)
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	adr = CPU->A[(Opcode >> 9) & 7] - 2;
	CPU->A[(Opcode >> 9) & 7] = adr;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(14)

// MOVEW
_OP(0x3167)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[7] - 2;
	CPU->A[7] = adr;
	PRE_IO
	READ_WORD_F(adr, res)
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	adr = CPU->A[(Opcode >> 9) & 7] + (s32)(s16)FETCH_WORD;
	PC += 2;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(18)

// MOVEW
_OP(0x31A7)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[7] - 2;
	CPU->A[7] = adr;
	PRE_IO
	READ_WORD_F(adr, res)
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	adr = CPU->A[(Opcode >> 9) & 7];
	DECODE_EXT_WORD
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(20)

// MOVEW
_OP(0x31E7)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[7] - 2;
	CPU->A[7] = adr;
	PRE_IO
	READ_WORD_F(adr, res)
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	adr = (s32)(s16)FETCH_WORD;
	PC += 2;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(18)

// MOVEW
_OP(0x33E7)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[7] - 2;
	CPU->A[7] = adr;
	PRE_IO
	READ_WORD_F(adr, res)
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	adr = (s32)FETCH_LONG;
	PC += 4;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(22)

// MOVEW
_OP(0x3EE7)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[7] - 2;
	CPU->A[7] = adr;
	PRE_IO
	READ_WORD_F(adr, res)
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	adr = CPU->A[7];
	CPU->A[7] += 2;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(14)

// MOVEW
_OP(0x3F27)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[7] - 2;
	CPU->A[7] = adr;
	PRE_IO
	READ_WORD_F(adr, res)
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	adr = CPU->A[7] - 2;
	CPU->A[7] = adr;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(14)

// MOVEAW
_OP(0x3040)
{
	u32 adr, res;
	u32 src, dst;

	res = (s32)(s16)CPU->D[(Opcode >> 0) & 7];
	CPU->A[(Opcode >> 9) & 7] = res;
}
RET(4)

// MOVEAW
_OP(0x3048)
{
	u32 adr, res;
	u32 src, dst;

	res = (s32)(s16)CPU->A[(Opcode >> 0) & 7];
	CPU->A[(Opcode >> 9) & 7] = res;
}
RET(4)

// MOVEAW
_OP(0x3050)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7];
	PRE_IO
	READSX_WORD_F(adr, res)
	CPU->A[(Opcode >> 9) & 7] = res;
	POST_IO
}
RET(8)

// MOVEAW
_OP(0x3058)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7];
	CPU->A[(Opcode >> 0) & 7] += 2;
	PRE_IO
	READSX_WORD_F(adr, res)
	CPU->A[(Opcode >> 9) & 7] = res;
	POST_IO
}
RET(8)

// MOVEAW
_OP(0x3060)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7] - 2;
	CPU->A[(Opcode >> 0) & 7] = adr;
	PRE_IO
	READSX_WORD_F(adr, res)
	CPU->A[(Opcode >> 9) & 7] = res;
	POST_IO
}
RET(10)

// MOVEAW
_OP(0x3068)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7] + (s32)(s16)FETCH_WORD;
	PC += 2;
	PRE_IO
	READSX_WORD_F(adr, res)
	CPU->A[(Opcode >> 9) & 7] = res;
	POST_IO
}
RET(12)

// MOVEAW
_OP(0x3070)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7];
	DECODE_EXT_WORD
	PRE_IO
	READSX_WORD_F(adr, res)
	CPU->A[(Opcode >> 9) & 7] = res;
	POST_IO
}
RET(14)

// MOVEAW
_OP(0x3078)
{
	u32 adr, res;
	u32 src, dst;

	adr = (s32)(s16)FETCH_WORD;
	PC += 2;
	PRE_IO
	READSX_WORD_F(adr, res)
	CPU->A[(Opcode >> 9) & 7] = res;
	POST_IO
}
RET(12)

// MOVEAW
_OP(0x3079)
{
	u32 adr, res;
	u32 src, dst;

	adr = (s32)FETCH_LONG;
	PC += 4;
	PRE_IO
	READSX_WORD_F(adr, res)
	CPU->A[(Opcode >> 9) & 7] = res;
	POST_IO
}
RET(16)

// MOVEAW
_OP(0x307A)
{
	u32 adr, res;
	u32 src, dst;

	adr = (PC - CPU->BasePC) + (s32)(s16)FETCH_WORD;
	PC += 2;
	PRE_IO
	READSX_WORD_F(adr, res)
	CPU->A[(Opcode >> 9) & 7] = res;
	POST_IO
}
RET(12)

// MOVEAW
_OP(0x307B)
{
	u32 adr, res;
	u32 src, dst;

	adr = PC - CPU->BasePC;
	DECODE_EXT_WORD
	PRE_IO
	READSX_WORD_F(adr, res)
	CPU->A[(Opcode >> 9) & 7] = res;
	POST_IO
}
RET(14)

// MOVEAW
_OP(0x307C)
{
	u32 adr, res;
	u32 src, dst;

	res = (s32)(s16)FETCH_WORD;
	PC += 2;
	CPU->A[(Opcode >> 9) & 7] = res;
}
RET(8)

// MOVEAW
_OP(0x305F)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[7];
	CPU->A[7] += 2;
	PRE_IO
	READSX_WORD_F(adr, res)
	CPU->A[(Opcode >> 9) & 7] = res;
	POST_IO
}
RET(8)

// MOVEAW
_OP(0x3067)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[7] - 2;
	CPU->A[7] = adr;
	PRE_IO
	READSX_WORD_F(adr, res)
	CPU->A[(Opcode >> 9) & 7] = res;
	POST_IO
}
RET(10)
