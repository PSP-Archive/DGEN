
// ASRk
_OP(0xE000)
{
	u32 adr, res;
	u32 src, dst;

	u32 sft;

	sft = (((Opcode >> 9) - 1) & 7) + 1;
	CCnt -= sft * 2;
	src = (s32)(s8)CPU->D[(Opcode >> 0) & 7];
	CPU->flag_V = 0;
	CPU->flag_X = CPU->flag_C = src << ((C68K_SR_C_SFT + 1) - sft);
	res = ((s32)src) >> sft;
	CPU->flag_N = res >> 0;
	CPU->flag_notZ = res;
	*(u8*)(&CPU->D[(Opcode >> 0) & 7]) = res;
}
RET(6)

// ASRk
_OP(0xE040)
{
	u32 adr, res;
	u32 src, dst;

	u32 sft;

	sft = (((Opcode >> 9) - 1) & 7) + 1;
	CCnt -= sft * 2;
	src = (s32)(s16)CPU->D[(Opcode >> 0) & 7];
	CPU->flag_V = 0;
	CPU->flag_X = CPU->flag_C = src << ((C68K_SR_C_SFT + 1) - sft);
	res = ((s32)src) >> sft;
	CPU->flag_N = res >> 8;
	CPU->flag_notZ = res;
	*(u16*)(&CPU->D[(Opcode >> 0) & 7]) = res;
}
RET(6)

// ASRk
_OP(0xE080)
{
	u32 adr, res;
	u32 src, dst;

	u32 sft;

	sft = (((Opcode >> 9) - 1) & 7) + 1;
	CCnt -= sft * 2;
	src = (s32)(s32)CPU->D[(Opcode >> 0) & 7];
	CPU->flag_V = 0;
	CPU->flag_X = CPU->flag_C = src << ((C68K_SR_C_SFT + 1) - sft);
	res = ((s32)src) >> sft;
	CPU->flag_N = res >> 24;
	CPU->flag_notZ = res;
	*(u32*)(&CPU->D[(Opcode >> 0) & 7]) = res;
}
RET(8)

// LSRk
_OP(0xE008)
{
	u32 adr, res;
	u32 src, dst;

	u32 sft;

	sft = (((Opcode >> 9) - 1) & 7) + 1;
	CCnt -= sft * 2;
	src = (u8)CPU->D[(Opcode >> 0) & 7];
	CPU->flag_N = CPU->flag_V = 0;
	CPU->flag_X = CPU->flag_C = src << ((C68K_SR_C_SFT + 1) - sft);
	res = src >> sft;
	CPU->flag_notZ = res;
	*(u8*)(&CPU->D[(Opcode >> 0) & 7]) = res;
}
RET(6)

// LSRk
_OP(0xE048)
{
	u32 adr, res;
	u32 src, dst;

	u32 sft;

	sft = (((Opcode >> 9) - 1) & 7) + 1;
	CCnt -= sft * 2;
	src = (u16)CPU->D[(Opcode >> 0) & 7];
	CPU->flag_N = CPU->flag_V = 0;
	CPU->flag_X = CPU->flag_C = src << ((C68K_SR_C_SFT + 1) - sft);
	res = src >> sft;
	CPU->flag_notZ = res;
	*(u16*)(&CPU->D[(Opcode >> 0) & 7]) = res;
}
RET(6)

// LSRk
_OP(0xE088)
{
	u32 adr, res;
	u32 src, dst;

	u32 sft;

	sft = (((Opcode >> 9) - 1) & 7) + 1;
	CCnt -= sft * 2;
	src = (u32)CPU->D[(Opcode >> 0) & 7];
	CPU->flag_N = CPU->flag_V = 0;
	CPU->flag_X = CPU->flag_C = src << ((C68K_SR_C_SFT + 1) - sft);
	res = src >> sft;
	CPU->flag_notZ = res;
	*(u32*)(&CPU->D[(Opcode >> 0) & 7]) = res;
}
RET(8)

// ROXRk
_OP(0xE010)
{
	u32 adr, res;
	u32 src, dst;

	u32 sft;

	sft = (((Opcode >> 9) - 1) & 7) + 1;
	CCnt -= sft * 2;
	src = (u8)CPU->D[(Opcode >> 0) & 7];
	src |= (CPU->flag_X & C68K_SR_X) << 0;
	res = (src >> sft) | (src << (9 - sft));
	CPU->flag_X = CPU->flag_C = res >> 0;
	CPU->flag_V = 0;
	CPU->flag_N = res >> 0;
	CPU->flag_notZ = res & 0x000000FF;
	*(u8*)(&CPU->D[(Opcode >> 0) & 7]) = res;
}
RET(6)

// ROXRk
_OP(0xE050)
{
	u32 adr, res;
	u32 src, dst;

	u32 sft;

	sft = (((Opcode >> 9) - 1) & 7) + 1;
	CCnt -= sft * 2;
	src = (u16)CPU->D[(Opcode >> 0) & 7];
	src |= (CPU->flag_X & C68K_SR_X) << 8;
	res = (src >> sft) | (src << (17 - sft));
	CPU->flag_X = CPU->flag_C = res >> 8;
	CPU->flag_V = 0;
	CPU->flag_N = res >> 8;
	CPU->flag_notZ = res & 0x0000FFFF;
	*(u16*)(&CPU->D[(Opcode >> 0) & 7]) = res;
}
RET(6)

// ROXRk
_OP(0xE090)
{
	u32 adr, res;
	u32 src, dst;

	u32 sft;

	sft = (((Opcode >> 9) - 1) & 7) + 1;
	CCnt -= sft * 2;
	src = (u32)CPU->D[(Opcode >> 0) & 7];
	CPU->flag_C = src << ((C68K_SR_C_SFT + 1) - sft);
	if (sft == 1) res = (src >> 1) | ((CPU->flag_X & C68K_SR_X) << (32 - (C68K_SR_X_SFT + 1)));
	else res = (src >> sft) | (src << (33 - sft)) | ((CPU->flag_X & C68K_SR_X) << (32 - (C68K_SR_X_SFT + sft)));
	CPU->flag_X = CPU->flag_C;
	CPU->flag_V = 0;
	CPU->flag_N = res >> 24;
	CPU->flag_notZ = res;
	*(u32*)(&CPU->D[(Opcode >> 0) & 7]) = res;
}
RET(8)

// RORk
_OP(0xE018)
{
	u32 adr, res;
	u32 src, dst;

	u32 sft;

	sft = (((Opcode >> 9) - 1) & 7) + 1;
	CCnt -= sft * 2;
	src = (u8)CPU->D[(Opcode >> 0) & 7];
	CPU->flag_V = 0;
	CPU->flag_C = src << ((C68K_SR_C_SFT + 1) - sft);
	res = (src >> sft) | (src << (8 - sft));
	CPU->flag_N = res >> 0;
	CPU->flag_notZ = res & 0x000000FF;
	*(u8*)(&CPU->D[(Opcode >> 0) & 7]) = res;
}
RET(6)

// RORk
_OP(0xE058)
{
	u32 adr, res;
	u32 src, dst;

	u32 sft;

	sft = (((Opcode >> 9) - 1) & 7) + 1;
	CCnt -= sft * 2;
	src = (u16)CPU->D[(Opcode >> 0) & 7];
	CPU->flag_V = 0;
	CPU->flag_C = src << ((C68K_SR_C_SFT + 1) - sft);
	res = (src >> sft) | (src << (16 - sft));
	CPU->flag_N = res >> 8;
	CPU->flag_notZ = res & 0x0000FFFF;
	*(u16*)(&CPU->D[(Opcode >> 0) & 7]) = res;
}
RET(6)

// RORk
_OP(0xE098)
{
	u32 adr, res;
	u32 src, dst;

	u32 sft;

	sft = (((Opcode >> 9) - 1) & 7) + 1;
	CCnt -= sft * 2;
	src = (u32)CPU->D[(Opcode >> 0) & 7];
	CPU->flag_V = 0;
	CPU->flag_C = src << ((C68K_SR_C_SFT + 1) - sft);
	res = (src >> sft) | (src << (32 - sft));
	CPU->flag_N = res >> 24;
	CPU->flag_notZ = res;
	*(u32*)(&CPU->D[(Opcode >> 0) & 7]) = res;
}
RET(8)

// ASLk
_OP(0xE100)
{
	u32 adr, res;
	u32 src, dst;

	u32 sft;

	sft = (((Opcode >> 9) - 1) & 7) + 1;
	CCnt -= sft * 2;
	src = (u8)CPU->D[(Opcode >> 0) & 7];
	if (sft < 8)
	{
		CPU->flag_X = CPU->flag_C = src << (0 + sft);
		res = src << sft;
		CPU->flag_N = res >> 0;
		CPU->flag_notZ = res & 0x000000FF;
	*(u8*)(&CPU->D[(Opcode >> 0) & 7]) = res;
		CPU->flag_V = 0;
		if ((sft > 7) && (src)) CPU->flag_V = C68K_SR_V;
		else
		{
			u32 msk = (((s32)0x80000000) >> (sft + 24)) & 0x000000FF;
			src &= msk;
			if ((src) && (src != msk)) CPU->flag_V = C68K_SR_V;
		}
	RET3(6)
	}

	if (src) CPU->flag_V = C68K_SR_V;
	else CPU->flag_V = 0;
	CPU->flag_X = CPU->flag_C = src << C68K_SR_C_SFT;
	res = 0;
	*(u8*)(&CPU->D[(Opcode >> 0) & 7]) = res;
	CPU->flag_N = 0;
	CPU->flag_notZ = 0;
}
RET(6)

