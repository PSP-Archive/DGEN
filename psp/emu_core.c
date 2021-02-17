///////////////////////////////////////////////////////////////////////////
/// emu_core.c
///////////////////////////////////////////////////////////////////////////
#include "emu_core.h"
///////////////////////////////////////////////////////////////////////////

//unsigned char    sEmuCoreBuff[ MAX_EMU_CORE_BUFF ] __attribute__((aligned(64))) ;
int              emu_mode            = EMU_MD      ;
EMU_CONFIG       emu_config[MAX_EMU]               ;
EMU_STATE        emu_state                         ;
EMU_CHEAT        emu_cheat                         ;
int              emu_buff_id = -1                   ;

///////////////////////////////////////////////////////////////////////////

unsigned char* EmuCore_GetBuffer( unsigned long size )
{
	if( emu_buff_id < 0 )
	{
		{
			int num = 0 ;
			while( sceKernelMaxFreeMemSize() < size )
			{
				sceKernelFreePartitionMemory( num ) ;
				num++ ;
				if( num > 1000 ){ break ; }
			}
		}

		int buff_id = sceKernelAllocPartitionMemory( 2, "main_buff", 0, size, NULL ) ;

		if( buff_id < 0 ){ return 0 ; }

		emu_buff_id = buff_id ;
	}

	unsigned long buff = (unsigned long)sceKernelGetBlockHeadAddr( emu_buff_id ) ;

	buff &= 0xFFFFFF40 ; // aligned 64bit
	//buff |= 0x40000000 ; // uncashed

	return (unsigned char*)buff ;
}


int EmuCore_IsSaveSRAM ()
{
	if( emu_mode == EMU_MD )
	{
		return md_is_save_sram() ;
	}

	return 0 ;
}

int EmuCore_SaveSRAM( const char* path )
{
	if( emu_mode == EMU_MD )
	{
		return md_save_sram( path ) ;
	}

	return 0 ;
}

///////////////////////////////////////////////////////////////////////////
