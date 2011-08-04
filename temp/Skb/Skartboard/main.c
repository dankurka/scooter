//////////////////////////////////////////////////////////////////
//
//   $Header: /cvs/Controller\040Board/AVR-Code/Testsuite/main.c,v 1.22 2007/07/21 10:09:24 rainer Exp $
//   $Source: /cvs/Controller\040Board/AVR-Code/Testsuite/main.c,v $
//   $Revision: 1.22 $
//
//////////////////////////////////////////////////////////////////
//
// Project: Controllerboard of balancing Scooter, Testsuite
//
// Author: Rainer Ostendorf
//
// Description: Main routine, overall program flow control
//
//	$Log: main.c,v $
//	Revision 1.22  2007/07/21 10:09:24  rainer
//	Aktuelle Testversion auf Segway No.2 (FH-Segway). Modzifiziert: Gyro-Offset
//
//	Revision 1.21  2007/06/25 19:58:04  rainer
//	Put controllers parameters to stability limit
//
//	Revision 1.20  2007/06/25 12:08:48  rainer
//	Added stable timebase for the stabilizer (beta)
//
//	Revision 1.19  2007/06/13 03:34:11  rainer
//	rainer, added realtime-clock. modified main
//
//	Revision 1.18  2007/06/11 17:37:57  rainer
//	Rainer: Changed stabilizer for first test drives. Currently no limit in PWM and no steering active.
//
//	Revision 1.17  2007/04/18 17:44:03  kurt
//	added steering support(beta)
//
//	Revision 1.16  2007/04/18 01:36:18  rainer
//	Regler angepasst, zum Testen RS232 Konsole eingebaut um Regler-Parameter zu variieren. Rainer.
//
//	Revision 1.15  2006/11/26 20:41:29  rainer
//	Changed polarity of PWM-Signal. Adjusted stabilization control parameters.
//
//	Revision 1.14  2006/11/24 22:04:28  rainer
//	Player around with real sensors and stabilization algorithm. Basic function seems to be ok, finetuning of factors to do.
//
//	Revision 1.13  2006/11/19 18:53:43  rainer
//	completed monitor-routine by adding gyro temperatures and PWM parameters
//
//	Revision 1.12  2006/11/18 18:50:44  rainer
//	Changed PWM-Mode from dual-slope-mode to fast, single-slope mode. Added security checks for PWM-timings (max frequence, min pulse-width) to protect the h-bridges.
//
//	Revision 1.11  2006/11/17 13:30:48  rainer
//	Moved bunch of entropy from main.h to helpers.h. See random[] there
//
//	Revision 1.10  2006/11/17 13:23:08  rainer
//	Edited functions for frequency-updates to the PWM: Now the Threshold value is written before the new TOP value is written. For this purpose, two new variables were introduced in motor_pwm.h holding the duty-cycle of each motor
//
//	Revision 1.9  2006/11/13 14:33:47  rainer
//	fixed typo
//
//	Revision 1.8  2006/11/12 22:44:17  rainer
//	Added serial debug monitor and support for IR-LED PWM-Signals.
//
//	Revision 1.7  2006/11/12 17:47:15  rainer
//	Added support for PWM-Duty-cycle measurement with Timer3. Tuned SPI-Code.
//
//	Revision 1.6  2006/11/12 02:13:06  rainer
//	Added stubs for Accelerometer. To be finished. Another day...
//
//	Revision 1.5  2006/11/12 00:59:42  rainer
//	Added support for interrupt-driven use of the Gyros. Cleaned up main() a bit.
//
//	Revision 1.4  2006/11/11 19:39:37  rainer
//	Moved test/demo routines to their modules implementation files.
//
//	Revision 1.3  2006/11/11 00:03:55  rainer
//	Successfully tested support for gyroscopes
//
//	Revision 1.2  2006/11/08 23:44:41  rainer
//	Verpeilung beim checkin behoben.
//
//	Revision 1.4  2006/11/08 23:31:36  rainer
//	Changes to PWM and SPI
//
//	Revision 1.2  2006/11/07 23:12:51  rainer
//	Added support for SPI. Tested SPI.
//
//	Revision 1.1  2006/11/06 22:38:42  rainer
//	Initial checkin.
//	Supported features: LEDs, UART1, Motor-PWM, ADC
//	Rainer. 11/06/2006
//
//
//
/////////////////////////////////////////////////////////////////
 
/* AVR - Includes, avr-libc needs to be installed and linked against */
#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h> /* for PSTR-Makro */

/* Local includes */
#include "main.h"
#include "gpio.h"
#include "helpers.h"
#include "uart.h"
#include "leds.h"
#include "motor_pwm.h"
#include "adc.h"
#include "gyro_spi.h"
#include "accel_capture.h"
#include "globals.h"
#include "monitor.h"
#include "irled_pwm.h"
#include "stabilizer.h"
#include "time_ref.h"

void print_status()
{
	/* say hello to the world .. */
	uart_puts_P("-- Controllerboard $Revision: 1.22 $ online --\n\r");
	uart_puts_P("Initialized UART-Interface: COM 1\n\r");
	uart1_puts_P(">Hello Display Controller]\n\r");
}  
int main( void )
{ 
		
	uint16_t steer_adc_val; /* buffer for the steering ADC value at startup */ 
	
	/* initialize all subsystems */
	init_port_directions();
	
	init_port_values();
	
	init_uart();
	print_status();	
	
	init_motor_pwm();
	set_motor_pwm_freq(14500);
	set_motor1( 0, 0 );
	set_motor2( 0, 0 );
	
	init_time_ref();
	uart_puts_P("Initialized system timer\n\r");
	
	uart_puts_P("Initialized Motor PWM\n\r");
	
	init_adc();
	uart_puts_P("Initialized AD-Converters\n\r");
	
	init_spi_gyro( 1 ); /* enable spi in interrupt-mode */
	uart_puts_P("Initialized Gyroskopes\n\r");
	
	init_accel_capture();
	uart_puts_P("Initialized Acclerometer\n\r");
	
	init_irled_pwm();
	uart_puts_P("Initialized Infread LEDs\n\r");
		
	clear_led(0);
	clear_led(1);
	clear_led(2);
	clear_led(3);
 
	/* get the current value of the steering */
	adc_sel_channel( ADC_STEERING_1 );
	steer_adc_val = read_adc();
	delay_ms(50);
	steering_zero_point = steer_adc_val;
	steer_adc_val = read_adc();
	steering_zero_point = (uint16_t)((steering_zero_point + steer_adc_val)/2); 
	
	/* start the engine */
	enable_motor1();
	enable_motor2();	
		
	while(1)
	{
		
		runtime_counter = 0;
		toggle_led(3);
		
		//delay_ms(1000);
		//monitor();
		
		stabilizer_loop();
		
		if(runtime_counter >= 10)
			set_led(0);
		while( runtime_counter < 10)
		{
		  ; /*nop*/
		}
		
		
				
	}
			
    return 0;
}

