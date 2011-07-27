//////////////////////////////////////////////////////////////////
//
//   $Header: /cvs/Controller\040Board/AVR-Code/Testsuite/stabilizer.c,v 1.22 2009/03/03 03:04:05 kurt Exp $
//   $Source: /cvs/Controller\040Board/AVR-Code/Testsuite/stabilizer.c,v $
//   $Revision: 1.22 $
//
//////////////////////////////////////////////////////////////////
//
// Project: Controllerboard of balancing Scooter, Testsuite
//
// Author: Original C167 code by segway team, adaption to AVR by Rainer Ostendorf
//
// Description: controller algrorithm for platform stabilization
//
//	$Log: stabilizer.c,v $
//	Revision 1.22  2009/03/03 03:04:05  kurt
//	enabled debug by default
//
//	Revision 1.21  2007/09/25 07:36:56  rainer
//	Rainer: Parameter for Tiltback adjusted.
//
//	Revision 1.20  2007/09/24 21:59:05  rainer
//	Rainer: Adapted stabilizer-parameters for improved stability. State of Software before segway-presentation (IHK) on 8.Nov.2007
//
//	Revision 1.19  2007/09/23 22:56:59  rainer
//	Rainer: added beeper, added some display com channels, modified tiltback, modified steering
//
//	Revision 1.18  2007/09/15 12:33:24  rainer
//	Rainer: Added Tilt-Back. Beta! Works but needs to be optimized. Note cur_speed is negative when running forward!
//
//	Revision 1.17  2007/09/12 04:17:02  rainer
//	Rainer: Worked the weekend on software-features: Implemented display-communication, battery-monitoring, beeper support
//
//	Revision 1.16  2007/09/09 17:08:07  rainer
//	Rainer: Added battery voltage sampling, implemented some new ser-com objects
//
//	Revision 1.15  2007/09/04 02:40:33  rainer
//	Added display communication protocol
//
//	Revision 1.14  2007/07/21 10:09:24  rainer
//	Aktuelle Testversion auf Segway No.2 (FH-Segway). Modzifiziert: Gyro-Offset
//
//	Revision 1.13  2007/06/25 19:58:04  rainer
//	Put controllers parameters to stability limit
//
//	Revision 1.12  2007/06/25 12:08:48  rainer
//	Added stable timebase for the stabilizer (beta)
//
//	Revision 1.11  2007/06/13 03:34:11  rainer
//	rainer, added realtime-clock. modified main
//
//	Revision 1.10  2007/06/11 17:37:57  rainer
//	Rainer: Changed stabilizer for first test drives. Currently no limit in PWM and no steering active.
//
//	Revision 1.9  2007/04/18 18:01:59  kurt
//	changed steering direction
//
//	Revision 1.8  2007/04/18 17:44:03  kurt
//	added steering support(beta)
//
//	Revision 1.7  2007/04/18 04:18:16  kurt
//	*** empty log message ***
//
//	Revision 1.6  2007/04/18 01:40:14  rainer
//	cur_speed hinzugefügt
//
//	Revision 1.5  2007/04/18 01:36:18  rainer
//	Regler angepasst, zum Testen RS232 Konsole eingebaut um Regler-Parameter zu variieren. Rainer.
//
//	Revision 1.4  2006/11/26 20:41:29  rainer
//	Changed polarity of PWM-Signal. Adjusted stabilization control parameters.
//
//	Revision 1.3  2006/11/24 22:04:28  rainer
//	Player around with real sensors and stabilization algorithm. Basic function seems to be ok, finetuning of factors to do.
//
//	Revision 1.2  2006/11/20 00:12:26  rainer
//	fixed typo
//
//	Revision 1.1  2006/11/20 00:04:46  rainer
//	Initial checkin. stabilization control algorithm, ported from C167. not yet tested.
//
//
//
//
/////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>

#include <inttypes.h>
#include "globals.h" /* there the ringbuffers for sensor-data are defined */
#include "stabilizer.h" /* constants and scaling factor for controller-algorithm */
#include "adc.h" /* for reading the steering potentiometer */
#include "motor_pwm.h" /* for PWM resolution */
#include "leds.h" /* for turning on led 2 when starting */
#include "uart.h"
#include "beeper.h"

#define STABILIZE_DEBUG 1

