// DGen/SDL v1.15+

// Module for loading in the different rom image types (.bin/.smd)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifdef _PSP10 ////////////////////////////////////////////////////////////////////////////////

#ifndef WIN32
#include <unistd.h>
#endif //


#ifdef _USE_ZLIB
#include "../psp/psp_std.h"
#include "../lib/zlibInterface.h"
#include "../lib/zlib.h"
//#include "../psp/gz.h"


typedef struct _UNZIP_INFO
{
	unsigned char* buff ;
	int            len  ;

} UNZIP_INFO ;

// Unzip �R�[���o�b�N
int funcUnzipCallback(int nCallbackId, unsigned long ulExtractSize, unsigned long ulCurrentPosition,
                      const void *pData, unsigned long ulDataSize, unsigned long ulUserData)
{
    const char *pszFileName;
	char *pszExt;
    const unsigned char *pbData;
    UNZIP_INFO* pInfo = (UNZIP_INFO*)ulUserData;

    switch( nCallbackId )
	{
    case UZCB_FIND_FILE:
//		pInfo->len = ulExtractSize;
//		return UZCBR_CANCEL ;
//		return UZCBR_PASS;

		pszFileName = (const char *)pData ;

		pszExt = strrchr( pszFileName , '.' ) ;
		if( pszExt ) 
		{
			if( (strcmp( pszExt, ".bin") == 0) || (strcmp( pszExt, ".smd") == 0)
			 || (strcmp( pszExt, ".BIN") == 0) || (strcmp( pszExt, ".SMD") == 0) )
			{
				// �W�J���閼�O�Arom size���o���Ă���
				//strcpy(pRomInfo->szFileName, pszFileName);
				pInfo->len = ulExtractSize;
				return UZCBR_OK;
			}
		}
        break;

    case UZCB_EXTRACT_PROGRESS:
		pbData = (const unsigned char *)pData;
		// �W�J���ꂽ�f�[�^���i�[���悤
		memcpy( pInfo->buff + ulCurrentPosition, pbData, ulDataSize );
		//draw_load_rom_progress(ulCurrentPosition + ulDataSize, ulExtractSize);
//		return UZCBR_CANCEL ;
		return UZCBR_OK;

    default: // unknown...
        break;
    }

    return UZCBR_PASS;
}

#endif //_USE_ZLIB

unsigned char chunk_buf[ 16384 ] ;

int smd_to_bin( unsigned char *into, int len )
{
	int got_to, i, j, chunk_count;

	chunk_count = ( (len - 512) / 16384 );

	for (got_to=0,i=0; i<chunk_count; i++,got_to+=16384)
	{
		memcpy( chunk_buf, &into[got_to + 512], 16384 ) ;

		for( j=0; j < 8192 ; j++ )
		{ 
			into[got_to+(j<<1)+1] = chunk_buf[j     ];
			into[got_to+(j<<1)+0] = chunk_buf[j+8192]; 
		}
	}

	return (chunk_count * 16384); ;
}

