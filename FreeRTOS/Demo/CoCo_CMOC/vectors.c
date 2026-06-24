/* modrx.c -- wireless controller receiver for robots
   Copyright 2004 Robotronics, Inc.
   Author Jefferson Smith

   This file is part of the Modular Robot Design.
*/

#include "cpu.h"
#include <sys/ports.h>
#include <sys/interrupts.h>

void fatal_interrupt ()
{
  /* Infinite loop for debugging
     Returning would not help as it's necessary to clear the interrupt flag.
     */
  for (;;) cop_optional_reset();
}

#ifdef USE_INTERRUPT_TABLE

/* NOTE: these ISR must be in non-banked memory (near) */

/* Manual context switch function.  This is the SWI ISR. */
void ATTR_INT ATTR_NEAR vPortYield( void );

/* Tick context switch function.  This is the timer ISR. */
void ATTR_INT ATTR_NEAR vPortTickInterrupt( void );

void ATTR_INT ATTR_NEAR vCOM_ISR( void );

/* Interrupt vectors table.

   Note: the `XXX_handler: foo' notation is a GNU extension which is
   used here to ensure correct association of the handler in the struct.
   This is why the order of handlers declared below does not follow
   the MCU order.  */
#endif

