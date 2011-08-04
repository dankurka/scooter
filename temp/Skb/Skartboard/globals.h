//////////////////////////////////////////////////////////////////
//
//   $Header: /cvs/Controller\040Board/AVR-Code/Testsuite/globals.h,v 1.10 2007/06/13 03:34:11 rainer Exp $
//   $Source: /cvs/Controller\040Board/AVR-Code/Testsuite/globals.h,v $
//   $Revision: 1.10 $
//
//////////////////////////////////////////////////////////////////
//
// Project: Controllerboard of balancing Scooter, Testsuite
//
// Author: Rainer Ostendorf
//
// Description: Global parameters used to pass information between ISR and normal routines
//
//	$Log: globals.h,v $
//	Revision 1.10  2007/06/13 03:34:11  rainer
//	rainer, added realtime-clock. modified main
//
//	Revision 1.9  2007/04/18 01:36:18  rainer
//	Regler angepasst, zum Testen RS232 Konsole eingebaut um Regler-Parameter zu variieren. Rainer.
//
//	Revision 1.8  2006/11/24 22:04:28  rainer
//	Player around with real sensors and stabilization algorithm. Basic function seems to be ok, finetuning of factors to do.
//
//	Revision 1.7  2006/11/20 09:45:46  rainer
//	Improved doxygen documentation, killed some bugs there.
//
//	Revision 1.6  2006/11/18 18:47:38  rainer
//	Added support for reading the temerature of the Gyros.
//
//	Revision 1.5  2006/11/17 13:23:08  rainer
//	Edited functions for frequency-updates to the PWM: Now the Threshold value is written before the new TOP value is written. For this purpose, two new variables were introduced in motor_pwm.h holding the duty-cycle of each motor
//
//	Revision 1.4  2006/11/12 22:44:17  rainer
//	Added serial debug monitor and support for IR-LED PWM-Signals.
//
//	Revision 1.3  2006/11/12 17:47:15  rainer
//	Added support for PWM-Duty-cycle measurement with Timer3. Tuned SPI-Code.
//
//	Revision 1.2  2006/11/12 00:59:42  rainer
//	Added support for interrupt-driven use of the Gyros. Cleaned up main() a bit.
//
//	Revision 1.1  2006/11/12 00:50:56  rainer
//	Initial checkin. Global variables.
//
//
//
//
/////////////////////////////////////////////////////////////////


/** 
 *  @defgroup ro_globals Global variables 
 *  @code #include <leds.h> @endcode
 * 
 *  @brief global Variables for passing information between ISR and normal Code
 *
 * 
 * This file contains variables for intercommunication of sofware-modules
 * especially between Interrupt-Service-Routines that read the external sensors
 * and the control alorithms.
 *   
 *  @author Rainer Ostendorf
 */

/**@{*/

#ifndef GLOBAL_H_
#define GLOBAL_H_

#include <inttypes.h>

#include "stabilizer.h"

/* Defines */

/** Size of the samples ringbuffer for Gyro1.
 * The size is binary ANDed with the ringbuffer-index as
 * a real modulo would be much to expensive. So only values
 * the are 2 to the power of n with n = 0, 1, 2, .. are allowed here
 * e.g.: 1,2,4,8,16,32,64, 128, 255  using other value will lead
 * to unpredictable results, as the index will not increment linear 
 * Please be aware that a 8bit variable is used to hold this value,
 * so the maximum size is 255 ! */
 #define GYRO1_SAMPLES_BUFSIZE 8
 
 /** Size of the samples ringbuffer for gyro2 .
  * Same restrictions as for Gyro1_Bufsize -> el. of n = 2^x !*/
#define GYRO2_SAMPLES_BUFSIZE 8 

/** Size of the samples ringbuffer for accelerometer.  
  * Same restrictions as for Gyro1_Bufsize -> el. of n = 2^x !*/
#define ACCL_SAMPLES_BUFSIZE 8

/* ******** GYROSCOPES VALUES ************** */

/** Index variables, holding the latest write positions of the ISR. */
/* The lastes values is at sample[ (index-1)%BUFSIZE ], the ISR updates index  
 * after each write to buffer */
uint8_t gyro1_idx;
uint8_t gyro2_idx;
/** Ringbuffers, the ISR saves the newest samples to */
uint16_t gyro1_samples[ GYRO1_SAMPLES_BUFSIZE ];
uint16_t gyro2_samples[ GYRO1_SAMPLES_BUFSIZE ];
/** Current temperature value of the Gyros */
uint16_t gyro1_temp;
uint16_t gyro2_temp;

/** Flag to trigger the finite state maschine within the
 * SPI-ISR to read the temperature of each sensor once
 * when set to 1. After the temp.-values are saved to their
 * global accessible variables, the flag is cleared.
 */
 uint8_t read_gyro_temp;

/* ******** Accelerometer VALUES ************** */

/** Index to accelerometer-samples ringbuffer */
uint8_t accl_high_idx;
uint8_t accl_low_idx;
/** Ringbuffer holding last value of accelerometer */
uint16_t accl_high_samples[ ACCL_SAMPLES_BUFSIZE ];
uint16_t accl_low_samples[ ACCL_SAMPLES_BUFSIZE ];

uint16_t steering_zero_point;

#endif /*GLOBAL_H_*/

/*@}*/

