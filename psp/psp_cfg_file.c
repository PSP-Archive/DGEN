///////////////////////////////////////////////////////////////////////////
/// psp_cfg_file.c
///////////////////////////////////////////////////////////////////////////
#include "psp_cfg_file.h"
///////////////////////////////////////////////////////////////////////////
#define PSP_O_RDONLY	0x0001 
#define PSP_O_WRONLY	0x0002 
#define PSP_O_RDWR		0x0003 
#define PSP_O_NBLOCK	0x0010 
#define PSP_O_APPEND	0x0100 
#define PSP_O_CREAT		0x0200 
#define PSP_O_TRUNC		0x0400 
#define PSP_O_NOWAIT	0x8000 
///////////////////////////////////////////////////////////////////////////
#define MAX_CFG_FILEBUFF  (0x2000)
///////////////////////////////////////////////////////////////////////////
int  cfg_filelen = 0;
char cfg_filebuff[ MAX_CFG_FILEBUFF ] ;
///////////////////////////////////////////////////////////////////////////
void PSP_CfgFile_Init ( const char* header )
{
	cfg_filelen = 0 ;
	strcpy( cfg_filebuff, header ) ; cfg_filelen += strlen(header) ;
	strcat( cfg_filebuff, "\r\n" ) ; cfg_filelen += 2              ;
}

#ifdef WIN32
int PSP_CfgFile_Read  ( const char* path ){ return 1 ; }
int PSP_CfgFile_Write ( const char* path ){ return 1 ; }
#else

int PSP_CfgFile_Read  ( const char* path )
{
	int fid = sceIoOpen( path, PSP_O_RDONLY, 0644 );

	if( fid >= 0 )
	{
		cfg_filelen = sceIoRead( fid, cfg_filebuff, MAX_CFG_FILEBUFF ) ;

		sceIoClose( fid ) ;

		return 1 ;
	}

	return 0 ;
}

int PSP_CfgFile_Write ( const char* path )
{
	int fid = sceIoOpen( path, PSP_O_WRONLY|PSP_O_TRUNC|PSP_O_CREAT, 0644 );

	if( fid >= 0 )
	{
		sceIoWrite( fid, cfg_filebuff, cfg_filelen ) ;

		sceIoClose( fid ) ;

		return 1 ;
	}

	return 0 ;

}

#endif //WIN32

int PSP_CfgFile_FindKey( const char* key )
{
	int n, m, k , keylen;

	keylen = strlen( key ) ;

	for( n=0; n < cfg_filelen - keylen ; n++ )
	{
		if( key[0] == cfg_filebuff[n] )
		{
			k = 1 ;
			for( m=0; m < keylen ; m++ )
			{
				if( key[m] != cfg_filebuff[n+m] ){ k=0; break ; }
			}

			if( k ){ return n + keylen ; }
		}
	}

	return -1 ;
}

int PSP_CfgFile_GetInt( int pos )
{
	char val[8]  ;
	int  n       ;
	int  ct  = 0 ;
	int  deg = 1 ;
	int  ret = 0 ;

	for( n=pos; n < pos+8 ; n++ )
	{
		if( (cfg_filebuff[n] == '\r') && (cfg_filebuff[n+1] == '\n') )
		{
			val[ct] = 0 ;
			break ;
		}
		else if( (cfg_filebuff[n] < '0') || (cfg_filebuff[n] > '9') )
		{
			return 0 ;
		}

		val[ct] = cfg_filebuff[n] ;
		ct++              ;
		deg *= 10         ;
	}

	deg /= 10 ;

	for( n=0; n < ct ; n++ )
	{
		ret += (val[n] - '0') * deg ;
		deg /= 10 ;
	}

	return ret ;
}

int PSP_CfgFile_GetHex( int pos )
{
	int n ;
	int ret = 0 ;

	for( n=pos; n < pos+8 ; n++ )
	{
		if( (cfg_filebuff[n] == '\r') && (cfg_filebuff[n+1] == '\n') )
		{
			break ;
		}
		else if( ((cfg_filebuff[n] >= '0') && (cfg_filebuff[n] <= '9')) )
		{
			ret = (ret << 4) | (cfg_filebuff[n] - '0') ;
		}
		else if( (cfg_filebuff[n] >= 'A') && (cfg_filebuff[n] <= 'F') )
		{
			ret = (ret << 4) | (cfg_filebuff[n] - 'A') + 10 ;
		}
		else if( (cfg_filebuff[n] >= 'a') && (cfg_filebuff[n] <= 'f') )
		{
			ret = (ret << 4) | (cfg_filebuff[n] - 'a') + 10 ;
		}
		else { return 0 ; }
	}

	return ret ;
}

