////////////////////////////////////////////////////////
/// psp.c
////////////////////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <string.h>
#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include "psp_main.h"
////////////////////////////////////////////////////////

/*
#define MAX_MALLOC (100)
int   malloc_id [ MAX_MALLOC ] ;
void* malloc_ptr[ MAX_MALLOC ] ;

void* malloc( size_t size )
{
	int n ;
	char name[] = "00_malloc" ;

	for( n=0; n < MAX_MALLOC ; n++ )
	{
		if( malloc_id[n] == 0 )
		{
			//name[0] = '0' + n / 10 ;
			//name[1] = '0' + n % 10 ;
			malloc_id[n] = sceKernelAllocPartitionMemory( 2, name, 0, size, NULL ) ;

			if( malloc_id[n] )
			{
				malloc_ptr[n] = (void*)sceKernelGetBlockHeadAddr( malloc_id[n] ) ;
				return malloc_ptr[n] ;
			}
		}
	}

	return 0 ;
}

void *calloc( size_t num, size_t size )
{
	void* p ;

	p = malloc( num * size ) ;
	if( p )
	{
		memset( p, 0, num * size ) ;
		return p ;
	}

	return 0 ;
}

void free(void *ptr)
{
	int n ;

	for( n=0; n < MAX_MALLOC ; n++ )
	{
		if( malloc_ptr[n] == ptr )
		{
			sceKernelFreePartitionMemory( malloc_id[n] ) ;
			malloc_id [n] = 0 ;
			malloc_ptr[n] = 0 ;
		}
	}
}
//*/

#ifndef _USE_PSPSDK

void *memcpy(void *dest, const void *src, size_t n)
{
	int i ;

	for ( i = 0; i < n; i++ )
	{
		((char*)dest)[i] = ((char*)src)[i];
	}
}

void *memset(void *buf, int ch, size_t n)
{
	int i ;

	for ( i = 0; i < n; i++ )
	{
		((char*)buf)[i] = (char)ch;
	}
}

int memcmp( const void *buf1, const void *buf2, size_t count )
{
	int ret;
	int i;
	
	for(i=0; i<count; i++){
		ret = ((unsigned char*)buf1)[i] - ((unsigned char*)buf2)[i];
		if(ret!=0)
			return ret;
	}
	return 0;
}


int strcmp( const char *str1, const char *str2 )
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

	return -1 ;
}

int stricmp(const char *str1, const char *str2)
{
	char c1, c2;
	for(;;){
		c1 = *str1;
		if(c1>=0x61 && c1<=0x7A) c1-=0x20;
		c2 = *str2;
		if(c2>=0x61 && c2<=0x7A) c2-=0x20;
		
		if(c1!=c2)
			return 1;
		else if(c1==0)
			return 0;
		
		str1++; str2++;
	}

	return 1;
}

size_t strlen( const char *string )
{
	int ret;
	
	for(ret=0; string[ret]; ret++)
		;
	
	return ret;
}

char *strcpy( char *dest, const char *src )
{
	int i;
	
	for(i=0; src[i]; i++)
		dest[i] = src[i];
	dest[i] = 0;
	
	return dest;
}

char *strncpy( char *dest, const char *src, size_t count )
{
	int		i;

	for ( i = 0; i < count; i++ )
	{
		dest[i] = src[i];
	}
	dest[i] = 0;

	return dest;
}

char* strcat(char *dest, const char *src)
{
	int i;
	int len;
	
	len=strlen(dest);
	for(i=0; src[i]; i++)
		dest[len+i] = src[i];
	dest[len+i] = 0;
	
	return dest;
}

char* strrchr(const char *src, int c)
{
	int len;
	
	len=strlen(src);
	while(len>0){
		len--;
		if(*(src+len) == c)
			return (char*)(src+len);
	}
	
	return NULL;
}

int fprintf( FILE *stream, const char *format, ... )
{
	return 0 ;
}

int sprintf ( char *string, const char *format, ... )
{
	return 0 ;
}

int printf ( const char *format, ... )
{
	return 0 ;
}

int puts( const char *string )
{
	return 0 ;
}

