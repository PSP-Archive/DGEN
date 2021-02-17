
// CMP
_OP(0xB000)
{
	u32 adr, res;
	u32 src, dst;

	src = (u8)CPU->D[(Opcode >> 0) & 7];
	dst = (u8)CPU->D[(Opcode >> 9) & 7];
	res = dst - src;
	CPU->flag_N = CPU->flag_C = res;
	CPU->flag_V = (src ^ dst) & (res ^ dst);
	CPU->flag_notZ = res & 0xFF;
}
RET(4)

// CMP
_OP(0xB008)
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
	CPU->flag_N = CPU->flag_C = res;
	CPU->flag_V = (src ^ dst) & (res ^ dst);
	CPU->flag_notZ = res & 0xFF;
}
RET(4)

// CMP
_OP(0xB010)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7];
	PRE_IO
	READ_BYTE_F(adr, src)
	dst = (u8)CPU->D[(Opcode >> 9) & 7];
	res = dst - src;
	CPU->flag_N = CPU->flag_C = res;
	CPU->flag_V = (src ^ dst) & (res ^ dst);
	CPU->flag_notZ = res & 0xFF;
	POST_IO
}
RET(8)

// CMP
_OP(0xB018)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7];
	CPU->A[(Opcode >> 0) & 7] += 1;
	PRE_IO
	READ_BYTE_F(adr, src)
	dst = (u8)CPU->D[(Opcode >> 9) & 7];
	res = dst - src;
	CPU->flag_N = CPU->flag_C = res;
	CPU->flag_V = (src ^ dst) & (res ^ dst);
	CPU->flag_notZ = res & 0xFF;
	POST_IO
}
RET(8)

// CMP
_OP(0xB020)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7] - 1;
	CPU->A[(Opcode >> 0) & 7] = adr;
	PRE_IO
	READ_BYTE_F(adr, src)
	dst = (u8)CPU->D[(Opcode >> 9) & 7];
	res = dst - src;
	CPU->flag_N = CPU->flag_C = res;
	CPU->flag_V = (src ^ dst) & (res ^ dst);
	CPU->flag_notZ = res & 0xFF;
	POST_IO
}
RET(10)

// CMP
_OP(0xB028)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7] + (s32)(s16)FETCH_WORD;
	PC += 2;
	PRE_IO
	READ_BYTE_F(adr, src)
	dst = (u8)CPU->D[(Opcode >> 9) & 7];
	res = dst - src;
	CPU->flag_N = CPU->flag_C = res;
	CPU->flag_V = (src ^ dst) & (res ^ dst);
	CPU->flag_notZ = res & 0xFF;
	POST_IO
}
RET(12)

// CMP
_OP(0xB030)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7];
	DECODE_EXT_WORD
	PRE_IO
	READ_BYTE_F(adr, src)
	dst = (u8)CPU->D[(Opcode >> 9) & 7];
	res = dst - src;
	CPU->flag_N = CPU->flag_C = res;
	CPU->flag_V = (src ^ dst) & (res ^ dst);
	CPU->flag_notZ = res & 0xFF;
	POST_IO
}
RET(14)

// CMP
_OP(0xB038)
{
	u32 adr, res;
	u32 src, dst;

	adr = (s32)(s16)FETCH_WORD;
	PC += 2;
	PRE_IO
	READ_BYTE_F(adr, src)
	dst = (u8)CPU->D[(Opcode >> 9) & 7];
	res = dst - src;
	CPU->flag_N = CPU->flag_C = res;
	CPU->flag_V = (src ^ dst) & (res ^ dst);
	CPU->flag_notZ = res & 0xFF;
	POST_IO
}
RET(12)

// CMP
_OP(0xB039)
{
	u32 adr, res;
	u32 src, dst;

	adr = (s32)FETCH_LONG;
	PC += 4;
	PRE_IO
	READ_BYTE_F(adr, src)
	dst = (u8)CPU->D[(Opcode >> 9) & 7];
	res = dst - src;
	CPU->flag_N = CPU->flag_C = res;
	CPU->flag_V = (src ^ dst) & (res ^ dst);
	CPU->flag_notZ = res & 0xFF;
	POST_IO
}
RET(16)

// CMP
_OP(0xB03A)
{
	u32 adr, res;
	u32 src, dst;

	adr = (PC - CPU->BasePC) + (s32)(s16)FETCH_WORD;
	PC += 2;
	PRE_IO
	READ_BYTE_F(adr, src)
	dst = (u8)CPU->D[(Opcode >> 9) & 7];
	res = dst - src;
	CPU->flag_N = CPU->flag_C = res;
	CPU->flag_V = (src ^ dst) & (res ^ dst);
	CPU->flag_notZ = res & 0xFF;
	POST_IO
}
RET(12)

// CMP
_OP(0xB03B)
{
	u32 adr, res;
	u32 src, dst;

	adr = PC - CPU->BasePC;
	DECODE_EXT_WORD
	PRE_IO
	READ_BYTE_F(adr, src)
	dst = (u8)CPU->D[(Opcode >> 9) & 7];
	res = dst - src;
	CPU->flag_N = CPU->flag_C = res;
	CPU->flag_V = (src ^ dst) & (res ^ dst);
	CPU->flag_notZ = res & 0xFF;
	POST_IO
}
RET(14)

// CMP
_OP(0xB03C)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_BYTE;
	PC += 2;
	dst = (u8)CPU->D[(Opcode >> 9) & 7];
	res = dst - src;
	CPU->flag_N = CPU->flag_C = res;
	CPU->flag_V = (src ^ dst) & (res ^ dst);
	CPU->flag_notZ = res & 0xFF;
}
RET(8)