#ifdef STABILIZE_DEBUG
#include "uart.h"
#endif


#ifdef STABILIZE_DEBUG
#define DEBUG_OUTPUT_RATE 100 /* output every n'th run */
int usecount;
#endif







double k_angle = K_ANGLE ; /* factor of angle in "balance" calculation */
double k_omega = K_OMEGA; /* factor of omega in "balance" calculation */
double cur_speed_factor = CUR_SPEED_FACTOR; /* factor of omega in "balance" calculation */
double phi=0.0;


char first_entry = 1; /*ersters mal in der Reglerroutine bis der Motor starten darf*/
char motor_start = 0; /* are we allowed to start the engines ? */

/* value holding the current speed of the device */  
double cur_speed=0.0;

/* analogue value of the steering potentiometer */
int steering_potential;

/* difference between zero-point and potential */
int steering_diff;

/* difference between zero-point and minimum or maximum */
int steering_range;

/* set value after the steering command was applied to it */
double steer_cmd=0.0;

/* variable to save the maximum steering ADV value in */
int steering_max;
/* variable to save the maximum steering ADV value in */
int steering_min;

/* overspeed, to limit speed by tilting back */
double overspeed=0.0;

/* overspeed-integral, to limit speed by tilting back */
double overspeed_integral=0.0;

/* set-point, the scooter tries to stasbilize to this value */
double set_point = SET_POINT;

