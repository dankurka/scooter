//////////////////////////////////////////////////////////////////
//
//   $Header: /cvs/Controller\040Board/AVR-Code/Testsuite/stabilizer.h,v 1.23 2009/03/03 03:38:38 kurt Exp $
//   $Source: /cvs/Controller\040Board/AVR-Code/Testsuite/stabilizer.h,v $
//   $Revision: 1.23 $
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
//	Revision 1.23  2009/03/03 03:38:38  kurt
//	changed ste point for daniel segway
//
//	Revision 1.22  2009/03/03 03:04:25  kurt
//	changed daniel zero point (SET POINT)
//
//	Revision 1.21  2009/02/24 08:43:01  kurt
//	added daniels segway with all constants
//
//	Revision 1.20  2008/03/21 23:39:26  rainer
//	increased max speed
//
//	Revision 1.19  2007/09/25 07:36:56  rainer
//	Rainer: Parameter for Tiltback adjusted.
//
//	Revision 1.18  2007/09/24 21:59:06  rainer
//	Rainer: Adapted stabilizer-parameters for improved stability. State of Software before segway-presentation (IHK) on 8.Nov.2007
//
//	Revision 1.17  2007/09/23 22:56:59  rainer
//	Rainer: added beeper, added some display com channels, modified tiltback, modified steering
//
//	Revision 1.16  2007/09/15 12:33:24  rainer
//	Rainer: Added Tilt-Back. Beta! Works but needs to be optimized. Note cur_speed is negative when running forward!
//
//	Revision 1.15  2007/09/12 04:17:02  rainer
//	Rainer: Worked the weekend on software-features: Implemented display-communication, battery-monitoring, beeper support
//
//	Revision 1.14  2007/09/09 17:08:07  rainer
//	Rainer: Added battery voltage sampling, implemented some new ser-com objects
//
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

//#define SERIALNO_1 1 /* Daniels */
#define SERIALNO_2 1 /* FH */
//#define SERIALNO_3 1 /* Rainers */

#ifndef STABILIZER_H_
#define STABILIZER_H_

/** PI ... */
#define PI 3.14

/** the initial value, the gyroscope 1 measures, when not beeing moved */ 

#ifdef SERIALNO_1
#define GYRO1_ZERO_VALUE -65.0  /* Offset of 1983 (for not using twos-complement) */
#endif

#ifdef SERIALNO_2
#define GYRO1_ZERO_VALUE 42.0 /* Offset of 1983 (for not using twos-complement) */
#endif

#ifdef SERIALNO_3
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


/* maximum steering lock (lenkeinschlag) */
#define MAX_STEERING_LOCK 0.1

/** tilt angle factor for kalman algorithm
 * TODO: add detailed description here!
 * This is not documented in original code */
#ifdef SERIALNO_1
#define K_ANGLE 3.5
#endif
#ifdef SERIALNO_2
 #define K_ANGLE 13.0
#endif
#ifdef SERIALNO_3
 #define K_ANGLE 13.0
#endif
/** angular speed (omega) factor for kalman algorithm
 * TODO: add detailed description here!
 * This is not documented in original code */

#ifdef SERIALNO_1
#define K_OMEGA 0.1
#endif
#ifdef SERIALNO_2
#define K_OMEGA 0.18
#endif
#ifdef SERIALNO_3
#define K_OMEGA 0.17
#endif

/** correction factor for kalman algorithm
 * TODO: add detailed description here!
 * This is not documented in original code */
#define K_CORR_FACTOR 0.1

/** setpoint (sollwert) of the controlling algorithm 
 * TODO: Check comment! Add detailed desciption */

#ifdef SERIALNO_1
#define SET_POINT -0.1
#endif

#ifdef SERIALNO_2
#define SET_POINT 0.0
#endif

#ifdef SERIALNO_3
#define SET_POINT 0.08
#endif


/** factor to scale the set value with for current speed 
 * TODO: Check comment! Add detailed desciption */
#define CUR_SPEED_FACTOR 1.0

/** Speedlimit for tilt back mechanism. 
 * User is kept from accelerating above this speed
 * by tilting back the set point */
#define TILT_BACK_SPEED 0.2

/** maximum duty cycle in forward direction 0.0 to 1.0 */
#define MAX_FORWARD_DUTY 1.0

/** maximum duty cycle in forward direction 0.0 to -1.0 */
#define MAX_BACKWARD_DUTY -1.0


/* variable to save the maximum steering ADV value in */
extern int steering_max;
/* variable to save the maximum steering ADV value in */
extern int steering_min;

/* make cur_speed accessibel for serial com */ 
extern double cur_speed;

extern double phi;

extern double set_point;

/* analogue value of the steering potentiometer */
extern int steering_potential;

/* difference between zero-point and potential */
extern int steering_diff;

/* difference between zero-point and minimum or maximum */
extern int steering_range;

extern double steer_cmd;

/* make phi accessibel for serial com */
extern double phi;

void stabilizer_loop( void );


#endif /*STABILIZER_H_*/