// CMP
_OP(0xB01F)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[7];
	CPU->A[7] += 2;
	PRE_IO
	READ_BYTE_F(adr, src)
	dst = (u8)CPU->D[(Opcode >> 9) & 7];
	res = dst - src;
	CPU->flag_N = CPU->flag_C = res;
	CPU->flag_V = (src ^ dst) & (res ^ dst);
	CPU->flag_notZ = res & 0xFF;
	POST_IO
}
RET(8)

// CMP
_OP(0xB027)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[7] - 2;
	CPU->A[7] = adr;
	PRE_IO
	READ_BYTE_F(adr, src)
	dst = (u8)CPU->D[(Opcode >> 9) & 7];
	res = dst - src;
	CPU->flag_N = CPU->flag_C = res;
	CPU->flag_V = (src ^ dst) & (res ^ dst);
	CPU->flag_notZ = res & 0xFF;
	POST_IO
}
RET(10)

// CMP
_OP(0xB040)
{
	u32 adr, res;
	u32 src, dst;

	src = (u16)CPU->D[(Opcode >> 0) & 7];
	dst = (u16)CPU->D[(Opcode >> 9) & 7];
	res = dst - src;
	CPU->flag_V = ((src ^ dst) & (res ^ dst)) >> 8;
	CPU->flag_N = CPU->flag_C = res >> 8;
	CPU->flag_notZ = res & 0xFFFF;
}
RET(4)

// CMP
_OP(0xB048)
{
	u32 adr, res;
	u32 src, dst;

	src = (u16)CPU->A[(Opcode >> 0) & 7];
	dst = (u16)CPU->D[(Opcode >> 9) & 7];
	res = dst - src;
	CPU->flag_V = ((src ^ dst) & (res ^ dst)) >> 8;
	CPU->flag_N = CPU->flag_C = res >> 8;
	CPU->flag_notZ = res & 0xFFFF;
}
RET(4)

// CMP
_OP(0xB050)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7];
	PRE_IO
	READ_WORD_F(adr, src)
	dst = (u16)CPU->D[(Opcode >> 9) & 7];
	res = dst - src;
	CPU->flag_V = ((src ^ dst) & (res ^ dst)) >> 8;
	CPU->flag_N = CPU->flag_C = res >> 8;
	CPU->flag_notZ = res & 0xFFFF;
	POST_IO
}
RET(8)

// CMP
_OP(0xB058)
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
	CPU->flag_N = CPU->flag_C = res >> 8;
	CPU->flag_notZ = res & 0xFFFF;
	POST_IO
}
RET(8)

// CMP
_OP(0xB060)
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
	CPU->flag_N = CPU->flag_C = res >> 8;
	CPU->flag_notZ = res & 0xFFFF;
	POST_IO
}
RET(10)

// CMP
_OP(0xB068)
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
	CPU->flag_N = CPU->flag_C = res >> 8;
	CPU->flag_notZ = res & 0xFFFF;
	POST_IO
}
RET(12)

// CMP
_OP(0xB070)
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
	CPU->flag_N = CPU->flag_C = res >> 8;
	CPU->flag_notZ = res & 0xFFFF;
	POST_IO
}
RET(14)

// CMP
_OP(0xB078)
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
	CPU->flag_N = CPU->flag_C = res >> 8;
	CPU->flag_notZ = res & 0xFFFF;
	POST_IO
}
RET(12)

// CMP
_OP(0xB079)
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
	CPU->flag_N = CPU->flag_C = res >> 8;
	CPU->flag_notZ = res & 0xFFFF;
	POST_IO
}
RET(16)

// CMP
_OP(0xB07A)
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
	CPU->flag_N = CPU->flag_C = res >> 8;
	CPU->flag_notZ = res & 0xFFFF;
	POST_IO
}
RET(12)

// CMP
_OP(0xB07B)
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
	CPU->flag_N = CPU->flag_C = res >> 8;
	CPU->flag_notZ = res & 0xFFFF;
	POST_IO
}
RET(14)

// CMP
_OP(0xB07C)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_WORD;
	PC += 2;
	dst = (u16)CPU->D[(Opcode >> 9) & 7];
	res = dst - src;
	CPU->flag_V = ((src ^ dst) & (res ^ dst)) >> 8;
	CPU->flag_N = CPU->flag_C = res >> 8;
	CPU->flag_notZ = res & 0xFFFF;
}
RET(8)

// CMP
_OP(0xB05F)
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
	CPU->flag_N = CPU->flag_C = res >> 8;
	CPU->flag_notZ = res & 0xFFFF;
	POST_IO
}
RET(8)

// CMP
_OP(0xB067)
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
	CPU->flag_N = CPU->flag_C = res >> 8;
	CPU->flag_notZ = res & 0xFFFF;
	POST_IO
}
RET(10)

// CMP
_OP(0xB080)
{
	u32 adr, res;
	u32 src, dst;

	src = (u32)CPU->D[(Opcode >> 0) & 7];
	dst = (u32)CPU->D[(Opcode >> 9) & 7];
	res = dst - src;
	CPU->flag_notZ = res;
	CPU->flag_C = ((src & res & 1) + (src >> 1) + (res >> 1)) >> 23;
	CPU->flag_V = ((src ^ dst) & (res ^ dst)) >> 24;
	CPU->flag_N = res >> 24;
}
RET(6)

// CMP
_OP(0xB088)
{
	u32 adr, res;
	u32 src, dst;

	src = (u32)CPU->A[(Opcode >> 0) & 7];
	dst = (u32)CPU->D[(Opcode >> 9) & 7];
	res = dst - src;
	CPU->flag_notZ = res;
	CPU->flag_C = ((src & res & 1) + (src >> 1) + (res >> 1)) >> 23;
	CPU->flag_V = ((src ^ dst) & (res ^ dst)) >> 24;
	CPU->flag_N = res >> 24;
}
RET(6)