///////////////////////////////////////////////////
/*
#define PSP_O_RDONLY	0x0001 
#define PSP_O_WRONLY	0x0002 
#define PSP_O_RDWR		0x0003 
#define PSP_O_NBLOCK	0x0010 
#define PSP_O_APPEND	0x0100 
#define PSP_O_CREAT		0x0200 
#define PSP_O_TRUNC		0x0400 
#define PSP_O_NOWAIT	0x8000 

FILE* fopen(const char *path, const char *mode)
{
	int		psp_mode;
	int		fd;

	switch ( mode[0] ){
	case 'r':
		psp_mode = PSP_O_RDONLY;
		break;

	case 'w':
		psp_mode = PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC;
		break;

	case 'a':
		psp_mode = PSP_O_RDWR | PSP_O_APPEND;
		break;
	}

	fd = sceIoOpen( path, psp_mode, 0777 );
	if ( fd < 0 ){
		return NULL;
	}

	return (FILE*)fd;
}

int fclose(FILE *fp)
{
	sceIoClose( (int)fp );

	return 0;
}

int fgetc(FILE *fp)
{
	char	c;

	sceIoRead( (int)fp, &c, sizeof(char) );

	return (int)c;
}

size_t fread(void *buf, size_t size, size_t n, FILE *fp)
{
	int		ret;

	ret = sceIoRead( (int)fp, buf, size * n );

	return ret / size;
}

size_t fwrite(const void *buf, size_t size, size_t n, FILE *fp)
{
	sceIoWrite( (int)fp, (void*)buf, size * n );

	return n;
}
*/

/*
int _fseek( FILE *fp, long long offset, int origin )
{
	return sceIoLseek( (int)fp, offset, origin );
}
*/

/*
long _ftell( FILE *stream )
{
	return 1024 * 1024 ;
}
*/

/*
long _flength( FILE* fp )
{
	long ret = 0 ;
	_fseek( fp, 0, SEEK_SET ) ;
	ret = _fseek( fp, 0, SEEK_END ) ;
	_fseek( fp, 0, SEEK_SET ) ;
	return ret ;
}
//*/

/*
#define BUFF_FLENGTH  (128*1024)
unsigned char flength_buff[ BUFF_FLENGTH ] ;

//long _flength( FILE* fp )
long _flength( const char* pPath )
{
	int rd, rd_all = 0 ;
	FILE* fp ;

	fp = (FILE*)sceIoOpen( pPath, PSP_O_RDONLY, 0777 );
	if( fp < 0 ){ return 0 ; }

//	sceIoLseek( fp, 0, SEEK_SET ) ;

	while(1)
	{
		rd = sceIoRead( (int)fp, flength_buff, BUFF_FLENGTH );
		rd_all += rd ;

		if( rd < BUFF_FLENGTH ){ break ; }
	}

//	sceIoLseek( fp, 0, SEEK_SET ) ;
	sceIoClose( (int)fp );

	return rd_all ;
}
//*/

/*
long _flength( const char* pPath )
{
	PSP_FILE_ATTRIBUTE attr ;
	memset( &attr, 0, sizeof(PSP_FILE_ATTRIBUTE) ) ;

	int h = sceIoDopen( pPath ) ;

	sceIoDread( h, &attr ) ;

	sceIoDclose( h ) ;

	return attr.size ;
}
*/

///////////////////////////////////////////////////

double pow( double x, double y )
{
	return 1.0 ;
}

double sin( double x )
{
	return 1.0 ;
}

double log10( double x )
{
	return 1.0 ;
}

