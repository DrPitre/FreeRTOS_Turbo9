/*
 * FreeRTOS V202212.01
 * Copyright (C) 2020 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <cmoc.h>
#include "FreeRTOS.h"
#include "task.h"
#include "cpu.h"

static volatile unsigned char * const coco_screen = ( volatile unsigned char * ) 0x0400;

static unsigned char coco_screen_char( char c )
{
    if( c == ' ' )
    {
        return 0x60;
    }

    return ( unsigned char ) c;
}

static void coco_screen_clear( void )
{
    unsigned short i;

    for( i = 0; i < 512; i++ )
    {
        coco_screen[ i ] = 0x60;
    }
}

static void coco_screen_puts_at( unsigned char row, const char *text )
{
    volatile unsigned char *p = coco_screen + ( row * 32 );
    unsigned char col;

    for( col = 0; col < 32; col++ )
    {
        p[ col ] = 0x60;
    }

    col = 0;
    while( ( *text != '\0' ) && ( col < 32 ) )
    {
        p[ col ] = coco_screen_char( *text );
        col++;
        text++;
    }
}

static void coco_screen_put_hex_at( unsigned char row, unsigned char col, unsigned char value )
{
    static const char hex[] = "0123456789ABCDEF";
    volatile unsigned char *p = coco_screen + ( row * 32 ) + col;

    p[ 0 ] = hex[ ( value >> 4 ) & 0x0f ];
    p[ 1 ] = hex[ value & 0x0f ];
}

static void terminal_task( void *parameters )
{
    unsigned char ticks = 0;

    ( void ) parameters;

    coco_screen_puts_at( 2, "TASK STARTED" );
    coco_screen_puts_at( 3, "HEARTBEAT 00" );

    while( 1 )
    {
        coco_screen_put_hex_at( 3, 10, ticks );
        ticks++;

        vTaskDelay( 60 );
    }
}

int ATTR_BANK0 main( void )
{
    TaskHandle_t task;
    BaseType_t ret;

    coco_screen_clear();
    coco_screen_puts_at( 0, "FREERTOS COCO MAIN" );

    ret = xTaskCreate( terminal_task,
                       "TM",
                       256,
                       NULL,
                       1,
                       ( TaskHandle_t * ) &task );

    if( ret != pdPASS )
    {
        coco_screen_puts_at( 1, "XTASKCREATE FAILED" );
        for( ;; );
    }

    coco_screen_puts_at( 1, "STARTING SCHEDULER" );
    vTaskStartScheduler();

    coco_screen_puts_at( 4, "SCHEDULER RETURNED" );
    for( ;; );
    return 0;
}
