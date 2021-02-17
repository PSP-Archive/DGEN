#include "zlibFileMemory.h"
#include "zlibType.h"
#include "zconf.h"
#include "syscall.h"

void ___itoa(int val, char *s)
{
	char *t;
	int mod;

	if(val < 0) {
		*s++ = '-';
		val = -val;
	}
	t = s;

	while(val) {
		mod = val % 10;
		*t++ = (char)mod + '0';
		val /= 10;
	}

	if(s == t)
		*t++ = '0';

	*t = '\0';

	___strrev(s);

}

void ___itoa32(unsigned long val, char *s) {
	char *t;
	unsigned long mod;

	if(val < 0) {
		*s++ = '-';
		val = -val;
	}
	t = s;

	while(val) {
		mod = val % 10;
		*t++ = (char)mod + '0';
		val /= 10;
	}

	if(s == t)
		*t++ = '0';

	*t = '\0';

	___strrev(s);
}
void* ___memcpy(void *buf1, const void *buf2, int n)
{
	while(n-->0)
		((unsigned char*)buf1)[n] = ((unsigned char*)buf2)[n];
	return buf1;
}

int ___strcmp(const char *str1, const char *str2)
{
	char c1, c2;
	for(;;){
		c1 = *str1;
		c2 = *str2;

		if(c1!=c2)
			return 1;
		else if(c1==0)
			return 0;

		str1++; str2++;
	}
}

size_t ___strlen(const char *s)
{
	int ret;

	for(ret=0; s[ret]; ret++)
		;

	return ret;
}

char* ___strcpy(char *dest, const char *src)
{
	int i;

	for(i=0; src[i]; i++)
		dest[i] = src[i];
	dest[i] = 0;

	return dest;
}

void ___strrev(char *s){
	char tmp;
	int i;
	int len = ___strlen(s);

	for(i=0; i<len/2; i++){
		tmp = s[i];
		s[i] = s[len-1-i];
		s[len-1-i] = tmp;
	}
}

char* ___strcat(char *dest, const char *src)
{
	int i;
	int len;

	len=___strlen(dest);
	for(i=0; src[i]; i++)
		dest[len+i] = src[i];
	dest[len+i] = 0;

	return dest;
}

//FILE ___zlibfb;
#define MAX_PATH          260
#define AllocBuffSize     65536

//unsigned long long ___zlibTmpBuffSize;

//unsigned char *___zlibTmpBuff;	//作業用。入力zipをここに展開する
//unsigned int  ___zlibInZipBuffPointer;		//上記メモリをどこまで使ったか
//unsigned char ___zlibInZipFileName[MAX_PATH];	//作業用。入力zipをここに展開する
//unsigned int  ___zlibInZipSize;			//入力zipの大きさ[バイト]

typedef struct {
	int nFileId;			// file id
	unsigned long ulPtr;	// file pointer
}zlibInFile;
unsigned char ___zlibAllocBuff[AllocBuffSize];	//メモリの確保
unsigned int  ___zlibAllocBuffPointer;	//上記メモリをどこまで使ったか

zlibInFile ___zlibInFile = {0, 0};

FILE *___fopen( const char *filename, const char *mode )
{
	int fb1;

	fb1 = sceIoOpen(filename,O_RDONLY,0777);
	if(fb1 < 0)
	{
		return NULL;
	}
	___zlibInFile.nFileId = fb1;
	___zlibInFile.ulPtr = 0;
	___zlibAllocBuffPointer = 0;
	return (FILE *)&___zlibInFile;
}

size_t ___fwrite( const void*buffer, size_t size, size_t count, FILE *stream )
{

	return 0;
}

long ___ftell( FILE *stream )
{
	return ___zlibInFile.ulPtr;
}

int ___fseek( FILE *stream, long offset, int origin )
{
	unsigned long ulPtr;
	ulPtr = sceIoLseek(___zlibInFile.nFileId, offset, origin);
	___zlibInFile.ulPtr = ulPtr;
	return ulPtr;
}

int ___fclose( FILE *stream )
{
	sceIoClose(___zlibInFile.nFileId);
	return 0;
}

int ___ferror( FILE *stream )
{

	return 0;
}

size_t ___fread( void *buffer, size_t size, size_t count, FILE *stream )
{
	int ret = 0;
	int readSize = size * count;

	readSize = sceIoRead(___zlibInFile.nFileId, buffer, readSize);
	___zlibInFile.ulPtr += readSize;

	return readSize;
}

void ___free( void *memblock )
{
	return;
}

void *___calloc( size_t num, size_t size )
{
	return NULL;
}

void *___malloc( size_t size )
{
	___zlibAllocBuffPointer += size;

	if(___zlibAllocBuffPointer > AllocBuffSize)
	{
		// メモリ不足
		return NULL;
	}

	return &___zlibAllocBuff[___zlibAllocBuffPointer - size];
}