/*
///////////////////////////////////////////////////
#define _ZLIB
#ifndef _ZLIB
///////////////////////////////////////////////////
#define PSP_O_RDONLY	0x0001 
#define PSP_O_WRONLY	0x0002 
#define PSP_O_RDWR		0x0003 
#define PSP_O_NBLOCK	0x0010 
#define PSP_O_APPEND	0x0100 
#define PSP_O_CREAT		0x0200 
#define PSP_O_TRUNC		0x0400 
#define PSP_O_NOWAIT	0x8000 

FILE *fopen(const char *path, const char *mode)
{
	int		psp_mode;
	int		fd;

	switch ( mode[0] ){
	case 'r':
		psp_mode = PSP_O_RDONLY;
		break;

	case 'w':
		psp_mode = PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC;
		break;

	case 'a':
		psp_mode = PSP_O_RDWR | PSP_O_APPEND;
		break;
	}

	fd = sceIoOpen( path, psp_mode, 0777 );
	if ( fd < 0 ){
		return NULL;
	}

	return (FILE*)fd;
}

int fclose(FILE *fp)
{
	sceIoClose( (int)fp );

	return 0;
}

int fgetc(FILE *fp)
{
	char	c;

	sceIoRead( (int)fp, &c, sizeof(char) );

	return (int)c;
}

size_t fread(void *buf, size_t size, size_t n, FILE *fp)
{
	int		ret;

	ret = sceIoRead( (int)fp, buf, size * n );

	return ret / size;
}

size_t fwrite(const void *buf, size_t size, size_t n, FILE *fp)
{
	sceIoWrite( (int)fp, (void*)buf, size * n );

	return n;
}
///////////////////////////////////////////////////
#endif // _ZLIB
///////////////////////////////////////////////////
*/

////////////////////////////////////////////////////////

/* prototypes for local routines */
static void shortsort(char *lo, char *hi, unsigned width,
                int (*comp)(const void *, const void *));
static void swap(char *p, char *q, unsigned int width);

/* this parameter defines the cutoff between using quick sort and
   insertion sort for arrays; arrays with lengths shorter or equal to the
   below value use insertion sort */

#define CUTOFF 8            /* testing shows that this is good value */


/***
*qsort(base, num, wid, comp) - quicksort function for sorting arrays
*
*Purpose:
*       quicksort the array of elements
*       side effects:  sorts in place
*
*Entry:
*       char *base = pointer to base of array
*       unsigned num  = number of elements in the array
*       unsigned width = width in bytes of each array element
*       int (*comp)() = pointer to function returning analog of strcmp for
*               strings, but supplied by user for comparing the array elements.
*               it accepts 2 pointers to elements and returns neg if 1<2, 0 if
*               1=2, pos if 1>2.
*
*Exit:
*       returns void
*
*Exceptions:
*
*******************************************************************************/

/* sort the array between lo and hi (inclusive) */