// ASLk
_OP(0xE140)
{
	u32 adr, res;
	u32 src, dst;

	u32 sft;

	sft = (((Opcode >> 9) - 1) & 7) + 1;
	CCnt -= sft * 2;
	src = (u16)CPU->D[(Opcode >> 0) & 7];
		CPU->flag_X = CPU->flag_C = src >> (8 - sft);
		res = src << sft;
		CPU->flag_N = res >> 8;
		CPU->flag_notZ = res & 0x0000FFFF;
	*(u16*)(&CPU->D[(Opcode >> 0) & 7]) = res;
		CPU->flag_V = 0;
		{
			u32 msk = (((s32)0x80000000) >> (sft + 16)) & 0x0000FFFF;
			src &= msk;
			if ((src) && (src != msk)) CPU->flag_V = C68K_SR_V;
		}
}
RET(6)

// ASLk
_OP(0xE180)
{
	u32 adr, res;
	u32 src, dst;

	u32 sft;

	sft = (((Opcode >> 9) - 1) & 7) + 1;
	CCnt -= sft * 2;
	src = (u32)CPU->D[(Opcode >> 0) & 7];
		CPU->flag_X = CPU->flag_C = src >> (24 - sft);
		res = src << sft;
		CPU->flag_N = res >> 24;
		CPU->flag_notZ = res & 0xFFFFFFFF;
	*(u32*)(&CPU->D[(Opcode >> 0) & 7]) = res;
		CPU->flag_V = 0;
		{
			u32 msk = (((s32)0x80000000) >> (sft + 0)) & 0xFFFFFFFF;
			src &= msk;
			if ((src) && (src != msk)) CPU->flag_V = C68K_SR_V;
		}
}
RET(8)

// LSLk
_OP(0xE108)
{
	u32 adr, res;
	u32 src, dst;

	u32 sft;

	sft = (((Opcode >> 9) - 1) & 7) + 1;
	CCnt -= sft * 2;
	src = (u8)CPU->D[(Opcode >> 0) & 7];
	CPU->flag_V = 0;
	CPU->flag_X = CPU->flag_C = src << (0 + sft);
	res = src << sft;
	CPU->flag_N = res >> 0;
	CPU->flag_notZ = res & 0x000000FF;
	*(u8*)(&CPU->D[(Opcode >> 0) & 7]) = res;
}
RET(6)

// LSLk
_OP(0xE148)
{
	u32 adr, res;
	u32 src, dst;

	u32 sft;

	sft = (((Opcode >> 9) - 1) & 7) + 1;
	CCnt -= sft * 2;
	src = (u16)CPU->D[(Opcode >> 0) & 7];
	CPU->flag_V = 0;
	CPU->flag_X = CPU->flag_C = src >> (8 - sft);
	res = src << sft;
	CPU->flag_N = res >> 8;
	CPU->flag_notZ = res & 0x0000FFFF;
	*(u16*)(&CPU->D[(Opcode >> 0) & 7]) = res;
}
RET(6)

// LSLk
_OP(0xE188)
{
	u32 adr, res;
	u32 src, dst;

	u32 sft;

	sft = (((Opcode >> 9) - 1) & 7) + 1;
	CCnt -= sft * 2;
	src = (u32)CPU->D[(Opcode >> 0) & 7];
	CPU->flag_V = 0;
	CPU->flag_X = CPU->flag_C = src >> (24 - sft);
	res = src << sft;
	CPU->flag_N = res >> 24;
	CPU->flag_notZ = res & 0xFFFFFFFF;
	*(u32*)(&CPU->D[(Opcode >> 0) & 7]) = res;
}
RET(8)

// ROXLk
_OP(0xE110)
{
	u32 adr, res;
	u32 src, dst;

	u32 sft;

	sft = (((Opcode >> 9) - 1) & 7) + 1;
	CCnt -= sft * 2;
	src = (u8)CPU->D[(Opcode >> 0) & 7];
	src |= (CPU->flag_X & C68K_SR_X) << 0;
	res = (src << sft) | (src >> (9 - sft));
	CPU->flag_X = CPU->flag_C = res >> 0;
	CPU->flag_V = 0;
	CPU->flag_N = res >> 0;
	CPU->flag_notZ = res & 0x000000FF;
	*(u8*)(&CPU->D[(Opcode >> 0) & 7]) = res;
}
RET(6)

// ROXLk
_OP(0xE150)
{
	u32 adr, res;
	u32 src, dst;

	u32 sft;

	sft = (((Opcode >> 9) - 1) & 7) + 1;
	CCnt -= sft * 2;
	src = (u16)CPU->D[(Opcode >> 0) & 7];
	src |= (CPU->flag_X & C68K_SR_X) << 8;
	res = (src << sft) | (src >> (17 - sft));
	CPU->flag_X = CPU->flag_C = res >> 8;
	CPU->flag_V = 0;
	CPU->flag_N = res >> 8;
	CPU->flag_notZ = res & 0x0000FFFF;
	*(u16*)(&CPU->D[(Opcode >> 0) & 7]) = res;
}
RET(6)

// ROXLk
_OP(0xE190)
{
	u32 adr, res;
	u32 src, dst;

	u32 sft;

	sft = (((Opcode >> 9) - 1) & 7) + 1;
	CCnt -= sft * 2;
	src = (u32)CPU->D[(Opcode >> 0) & 7];
	CPU->flag_C = src >> ((32 - C68K_SR_C_SFT) - sft);
	if (sft == 1) res = (src << 1) | ((CPU->flag_X & C68K_SR_X) >> ((C68K_SR_X_SFT + 1) - 1));
	else res = (src << sft) | (src >> (33 - sft)) | ((CPU->flag_X & C68K_SR_X) >> ((C68K_SR_X_SFT + 1) - sft));
	CPU->flag_X = CPU->flag_C;
	CPU->flag_V = 0;
	CPU->flag_N = res >> 24;
	CPU->flag_notZ = res;
	*(u32*)(&CPU->D[(Opcode >> 0) & 7]) = res;
}
RET(8)

// ROLk
_OP(0xE118)
{
	u32 adr, res;
	u32 src, dst;

	u32 sft;

	sft = (((Opcode >> 9) - 1) & 7) + 1;
	CCnt -= sft * 2;
	src = (u8)CPU->D[(Opcode >> 0) & 7];
	CPU->flag_V = 0;
	CPU->flag_C = src << (0 + sft);
	res = (src << sft) | (src >> (8 - sft));
	CPU->flag_N = res >> 0;
	CPU->flag_notZ = res & 0x000000FF;
	*(u8*)(&CPU->D[(Opcode >> 0) & 7]) = res;
}
RET(6)

// ROLk
_OP(0xE158)
{
	u32 adr, res;
	u32 src, dst;

	u32 sft;

	sft = (((Opcode >> 9) - 1) & 7) + 1;
	CCnt -= sft * 2;
	src = (u16)CPU->D[(Opcode >> 0) & 7];
	CPU->flag_V = 0;
	CPU->flag_C = src >> (8 - sft);
	res = (src << sft) | (src >> (16 - sft));
	CPU->flag_N = res >> 8;
	CPU->flag_notZ = res & 0x0000FFFF;
	*(u16*)(&CPU->D[(Opcode >> 0) & 7]) = res;
}
RET(6)

// ROLk
_OP(0xE198)
{
	u32 adr, res;
	u32 src, dst;

	u32 sft;

	sft = (((Opcode >> 9) - 1) & 7) + 1;
	CCnt -= sft * 2;
	src = (u32)CPU->D[(Opcode >> 0) & 7];
	CPU->flag_V = 0;
	CPU->flag_C = src >> (24 - sft);
	res = (src << sft) | (src >> (32 - sft));
	CPU->flag_N = res >> 24;
	CPU->flag_notZ = res;
	*(u32*)(&CPU->D[(Opcode >> 0) & 7]) = res;
}
RET(8)

// ASRD
_OP(0xE020)
{
	u32 adr, res;
	u32 src, dst;

	u32 sft;

	sft = CPU->D[(Opcode >> 9) & 7] & 0x3F;
	src = (s32)(s8)CPU->D[(Opcode >> 0) & 7];
	if (sft)
	{
	CCnt -= sft * 2;
		if (sft < 8)
		{
			CPU->flag_V = 0;
			CPU->flag_X = CPU->flag_C = src << ((C68K_SR_C_SFT + 1) - sft);
			res = ((s32)src) >> sft;
			CPU->flag_N = res >> 0;
			CPU->flag_notZ = res;
	*(u8*)(&CPU->D[(Opcode >> 0) & 7]) = res;
	RET3(6)
		}

		if (src & (1 << 7))
		{
			CPU->flag_N = C68K_SR_N;
			CPU->flag_notZ = 1;
			CPU->flag_V = 0;
			CPU->flag_C = C68K_SR_C;
			CPU->flag_X = C68K_SR_X;
			res = 0x000000FF;
	*(u8*)(&CPU->D[(Opcode >> 0) & 7]) = res;
	RET3(6)
		}

		CPU->flag_N = 0;
		CPU->flag_notZ = 0;
		CPU->flag_V = 0;
		CPU->flag_C = 0;
		CPU->flag_X = 0;
		res = 0;
	*(u8*)(&CPU->D[(Opcode >> 0) & 7]) = res;
	RET3(6)
	}

	CPU->flag_V = 0;
	CPU->flag_C = 0;
	CPU->flag_N = src >> 0;
	CPU->flag_notZ = src;
}
RET(6)

