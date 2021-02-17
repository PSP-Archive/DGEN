////////////////////////////////////////////////
/// pcecore_main.c
////////////////////////////////////////////////
#include "../pcecore/MainBoard.h"
#include "../pcecore/CriticalSection.h"
#include "../pcecore/ScreenInterface.h"
#include "../pcecore/InputInterface.h"
#include "../pcecore/CdromInterface.h"
#include "../pcecore/CdFader.h"
////////////////////////////////////////////////
#include "emu_config.h"
#include "psp_pad.h"
////////////////////////////////////////////////

////////////////////////////////////////////////
/// for GLOBAL
////////////////////////////////////////////////

static unsigned char* pScreen = NULL ;
static short* pSound = NULL ;
int line_width = 512 ;
int is_pce_loadrom = 0 ;
static void (*pSoundCallBack)(Sint16* pBuf, Sint32 nSamples) = NULL;
int pce_sound_buff_len = 512*4 ;
unsigned long pce_paddata = 0 ;

extern unsigned char*  pce_malloc_pMainRam   ; //[0x2000 +4];
extern unsigned char*  pce_malloc_pBackupRam ; //[0x2000 +4];
extern unsigned char*  pce_malloc_pBufferRam ; //[0x40000+4];
extern unsigned char*  _AcRam                ; // 0x200000
extern unsigned char*  pce_rom_buf_ptr       ; 
extern unsigned long   pce_max_rom_size      ;


void pce_init_memory( unsigned char* pCoreBuff, unsigned long nBuffSize )
{
	pce_malloc_pMainRam   = pCoreBuff                        ;
	pce_malloc_pBackupRam = pce_malloc_pMainRam   + 0x2000   ;
	pce_malloc_pBufferRam = pce_malloc_pBackupRam + 0x2000   ;
	_AcRam                = pce_malloc_pBufferRam + 0x40000  ;
	pce_rom_buf_ptr       = _AcRam                + 0x200000 ;

	pce_max_rom_size = nBuffSize - 3 * 1024 * 1024 ;
}

void pce_init()
{
	CS_Init() ;
	SCREEN_Init( 320, 224, 16, SCREEN_FDEFAULT ) ;
	CDFADER_Init( 50,50,50 ) ;
}

void pce_setconfig( EMU_CONFIG* emc )
{
}

int pce_load_rom( const char* path )
{
	if( !MAINBOARD_Init( path ) )
	{
		return -1 ;
	}

	if( !MAINBOARD_ChangeSoundMode( 512 * 4, 44100, 4, 100, FALSE) )
	{
		return -2 ;
	}

	MAINBOARD_ChangeFPS( 60 ) ;

	is_pce_loadrom = 1 ;

	return 0 ;
}

void pce_setframe( unsigned char* p, int width )
{
	pScreen = p ;
	line_width = width ;
}

void pce_setsound( short* p, short len )
{
	pSound = p ;
	pce_sound_buff_len = len ;
}

void pce_sound_write()
{
	if( pSound && pSoundCallBack )
	{
		(*pSoundCallBack)( pSound, pce_sound_buff_len ) ;
	}

	/*
	int sound_buf_len = pce_sound_buff_len ; //(PCE_SOUND_BUFF_LEN + PCE_SOUND_BUFF_LEN * 0); //setting.sound_buffer) ;
	static unsigned int nextbank=1;
	unsigned int playbank=cur_play/sound_buf_len;
	short* p = (short*)&sound_buf[nextbank*sound_buf_len*2] ;
	if (nextbank!=playbank) 
	{
		if( pSoundCallBack )
		{
			(*pSoundCallBack)( p, sound_buf_len ) ;

			nextbank++;
			if( (sound_buf_len * (nextbank+1)) > (SOUND_BANKLEN*4) ){ nextbank = 0; }
		}
	}
	*/
}

int pce_sound_update( short* p, short len )
{
	int i ;

	pSound = p ;
	pce_sound_buff_len = len ;

	if( pSound && pSoundCallBack )
	{
		(*pSoundCallBack)( pSound, pce_sound_buff_len ) ;
	}
}

