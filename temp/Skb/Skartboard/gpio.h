//////////////////////////////////////////////////////////////////
//
//   $Header: /cvs/Controller\040Board/AVR-Code/Testsuite/gpio.h,v 1.4 2006/12/13 09:38:43 rainer Exp $
//   $Source: /cvs/Controller\040Board/AVR-Code/Testsuite/gpio.h,v $
//   $Revision: 1.4 $
//
//////////////////////////////////////////////////////////////////
//
// Project: Controllerboard of balancing Scooter, Testsuite
//
// Author: Rainer Ostendorf
//
// Description: Routines for using the gereral Purpose IO-Signals and Buttons
//
//	$Log: gpio.h,v $
//	Revision 1.4  2006/12/13 09:38:43  rainer
//	Change doxygen doc for macros
//
//	Revision 1.3  2006/11/20 09:45:46  rainer
//	Improved doxygen documentation, killed some bugs there.
//
//	Revision 1.2  2006/11/12 00:59:42  rainer
//	Added support for interrupt-driven use of the Gyros. Cleaned up main() a bit.
//
//	Revision 1.1  2006/11/11 19:21:50  rainer
//	Initial checkin, GPIO-Routines.
//
//
//
//
/////////////////////////////////////////////////////////////////

/** 
 *  @defgroup ro_gpio GPIO/Port Routines
 *  @code #include <gpio.h> @endcode
 * 
 *  @brief Routines for controlling the general purpose IO-ports and signals
 *
 * 
 * This Routines are for initializing and using the general purpose I/O-Signals.
 *   
 *  @note Based on Atmel Datasheet for Atmega128, Nov. 2006
 *  @author Rainer Ostendorf
 */

/**@{*/

#ifndef GPIO_H_
#define GPIO_H_

/** \def BUT1 Define for reading the Button. Value is true if Button 1 is pressed */
#define BUT1 (!( PINA & (1<<PA0) ))
/** \def BUT2 Define for reading the Button. Value is true if Button 2 is pressed */
#define BUT2 (!( PINA & (1<<PA1))) 

/** Initialize the IO-Ports Direction registers (input, output)*/
void init_port_directions( void );

/** Initialize the IO-Ports Value registers */
void init_port_values( void );

#endif /*GPIO_H_*/


/**@}*/