// ASRD
_OP(0xE060)
{
	u32 adr, res;
	u32 src, dst;

	u32 sft;

	sft = CPU->D[(Opcode >> 9) & 7] & 0x3F;
	src = (s32)(s16)CPU->D[(Opcode >> 0) & 7];
	if (sft)
	{
	CCnt -= sft * 2;
		if (sft < 16)
		{
			CPU->flag_V = 0;
			CPU->flag_X = CPU->flag_C = (src >> (sft - 1)) << C68K_SR_C_SFT;
			res = ((s32)src) >> sft;
			CPU->flag_N = res >> 8;
			CPU->flag_notZ = res;
	*(u16*)(&CPU->D[(Opcode >> 0) & 7]) = res;
	RET3(6)
		}

		if (src & (1 << 15))
		{
			CPU->flag_N = C68K_SR_N;
			CPU->flag_notZ = 1;
			CPU->flag_V = 0;
			CPU->flag_C = C68K_SR_C;
			CPU->flag_X = C68K_SR_X;
			res = 0x0000FFFF;
	*(u16*)(&CPU->D[(Opcode >> 0) & 7]) = res;
	RET3(6)
		}

		CPU->flag_N = 0;
		CPU->flag_notZ = 0;
		CPU->flag_V = 0;
		CPU->flag_C = 0;
		CPU->flag_X = 0;
		res = 0;
	*(u16*)(&CPU->D[(Opcode >> 0) & 7]) = res;
	RET3(6)
	}

	CPU->flag_V = 0;
	CPU->flag_C = 0;
	CPU->flag_N = src >> 8;
	CPU->flag_notZ = src;
}
RET(6)

// ASRD
_OP(0xE0A0)
{
	u32 adr, res;
	u32 src, dst;

	u32 sft;

	sft = CPU->D[(Opcode >> 9) & 7] & 0x3F;
	src = (s32)(s32)CPU->D[(Opcode >> 0) & 7];
	if (sft)
	{
	CCnt -= sft * 2;
		if (sft < 32)
		{
			CPU->flag_V = 0;
			CPU->flag_X = CPU->flag_C = (src >> (sft - 1)) << C68K_SR_C_SFT;
			res = ((s32)src) >> sft;
			CPU->flag_N = res >> 24;
			CPU->flag_notZ = res;
	*(u32*)(&CPU->D[(Opcode >> 0) & 7]) = res;
	RET3(6)
		}

		if (src & (1 << 31))
		{
			CPU->flag_N = C68K_SR_N;
			CPU->flag_notZ = 1;
			CPU->flag_V = 0;
			CPU->flag_C = C68K_SR_C;
			CPU->flag_X = C68K_SR_X;
			res = 0xFFFFFFFF;
	*(u32*)(&CPU->D[(Opcode >> 0) & 7]) = res;
	RET3(6)
		}

		CPU->flag_N = 0;
		CPU->flag_notZ = 0;
		CPU->flag_V = 0;
		CPU->flag_C = 0;
		CPU->flag_X = 0;
		res = 0;
	*(u32*)(&CPU->D[(Opcode >> 0) & 7]) = res;
	RET3(6)
	}

	CPU->flag_V = 0;
	CPU->flag_C = 0;
	CPU->flag_N = src >> 24;
	CPU->flag_notZ = src;
}
RET(6)

// LSRD
_OP(0xE028)
{
	u32 adr, res;
	u32 src, dst;

	u32 sft;

	sft = CPU->D[(Opcode >> 9) & 7] & 0x3F;
	src = (u8)CPU->D[(Opcode >> 0) & 7];
	if (sft)
	{
	CCnt -= sft * 2;
		if (sft <= 8)
		{
			CPU->flag_N = CPU->flag_V = 0;
			CPU->flag_X = CPU->flag_C = src << ((C68K_SR_C_SFT + 1) - sft);
			res = src >> sft;
			CPU->flag_notZ = res;
	*(u8*)(&CPU->D[(Opcode >> 0) & 7]) = res;
	RET3(6)
		}

		CPU->flag_X = CPU->flag_C = 0;
		CPU->flag_N = 0;
		CPU->flag_notZ = 0;
		CPU->flag_V = 0;
		res = 0;
	*(u8*)(&CPU->D[(Opcode >> 0) & 7]) = res;
	RET3(6)
	}

	CPU->flag_V = 0;
	CPU->flag_C = 0;
	CPU->flag_N = src >> 0;
	CPU->flag_notZ = src;
}
RET(6)

// LSRD
_OP(0xE068)
{
	u32 adr, res;
	u32 src, dst;

	u32 sft;

	sft = CPU->D[(Opcode >> 9) & 7] & 0x3F;
	src = (u16)CPU->D[(Opcode >> 0) & 7];
	if (sft)
	{
	CCnt -= sft * 2;
		if (sft <= 16)
		{
			CPU->flag_N = CPU->flag_V = 0;
			CPU->flag_X = CPU->flag_C = (src >> (sft - 1)) << C68K_SR_C_SFT;
			res = src >> sft;
			CPU->flag_notZ = res;
	*(u16*)(&CPU->D[(Opcode >> 0) & 7]) = res;
	RET3(6)
		}

		CPU->flag_X = CPU->flag_C = 0;
		CPU->flag_N = 0;
		CPU->flag_notZ = 0;
		CPU->flag_V = 0;
		res = 0;
	*(u16*)(&CPU->D[(Opcode >> 0) & 7]) = res;
	RET3(6)
	}

	CPU->flag_V = 0;
	CPU->flag_C = 0;
	CPU->flag_N = src >> 8;
	CPU->flag_notZ = src;
}
RET(6)

// LSRD
_OP(0xE0A8)
{
	u32 adr, res;
	u32 src, dst;

	u32 sft;

	sft = CPU->D[(Opcode >> 9) & 7] & 0x3F;
	src = (u32)CPU->D[(Opcode >> 0) & 7];
	if (sft)
	{
	CCnt -= sft * 2;
		if (sft < 32)
		{
			CPU->flag_N = CPU->flag_V = 0;
			CPU->flag_X = CPU->flag_C = (src >> (sft - 1)) << C68K_SR_C_SFT;
			res = src >> sft;
			CPU->flag_notZ = res;
	*(u32*)(&CPU->D[(Opcode >> 0) & 7]) = res;
	RET3(6)
		}

		if (sft == 32) CPU->flag_C = src >> (31 - C68K_SR_C_SFT);
		else CPU->flag_C = 0;
		CPU->flag_X = CPU->flag_C;
		CPU->flag_N = 0;
		CPU->flag_notZ = 0;
		CPU->flag_V = 0;
		res = 0;
	*(u32*)(&CPU->D[(Opcode >> 0) & 7]) = res;
	RET3(6)
	}

	CPU->flag_V = 0;
	CPU->flag_C = 0;
	CPU->flag_N = src >> 24;
	CPU->flag_notZ = src;
}
RET(6)

// ROXRD
_OP(0xE030)
{
	u32 adr, res;
	u32 src, dst;

	u32 sft;

	sft = CPU->D[(Opcode >> 9) & 7] & 0x3F;
	src = (u8)CPU->D[(Opcode >> 0) & 7];
	if (sft)
	{
	CCnt -= sft * 2;
		sft %= 9;

		src |= (CPU->flag_X & C68K_SR_X) << 0;
		res = (src >> sft) | (src << (9 - sft));
		CPU->flag_X = CPU->flag_C = res >> 0;
		CPU->flag_V = 0;
		CPU->flag_N = res >> 0;
		CPU->flag_notZ = res & 0x000000FF;
	*(u8*)(&CPU->D[(Opcode >> 0) & 7]) = res;
	RET3(6)
	}

	CPU->flag_V = 0;
	CPU->flag_C = CPU->flag_X;
	CPU->flag_N = src >> 0;
	CPU->flag_notZ = src;
}
RET(6)

// ROXRD
_OP(0xE070)
{
	u32 adr, res;
	u32 src, dst;

	u32 sft;

	sft = CPU->D[(Opcode >> 9) & 7] & 0x3F;
	src = (u16)CPU->D[(Opcode >> 0) & 7];
	if (sft)
	{
	CCnt -= sft * 2;
		sft %= 17;

		src |= (CPU->flag_X & C68K_SR_X) << 8;
		res = (src >> sft) | (src << (17 - sft));
		CPU->flag_X = CPU->flag_C = res >> 8;
		CPU->flag_V = 0;
		CPU->flag_N = res >> 8;
		CPU->flag_notZ = res & 0x0000FFFF;
	*(u16*)(&CPU->D[(Opcode >> 0) & 7]) = res;
	RET3(6)
	}

	CPU->flag_V = 0;
	CPU->flag_C = CPU->flag_X;
	CPU->flag_N = src >> 8;
	CPU->flag_notZ = src;
}
RET(6)

// ROXRD
_OP(0xE0B0)
{
	u32 adr, res;
	u32 src, dst;

	u32 sft;

	sft = CPU->D[(Opcode >> 9) & 7] & 0x3F;
	src = (u32)CPU->D[(Opcode >> 0) & 7];
	if (sft)
	{
	CCnt -= sft * 2;
		sft %= 33;

		if (sft != 0)
		{
			if (sft == 1) res = (src >> 1) | ((CPU->flag_X & C68K_SR_X) << (32 - (C68K_SR_X_SFT + 1)));
			else res = (src >> sft) | (src << (33 - sft)) | (((CPU->flag_X & C68K_SR_X) << (32 - (C68K_SR_X_SFT + 1))) >> (sft - 1));
			CPU->flag_X = (src >> (32 - sft)) << C68K_SR_X_SFT;
		}
		else res = src;
		CPU->flag_C = CPU->flag_X;
		CPU->flag_V = 0;
		CPU->flag_N = res >> 24;
		CPU->flag_notZ = res;
	*(u32*)(&CPU->D[(Opcode >> 0) & 7]) = res;
	RET3(6)
	}

	CPU->flag_V = 0;
	CPU->flag_C = CPU->flag_X;
	CPU->flag_N = src >> 24;
	CPU->flag_notZ = src;
}
RET(6)

