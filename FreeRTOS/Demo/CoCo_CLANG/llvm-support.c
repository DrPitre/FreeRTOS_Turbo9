#include <stddef.h>

void *memcpy( void *dest, const void *src, size_t n )
{
    unsigned char *d = ( unsigned char * ) dest;
    const unsigned char *s = ( const unsigned char * ) src;

    while( n-- != 0 )
    {
        *d++ = *s++;
    }

    return dest;
}

void *memset( void *s, int c, size_t n )
{
    unsigned char *p = ( unsigned char * ) s;

    while( n-- != 0 )
    {
        *p++ = ( unsigned char ) c;
    }

    return s;
}

int printf( const char *format, ... )
{
    ( void ) format;
    return 0;
}
