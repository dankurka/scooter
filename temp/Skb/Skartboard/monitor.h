//////////////////////////////////////////////////////////////////
//
//   $Header: /cvs/Controller\040Board/AVR-Code/Testsuite/monitor.h,v 1.1 2006/11/12 22:44:17 rainer Exp $
//   $Source: /cvs/Controller\040Board/AVR-Code/Testsuite/monitor.h,v $
//   $Revision: 1.1 $
//
//////////////////////////////////////////////////////////////////
//
// Project: Controllerboard of balancing Scooter, Testsuite
//
// Author: Rainer Ostendorf
//
// Description: Routines for manipulating variables and showing system status
//
//	$Log: monitor.h,v $
//	Revision 1.1  2006/11/12 22:44:17  rainer
//	Added serial debug monitor and support for IR-LED PWM-Signals.
//
//
//
//
/////////////////////////////////////////////////////////////////


/** 
 *  @defgroup ro_monitor  Serial Monitor
 *  @code #include <monitor.h> @endcode
 * 
 *  @brief A serial debug monitor
 *
 * 
 * The monitor program shows internal system-parameter
 * and allows manipulations of variables 
 *   
 *  @note Based on Atmel Datasheet for Atmega128, Nov. 2006
 *  @author Rainer Ostendorf
 */

/**@{*/


#ifndef MONITOR_H_
#define MONITOR_H_

#include <avr/pgmspace.h>
#include "uart.h"


#define BUFLEN 32

void monitor();

#endif /*MONITOR_H_*/

/**@}*/