// RORD
_OP(0xE038)
{
	u32 adr, res;
	u32 src, dst;

	u32 sft;

	sft = CPU->D[(Opcode >> 9) & 7] & 0x3F;
	src = (u8)CPU->D[(Opcode >> 0) & 7];
	if (sft)
	{
	CCnt -= sft * 2;
		sft &= 0x07;
		
		CPU->flag_C = src << (C68K_SR_C_SFT - ((sft - 1) & 7));
		res = (src >> sft) | (src << (8 - sft));
		CPU->flag_V = 0;
		CPU->flag_N = res >> 0;
		CPU->flag_notZ = res & 0x000000FF;
	*(u8*)(&CPU->D[(Opcode >> 0) & 7]) = res;
	RET3(6)
	}

	CPU->flag_V = 0;
	CPU->flag_C = 0;
	CPU->flag_N = src >> 0;
	CPU->flag_notZ = src;
}
RET(6)

// RORD
_OP(0xE078)
{
	u32 adr, res;
	u32 src, dst;

	u32 sft;

	sft = CPU->D[(Opcode >> 9) & 7] & 0x3F;
	src = (u16)CPU->D[(Opcode >> 0) & 7];
	if (sft)
	{
	CCnt -= sft * 2;
		sft &= 0x0F;
		
		CPU->flag_C = (src >> ((sft - 1) & 15)) << C68K_SR_C_SFT;
		res = (src >> sft) | (src << (16 - sft));
		CPU->flag_V = 0;
		CPU->flag_N = res >> 8;
		CPU->flag_notZ = res & 0x0000FFFF;
	*(u16*)(&CPU->D[(Opcode >> 0) & 7]) = res;
	RET3(6)
	}

	CPU->flag_V = 0;
	CPU->flag_C = 0;
	CPU->flag_N = src >> 8;
	CPU->flag_notZ = src;
}
RET(6)

// RORD
_OP(0xE0B8)
{
	u32 adr, res;
	u32 src, dst;

	u32 sft;

	sft = CPU->D[(Opcode >> 9) & 7] & 0x3F;
	src = (u32)CPU->D[(Opcode >> 0) & 7];
	if (sft)
	{
	CCnt -= sft * 2;
		sft &= 0x1F;
		
		CPU->flag_C = (src >> ((sft - 1) & 31)) << C68K_SR_C_SFT;
		res = (src >> sft) | (src << (32 - sft));
		CPU->flag_V = 0;
		CPU->flag_N = res >> 24;
		CPU->flag_notZ = res;
	*(u32*)(&CPU->D[(Opcode >> 0) & 7]) = res;
	RET3(6)
	}

	CPU->flag_V = 0;
	CPU->flag_C = 0;
	CPU->flag_N = src >> 24;
	CPU->flag_notZ = src;
}
RET(6)

// ASLD
_OP(0xE120)
{
	u32 adr, res;
	u32 src, dst;

	u32 sft;

	sft = CPU->D[(Opcode >> 9) & 7] & 0x3F;
	src = (u8)CPU->D[(Opcode >> 0) & 7];
	if (sft)
	{
	CCnt -= sft * 2;
		if (sft < 8)
		{
			CPU->flag_X = CPU->flag_C = (src << sft) >> 0;
			res = (src << sft) & 0x000000FF;
			CPU->flag_N = res >> 0;
			CPU->flag_notZ = res;
	*(u8*)(&CPU->D[(Opcode >> 0) & 7]) = res;
			CPU->flag_V = 0;
			{
				u32 msk = (((s32)0x80000000) >> (sft + 24)) & 0x000000FF;
				src &= msk;
				if ((src) && (src != msk)) CPU->flag_V = C68K_SR_V;
			}
	RET3(6)
		}

		if (sft == 256) CPU->flag_C = src << C68K_SR_C_SFT;
		else CPU->flag_C = 0;
		CPU->flag_X = CPU->flag_C;
		if (src) CPU->flag_V = C68K_SR_V;
		else CPU->flag_V = 0;
		res = 0;
	*(u8*)(&CPU->D[(Opcode >> 0) & 7]) = res;
		CPU->flag_N = 0;
		CPU->flag_notZ = 0;
	RET3(6)
	}

	CPU->flag_V = 0;
	CPU->flag_C = 0;
	CPU->flag_N = src >> 0;
	CPU->flag_notZ = src;
}
RET(6)

// ASLD
_OP(0xE160)
{
	u32 adr, res;
	u32 src, dst;

	u32 sft;

	sft = CPU->D[(Opcode >> 9) & 7] & 0x3F;
	src = (u16)CPU->D[(Opcode >> 0) & 7];
	if (sft)
	{
	CCnt -= sft * 2;
		if (sft < 16)
		{
			CPU->flag_X = CPU->flag_C = (src << sft) >> 8;
			res = (src << sft) & 0x0000FFFF;
			CPU->flag_N = res >> 8;
			CPU->flag_notZ = res;
	*(u16*)(&CPU->D[(Opcode >> 0) & 7]) = res;
			CPU->flag_V = 0;
			{
				u32 msk = (((s32)0x80000000) >> (sft + 16)) & 0x0000FFFF;
				src &= msk;
				if ((src) && (src != msk)) CPU->flag_V = C68K_SR_V;
			}
	RET3(6)
		}

		if (sft == 65536) CPU->flag_C = src << C68K_SR_C_SFT;
		else CPU->flag_C = 0;
		CPU->flag_X = CPU->flag_C;
		if (src) CPU->flag_V = C68K_SR_V;
		else CPU->flag_V = 0;
		res = 0;
	*(u16*)(&CPU->D[(Opcode >> 0) & 7]) = res;
		CPU->flag_N = 0;
		CPU->flag_notZ = 0;
	RET3(6)
	}

	CPU->flag_V = 0;
	CPU->flag_C = 0;
	CPU->flag_N = src >> 8;
	CPU->flag_notZ = src;
}
RET(6)

// ASLD
_OP(0xE1A0)
{
	u32 adr, res;
	u32 src, dst;

	u32 sft;

	sft = CPU->D[(Opcode >> 9) & 7] & 0x3F;
	src = (u32)CPU->D[(Opcode >> 0) & 7];
	if (sft)
	{
	CCnt -= sft * 2;
		if (sft < 32)
		{
			CPU->flag_X = CPU->flag_C = (src >> (32 - sft)) << C68K_SR_C_SFT;
			res = src << sft;
			CPU->flag_N = res >> 24;
			CPU->flag_notZ = res;
	*(u32*)(&CPU->D[(Opcode >> 0) & 7]) = res;
			CPU->flag_V = 0;
			{
				u32 msk = (((s32)0x80000000) >> (sft + 0)) & 0xFFFFFFFF;
				src &= msk;
				if ((src) && (src != msk)) CPU->flag_V = C68K_SR_V;
			}
	RET3(6)
		}

		if (sft == 0) CPU->flag_C = src << C68K_SR_C_SFT;
		else CPU->flag_C = 0;
		CPU->flag_X = CPU->flag_C;
		if (src) CPU->flag_V = C68K_SR_V;
		else CPU->flag_V = 0;
		res = 0;
	*(u32*)(&CPU->D[(Opcode >> 0) & 7]) = res;
		CPU->flag_N = 0;
		CPU->flag_notZ = 0;
	RET3(6)
	}

	CPU->flag_V = 0;
	CPU->flag_C = 0;
	CPU->flag_N = src >> 24;
	CPU->flag_notZ = src;
}
RET(6)

// LSLD
_OP(0xE128)
{
	u32 adr, res;
	u32 src, dst;

	u32 sft;

	sft = CPU->D[(Opcode >> 9) & 7] & 0x3F;
	src = (u8)CPU->D[(Opcode >> 0) & 7];
	if (sft)
	{
	CCnt -= sft * 2;
		if (sft <= 8)
		{
			CPU->flag_X = CPU->flag_C = (src << sft) >> 0;
			res = (src << sft) & 0x000000FF;
			CPU->flag_V = 0;
			CPU->flag_N = res >> 0;
			CPU->flag_notZ = res;
	*(u8*)(&CPU->D[(Opcode >> 0) & 7]) = res;
	RET3(6)
		}

		CPU->flag_X = CPU->flag_C = 0;
		CPU->flag_N = 0;
		CPU->flag_notZ = 0;
		CPU->flag_V = 0;
		res = 0;
	*(u8*)(&CPU->D[(Opcode >> 0) & 7]) = res;
	RET3(6)
	}

	CPU->flag_V = 0;
	CPU->flag_C = 0;
	CPU->flag_N = src >> 0;
	CPU->flag_notZ = src;
}
RET(6)

// LSLD
_OP(0xE168)
{
	u32 adr, res;
	u32 src, dst;

	u32 sft;

	sft = CPU->D[(Opcode >> 9) & 7] & 0x3F;
	src = (u16)CPU->D[(Opcode >> 0) & 7];
	if (sft)
	{
	CCnt -= sft * 2;
		if (sft <= 16)
		{
			CPU->flag_X = CPU->flag_C = (src << sft) >> 8;
			res = (src << sft) & 0x0000FFFF;
			CPU->flag_V = 0;
			CPU->flag_N = res >> 8;
			CPU->flag_notZ = res;
	*(u16*)(&CPU->D[(Opcode >> 0) & 7]) = res;
	RET3(6)
		}

		CPU->flag_X = CPU->flag_C = 0;
		CPU->flag_N = 0;
		CPU->flag_notZ = 0;
		CPU->flag_V = 0;
		res = 0;
	*(u16*)(&CPU->D[(Opcode >> 0) & 7]) = res;
	RET3(6)
	}

	CPU->flag_V = 0;
	CPU->flag_C = 0;
	CPU->flag_N = src >> 8;
	CPU->flag_notZ = src;
}
RET(6)

