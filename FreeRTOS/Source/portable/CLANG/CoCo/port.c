/*
 * FreeRTOS Kernel V10.5.1
 * Copyright (C) 2021 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * SPDX-License-Identifier: MIT
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
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * https://www.FreeRTOS.org
 * https://github.com/FreeRTOS
 *
 */

/* CLANG/CoCo port, derived from the CMOC/CoCo port. */

/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"

/*-----------------------------------------------------------
 * Implementation of functions defined in portable.h for the CoCo port.
 *----------------------------------------------------------*/


/*
 * Configure a timer to generate the RTOS tick at the frequency specified
 * within FreeRTOSConfig.h.
 */
static void prvSetupTimerInterrupt( void );

/* NOTE: Interrupt service routines must be in non-banked memory - as does the
scheduler startup function. */
#define ATTR_NEAR
#define ATTR_NAKED __attribute__((naked))

/* Manual context switch function.  This is the SWI ISR. */
// __attribute__((interrupt))
void ATTR_NEAR ATTR_NAKED vPortYield( void );
void ATTR_NEAR vPortYieldHandler( void );

/* Tick context switch function.  This is the timer ISR. */
// __attribute__((interrupt))
void ATTR_NEAR ATTR_NAKED vPortTickInterrupt( void );
void ATTR_NEAR vPortTickInterruptHandler( void );

/* Function in non-banked memory which actually switches to first task. */
BaseType_t ATTR_NEAR xStartSchedulerNear( void );

/* Calls to portENTER_CRITICAL() can be nested.  When they are nested the
critical section should not be left (i.e. interrupts should not be re-enabled)
until the nesting depth reaches 0.  This variable simply tracks the nesting
depth.  Each task maintains it's own critical nesting depth variable so
uxCriticalNesting is saved and restored from the task stack during a context
switch. */
volatile UBaseType_t uxCriticalNesting = 0x80;  // un-initialized

/*-----------------------------------------------------------*/

/*
 * See header file for description.
 */
StackType_t *pxPortInitialiseStack( StackType_t *pxTopOfStack, TaskFunction_t pxCode, void *pvParameters )
{
	/* Build the initial 6809 interrupt frame that portRESTORE_CONTEXT + RTI
	   will consume when the task first runs.  Frame layout from [S] upward:

	     [S+0]  CC        [S+1]  A        [S+2]  B        [S+3]  DP
	     [S+4..5]  X      [S+6..7]  Y     [S+8..9]  U     [S+10..11]  PC

	   One extra byte below [S] holds the critical-nesting count, which
	   portRESTORE_CONTEXT pops (via PULS A) before issuing RTI.

	   Bytes are pushed from highest address down; the last push becomes [S]
	   after portRESTORE_CONTEXT pops the nesting count. */

	/* PC: big-endian - MSB at lower address ([S+10]), LSB at higher ([S+11]). */
	*pxTopOfStack   = ( StackType_t ) *( ((StackType_t *) (&pxCode) ) + 1 ); /* PC_LSB */
	*--pxTopOfStack = ( StackType_t ) *( ((StackType_t *) (&pxCode) ) + 0 ); /* PC_MSB */

	/* U, Y, X: debug sentinels only - values do not affect task execution. */
	*--pxTopOfStack = ( StackType_t ) 0xff;
	*--pxTopOfStack = ( StackType_t ) 0xee;

	*--pxTopOfStack = ( StackType_t ) 0xff;
	*--pxTopOfStack = ( StackType_t ) 0xee;

	*--pxTopOfStack = ( StackType_t ) 0xdd;
	*--pxTopOfStack = ( StackType_t ) 0xcc;

	/* D (A:B) = pvParameters.  CMOC passes the first 16-bit argument in D
	   where A holds the MSB and B holds the LSB.  RTI restores A from [S+1]
	   and B from [S+2], so they must be pushed in the order: DP, B, A
	   (DP lands at [S+3], B at [S+2], A at [S+1]). */
	*--pxTopOfStack = ( StackType_t ) 0x00;                                         /* DP */
	*--pxTopOfStack = ( StackType_t ) *( ((StackType_t *) (&pvParameters) ) + 1 );  /* B = LSB */
	*--pxTopOfStack = ( StackType_t ) *( ((StackType_t *) (&pvParameters) ) + 0 );  /* A = MSB */

	/* CCR: S bit set (stop disabled), I bit clear (IRQ enabled on task start). */
	*--pxTopOfStack = ( StackType_t ) 0x80;

	/* Critical nesting depth 0: not in a critical section. */
	*--pxTopOfStack = ( StackType_t ) 0x00;

	return pxTopOfStack;
}
/*-----------------------------------------------------------*/

