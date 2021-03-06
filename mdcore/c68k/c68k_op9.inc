
// SUBaD
_OP(0x9000)
{
	u32 adr, res;
	u32 src, dst;

	src = (u8)CPU->D[(Opcode >> 0) & 7];
	dst = (u8)CPU->D[(Opcode >> 9) & 7];
	res = dst - src;
	CPU->flag_N = CPU->flag_X = CPU->flag_C = res;
	CPU->flag_V = (src ^ dst) & (res ^ dst);
	CPU->flag_notZ = res & 0xFF;
	*(u8*)(&CPU->D[(Opcode >> 9) & 7]) = res;
}
RET(4)

// SUBaD
_OP(0x9008)
{
	u32 adr, res;
	u32 src, dst;

	// can't read byte from Ax registers !
	CPU->Status |= C68K_FAULTED;
	CCnt = 0;
	//warning!!
	//goto C68k_Exec_Really_End;
	FAULTED() ;
	dst = (u8)CPU->D[(Opcode >> 9) & 7];
	res = dst - src;
	CPU->flag_N = CPU->flag_X = CPU->flag_C = res;
	CPU->flag_V = (src ^ dst) & (res ^ dst);
	CPU->flag_notZ = res & 0xFF;
	*(u8*)(&CPU->D[(Opcode >> 9) & 7]) = res;
}
RET(4)

// SUBaD
_OP(0x9010)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7];
	PRE_IO
	READ_BYTE_F(adr, src)
	dst = (u8)CPU->D[(Opcode >> 9) & 7];
	res = dst - src;
	CPU->flag_N = CPU->flag_X = CPU->flag_C = res;
	CPU->flag_V = (src ^ dst) & (res ^ dst);
	CPU->flag_notZ = res & 0xFF;
	*(u8*)(&CPU->D[(Opcode >> 9) & 7]) = res;
	POST_IO
}
RET(8)

// SUBaD
_OP(0x9018)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7];
	CPU->A[(Opcode >> 0) & 7] += 1;
	PRE_IO
	READ_BYTE_F(adr, src)
	dst = (u8)CPU->D[(Opcode >> 9) & 7];
	res = dst - src;
	CPU->flag_N = CPU->flag_X = CPU->flag_C = res;
	CPU->flag_V = (src ^ dst) & (res ^ dst);
	CPU->flag_notZ = res & 0xFF;
	*(u8*)(&CPU->D[(Opcode >> 9) & 7]) = res;
	POST_IO
}
RET(8)

// SUBaD
_OP(0x9020)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7] - 1;
	CPU->A[(Opcode >> 0) & 7] = adr;
	PRE_IO
	READ_BYTE_F(adr, src)
	dst = (u8)CPU->D[(Opcode >> 9) & 7];
	res = dst - src;
	CPU->flag_N = CPU->flag_X = CPU->flag_C = res;
	CPU->flag_V = (src ^ dst) & (res ^ dst);
	CPU->flag_notZ = res & 0xFF;
	*(u8*)(&CPU->D[(Opcode >> 9) & 7]) = res;
	POST_IO
}
RET(10)

// SUBaD
_OP(0x9028)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7] + (s32)(s16)FETCH_WORD;
	PC += 2;
	PRE_IO
	READ_BYTE_F(adr, src)
	dst = (u8)CPU->D[(Opcode >> 9) & 7];
	res = dst - src;
	CPU->flag_N = CPU->flag_X = CPU->flag_C = res;
	CPU->flag_V = (src ^ dst) & (res ^ dst);
	CPU->flag_notZ = res & 0xFF;
	*(u8*)(&CPU->D[(Opcode >> 9) & 7]) = res;
	POST_IO
}
RET(12)

// SUBaD
_OP(0x9030)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7];
	DECODE_EXT_WORD
	PRE_IO
	READ_BYTE_F(adr, src)
	dst = (u8)CPU->D[(Opcode >> 9) & 7];
	res = dst - src;
	CPU->flag_N = CPU->flag_X = CPU->flag_C = res;
	CPU->flag_V = (src ^ dst) & (res ^ dst);
	CPU->flag_notZ = res & 0xFF;
	*(u8*)(&CPU->D[(Opcode >> 9) & 7]) = res;
	POST_IO
}
RET(14)

// SUBaD
_OP(0x9038)
{
	u32 adr, res;
	u32 src, dst;

	adr = (s32)(s16)FETCH_WORD;
	PC += 2;
	PRE_IO
	READ_BYTE_F(adr, src)
	dst = (u8)CPU->D[(Opcode >> 9) & 7];
	res = dst - src;
	CPU->flag_N = CPU->flag_X = CPU->flag_C = res;
	CPU->flag_V = (src ^ dst) & (res ^ dst);
	CPU->flag_notZ = res & 0xFF;
	*(u8*)(&CPU->D[(Opcode >> 9) & 7]) = res;
	POST_IO
}
RET(12)

// SUBaD
_OP(0x9039)
{
	u32 adr, res;
	u32 src, dst;

	adr = (s32)FETCH_LONG;
	PC += 4;
	PRE_IO
	READ_BYTE_F(adr, src)
	dst = (u8)CPU->D[(Opcode >> 9) & 7];
	res = dst - src;
	CPU->flag_N = CPU->flag_X = CPU->flag_C = res;
	CPU->flag_V = (src ^ dst) & (res ^ dst);
	CPU->flag_notZ = res & 0xFF;
	*(u8*)(&CPU->D[(Opcode >> 9) & 7]) = res;
	POST_IO
}
RET(16)

// SUBaD
_OP(0x903A)
{
	u32 adr, res;
	u32 src, dst;

	adr = (PC - CPU->BasePC) + (s32)(s16)FETCH_WORD;
	PC += 2;
	PRE_IO
	READ_BYTE_F(adr, src)
	dst = (u8)CPU->D[(Opcode >> 9) & 7];
	res = dst - src;
	CPU->flag_N = CPU->flag_X = CPU->flag_C = res;
	CPU->flag_V = (src ^ dst) & (res ^ dst);
	CPU->flag_notZ = res & 0xFF;
	*(u8*)(&CPU->D[(Opcode >> 9) & 7]) = res;
	POST_IO
}
RET(12)

