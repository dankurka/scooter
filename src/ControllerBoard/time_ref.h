//////////////////////////////////////////////////////////////////
//
//   $Header: /cvs/Controller\040Board/AVR-Code/Testsuite/time_ref.h,v 1.2 2007/06/25 12:08:48 rainer Exp $
//   $Source: /cvs/Controller\040Board/AVR-Code/Testsuite/time_ref.h,v $
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
//  $Log: time_ref.h,v $
//  Revision 1.2  2007/06/25 12:08:48  rainer
//  Added stable timebase for the stabilizer (beta)
//
//  Revision 1.1  2007/06/13 03:33:49  rainer
//  Rainer: Initial checkin. Realtime system clock.
//
//
//
/////////////////////////////////////////////////////////////////

/** 
 *  @defgroup ro_timeref  Time reference
 *  @code #include <time_ref.h> @endcode
 * 
 *  @brief Code providing a time reference using Timer/Counter2
 *
 * 
 *	This code provides intitalization code and getter/setter
 * functions for using the systems time-reference.
 *  
 *  @note Based on Atmel Datasheet for Atmega128, Nov. 2006
 *  @author Rainer Ostendorf
 */

/**@{*/


#ifndef TIME_REF_H_
#define TIME_REF_H_

/* stopwatch for the stabilizer loop */
uint8_t runtime_counter;

/* current system time in ms */
uint16_t systime_millisec;

uint8_t systime_sec;

uint8_t systime_min;

uint8_t systime_h;

/* function to initialize the Timer8 */
void init_time_ref(void);


#endif /*TIME_REF_H_*/

/**@}*/

