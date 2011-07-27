//////////////////////////////////////////////////////////////////
//
//   $Header: /cvs/Controller\040Board/AVR-Code/Testsuite/beeper.c,v 1.2 2007/09/23 22:56:59 rainer Exp $
//   $Source: /cvs/Controller\040Board/AVR-Code/Testsuite/beeper.c,v $
//   $Revision: 1.2 $
//
//////////////////////////////////////////////////////////////////
//
// Project: Controllerboard of balancing Scooter, Testsuite
//
// Author: Rainer Ostendorf
//
// Description: Timer controlling the PWM-Signal driving the beeper
//
//	$Log: beeper.c,v $
//	Revision 1.2  2007/09/23 22:56:59  rainer
//	Rainer: added beeper, added some display com channels, modified tiltback, modified steering
//
//	Revision 1.1  2007/09/12 04:17:02  rainer
//	Rainer: Worked the weekend on software-features: Implemented display-communication, battery-monitoring, beeper support
//
//
//
//
//
/////////////////////////////////////////////////////////////////


#include <avr/io.h>
#include "beeper.h"
#include "helpers.h"

/* Initialize beeper system */
void init_beeper()
{
	/* turn on beeper */ 	
	DDRB |= (1<<PB7);
	delay_ms(50);
	/* turn off beeper */ 
	DDRB &= ~(1<<PB7);
	delay_ms(50);
	/* turn on beeper */ 	
	DDRB |= (1<<PB7);
	delay_ms(50);
	/* turn off beeper */
	DDRB &= ~(1<<PB7);
		
}

/** turn on system beeper */
inline void beeper_on()
{
	/* turn on beeper */ 	
	DDRB |= (1<<PB7);
}

/** turn off system beeper */
inline void beeper_off()
{
	/* turn on beeper */ 	
	DDRB &= ~(1<<PB7);
}