void pce_setpaddata( unsigned long buttons )
{
	pce_paddata = buttons ;
}

int pce_run()
{
	if( !is_pce_loadrom ){ return 0 ; }

	MAINBOARD_AdvanceFrame() ;

//	pce_sound_write() ;

	return 1 ;
}

int pce_run_skip()
{
	if( !is_pce_loadrom ){ return 0 ; }

	MAINBOARD_AdvanceFrame() ;

//	pce_sound_write() ;

	return 1 ;
}

////////////////////////////////////////////////
/// for SCREEN
////////////////////////////////////////////////

BOOL
SCREEN_IsFullScreen()
{
	return FALSE ;
}

BOOL
SCREEN_ToggleFullScreen()
{
	return TRUE ;
}

BOOL
SCREEN_Init(
	Sint32		width,
	Sint32		height,
	Sint32		bpp,
	Uint32		flags)
{
	return TRUE ;
}

Sint32
SCREEN_GetWidth()
{
	return 320 ;
}

Sint32
SCREEN_GetHeight()
{
	return 224 ;
}

Sint32
SCREEN_GetRshift()
{
	return 10 ;
}

Sint32
SCREEN_GetGshift()
{
	return 5 ;
}

Sint32
SCREEN_GetBshift()
{
	return 0 ;
}

Uint32
SCREEN_GetRmask()
{
	return 0x0000001F ;
}

Uint32
SCREEN_GetGmask()
{
	return 0x0000001F << 5 ;
}

Uint32
SCREEN_GetBmask()
{
	return 0x0000001F << 10 ;
}

Uint32
SCREEN_GetBytesPerPixel()
{
	return 2 ;
}

BOOL
SCREEN_ChangeMode(
	Sint32		width,
	Sint32		height,
	Sint32		bpp,
	Uint32		flags)
{
	return 0 ;
}

void
SCREEN_SetFPS(
	int		fps)
{
}

BOOL
SCREEN_WaitVBlank()
{
	return TRUE ;
}

BOOL
SCREEN_ShouldDraw()
{
	return TRUE ;
}

void
SCREEN_DrawText(
	const Sint32	x,
	const Sint32	y,
	const Uint32	fgColor,
	const Uint32	bgColor,
	const BOOL		bOpaque,
	const char*		pText)
{
}

BOOL SCREEN_Lock()
{
	return TRUE ;
}

void* SCREEN_GetBuffer()
{
	return pScreen ;
}

const Sint32 SCREEN_GetBufferPitch()
{
	return 0 ;
}

void SCREEN_Unlock()
{
}

void SCREEN_FillRect(
	Sint32		x,
	Sint32		y,
	Sint32		width,
	Sint32		height,
	Uint32		color)
{

}

void
SCREEN_Update(
	Sint32		x,
	Sint32		y,
	Sint32		width,
	Sint32		height)
{

}

void
SCREEN_Blt(
	const void*		pSrc,
	Sint32		srcX,
	Sint32		srcY,
	Sint32		dstX,
	Sint32		dstY,
	Sint32		srcW,
	Sint32		srcH,
	Sint32		dstW,
	Sint32		dstH,
	Sint32		srcPitch,
	Uint32		alpha)
{
	Uint32*	pSrc32		= (Uint32*)( (Uint16*)pSrc + srcY * srcPitch + srcX );
	Uint32*	pDst32		= (Uint32*)( pScreen + dstY * line_width + dstX *2 ) ;
	int x, y ;

	for (y = 0; y < dstH; y++)
	{
		//memcpy(pDst16, pSrc16, dstW*2);
		for( x=0; x < dstW/2 ; x++ )
		{
			pDst32[x] = pSrc32[x] ;
		}

		pSrc32 += srcPitch   / 2 ;
		pDst32 += line_width / 2 ;
	}
}

void SCREEN_Deinit()
{
}

