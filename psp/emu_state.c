///////////////////////////////////////////////////////////////////////////
/// emu_state.c
///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
#include "emu_core.h"
#include "psp_cfg_file.h"
#include "psp_std.h"
#include <stdio.h>

#ifdef  _USE_ZLIB
#include "../lib/zlib.h"
#endif //_USE_ZLIB
///////////////////////////////////////////////////////////////////////////
#define DEF_STATE_CFG  "STATE/DEFAULT.INI"
#define EXT_STATE_CFG  ".INI"
#define EMU_STATE_FILEVER  (1)
const char SZ_STATE_TITLE         [] = "[DGEN for PSP State Config]" ;
const char SZ_STATE_KEY_VERSION   [] = "VERSION =" ;
const char SZ_STATE_KEY_INCREMENT [] = "INCMODE =" ;
const char SZ_STATE_KEY_SAVE_M1   [] = "SAVE_M1 =" ;
const char SZ_STATE_KEY_SAVE_M2   [] = "SAVE_M2 =" ;
const char SZ_STATE_KEY_SLOT_SEL  [] = "SLOT_SL =" ;
const char SZ_STATE_KEY_SLOT_INC  [] = "SLOT_IN =" ;
#define EMU_STATE_SLOTVER  (3)
///////////////////////////////////////////////////////////////////////////
char emu_state_path[ 128 ] ;
char emu_state_ext [] = ".st0" ;

unsigned short state_temp_image[ STATE_IMAGE_CY ][ STATE_IMAGE_CX ] ;

#ifdef WIN32
unsigned char buff_zlib[ sizeof(EMU_STATE_SLOT) * 2 ] ;
#else //WIN32
unsigned char buff_zlib[ sizeof(EMU_STATE_SLOT) * 2 ] __attribute__((aligned(32))) ;
#endif //WIN32

EMU_STATE_CFG  emu_state_cfg_def ;

int EmuStateCfg_Load ( EMU_STATE_CFG* esc, const char* path )
{
	int  pos, ver    ;

	if( PSP_CfgFile_Read( path ) )
	{
		pos = PSP_CfgFile_FindKey( SZ_STATE_TITLE ) ;
		if( pos < 0 ){ return 0 ; }

		pos = PSP_CfgFile_FindKey( SZ_STATE_KEY_VERSION ) ;
		if( pos > 0 ){ ver = PSP_CfgFile_GetInt( pos ) ; }

		pos = PSP_CfgFile_FindKey( SZ_STATE_KEY_INCREMENT ) ;
		if( pos > 0 ){ esc->loop_mode = PSP_CfgFile_GetInt( pos ) ; }

		pos = PSP_CfgFile_FindKey( SZ_STATE_KEY_SAVE_M1 ) ;
		if( pos > 0 ){ esc->save_mode1 = PSP_CfgFile_GetInt( pos ) ; }

		pos = PSP_CfgFile_FindKey( SZ_STATE_KEY_SAVE_M2 ) ;
		if( pos > 0 ){ esc->save_mode2 = PSP_CfgFile_GetInt( pos ) ; }

		pos = PSP_CfgFile_FindKey( SZ_STATE_KEY_SLOT_SEL ) ;
		if( pos > 0 ){ esc->slot_sel = PSP_CfgFile_GetInt( pos ) ; }

		pos = PSP_CfgFile_FindKey( SZ_STATE_KEY_SLOT_INC ) ;
		if( pos > 0 ){ esc->slot_inc = PSP_CfgFile_GetInt( pos ) ; }

		EmuStateCfg_CheckValue( esc ) ;

		return 1 ;
	}

	return 0 ;
}

int EmuStateCfg_Save ( EMU_STATE_CFG* esc, const char* path )
{
	PSP_CfgFile_Init   ( SZ_STATE_TITLE ) ;

	PSP_CfgFile_MakeInt( SZ_STATE_KEY_VERSION   , EMU_STATE_FILEVER ) ;
	PSP_CfgFile_MakeInt( SZ_STATE_KEY_INCREMENT , esc->loop_mode     ) ;
	PSP_CfgFile_MakeInt( SZ_STATE_KEY_SAVE_M1   , esc->save_mode1    ) ;
	PSP_CfgFile_MakeInt( SZ_STATE_KEY_SAVE_M2   , esc->save_mode2    ) ;
	PSP_CfgFile_MakeInt( SZ_STATE_KEY_SLOT_SEL  , esc->slot_sel      ) ;
	PSP_CfgFile_MakeInt( SZ_STATE_KEY_SLOT_INC  , esc->slot_inc      ) ;

	return PSP_CfgFile_Write( path ) ;
}

