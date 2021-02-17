///////////////////////////////////////////////////////////////////////////
/// emu_cheat.c
///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
#include "emu_cheat.h"
#include "emu_core.h"
#include "psp_cfg_file.h"
///////////////////////////////////////////////////////////////////////////
#define EMU_CHEAT_FILEVER  (1)
const char SZ_CHEATFILE_TITLE   [] = "[DGEN for PSP CheatFile]" ;
const char SZ_CHEAT_KEY_VERSION [] = "VERSION=" ;
const char SZ_CHEAT_KEY_ALL_ON  [] = "ALL_ON =" ;
      char SZ_CHEAT_KEY_FLAG    [] = "FLAG_00=" ;
      char SZ_CHEAT_KEY_ADDR    [] = "ADDR_00=" ;
      char SZ_CHEAT_KEY_DATA    [] = "DATA_00=" ;
      char SZ_CHEAT_KEY_CODE    [] = "CODE_00=" ;
      char SZ_CHEAT_KEY_NAME    [] = "NAME_00=" ;
///////////////////////////////////////////////////////////////////////////

int  EmuCheat_Init( EMU_CHEAT* emct )
{
	int n ;

	emct->all_on     = 0 ;
	emct->all_on_old = 0 ;

	for( n=0; n < MAX_CHEAT_NUM ; n++ )
	{
		emct->flag[n] = 0              ;
		emct->addr[n] = MIN_CHEAT_ADDR ;
		emct->data[n] = 0              ;

		memset( emct->name[n], ' ', MAX_CHEAT_NAME ) ;
		emct->name[n][MAX_CHEAT_NAME] = 0 ;
	}

	return 0 ;
}

void EmuCheat_CheckValue( EMU_CHEAT* emct )
{
	int n ;

	for( n=0; n < MAX_CHEAT_NUM ; n++ )
	{
		if(      emct->addr[n] < MIN_CHEAT_ADDR ){ emct->addr[n] = MIN_CHEAT_ADDR ; }
		else if( emct->addr[n] > MAX_CHEAT_ADDR ){ emct->addr[n] = MAX_CHEAT_ADDR ; }
	}
}

const char genie_chars[] = "AaBbCcDdEeFfGgHhJjKkLlMmNnPpRrSsTtVvWwXxYyZz0O1I2233445566778899" ;

int  EmuCheat_CalcGG( const char* code_p, unsigned long* addr_p, unsigned long* data_p )
{
	int   i ;
	char* x;
	char  code[9];
	unsigned long n, addr, data ;

	memcpy( &code[0], &code_p[0], 4 ) ;
	memcpy( &code[4], &code_p[5], 4 ) ;
	code[8] = 0 ;

	addr = data = 0 ;

	for( i=0; i < 8 ; i++ )
	{
		/* If strchr returns NULL, we were given a bad character */
		if( !(x = strchr(genie_chars, code[i])) ){ return 0; }
		n = (unsigned long)(x - genie_chars) >> 1;

		/* Now, based on which character this is, fit it into the result */
		switch(i)
		{
		case 0:
			/* ____ ____ ____ ____ ____ ____ : ____ ____ ABCD E___ */
			data |= n << 3;
			break;
		case 1:
			/* ____ ____ DE__ ____ ____ ____ : ____ ____ ____ _ABC */
			data |= n >> 2;
			addr |= (n & 3) << 14;
			break;
		case 2:
			/* ____ ____ __AB CDE_ ____ ____ : ____ ____ ____ ____ */
			addr |= n << 9;
			break;
		case 3:
			/* BCDE ____ ____ ___A ____ ____ : ____ ____ ____ ____ */
			addr |= (n & 0xF) << 20 | (n >> 4) << 8;
			break;
		case 4:
			/* ____ ABCD ____ ____ ____ ____ : ___E ____ ____ ____ */
			data |= (n & 1) << 12;
			addr |= (n >> 1) << 16;
			break;
		case 5:
			/* ____ ____ ____ ____ ____ ____ : E___ ABCD ____ ____ */
			data |= (n & 1) << 15 | (n >> 1) << 8;
			break;
		case 6:
			/* ____ ____ ____ ____ CDE_ ____ : _AB_ ____ ____ ____ */
			data |= (n >> 3) << 13;
			addr |= (n & 7) << 5;
			break;
		case 7:
			/* ____ ____ ____ ____ ___A BCDE : ____ ____ ____ ____ */
			addr |= n;
			break;
		}
		/* Go around again */
	}

	if(      addr < MIN_CHEAT_ADDR ){ return 0 ; }
	else if( addr > MAX_CHEAT_ADDR ){ return 0 ; }

	*addr_p = addr ;
	*data_p = data ;

	return 1;
}