// LSLD
_OP(0xE1A8)
{
	u32 adr, res;
	u32 src, dst;

	u32 sft;

	sft = CPU->D[(Opcode >> 9) & 7] & 0x3F;
	src = (u32)CPU->D[(Opcode >> 0) & 7];
	if (sft)
	{
	CCnt -= sft * 2;
		if (sft < 32)
		{
			CPU->flag_X = CPU->flag_C = (src >> (32 - sft)) << C68K_SR_C_SFT;
			res = src << sft;
			CPU->flag_V = 0;
			CPU->flag_N = res >> 24;
			CPU->flag_notZ = res;
	*(u32*)(&CPU->D[(Opcode >> 0) & 7]) = res;
	RET3(6)
		}

		if (sft == 32) CPU->flag_C = src << C68K_SR_C_SFT;
		else CPU->flag_C = 0;
		CPU->flag_X = CPU->flag_C;
		CPU->flag_N = 0;
		CPU->flag_notZ = 0;
		CPU->flag_V = 0;
		res = 0;
	*(u32*)(&CPU->D[(Opcode >> 0) & 7]) = res;
	RET3(6)
	}

	CPU->flag_V = 0;
	CPU->flag_C = 0;
	CPU->flag_N = src >> 24;
	CPU->flag_notZ = src;
}
RET(6)

// ROXLD
_OP(0xE130)
{
	u32 adr, res;
	u32 src, dst;

	u32 sft;

	sft = CPU->D[(Opcode >> 9) & 7] & 0x3F;
	src = (u8)CPU->D[(Opcode >> 0) & 7];
	if (sft)
	{
	CCnt -= sft * 2;
		sft %= 9;

		src |= (CPU->flag_X & C68K_SR_X) << 0;
		res = (src << sft) | (src >> (9 - sft));
		CPU->flag_X = CPU->flag_C = res >> 0;
		CPU->flag_V = 0;
		CPU->flag_N = res >> 0;
		CPU->flag_notZ = res & 0x000000FF;
	*(u8*)(&CPU->D[(Opcode >> 0) & 7]) = res;
	RET3(6)
	}

	CPU->flag_V = 0;
	CPU->flag_C = CPU->flag_X;
	CPU->flag_N = src >> 0;
	CPU->flag_notZ = src;
}
RET(6)

// ROXLD
_OP(0xE170)
{
	u32 adr, res;
	u32 src, dst;

	u32 sft;

	sft = CPU->D[(Opcode >> 9) & 7] & 0x3F;
	src = (u16)CPU->D[(Opcode >> 0) & 7];
	if (sft)
	{
	CCnt -= sft * 2;
		sft %= 17;

		src |= (CPU->flag_X & C68K_SR_X) << 8;
		res = (src << sft) | (src >> (17 - sft));
		CPU->flag_X = CPU->flag_C = res >> 8;
		CPU->flag_V = 0;
		CPU->flag_N = res >> 8;
		CPU->flag_notZ = res & 0x0000FFFF;
	*(u16*)(&CPU->D[(Opcode >> 0) & 7]) = res;
	RET3(6)
	}

	CPU->flag_V = 0;
	CPU->flag_C = CPU->flag_X;
	CPU->flag_N = src >> 8;
	CPU->flag_notZ = src;
}
RET(6)

// ROXLD
_OP(0xE1B0)
{
	u32 adr, res;
	u32 src, dst;

	u32 sft;

	sft = CPU->D[(Opcode >> 9) & 7] & 0x3F;
	src = (u32)CPU->D[(Opcode >> 0) & 7];
	if (sft)
	{
	CCnt -= sft * 2;
		sft %= 33;

		if (sft != 0)
		{
			if (sft == 1) res = (src << 1) | ((CPU->flag_X >> ((C68K_SR_X_SFT + 1) - 1)) & 1);
			else res = (src << sft) | (src >> (33 - sft)) | (((CPU->flag_X >> ((C68K_SR_X_SFT + 1) - 1)) & 1) << (sft - 1));
			CPU->flag_X = (src >> (32 - sft)) << C68K_SR_X_SFT;
		}
		else res = src;
		CPU->flag_C = CPU->flag_X;
		CPU->flag_V = 0;
		CPU->flag_N = res >> 24;
		CPU->flag_notZ = res;
	*(u32*)(&CPU->D[(Opcode >> 0) & 7]) = res;
	RET3(6)
	}

	CPU->flag_V = 0;
	CPU->flag_C = CPU->flag_X;
	CPU->flag_N = src >> 24;
	CPU->flag_notZ = src;
}
RET(6)

// ROLD
_OP(0xE138)
{
	u32 adr, res;
	u32 src, dst;

	u32 sft;

	sft = CPU->D[(Opcode >> 9) & 7] & 0x3F;
	src = (u8)CPU->D[(Opcode >> 0) & 7];
	if (sft)
	{
	CCnt -= sft * 2;
		if (sft &= 0x07)
		{
			CPU->flag_C = (src << sft) >> 0;
			res = ((src << sft) | (src >> (8 - sft))) & 0x000000FF;
			CPU->flag_V = 0;
			CPU->flag_N = res >> 0;
			CPU->flag_notZ = res;
	*(u8*)(&CPU->D[(Opcode >> 0) & 7]) = res;
	RET3(6)
		}

		CPU->flag_V = 0;
		CPU->flag_C = src << C68K_SR_C_SFT;
		CPU->flag_N = src >> 0;
		CPU->flag_notZ = src;
	RET3(6)
	}

	CPU->flag_V = 0;
	CPU->flag_C = 0;
	CPU->flag_N = src >> 0;
	CPU->flag_notZ = src;
}
RET(6)

// ROLD
_OP(0xE178)
{
	u32 adr, res;
	u32 src, dst;

	u32 sft;

	sft = CPU->D[(Opcode >> 9) & 7] & 0x3F;
	src = (u16)CPU->D[(Opcode >> 0) & 7];
	if (sft)
	{
	CCnt -= sft * 2;
		if (sft &= 0x0F)
		{
			CPU->flag_C = (src << sft) >> 8;
			res = ((src << sft) | (src >> (16 - sft))) & 0x0000FFFF;
			CPU->flag_V = 0;
			CPU->flag_N = res >> 8;
			CPU->flag_notZ = res;
	*(u16*)(&CPU->D[(Opcode >> 0) & 7]) = res;
	RET3(6)
		}

		CPU->flag_V = 0;
		CPU->flag_C = src << C68K_SR_C_SFT;
		CPU->flag_N = src >> 8;
		CPU->flag_notZ = src;
	RET3(6)
	}

	CPU->flag_V = 0;
	CPU->flag_C = 0;
	CPU->flag_N = src >> 8;
	CPU->flag_notZ = src;
}
RET(6)

// ROLD
_OP(0xE1B8)
{
	u32 adr, res;
	u32 src, dst;

	u32 sft;

	sft = CPU->D[(Opcode >> 9) & 7] & 0x3F;
	src = (u32)CPU->D[(Opcode >> 0) & 7];
	if (sft)
	{
	CCnt -= sft * 2;
		if (sft &= 0x1F)
		{
			CPU->flag_C = (src >> (32 - sft)) << C68K_SR_C_SFT;
			res = (src << sft) | (src >> (32 - sft));
			CPU->flag_V = 0;
			CPU->flag_N = res >> 24;
			CPU->flag_notZ = res;
	*(u32*)(&CPU->D[(Opcode >> 0) & 7]) = res;
	RET3(6)
		}

		CPU->flag_V = 0;
		CPU->flag_C = src << C68K_SR_C_SFT;
		CPU->flag_N = src >> 24;
		CPU->flag_notZ = src;
	RET3(6)
	}

	CPU->flag_V = 0;
	CPU->flag_C = 0;
	CPU->flag_N = src >> 24;
	CPU->flag_notZ = src;
}
RET(6)

// ASR
_OP(0xE0D0)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7];
	PRE_IO
	READ_WORD_F(adr, src)
	CPU->flag_V = 0;
	CPU->flag_X = CPU->flag_C = src << C68K_SR_C_SFT;
	res = (src >> 1) | (src & (1 << 15));
	CPU->flag_N = res >> 8;
	CPU->flag_notZ = res;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(12)

// ASR
_OP(0xE0D8)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7];
	CPU->A[(Opcode >> 0) & 7] += 2;
	PRE_IO
	READ_WORD_F(adr, src)
	CPU->flag_V = 0;
	CPU->flag_X = CPU->flag_C = src << C68K_SR_C_SFT;
	res = (src >> 1) | (src & (1 << 15));
	CPU->flag_N = res >> 8;
	CPU->flag_notZ = res;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(12)

// ASR
_OP(0xE0E0)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7] - 2;
	CPU->A[(Opcode >> 0) & 7] = adr;
	PRE_IO
	READ_WORD_F(adr, src)
	CPU->flag_V = 0;
	CPU->flag_X = CPU->flag_C = src << C68K_SR_C_SFT;
	res = (src >> 1) | (src & (1 << 15));
	CPU->flag_N = res >> 8;
	CPU->flag_notZ = res;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(14)

// ASR
_OP(0xE0E8)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7] + (s32)(s16)FETCH_WORD;
	PC += 2;
	PRE_IO
	READ_WORD_F(adr, src)
	CPU->flag_V = 0;
	CPU->flag_X = CPU->flag_C = src << C68K_SR_C_SFT;
	res = (src >> 1) | (src & (1 << 15));
	CPU->flag_N = res >> 8;
	CPU->flag_notZ = res;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(16)