__inline void EmuStateCfg_Copy ( EMU_STATE_CFG* dst, EMU_STATE_CFG* src )
{
	dst->loop_mode  = src->loop_mode  ;
	dst->save_mode1 = src->save_mode1 ;
	dst->save_mode2 = src->save_mode2 ;
	dst->slot_inc   = src->slot_inc   ;
	dst->slot_sel   = src->slot_sel   ;
}

void EmuStateCfg_InitDefault   ( const char* path )
{
	char buff[ 128 ]     ;
	strcpy( buff, path  );
	strcat( buff, "STATE" );

#ifndef WIN32
	sceIoMkdir( buff, 0777 );
#endif //WIN32

	strcpy( buff, path          ) ;
	strcat( buff, DEF_STATE_CFG ) ;

	EmuStateCfg_Load( &emu_state_cfg_def, buff ) ;
}

void EmuStateCfg_SaveDefault   ( const char* path )
{
	char buff[ 128 ]     ;
	strcpy( buff, path          ) ;
	strcat( buff, DEF_STATE_CFG ) ;

	EmuStateCfg_Save( &emu_state_cfg_def, buff ) ;
}

void EmuStateCfg_SetDefault  ( EMU_STATE_CFG* esc )
{
	EmuStateCfg_Copy( &emu_state_cfg_def, esc ) ;
}

void EmuStateCfg_CheckValue ( EMU_STATE_CFG* esc )
{
	if(      esc->save_mode1 < STATE_MODE_BOTH   ){ esc->save_mode1 = STATE_MODE_BOTH ; }
	else if( esc->save_mode1 > STATE_MODE_MEM    ){ esc->save_mode1 = STATE_MODE_MEM  ; }

	if(      esc->save_mode2 < STATE_MODE_BOTH   ){ esc->save_mode2 = STATE_MODE_BOTH ; }
	else if( esc->save_mode2 > STATE_MODE_MEM    ){ esc->save_mode2 = STATE_MODE_MEM  ; }

	if(      esc->loop_mode < STATE_LOOP_0_9     ){ esc->loop_mode  = STATE_LOOP_0_9  ; }
	else if( esc->loop_mode > STATE_LOOP_5_9     ){ esc->loop_mode  = STATE_LOOP_5_9  ; }

	if(      esc->slot_sel  < 0                  ){ esc->slot_sel = (MAX_STATE_SLOT-1); }
	else if( esc->slot_sel  > (MAX_STATE_SLOT-1) ){ esc->slot_sel = 0                 ; }

	//if(      esc->slot_inc  < 0                  ){ esc->slot_inc = 0                  ; }
	//else if( esc->slot_inc  > (MAX_STATE_SLOT-1) ){ esc->slot_inc = (MAX_STATE_SLOT-1) ; }

	switch( esc->loop_mode )
	{
	case STATE_LOOP_0_4: if( esc->slot_inc > 4 ){ esc->slot_inc = 0 ; } break ;
	case STATE_LOOP_5_9: 
		if     ( esc->slot_inc > 9 ){ esc->slot_inc = 5 ; } 
		else if( esc->slot_inc < 5 ){ esc->slot_inc = 9 ; } 
		break ;
	case STATE_LOOP_0_9: if( esc->slot_inc > 9 ){ esc->slot_inc = 0 ; } break ;
	}
}

void EmuState_Init ( EMU_STATE* es )
{
	int n ;

	es->cfg.loop_mode  = 0 ; // インクリメントステートセーブ
	es->cfg.save_mode1 = 0 ; // ステート保存モード（SLOT0-4）
	es->cfg.save_mode2 = 0 ; // ステート保存モード（SLOT5-9）
	es->cfg.dmy1       = 0 ;

	es->cfg.slot_sel   = 0 ;
	es->cfg.slot_inc   = 0 ;
	es->cfg.dmy2       = 0 ;
	es->cfg.dmy3       = 0 ;

	for( n=0; n < MAX_STATE_SLOT ; n++ )
	{
		EmuStateSlot_Init( &es->slot[n] ) ;
	}
}

