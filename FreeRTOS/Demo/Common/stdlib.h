#ifndef FREERTOS_DEMO_STDLIB_H
#define FREERTOS_DEMO_STDLIB_H
#ifdef CMOC_FREERTOS
#include <cmoc.h>
#else
#include <stddef.h>
void *malloc( size_t size );
void free( void *ptr );
#endif
#endif