// If 'into' is NULL returns rom size, otherwise expect
// 'into' to be a buffer big enough for the rom size
// (i.e. pass NULL, malloc, pass pointer, emulate, free pointer)
int load_rom_into(char *name,unsigned char *into, int max_size)
{
#ifdef _USE_ZLIB
	UNZIP_INFO info ;
	gzFile fid ;
#endif // _USE_ZLIB

	int len, nRet ;
	unsigned char magicbuf[10];
	FILE *romfile;
	int fd ;
	char buff[128] ;
	if (name==NULL) return -1;

	/* Open the file and get the first little shnippit of it so we can check
	* the magic numbers on it. */

#ifdef _SCE_IO
	fd = sceIoOpen( name, PSP_O_RDONLY, 0644 ) ;
	if( !fd ){ return -4 ; }
	sceIoRead ( fd, magicbuf, 10 ) ;
	sceIoClose( fd ) ;
#else

	if(!(romfile = fopen(name, "rb"))) return -4;
	fread(magicbuf, 10, 1, romfile);
	fclose(romfile);

#endif //

#ifdef _USE_ZLIB

	/* Check for the magic on various gzip-supported compressions */
	if( (magicbuf[0] == 037 && magicbuf[1] == 0213) ) /* gzipped    (.gz)    */
	{

		fid = gzopen( name, "r" );

		if( fid )
		{
			char* p = strrchr( name, '.' ) ;
			if( (strcmp( p, ".TGZ" ) == 0) || (strcmp( p, ".tgz" ) == 0) )
			{
				gzread( fid, into, 512 );
			}

			/*
			if( !gzGetOrigName( name, buff, fid ) )
			{
				gzclose( fid );
				return -10 ;
			}

			if( getExtId(name) != EXT_GB )
			{
				gzclose( fid );
				return -11 ;
			}
			*/

			len = gzread( fid, into, max_size );
			gzclose( fid );
		}
		else{ return -7 ; }

		//return -len ;

		//strcpy( buff, name ) ;
		//strcat( buff, ".bin" ) ;
		//if( !(romfile = fopen(buff, "wb")) ) return -5;
		//fwrite( into, len, 1, romfile );
		//fclose( romfile );
	}
	else if( (magicbuf[0] == 'P') && (magicbuf[1] == 'K') )   /* ZIP        (.zip ;) */
	{
		info.buff = into ;

		// Unzip�R�[���o�b�N�Z�b�g
		Unzip_setCallback( funcUnzipCallback );

		// Unzip�W�J����
	    nRet = Unzip_execExtract( name, (unsigned long)&info);

		if (nRet != UZEXR_OK) 
		{
			// �ǂݍ��ݎ��s�I - ���̃R�[�h�ł́AUZEXR_CANCEL�������ɗ���
			// ���܂����R�[���o�b�N�ŃL�����Z�����ĂȂ��̂Ŗ���
			return -10 ;
		}

		len = info.len ;

		/*
		strcpy( buff, name ) ;
		strcat( buff, ".bin" ) ;
		if( !(romfile = fopen(buff, "wb")) ) return -5;
		fwrite( into, len, 1, romfile );
		fclose( romfile );
		return -len ;
		*/
	}
	else

#endif //_USE_ZLIB

	{
#ifdef _SCE_IO
		fd = sceIoOpen( name, PSP_O_RDONLY, 0644 ) ;
		if( !fd ){ return -4 ; }
		len = sceIoRead ( fd, into, max_size ) ;
		sceIoClose( fd ) ;

#else //_SCE_IO
		if( !(romfile = fopen(name, "rb")) ) return -5;
		len = fread( into, 1, max_size, romfile );
		fclose(romfile);
#endif //_SCE_IO
	}


	if( (into[8] == 0xaa) && (into[9] == 0xbb) )/* Next check for SMD magic */
	{
		return smd_to_bin( into, len ) ;
	}

	return len ;
}



#else // _PSP10 //////////////////////////////////////////////////

static int load_bin_into(char *name,unsigned char *into)
{
  FILE *hand=NULL;
  int file_size=0;

  hand=fopen(name,"rb");
  if (hand==NULL)
    return -2;
  fseek(hand,0,SEEK_END);
  file_size=ftell(hand);
  fseek(hand,0,SEEK_SET);
  fclose(hand);
  if (into==NULL)
  {
	  return file_size;
  }

  hand=fopen(name,"rb");
  if (hand==NULL)
    return -2;

  if( fread(into,1,file_size,hand) != file_size )
  {
	fclose(hand);
	return file_size ;
  }

  fclose(hand);
  return 0;
}

/*
  WHAT YOU FIND IN THE 512 BYTES HEADER:

0: Number of blocks                           1
1: H03                                        *
2: SPLIT?                                     2
8: HAA                                        *
9: HBB                                        *
ALL OTHER BYTES: H00

1: This first byte should have the number of 16KB blocks the rom has.
The header isn't part of the formula, so this number is:
            [size of rom-512]/16386
   If the size is more than 255, the value should be H00.

2: This byte indicates if the ROM is a part of a splitted rom series. If
the rom is the last part of the series (or isn't a splitted rom at all),
this byte should be H00. In other cases should be H40. See "CREATING
SPLITTED ROMS" for details on this format.
*/

// 16k chunks, even bytes first then odd