// SUBaD
_OP(0x903B)
{
	u32 adr, res;
	u32 src, dst;

	adr = PC - CPU->BasePC;
	DECODE_EXT_WORD
	PRE_IO
	READ_BYTE_F(adr, src)
	dst = (u8)CPU->D[(Opcode >> 9) & 7];
	res = dst - src;
	CPU->flag_N = CPU->flag_X = CPU->flag_C = res;
	CPU->flag_V = (src ^ dst) & (res ^ dst);
	CPU->flag_notZ = res & 0xFF;
	*(u8*)(&CPU->D[(Opcode >> 9) & 7]) = res;
	POST_IO
}
RET(14)

// SUBaD
_OP(0x903C)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_BYTE;
	PC += 2;
	dst = (u8)CPU->D[(Opcode >> 9) & 7];
	res = dst - src;
	CPU->flag_N = CPU->flag_X = CPU->flag_C = res;
	CPU->flag_V = (src ^ dst) & (res ^ dst);
	CPU->flag_notZ = res & 0xFF;
	*(u8*)(&CPU->D[(Opcode >> 9) & 7]) = res;
}
RET(8)

// SUBaD
_OP(0x901F)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[7];
	CPU->A[7] += 2;
	PRE_IO
	READ_BYTE_F(adr, src)
	dst = (u8)CPU->D[(Opcode >> 9) & 7];
	res = dst - src;
	CPU->flag_N = CPU->flag_X = CPU->flag_C = res;
	CPU->flag_V = (src ^ dst) & (res ^ dst);
	CPU->flag_notZ = res & 0xFF;
	*(u8*)(&CPU->D[(Opcode >> 9) & 7]) = res;
	POST_IO
}
RET(8)

// SUBaD
_OP(0x9027)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[7] - 2;
	CPU->A[7] = adr;
	PRE_IO
	READ_BYTE_F(adr, src)
	dst = (u8)CPU->D[(Opcode >> 9) & 7];
	res = dst - src;
	CPU->flag_N = CPU->flag_X = CPU->flag_C = res;
	CPU->flag_V = (src ^ dst) & (res ^ dst);
	CPU->flag_notZ = res & 0xFF;
	*(u8*)(&CPU->D[(Opcode >> 9) & 7]) = res;
	POST_IO
}
RET(10)

// SUBaD
_OP(0x9040)
{
	u32 adr, res;
	u32 src, dst;

	src = (u16)CPU->D[(Opcode >> 0) & 7];
	dst = (u16)CPU->D[(Opcode >> 9) & 7];
	res = dst - src;
	CPU->flag_V = ((src ^ dst) & (res ^ dst)) >> 8;
	CPU->flag_N = CPU->flag_X = CPU->flag_C = res >> 8;
	CPU->flag_notZ = res & 0xFFFF;
	*(u16*)(&CPU->D[(Opcode >> 9) & 7]) = res;
}
RET(4)

// SUBaD
_OP(0x9048)
{
	u32 adr, res;
	u32 src, dst;

	src = (u16)CPU->A[(Opcode >> 0) & 7];
	dst = (u16)CPU->D[(Opcode >> 9) & 7];
	res = dst - src;
	CPU->flag_V = ((src ^ dst) & (res ^ dst)) >> 8;
	CPU->flag_N = CPU->flag_X = CPU->flag_C = res >> 8;
	CPU->flag_notZ = res & 0xFFFF;
	*(u16*)(&CPU->D[(Opcode >> 9) & 7]) = res;
}
RET(4)

// SUBaD
_OP(0x9050)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7];
	PRE_IO
	READ_WORD_F(adr, src)
	dst = (u16)CPU->D[(Opcode >> 9) & 7];
	res = dst - src;
	CPU->flag_V = ((src ^ dst) & (res ^ dst)) >> 8;
	CPU->flag_N = CPU->flag_X = CPU->flag_C = res >> 8;
	CPU->flag_notZ = res & 0xFFFF;
	*(u16*)(&CPU->D[(Opcode >> 9) & 7]) = res;
	POST_IO
}
RET(8)

// SUBaD
_OP(0x9058)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7];
	CPU->A[(Opcode >> 0) & 7] += 2;
	PRE_IO
	READ_WORD_F(adr, src)
	dst = (u16)CPU->D[(Opcode >> 9) & 7];
	res = dst - src;
	CPU->flag_V = ((src ^ dst) & (res ^ dst)) >> 8;
	CPU->flag_N = CPU->flag_X = CPU->flag_C = res >> 8;
	CPU->flag_notZ = res & 0xFFFF;
	*(u16*)(&CPU->D[(Opcode >> 9) & 7]) = res;
	POST_IO
}
RET(8)

// SUBaD
_OP(0x9060)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7] - 2;
	CPU->A[(Opcode >> 0) & 7] = adr;
	PRE_IO
	READ_WORD_F(adr, src)
	dst = (u16)CPU->D[(Opcode >> 9) & 7];
	res = dst - src;
	CPU->flag_V = ((src ^ dst) & (res ^ dst)) >> 8;
	CPU->flag_N = CPU->flag_X = CPU->flag_C = res >> 8;
	CPU->flag_notZ = res & 0xFFFF;
	*(u16*)(&CPU->D[(Opcode >> 9) & 7]) = res;
	POST_IO
}
RET(10)

// SUBaD
_OP(0x9068)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7] + (s32)(s16)FETCH_WORD;
	PC += 2;
	PRE_IO
	READ_WORD_F(adr, src)
	dst = (u16)CPU->D[(Opcode >> 9) & 7];
	res = dst - src;
	CPU->flag_V = ((src ^ dst) & (res ^ dst)) >> 8;
	CPU->flag_N = CPU->flag_X = CPU->flag_C = res >> 8;
	CPU->flag_notZ = res & 0xFFFF;
	*(u16*)(&CPU->D[(Opcode >> 9) & 7]) = res;
	POST_IO
}
RET(12)

// SUBaD
_OP(0x9070)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7];
	DECODE_EXT_WORD
	PRE_IO
	READ_WORD_F(adr, src)
	dst = (u16)CPU->D[(Opcode >> 9) & 7];
	res = dst - src;
	CPU->flag_V = ((src ^ dst) & (res ^ dst)) >> 8;
	CPU->flag_N = CPU->flag_X = CPU->flag_C = res >> 8;
	CPU->flag_notZ = res & 0xFFFF;
	*(u16*)(&CPU->D[(Opcode >> 9) & 7]) = res;
	POST_IO
}
RET(14)

