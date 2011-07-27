//////////////////////////////////////////////////////////////////
//
//   $Header: /cvs/Controller\040Board/AVR-Code/Testsuite/motor_pwm.h,v 1.6 2007/06/11 17:37:57 rainer Exp $
//   $Source: /cvs/Controller\040Board/AVR-Code/Testsuite/motor_pwm.h,v $
//   $Revision: 1.6 $
//
//////////////////////////////////////////////////////////////////
//
// Project: Controllerboard of balancing Scooter, Testsuite
//
// Author: Rainer Ostendorf
//
// Description: Routines for initializing and controlling the PWM-Signal towards the motors
//
//	$Log: motor_pwm.h,v $
//	Revision 1.6  2007/06/11 17:37:57  rainer
//	Rainer: Changed stabilizer for first test drives. Currently no limit in PWM and no steering active.
//
//	Revision 1.5  2006/11/20 00:06:02  rainer
//	fixed doxygen comment
//
//	Revision 1.4  2006/11/18 18:50:44  rainer
//	Changed PWM-Mode from dual-slope-mode to fast, single-slope mode. Added security checks for PWM-timings (max frequence, min pulse-width) to protect the h-bridges.
//
//	Revision 1.3  2006/11/17 13:23:08  rainer
//	Edited functions for frequency-updates to the PWM: Now the Threshold value is written before the new TOP value is written. For this purpose, two new variables were introduced in motor_pwm.h holding the duty-cycle of each motor
//
//	Revision 1.2  2006/11/08 23:44:41  rainer
//	Verpeilung beim checkin behoben.
//
//	Revision 1.4  2006/11/08 23:31:35  rainer
//	Changes to PWM and SPI
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
 *  @defgroup ro_motor_pwm  Motor PWM
 *  @code #include <motor_pwm.h> @endcode
 * 
 *  @brief Routines for controlling PWM-Signals towards the Motors
 *
 * 
 * This Routines are for setting the PWMs base frequency and for
 * scaling the modulated duty cycle. 
 *   
 *  @note Based on Atmel Datasheet for Atmega128, Nov. 2006
 *  @author Rainer Ostendorf
 */

/**@{*/


#ifndef MOTOR_PWM_H_
#define MOTOR_PWM_H_


/* Number of steps the PWM duty-cycle is 
 * divided to. Typically 255
 */
#define PWM_RESOLUTION 255

/* SECURITY FEATURE:
 * cut-off frequency to protect the h-bridges from
 * beeing stressed to much by high pwm-frequencies.
 * any frequency above this will be mapped to PWM_SECURITY_MAX_FREQ.
 * Typically about 10000 to 15000 Hz.
*/
#define PWM_SECURITY_MAX_FREQ 15000

/* SECURITY FEATURE:
 * minimal active time of the PWM signal.
 * To avoid the active-times of the PWM-signal
 * to get to short, as that increases the power dissipation
 * to a critical level (FETs running in analogue mode) */
#define PWM_SECURITY_MIN_DUTY (PWM_RESOLUTION / 200)

/** Internal variables for motor-dutycycles. */
/** Please dont touch these values, as they are used by the
 * set_pwm_frequency-method to adjust the theshold (OCRx)
 * values, after a update to the TOP-value (ICP) of the counter was performed 
 */ 
uint8_t __motor1_duty;
uint8_t __motor2_duty;

/** Initialize the PWM-Signal generation for the both motors */
void init_motor_pwm( void );

/** Set the base frequency for the PWM signal 
 * @param hz frequency in Hertz
 */
void set_motor_pwm_freq(uint16_t hz);

/** set enable-signal of motor 1 to high */
inline void enable_motor1( void );

/** set enable-signal of motor 2 to high */
inline void enable_motor2( void );

/** set enable-signal of motor 1 to low */
inline void disable_motor1( void );

/** set enable-signal of motor 2 to low */
inline void disable_motor2( void );


/** Set the duty-cycle of motor 1 
 * @param speed Percentage of duty (high) level during clock cycle
 * 				the maximum speed is PWM_RESOLUTION (typlicalle 255),
 * 				the minimum speed is 0.
 * @param dir Direction of the motor. Either 0(forward) or 1(backward)
 */
inline void set_motor1( uint8_t speed, uint8_t dir );

/** Set the duty-cycle of motor 2 
 * @param speed Percentage of duty (high) level during clock cycle
 * 				the maximum speed is PWM_RESOLUTION (typlicalle 255),
 * 				the minimum speed is 0.
 * @param dir Direction of the motor. Either 0 (forward) or 1(backward) 
 */
inline void set_motor2( uint8_t speed,  uint8_t dir );


#endif /*MOTOR_PWM_H_*/

/**@}*/
