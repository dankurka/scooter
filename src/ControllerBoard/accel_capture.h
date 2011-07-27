//////////////////////////////////////////////////////////////////
//
//   $Header: /cvs/Controller\040Board/AVR-Code/Testsuite/accel_capture.h,v 1.3 2006/11/20 09:45:46 rainer Exp $
//   $Source: /cvs/Controller\040Board/AVR-Code/Testsuite/accel_capture.h,v $
//   $Revision: 1.3 $
//
//////////////////////////////////////////////////////////////////
//
// Project: Controllerboard of balancing Scooter, Testsuite
//
// Author: Rainer Ostendorf
//
// Description: Routines for capturing the duty-cycle of the accelerometer
//
//  $Log: accel_capture.h,v $
//  Revision 1.3  2006/11/20 09:45:46  rainer
//  Improved doxygen documentation, killed some bugs there.
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

/** 
 *  @defgroup ro_accel_capcom  Accelerometer Capture'n'Compare
 *  @code #include <accel_capcom.h> @endcode
 * 
 *  @brief Code for reading the Accelerometer
 *
 * 
 *	
 *  
 *  @note Based on Atmel Datasheet for Atmega128, Nov. 2006
 *  @author Rainer Ostendorf
 */

/**@{*/

#ifndef ACCEL_CAPCOM_H_
#define ACCEL_CAPCOM_H_

/** Initalize the Capture of Events on ICP3. 
 *  Configure Timer 3 to generate an interrupt on
 *  each falling flank of ICP3 and save the current 
 *  timer-value to a buffer. 
 */ 
void init_accel_capture( );

#endif /*ACCEL_CAPCOM_H_*/

/**@}*/
