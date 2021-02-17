#ifndef ___ZLIBFILEMEMORY
#define ___ZLIBFILEMEMORY

#include "zlibType.h"
#include "zlibFileMemory.h"

extern void ___itoa(int val, char *s);
extern void ___itoa32(unsigned long val, char *s) ;
extern void* ___memcpy(void *buf1, const void *buf2, int n);
extern int ___strcmp(const char *str1, const char *str2);
extern size_t ___strlen(const char *s);
extern char* ___strcpy(char *dest, const char *src);
extern void ___strrev(char *s);
extern char* ___strcat(char *dest, const char *src);
extern void ___DrawStr(char *c);
extern void ___DrawInt(int i);
extern FILE *___fopen( const char *filename, const char *mode );
extern size_t ___fwrite( const void*buffer, size_t size, size_t count, FILE *stream );
extern long ___ftell( FILE *stream );
extern int ___fclose( FILE *stream );
extern int ___ferror( FILE *stream );
extern size_t ___fread( void *buffer, size_t size, size_t count, FILE *stream );
extern void ___free( void *memblock );
extern void *___calloc( size_t num, size_t size );
extern void *___malloc( size_t size );
#endif //___ZLIBFILEMEMORY