// CMP
_OP(0xB090)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7];
	PRE_IO
	READ_LONG_F(adr, src)
	dst = (u32)CPU->D[(Opcode >> 9) & 7];
	res = dst - src;
	CPU->flag_notZ = res;
	CPU->flag_C = ((src & res & 1) + (src >> 1) + (res >> 1)) >> 23;
	CPU->flag_V = ((src ^ dst) & (res ^ dst)) >> 24;
	CPU->flag_N = res >> 24;
	POST_IO
}
RET(14)

// CMP
_OP(0xB098)
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
	CPU->flag_C = ((src & res & 1) + (src >> 1) + (res >> 1)) >> 23;
	CPU->flag_V = ((src ^ dst) & (res ^ dst)) >> 24;
	CPU->flag_N = res >> 24;
	POST_IO
}
RET(14)

// CMP
_OP(0xB0A0)
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
	CPU->flag_C = ((src & res & 1) + (src >> 1) + (res >> 1)) >> 23;
	CPU->flag_V = ((src ^ dst) & (res ^ dst)) >> 24;
	CPU->flag_N = res >> 24;
	POST_IO
}
RET(16)

// CMP
_OP(0xB0A8)
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
	CPU->flag_C = ((src & res & 1) + (src >> 1) + (res >> 1)) >> 23;
	CPU->flag_V = ((src ^ dst) & (res ^ dst)) >> 24;
	CPU->flag_N = res >> 24;
	POST_IO
}
RET(18)

// CMP
_OP(0xB0B0)
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
	CPU->flag_C = ((src & res & 1) + (src >> 1) + (res >> 1)) >> 23;
	CPU->flag_V = ((src ^ dst) & (res ^ dst)) >> 24;
	CPU->flag_N = res >> 24;
	POST_IO
}
RET(20)

// CMP
_OP(0xB0B8)
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
	CPU->flag_C = ((src & res & 1) + (src >> 1) + (res >> 1)) >> 23;
	CPU->flag_V = ((src ^ dst) & (res ^ dst)) >> 24;
	CPU->flag_N = res >> 24;
	POST_IO
}
RET(18)

// CMP
_OP(0xB0B9)
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
	CPU->flag_C = ((src & res & 1) + (src >> 1) + (res >> 1)) >> 23;
	CPU->flag_V = ((src ^ dst) & (res ^ dst)) >> 24;
	CPU->flag_N = res >> 24;
	POST_IO
}
RET(22)

// CMP
_OP(0xB0BA)
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
	CPU->flag_C = ((src & res & 1) + (src >> 1) + (res >> 1)) >> 23;
	CPU->flag_V = ((src ^ dst) & (res ^ dst)) >> 24;
	CPU->flag_N = res >> 24;
	POST_IO
}
RET(18)

// CMP
_OP(0xB0BB)
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
	CPU->flag_C = ((src & res & 1) + (src >> 1) + (res >> 1)) >> 23;
	CPU->flag_V = ((src ^ dst) & (res ^ dst)) >> 24;
	CPU->flag_N = res >> 24;
	POST_IO
}
RET(20)

// CMP
_OP(0xB0BC)
{
	u32 adr, res;
	u32 src, dst;

	src = FETCH_LONG;
	PC += 4;
	dst = (u32)CPU->D[(Opcode >> 9) & 7];
	res = dst - src;
	CPU->flag_notZ = res;
	CPU->flag_C = ((src & res & 1) + (src >> 1) + (res >> 1)) >> 23;
	CPU->flag_V = ((src ^ dst) & (res ^ dst)) >> 24;
	CPU->flag_N = res >> 24;
}
RET(14)

// CMP
_OP(0xB09F)
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
	CPU->flag_C = ((src & res & 1) + (src >> 1) + (res >> 1)) >> 23;
	CPU->flag_V = ((src ^ dst) & (res ^ dst)) >> 24;
	CPU->flag_N = res >> 24;
	POST_IO
}
RET(14)

// CMP
_OP(0xB0A7)
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
	CPU->flag_C = ((src & res & 1) + (src >> 1) + (res >> 1)) >> 23;
	CPU->flag_V = ((src ^ dst) & (res ^ dst)) >> 24;
	CPU->flag_N = res >> 24;
	POST_IO
}
RET(16)

// CMPM
_OP(0xB108)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7];
	CPU->A[(Opcode >> 0) & 7] += 1;
	PRE_IO
	READ_BYTE_F(adr, src)
	adr = CPU->A[(Opcode >> 9) & 7];
	CPU->A[(Opcode >> 9) & 7] += 1;
	READ_BYTE_F(adr, dst)
	res = dst - src;
	CPU->flag_N = CPU->flag_C = res;
	CPU->flag_V = (src ^ dst) & (res ^ dst);
	CPU->flag_notZ = res & 0xFF;
	POST_IO
}
RET(12)

// CMPM
_OP(0xB148)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7];
	CPU->A[(Opcode >> 0) & 7] += 2;
	PRE_IO
	READ_WORD_F(adr, src)
	adr = CPU->A[(Opcode >> 9) & 7];
	CPU->A[(Opcode >> 9) & 7] += 2;
	READ_WORD_F(adr, dst)
	res = dst - src;
	CPU->flag_V = ((src ^ dst) & (res ^ dst)) >> 8;
	CPU->flag_N = CPU->flag_C = res >> 8;
	CPU->flag_notZ = res & 0xFFFF;
	POST_IO
}
RET(12)

