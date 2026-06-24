#ifndef _TURBO9_PORTS_H
#define _TURBO9_PORTS_H

#include "ports_def.h"

/** Define default SCI port registers */
#if defined(M6812_DEF_SCI)

#if M6812_DEF_SCI==2
# define SCI_BASE	SCI2_BASE
#elif M6812_DEF_SCI==1
# define SCI_BASE	SCI1_BASE
#else /* default M6812_DEF_SCI==0 */
# define SCI_BASE	SCI0_BASE
#endif /* default M6812_DEF_SCI==0 */

#else  /* M6812_DEF_SCI not defined */
# define SCI_BASE	SCI0_BASE
#endif /* M6812_DEF_SCI */

# define SCIBD		PORTIO_16(SCI_BASE + _SCIBD)
# define SCICR1		PORTIO_8(SCI_BASE + _SCICR1)
# define SCICR2		PORTIO_8(SCI_BASE + _SCICR2)
# define SCISR1		PORTIO_8(SCI_BASE + _SCISR1)
# define SCISR2		PORTIO_8(SCI_BASE + _SCISR2)
# define SCIDRL		PORTIO_8(SCI_BASE + _SCIDRL)

extern unsigned short
get_timer_counter (void);

extern void
set_timer_counter (unsigned short value);

extern unsigned short
get_input_capture_1 (void);

extern  void
set_input_capture_1 (unsigned short value);

extern unsigned short
get_input_capture_2 (void);

extern void
set_input_capture_2 (unsigned short value);

extern unsigned short
get_input_capture_3 (void);

extern void
set_input_capture_3 (unsigned short value);

/* Get output compare 16-bit register.  */
extern unsigned short
get_output_compare_1 (void);

extern void
set_output_compare_1 (unsigned short value);

extern unsigned short
get_output_compare_2 (void);

extern void
set_output_compare_2 (unsigned short value);

extern unsigned short
get_output_compare_3 (void);

extern void
set_output_compare_3 (unsigned short value);

extern unsigned short
get_output_compare_4 (void);

extern void
set_output_compare_4 (unsigned short value);

extern unsigned short
get_output_compare_5 (void);

extern void
set_output_compare_5 (unsigned short value);

/* Reset the COP.  */
extern void
cop_reset (void);

extern void
cop_optional_reset (void);

/* Acknowledge the timer interrupt.  */
extern void
timer_acknowledge (void);

/* Initialize the timer.  */
extern void
timer_initialize_rate (unsigned char divisor);

#endif /* _TURBO9_PORTS_H */