// ASR
_OP(0xE0F0)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7];
	DECODE_EXT_WORD
	PRE_IO
	READ_WORD_F(adr, src)
	CPU->flag_V = 0;
	CPU->flag_X = CPU->flag_C = src << C68K_SR_C_SFT;
	res = (src >> 1) | (src & (1 << 15));
	CPU->flag_N = res >> 8;
	CPU->flag_notZ = res;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(18)

// ASR
_OP(0xE0F8)
{
	u32 adr, res;
	u32 src, dst;

	adr = (s32)(s16)FETCH_WORD;
	PC += 2;
	PRE_IO
	READ_WORD_F(adr, src)
	CPU->flag_V = 0;
	CPU->flag_X = CPU->flag_C = src << C68K_SR_C_SFT;
	res = (src >> 1) | (src & (1 << 15));
	CPU->flag_N = res >> 8;
	CPU->flag_notZ = res;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(16)

// ASR
_OP(0xE0F9)
{
	u32 adr, res;
	u32 src, dst;

	adr = (s32)FETCH_LONG;
	PC += 4;
	PRE_IO
	READ_WORD_F(adr, src)
	CPU->flag_V = 0;
	CPU->flag_X = CPU->flag_C = src << C68K_SR_C_SFT;
	res = (src >> 1) | (src & (1 << 15));
	CPU->flag_N = res >> 8;
	CPU->flag_notZ = res;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(20)

// ASR
_OP(0xE0DF)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[7];
	CPU->A[7] += 2;
	PRE_IO
	READ_WORD_F(adr, src)
	CPU->flag_V = 0;
	CPU->flag_X = CPU->flag_C = src << C68K_SR_C_SFT;
	res = (src >> 1) | (src & (1 << 15));
	CPU->flag_N = res >> 8;
	CPU->flag_notZ = res;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(12)

// ASR
_OP(0xE0E7)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[7] - 2;
	CPU->A[7] = adr;
	PRE_IO
	READ_WORD_F(adr, src)
	CPU->flag_V = 0;
	CPU->flag_X = CPU->flag_C = src << C68K_SR_C_SFT;
	res = (src >> 1) | (src & (1 << 15));
	CPU->flag_N = res >> 8;
	CPU->flag_notZ = res;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(14)

// LSR
_OP(0xE2D0)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7];
	PRE_IO
	READ_WORD_F(adr, src)
	CPU->flag_N = CPU->flag_V = 0;
	CPU->flag_X = CPU->flag_C = src << C68K_SR_C_SFT;
	res = src >> 1;
	CPU->flag_notZ = res;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(12)

// LSR
_OP(0xE2D8)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7];
	CPU->A[(Opcode >> 0) & 7] += 2;
	PRE_IO
	READ_WORD_F(adr, src)
	CPU->flag_N = CPU->flag_V = 0;
	CPU->flag_X = CPU->flag_C = src << C68K_SR_C_SFT;
	res = src >> 1;
	CPU->flag_notZ = res;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(12)

// LSR
_OP(0xE2E0)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7] - 2;
	CPU->A[(Opcode >> 0) & 7] = adr;
	PRE_IO
	READ_WORD_F(adr, src)
	CPU->flag_N = CPU->flag_V = 0;
	CPU->flag_X = CPU->flag_C = src << C68K_SR_C_SFT;
	res = src >> 1;
	CPU->flag_notZ = res;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(14)

// LSR
_OP(0xE2E8)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7] + (s32)(s16)FETCH_WORD;
	PC += 2;
	PRE_IO
	READ_WORD_F(adr, src)
	CPU->flag_N = CPU->flag_V = 0;
	CPU->flag_X = CPU->flag_C = src << C68K_SR_C_SFT;
	res = src >> 1;
	CPU->flag_notZ = res;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(16)

// LSR
_OP(0xE2F0)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7];
	DECODE_EXT_WORD
	PRE_IO
	READ_WORD_F(adr, src)
	CPU->flag_N = CPU->flag_V = 0;
	CPU->flag_X = CPU->flag_C = src << C68K_SR_C_SFT;
	res = src >> 1;
	CPU->flag_notZ = res;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(18)

// LSR
_OP(0xE2F8)
{
	u32 adr, res;
	u32 src, dst;

	adr = (s32)(s16)FETCH_WORD;
	PC += 2;
	PRE_IO
	READ_WORD_F(adr, src)
	CPU->flag_N = CPU->flag_V = 0;
	CPU->flag_X = CPU->flag_C = src << C68K_SR_C_SFT;
	res = src >> 1;
	CPU->flag_notZ = res;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(16)

// LSR
_OP(0xE2F9)
{
	u32 adr, res;
	u32 src, dst;

	adr = (s32)FETCH_LONG;
	PC += 4;
	PRE_IO
	READ_WORD_F(adr, src)
	CPU->flag_N = CPU->flag_V = 0;
	CPU->flag_X = CPU->flag_C = src << C68K_SR_C_SFT;
	res = src >> 1;
	CPU->flag_notZ = res;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(20)

// LSR
_OP(0xE2DF)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[7];
	CPU->A[7] += 2;
	PRE_IO
	READ_WORD_F(adr, src)
	CPU->flag_N = CPU->flag_V = 0;
	CPU->flag_X = CPU->flag_C = src << C68K_SR_C_SFT;
	res = src >> 1;
	CPU->flag_notZ = res;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(12)

// LSR
_OP(0xE2E7)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[7] - 2;
	CPU->A[7] = adr;
	PRE_IO
	READ_WORD_F(adr, src)
	CPU->flag_N = CPU->flag_V = 0;
	CPU->flag_X = CPU->flag_C = src << C68K_SR_C_SFT;
	res = src >> 1;
	CPU->flag_notZ = res;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(14)

// ROXR
_OP(0xE4D0)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7];
	PRE_IO
	READ_WORD_F(adr, src)
	CPU->flag_V = 0;
	res = (src >> 1) | ((CPU->flag_X & C68K_SR_X) << 7);
	CPU->flag_C = CPU->flag_X = src << C68K_SR_C_SFT;
	CPU->flag_N = res >> 8;
	CPU->flag_notZ = res;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(12)

// ROXR
_OP(0xE4D8)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7];
	CPU->A[(Opcode >> 0) & 7] += 2;
	PRE_IO
	READ_WORD_F(adr, src)
	CPU->flag_V = 0;
	res = (src >> 1) | ((CPU->flag_X & C68K_SR_X) << 7);
	CPU->flag_C = CPU->flag_X = src << C68K_SR_C_SFT;
	CPU->flag_N = res >> 8;
	CPU->flag_notZ = res;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(12)

// ROXR
_OP(0xE4E0)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7] - 2;
	CPU->A[(Opcode >> 0) & 7] = adr;
	PRE_IO
	READ_WORD_F(adr, src)
	CPU->flag_V = 0;
	res = (src >> 1) | ((CPU->flag_X & C68K_SR_X) << 7);
	CPU->flag_C = CPU->flag_X = src << C68K_SR_C_SFT;
	CPU->flag_N = res >> 8;
	CPU->flag_notZ = res;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(14)

// ROXR
_OP(0xE4E8)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7] + (s32)(s16)FETCH_WORD;
	PC += 2;
	PRE_IO
	READ_WORD_F(adr, src)
	CPU->flag_V = 0;
	res = (src >> 1) | ((CPU->flag_X & C68K_SR_X) << 7);
	CPU->flag_C = CPU->flag_X = src << C68K_SR_C_SFT;
	CPU->flag_N = res >> 8;
	CPU->flag_notZ = res;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(16)

// ROXR
_OP(0xE4F0)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7];
	DECODE_EXT_WORD
	PRE_IO
	READ_WORD_F(adr, src)
	CPU->flag_V = 0;
	res = (src >> 1) | ((CPU->flag_X & C68K_SR_X) << 7);
	CPU->flag_C = CPU->flag_X = src << C68K_SR_C_SFT;
	CPU->flag_N = res >> 8;
	CPU->flag_notZ = res;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(18)

// ROXR
_OP(0xE4F8)
{
	u32 adr, res;
	u32 src, dst;

	adr = (s32)(s16)FETCH_WORD;
	PC += 2;
	PRE_IO
	READ_WORD_F(adr, src)
	CPU->flag_V = 0;
	res = (src >> 1) | ((CPU->flag_X & C68K_SR_X) << 7);
	CPU->flag_C = CPU->flag_X = src << C68K_SR_C_SFT;
	CPU->flag_N = res >> 8;
	CPU->flag_notZ = res;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(16)

// ROXR
_OP(0xE4F9)
{
	u32 adr, res;
	u32 src, dst;

	adr = (s32)FETCH_LONG;
	PC += 4;
	PRE_IO
	READ_WORD_F(adr, src)
	CPU->flag_V = 0;
	res = (src >> 1) | ((CPU->flag_X & C68K_SR_X) << 7);
	CPU->flag_C = CPU->flag_X = src << C68K_SR_C_SFT;
	CPU->flag_N = res >> 8;
	CPU->flag_notZ = res;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(20)

// ROXR
_OP(0xE4DF)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[7];
	CPU->A[7] += 2;
	PRE_IO
	READ_WORD_F(adr, src)
	CPU->flag_V = 0;
	res = (src >> 1) | ((CPU->flag_X & C68K_SR_X) << 7);
	CPU->flag_C = CPU->flag_X = src << C68K_SR_C_SFT;
	CPU->flag_N = res >> 8;
	CPU->flag_notZ = res;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(12)

// ROXR
_OP(0xE4E7)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[7] - 2;
	CPU->A[7] = adr;
	PRE_IO
	READ_WORD_F(adr, src)
	CPU->flag_V = 0;
	res = (src >> 1) | ((CPU->flag_X & C68K_SR_X) << 7);
	CPU->flag_C = CPU->flag_X = src << C68K_SR_C_SFT;
	CPU->flag_N = res >> 8;
	CPU->flag_notZ = res;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(14)

// ROR
_OP(0xE6D0)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7];
	PRE_IO
	READ_WORD_F(adr, src)
	CPU->flag_V = 0;
	CPU->flag_C = src << C68K_SR_C_SFT;
	res = (src >> 1) | (src << 15);
	CPU->flag_N = res >> 8;
	CPU->flag_notZ = res & 0x0000FFFF;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(12)

// ROR
_OP(0xE6D8)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7];
	CPU->A[(Opcode >> 0) & 7] += 2;
	PRE_IO
	READ_WORD_F(adr, src)
	CPU->flag_V = 0;
	CPU->flag_C = src << C68K_SR_C_SFT;
	res = (src >> 1) | (src << 15);
	CPU->flag_N = res >> 8;
	CPU->flag_notZ = res & 0x0000FFFF;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(12)

