///////////////////////////////////////////////////////////////////////////
/// psp_irda.c
///////////////////////////////////////////////////////////////////////////
#include "psp_irda.h"
#include "psp_std.h"
///////////////////////////////////////////////////////////////////////////
int psp_irda_fd = 0 ;
///////////////////////////////////////////////////////////////////////////

int PSP_IrDA_Open ()
{
	if( psp_irda_fd == 0 )
	{
		psp_irda_fd = sceIoOpen( "irda0:", PSP_O_RDWR, 0 ) ;
	}

	return psp_irda_fd ;
}

int PSP_IrDA_Close()
{
	if( psp_irda_fd )
	{
		sceIoClose( psp_irda_fd ) ;
	}

	psp_irda_fd = 0 ;
}

int PSP_IrDA_IsOpen ()
{
	return psp_irda_fd ;
}

int PSP_IrDA_Write( void* pData, int nLen )
{
	if( psp_irda_fd )
	{
		return sceIoWrite( psp_irda_fd, pData, nLen ) ;
	}

	return 0 ;
}

int PSP_IrDA_Read ( void* pData, int nLen )
{
	if( psp_irda_fd )
	{
		return sceIoRead( psp_irda_fd, pData, nLen ) ;
	}

	return 0 ;
}

int PSP_IrDA_ReadEx ( void* pData, int nLen, unsigned long key )
{
	int n, len, retry ;
	unsigned char buff[ 128 ] ;

	if( psp_irda_fd )
	{
		retry = 0 ;
		len   = 0 ;
		for(;;)
		{
			len += sceIoRead( psp_irda_fd, &buff[len], 127 - len ) ;

			if( len > nLen ){ break ; }

			retry++ ;
			if( retry > 1024 ){ break ; }
		}

		for( n=0; n < len - nLen; n++ )
		{
			if( memcmp( &buff[n], &key, 4 ) == 0 )
			{
				memcpy( pData, &buff[n], nLen ) ;
				return nLen ;
			}
		}
	}

	return 0 ;
}

///////////////////////////////////////////////////////////////////////////
