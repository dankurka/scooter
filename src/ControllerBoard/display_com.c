//////////////////////////////////////////////////////////////////
//
//   $Header: /cvs/Controller\040Board/AVR-Code/Testsuite/display_com.c,v 1.4 2007/09/23 22:56:59 rainer Exp $
//   $Source: /cvs/Controller\040Board/AVR-Code/Testsuite/display_com.c,v $
//   $Revision: 1.4 $
//
//////////////////////////////////////////////////////////////////
//
// Project: Controllerboard of balancing Scooter, Testsuite
//
// Author: Rainer Ostendorf
//
// Description: Code for communication with the display unit
//
//	$Log: display_com.c,v $
//	Revision 1.4  2007/09/23 22:56:59  rainer
//	Rainer: added beeper, added some display com channels, modified tiltback, modified steering
//
//	Revision 1.3  2007/09/12 04:17:02  rainer
//	Rainer: Worked the weekend on software-features: Implemented display-communication, battery-monitoring, beeper support
//
//	Revision 1.2  2007/09/09 17:08:07  rainer
//	Rainer: Added battery voltage sampling, implemented some new ser-com objects
//
//	Revision 1.1  2007/09/04 02:40:33  rainer
//	Added display communication protocol
//
//
//
//
/////////////////////////////////////////////////////////////////

/* AVR - Includes, avr-libc needs to be installed and linked against */
#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>

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
#include "display_com.h"
#include "beeper.h"


/* array to save the received data to */
char com_data[33];
uint8_t com_datalen;


void handle_display_com(void)
{
	/* holds return value of uart-procedures */
	uint16_t buf;
	/* lower byte of getc() -> received char */
	uint8_t rec_char;
	/* higher byte of getc() -> uart status  */
	uint8_t rec_status;
	
	
	buf = uart1_getc();
	rec_char = (uint8_t) buf & 0xFF;
	rec_status = (uint8_t) (buf>>8);
	
	/* when no data is available: immediatly return */
	if( rec_status ==(UART_NO_DATA>>8)  )
	{
		return;
	}
	
	/* when we lost data: flush buffer and reset FSM */
	if( rec_status != 0 )
	{
		while( uart1_getc() > 0xFF )
			;
		com_state = idle;
		return;
	}
		
	
	switch(com_state)
	{
		case	idle:
				/* wait for SOT-character */
				if( rec_char == '<')
					com_state = got_sot;
		break;	
		case	got_sot:
				//uart1_puts("SOT");
				/* wait for command */
				if(	(rec_char == 'W') || (rec_char == 'R') )
				{
					/* save command */
					com_state = got_command;
					com_cmd = rec_char;
				}
				else
						com_state = idle;		
		break;
		case	got_command:
				//uart1_puts("cmd");
				/* wait for first seperator (between cmd and ch)  */
				if( rec_char == ' ')
						com_state = got_sep1;
				else
						com_state = idle;
		break;
		case	got_sep1:
				//uart1_puts("sep1");
				/* wait for channel number byte 1/3 */
				if( rec_char >= '0' && rec_char <='2' )
				{
					com_ch = (rec_char -'0')*100;
					com_state = got_channel_1;
				}
				else
					com_state = idle;
		break;
		case	got_channel_1:
				//uart1_puts("ch1");
				/* wait for channel number, byte 2/3 */
				if( rec_char >= '0' && rec_char <='9' )
					{
						com_ch += (rec_char -'0')*10;
						com_state = got_channel_2;
					}
				else
					com_state = idle;
		
		break;
		case	got_channel_2:
				//uart1_puts("ch2");
				/* wait for channel number, byte 3/3 */
				if( rec_char >= '0' && rec_char <='9' )
				{
					com_ch += (rec_char -'0');
					com_state = got_channel_3;
				}
				else
					com_state = idle;		
		break;
		case	got_channel_3: 
				//uart1_puts("ch3");
				/* wait for second seperator (between ch and val)  */
				if( rec_char == ' ')
				{
					com_datalen = 0;
					com_state = got_sep2;
				}
				else
					com_state = idle;
		break;
		case	got_sep2:
				//uart1_puts("sep2");
				/* wait for value */
				
				if( com_cmd == 'R')
				{
					if( rec_char == '-' )
						com_state = got_value;
					else
						com_state = idle;
				}
				else if( com_cmd == 'W' )
				{
					/* wait for value */
					if( rec_char == ']' )
					{
						com_state = got_eot;
						break;
					}
					/* save received byte */
					com_data[com_datalen] = rec_char;
					
				    if( com_datalen > 31 )
						com_state = idle;
		
					com_datalen++;
				}
		break;
		case	got_value:
				//uart1_puts("val");
				/* wait for eot */
				if( rec_char == ']' )
				{
					if(com_cmd == 'R')
						com_response_read(com_ch);
					else if(com_cmd == 'W')
						com_response_write(com_ch);
					
				}
				com_state = idle;
		break;
		default:
			com_state = idle;
		break;
	}
}