void stabilizer_loop( void )
{
		/* *****************************  V A R I A B L E S ******************/
		/* the two main physical values:
		 * 
		 * 	- the angle of the tilt, the platform has 
		 * 	  against the direct of gravitiy
		 *  
		 *  - the angular speed of the platform around the 
		 *    the axle of the wheels
		 * */
		double tilt_degrees;
		double angular_speed;
		
		/* average value of the first gyroskope, 
		 * calculated out of the current values in the
		 * gyro1 ringbuffer
		 */
		int16_t gyro1_average=0;
		
		/* average value of the accelerometer, 
		 * calculated out of the current values in the
		 * acclerometer ringbuffer
		 */
		uint32_t accel_high_average=0;
		uint32_t accel_low_average=0;
		
		/* PHI-value of kalman filter, estimate/predict the next position */


		/* Delta t: Time interval between a loop execution */
		double delta_t = 0.01; /* 10 milliseconds */ 
		
		/* set value (actuating variable) of controlling algorithm */  
		double balance=0.0;
		
		/* PWM duty value for motor 1 */
		double motor1_duty=0.0;
		uint8_t motor1_dir=0;
		/* PWM duty value for motor 2 */
		double motor2_duty=0.0;
		uint8_t motor2_dir=0;
		
		/* index variable, for summing up ringbuffer-values */
		uint8_t rb_idx;
		
		
		
		/* buffer for external incoming command */
		#ifdef STABILZE_DEBUG
		unsigned char inkey=0;
		#endif /* STABILIZE_DEBUG */
		
		//char buf[64]; /* buffer for debug output */
		
		#ifdef STABILIZE_DEBUG
			char buf[64]; /* buffer for debug output */
		#endif
		
		/* *****************************  C O D E ******************/
		/* controller algorithm starts here */
		
		/* get the current value of the steering */
		adc_sel_channel( ADC_STEERING_1 );
		steering_potential = (int)read_adc();
		
		/* remember the maximum steering value */
		if( steering_potential < steering_min)
		{
			steering_min = steering_potential;
		}
		else if( steering_potential > steering_max)
		{
			steering_max = steering_potential;
		}


		
		/* 
		 * 1st STEP: calculate the current average values of the sensors 
		 * */
		
		/* Accelerometer average: */
		accel_high_average = 0;
		accel_low_average = 0; 
		for( rb_idx = 0; rb_idx < ACCL_SAMPLES_BUFSIZE; rb_idx++ )
		{
			accel_high_average += 	accl_high_samples[ rb_idx ];
			accel_low_average +=    accl_low_samples[ rb_idx ];
		}
		/* optimization: replace division by rightshift. currently works only for Bufsize 8!
		 * ist the same as: accel_average = accel_average / ACCL_SAMPLES_BUFSIZE; */
		accel_high_average = (accel_high_average >> 3);
		accel_low_average = (accel_low_average >> 3);
		
		/* Gyroscope1 average: */
		gyro1_average = 0;
		for( rb_idx = 0; rb_idx < GYRO1_SAMPLES_BUFSIZE; rb_idx++ )
		{
			if( gyro1_samples[rb_idx] & 0x0800 )
			{
					gyro1_samples[rb_idx] |= 0xF000;
			}				
			gyro1_average += gyro1_samples[ rb_idx ];
		}
		/* optimization: replace division by rightshift. currently works only for Bufsize 8! 
		 * gyro1_average = gyro1_average / GYRO1_SAMPLES_BUFSIZE; */
		gyro1_average = (gyro1_average >> 3);
		
		#ifdef STABILIZE_DEBUG
			if(!(usecount%DEBUG_OUTPUT_RATE) ) /* output every 1000 run */
			{
				snprintf(buf,64,"\n\r1# G_avg %d, A_h %d A_l %d\n\r", 
						gyro1_average, (int)accel_high_average, (int)accel_low_average ); 
				uart_puts(buf);
			}
		#endif
		
		/*
		 *  2nd STEP: scale the sensor-values to correct physical values 
		 * */
		
		/* calculate tilt degree from acclerometer value */
		/* TODO: sin(x) seems to be appromimates by 1.0 ? in case: add this as info/comment for clarity */
		
		#ifdef SERIALNO_3
		tilt_degrees =  ( ((double)accel_high_average/ ( (double)accel_high_average+(double)accel_low_average))  - 0.5) * ACCL_TO_DEGREE_FACTOR ;
		#else
		tilt_degrees =  ( ((double)accel_high_average/ ( (double)accel_high_average+(double)accel_low_average))  - 0.5) * ACCL_TO_DEGREE_FACTOR + 0.58;
		#endif
		/* calculate angular speed from gyroskop1 value */
		angular_speed = ((double)gyro1_average - GYRO1_ZERO_VALUE) * GYRO_TO_OMEGA_FACTOR;
		#ifdef STABILIZE_DEBUG
		if(!(usecount%DEBUG_OUTPUT_RATE) ) /* output every 1000 run */
		{
			snprintf(buf, 64, "2# t: %d, s %d\n\r", (int)(tilt_degrees*1000), (int)(angular_speed*1000));
			uart_puts(buf);
		}
		#endif
		
		
		/* 
		 * 3rd STEP: process kalman step, predict new tilt angle 
		 * */
		/* ANNOTATION: The correction factor "k_korrekt" was 1.0, so it was omitted.
		 * it is define as  K_CORR_FACTOR in stabilizer.h and can be added, if needed 
		 * */
		phi += (angular_speed + (tilt_degrees - phi)* K_CORR_FACTOR) * delta_t;
		/* limit kalman output */
		if( phi > (PI/4.0) )
			phi = (PI/4.0);
		else if( phi < ( (-1.0)*(PI/4.0) ) )
			phi = (-1)*(PI/4.0);
		
		#ifdef STABILIZE_DEBUG
		if(!(usecount%DEBUG_OUTPUT_RATE) ) /* output every 1000 run */
		{
			snprintf(buf, 64, "3# phi: %d\n\r", 
				(int)(phi*1000) );
			uart_puts(buf);
		}
		#endif
		/*
		 * 4th STEP: calculate new setting value from predicted tilt angle and set point
		 */
		/* calculate new value as a weighted sum of tilt-angle and angular speed */
		balance = ( k_angle *( phi + set_point )) + ( k_omega * angular_speed);
		
		/* update current speed */
		cur_speed += cur_speed_factor * balance * delta_t;
		if(cur_speed > 1.0)
			cur_speed = 1.0;
		else if(cur_speed < -1.0)
			cur_speed = -1.0;
		/* add current speed to set value */
		
		/** Tilt back mechanism */		
		if( cur_speed < (-1.0)*TILT_BACK_SPEED )
		{
				overspeed = ((-1)*cur_speed) - TILT_BACK_SPEED;
				
				/* there is a step when integration starts
				 * this step determines the strength of the pulse back,
				 * when the top speed is reached */
				
				overspeed_integral += (overspeed+0.005)*delta_t;
				
				if( overspeed_integral < 0.1 )
					overspeed_integral = 0.1;
				else if ( overspeed_integral > 5.0 )
					overspeed_integral = 5.0;
				
				beeper_on();
				
		}
		else
		{
			overspeed = 0.0;
			overspeed_integral = overspeed_integral - 0.2*delta_t;
			if( overspeed_integral < 0.0)
				overspeed_integral = 0.0;
			beeper_off();
		}
		
		set_point = SET_POINT - 0.3*overspeed - 0.2*overspeed_integral;
		/* 0.017 rad == ca. 1 degree */
		if( set_point < -0.3  )
			set_point = -0.3;
		else if( set_point > 0.3  )
				set_point = 0.3;
		
		if(motor_start)
		{
			balance += cur_speed;
		}else{
			cur_speed = 0.0;
		}
		
			
		#ifdef STABILIZE_DEBUG
		if(!(usecount%DEBUG_OUTPUT_RATE) ) /* output every n'th run */
		{
			snprintf(buf, 64, "4# bal: %d, cur:%d\n\r", 
				(int)(balance*1000),(int)(cur_speed*1000) );
			uart_puts(buf);
		}
		#endif
		/*
		 * 5th STEP: apply steering, limit output, set motors
		 */
		//decrease steering with velocity
		
		steering_diff = steering_potential - steering_zero_point;
		if( steering_potential > steering_zero_point )
		{
			steering_range = steering_max - steering_zero_point; 	
		}
		else
		{
			steering_range = steering_zero_point - steering_min; 			
		}
		
		steer_cmd = (double)((double)steering_diff/(double)steering_range);
		steer_cmd *= MAX_STEERING_LOCK;
		
		if(cur_speed > 0.0)
			steer_cmd = (steer_cmd)/(0.3+cur_speed);
		else
			steer_cmd = (steer_cmd)/(0.3-cur_speed);
						
		/* apply the steer command to the set value for each wheel */
		motor1_duty = (balance + steer_cmd); 
		motor2_duty = (balance - steer_cmd); 
		
		/* Only start motors if the output is less than 5% */
		
		if(first_entry)
		{
			if( (motor1_duty < 0.05) && (motor1_duty > -0.05) && (motor2_duty < 0.05) && (motor2_duty > -0.05)
				&& ( steering_max > (steering_potential+20) ) && ( steering_min < (steering_potential-20) )	) 
			{
					motor_start = 1;
					first_entry = 0;
					/* start the engine */
					enable_motor1();
					enable_motor2();
					set_led(2);
			}			
		} 
		#ifdef STABILIZE_DEBUG
		usecount++;
		#endif 
		if(!motor_start)
			return;
		
		/* cut the set value to maximum duty cycle (100%, 1.0) */
		if( motor1_duty > MAX_FORWARD_DUTY ) /* limit foward speed 1*/ 
			motor1_duty = MAX_FORWARD_DUTY;
		
		else if( motor1_duty < 0.0 )
		{  
			motor1_dir = 1; /* drive M1 backward */
			if( motor1_duty < MAX_BACKWARD_DUTY ) /* limit backward speed 1 */ 
				motor1_duty = MAX_BACKWARD_DUTY;
			motor1_duty *= -1.0; /* duty  must be positive */
		}	
		
		if( motor2_duty > MAX_FORWARD_DUTY ) /* limit forward speed 2 */
			motor2_duty = MAX_FORWARD_DUTY;
		else if( motor2_duty < 0.0 )
		{
			motor2_dir = 1; /* drive M2 backward */ 
			if( motor2_duty < MAX_BACKWARD_DUTY )
				motor2_duty = MAX_BACKWARD_DUTY; /* limit backwart speed 2 */
			motor2_duty *= -1.0; /* duty  must be positive */
		}
		
		
		
		#ifdef STABILIZE_DEBUG
		if(!(usecount%DEBUG_OUTPUT_RATE) ) /* output every n'th run */
		{
			snprintf(buf, 64, "5# M1: %d,%d M2 %d,%d\n\r", 
				(int)(motor1_duty*PWM_RESOLUTION), motor1_dir,(int)(motor2_duty*PWM_RESOLUTION), motor2_dir);
			uart_puts(buf);
		}
		#endif
		
		/* set the motors PWM signal */
		set_motor1( (uint8_t)(motor1_duty*PWM_RESOLUTION), motor1_dir );
		set_motor2( (uint8_t)(motor2_duty*PWM_RESOLUTION), motor2_dir );
		
		
}

