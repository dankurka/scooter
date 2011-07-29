//////////////////////////////////////////////////////////////////
//
//   $Header: /cvs/Controller\040Board/AVR-Code/Testsuite/adc.c,v 1.3 2006/11/13 14:35:57 rainer Exp $
//   $Source: /cvs/Controller\040Board/AVR-Code/Testsuite/adc.c,v $
//   $Revision: 1.3 $
//
//////////////////////////////////////////////////////////////////
//
// Project: Controllerboard of balancing Scooter, Testsuite
//
// Author: Rainer Ostendorf
//
// Description: Routines for using the onchip Analog-Digital-Converter
//
//	$Log: adc.c,v $
//	Revision 1.3  2006/11/13 14:35:57  rainer
//	Added includes.
//
//	Revision 1.2  2006/11/12 00:59:42  rainer
//	Added support for interrupt-driven use of the Gyros. Cleaned up main() a bit.
//
//	Revision 1.1  2006/11/08 12:49:54  rainer
//	Umsortiert. VHDL-Daten bitte jetzt nach VHDL-Code
//
//	Revision 1.2  2006/11/07 23:17:00  rainer
//	no real change
//
//	Revision 1.1  2006/11/06 22:38:42  rainer
//	Initial checkin.
//	Supported features: LEDs, UART1, Motor-PWM, ADC
//	Rainer. 11/06/2006
//
//
//
/////////////////////////////////////////////////////////////////

#include <stdlib.h>
#include <stdio.h>
#include <avr/io.h>
#include "uart.h"
#include "adc.h"


/* Initialize ADC, but not yet start a conversion
 */
void init_adc()
{
		/* set ADC Multiplexer register 
		 * REFS1 = 0, REFS0 = 1 -> Avcc is Vref
		 * ADLAR = 0 -> right-adjust conversion result
		 * MUX4-0 = 00100 -> ADC4 (Header Steering 1)
		 */
		 ADMUX = (1<<REFS0) | (1<<MUX2);
		 
		 /* set ADC Control Register A
		  * ADEN = 1 -> switch on ADC
		  * ADSC = 0 -> not yet start conversion
		  * ADFR = 0 -> dont operate in free-running mode
		  * ADIF = -> .. interrupt flag
		  * ADIE = 0 -> deactive interrupts by ADC
		  * ADPS2-0 = 111 -> prescaler of 128 -> ADC-Clock is 125kHz, 
		  * 				slowest possible for maximum resolution
		  * */
		 ADCSRA = (1<<ADEN) | (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);
}


/** select the active ADC-Channel from input multiplexer
 * @param channel ADC-Channel to read, 0-7
 */
void adc_sel_channel(char channel)
{
		ADMUX = ADMUX & 0xE0; /* clear lower 5 bits */
		ADMUX = ADMUX | (channel & 0x1F); /* set channel, upper 3 bits masked */
}

/** start an ADConversion, busy-wait for the result and return it
 *  a conversion will last approx. 12us (calculated, not measured, TODO)
 * @return the conversion result
 */
unsigned int read_adc()
{
		/* set "start conversion" bit in adc control register */
		ADCSRA = ADCSRA | (1<<ADSC);
		/* busy-wait for sampling-result */
		while( !(ADCSRA & (1<<ADIF)) )
			/* nop */;
		return ADC;
}

/** demo-code: read in all ADC channels and print their
 * result to the first uart.
 */
void print_all_adc( void )
{
	char buf[20];
	int i,o;
	for(o=0; o<8; o++)
	{
		adc_sel_channel(o); /* select channel */
		i = read_adc(); /* read channel */
		snprintf(buf, 20, "ADC %d: %d/1024 \r\n", o, i);
		uart_puts(buf); 
	}
}