// SUBaD
_OP(0x9078)
{
	u32 adr, res;
	u32 src, dst;

	adr = (s32)(s16)FETCH_WORD;
	PC += 2;
	PRE_IO
	READ_WORD_F(adr, src)
	dst = (u16)CPU->D[(Opcode >> 9) & 7];
	res = dst - src;
	CPU->flag_V = ((src ^ dst) & (res ^ dst)) >> 8;
	CPU->flag_N = CPU->flag_X = CPU->flag_C = res >> 8;
	CPU->flag_notZ = res & 0xFFFF;
	*(u16*)(&CPU->D[(Opcode >> 9) & 7]) = res;
	POST_IO
}
RET(12)

// SUBaD
_OP(0x9079)
{
	u32 adr, res;
	u32 src, dst;

	adr = (s32)FETCH_LONG;
	PC += 4;
	PRE_IO
	READ_WORD_F(adr, src)
	dst = (u16)CPU->D[(Opcode >> 9) & 7];
	res = dst - src;
	CPU->flag_V = ((src ^ dst) & (res ^ dst)) >> 8;
	CPU->flag_N = CPU->flag_X = CPU->flag_C = res >> 8;
	CPU->flag_notZ = res & 0xFFFF;
	*(u16*)(&CPU->D[(Opcode >> 9) & 7]) = res;
	POST_IO
}
RET(16)

// SUBaD
_OP(0x907A)
{
	u32 adr, res;
	u32 src, dst;

	adr = (PC - CPU->BasePC) + (s32)(s16)FETCH_WORD;
	PC += 2;
	PRE_IO
	READ_WORD_F(adr, src)
	dst = (u16)CPU->D[(Opcode >> 9) & 7];
	res = dst - src;
	CPU->flag_V = ((src ^ dst) & (res ^ dst)) >> 8;
	CPU->flag_N = CPU->flag_X = CPU->flag_C = res >> 8;
	CPU->flag_notZ = res & 0xFFFF;
	*(u16*)(&CPU->D[(Opcode >> 9) & 7]) = res;
	POST_IO
}
RET(12)

// SUBaD
_OP(0x907B)
{
	u32 adr, res;
	u32 src, dst;

	adr = PC - CPU->BasePC;
	DECODE_EXT_WORD
	PRE_IO
	READ_WORD_F(adr, src)
	dst = (u16)CPU->D[(Opcode >> 9) & 7];
	res = dst - src;
	CPU->flag_V = ((src ^ dst) & (res ^ dst)) >> 8;
	CPU->flag_N = CPU->flag_X = CPU->flag_C = res >> 8;
	CPU->flag_notZ = res & 0xFFFF;
	*(u16*)(&CPU->D[(Opcode >> 9) & 7]) = res;
	POST_IO
}
RET(14)

// SUBaD
_OP(0x907C)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_WORD;
	PC += 2;
	dst = (u16)CPU->D[(Opcode >> 9) & 7];
	res = dst - src;
	CPU->flag_V = ((src ^ dst) & (res ^ dst)) >> 8;
	CPU->flag_N = CPU->flag_X = CPU->flag_C = res >> 8;
	CPU->flag_notZ = res & 0xFFFF;
	*(u16*)(&CPU->D[(Opcode >> 9) & 7]) = res;
}
RET(8)

// SUBaD
_OP(0x905F)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[7];
	CPU->A[7] += 2;
	PRE_IO
	READ_WORD_F(adr, src)
	dst = (u16)CPU->D[(Opcode >> 9) & 7];
	res = dst - src;
	CPU->flag_V = ((src ^ dst) & (res ^ dst)) >> 8;
	CPU->flag_N = CPU->flag_X = CPU->flag_C = res >> 8;
	CPU->flag_notZ = res & 0xFFFF;
	*(u16*)(&CPU->D[(Opcode >> 9) & 7]) = res;
	POST_IO
}
RET(8)

// SUBaD
_OP(0x9067)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[7] - 2;
	CPU->A[7] = adr;
	PRE_IO
	READ_WORD_F(adr, src)
	dst = (u16)CPU->D[(Opcode >> 9) & 7];
	res = dst - src;
	CPU->flag_V = ((src ^ dst) & (res ^ dst)) >> 8;
	CPU->flag_N = CPU->flag_X = CPU->flag_C = res >> 8;
	CPU->flag_notZ = res & 0xFFFF;
	*(u16*)(&CPU->D[(Opcode >> 9) & 7]) = res;
	POST_IO
}
RET(10)

// SUBaD
_OP(0x9080)
{
	u32 adr, res;
	u32 src, dst;

	src = (u32)CPU->D[(Opcode >> 0) & 7];
	dst = (u32)CPU->D[(Opcode >> 9) & 7];
	res = dst - src;
	CPU->flag_notZ = res;
	CPU->flag_X = CPU->flag_C = ((src & res & 1) + (src >> 1) + (res >> 1)) >> 23;
	CPU->flag_V = ((src ^ dst) & (res ^ dst)) >> 24;
	CPU->flag_N = res >> 24;
	*(u32*)(&CPU->D[(Opcode >> 9) & 7]) = res;
}
RET(8)

// SUBaD
_OP(0x9088)
{
	u32 adr, res;
	u32 src, dst;

	src = (u32)CPU->A[(Opcode >> 0) & 7];
	dst = (u32)CPU->D[(Opcode >> 9) & 7];
	res = dst - src;
	CPU->flag_notZ = res;
	CPU->flag_X = CPU->flag_C = ((src & res & 1) + (src >> 1) + (res >> 1)) >> 23;
	CPU->flag_V = ((src ^ dst) & (res ^ dst)) >> 24;
	CPU->flag_N = res >> 24;
	*(u32*)(&CPU->D[(Opcode >> 9) & 7]) = res;
}
RET(8)

// SUBaD
_OP(0x9090)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7];
	PRE_IO
	READ_LONG_F(adr, src)
	dst = (u32)CPU->D[(Opcode >> 9) & 7];
	res = dst - src;
	CPU->flag_notZ = res;
	CPU->flag_X = CPU->flag_C = ((src & res & 1) + (src >> 1) + (res >> 1)) >> 23;
	CPU->flag_V = ((src ^ dst) & (res ^ dst)) >> 24;
	CPU->flag_N = res >> 24;
	*(u32*)(&CPU->D[(Opcode >> 9) & 7]) = res;
	POST_IO
}
RET(14)