void EmuState_Load  ( EMU_STATE* es, const char* path )
{
	int  n ;

	strcpy( emu_state_path, path          ) ;
	strcat( emu_state_path, EXT_STATE_CFG ) ;

	EmuStateCfg_Copy( &es->cfg, &emu_state_cfg_def ) ;

	EmuStateCfg_Load( &es->cfg, emu_state_path ) ;

	for( n=0; n < MAX_STATE_SLOT ; n++ )
	{
		emu_state_ext[3] = '0' + n ;
		strcpy( emu_state_path, path          ) ;
		strcat( emu_state_path, emu_state_ext ) ;

		EmuStateSlot_Init    ( &es->slot[n] ) ;
		EmuStateSlot_Load_MSD( &es->slot[n], emu_state_path ) ;
	}
}

void EmuState_Save  ( EMU_STATE* es, const char* path )
{
	int  n ;

	strcpy( emu_state_path, path          ) ;
	strcat( emu_state_path, EXT_STATE_CFG ) ;

	if( EmuStateCfg_Save( &es->cfg, emu_state_path ) )
	{
		if( es->cfg.save_mode1 == STATE_MODE_BOTH )
		{
			for( n=0; n < MAX_STATE_SLOT/2 ; n++ )
			{
				emu_state_ext[3] = '0' + n ;
				strcpy( emu_state_path, path          ) ;
				strcat( emu_state_path, emu_state_ext ) ;

				if( es->slot[n].flag & STATE_FLAG_UPDATE )
				{
					EmuStateSlot_Save_MSD( &es->slot[n], emu_state_path ) ;
				}
			}
		}

		if( es->cfg.save_mode2 == STATE_MODE_BOTH )
		{
			for( n=MAX_STATE_SLOT/2; n < MAX_STATE_SLOT ; n++ )
			{
				emu_state_ext[3] = '0' + n ;

				strcpy( emu_state_path, path          ) ;
				strcat( emu_state_path, emu_state_ext ) ;

				if( es->slot[n].flag & STATE_FLAG_UPDATE )
				{
					EmuStateSlot_Save_MSD( &es->slot[n], emu_state_path ) ;
				}
			}
		}
	}
}

int EmuState_SaveSlot( EMU_STATE* es, int slot, unsigned short* image, const char* path )
{
	if( EmuStateSlot_Save_MEM( &es->slot[slot], image ) )
	{
		if( es->cfg.save_mode1 == STATE_MODE_MSD )
		{
			if( (slot >= 0) && (slot < MAX_STATE_SLOT/2) )
			{
				emu_state_ext[3] = '0' + slot ;

				strcpy( emu_state_path, path          ) ;
				strcat( emu_state_path, emu_state_ext ) ;

				return EmuStateSlot_Save_MSD( &es->slot[slot], emu_state_path ) ;
			}
		}

		if( es->cfg.save_mode2 == STATE_MODE_MSD )
		{
			if( (slot >= MAX_STATE_SLOT/2) && (slot < MAX_STATE_SLOT) )
			{
				emu_state_ext[3] = '0' + slot ;

				strcpy( emu_state_path, path          ) ;
				strcat( emu_state_path, emu_state_ext ) ;

				return EmuStateSlot_Save_MSD( &es->slot[slot], emu_state_path ) ;
			}
		}

		return 1 ;
	}

	return 0 ;
}

int  EmuState_DeleteSlot ( EMU_STATE* es, int slot, const char* path )
{
	if( es->slot[slot].flag )
	{
		es->slot[slot].flag = 0 ;

		emu_state_ext[3] = '0' + slot ;

		strcpy( emu_state_path, path          ) ;
		strcat( emu_state_path, emu_state_ext ) ;

#ifdef _SCE_IO
		sceIoRemove( emu_state_path ) ;
#else //_SCE_IO
		remove( emu_state_path ) ;
#endif //_SCE_IO

		return 1 ;
	}

	return 0 ;
}

