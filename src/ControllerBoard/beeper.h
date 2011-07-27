//////////////////////////////////////////////////////////////////
//
//   $Header: /cvs/Controller\040Board/AVR-Code/Testsuite/beeper.h,v 1.2 2007/09/23 22:56:59 rainer Exp $
//   $Source: /cvs/Controller\040Board/AVR-Code/Testsuite/beeper.h,v $
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
//	$Log: beeper.h,v $
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


/** 
 *  @defgroup ro_beeper Beeper control
 *  @code #include <beeper.h> @endcode
 * 
 *  @brief Routines for controlling the PWM-Signal of the alarm beeper
 *
 * 
 * This code initializes the PWM for the Infrared-LEDs of the
 * light-barrier and provides functions to beep
 *   
 *  @note Based on Atmel Datasheet for Atmega128, Nov. 2006
 *  @author Rainer Ostendorf
 */

/**@{*/


#ifndef BEEPER_H_
#define BEEPER_H_

/* Initialize beeper system */
void init_beeper();

/** turn on system beeper */
inline void beeper_on();

/** turn off system beeper */
inline void beeper_off();

#endif /*BEEPER_H_*/



/**@}*/