int  EmuCheat_Load( EMU_CHEAT* emct, const char* path )
{
	int n, pos, ver ; 
	char num[2] ;

	EmuCheat_Init( emct ) ;

	if( PSP_CfgFile_Read( path ) )
	{
		pos = PSP_CfgFile_FindKey( SZ_CHEATFILE_TITLE ) ;
		if( pos < 0 ){ return 0 ; }

		pos = PSP_CfgFile_FindKey( SZ_CHEAT_KEY_VERSION ) ;
		if( pos > 0 ){ ver = PSP_CfgFile_GetInt( pos ) ; }

		pos = PSP_CfgFile_FindKey( SZ_CHEAT_KEY_VERSION ) ;
		if( pos > 0 ){ ver = PSP_CfgFile_GetInt( pos ) ; }

		pos = PSP_CfgFile_FindKey( SZ_CHEAT_KEY_ALL_ON ) ;
		if( pos > 0 ){ emct->all_on = PSP_CfgFile_GetInt( pos ) ; }

		for( n=0; n < MAX_CHEAT_NUM ; n++ )
		{
			num[0] = '0' + (n+1) / 10 ;
			num[1] = '0' + (n+1) % 10 ;
			SZ_CHEAT_KEY_FLAG[5] = num[0] ; SZ_CHEAT_KEY_FLAG[6] = num[1] ;
			SZ_CHEAT_KEY_ADDR[5] = num[0] ; SZ_CHEAT_KEY_ADDR[6] = num[1] ;
			SZ_CHEAT_KEY_DATA[5] = num[0] ; SZ_CHEAT_KEY_DATA[6] = num[1] ;
			SZ_CHEAT_KEY_CODE[5] = num[0] ; SZ_CHEAT_KEY_CODE[6] = num[1] ;
			SZ_CHEAT_KEY_NAME[5] = num[0] ; SZ_CHEAT_KEY_NAME[6] = num[1] ;

			pos = PSP_CfgFile_FindKey( SZ_CHEAT_KEY_FLAG ) ;
			if( pos > 0 ){ emct->flag[n] = PSP_CfgFile_GetInt( pos ) ; }

			pos = PSP_CfgFile_FindKey( SZ_CHEAT_KEY_ADDR ) ;
			if( pos > 0 ){ emct->addr[n] = PSP_CfgFile_GetHex( pos ) ; }

			pos = PSP_CfgFile_FindKey( SZ_CHEAT_KEY_DATA ) ;
			if( pos > 0 ){ emct->data[n] = PSP_CfgFile_GetHex( pos ) ; }

			pos = PSP_CfgFile_FindKey( SZ_CHEAT_KEY_CODE ) ;
			if( pos > 0 ){ PSP_CfgFile_GetStr( pos, emct->code[n], MAX_CHEAT_GG   ) ; }

			pos = PSP_CfgFile_FindKey( SZ_CHEAT_KEY_NAME ) ;
			if( pos > 0 ){ PSP_CfgFile_GetStr( pos, emct->name[n], MAX_CHEAT_NAME ) ; }
		}

		EmuCheat_CheckValue( emct ) ;

		//emct->all_on_old = emct->all_on ;
		emct->all_on     = 0            ;
		emct->modifyed   = 0            ;

		return 1 ;
	}

	return 0 ;
}

int  EmuCheat_Save( EMU_CHEAT* emct, const char* path )
{
	int n, pos ;
	char num[2] ;

	if( emct->modifyed ) //|| (emct->all_on != emct->all_on_old) )
	{
		PSP_CfgFile_Init    ( SZ_CHEATFILE_TITLE ) ;
		PSP_CfgFile_MakeInt ( SZ_CHEAT_KEY_VERSION , EMU_CHEAT_FILEVER ) ;
		PSP_CfgFile_MakeInt ( SZ_CHEAT_KEY_ALL_ON  , emct->all_on      ) ;

		for( n=0; n < MAX_CHEAT_NUM ; n++ )
		{
			num[0] = '0' + (n+1) / 10 ;
			num[1] = '0' + (n+1) % 10 ;
			SZ_CHEAT_KEY_FLAG[5] = num[0] ; SZ_CHEAT_KEY_FLAG[6] = num[1] ;
			SZ_CHEAT_KEY_ADDR[5] = num[0] ; SZ_CHEAT_KEY_ADDR[6] = num[1] ;
			SZ_CHEAT_KEY_DATA[5] = num[0] ; SZ_CHEAT_KEY_DATA[6] = num[1] ;
			SZ_CHEAT_KEY_CODE[5] = num[0] ; SZ_CHEAT_KEY_CODE[6] = num[1] ;
			SZ_CHEAT_KEY_NAME[5] = num[0] ; SZ_CHEAT_KEY_NAME[6] = num[1] ;

			PSP_CfgFile_MakeInt ( SZ_CHEAT_KEY_FLAG , emct->flag[n] ) ;
			PSP_CfgFile_MakeHex ( SZ_CHEAT_KEY_ADDR , emct->addr[n] ) ;
			PSP_CfgFile_MakeHex ( SZ_CHEAT_KEY_DATA , emct->data[n] ) ;
			PSP_CfgFile_MakeStr ( SZ_CHEAT_KEY_CODE , emct->code[n] ) ;
			PSP_CfgFile_MakeStr ( SZ_CHEAT_KEY_NAME , emct->name[n] ) ;
		}

		if( PSP_CfgFile_Write( path ) )
		{
			emct->all_on_old = emct->all_on ;
			emct->modifyed   = 0            ;

			return 1 ;
		}
	}

	return 0 ;
}

void EmuCheat_Exec( EMU_CHEAT* emct )
{
	int n ;

	if( emct->all_on )
	{
		for( n=0; n < MAX_CHEAT_NUM ; n++ )
		{
			if( emct->flag[n] )
			{
				md_cheat_exec( emct->addr[n], emct->data[n] ) ;
			}
		}
	}
}

///////////////////////////////////////////////////////////////////////////