void com_response_read(uint8_t com_ch)
{
	
	uint16_t int_buf;
	int i;
	
	/* send <SOT> */
	uart1_putc('<');
	/* send status command */
	uart1_putc('S');
	/* send first seperator */
	uart1_putc(' ');
	/* print channel */
	uart1_putc( (com_ch/100)+'0' );
	uart1_putc( (com_ch%100)/10+'0' );
	uart1_putc( (com_ch%10)+'0' );
	/* print second seperator */
	uart1_putc(' ');
	
	
	/* send data */
	switch(com_ch)
	{
		case CH_MOT1_PWM:
			uart1_putc( '0'+ (__motor1_duty/100) );
			uart1_putc( '0'+ (__motor1_duty%100)/10 );
			uart1_putc( '0'+ (__motor1_duty%10) );
		break;
		
		case CH_MOT2_PWM:
			uart1_putc( '0'+ (__motor2_duty/100) );
			uart1_putc( '0'+ (__motor2_duty%100)/10 );
			uart1_putc( '0'+ (__motor2_duty%10) );
		break;
				
		case CH_MOT1_DIR:
			/* 0 or 1 */
			uart1_putc( '0' + (( PIND&(1<<6)) >>6) );
		break;
		
		case CH_MOT2_DIR:
			/* 0 or 1 */
			 uart1_putc( '0' + ((PIND&(1<<7))>>7) );
		break;
			
		case CH_MOT1_STATUS:
			/* 0 or 1 */
			uart1_putc( '0' + (( PINC&(1<<6)) >>6) );
		break;
		
		case CH_MOT2_STATUS:
			/* 0 or 1 */
			uart1_putc( '0' + (( PINC&(1<<7)) >>7) );
		break;
		
		case CH_ACCL:
			int_buf= ( accl_high_samples[accl_high_idx]-accl_low_samples[accl_low_idx] );
			uart1_putc( '0' + (int_buf / 1000)     );
			uart1_putc( '0' + (int_buf / 100 )%10  );
			uart1_putc( '0' + (int_buf / 10  )%10  );
			uart1_putc( '0' + (int_buf % 10  )     );
		break;
		
		case CH_GYR1_VAL:
			int_buf = gyro1_samples[gyro1_idx];
			uart1_putc( '0' + (int_buf / 1000)     );
			uart1_putc( '0' + (int_buf / 100 )%10  );
			uart1_putc( '0' + (int_buf / 10  )%10  );
			uart1_putc( '0' + (int_buf % 10  )     );
		break;				
			
		case CH_GYR2_VAL:
			int_buf = gyro2_samples[gyro2_idx];
			uart1_putc( '0' + (int_buf / 1000)     );
			uart1_putc( '0' + (int_buf / 100 )%10  );
			uart1_putc( '0' + (int_buf / 10  )%10  );
			uart1_putc( '0' + (int_buf % 10  )     );
		break;
		case CH_ADC1:
					/* this is the battery */
					/* sum up ringbuffer */
					int_buf = 0;
					for(i=0; i<BAT_SAMPLES_BUFSIZE; i++)
						int_buf+= battery_samples[i];
					/* divide by 16 */
					int_buf = int_buf>>4;
			
					uart1_putc( '0' + (int_buf / 1000)     );
					uart1_putc( '0' + (int_buf / 100 )%10  );
					uart1_putc( '0' + (int_buf / 10  )%10  );
					uart1_putc( '0' + (int_buf % 10  )     );
		break;
		
		case CH_CUR_SPEED:
			int_buf = cur_speed * 1000;
			uart1_putc( '0' + (int_buf / 1000)     );
			uart1_putc( '0' + (int_buf / 100 )%10  );
			uart1_putc( '0' + (int_buf / 10  )%10  );
			uart1_putc( '0' + (int_buf % 10  )     );
		break;
		
		case CH_SET_POINT:
			int_buf = set_point * 1000;
			uart1_putc( '0' + (int_buf / 1000)     );
			uart1_putc( '0' + (int_buf / 100 )%10  );
			uart1_putc( '0' + (int_buf / 10  )%10  );
			uart1_putc( '0' + (int_buf % 10  )     );
		break;
		
		case CH_PHI:
			int_buf = phi * 10000;
			uart1_putc( '0' + (int_buf / 1000)     );
			uart1_putc( '0' + (int_buf / 100 )%10  );
			uart1_putc( '0' + (int_buf / 10  )%10  );
			uart1_putc( '0' + (int_buf % 10  )     );
		break;
		
		case CH_STEER_ZERO:
			int_buf = steering_zero_point;
			uart1_putc( '0' + (int_buf / 1000)     );
			uart1_putc( '0' + (int_buf / 100 )%10  );
			uart1_putc( '0' + (int_buf / 10  )%10  );
			uart1_putc( '0' + (int_buf % 10  )     );
		break;
		
		/* ADC 4 is the steer */
		case CH_ADC4:
			int_buf = steering_potential;
			uart1_putc( '0' + (int_buf / 1000)     );
			uart1_putc( '0' + (int_buf / 100 )%10  );
			uart1_putc( '0' + (int_buf / 10  )%10  );
			uart1_putc( '0' + (int_buf % 10  )     );
		break;
		
		case CH_STEER_CMD:
			int_buf = steer_cmd * 1000;
			uart1_putc( '0' + (int_buf / 1000)     );
			uart1_putc( '0' + (int_buf / 100 )%10  );
			uart1_putc( '0' + (int_buf / 10  )%10  );
			uart1_putc( '0' + (int_buf % 10  )     );
		break;
		
		case CH_GYR1_TEMP:
		case CH_GYR2_TEMP:
		
		case CH_ADC2:
		case CH_ADC3:
		case CH_ADC5:
		case CH_ADC6:
		case CH_ADC7:
		case CH_ADC8:
		case CH_ADC9:
		case CH_ADC10:
		

		case CH_GYR1_ZERO:
		case CH_GYR_TO_OMEGA:
		case CH_ACCL_TO_DEGR:
		case CH_STEER_RESP:
		case CH_MAX_STEER:
		case CH_K_ANGLE:
		case CH_K_OMEGA:
		case CH_K_CORR:

		case CH_CURSPEED_FAC:
								
		case CH_FWVER:
		case CH_SERNR:
		case CH_SYSTIME_MIL:
		case CH_SYSTIME_SEC:
		case CH_SYSTIME_MIN:
		case CH_SYSTIME_H:		
				
		default:
			uart1_putc('-');
		break;
	}
	
	
	/* send EOT */
	uart1_putc(']');
	return;
	
}

void com_response_write(uint8_t com_ch)
{
	
	switch(com_ch)
	{
		case CH_BEEPER:
			if( com_data[0] == '1' )
				beeper_on();
			if( com_data[0] == '0' )
				beeper_off();
		break;
		
		
		default:
		break;
	}
	
	return;
}






