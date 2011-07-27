//////////////////////////////////////////////////////////////////
//
//   $Header: /cvs/Controller\040Board/AVR-Code/Testsuite/gpio.c,v 1.3 2007/09/12 04:17:02 rainer Exp $
//   $Source: /cvs/Controller\040Board/AVR-Code/Testsuite/gpio.c,v $
//   $Revision: 1.3 $
//
//////////////////////////////////////////////////////////////////
//
// Project: Controllerboard of balancing Scooter, Testsuite
//
// Author: Rainer Ostendorf
//
// Description: Routines for using the general purpose IO-signals and the buttons
//
//	$Log: gpio.c,v $
//	Revision 1.3  2007/09/12 04:17:02  rainer
//	Rainer: Worked the weekend on software-features: Implemented display-communication, battery-monitoring, beeper support
//
//	Revision 1.2  2006/12/18 22:55:18  rainer
//	switched polarity of motor-enable lines in init_port_values to deactivate
//	them by default.
//
//	Revision 1.1  2006/11/11 19:21:50  rainer
//	Initial checkin, GPIO-Routines.
//
//
//
//
/////////////////////////////////////////////////////////////////


#include <avr/io.h>

/** Initialize the IO-Ports Direction registers (input, output)*/
void init_port_directions( void )
{
	/* Port A: onboard PushButtons, Steering-Header, Gyro selftest IOs */
	DDRA=0x00;
	/* Port B: SPI, IR-LED PWM out, MOTOR1 and 2 PWM out */
	DDRB=0x00 | (1<<5) | (1<<6) | (1<<7);
	/* Port C: onboard Debug-LEDs, unused onboard bufferd outputs */
	DDRC=0xFF;
	/* Port D: Motor OK in, second UART (to display), SPI-CS, Motor1,2 directions */
	DDRD=0x08;
	/* Port E: first UART (debug), Tacho signals, Accelerometer */
	DDRE=0x00;
	/* Port F: Analog-Digital Converter */
	DDRF=0x00;
	/* Port G: Display IO and IR-LEDs in */
	DDRG=0x00;
}

/** Initialize the IO-Ports Value registers */
void init_port_values( void )
{
	/* LOCK motors */
	PORTC = 0xFF;
}
