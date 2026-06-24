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

static void coco_task( void *parameters )
{
    ( void ) parameters;

    printf( "FreeRTOS CoCo demo starting\n" );

    while( 1 )
    {
        printf( "FreeRTOS CoCo task heartbeat\n" );
        vTaskDelay( 60 );
    }
}

int ATTR_BANK0 main( void )
{
    TaskHandle_t task;
    BaseType_t ret;

    ret = xTaskCreate( coco_task,
                       "COCO",
                       256,
                       NULL,
                       1,
                       ( TaskHandle_t * ) &task );

    if( ret != pdPASS )
    {
        printf( "xTaskCreate failed\n" );
        for( ;; );
    }

    vTaskStartScheduler();

    for( ;; );
    return 0;
}