void qsort (
    void *base,
    unsigned num,
    unsigned width,
    int (*comp)(const void *, const void *)
    )
{
    char *lo, *hi;              /* ends of sub-array currently sorting */
    char *mid;                  /* points to middle of subarray */
    char *loguy, *higuy;        /* traveling pointers for partition step */
    unsigned size;              /* size of the sub-array */
    char *lostk[30], *histk[30];
    int stkptr;                 /* stack for saving sub-array to be processed */

    /* Note: the number of stack entries required is no more than
       1 + log2(size), so 30 is sufficient for any array */

    if (num < 2 || width == 0)
        return;                 /* nothing to do */

    stkptr = 0;                 /* initialize stack */

    lo = base;
    hi = (char *)base + width * (num-1);        /* initialize limits */

    /* this entry point is for pseudo-recursion calling: setting
       lo and hi and jumping to here is like recursion, but stkptr is
       prserved, locals aren't, so we preserve stuff on the stack */
recurse:

    size = (hi - lo) / width + 1;        /* number of el's to sort */

    /* below a certain size, it is faster to use a O(n^2) sorting method */
    if (size <= CUTOFF) {
         shortsort(lo, hi, width, comp);
    }
    else {
        /* First we pick a partititioning element.  The efficiency of the
           algorithm demands that we find one that is approximately the
           median of the values, but also that we select one fast.  Using
           the first one produces bad performace if the array is already
           sorted, so we use the middle one, which would require a very
           wierdly arranged array for worst case performance.  Testing shows
           that a median-of-three algorithm does not, in general, increase
           performance. */

        mid = lo + (size / 2) * width;      /* find middle element */
        swap(mid, lo, width);               /* swap it to beginning of array */

        /* We now wish to partition the array into three pieces, one
           consisiting of elements <= partition element, one of elements
           equal to the parition element, and one of element >= to it.  This
           is done below; comments indicate conditions established at every
           step. */

        loguy = lo;
        higuy = hi + width;

        /* Note that higuy decreases and loguy increases on every iteration,
           so loop must terminate. */
        for (;;) {
            /* lo <= loguy < hi, lo < higuy <= hi + 1,
               A[i] <= A[lo] for lo <= i <= loguy,
               A[i] >= A[lo] for higuy <= i <= hi */

            do  {
                loguy += width;
            } while (loguy <= hi && comp(loguy, lo) <= 0);

            /* lo < loguy <= hi+1, A[i] <= A[lo] for lo <= i < loguy,
               either loguy > hi or A[loguy] > A[lo] */

            do  {
                higuy -= width;
            } while (higuy > lo && comp(higuy, lo) >= 0);

            /* lo-1 <= higuy <= hi, A[i] >= A[lo] for higuy < i <= hi,
               either higuy <= lo or A[higuy] < A[lo] */

            if (higuy < loguy)
                break;

            /* if loguy > hi or higuy <= lo, then we would have exited, so
               A[loguy] > A[lo], A[higuy] < A[lo],
               loguy < hi, highy > lo */

            swap(loguy, higuy, width);

            /* A[loguy] < A[lo], A[higuy] > A[lo]; so condition at top
               of loop is re-established */
        }

        /*     A[i] >= A[lo] for higuy < i <= hi,
               A[i] <= A[lo] for lo <= i < loguy,
               higuy < loguy, lo <= higuy <= hi
           implying:
               A[i] >= A[lo] for loguy <= i <= hi,
               A[i] <= A[lo] for lo <= i <= higuy,
               A[i] = A[lo] for higuy < i < loguy */

        swap(lo, higuy, width);     /* put partition element in place */

        /* OK, now we have the following:
              A[i] >= A[higuy] for loguy <= i <= hi,
              A[i] <= A[higuy] for lo <= i < higuy
              A[i] = A[lo] for higuy <= i < loguy    */

        /* We've finished the partition, now we want to sort the subarrays
           [lo, higuy-1] and [loguy, hi].
           We do the smaller one first to minimize stack usage.
           We only sort arrays of length 2 or more.*/

        if ( higuy - 1 - lo >= hi - loguy ) {
            if (lo + width < higuy) {
                lostk[stkptr] = lo;
                histk[stkptr] = higuy - width;
                ++stkptr;
            }                           /* save big recursion for later */

            if (loguy < hi) {
                lo = loguy;
                goto recurse;           /* do small recursion */
            }
        }
        else {
            if (loguy < hi) {
                lostk[stkptr] = loguy;
                histk[stkptr] = hi;
                ++stkptr;               /* save big recursion for later */
            }

            if (lo + width < higuy) {
                hi = higuy - width;
                goto recurse;           /* do small recursion */
            }
        }
    }

    /* We have sorted the array, except for any pending sorts on the stack.
       Check if there are any, and do them. */

    --stkptr;
    if (stkptr >= 0) {
        lo = lostk[stkptr];
        hi = histk[stkptr];
        goto recurse;           /* pop subarray from stack */
    }
    else
        return;                 /* all subarrays done */
}


/***
*shortsort(hi, lo, width, comp) - insertion sort for sorting short arrays
*
*Purpose:
*       sorts the sub-array of elements between lo and hi (inclusive)
*       side effects:  sorts in place
*       assumes that lo < hi
*
*Entry:
*       char *lo = pointer to low element to sort
*       char *hi = pointer to high element to sort
*       unsigned width = width in bytes of each array element
*       int (*comp)() = pointer to function returning analog of strcmp for
*               strings, but supplied by user for comparing the array elements.
*               it accepts 2 pointers to elements and returns neg if 1<2, 0 if
*               1=2, pos if 1>2.
*
*Exit:
*       returns void
*
*Exceptions:
*
*******************************************************************************/