int EmuState_GetSaveCount ( EMU_STATE* es )
{
	int n, ct = 0 ;

	if( es->cfg.save_mode1 == STATE_MODE_BOTH )
	{
		for( n=0; n < MAX_STATE_SLOT/2 ; n++ )
		{
			if( es->slot[n].flag & STATE_FLAG_UPDATE )
			{
				ct++ ;
			}
		}
	}

	if( es->cfg.save_mode2 == STATE_MODE_BOTH )
	{
		for( n=MAX_STATE_SLOT/2; n < MAX_STATE_SLOT ; n++ )
		{
			if( es->slot[n].flag & STATE_FLAG_UPDATE )
			{
				ct++ ;
			}
		}
	}

	return ct ;
}

void EmuState_CalcInc ( EMU_STATE* es )
{
	es->cfg.slot_inc++ ;

	switch( es->cfg.loop_mode )
	{
	case STATE_LOOP_0_4: if( es->cfg.slot_inc > 4 ){ es->cfg.slot_inc = 0 ; } break ;
	case STATE_LOOP_5_9: 
		if     ( es->cfg.slot_inc > 9 ){ es->cfg.slot_inc = 5 ; } 
		else if( es->cfg.slot_inc < 5 ){ es->cfg.slot_inc = 9 ; } 
		break ;
	case STATE_LOOP_0_9: if( es->cfg.slot_inc > 9 ){ es->cfg.slot_inc = 0 ; } break ;
	}
}

void EmuState_Copy ( EMU_STATE* dst, EMU_STATE* src )
{
	int n ;
	
	EmuStateCfg_Copy( &dst->cfg, &src->cfg ) ;

	for( n=0; n < MAX_STATE_SLOT ; n++ )
	{
		EmuStateSlot_Copy( &dst->slot[n], &src->slot[n] ) ;
	}
}

void EmuStateSlot_Init ( EMU_STATE_SLOT* ess )
{
	ess->version   = EMU_STATE_SLOTVER ;
	ess->flag      = 0 ;
	ess->buff_len  = 0 ;

	memset( ess->image, 0, STATE_IMAGE_CX * STATE_IMAGE_CY * sizeof(short) ) ;
	memset( ess->buff , 0, MAX_STATE_BUFF ) ;
	memset( &ess->date, 0, sizeof(PSP_DATE_TIME) ) ;
}

int EmuStateSlot_Load_MEM ( EMU_STATE_SLOT* ess )
{
	if( ess->flag & STATE_FLAG_ACTIVE )
	{
		return md_set_state( ess ) ;
	}

	return 0 ;
}

int EmuStateSlot_Save_MEM ( EMU_STATE_SLOT* ess, unsigned short* image )
{
	int x, y ;

	if( !md_is_loadrom() ){ return 0 ; }

	ess->flag |= (STATE_FLAG_ACTIVE | STATE_FLAG_UPDATE) ;

	ess->version = EMU_STATE_SLOTVER ;

	memcpy( &ess->image[0][0], image, STATE_IMAGE_CX * STATE_IMAGE_CY * sizeof(short) ) ;

#ifdef _SCE_IO
	sceRtcGetCurrentClockLocalTime( &ess->date ) ;
#else //_SCE_IO
#endif //_SCE_IO

	return md_get_state( ess ) ;
}