// SUBaD
_OP(0x9098)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7];
	CPU->A[(Opcode >> 0) & 7] += 4;
	PRE_IO
	READ_LONG_F(adr, src)
	dst = (u32)CPU->D[(Opcode >> 9) & 7];
	res = dst - src;
	CPU->flag_notZ = res;
	CPU->flag_X = CPU->flag_C = ((src & res & 1) + (src >> 1) + (res >> 1)) >> 23;
	CPU->flag_V = ((src ^ dst) & (res ^ dst)) >> 24;
	CPU->flag_N = res >> 24;
	*(u32*)(&CPU->D[(Opcode >> 9) & 7]) = res;
	POST_IO
}
RET(14)

// SUBaD
_OP(0x90A0)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7] - 4;
	CPU->A[(Opcode >> 0) & 7] = adr;
	PRE_IO
	READ_LONG_F(adr, src)
	dst = (u32)CPU->D[(Opcode >> 9) & 7];
	res = dst - src;
	CPU->flag_notZ = res;
	CPU->flag_X = CPU->flag_C = ((src & res & 1) + (src >> 1) + (res >> 1)) >> 23;
	CPU->flag_V = ((src ^ dst) & (res ^ dst)) >> 24;
	CPU->flag_N = res >> 24;
	*(u32*)(&CPU->D[(Opcode >> 9) & 7]) = res;
	POST_IO
}
RET(16)

// SUBaD
_OP(0x90A8)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7] + (s32)(s16)FETCH_WORD;
	PC += 2;
	PRE_IO
	READ_LONG_F(adr, src)
	dst = (u32)CPU->D[(Opcode >> 9) & 7];
	res = dst - src;
	CPU->flag_notZ = res;
	CPU->flag_X = CPU->flag_C = ((src & res & 1) + (src >> 1) + (res >> 1)) >> 23;
	CPU->flag_V = ((src ^ dst) & (res ^ dst)) >> 24;
	CPU->flag_N = res >> 24;
	*(u32*)(&CPU->D[(Opcode >> 9) & 7]) = res;
	POST_IO
}
RET(18)

// SUBaD
_OP(0x90B0)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7];
	DECODE_EXT_WORD
	PRE_IO
	READ_LONG_F(adr, src)
	dst = (u32)CPU->D[(Opcode >> 9) & 7];
	res = dst - src;
	CPU->flag_notZ = res;
	CPU->flag_X = CPU->flag_C = ((src & res & 1) + (src >> 1) + (res >> 1)) >> 23;
	CPU->flag_V = ((src ^ dst) & (res ^ dst)) >> 24;
	CPU->flag_N = res >> 24;
	*(u32*)(&CPU->D[(Opcode >> 9) & 7]) = res;
	POST_IO
}
RET(20)

// SUBaD
_OP(0x90B8)
{
	u32 adr, res;
	u32 src, dst;

	adr = (s32)(s16)FETCH_WORD;
	PC += 2;
	PRE_IO
	READ_LONG_F(adr, src)
	dst = (u32)CPU->D[(Opcode >> 9) & 7];
	res = dst - src;
	CPU->flag_notZ = res;
	CPU->flag_X = CPU->flag_C = ((src & res & 1) + (src >> 1) + (res >> 1)) >> 23;
	CPU->flag_V = ((src ^ dst) & (res ^ dst)) >> 24;
	CPU->flag_N = res >> 24;
	*(u32*)(&CPU->D[(Opcode >> 9) & 7]) = res;
	POST_IO
}
RET(18)

// SUBaD
_OP(0x90B9)
{
	u32 adr, res;
	u32 src, dst;

	adr = (s32)FETCH_LONG;
	PC += 4;
	PRE_IO
	READ_LONG_F(adr, src)
	dst = (u32)CPU->D[(Opcode >> 9) & 7];
	res = dst - src;
	CPU->flag_notZ = res;
	CPU->flag_X = CPU->flag_C = ((src & res & 1) + (src >> 1) + (res >> 1)) >> 23;
	CPU->flag_V = ((src ^ dst) & (res ^ dst)) >> 24;
	CPU->flag_N = res >> 24;
	*(u32*)(&CPU->D[(Opcode >> 9) & 7]) = res;
	POST_IO
}
RET(22)

// SUBaD
_OP(0x90BA)
{
	u32 adr, res;
	u32 src, dst;

	adr = (PC - CPU->BasePC) + (s32)(s16)FETCH_WORD;
	PC += 2;
	PRE_IO
	READ_LONG_F(adr, src)
	dst = (u32)CPU->D[(Opcode >> 9) & 7];
	res = dst - src;
	CPU->flag_notZ = res;
	CPU->flag_X = CPU->flag_C = ((src & res & 1) + (src >> 1) + (res >> 1)) >> 23;
	CPU->flag_V = ((src ^ dst) & (res ^ dst)) >> 24;
	CPU->flag_N = res >> 24;
	*(u32*)(&CPU->D[(Opcode >> 9) & 7]) = res;
	POST_IO
}
RET(18)

// SUBaD
_OP(0x90BB)
{
	u32 adr, res;
	u32 src, dst;

	adr = PC - CPU->BasePC;
	DECODE_EXT_WORD
	PRE_IO
	READ_LONG_F(adr, src)
	dst = (u32)CPU->D[(Opcode >> 9) & 7];
	res = dst - src;
	CPU->flag_notZ = res;
	CPU->flag_X = CPU->flag_C = ((src & res & 1) + (src >> 1) + (res >> 1)) >> 23;
	CPU->flag_V = ((src ^ dst) & (res ^ dst)) >> 24;
	CPU->flag_N = res >> 24;
	*(u32*)(&CPU->D[(Opcode >> 9) & 7]) = res;
	POST_IO
}
RET(20)

// SUBaD
_OP(0x90BC)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_LONG;
	PC += 4;
	dst = (u32)CPU->D[(Opcode >> 9) & 7];
	res = dst - src;
	CPU->flag_notZ = res;
	CPU->flag_X = CPU->flag_C = ((src & res & 1) + (src >> 1) + (res >> 1)) >> 23;
	CPU->flag_V = ((src ^ dst) & (res ^ dst)) >> 24;
	CPU->flag_N = res >> 24;
	*(u32*)(&CPU->D[(Opcode >> 9) & 7]) = res;
}
RET(16)

// SUBaD
_OP(0x909F)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[7];
	CPU->A[7] += 4;
	PRE_IO
	READ_LONG_F(adr, src)
	dst = (u32)CPU->D[(Opcode >> 9) & 7];
	res = dst - src;
	CPU->flag_notZ = res;
	CPU->flag_X = CPU->flag_C = ((src & res & 1) + (src >> 1) + (res >> 1)) >> 23;
	CPU->flag_V = ((src ^ dst) & (res ^ dst)) >> 24;
	CPU->flag_N = res >> 24;
	*(u32*)(&CPU->D[(Opcode >> 9) & 7]) = res;
	POST_IO
}
RET(14)