int  PSP_CfgFile_GetStr ( int pos, char* str, int max )
{
	int n, m = 0 ;

	for( n=pos; n < cfg_filelen -1 ; n++ )
	{
		if( (cfg_filebuff[n] == '\r') && (cfg_filebuff[n+1] == '\n') )
		{
			break ;
		}
		else
		{
			str[m++] = cfg_filebuff[n] ;
			if( m >= max ){ break; }
		}
	}

	str[m] = 0 ;

	return m ;
}

int PSP_CfgFile_MakeInt( const char* key, int val )
{
	int len ;

	len = strlen(key) + cfg_filelen ;
	strcat( cfg_filebuff, key ) ;

	if( val < 10 )
	{ 
		cfg_filebuff[len++] = '0' + val ;  
	}
	else if( val < 100 )
	{
		cfg_filebuff[len++] = '0' + val / 10 ;  
		cfg_filebuff[len++] = '0' + val % 10 ;  
	}
	else if( val < 1000 )
	{
		cfg_filebuff[len++] = '0' +  val / 100      ; 
		cfg_filebuff[len++] = '0' + (val / 10) % 10 ; 
		cfg_filebuff[len++] = '0' +  val % 10       ; 
	}

	cfg_filebuff[len++] = '\r' ;
	cfg_filebuff[len++] = '\n' ;
	cfg_filebuff[len  ] = 0    ;

	cfg_filelen = len ;
	return len ;
}

int PSP_CfgFile_MakeHex( const char* key, int val )
{
	int len , t;

	len = strlen(key) + cfg_filelen ;
	strcat( cfg_filebuff, key ) ;

	if( val < 0x100 )
	{ 
		t = (val >> 4) & 0x0F ;
		cfg_filebuff[len++] = (t < 10) ? ('0' + t) : ('A' + t - 10) ;  
		t = (val     ) & 0x0F ;
		cfg_filebuff[len++] = (t < 10) ? ('0' + t) : ('A' + t - 10) ;  
	}
	else if( val < 0x10000 )
	{
		t = (val >> 12) & 0x0F ;
		cfg_filebuff[len++] = (t < 10) ? ('0' + t) : ('A' + t - 10) ;  
		t = (val >>  8) & 0x0F ;
		cfg_filebuff[len++] = (t < 10) ? ('0' + t) : ('A' + t - 10) ;  
		t = (val >>  4) & 0x0F ;
		cfg_filebuff[len++] = (t < 10) ? ('0' + t) : ('A' + t - 10) ;  
		t = (val      ) & 0x0F ;
		cfg_filebuff[len++] = (t < 10) ? ('0' + t) : ('A' + t - 10) ;  
	}
	else
	{
		t = (val >> 28) & 0x0F ;
		cfg_filebuff[len++] = (t < 10) ? ('0' + t) : ('A' + t - 10) ;  
		t = (val >> 24) & 0x0F ;
		cfg_filebuff[len++] = (t < 10) ? ('0' + t) : ('A' + t - 10) ;  
		t = (val >> 20) & 0x0F ;
		cfg_filebuff[len++] = (t < 10) ? ('0' + t) : ('A' + t - 10) ;  
		t = (val >> 16) & 0x0F ;
		cfg_filebuff[len++] = (t < 10) ? ('0' + t) : ('A' + t - 10) ;  
		t = (val >> 12) & 0x0F ;
		cfg_filebuff[len++] = (t < 10) ? ('0' + t) : ('A' + t - 10) ;  
		t = (val >>  8) & 0x0F ;
		cfg_filebuff[len++] = (t < 10) ? ('0' + t) : ('A' + t - 10) ;  
		t = (val >>  4) & 0x0F ;
		cfg_filebuff[len++] = (t < 10) ? ('0' + t) : ('A' + t - 10) ;  
		t = (val      ) & 0x0F ;
		cfg_filebuff[len++] = (t < 10) ? ('0' + t) : ('A' + t - 10) ;  
	}

	cfg_filebuff[len++] = '\r' ;
	cfg_filebuff[len++] = '\n' ;
	cfg_filebuff[len  ] = 0    ;

	cfg_filelen = len ;
	return len ;
}

int  PSP_CfgFile_MakeStr ( const char* key, char* str )
{
	strcat( cfg_filebuff, key ) ;
	strcat( cfg_filebuff, str ) ;
	strcat( cfg_filebuff, "\r\n" ) ;
	cfg_filelen += strlen( key ) + strlen( str ) + 2 ;
	return cfg_filelen ;
}

///////////////////////////////////////////////////////////////////////////
