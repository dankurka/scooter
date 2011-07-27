//////////////////////////////////////////////////////////////////
//
//   $Header: /cvs/Controller\040Board/AVR-Code/Testsuite/irled_pwm.c,v 1.1 2006/11/12 22:44:17 rainer Exp $
//   $Source: /cvs/Controller\040Board/AVR-Code/Testsuite/irled_pwm.c,v $
//   $Revision: 1.1 $
//
//////////////////////////////////////////////////////////////////
//
// Project: Controllerboard of balancing Scooter, Testsuite
//
// Author: Rainer Ostendorf
//
// Description: Timer controlling the PWM-Signal driving the external Infrared-LEDs
//
//	$Log: irled_pwm.c,v $
//	Revision 1.1  2006/11/12 22:44:17  rainer
//	Added serial debug monitor and support for IR-LED PWM-Signals.
//
//
//
//
/////////////////////////////////////////////////////////////////


#include <avr/io.h>
#include "irled_pwm.h"

void init_irled_pwm(void)
{
		/** Set Timer0 Control Register
		 * 7 FOC0 = 0 -> no force of compare match
		 * 6,3 WGM01, WMG00 = 1,1 -> Fast-PWM-Mode
		 * 5,4 COM01, COM00 = 1,0 -> Set OC2 at Bottom, clear on compare match 
		 * 2,1,0 CS02,CS01,CS00 = 0,1,0 Prescaler of 8 to f_cpu (=>2Mhz fcnt)
		 */
		 TCCR0 = 0x00 | (1<<WGM01) | (1<<WGM00) | (1<<COM01) | (1<<CS01);
		 
		 /* initialize Counter register */
		 TCNT0 = 0;
		 
		 /* set Timer2 Output compare register (128 -> 50/50 duty cycle with top=255 */
		 OCR0 = 128;
		 
		 /* set OC2 to act as output */
		 DDRB = DDRB | (1<<PB4) ;
}