////////////////////////////////////////////////
/// for Audio
////////////////////////////////////////////////

BOOL
AOUT_Init(
	Uint32		bufSize,			// in samples 
	Uint32		sampleRate,
	BOOL		bStereo,
	void		(*pCallBack)(Sint16* pBuf, Sint32 nSamples))
{
	pce_sound_buff_len = bufSize   ;
	pSoundCallBack = pCallBack ;
	return TRUE ;
}

BOOL AOUT_Lock()
{
	return TRUE ;
}

void AOUT_Unlock()
{
}

void AOUT_Play( BOOL bPlay)
{

}

void AOUT_Deinit()
{
}

//////////////////////////////////

BOOL CS_Init(){ return TRUE ; }
BOOL CS_Deinit(){ return TRUE ; }
Sint32 CS_Create(){ return 0 ; }
BOOL CS_Delete( const Sint32		id){ return TRUE ; }
BOOL CS_Enter( const Sint32		id){ return TRUE ; }
BOOL CS_Leave( const Sint32		id){ return TRUE ; }

////////////////////////////////////////////////
/// for INPUT
////////////////////////////////////////////////


BOOL	INPUT_Init(Sint32	inputType)
{
	return TRUE ;
}

void	INPUT_Deinit()
{
}

void	INPUT_UpdateState()
{
}

void	INPUT_ClearState()
{
}

//BOOL	INPUT_IsAnyButtonPressed();
//BOOL	INPUT_IsAnyButtonTriggered();

BOOL	INPUT_IsPressed(Sint32 userButtonID)
{
	switch( userButtonID )
	{
	case JOYPAD_BUTTON_I      : return ( pce_paddata & CTRL_CIRCLE   ) ;
	case JOYPAD_BUTTON_II     : return ( pce_paddata & CTRL_CROSS    ) ;
	case JOYPAD_BUTTON_III    : return ( pce_paddata & CTRL_SQUARE   ) ;
	case JOYPAD_BUTTON_IV     : return ( pce_paddata & CTRL_TRIANGLE ) ;
	case JOYPAD_BUTTON_V      : return ( pce_paddata & CTRL_LTRIGGER ) ;
	case JOYPAD_BUTTON_VI     : return ( pce_paddata & CTRL_RTRIGGER ) ;
	case JOYPAD_BUTTON_SELECT : return ( pce_paddata & CTRL_SELECT   ) ;
	case JOYPAD_BUTTON_RUN    : return ( pce_paddata & CTRL_START    ) ;
	case JOYPAD_BUTTON_UP     : return ( pce_paddata & CTRL_UP       ) ;
	case JOYPAD_BUTTON_DOWN   : return ( pce_paddata & CTRL_DOWN     ) ;
	case JOYPAD_BUTTON_LEFT   : return ( pce_paddata & CTRL_LEFT     ) ;
	case JOYPAD_BUTTON_RIGHT  : return ( pce_paddata & CTRL_RIGHT    ) ;
	}

	return FALSE ;
}

BOOL	INPUT_IsTriggered(Sint32 userButtonID)
{
	return FALSE ;
}

BOOL	INPUT_IsKeyPressed(Sint32 userKeyID)
{
	return FALSE ;
}

//BOOL	INPUT_ChangeDevice(Sint32	inputType);

//void	INPUT_SetRepeatRate(Sint32 repeatRate);
//void	INPUT_SetRepeatDelay(Sint32 repeatDelay);

BOOL	INPUT_ConnectButton(Sint32 userButtonID, Sint32 inputButtonID)
{
	return TRUE ;
}

BOOL	INPUT_ConnectKey(Sint32 userButtonID, Sint32 inputKeyID)
{
	return TRUE ;
}

void	INPUT_Record(BOOL bRecord)
{
}

void	INPUT_Playback(BOOL bPlayback)
{
}


BOOL	INPUT_WriteBuffer(FILE*	fp)
{
	return TRUE ;
}