// SUBaD
_OP(0x90A7)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[7] - 4;
	CPU->A[7] = adr;
	PRE_IO
	READ_LONG_F(adr, src)
	dst = (u32)CPU->D[(Opcode >> 9) & 7];
	res = dst - src;
	CPU->flag_notZ = res;
	CPU->flag_X = CPU->flag_C = ((src & res & 1) + (src >> 1) + (res >> 1)) >> 23;
	CPU->flag_V = ((src ^ dst) & (res ^ dst)) >> 24;
	CPU->flag_N = res >> 24;
	*(u32*)(&CPU->D[(Opcode >> 9) & 7]) = res;
	POST_IO
}
RET(16)

// SUBDa
_OP(0x9110)
{
	u32 adr, res;
	u32 src, dst;

	src = (u8)CPU->D[(Opcode >> 9) & 7];
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
RET(12)

// SUBDa
_OP(0x9118)
{
	u32 adr, res;
	u32 src, dst;

	src = (u8)CPU->D[(Opcode >> 9) & 7];
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
RET(12)

// SUBDa
_OP(0x9120)
{
	u32 adr, res;
	u32 src, dst;

	src = (u8)CPU->D[(Opcode >> 9) & 7];
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
RET(14)

// SUBDa
_OP(0x9128)
{
	u32 adr, res;
	u32 src, dst;

	src = (u8)CPU->D[(Opcode >> 9) & 7];
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
RET(16)

// SUBDa
_OP(0x9130)
{
	u32 adr, res;
	u32 src, dst;

	src = (u8)CPU->D[(Opcode >> 9) & 7];
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
RET(18)

// SUBDa
_OP(0x9138)
{
	u32 adr, res;
	u32 src, dst;

	src = (u8)CPU->D[(Opcode >> 9) & 7];
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
RET(16)

// SUBDa
_OP(0x9139)
{
	u32 adr, res;
	u32 src, dst;

	src = (u8)CPU->D[(Opcode >> 9) & 7];
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
RET(20)

// SUBDa
_OP(0x911F)
{
	u32 adr, res;
	u32 src, dst;

	src = (u8)CPU->D[(Opcode >> 9) & 7];
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
RET(12)

// SUBDa
_OP(0x9127)
{
	u32 adr, res;
	u32 src, dst;

	src = (u8)CPU->D[(Opcode >> 9) & 7];
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
RET(14)

// SUBDa
_OP(0x9150)
{
	u32 adr, res;
	u32 src, dst;

	src = (u16)CPU->D[(Opcode >> 9) & 7];
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
RET(12)

// SUBDa
_OP(0x9158)
{
	u32 adr, res;
	u32 src, dst;

	src = (u16)CPU->D[(Opcode >> 9) & 7];
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
RET(12)

// SUBDa
_OP(0x9160)
{
	u32 adr, res;
	u32 src, dst;

	src = (u16)CPU->D[(Opcode >> 9) & 7];
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
RET(14)

// SUBDa
_OP(0x9168)
{
	u32 adr, res;
	u32 src, dst;

	src = (u16)CPU->D[(Opcode >> 9) & 7];
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
RET(16)

// SUBDa
_OP(0x9170)
{
	u32 adr, res;
	u32 src, dst;

	src = (u16)CPU->D[(Opcode >> 9) & 7];
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
RET(18)

// SUBDa
_OP(0x9178)
{
	u32 adr, res;
	u32 src, dst;

	src = (u16)CPU->D[(Opcode >> 9) & 7];
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
RET(16)

// SUBDa
_OP(0x9179)
{
	u32 adr, res;
	u32 src, dst;

	src = (u16)CPU->D[(Opcode >> 9) & 7];
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
RET(20)

// SUBDa
_OP(0x915F)
{
	u32 adr, res;
	u32 src, dst;

	src = (u16)CPU->D[(Opcode >> 9) & 7];
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
RET(12)

// SUBDa
_OP(0x9167)
{
	u32 adr, res;
	u32 src, dst;

	src = (u16)CPU->D[(Opcode >> 9) & 7];
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
RET(14)

// SUBDa
_OP(0x9190)
{
	u32 adr, res;
	u32 src, dst;

	src = (u32)CPU->D[(Opcode >> 9) & 7];
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
RET(20)

// SUBDa
_OP(0x9198)
{
	u32 adr, res;
	u32 src, dst;

	src = (u32)CPU->D[(Opcode >> 9) & 7];
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
RET(20)

// SUBDa
_OP(0x91A0)
{
	u32 adr, res;
	u32 src, dst;

	src = (u32)CPU->D[(Opcode >> 9) & 7];
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
RET(22)

// SUBDa
_OP(0x91A8)
{
	u32 adr, res;
	u32 src, dst;

	src = (u32)CPU->D[(Opcode >> 9) & 7];
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
RET(24)

// SUBDa
_OP(0x91B0)
{
	u32 adr, res;
	u32 src, dst;

	src = (u32)CPU->D[(Opcode >> 9) & 7];
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
RET(26)

// SUBDa
_OP(0x91B8)
{
	u32 adr, res;
	u32 src, dst;

	src = (u32)CPU->D[(Opcode >> 9) & 7];
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
RET(24)

// SUBDa
_OP(0x91B9)
{
	u32 adr, res;
	u32 src, dst;

	src = (u32)CPU->D[(Opcode >> 9) & 7];
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
RET(28)

// SUBDa
_OP(0x919F)
{
	u32 adr, res;
	u32 src, dst;

	src = (u32)CPU->D[(Opcode >> 9) & 7];
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
RET(20)

// SUBDa
_OP(0x91A7)
{
	u32 adr, res;
	u32 src, dst;

	src = (u32)CPU->D[(Opcode >> 9) & 7];
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
RET(22)

// SUBX
_OP(0x9100)
{
	u32 adr, res;
	u32 src, dst;

	src = (u8)CPU->D[(Opcode >> 0) & 7];
	dst = (u8)CPU->D[(Opcode >> 9) & 7];
	res = dst - src - ((CPU->flag_X >> 8) & 1);
	CPU->flag_N = CPU->flag_X = CPU->flag_C = res;
	CPU->flag_V = (src ^ dst) & (res ^ dst);
	CPU->flag_notZ |= res & 0xFF;
	*(u8*)(&CPU->D[(Opcode >> 9) & 7]) = res;
}
RET(4)

// SUBX
_OP(0x9140)
{
	u32 adr, res;
	u32 src, dst;

	src = (u16)CPU->D[(Opcode >> 0) & 7];
	dst = (u16)CPU->D[(Opcode >> 9) & 7];
	res = dst - src - ((CPU->flag_X >> 8) & 1);
	CPU->flag_V = ((src ^ dst) & (res ^ dst)) >> 8;
	CPU->flag_N = CPU->flag_X = CPU->flag_C = res >> 8;
	CPU->flag_notZ |= res & 0xFFFF;
	*(u16*)(&CPU->D[(Opcode >> 9) & 7]) = res;
}
RET(4)

// SUBX
_OP(0x9180)
{
	u32 adr, res;
	u32 src, dst;

	src = (u32)CPU->D[(Opcode >> 0) & 7];
	dst = (u32)CPU->D[(Opcode >> 9) & 7];
	res = dst - src - ((CPU->flag_X >> 8) & 1);
	CPU->flag_notZ |= res;
	CPU->flag_X = CPU->flag_C = ((src & res & 1) + (src >> 1) + (res >> 1)) >> 23;
	CPU->flag_V = ((src ^ dst) & (res ^ dst)) >> 24;
	CPU->flag_N = res >> 24;
	*(u32*)(&CPU->D[(Opcode >> 9) & 7]) = res;
}
RET(8)

// SUBXM
_OP(0x9108)
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
	res = dst - src - ((CPU->flag_X >> 8) & 1);
	CPU->flag_N = CPU->flag_X = CPU->flag_C = res;
	CPU->flag_V = (src ^ dst) & (res ^ dst);
	CPU->flag_notZ |= res & 0xFF;
	WRITE_BYTE_F(adr, res)
	POST_IO
}
RET(18)

// SUBXM
_OP(0x9148)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7] - 2;
	CPU->A[(Opcode >> 0) & 7] = adr;
	PRE_IO
	READ_WORD_F(adr, src)
	adr = CPU->A[(Opcode >> 9) & 7] - 2;
	CPU->A[(Opcode >> 9) & 7] = adr;
	READ_WORD_F(adr, dst)
	res = dst - src - ((CPU->flag_X >> 8) & 1);
	CPU->flag_V = ((src ^ dst) & (res ^ dst)) >> 8;
	CPU->flag_N = CPU->flag_X = CPU->flag_C = res >> 8;
	CPU->flag_notZ |= res & 0xFFFF;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(18)

// SUBXM
_OP(0x9188)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7] - 4;
	CPU->A[(Opcode >> 0) & 7] = adr;
	PRE_IO
	READ_LONG_F(adr, src)
	adr = CPU->A[(Opcode >> 9) & 7] - 4;
	CPU->A[(Opcode >> 9) & 7] = adr;
	READ_LONG_F(adr, dst)
	res = dst - src - ((CPU->flag_X >> 8) & 1);
	CPU->flag_notZ |= res;
	CPU->flag_X = CPU->flag_C = ((src & res & 1) + (src >> 1) + (res >> 1)) >> 23;
	CPU->flag_V = ((src ^ dst) & (res ^ dst)) >> 24;
	CPU->flag_N = res >> 24;
	WRITE_LONG_F(adr, res)
	POST_IO
}
RET(30)

// SUBX7M
_OP(0x910F)
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
	res = dst - src - ((CPU->flag_X >> 8) & 1);
	CPU->flag_N = CPU->flag_X = CPU->flag_C = res;
	CPU->flag_V = (src ^ dst) & (res ^ dst);
	CPU->flag_notZ |= res & 0xFF;
	WRITE_BYTE_F(adr, res)
	POST_IO
}
RET(18)

// SUBX7M
_OP(0x914F)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[7] - 2;
	CPU->A[7] = adr;
	PRE_IO
	READ_WORD_F(adr, src)
	adr = CPU->A[(Opcode >> 9) & 7] - 2;
	CPU->A[(Opcode >> 9) & 7] = adr;
	READ_WORD_F(adr, dst)
	res = dst - src - ((CPU->flag_X >> 8) & 1);
	CPU->flag_V = ((src ^ dst) & (res ^ dst)) >> 8;
	CPU->flag_N = CPU->flag_X = CPU->flag_C = res >> 8;
	CPU->flag_notZ |= res & 0xFFFF;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(18)

// SUBX7M
_OP(0x918F)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[7] - 4;
	CPU->A[7] = adr;
	PRE_IO
	READ_LONG_F(adr, src)
	adr = CPU->A[(Opcode >> 9) & 7] - 4;
	CPU->A[(Opcode >> 9) & 7] = adr;
	READ_LONG_F(adr, dst)
	res = dst - src - ((CPU->flag_X >> 8) & 1);
	CPU->flag_notZ |= res;
	CPU->flag_X = CPU->flag_C = ((src & res & 1) + (src >> 1) + (res >> 1)) >> 23;
	CPU->flag_V = ((src ^ dst) & (res ^ dst)) >> 24;
	CPU->flag_N = res >> 24;
	WRITE_LONG_F(adr, res)
	POST_IO
}
RET(30)

// SUBXM7
_OP(0x9F08)
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
	res = dst - src - ((CPU->flag_X >> 8) & 1);
	CPU->flag_N = CPU->flag_X = CPU->flag_C = res;
	CPU->flag_V = (src ^ dst) & (res ^ dst);
	CPU->flag_notZ |= res & 0xFF;
	WRITE_BYTE_F(adr, res)
	POST_IO
}
RET(18)

// SUBXM7
_OP(0x9F48)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7] - 2;
	CPU->A[(Opcode >> 0) & 7] = adr;
	PRE_IO
	READ_WORD_F(adr, src)
	adr = CPU->A[7] - 2;
	CPU->A[7] = adr;
	READ_WORD_F(adr, dst)
	res = dst - src - ((CPU->flag_X >> 8) & 1);
	CPU->flag_V = ((src ^ dst) & (res ^ dst)) >> 8;
	CPU->flag_N = CPU->flag_X = CPU->flag_C = res >> 8;
	CPU->flag_notZ |= res & 0xFFFF;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(18)

// SUBXM7
_OP(0x9F88)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7] - 4;
	CPU->A[(Opcode >> 0) & 7] = adr;
	PRE_IO
	READ_LONG_F(adr, src)
	adr = CPU->A[7] - 4;
	CPU->A[7] = adr;
	READ_LONG_F(adr, dst)
	res = dst - src - ((CPU->flag_X >> 8) & 1);
	CPU->flag_notZ |= res;
	CPU->flag_X = CPU->flag_C = ((src & res & 1) + (src >> 1) + (res >> 1)) >> 23;
	CPU->flag_V = ((src ^ dst) & (res ^ dst)) >> 24;
	CPU->flag_N = res >> 24;
	WRITE_LONG_F(adr, res)
	POST_IO
}
RET(30)

// SUBX7M7
_OP(0x9F0F)
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
	res = dst - src - ((CPU->flag_X >> 8) & 1);
	CPU->flag_N = CPU->flag_X = CPU->flag_C = res;
	CPU->flag_V = (src ^ dst) & (res ^ dst);
	CPU->flag_notZ |= res & 0xFF;
	WRITE_BYTE_F(adr, res)
	POST_IO
}
RET(18)

// SUBX7M7
_OP(0x9F4F)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[7] - 2;
	CPU->A[7] = adr;
	PRE_IO
	READ_WORD_F(adr, src)
	adr = CPU->A[7] - 2;
	CPU->A[7] = adr;
	READ_WORD_F(adr, dst)
	res = dst - src - ((CPU->flag_X >> 8) & 1);
	CPU->flag_V = ((src ^ dst) & (res ^ dst)) >> 8;
	CPU->flag_N = CPU->flag_X = CPU->flag_C = res >> 8;
	CPU->flag_notZ |= res & 0xFFFF;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(18)

// SUBX7M7
_OP(0x9F8F)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[7] - 4;
	CPU->A[7] = adr;
	PRE_IO
	READ_LONG_F(adr, src)
	adr = CPU->A[7] - 4;
	CPU->A[7] = adr;
	READ_LONG_F(adr, dst)
	res = dst - src - ((CPU->flag_X >> 8) & 1);
	CPU->flag_notZ |= res;
	CPU->flag_X = CPU->flag_C = ((src & res & 1) + (src >> 1) + (res >> 1)) >> 23;
	CPU->flag_V = ((src ^ dst) & (res ^ dst)) >> 24;
	CPU->flag_N = res >> 24;
	WRITE_LONG_F(adr, res)
	POST_IO
}
RET(30)

// SUBA
_OP(0x90C0)
{
	u32 adr, res;
	u32 src, dst;

	src = (s32)(s16)CPU->D[(Opcode >> 0) & 7];
	dst = (u32)CPU->A[(Opcode >> 9) & 7];
	res = dst - src;
	CPU->A[(Opcode >> 9) & 7] = res;
}
RET(8)

// SUBA
_OP(0x90C8)
{
	u32 adr, res;
	u32 src, dst;

	src = (s32)(s16)CPU->A[(Opcode >> 0) & 7];
	dst = (u32)CPU->A[(Opcode >> 9) & 7];
	res = dst - src;
	CPU->A[(Opcode >> 9) & 7] = res;
}
RET(8)

// SUBA
_OP(0x90D0)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7];
	PRE_IO
	READSX_WORD_F(adr, src)
	dst = (u32)CPU->A[(Opcode >> 9) & 7];
	res = dst - src;
	CPU->A[(Opcode >> 9) & 7] = res;
	POST_IO
}
RET(10)

// SUBA
_OP(0x90D8)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7];
	CPU->A[(Opcode >> 0) & 7] += 2;
	PRE_IO
	READSX_WORD_F(adr, src)
	dst = (u32)CPU->A[(Opcode >> 9) & 7];
	res = dst - src;
	CPU->A[(Opcode >> 9) & 7] = res;
	POST_IO
}
RET(10)

// SUBA
_OP(0x90E0)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7] - 2;
	CPU->A[(Opcode >> 0) & 7] = adr;
	PRE_IO
	READSX_WORD_F(adr, src)
	dst = (u32)CPU->A[(Opcode >> 9) & 7];
	res = dst - src;
	CPU->A[(Opcode >> 9) & 7] = res;
	POST_IO
}
RET(12)

// SUBA
_OP(0x90E8)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7] + (s32)(s16)FETCH_WORD;
	PC += 2;
	PRE_IO
	READSX_WORD_F(adr, src)
	dst = (u32)CPU->A[(Opcode >> 9) & 7];
	res = dst - src;
	CPU->A[(Opcode >> 9) & 7] = res;
	POST_IO
}
RET(14)

// SUBA
_OP(0x90F0)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7];
	DECODE_EXT_WORD
	PRE_IO
	READSX_WORD_F(adr, src)
	dst = (u32)CPU->A[(Opcode >> 9) & 7];
	res = dst - src;
	CPU->A[(Opcode >> 9) & 7] = res;
	POST_IO
}
RET(16)

// SUBA
_OP(0x90F8)
{
	u32 adr, res;
	u32 src, dst;

	adr = (s32)(s16)FETCH_WORD;
	PC += 2;
	PRE_IO
	READSX_WORD_F(adr, src)
	dst = (u32)CPU->A[(Opcode >> 9) & 7];
	res = dst - src;
	CPU->A[(Opcode >> 9) & 7] = res;
	POST_IO
}
RET(14)

// SUBA
_OP(0x90F9)
{
	u32 adr, res;
	u32 src, dst;

	adr = (s32)FETCH_LONG;
	PC += 4;
	PRE_IO
	READSX_WORD_F(adr, src)
	dst = (u32)CPU->A[(Opcode >> 9) & 7];
	res = dst - src;
	CPU->A[(Opcode >> 9) & 7] = res;
	POST_IO
}
RET(18)

// SUBA
_OP(0x90FA)
{
	u32 adr, res;
	u32 src, dst;

	adr = (PC - CPU->BasePC) + (s32)(s16)FETCH_WORD;
	PC += 2;
	PRE_IO
	READSX_WORD_F(adr, src)
	dst = (u32)CPU->A[(Opcode >> 9) & 7];
	res = dst - src;
	CPU->A[(Opcode >> 9) & 7] = res;
	POST_IO
}
RET(14)

// SUBA
_OP(0x90FB)
{
	u32 adr, res;
	u32 src, dst;

	adr = PC - CPU->BasePC;
	DECODE_EXT_WORD
	PRE_IO
	READSX_WORD_F(adr, src)
	dst = (u32)CPU->A[(Opcode >> 9) & 7];
	res = dst - src;
	CPU->A[(Opcode >> 9) & 7] = res;
	POST_IO
}
RET(16)

// SUBA
_OP(0x90FC)
{
	u32 adr, res;
	u32 src, dst;

	src = (s32)(s16)FETCH_WORD;
	PC += 2;
	dst = (u32)CPU->A[(Opcode >> 9) & 7];
	res = dst - src;
	CPU->A[(Opcode >> 9) & 7] = res;
}
RET(12)

// SUBA
_OP(0x90DF)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[7];
	CPU->A[7] += 2;
	PRE_IO
	READSX_WORD_F(adr, src)
	dst = (u32)CPU->A[(Opcode >> 9) & 7];
	res = dst - src;
	CPU->A[(Opcode >> 9) & 7] = res;
	POST_IO
}
RET(10)

// SUBA
_OP(0x90E7)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[7] - 2;
	CPU->A[7] = adr;
	PRE_IO
	READSX_WORD_F(adr, src)
	dst = (u32)CPU->A[(Opcode >> 9) & 7];
	res = dst - src;
	CPU->A[(Opcode >> 9) & 7] = res;
	POST_IO
}
RET(12)

// SUBA
_OP(0x91C0)
{
	u32 adr, res;
	u32 src, dst;

	src = (s32)(s32)CPU->D[(Opcode >> 0) & 7];
	dst = (u32)CPU->A[(Opcode >> 9) & 7];
	res = dst - src;
	CPU->A[(Opcode >> 9) & 7] = res;
}
RET(6)

// SUBA
_OP(0x91C8)
{
	u32 adr, res;
	u32 src, dst;

	src = (s32)(s32)CPU->A[(Opcode >> 0) & 7];
	dst = (u32)CPU->A[(Opcode >> 9) & 7];
	res = dst - src;
	CPU->A[(Opcode >> 9) & 7] = res;
}
RET(6)

// SUBA
_OP(0x91D0)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7];
	PRE_IO
	READSX_LONG_F(adr, src)
	dst = (u32)CPU->A[(Opcode >> 9) & 7];
	res = dst - src;
	CPU->A[(Opcode >> 9) & 7] = res;
	POST_IO
}
RET(14)

// SUBA
_OP(0x91D8)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7];
	CPU->A[(Opcode >> 0) & 7] += 4;
	PRE_IO
	READSX_LONG_F(adr, src)
	dst = (u32)CPU->A[(Opcode >> 9) & 7];
	res = dst - src;
	CPU->A[(Opcode >> 9) & 7] = res;
	POST_IO
}
RET(14)

