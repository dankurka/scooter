//////////////////////////////////////////////////////////////////
//
//   $Header: /cvs/Controller\040Board/AVR-Code/Testsuite/accel_capture.c,v 1.4 2006/11/24 22:04:28 rainer Exp $
//   $Source: /cvs/Controller\040Board/AVR-Code/Testsuite/accel_capture.c,v $
//   $Revision: 1.4 $
//
//////////////////////////////////////////////////////////////////
//
// Project: Controllerboard of balancing Scooter, Testsuite
//
// Author: Rainer Ostendorf
//
// Description: Routines for capturing the duty-cylcle of the accelerometer
//
//  $Log: accel_capture.c,v $
//  Revision 1.4  2006/11/24 22:04:28  rainer
//  Player around with real sensors and stabilization algorithm. Basic function seems to be ok, finetuning of factors to do.
//
//  Revision 1.3  2006/11/12 22:44:17  rainer
//  Added serial debug monitor and support for IR-LED PWM-Signals.
//
//  Revision 1.2  2006/11/12 17:47:15  rainer
//  Added support for PWM-Duty-cycle measurement with Timer3. Tuned SPI-Code.
//
//  Revision 1.1  2006/11/12 02:13:06  rainer
//  Added stubs for Accelerometer. To be finished. Another day...
//
//
//
//
//
/////////////////////////////////////////////////////////////////

#include <avr/io.h>
#include <avr/interrupt.h> 
#include "accel_capture.h"
#include "globals.h"


SIGNAL(  SIG_INPUT_CAPTURE3 )
{
		
		if( TCCR3B &( 1<<ICES3) ) /* if edge was rising */
		{
			/* save the captured value */
			accl_low_samples[ accl_low_idx++ ]  = ICR3;
			accl_low_idx &= (ACCL_SAMPLES_BUFSIZE-1);
			TCNT3 = 0x00; /* reset counter to start from 0 */
			TCCR3B &= ~( 1<<ICES3 ); /* trigger to falling edge */
		}
		else /* edge was falling */
		{
			/* save the captured value */
			accl_high_samples[ accl_high_idx++ ]  = ICR3;
			accl_high_idx &= (ACCL_SAMPLES_BUFSIZE-1);
			/* cut index value to array-size */
			TCNT3 = 0x00; /* reset counter to start from 0 */
			TCCR3B |= ( 1<<ICES3 ); /* trigger to rising edge */
		}
}


void init_accel_capture( )
{	
	/** Set Timer3's Control Register A */
	/** 7,6: COM3A1, COM3A0 = 00 -> Normal port operation of OC3A 
	 *  5,4: COM3B1, COM3B1 = 00 -> Normal port operation of OC3B   
	 *  3,2: COM3C1, COM3C1 = 00 -> Normal port operation of OC3C
	 *  1,0: WGM31, WGM30 = 00 -> Normal mode of operation for counter, 
	 * 								no waveform generation
	 */
	TCCR3A = 0x00;
	
	/** Set Timer3's Control Register B */
	/** 7: ICNC3 = 0 -> no noise cancellation
	 *  6: ICES3 = 1 -> trigger on rising edge of ICP3
	 *  4,3: WGM33, WGM32 = 00 -> no waveform generation
	 *  2,1,0: CS32, CS31,CS30 = 0,0,1 -> no prescaling, timer runs with full CPU speed
	 */
	 TCCR3B = 0x00 | (1<<ICES3) | (1<<CS30) ;
	 
	 /** Set Timer extended IRQ Mask Register */
	 /** 7,6 Reserved = 0 
	  *  5: TICIE3 = 1 Enable Timer 3 capture interrupt
	  *  4: OCIE3A = 0 disable CompareMatch Interrupt A
	  *  3: OCIE3B = 0 disable CompareMatch Interrupt B
	  *  2: TOIE3 = 0 disable Timer3 overflow interrupt
	  *  1: OCIE3C = 0 diable CompareMatch Interrupt C
	  *  0: OCIE1C = dont change - belong to timer 1!
	  */
	  /* clear everything but the first bit (as it is for Timer 1) */
	 ETIMSK = ETIMSK & ( 1); 
	 ETIMSK = ETIMSK | (1<<TICIE3);
	 
	 /* clear index to accel-samples ringbuffer */
	 accl_low_idx = 0;
	 accl_high_idx = 0;
	 
	 /* ICP3 to be an input */
	 DDRE = DDRE &~ (PE7);
}
