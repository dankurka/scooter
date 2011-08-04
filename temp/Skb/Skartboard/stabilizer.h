//////////////////////////////////////////////////////////////////
//
//   $Header: /cvs/Controller\040Board/AVR-Code/Testsuite/stabilizer.h,v 1.13 2007/07/21 10:09:24 rainer Exp $
//   $Source: /cvs/Controller\040Board/AVR-Code/Testsuite/stabilizer.h,v $
//   $Revision: 1.13 $
//
//////////////////////////////////////////////////////////////////
//
// Project: Controllerboard of balancing Scooter, Testsuite
//
// Author: Original C167 code by segway team, adaption to AVR by Rainer Ostendorf
//
// Description: controller algrorithm for platform stabilization
//
//	$Log: stabilizer.h,v $
//	Revision 1.13  2007/07/21 10:09:24  rainer
//	Aktuelle Testversion auf Segway No.2 (FH-Segway). Modzifiziert: Gyro-Offset
//
//	Revision 1.12  2007/06/25 19:58:04  rainer
//	Put controllers parameters to stability limit
//
//	Revision 1.11  2007/06/25 12:08:48  rainer
//	Added stable timebase for the stabilizer (beta)
//
//	Revision 1.10  2007/06/13 03:34:11  rainer
//	rainer, added realtime-clock. modified main
//
//	Revision 1.9  2007/06/11 17:37:57  rainer
//	Rainer: Changed stabilizer for first test drives. Currently no limit in PWM and no steering active.
//
//	Revision 1.8  2007/04/18 17:44:03  kurt
//	added steering support(beta)
//
//	Revision 1.7  2007/04/18 04:23:49  kurt
//	*** empty log message ***
//
//	Revision 1.6  2007/04/18 04:18:16  kurt
//	*** empty log message ***
//
//	Revision 1.5  2007/04/18 01:40:14  rainer
//	cur_speed hinzugefügt
//
//	Revision 1.4  2007/04/18 01:36:18  rainer
//	Regler angepasst, zum Testen RS232 Konsole eingebaut um Regler-Parameter zu variieren. Rainer.
//
//	Revision 1.3  2006/11/26 20:41:29  rainer
//	Changed polarity of PWM-Signal. Adjusted stabilization control parameters.
//
//	Revision 1.2  2006/11/24 22:04:28  rainer
//	Player around with real sensors and stabilization algorithm. Basic function seems to be ok, finetuning of factors to do.
//
//	Revision 1.1  2006/11/20 00:04:46  rainer
//	Initial checkin. stabilization control algorithm, ported from C167. not yet tested.
//
//
//
//
/////////////////////////////////////////////////////////////////


/** 
 *  @defgroup ro_stabilizer  Stabilization control
 *  @code #include <stabilizer.h> @endcode
 * 
 *  @brief Controller code for stabilizing the platform
 *
 * 
 * This file contains the controller algorithm for stabilizing
 * the platform by calculating the current position of the device
 * and setting the PWM-Signals of the motors accordingly, so the
 * platform is in balance.  
 *  
 *  @note Based on Atmel Datasheet for Atmega128, Nov. 2006
 *  @author Original C167 code by segway team, adaption to AVR by Rainer Ostendorf
 */

// #define SERIALNO_1 1
#define SERIALNO_2 1


#ifndef STABILIZER_H_
#define STABILIZER_H_

/** PI ... */
#define PI 3.14

/** the initial value, the gyroscope 1 measures, when not beeing moved */ 

#ifdef SERIALNO_1
#define GYRO1_ZERO_VALUE -65.0  /* Offset of 1983 (for not using twos-complement) */
#endif

#ifdef SERIALNO_2
#define GYRO1_ZERO_VALUE -14.0 /* Offset of 1983 (for not using twos-complement) */
#endif

/** scaling factor to calculate the angular speed (omega) from
 * the Gyroscope1 value 
 * 
 * from orignal code :
 *   OMEGA   = ( ( (GYRO-GYRO_zero) / 2048) * 2 * PI * 300) / 360;
 * <=> OMEGA = ( (GYRO-GYRO_zero) * 0.920388) / 360 
 * <=> OMEGA = ( (GYRO-GYRO_zero) * 2.556634E-3
 *  
 * TODO: Needs to be adapted to sensor 
 */
#define GYRO_TO_OMEGA_FACTOR 0.002556634



/** correction factor for scaling the acclerometer value to degrees */
#define ACCL_TO_DEGREE_FACTOR 25.0 /* from original code: (0.04)^-1 */


/** zero point of the steering potentiometer. 
 * (10Bit resolution -> 1024 max, so 1024/2)
 * */
#define STEERING_ZERO_POINT 378   

/* responsivness of the steering */
#define STEERING_RESPONSIVENESS 0.0012

/* maximum steering lock (lenkeinschlag) */
#define MAX_STEERING_LOCK 0.2

/** tilt angle factor for kalman algorithm
 * TODO: add detailed description here!
 * This is not documented in original code */
#define K_ANGLE 25.0

/** angular speed (omega) factor for kalman algorithm
 * TODO: add detailed description here!
 * This is not documented in original code */
#define K_OMEGA 50.0

/** correction factor for kalman algorithm
 * TODO: add detailed description here!
 * This is not documented in original code */
#define K_CORR_FACTOR 0.1

/** setpoint (sollwert) of the controlling algorithm 
 * TODO: Check comment! Add detailed desciption */
#define SET_POINT -0.12

/** factor to scale the set value with for current speed 
 * TODO: Check comment! Add detailed desciption */
#define CUR_SPEED_FACTOR 120.0

#define STABILIZE_PARAMS 1
#define STABILIZE_DEBUG 1


void stabilizer_loop( void );


#endif /*STABILIZER_H_*/
