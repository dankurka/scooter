//////////////////////////////////////////////////////////////////
//
//   $Header: /cvs/Controller\040Board/AVR-Code/Testsuite/leds.h,v 1.3 2006/11/12 22:44:17 rainer Exp $
//   $Source: /cvs/Controller\040Board/AVR-Code/Testsuite/leds.h,v $
//   $Revision: 1.3 $
//
//////////////////////////////////////////////////////////////////
//
// Project: Controllerboard of balancing Scooter, Testsuite
//
// Author: Rainer Ostendorf
//
// Description: Routines for setting the onboard Debug LEDs
//
//	$Log: leds.h,v $
//	Revision 1.3  2006/11/12 22:44:17  rainer
//	Added serial debug monitor and support for IR-LED PWM-Signals.
//
//	Revision 1.2  2006/11/12 00:59:42  rainer
//	Added support for interrupt-driven use of the Gyros. Cleaned up main() a bit.
//
//	Revision 1.1  2006/11/08 12:49:54  rainer
//	Umsortiert. VHDL-Daten bitte jetzt nach VHDL-Code
//
//	Revision 1.2  2006/11/06 23:20:14  rainer
//	Added some documentation and doxygen-support.
//
//	Revision 1.1  2006/11/06 22:38:42  rainer
//	Initial checkin.
//	Supported features: LEDs, UART1, Motor-PWM, ADC
//	Rainer. 11/06/2006
//
//
//
/////////////////////////////////////////////////////////////////


/** 
 *  @defgroup ro_leds Onboard Debug LEDs
 *  @code #include <leds.h> @endcode
 * 
 *  @brief Routines for controlling the onboard Debug LEDs
 *
 * 
 * This Routines are for setting, clearing and toggeling the four
 * onboard LEDs
 *   
 *  @note Based on Atmel Datasheet for Atmega128, Nov. 2006
 *  @author Rainer Ostendorf
 */

/**@{*/

#ifndef LEDS_H_
#define LEDS_H_

/** turn on the given led 
 * @param led number of the led to turn on (0-3)
 * @return -1 if led has illegal value, 0 else
 */
int set_led( char led );

/** turn off the given led 
 * @param led number of the led to turn off (0-3)
 * @return -1 if led has illegal value, 0 else
 */
int clear_led( char led );


/** toggle the given led 
 * @param led number of the led to toggle (0-3)
 * @return -1 if led has illegal value, 0 else
 */
int toggle_led( char led );

/** led the leds 1 and 3 fade in and out.
 * BEWARE: this functions uses busy waiting and wastes a
 * huge amount of cpu-time!
 */
void led_pwm_fade( void );

#endif /*LEDS_H_*/

/**@}*/