// ROR
_OP(0xE6E0)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7] - 2;
	CPU->A[(Opcode >> 0) & 7] = adr;
	PRE_IO
	READ_WORD_F(adr, src)
	CPU->flag_V = 0;
	CPU->flag_C = src << C68K_SR_C_SFT;
	res = (src >> 1) | (src << 15);
	CPU->flag_N = res >> 8;
	CPU->flag_notZ = res & 0x0000FFFF;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(14)

// ROR
_OP(0xE6E8)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7] + (s32)(s16)FETCH_WORD;
	PC += 2;
	PRE_IO
	READ_WORD_F(adr, src)
	CPU->flag_V = 0;
	CPU->flag_C = src << C68K_SR_C_SFT;
	res = (src >> 1) | (src << 15);
	CPU->flag_N = res >> 8;
	CPU->flag_notZ = res & 0x0000FFFF;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(16)

// ROR
_OP(0xE6F0)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7];
	DECODE_EXT_WORD
	PRE_IO
	READ_WORD_F(adr, src)
	CPU->flag_V = 0;
	CPU->flag_C = src << C68K_SR_C_SFT;
	res = (src >> 1) | (src << 15);
	CPU->flag_N = res >> 8;
	CPU->flag_notZ = res & 0x0000FFFF;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(18)

// ROR
_OP(0xE6F8)
{
	u32 adr, res;
	u32 src, dst;

	adr = (s32)(s16)FETCH_WORD;
	PC += 2;
	PRE_IO
	READ_WORD_F(adr, src)
	CPU->flag_V = 0;
	CPU->flag_C = src << C68K_SR_C_SFT;
	res = (src >> 1) | (src << 15);
	CPU->flag_N = res >> 8;
	CPU->flag_notZ = res & 0x0000FFFF;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(16)

// ROR
_OP(0xE6F9)
{
	u32 adr, res;
	u32 src, dst;

	adr = (s32)FETCH_LONG;
	PC += 4;
	PRE_IO
	READ_WORD_F(adr, src)
	CPU->flag_V = 0;
	CPU->flag_C = src << C68K_SR_C_SFT;
	res = (src >> 1) | (src << 15);
	CPU->flag_N = res >> 8;
	CPU->flag_notZ = res & 0x0000FFFF;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(20)

// ROR
_OP(0xE6DF)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[7];
	CPU->A[7] += 2;
	PRE_IO
	READ_WORD_F(adr, src)
	CPU->flag_V = 0;
	CPU->flag_C = src << C68K_SR_C_SFT;
	res = (src >> 1) | (src << 15);
	CPU->flag_N = res >> 8;
	CPU->flag_notZ = res & 0x0000FFFF;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(12)

// ROR
_OP(0xE6E7)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[7] - 2;
	CPU->A[7] = adr;
	PRE_IO
	READ_WORD_F(adr, src)
	CPU->flag_V = 0;
	CPU->flag_C = src << C68K_SR_C_SFT;
	res = (src >> 1) | (src << 15);
	CPU->flag_N = res >> 8;
	CPU->flag_notZ = res & 0x0000FFFF;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(14)

// ASL
_OP(0xE1D0)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7];
	PRE_IO
	READ_WORD_F(adr, src)
	CPU->flag_X = CPU->flag_C = src >> 7;
	res = src << 1;
	CPU->flag_V = (src ^ res) >> 8;
	CPU->flag_N = res >> 8;
	CPU->flag_notZ = res & 0x0000FFFF;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(12)

// ASL
_OP(0xE1D8)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7];
	CPU->A[(Opcode >> 0) & 7] += 2;
	PRE_IO
	READ_WORD_F(adr, src)
	CPU->flag_X = CPU->flag_C = src >> 7;
	res = src << 1;
	CPU->flag_V = (src ^ res) >> 8;
	CPU->flag_N = res >> 8;
	CPU->flag_notZ = res & 0x0000FFFF;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(12)

// ASL
_OP(0xE1E0)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7] - 2;
	CPU->A[(Opcode >> 0) & 7] = adr;
	PRE_IO
	READ_WORD_F(adr, src)
	CPU->flag_X = CPU->flag_C = src >> 7;
	res = src << 1;
	CPU->flag_V = (src ^ res) >> 8;
	CPU->flag_N = res >> 8;
	CPU->flag_notZ = res & 0x0000FFFF;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(14)

// ASL
_OP(0xE1E8)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7] + (s32)(s16)FETCH_WORD;
	PC += 2;
	PRE_IO
	READ_WORD_F(adr, src)
	CPU->flag_X = CPU->flag_C = src >> 7;
	res = src << 1;
	CPU->flag_V = (src ^ res) >> 8;
	CPU->flag_N = res >> 8;
	CPU->flag_notZ = res & 0x0000FFFF;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(16)

// ASL
_OP(0xE1F0)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7];
	DECODE_EXT_WORD
	PRE_IO
	READ_WORD_F(adr, src)
	CPU->flag_X = CPU->flag_C = src >> 7;
	res = src << 1;
	CPU->flag_V = (src ^ res) >> 8;
	CPU->flag_N = res >> 8;
	CPU->flag_notZ = res & 0x0000FFFF;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(18)

// ASL
_OP(0xE1F8)
{
	u32 adr, res;
	u32 src, dst;

	adr = (s32)(s16)FETCH_WORD;
	PC += 2;
	PRE_IO
	READ_WORD_F(adr, src)
	CPU->flag_X = CPU->flag_C = src >> 7;
	res = src << 1;
	CPU->flag_V = (src ^ res) >> 8;
	CPU->flag_N = res >> 8;
	CPU->flag_notZ = res & 0x0000FFFF;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(16)

// ASL
_OP(0xE1F9)
{
	u32 adr, res;
	u32 src, dst;

	adr = (s32)FETCH_LONG;
	PC += 4;
	PRE_IO
	READ_WORD_F(adr, src)
	CPU->flag_X = CPU->flag_C = src >> 7;
	res = src << 1;
	CPU->flag_V = (src ^ res) >> 8;
	CPU->flag_N = res >> 8;
	CPU->flag_notZ = res & 0x0000FFFF;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(20)

// ASL
_OP(0xE1DF)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[7];
	CPU->A[7] += 2;
	PRE_IO
	READ_WORD_F(adr, src)
	CPU->flag_X = CPU->flag_C = src >> 7;
	res = src << 1;
	CPU->flag_V = (src ^ res) >> 8;
	CPU->flag_N = res >> 8;
	CPU->flag_notZ = res & 0x0000FFFF;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(12)

// ASL
_OP(0xE1E7)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[7] - 2;
	CPU->A[7] = adr;
	PRE_IO
	READ_WORD_F(adr, src)
	CPU->flag_X = CPU->flag_C = src >> 7;
	res = src << 1;
	CPU->flag_V = (src ^ res) >> 8;
	CPU->flag_N = res >> 8;
	CPU->flag_notZ = res & 0x0000FFFF;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(14)

// LSL
_OP(0xE3D0)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7];
	PRE_IO
	READ_WORD_F(adr, src)
	CPU->flag_V = 0;
	CPU->flag_X = CPU->flag_C = src >> 7;
	res = src << 1;
	CPU->flag_N = res >> 8;
	CPU->flag_notZ = res & 0x0000FFFF;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(12)

// LSL
_OP(0xE3D8)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7];
	CPU->A[(Opcode >> 0) & 7] += 2;
	PRE_IO
	READ_WORD_F(adr, src)
	CPU->flag_V = 0;
	CPU->flag_X = CPU->flag_C = src >> 7;
	res = src << 1;
	CPU->flag_N = res >> 8;
	CPU->flag_notZ = res & 0x0000FFFF;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(12)

// LSL
_OP(0xE3E0)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7] - 2;
	CPU->A[(Opcode >> 0) & 7] = adr;
	PRE_IO
	READ_WORD_F(adr, src)
	CPU->flag_V = 0;
	CPU->flag_X = CPU->flag_C = src >> 7;
	res = src << 1;
	CPU->flag_N = res >> 8;
	CPU->flag_notZ = res & 0x0000FFFF;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(14)

// LSL
_OP(0xE3E8)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7] + (s32)(s16)FETCH_WORD;
	PC += 2;
	PRE_IO
	READ_WORD_F(adr, src)
	CPU->flag_V = 0;
	CPU->flag_X = CPU->flag_C = src >> 7;
	res = src << 1;
	CPU->flag_N = res >> 8;
	CPU->flag_notZ = res & 0x0000FFFF;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(16)

// LSL
_OP(0xE3F0)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7];
	DECODE_EXT_WORD
	PRE_IO
	READ_WORD_F(adr, src)
	CPU->flag_V = 0;
	CPU->flag_X = CPU->flag_C = src >> 7;
	res = src << 1;
	CPU->flag_N = res >> 8;
	CPU->flag_notZ = res & 0x0000FFFF;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(18)