// CMPM
_OP(0xB188)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7];
	CPU->A[(Opcode >> 0) & 7] += 4;
	PRE_IO
	READ_LONG_F(adr, src)
	adr = CPU->A[(Opcode >> 9) & 7];
	CPU->A[(Opcode >> 9) & 7] += 4;
	READ_LONG_F(adr, dst)
	res = dst - src;
	CPU->flag_notZ = res;
	CPU->flag_C = ((src & res & 1) + (src >> 1) + (res >> 1)) >> 23;
	CPU->flag_V = ((src ^ dst) & (res ^ dst)) >> 24;
	CPU->flag_N = res >> 24;
	POST_IO
}
RET(20)

// CMP7M
_OP(0xB10F)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[7];
	CPU->A[7] += 2;
	PRE_IO
	READ_BYTE_F(adr, src)
	adr = CPU->A[(Opcode >> 9) & 7];
	CPU->A[(Opcode >> 9) & 7] += 1;
	READ_BYTE_F(adr, dst)
	res = dst - src;
	CPU->flag_N = CPU->flag_C = res;
	CPU->flag_V = (src ^ dst) & (res ^ dst);
	CPU->flag_notZ = res & 0xFF;
	POST_IO
}
RET(12)

// CMP7M
_OP(0xB14F)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[7];
	CPU->A[7] += 2;
	PRE_IO
	READ_WORD_F(adr, src)
	adr = CPU->A[(Opcode >> 9) & 7];
	CPU->A[(Opcode >> 9) & 7] += 2;
	READ_WORD_F(adr, dst)
	res = dst - src;
	CPU->flag_V = ((src ^ dst) & (res ^ dst)) >> 8;
	CPU->flag_N = CPU->flag_C = res >> 8;
	CPU->flag_notZ = res & 0xFFFF;
	POST_IO
}
RET(12)

// CMP7M
_OP(0xB18F)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[7];
	CPU->A[7] += 4;
	PRE_IO
	READ_LONG_F(adr, src)
	adr = CPU->A[(Opcode >> 9) & 7];
	CPU->A[(Opcode >> 9) & 7] += 4;
	READ_LONG_F(adr, dst)
	res = dst - src;
	CPU->flag_notZ = res;
	CPU->flag_C = ((src & res & 1) + (src >> 1) + (res >> 1)) >> 23;
	CPU->flag_V = ((src ^ dst) & (res ^ dst)) >> 24;
	CPU->flag_N = res >> 24;
	POST_IO
}
RET(20)

// CMPM7
_OP(0xBF08)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7];
	CPU->A[(Opcode >> 0) & 7] += 1;
	PRE_IO
	READ_BYTE_F(adr, src)
	adr = CPU->A[7];
	CPU->A[7] += 2;
	READ_BYTE_F(adr, dst)
	res = dst - src;
	CPU->flag_N = CPU->flag_C = res;
	CPU->flag_V = (src ^ dst) & (res ^ dst);
	CPU->flag_notZ = res & 0xFF;
	POST_IO
}
RET(12)

// CMPM7
_OP(0xBF48)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7];
	CPU->A[(Opcode >> 0) & 7] += 2;
	PRE_IO
	READ_WORD_F(adr, src)
	adr = CPU->A[7];
	CPU->A[7] += 2;
	READ_WORD_F(adr, dst)
	res = dst - src;
	CPU->flag_V = ((src ^ dst) & (res ^ dst)) >> 8;
	CPU->flag_N = CPU->flag_C = res >> 8;
	CPU->flag_notZ = res & 0xFFFF;
	POST_IO
}
RET(12)

// CMPM7
_OP(0xBF88)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7];
	CPU->A[(Opcode >> 0) & 7] += 4;
	PRE_IO
	READ_LONG_F(adr, src)
	adr = CPU->A[7];
	CPU->A[7] += 4;
	READ_LONG_F(adr, dst)
	res = dst - src;
	CPU->flag_notZ = res;
	CPU->flag_C = ((src & res & 1) + (src >> 1) + (res >> 1)) >> 23;
	CPU->flag_V = ((src ^ dst) & (res ^ dst)) >> 24;
	CPU->flag_N = res >> 24;
	POST_IO
}
RET(20)

// CMP7M7
_OP(0xBF0F)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[7];
	CPU->A[7] += 2;
	PRE_IO
	READ_BYTE_F(adr, src)
	adr = CPU->A[7];
	CPU->A[7] += 2;
	READ_BYTE_F(adr, dst)
	res = dst - src;
	CPU->flag_N = CPU->flag_C = res;
	CPU->flag_V = (src ^ dst) & (res ^ dst);
	CPU->flag_notZ = res & 0xFF;
	POST_IO
}
RET(12)

// CMP7M7
_OP(0xBF4F)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[7];
	CPU->A[7] += 2;
	PRE_IO
	READ_WORD_F(adr, src)
	adr = CPU->A[7];
	CPU->A[7] += 2;
	READ_WORD_F(adr, dst)
	res = dst - src;
	CPU->flag_V = ((src ^ dst) & (res ^ dst)) >> 8;
	CPU->flag_N = CPU->flag_C = res >> 8;
	CPU->flag_notZ = res & 0xFFFF;
	POST_IO
}
RET(12)

// CMP7M7
_OP(0xBF8F)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[7];
	CPU->A[7] += 4;
	PRE_IO
	READ_LONG_F(adr, src)
	adr = CPU->A[7];
	CPU->A[7] += 4;
	READ_LONG_F(adr, dst)
	res = dst - src;
	CPU->flag_notZ = res;
	CPU->flag_C = ((src & res & 1) + (src >> 1) + (res >> 1)) >> 23;
	CPU->flag_V = ((src ^ dst) & (res ^ dst)) >> 24;
	CPU->flag_N = res >> 24;
	POST_IO
}
RET(20)