static void shortsort (
    char *lo,
    char *hi,
    unsigned width,
    int (*comp)(const void *, const void *)
    )
{
    char *p, *max;

    /* Note: in assertions below, i and j are alway inside original bound of
       array to sort. */

    while (hi > lo) {
        /* A[i] <= A[j] for i <= j, j > hi */
        max = lo;
        for (p = lo+width; p <= hi; p += width) {
            /* A[i] <= A[max] for lo <= i < p */
            if (comp(p, max) > 0) {
                max = p;
            }
            /* A[i] <= A[max] for lo <= i <= p */
        }

        /* A[i] <= A[max] for lo <= i <= hi */

        swap(max, hi, width);

        /* A[i] <= A[hi] for i <= hi, so A[i] <= A[j] for i <= j, j >= hi */

        hi -= width;

        /* A[i] <= A[j] for i <= j, j > hi, loop top condition established */
    }
    /* A[i] <= A[j] for i <= j, j > lo, which implies A[i] <= A[j] for i < j,
       so array is sorted */
}


/***
*swap(a, b, width) - swap two elements
*
*Purpose:
*       swaps the two array elements of size width
*
*Entry:
*       char *a, *b = pointer to two elements to swap
*       unsigned width = width in bytes of each array element
*
*Exit:
*       returns void
*
*Exceptions:
*
*******************************************************************************/

static void swap (
    char *a,
    char *b,
    unsigned width
    )
{
    char tmp;

    if ( a != b )
        /* Do the swap one character at a time to avoid potential alignment
           problems. */
        while ( width-- ) {
            tmp = *a;
            *a++ = *b;
            *b++ = tmp;
        }
}

////////////////////////////////////////////////////////



char cwd[260]={0};

char *getcwd(char *buf, size_t size)
{
	strcpy(buf,cwd);
	return buf;
}

int chdir(const char *path)
{
	int ret = sceIoChdir(path);
	if(ret==0) strcpy(cwd,path);
	return ret;
}

void _exit(int status)
{
	for(;;)
		sceKernelExitGame();
}

int unlink(const char *pathname)   
{
	return sceIoRemove(pathname);
}

static inline fdcnv(int fd)
{
	switch(fd) {
	case 0:
		return sceKernelStdin();
	case 1:
		return sceKernelStdout();
	case 2:
		return sceKernelStderr();
	default:
		return fd;
	}
}

int open(const char *pathname, int flags, ...)
{
	return sceIoOpen(pathname, flags, 0644);
}

int close(int fd)
{
	fd = fdcnv(fd);
	if(fd<0) return -1;
	return sceIoClose(fd);
}

long read(int fd, void *buf, size_t count)
{
	fd = fdcnv(fd);
	if(fd<0) return -1;
	return sceIoRead(fd, buf, count);
}

long write(int fd, const void *buf, size_t count)
{
	fd = fdcnv(fd);
	if(fd<0) return -1;
	return sceIoWrite(fd, (void*)buf, count);
}

off_t lseek(int fd, off_t offset, int dir)
{
	fd = fdcnv(fd);
	if(fd<0) return -1;
	return sceIoLseek(fd, offset, dir);
}

int fstat(int fd, struct stat *st)
{
	int cur = sceIoLseek(fd,0,SEEK_CUR);
	int size = sceIoLseek(fd,0,SEEK_END);
	sceIoLseek(fd,cur,SEEK_SET);
	
	memset(st,0,sizeof(*st));
	st->st_mode = S_IFCHR;
	st->st_size = size;
	return 0;
}

int stat(const char *file_name, struct stat *buf)
{
	//struct dirent de;
	//sceIoGetStat(file_name, &de);
	return -1;
}

int isatty(int desc)
{
	return 0;
}

#define SBRK_SIZE 1*1024*1024
static char alloc_buffer[SBRK_SIZE];
void* sbrk(ptrdiff_t incr)
{
	extern char end;
	static char *heap_end = alloc_buffer;
	static int total;
	char *prev_heap_end;
	
	if(heap_end < alloc_buffer+SBRK_SIZE){
		prev_heap_end = heap_end;
		heap_end += incr;
		total += incr;
		return (caddr_t) prev_heap_end;
	}else{
		return (caddr_t) -1;
	}
}

int kill(int pid, int sig)
{
	return -1;
}

int getpid(void)
{
	return 1;
}

time_t time(time_t *tm)
{
	return sceKernelLibcTime((unsigned long*)tm);
}

#else
double pow( double x, double y )
{
	return 1.0 ;
}


#endif // _USE_PSPSDK