void vPortEndScheduler( void )
{
	/* It is unlikely that the port will get stopped. */
}
/*-----------------------------------------------------------*/

static void (*capturedIRQAddress)();
static void (*capturedSWIAddress)();

static void prvSetupTimerInterrupt( void )
{
    /* piggy back off of the CoCo BASIC timer */
    void ( **orgIRQAddress )( void ) = ( void ( ** )( void ) ) 0x10D;
    void ( **orgSWIAddress )( void ) = ( void ( ** )( void ) ) 0x107;

    volatile char *orgSWIVector = ( volatile char * ) 0x106;

 portDISABLE_INTERRUPTS()
    /* save off original IRQ address */
    capturedIRQAddress = *orgIRQAddress;
    capturedSWIAddress = *orgSWIAddress;

    *orgIRQAddress = vPortTickInterrupt;
    *orgSWIVector = 0x7E;
    *orgSWIAddress = vPortYield;
}

/*-----------------------------------------------------------*/
BaseType_t xPortStartScheduler( void )
{
	/* Configure the timer that will generate the RTOS tick.  Interrupts are
	disabled when this function is called. */
	prvSetupTimerInterrupt();

	/* Restore the context of the first task. */
	portRESTORE_CONTEXT();

    portASM( "rti" );

	/* Should not get here! */
	return pdFALSE;
}
/*-----------------------------------------------------------*/

/*
 * Context switch functions.  These are interrupt service routines.
 */

/*
 * Manual context switch forced by calling portYIELD().  This is the SWI
 * handler.
 */
void ATTR_NAKED vPortYield( void )
{
	portASM( "lda uxCriticalNesting\n\t"
	         "pshs a\n\t"
	         "ldx pxCurrentTCB\n\t"
	         "sts 0,x\n\t"
	         "lbsr vPortYieldHandler\n\t"
	         "ldx pxCurrentTCB\n\t"
	         "lds 0,x\n\t"
	         "puls a\n\t"
	         "sta uxCriticalNesting\n\t"
	         "rti" );
}

void vPortYieldHandler( void )
{
	vTaskSwitchContext();
}
/*-----------------------------------------------------------*/

/*
 * RTOS tick interrupt service routine.  If the cooperative scheduler is
 * being used then this simply increments the tick count.  If the
 * preemptive scheduler is being used a context switch can occur.
 */
void ATTR_NAKED vPortTickInterrupt( void )
{
	portASM( "lda 0xff02\n\t"
	         "lda uxCriticalNesting\n\t"
	         "pshs a\n\t"
	         "ldx pxCurrentTCB\n\t"
	         "sts 0,x\n\t"
	         "lbsr vPortTickInterruptHandler\n\t"
	         "ldx pxCurrentTCB\n\t"
	         "lds 0,x\n\t"
	         "puls a\n\t"
	         "sta uxCriticalNesting\n\t"
	         "rti" );
}

void vPortTickInterruptHandler( void )
{
	#if configUSE_PREEMPTION == 1
	{
		if( xTaskIncrementTick() != pdFALSE )
		{
			vTaskSwitchContext();
		}
	}
	#else
	{
		xTaskIncrementTick();
	}
	#endif

}