BOOL	INPUT_ReadBuffer(FILE* fp)
{
	return TRUE ;
}

////////////////////////////////////////////////
/// for CDROM
////////////////////////////////////////////////
BOOL
CDIF_Init(void (*callback)(Uint32 flags))
{
	return TRUE ;
}


/*-----------------------------------------------------------------------------
	[Deinit]
		終了します。 
-----------------------------------------------------------------------------*/
void
CDIF_Deinit(void)
{
}

Sint32
CDIF_GetNumDevices()
{
	return 0 ;
}

BOOL
CDIF_SelectDevice(
	Sint32	deviceNum)
{
	return 0 ;
}

Sint32
CDIF_GetFirstTrack()
{
	return 0 ;
}

Sint32
CDIF_GetLastTrack()
{
	return 0 ; 
}

Uint32
CDIF_GetTrackStartPositionMSF(
	Sint32		track)
{
	return 0 ;
}

Uint32
CDIF_GetTrackStartPositionLBA(
	Sint32		track)
{
	return 0 ;
}


BOOL
CDIF_SetSpeed(
	Uint32		speed)
{
	return TRUE ;
}

BOOL
CDIF_IsDeviceBusy()
{
	return FALSE ;
}


/*-----------------------------------------------------------------------------
	[ReadSector]
		セクタを読み出します。
-----------------------------------------------------------------------------*/
BOOL
CDIF_ReadSector(
	Uint8*		pBuf,		// 読み出しデータ格納バッファ 
	Uint32		sector,		// セクタ番号 (0, 1, 2, ...) 
	Uint32		nSectors,	// 読み出すセクタ数 
	BOOL		bCallback) 	// コールバック通知を行なうかどうか 
{
	return TRUE ;
}

BOOL
CDIF_Seek(
	Uint8	minStart,
	Uint8	secStart,
	Uint8	frmStart,
	BOOL	bCallback)
{
	return TRUE ;
}

/*-----------------------------------------------------------------------------
	[PlayAudioMSF]
		指定のＭＳＦから再生します。 
-----------------------------------------------------------------------------*/
BOOL
CDIF_PlayAudioMSF(
	Uint8		minStart,
	Uint8		secStart,
	Uint8		frmStart,
	Uint8		minEnd,
	Uint8		secEnd,
	Uint8		frmEnd,
	BOOL		bCallback)
{
	return TRUE ;
}


/*-----------------------------------------------------------------------------
	[ReadSubChannelQ]
		Ｑサブチャネルを読み出します。
-----------------------------------------------------------------------------*/
BOOL
CDIF_ReadSubChannelQ(
	Uint8*		pBuf,		// 10-byte buffer
	BOOL		bCallback)
{
	return TRUE ;
}


/*-----------------------------------------------------------------------------
	[StopAudioTrack]
		トラックの再生を停止します。 
-----------------------------------------------------------------------------*/
BOOL
CDIF_StopAudioTrack(
	BOOL		bCallback)
{
	return TRUE ;
}


/*-----------------------------------------------------------------------------
	[PauseAudioTrack]
		トラックの再生を一時停止／再開します。 
-----------------------------------------------------------------------------*/
BOOL
CDIF_PauseAudioTrack(
	BOOL		bPause,
	BOOL		bCallback)
{
	return TRUE ;
}

////////////////////////////////////////////////
/// for CD Fader
////////////////////////////////////////////////

BOOL
CDFADER_Init(
	Sint32				volumeMaster,	// volume (0 - 100), < 0 for the value set on the volume slide
	Sint32				volumeWave,
	Sint32				volumeCd)
{
	return TRUE ;
}

void
CDFADER_Synchronize(
	Uint32				mixerHandle,
	Uint32				controlID)
{

}

void CDFADER_Deinit()
{
}

void CDFADER_FadeOut(Sint32		speed)
{
}

void CDFADER_FadeIn(Sint32		speed)
{
}

void CDFADER_AdvanceClock(Sint32		cycle)
{
}

////////////////////////////////////////////////