static int load_smd_into(char *name,unsigned char *into)
{
 unsigned char head[512]={0};
  FILE *hand=NULL;
  int chunk_count=0,file_chunks=0;
  int file_size=0;
  unsigned char *chunk_buf=NULL;
  int got_to=0,i;

  hand=fopen(name,"rb");
  if (hand==NULL)
    return -1;

  fseek(hand,0,SEEK_END);
  file_size=ftell(hand);
  fseek(hand,0,SEEK_SET);

  if (fread(head,1,512,hand)!=512) { fclose(hand); return -1; }

  //chunk_count=head[0];
  // Sometimes header is wrong apparently

  file_chunks=((file_size-512)/16384);

  chunk_count=file_chunks;

  //if (chunk_count>file_chunks) chunk_count=file_chunks;

  if (into==NULL){ fclose(hand); return (chunk_count*16384); }

  chunk_buf=malloc(16384);
  if (chunk_buf==NULL)
    {printf ("out of mem\n"); fclose(hand); return -1;}

  for (got_to=0,i=0; i<chunk_count; i++,got_to+=16384)
  {
    int j;
    // Deinterleave each chunk
    fread(chunk_buf,1,16384,hand);
    for (j=0;j<8192;j++)
      into[got_to+(j<<1)+1]=chunk_buf[j];
    for (j=0;j<8192;j++)
      into[got_to+(j<<1)+0]=chunk_buf[j+8192];
  }

  free(chunk_buf);
  fclose(hand);

  return 0;
}

// If 'into' is NULL returns rom size, otherwise expect
// 'into' to be a buffer big enough for the rom size
// (i.e. pass NULL, malloc, pass pointer, emulate, free pointer)

int load_rom_into(char *name,unsigned char *into)
{
  int format=0; // bin 0, smd 1
  int len;
  unsigned char magicbuf[10];
  FILE *romfile;
  if (name==NULL) return -1;

  /* Open the file and get the first little shnippit of it so we can check
   * the magic numbers on it. */
  if(!(romfile = fopen(name, "rb"))) return -4;
  fread(magicbuf, 10, 1, romfile);
  fclose(romfile);

  /* Check for the magic on various gzip-supported compressions */
  if((magicbuf[0] == 037 && magicbuf[1] == 036)  || /* compress'd (.Z)     */
     (magicbuf[0] == 037 && magicbuf[1] == 0213) || /* gzipped    (.gz)    */
     (magicbuf[0] == 037 && magicbuf[1] == 0236) || /* frozen     (.f,.z)  */
     (magicbuf[0] == 037 && magicbuf[1] == 0240) || /* LZH        (?)      */
     (magicbuf[0] == 'P' && magicbuf[1] == 'K'))    /* ZIP        (.zip ;) */
    {
      char temp[0x100], temp2[0x80];
      srand(time(NULL));
      /* Run it through gzip (I know this is cheap ;) */
      sprintf(temp2, "/tmp/dgenrom_%d_%d", rand(), rand());
      sprintf(temp, "gzip -S \"\" -cdq %s > %s", name, temp2);
      /* If gzip returned an error, stop */
      if(system(temp)) { remove(temp2); return -1; };
      /* Recurse with the new file */
      len = load_rom_into(temp2, into);
      remove(temp2);
      sync();
      return len;
    }
  /* Do bzip2 also */
  if(magicbuf[0] == 'B' && magicbuf[1] == 'Z' && magicbuf[2] == 'h')
    {
      /* Damn, this looks almost like the gzip stuff above. *lol* :) */
      char temp[0x100], temp2[0x80];
      srand(time(NULL));
      /* Go through bzip2 */
      sprintf(temp2, "/tmp/dgenrom_%d_%d", rand(), rand());
      sprintf(temp, "bzip2 -cd %s > %s", name, temp2);
      /* If we got an error, stop */
      if(system(temp)) { remove(temp2); return -1; };
      /* Recurse with the uncompressed file */
      len = load_rom_into(temp2, into);
      remove(temp2);
      sync();
      return len;
  }

  /* Next check for SMD magic */
  if(magicbuf[8] == 0xaa && magicbuf[9] == 0xbb)
    format = 1;
  /* Otherwise we can only hope it's binary */
  else format = 0;
	    
  switch (format)
  {
    case 1:  return load_smd_into(name,into);
    default: return load_bin_into(name,into);
  }
  return -3;
}

#endif //_PSP10 /////////////////////////////////////////////////////////////////////////
