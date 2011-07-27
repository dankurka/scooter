//////////////////////////////////////////////////////////////////
//
//   $Header: /cvs/Controller\040Board/AVR-Code/Testsuite/irled_pwm.h,v 1.1 2006/11/12 22:44:17 rainer Exp $
//   $Source: /cvs/Controller\040Board/AVR-Code/Testsuite/irled_pwm.h,v $
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
//	$Log: irled_pwm.h,v $
//	Revision 1.1  2006/11/12 22:44:17  rainer
//	Added serial debug monitor and support for IR-LED PWM-Signals.
//
//
//
//
/////////////////////////////////////////////////////////////////


/** 
 *  @defgroup ro_irpwm Infrared LEDs PWM
 *  @code #include <irled_pwm.h> @endcode
 * 
 *  @brief Routines for controlling the PWM-Signal of the Infrared-LED
 *
 * 
 * This code initializes the PWM for the Infrared-LEDs of the
 * light-barrier
 *   
 *  @note Based on Atmel Datasheet for Atmega128, Nov. 2006
 *  @author Rainer Ostendorf
 */

/**@{*/

#ifndef IRLED_PWM_H_
#define IRLED_PWM_H_

void init_irled_pwm(void);

#endif /*IRLED_PWM_H_*/

/**@}*/