// SUBA
_OP(0x91E0)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7] - 4;
	CPU->A[(Opcode >> 0) & 7] = adr;
	PRE_IO
	READSX_LONG_F(adr, src)
	dst = (u32)CPU->A[(Opcode >> 9) & 7];
	res = dst - src;
	CPU->A[(Opcode >> 9) & 7] = res;
	POST_IO
}
RET(16)

// SUBA
_OP(0x91E8)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7] + (s32)(s16)FETCH_WORD;
	PC += 2;
	PRE_IO
	READSX_LONG_F(adr, src)
	dst = (u32)CPU->A[(Opcode >> 9) & 7];
	res = dst - src;
	CPU->A[(Opcode >> 9) & 7] = res;
	POST_IO
}
RET(18)

// SUBA
_OP(0x91F0)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7];
	DECODE_EXT_WORD
	PRE_IO
	READSX_LONG_F(adr, src)
	dst = (u32)CPU->A[(Opcode >> 9) & 7];
	res = dst - src;
	CPU->A[(Opcode >> 9) & 7] = res;
	POST_IO
}
RET(20)

// SUBA
_OP(0x91F8)
{
	u32 adr, res;
	u32 src, dst;

	adr = (s32)(s16)FETCH_WORD;
	PC += 2;
	PRE_IO
	READSX_LONG_F(adr, src)
	dst = (u32)CPU->A[(Opcode >> 9) & 7];
	res = dst - src;
	CPU->A[(Opcode >> 9) & 7] = res;
	POST_IO
}
RET(18)

