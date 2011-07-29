//////////////////////////////////////////////////////////////////
//
//   $Header: /cvs/Controller\040Board/AVR-Code/Testsuite/time_ref.c,v 1.2 2007/06/25 12:08:48 rainer Exp $
//   $Source: /cvs/Controller\040Board/AVR-Code/Testsuite/time_ref.c,v $
//   $Revision: 1.2 $
//
//////////////////////////////////////////////////////////////////
//
// Project: Controllerboard of balancing Scooter, Testsuite
//
// Author: Rainer Ostendorf
//
// Description: Timer providing a stable system time reference
//
//  $Log: time_ref.c,v $
//  Revision 1.2  2007/06/25 12:08:48  rainer
//  Added stable timebase for the stabilizer (beta)
//
//  Revision 1.1  2007/06/13 03:33:49  rainer
//  Rainer: Initial checkin. Realtime system clock.
//
//
//
/////////////////////////////////////////////////////////////////


#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h> 

#include "time_ref.h" 

SIGNAL( SIG_OUTPUT_COMPARE2 )
{
	runtime_counter++;
	systime_millisec++;
	if( systime_millisec == 1000 )
	{
		systime_millisec = 0;	
		systime_sec++;
		if( systime_sec == 60 )
		{
			systime_sec =  0;
			systime_min += 1;
			if( systime_min == 60 )
			{
				systime_min = 0;
				systime_h++;
			}
		}
	}	
}


/* function to initialize the Timer8 */
void init_time_ref(void)
{
	/* set TCCR2 */
	/* FOC2 = 0, dont force output cmp match */
	/* WGM21=1, WGM20=0, CTC operation mode, run from 0x00 to OCR2 */
	/* COM21=0, COM20=0, normal operation, dont toggle OC2 signal on compare match */
	/* CS22=0, CS21=1, CS20=1, clock select = clk_io/164 = 16Mhz/64 = 250kHz
	 * when TOP is set to 250, a IRQ happens every 1 ms 
	 * */ 
	TCCR2 = 0x00 | (1<<WGM21) | (1<<CS21) | (1<<CS20) ;
	
	/* intitialize the counter register to 0 */
	TCNT2 = 0x00;
	
	/* set to TOP of counter to 250 */
	OCR2 = 250;
	
	/* enable TIMER2 interrupt */
	TIMSK = TIMSK | (1<<OCIE2);
	
	systime_millisec = 0;
	systime_sec = 0;
	systime_min = 0;
	systime_h = 0;
	
}


