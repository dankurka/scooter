//////////////////////////////////////////////////////////////////
//
//   $Header: /cvs/Controller\040Board/AVR-Code/Testsuite/motor_pwm.c,v 1.11 2007/06/11 17:37:57 rainer Exp $
//   $Source: /cvs/Controller\040Board/AVR-Code/Testsuite/motor_pwm.c,v $
//   $Revision: 1.11 $
//
//////////////////////////////////////////////////////////////////
//
// Project: Controllerboard of balancing Scooter, Testsuite
//
// Author: Rainer Ostendorf
//
// Description: Routines for initializing and controlling the PWM-Signal towards the motors
//
//	$Log: motor_pwm.c,v $
//	Revision 1.11  2007/06/11 17:37:57  rainer
//	Rainer: Changed stabilizer for first test drives. Currently no limit in PWM and no steering active.
//
//	Revision 1.10  2007/04/18 01:36:18  rainer
//	Regler angepasst, zum Testen RS232 Konsole eingebaut um Regler-Parameter zu variieren. Rainer.
//
//	Revision 1.9  2006/12/18 22:57:37  rainer
//	Disabled motors by default in the init routine. Swapped enable- and disable_motor functions. As the motor-enable lines are low-active
//	code was wrong.
//
//	Revision 1.8  2006/11/26 20:41:29  rainer
//	Changed polarity of PWM-Signal. Adjusted stabilization control parameters.
//
//	Revision 1.7  2006/11/18 18:50:44  rainer
//	Changed PWM-Mode from dual-slope-mode to fast, single-slope mode. Added security checks for PWM-timings (max frequence, min pulse-width) to protect the h-bridges.
//
//	Revision 1.4  2006/11/12 22:44:17  rainer
//	Added serial debug monitor and support for IR-LED PWM-Signals.
//
//	Revision 1.3  2006/11/12 00:59:42  rainer
//	Added support for interrupt-driven use of the Gyros. Cleaned up main() a bit.
//
//	Revision 1.2  2006/11/08 23:44:41  rainer
//	Verpeilung beim checkin behoben.
//
//	Revision 1.3  2006/11/08 23:31:35  rainer
//	Changes to PWM and SPI
//
//	Revision 1.1  2006/11/06 22:38:42  rainer
//	Initial checkin.
//	Supported features: LEDs, UART1, Motor-PWM, ADC
//	Rainer. 11/06/2006
//
//
//
/////////////////////////////////////////////////////////////////


#include <avr/io.h>

#include "main.h" /* for SYS_CLK */
#include "motor_pwm.h"


/** Initialize the PWM-Signal generation for the both motors */
void init_motor_pwm( void )
{
	/* set the direction signals for the motors as output */
	DDRD = DDRD | (1<<7) | (1<<6);
	
	/* set the enable signals for the motors as output */
	PORTC = PORTC | (1<<6) | (1<<7); /* for safety: disable low active motor enable-signals */
	DDRC = DDRC | (1<<7) | (1<<6);  
	
	/* Set the Timer 1 Control Register A */
	/* COM1A1 = 1, COM1A0 = 1 ,   
	 * COM1B1 = 1, COM1B0 1,
	 * COM1C1 = 0, COM1C0 = 0,
	 * 		PWM-Pins set on compare-match when upcounting, cleared when downcounting 
	 * OC1C disabled, normal operation 
	*  WGM11 = 1, WGM10 = 0 : fast pwm mode, ICR1 is TOP 
	*/
	TCCR1A = (1<<COM1A1) | (1<<COM1A0) | (1<<COM1B1) | (1<<COM1B0) | (1<<WGM11)  ;
	
	/* Set Timer1 Control Register B */
	/* ICNC1 = 0 - cause noise canceler inactive 
	 * ICES1 = 0 - input capture disabled
	 * WGM13  = 1, WGM12 = 1
	 * CS12=0, CS11=0, CS10=1 - no Prescaler, direct Clock (16Mhz) for counting 
	 */
	 TCCR1B = (1<<WGM13) | (1<<WGM12) | (1<<CS10);
	 
	/* Set the counter register to zero, so counter will start at 0, counting upwards*/
	TCNT1 = 0x0000;
	/* Set the output compare register 1A to 0 */
	OCR1A = 0;
	/* Set the output compare register 1A to 0 */
	OCR1B = 0;
	/* set the ICR1 Register to 0 (TOP) */
	ICR1 = 0;
	
	/* set the PWM-Pins as output */
	DDRB = DDRB | (1<<5) | (1<<6);
	
}

/** Set the base frequency for the PWM signal 
 * @param hz frequency in Hertz
 */
void set_motor_pwm_freq(uint16_t hz)
{
	if( hz > PWM_SECURITY_MAX_FREQ )
		hz = PWM_SECURITY_MAX_FREQ;
	uint16_t top =	(SYS_CLK / (hz) );
	OCR1A = ( top / PWM_RESOLUTION ) * __motor1_duty;
	OCR1B = ( top / PWM_RESOLUTION ) * __motor2_duty;
	ICR1 = top;	
		
}

inline void disable_motor1( void )
{
	PORTC = PORTC | (1<<6);
}

inline void enable_motor1( void )
{
	PORTC = PORTC & ~(1<<6);
}

inline void disable_motor2( void )
{
	PORTC = PORTC | (1<<7);
}

inline void enable_motor2( void )
{
	PORTC = PORTC & ~(1<<7);
}

inline void set_motor1( uint8_t speed, uint8_t dir )
{
	/* set direction flag */
	if( dir )
		PORTD = PORTD & ~(1<<6);
	else
		PORTD = PORTD | (1 << 6);
		
	/* security check to avoid the active-times of the PWM-signal
	 * to get to short, as that increases the power dissipation
	 * to a critical level (FETs running in analogue mode) */
	if( speed < PWM_SECURITY_MIN_DUTY )
		speed = 0;
		
	/* set duty cycle scaled to PWM cycle time */
	OCR1A = (ICR1 / PWM_RESOLUTION ) * speed;
	__motor1_duty = speed;
}

inline void set_motor2( uint8_t speed, uint8_t dir )
{
	/* set direction flag */
	if(!dir)
		PORTD = PORTD & ~(1<<7);
	else
		PORTD = PORTD | (1 << 7);
	
	/* security check to avoid the active-times of the PWM-signal
	 * to get to short, as that increases the power dissipation
	 * to a critical level (FETs running in analogue mode) */
	if( speed < PWM_SECURITY_MIN_DUTY )
		speed = 0;
	
	/* set duty cycle scaled to PWM cycle time */
	OCR1B = (ICR1 / PWM_RESOLUTION ) * speed;
	__motor2_duty = speed;
}