// EORDa
_OP(0xB100)
{
	u32 adr, res;
	u32 src, dst;

	src = (u8)CPU->D[(Opcode >> 9) & 7];
	res = (u8)CPU->D[(Opcode >> 0) & 7];
	res ^= src;
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res;
	*(u8*)(&CPU->D[(Opcode >> 0) & 7]) = res;
}
RET(4)

// EORDa
_OP(0xB110)
{
	u32 adr, res;
	u32 src, dst;

	src = (u8)CPU->D[(Opcode >> 9) & 7];
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
RET(12)

// EORDa
_OP(0xB118)
{
	u32 adr, res;
	u32 src, dst;

	src = (u8)CPU->D[(Opcode >> 9) & 7];
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
RET(12)

// EORDa
_OP(0xB120)
{
	u32 adr, res;
	u32 src, dst;

	src = (u8)CPU->D[(Opcode >> 9) & 7];
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
RET(14)

// EORDa
_OP(0xB128)
{
	u32 adr, res;
	u32 src, dst;

	src = (u8)CPU->D[(Opcode >> 9) & 7];
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
RET(16)

// EORDa
_OP(0xB130)
{
	u32 adr, res;
	u32 src, dst;

	src = (u8)CPU->D[(Opcode >> 9) & 7];
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
RET(18)

// EORDa
_OP(0xB138)
{
	u32 adr, res;
	u32 src, dst;

	src = (u8)CPU->D[(Opcode >> 9) & 7];
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
RET(16)

// EORDa
_OP(0xB139)
{
	u32 adr, res;
	u32 src, dst;

	src = (u8)CPU->D[(Opcode >> 9) & 7];
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
RET(20)

// EORDa
_OP(0xB11F)
{
	u32 adr, res;
	u32 src, dst;

	src = (u8)CPU->D[(Opcode >> 9) & 7];
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
RET(12)

// EORDa
_OP(0xB127)
{
	u32 adr, res;
	u32 src, dst;

	src = (u8)CPU->D[(Opcode >> 9) & 7];
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
RET(14)

// EORDa
_OP(0xB140)
{
	u32 adr, res;
	u32 src, dst;

	src = (u16)CPU->D[(Opcode >> 9) & 7];
	res = (u16)CPU->D[(Opcode >> 0) & 7];
	res ^= src;
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 8;
	*(u16*)(&CPU->D[(Opcode >> 0) & 7]) = res;
}
RET(4)

// EORDa
_OP(0xB150)
{
	u32 adr, res;
	u32 src, dst;

	src = (u16)CPU->D[(Opcode >> 9) & 7];
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
RET(12)

// EORDa
_OP(0xB158)
{
	u32 adr, res;
	u32 src, dst;

	src = (u16)CPU->D[(Opcode >> 9) & 7];
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
RET(12)

// EORDa
_OP(0xB160)
{
	u32 adr, res;
	u32 src, dst;

	src = (u16)CPU->D[(Opcode >> 9) & 7];
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
RET(14)

// EORDa
_OP(0xB168)
{
	u32 adr, res;
	u32 src, dst;

	src = (u16)CPU->D[(Opcode >> 9) & 7];
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
RET(16)

// EORDa
_OP(0xB170)
{
	u32 adr, res;
	u32 src, dst;

	src = (u16)CPU->D[(Opcode >> 9) & 7];
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
RET(18)

// EORDa
_OP(0xB178)
{
	u32 adr, res;
	u32 src, dst;

	src = (u16)CPU->D[(Opcode >> 9) & 7];
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
RET(16)

// EORDa
_OP(0xB179)
{
	u32 adr, res;
	u32 src, dst;

	src = (u16)CPU->D[(Opcode >> 9) & 7];
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
RET(20)

// EORDa
_OP(0xB15F)
{
	u32 adr, res;
	u32 src, dst;

	src = (u16)CPU->D[(Opcode >> 9) & 7];
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
RET(12)

// EORDa
_OP(0xB167)
{
	u32 adr, res;
	u32 src, dst;

	src = (u16)CPU->D[(Opcode >> 9) & 7];
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
RET(14)

// EORDa
_OP(0xB180)
{
	u32 adr, res;
	u32 src, dst;

	src = (u32)CPU->D[(Opcode >> 9) & 7];
	res = (u32)CPU->D[(Opcode >> 0) & 7];
	res ^= src;
	CPU->flag_C = 0;
	CPU->flag_V = 0;
	CPU->flag_notZ = res;
	CPU->flag_N = res >> 24;
	*(u32*)(&CPU->D[(Opcode >> 0) & 7]) = res;
}
RET(8)

// EORDa
_OP(0xB190)
{
	u32 adr, res;
	u32 src, dst;

	src = (u32)CPU->D[(Opcode >> 9) & 7];
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
RET(20)

// EORDa
_OP(0xB198)
{
	u32 adr, res;
	u32 src, dst;

	src = (u32)CPU->D[(Opcode >> 9) & 7];
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
RET(20)

// EORDa
_OP(0xB1A0)
{
	u32 adr, res;
	u32 src, dst;

	src = (u32)CPU->D[(Opcode >> 9) & 7];
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
RET(22)

// EORDa
_OP(0xB1A8)
{
	u32 adr, res;
	u32 src, dst;

	src = (u32)CPU->D[(Opcode >> 9) & 7];
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
RET(24)

// EORDa
_OP(0xB1B0)
{
	u32 adr, res;
	u32 src, dst;

	src = (u32)CPU->D[(Opcode >> 9) & 7];
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
RET(26)

// EORDa
_OP(0xB1B8)
{
	u32 adr, res;
	u32 src, dst;

	src = (u32)CPU->D[(Opcode >> 9) & 7];
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
RET(24)

// EORDa
_OP(0xB1B9)
{
	u32 adr, res;
	u32 src, dst;

	src = (u32)CPU->D[(Opcode >> 9) & 7];
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
RET(28)

// EORDa
_OP(0xB19F)
{
	u32 adr, res;
	u32 src, dst;

	src = (u32)CPU->D[(Opcode >> 9) & 7];
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
RET(20)

// EORDa
_OP(0xB1A7)
{
	u32 adr, res;
	u32 src, dst;

	src = (u32)CPU->D[(Opcode >> 9) & 7];
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
RET(22)

// CMPA
_OP(0xB0C0)
{
	u32 adr, res;
	u32 src, dst;

	src = (s32)(s16)CPU->D[(Opcode >> 0) & 7];
	dst = (u32)CPU->A[(Opcode >> 9) & 7];
	res = dst - src;
	CPU->flag_notZ = res;
	CPU->flag_C = ((src & res & 1) + (src >> 1) + (res >> 1)) >> 23;
	CPU->flag_V = ((src ^ dst) & (res ^ dst)) >> 24;
	CPU->flag_N = res >> 24;
}
RET(6)

// CMPA
_OP(0xB0C8)
{
	u32 adr, res;
	u32 src, dst;

	src = (s32)(s16)CPU->A[(Opcode >> 0) & 7];
	dst = (u32)CPU->A[(Opcode >> 9) & 7];
	res = dst - src;
	CPU->flag_notZ = res;
	CPU->flag_C = ((src & res & 1) + (src >> 1) + (res >> 1)) >> 23;
	CPU->flag_V = ((src ^ dst) & (res ^ dst)) >> 24;
	CPU->flag_N = res >> 24;
}
RET(6)

// CMPA
_OP(0xB0D0)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7];
	PRE_IO
	READSX_WORD_F(adr, src)
	dst = (u32)CPU->A[(Opcode >> 9) & 7];
	res = dst - src;
	CPU->flag_notZ = res;
	CPU->flag_C = ((src & res & 1) + (src >> 1) + (res >> 1)) >> 23;
	CPU->flag_V = ((src ^ dst) & (res ^ dst)) >> 24;
	CPU->flag_N = res >> 24;
	POST_IO
}
RET(10)

// CMPA
_OP(0xB0D8)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7];
	CPU->A[(Opcode >> 0) & 7] += 2;
	PRE_IO
	READSX_WORD_F(adr, src)
	dst = (u32)CPU->A[(Opcode >> 9) & 7];
	res = dst - src;
	CPU->flag_notZ = res;
	CPU->flag_C = ((src & res & 1) + (src >> 1) + (res >> 1)) >> 23;
	CPU->flag_V = ((src ^ dst) & (res ^ dst)) >> 24;
	CPU->flag_N = res >> 24;
	POST_IO
}
RET(10)

// CMPA
_OP(0xB0E0)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7] - 2;
	CPU->A[(Opcode >> 0) & 7] = adr;
	PRE_IO
	READSX_WORD_F(adr, src)
	dst = (u32)CPU->A[(Opcode >> 9) & 7];
	res = dst - src;
	CPU->flag_notZ = res;
	CPU->flag_C = ((src & res & 1) + (src >> 1) + (res >> 1)) >> 23;
	CPU->flag_V = ((src ^ dst) & (res ^ dst)) >> 24;
	CPU->flag_N = res >> 24;
	POST_IO
}
RET(12)

// CMPA
_OP(0xB0E8)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7] + (s32)(s16)FETCH_WORD;
	PC += 2;
	PRE_IO
	READSX_WORD_F(adr, src)
	dst = (u32)CPU->A[(Opcode >> 9) & 7];
	res = dst - src;
	CPU->flag_notZ = res;
	CPU->flag_C = ((src & res & 1) + (src >> 1) + (res >> 1)) >> 23;
	CPU->flag_V = ((src ^ dst) & (res ^ dst)) >> 24;
	CPU->flag_N = res >> 24;
	POST_IO
}
RET(14)

// CMPA
_OP(0xB0F0)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7];
	DECODE_EXT_WORD
	PRE_IO
	READSX_WORD_F(adr, src)
	dst = (u32)CPU->A[(Opcode >> 9) & 7];
	res = dst - src;
	CPU->flag_notZ = res;
	CPU->flag_C = ((src & res & 1) + (src >> 1) + (res >> 1)) >> 23;
	CPU->flag_V = ((src ^ dst) & (res ^ dst)) >> 24;
	CPU->flag_N = res >> 24;
	POST_IO
}
RET(16)

// CMPA
_OP(0xB0F8)
{
	u32 adr, res;
	u32 src, dst;

	adr = (s32)(s16)FETCH_WORD;
	PC += 2;
	PRE_IO
	READSX_WORD_F(adr, src)
	dst = (u32)CPU->A[(Opcode >> 9) & 7];
	res = dst - src;
	CPU->flag_notZ = res;
	CPU->flag_C = ((src & res & 1) + (src >> 1) + (res >> 1)) >> 23;
	CPU->flag_V = ((src ^ dst) & (res ^ dst)) >> 24;
	CPU->flag_N = res >> 24;
	POST_IO
}
RET(14)

// CMPA
_OP(0xB0F9)
{
	u32 adr, res;
	u32 src, dst;

	adr = (s32)FETCH_LONG;
	PC += 4;
	PRE_IO
	READSX_WORD_F(adr, src)
	dst = (u32)CPU->A[(Opcode >> 9) & 7];
	res = dst - src;
	CPU->flag_notZ = res;
	CPU->flag_C = ((src & res & 1) + (src >> 1) + (res >> 1)) >> 23;
	CPU->flag_V = ((src ^ dst) & (res ^ dst)) >> 24;
	CPU->flag_N = res >> 24;
	POST_IO
}
RET(18)

// CMPA
_OP(0xB0FA)
{
	u32 adr, res;
	u32 src, dst;

	adr = (PC - CPU->BasePC) + (s32)(s16)FETCH_WORD;
	PC += 2;
	PRE_IO
	READSX_WORD_F(adr, src)
	dst = (u32)CPU->A[(Opcode >> 9) & 7];
	res = dst - src;
	CPU->flag_notZ = res;
	CPU->flag_C = ((src & res & 1) + (src >> 1) + (res >> 1)) >> 23;
	CPU->flag_V = ((src ^ dst) & (res ^ dst)) >> 24;
	CPU->flag_N = res >> 24;
	POST_IO
}
RET(14)

// CMPA
_OP(0xB0FB)
{
	u32 adr, res;
	u32 src, dst;

	adr = PC - CPU->BasePC;
	DECODE_EXT_WORD
	PRE_IO
	READSX_WORD_F(adr, src)
	dst = (u32)CPU->A[(Opcode >> 9) & 7];
	res = dst - src;
	CPU->flag_notZ = res;
	CPU->flag_C = ((src & res & 1) + (src >> 1) + (res >> 1)) >> 23;
	CPU->flag_V = ((src ^ dst) & (res ^ dst)) >> 24;
	CPU->flag_N = res >> 24;
	POST_IO
}
RET(16)

// CMPA
_OP(0xB0FC)
{
	u32 adr, res;
	u32 src, dst;

	src = (s32)(s16)FETCH_WORD;
	PC += 2;
	dst = (u32)CPU->A[(Opcode >> 9) & 7];
	res = dst - src;
	CPU->flag_notZ = res;
	CPU->flag_C = ((src & res & 1) + (src >> 1) + (res >> 1)) >> 23;
	CPU->flag_V = ((src ^ dst) & (res ^ dst)) >> 24;
	CPU->flag_N = res >> 24;
}
RET(10)

// CMPA
_OP(0xB0DF)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[7];
	CPU->A[7] += 2;
	PRE_IO
	READSX_WORD_F(adr, src)
	dst = (u32)CPU->A[(Opcode >> 9) & 7];
	res = dst - src;
	CPU->flag_notZ = res;
	CPU->flag_C = ((src & res & 1) + (src >> 1) + (res >> 1)) >> 23;
	CPU->flag_V = ((src ^ dst) & (res ^ dst)) >> 24;
	CPU->flag_N = res >> 24;
	POST_IO
}
RET(10)

// CMPA
_OP(0xB0E7)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[7] - 2;
	CPU->A[7] = adr;
	PRE_IO
	READSX_WORD_F(adr, src)
	dst = (u32)CPU->A[(Opcode >> 9) & 7];
	res = dst - src;
	CPU->flag_notZ = res;
	CPU->flag_C = ((src & res & 1) + (src >> 1) + (res >> 1)) >> 23;
	CPU->flag_V = ((src ^ dst) & (res ^ dst)) >> 24;
	CPU->flag_N = res >> 24;
	POST_IO
}
RET(12)

// CMPA
_OP(0xB1C0)
{
	u32 adr, res;
	u32 src, dst;

	src = (s32)(s32)CPU->D[(Opcode >> 0) & 7];
	dst = (u32)CPU->A[(Opcode >> 9) & 7];
	res = dst - src;
	CPU->flag_notZ = res;
	CPU->flag_C = ((src & res & 1) + (src >> 1) + (res >> 1)) >> 23;
	CPU->flag_V = ((src ^ dst) & (res ^ dst)) >> 24;
	CPU->flag_N = res >> 24;
}
RET(6)

// CMPA
_OP(0xB1C8)
{
	u32 adr, res;
	u32 src, dst;

	src = (s32)(s32)CPU->A[(Opcode >> 0) & 7];
	dst = (u32)CPU->A[(Opcode >> 9) & 7];
	res = dst - src;
	CPU->flag_notZ = res;
	CPU->flag_C = ((src & res & 1) + (src >> 1) + (res >> 1)) >> 23;
	CPU->flag_V = ((src ^ dst) & (res ^ dst)) >> 24;
	CPU->flag_N = res >> 24;
}
RET(6)

// CMPA
_OP(0xB1D0)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7];
	PRE_IO
	READSX_LONG_F(adr, src)
	dst = (u32)CPU->A[(Opcode >> 9) & 7];
	res = dst - src;
	CPU->flag_notZ = res;
	CPU->flag_C = ((src & res & 1) + (src >> 1) + (res >> 1)) >> 23;
	CPU->flag_V = ((src ^ dst) & (res ^ dst)) >> 24;
	CPU->flag_N = res >> 24;
	POST_IO
}
RET(14)

// CMPA
_OP(0xB1D8)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7];
	CPU->A[(Opcode >> 0) & 7] += 4;
	PRE_IO
	READSX_LONG_F(adr, src)
	dst = (u32)CPU->A[(Opcode >> 9) & 7];
	res = dst - src;
	CPU->flag_notZ = res;
	CPU->flag_C = ((src & res & 1) + (src >> 1) + (res >> 1)) >> 23;
	CPU->flag_V = ((src ^ dst) & (res ^ dst)) >> 24;
	CPU->flag_N = res >> 24;
	POST_IO
}
RET(14)

// CMPA
_OP(0xB1E0)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7] - 4;
	CPU->A[(Opcode >> 0) & 7] = adr;
	PRE_IO
	READSX_LONG_F(adr, src)
	dst = (u32)CPU->A[(Opcode >> 9) & 7];
	res = dst - src;
	CPU->flag_notZ = res;
	CPU->flag_C = ((src & res & 1) + (src >> 1) + (res >> 1)) >> 23;
	CPU->flag_V = ((src ^ dst) & (res ^ dst)) >> 24;
	CPU->flag_N = res >> 24;
	POST_IO
}
RET(16)

// CMPA
_OP(0xB1E8)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7] + (s32)(s16)FETCH_WORD;
	PC += 2;
	PRE_IO
	READSX_LONG_F(adr, src)
	dst = (u32)CPU->A[(Opcode >> 9) & 7];
	res = dst - src;
	CPU->flag_notZ = res;
	CPU->flag_C = ((src & res & 1) + (src >> 1) + (res >> 1)) >> 23;
	CPU->flag_V = ((src ^ dst) & (res ^ dst)) >> 24;
	CPU->flag_N = res >> 24;
	POST_IO
}
RET(18)

// CMPA
_OP(0xB1F0)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7];
	DECODE_EXT_WORD
	PRE_IO
	READSX_LONG_F(adr, src)
	dst = (u32)CPU->A[(Opcode >> 9) & 7];
	res = dst - src;
	CPU->flag_notZ = res;
	CPU->flag_C = ((src & res & 1) + (src >> 1) + (res >> 1)) >> 23;
	CPU->flag_V = ((src ^ dst) & (res ^ dst)) >> 24;
	CPU->flag_N = res >> 24;
	POST_IO
}
RET(20)

// CMPA
_OP(0xB1F8)
{
	u32 adr, res;
	u32 src, dst;

	adr = (s32)(s16)FETCH_WORD;
	PC += 2;
	PRE_IO
	READSX_LONG_F(adr, src)
	dst = (u32)CPU->A[(Opcode >> 9) & 7];
	res = dst - src;
	CPU->flag_notZ = res;
	CPU->flag_C = ((src & res & 1) + (src >> 1) + (res >> 1)) >> 23;
	CPU->flag_V = ((src ^ dst) & (res ^ dst)) >> 24;
	CPU->flag_N = res >> 24;
	POST_IO
}
RET(18)

// CMPA
_OP(0xB1F9)
{
	u32 adr, res;
	u32 src, dst;

	adr = (s32)FETCH_LONG;
	PC += 4;
	PRE_IO
	READSX_LONG_F(adr, src)
	dst = (u32)CPU->A[(Opcode >> 9) & 7];
	res = dst - src;
	CPU->flag_notZ = res;
	CPU->flag_C = ((src & res & 1) + (src >> 1) + (res >> 1)) >> 23;
	CPU->flag_V = ((src ^ dst) & (res ^ dst)) >> 24;
	CPU->flag_N = res >> 24;
	POST_IO
}
RET(22)

// CMPA
_OP(0xB1FA)
{
	u32 adr, res;
	u32 src, dst;

	adr = (PC - CPU->BasePC) + (s32)(s16)FETCH_WORD;
	PC += 2;
	PRE_IO
	READSX_LONG_F(adr, src)
	dst = (u32)CPU->A[(Opcode >> 9) & 7];
	res = dst - src;
	CPU->flag_notZ = res;
	CPU->flag_C = ((src & res & 1) + (src >> 1) + (res >> 1)) >> 23;
	CPU->flag_V = ((src ^ dst) & (res ^ dst)) >> 24;
	CPU->flag_N = res >> 24;
	POST_IO
}
RET(18)

// CMPA
_OP(0xB1FB)
{
	u32 adr, res;
	u32 src, dst;

	adr = PC - CPU->BasePC;
	DECODE_EXT_WORD
	PRE_IO
	READSX_LONG_F(adr, src)
	dst = (u32)CPU->A[(Opcode >> 9) & 7];
	res = dst - src;
	CPU->flag_notZ = res;
	CPU->flag_C = ((src & res & 1) + (src >> 1) + (res >> 1)) >> 23;
	CPU->flag_V = ((src ^ dst) & (res ^ dst)) >> 24;
	CPU->flag_N = res >> 24;
	POST_IO
}
RET(20)

// CMPA
_OP(0xB1FC)
{
	u32 adr, res;
	u32 src, dst;

	src = (s32)(s32)FETCH_LONG;
	PC += 4;
	dst = (u32)CPU->A[(Opcode >> 9) & 7];
	res = dst - src;
	CPU->flag_notZ = res;
	CPU->flag_C = ((src & res & 1) + (src >> 1) + (res >> 1)) >> 23;
	CPU->flag_V = ((src ^ dst) & (res ^ dst)) >> 24;
	CPU->flag_N = res >> 24;
}
RET(14)

// CMPA
_OP(0xB1DF)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[7];
	CPU->A[7] += 4;
	PRE_IO
	READSX_LONG_F(adr, src)
	dst = (u32)CPU->A[(Opcode >> 9) & 7];
	res = dst - src;
	CPU->flag_notZ = res;
	CPU->flag_C = ((src & res & 1) + (src >> 1) + (res >> 1)) >> 23;
	CPU->flag_V = ((src ^ dst) & (res ^ dst)) >> 24;
	CPU->flag_N = res >> 24;
	POST_IO
}
RET(14)

// CMPA
_OP(0xB1E7)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[7] - 4;
	CPU->A[7] = adr;
	PRE_IO
	READSX_LONG_F(adr, src)
	dst = (u32)CPU->A[(Opcode >> 9) & 7];
	res = dst - src;
	CPU->flag_notZ = res;
	CPU->flag_C = ((src & res & 1) + (src >> 1) + (res >> 1)) >> 23;
	CPU->flag_V = ((src ^ dst) & (res ^ dst)) >> 24;
	CPU->flag_N = res >> 24;
	POST_IO
}
RET(16)