int EmuStateSlot_Load_MSD ( EMU_STATE_SLOT* ess, const char* path )
{
	int len ;
	unsigned long size = sizeof(buff_zlib) ;

#ifdef _SCE_IO

	int fid = sceIoOpen( path, PSP_O_RDONLY, 0644 );

	if( fid )
	{
		len = sceIoRead ( fid, ess, sizeof(EMU_STATE_SLOT) );
		sceIoClose( fid );

#else //_SCE_IO

	FILE* fid = fopen( path, "rb" );

	if( fid )
	{
		len = fread ( ess, 1, sizeof(EMU_STATE_SLOT), fid );
		fclose( fid );

#endif //_SCE_IO

		if( (len != sizeof(EMU_STATE_SLOT))
		 || (ess->title[0] != 'D')
		 || (ess->title[1] != 'G')
		 || (ess->title[2] != 'E')
		 || (ess->title[3] != 'N')
		 || (ess->version  >  EMU_STATE_SLOTVER )
		 )
		{

#ifdef _USE_ZLIB
			if( uncompress( buff_zlib, &size, (const unsigned char*)ess, len ) == Z_OK )
			{
				if( size == sizeof(EMU_STATE_SLOT) )
				{
					memcpy( ess, buff_zlib, size ) ;

					if( 
						(ess->title[0] != 'D')
					 || (ess->title[1] != 'G')
					 || (ess->title[2] != 'E')
					 || (ess->title[3] != 'N')
					 || (ess->version  > EMU_STATE_SLOTVER )
					 )
					{
						EmuStateSlot_Init ( ess ) ;
						return 0 ;
					}

					ess->flag &= ~STATE_FLAG_UPDATE ;
					return 1 ;
				}
			}
#endif //_USE_ZLIB

			EmuStateSlot_Init ( ess ) ;
			return 0 ;
		}

		ess->flag &= ~STATE_FLAG_UPDATE ;

		return 1 ;
	}

	return 0 ;
}

int EmuStateSlot_Save_MSD ( EMU_STATE_SLOT* ess, const char* path )
{
	unsigned long size = sizeof(buff_zlib) ;

	if( (ess->flag & STATE_FLAG_ACTIVE) ) //&& (ess->flag & STATE_FLAG_UPDATE) )
	{
#ifdef _SCE_IO
		int fid = sceIoOpen( path, PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0644 );
#else // _SCE_IO
		FILE* fid = fopen( path, "wb" );
#endif // _SCE_IO

		if( fid )
		{ 
			ess->flag &= ~STATE_FLAG_UPDATE ;

			ess->title[0] = 'D' ;
			ess->title[1] = 'G' ;
			ess->title[2] = 'E' ;
			ess->title[3] = 'N' ;
			ess->version  = EMU_STATE_SLOTVER ;

#ifdef _USE_ZLIB
			if( compress( buff_zlib, &size, (const unsigned char*)ess, sizeof(EMU_STATE_SLOT) ) == Z_OK )
			{

#ifdef _SCE_IO
				sceIoWrite( fid, buff_zlib, size ) ;
#else // _SCE_IO
				fwrite( buff_zlib, 1, size, fid );
#endif // _SCE_IO

			}
			else
#endif //_USE_ZLIB

			{
#ifdef _SCE_IO
				sceIoWrite( fid, ess, sizeof(EMU_STATE_SLOT) ) ;
#else // _SCE_IO
				fwrite( ess, 1, sizeof(EMU_STATE_SLOT), fid );
#endif // _SCE_IO
			}

#ifdef _SCE_IO
			sceIoClose( fid ) ;
#else // _SCE_IO
			fclose( fid );
#endif // _SCE_IO

			return 1 ;
		}
	}

	return 0 ;
}

void EmuStateSlot_Copy ( EMU_STATE_SLOT* dst, EMU_STATE_SLOT* src )
{
	dst->flag     = src->flag     ;
	dst->buff_len = src->buff_len ;

	memcpy( &dst->image[0][0] , &src->image[0][0], STATE_IMAGE_CY * STATE_IMAGE_CX * sizeof(short) ) ;

	memcpy( dst->buff , src->buff , MAX_STATE_BUFF ) ;

	dst->date = src->date ;
}

unsigned char* EmuState_GetBuffer() { return buff_zlib ; }

int EmuState_Compress ( void* buff, int len, int offset )
{
	unsigned long size = sizeof(buff_zlib) ;

#ifdef _USE_ZLIB
	if( compress( buff_zlib + offset, &size, (const unsigned char*)buff, len ) == Z_OK )
	{
		return size ;
	}
#endif //_USE_ZLIB

	return 0 ;
}

int EmuState_Uncompress ( void* buff, int len )
{
	unsigned long size = sizeof(buff_zlib) ;

#ifdef _USE_ZLIB
	if( uncompress( buff_zlib, &size, buff, len ) == Z_OK )
	{

		return size ;
	}
#endif //_USE_ZLIB

	return 0 ;
}

///////////////////////////////////////////////////////////////////////////