// SUBA
_OP(0x91F9)
{
	u32 adr, res;
	u32 src, dst;

	adr = (s32)FETCH_LONG;
	PC += 4;
	PRE_IO
	READSX_LONG_F(adr, src)
	dst = (u32)CPU->A[(Opcode >> 9) & 7];
	res = dst - src;
	CPU->A[(Opcode >> 9) & 7] = res;
	POST_IO
}
RET(22)

// SUBA
_OP(0x91FA)
{
	u32 adr, res;
	u32 src, dst;

	adr = (PC - CPU->BasePC) + (s32)(s16)FETCH_WORD;
	PC += 2;
	PRE_IO
	READSX_LONG_F(adr, src)
	dst = (u32)CPU->A[(Opcode >> 9) & 7];
	res = dst - src;
	CPU->A[(Opcode >> 9) & 7] = res;
	POST_IO
}
RET(18)

// SUBA
_OP(0x91FB)
{
	u32 adr, res;
	u32 src, dst;

	adr = PC - CPU->BasePC;
	DECODE_EXT_WORD
	PRE_IO
	READSX_LONG_F(adr, src)
	dst = (u32)CPU->A[(Opcode >> 9) & 7];
	res = dst - src;
	CPU->A[(Opcode >> 9) & 7] = res;
	POST_IO
}
RET(20)

// SUBA
_OP(0x91FC)
{
	u32 adr, res;
	u32 src, dst;

	src = (s32)(s32)FETCH_LONG;
	PC += 4;
	dst = (u32)CPU->A[(Opcode >> 9) & 7];
	res = dst - src;
	CPU->A[(Opcode >> 9) & 7] = res;
}
RET(14)

// SUBA
_OP(0x91DF)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[7];
	CPU->A[7] += 4;
	PRE_IO
	READSX_LONG_F(adr, src)
	dst = (u32)CPU->A[(Opcode >> 9) & 7];
	res = dst - src;
	CPU->A[(Opcode >> 9) & 7] = res;
	POST_IO
}
RET(14)

// SUBA
_OP(0x91E7)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[7] - 4;
	CPU->A[7] = adr;
	PRE_IO
	READSX_LONG_F(adr, src)
	dst = (u32)CPU->A[(Opcode >> 9) & 7];
	res = dst - src;
	CPU->A[(Opcode >> 9) & 7] = res;
	POST_IO
}
RET(16)