// LSL
_OP(0xE3F8)
{
	u32 adr, res;
	u32 src, dst;

	adr = (s32)(s16)FETCH_WORD;
	PC += 2;
	PRE_IO
	READ_WORD_F(adr, src)
	CPU->flag_V = 0;
	CPU->flag_X = CPU->flag_C = src >> 7;
	res = src << 1;
	CPU->flag_N = res >> 8;
	CPU->flag_notZ = res & 0x0000FFFF;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(16)

// LSL
_OP(0xE3F9)
{
	u32 adr, res;
	u32 src, dst;

	adr = (s32)FETCH_LONG;
	PC += 4;
	PRE_IO
	READ_WORD_F(adr, src)
	CPU->flag_V = 0;
	CPU->flag_X = CPU->flag_C = src >> 7;
	res = src << 1;
	CPU->flag_N = res >> 8;
	CPU->flag_notZ = res & 0x0000FFFF;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(20)

// LSL
_OP(0xE3DF)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[7];
	CPU->A[7] += 2;
	PRE_IO
	READ_WORD_F(adr, src)
	CPU->flag_V = 0;
	CPU->flag_X = CPU->flag_C = src >> 7;
	res = src << 1;
	CPU->flag_N = res >> 8;
	CPU->flag_notZ = res & 0x0000FFFF;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(12)

// LSL
_OP(0xE3E7)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[7] - 2;
	CPU->A[7] = adr;
	PRE_IO
	READ_WORD_F(adr, src)
	CPU->flag_V = 0;
	CPU->flag_X = CPU->flag_C = src >> 7;
	res = src << 1;
	CPU->flag_N = res >> 8;
	CPU->flag_notZ = res & 0x0000FFFF;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(14)

// ROXL
_OP(0xE5D0)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7];
	PRE_IO
	READ_WORD_F(adr, src)
	CPU->flag_V = 0;
	res = (src << 1) | ((CPU->flag_X & C68K_SR_X) >> 8);
	CPU->flag_X = CPU->flag_C = src >> 7;
	CPU->flag_N = res >> 8;
	CPU->flag_notZ = res & 0x0000FFFF;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(12)

// ROXL
_OP(0xE5D8)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7];
	CPU->A[(Opcode >> 0) & 7] += 2;
	PRE_IO
	READ_WORD_F(adr, src)
	CPU->flag_V = 0;
	res = (src << 1) | ((CPU->flag_X & C68K_SR_X) >> 8);
	CPU->flag_X = CPU->flag_C = src >> 7;
	CPU->flag_N = res >> 8;
	CPU->flag_notZ = res & 0x0000FFFF;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(12)

// ROXL
_OP(0xE5E0)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7] - 2;
	CPU->A[(Opcode >> 0) & 7] = adr;
	PRE_IO
	READ_WORD_F(adr, src)
	CPU->flag_V = 0;
	res = (src << 1) | ((CPU->flag_X & C68K_SR_X) >> 8);
	CPU->flag_X = CPU->flag_C = src >> 7;
	CPU->flag_N = res >> 8;
	CPU->flag_notZ = res & 0x0000FFFF;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(14)

// ROXL
_OP(0xE5E8)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7] + (s32)(s16)FETCH_WORD;
	PC += 2;
	PRE_IO
	READ_WORD_F(adr, src)
	CPU->flag_V = 0;
	res = (src << 1) | ((CPU->flag_X & C68K_SR_X) >> 8);
	CPU->flag_X = CPU->flag_C = src >> 7;
	CPU->flag_N = res >> 8;
	CPU->flag_notZ = res & 0x0000FFFF;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(16)

// ROXL
_OP(0xE5F0)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7];
	DECODE_EXT_WORD
	PRE_IO
	READ_WORD_F(adr, src)
	CPU->flag_V = 0;
	res = (src << 1) | ((CPU->flag_X & C68K_SR_X) >> 8);
	CPU->flag_X = CPU->flag_C = src >> 7;
	CPU->flag_N = res >> 8;
	CPU->flag_notZ = res & 0x0000FFFF;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(18)

// ROXL
_OP(0xE5F8)
{
	u32 adr, res;
	u32 src, dst;

	adr = (s32)(s16)FETCH_WORD;
	PC += 2;
	PRE_IO
	READ_WORD_F(adr, src)
	CPU->flag_V = 0;
	res = (src << 1) | ((CPU->flag_X & C68K_SR_X) >> 8);
	CPU->flag_X = CPU->flag_C = src >> 7;
	CPU->flag_N = res >> 8;
	CPU->flag_notZ = res & 0x0000FFFF;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(16)

// ROXL
_OP(0xE5F9)
{
	u32 adr, res;
	u32 src, dst;

	adr = (s32)FETCH_LONG;
	PC += 4;
	PRE_IO
	READ_WORD_F(adr, src)
	CPU->flag_V = 0;
	res = (src << 1) | ((CPU->flag_X & C68K_SR_X) >> 8);
	CPU->flag_X = CPU->flag_C = src >> 7;
	CPU->flag_N = res >> 8;
	CPU->flag_notZ = res & 0x0000FFFF;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(20)

// ROXL
_OP(0xE5DF)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[7];
	CPU->A[7] += 2;
	PRE_IO
	READ_WORD_F(adr, src)
	CPU->flag_V = 0;
	res = (src << 1) | ((CPU->flag_X & C68K_SR_X) >> 8);
	CPU->flag_X = CPU->flag_C = src >> 7;
	CPU->flag_N = res >> 8;
	CPU->flag_notZ = res & 0x0000FFFF;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(12)

// ROXL
_OP(0xE5E7)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[7] - 2;
	CPU->A[7] = adr;
	PRE_IO
	READ_WORD_F(adr, src)
	CPU->flag_V = 0;
	res = (src << 1) | ((CPU->flag_X & C68K_SR_X) >> 8);
	CPU->flag_X = CPU->flag_C = src >> 7;
	CPU->flag_N = res >> 8;
	CPU->flag_notZ = res & 0x0000FFFF;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(14)

// ROL
_OP(0xE7D0)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7];
	PRE_IO
	READ_WORD_F(adr, src)
	CPU->flag_V = 0;
	CPU->flag_C = src >> 7;
	res = (src << 1) | (src >> 15);
	CPU->flag_N = res >> 8;
	CPU->flag_notZ = res & 0x0000FFFF;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(12)

// ROL
_OP(0xE7D8)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7];
	CPU->A[(Opcode >> 0) & 7] += 2;
	PRE_IO
	READ_WORD_F(adr, src)
	CPU->flag_V = 0;
	CPU->flag_C = src >> 7;
	res = (src << 1) | (src >> 15);
	CPU->flag_N = res >> 8;
	CPU->flag_notZ = res & 0x0000FFFF;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(12)

// ROL
_OP(0xE7E0)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7] - 2;
	CPU->A[(Opcode >> 0) & 7] = adr;
	PRE_IO
	READ_WORD_F(adr, src)
	CPU->flag_V = 0;
	CPU->flag_C = src >> 7;
	res = (src << 1) | (src >> 15);
	CPU->flag_N = res >> 8;
	CPU->flag_notZ = res & 0x0000FFFF;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(14)

// ROL
_OP(0xE7E8)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7] + (s32)(s16)FETCH_WORD;
	PC += 2;
	PRE_IO
	READ_WORD_F(adr, src)
	CPU->flag_V = 0;
	CPU->flag_C = src >> 7;
	res = (src << 1) | (src >> 15);
	CPU->flag_N = res >> 8;
	CPU->flag_notZ = res & 0x0000FFFF;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(16)

// ROL
_OP(0xE7F0)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[(Opcode >> 0) & 7];
	DECODE_EXT_WORD
	PRE_IO
	READ_WORD_F(adr, src)
	CPU->flag_V = 0;
	CPU->flag_C = src >> 7;
	res = (src << 1) | (src >> 15);
	CPU->flag_N = res >> 8;
	CPU->flag_notZ = res & 0x0000FFFF;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(18)

// ROL
_OP(0xE7F8)
{
	u32 adr, res;
	u32 src, dst;

	adr = (s32)(s16)FETCH_WORD;
	PC += 2;
	PRE_IO
	READ_WORD_F(adr, src)
	CPU->flag_V = 0;
	CPU->flag_C = src >> 7;
	res = (src << 1) | (src >> 15);
	CPU->flag_N = res >> 8;
	CPU->flag_notZ = res & 0x0000FFFF;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(16)

// ROL
_OP(0xE7F9)
{
	u32 adr, res;
	u32 src, dst;

	adr = (s32)FETCH_LONG;
	PC += 4;
	PRE_IO
	READ_WORD_F(adr, src)
	CPU->flag_V = 0;
	CPU->flag_C = src >> 7;
	res = (src << 1) | (src >> 15);
	CPU->flag_N = res >> 8;
	CPU->flag_notZ = res & 0x0000FFFF;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(20)

// ROL
_OP(0xE7DF)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[7];
	CPU->A[7] += 2;
	PRE_IO
	READ_WORD_F(adr, src)
	CPU->flag_V = 0;
	CPU->flag_C = src >> 7;
	res = (src << 1) | (src >> 15);
	CPU->flag_N = res >> 8;
	CPU->flag_notZ = res & 0x0000FFFF;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(12)

// ROL
_OP(0xE7E7)
{
	u32 adr, res;
	u32 src, dst;

	adr = CPU->A[7] - 2;
	CPU->A[7] = adr;
	PRE_IO
	READ_WORD_F(adr, src)
	CPU->flag_V = 0;
	CPU->flag_C = src >> 7;
	res = (src << 1) | (src >> 15);
	CPU->flag_N = res >> 8;
	CPU->flag_notZ = res & 0x0000FFFF;
	WRITE_WORD_F(adr, res)
	POST_IO
}
RET(14)
