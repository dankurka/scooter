//////////////////////////////////////////////////////////////////
//
//   $Header: /cvs/Controller\040Board/AVR-Code/Testsuite/monitor.c,v 1.7 2007/06/13 03:34:11 rainer Exp $
//   $Source: /cvs/Controller\040Board/AVR-Code/Testsuite/monitor.c,v $
//   $Revision: 1.7 $
//
//////////////////////////////////////////////////////////////////
//
// Project: Controllerboard of balancing Scooter, Testsuite
//
// Author: Rainer Ostendorf
//
// Description: Routines for manipulating variables and showing system status
//
//	$Log: monitor.c,v $
//	Revision 1.7  2007/06/13 03:34:11  rainer
//	rainer, added realtime-clock. modified main
//
//	Revision 1.6  2007/04/18 17:44:03  kurt
//	added steering support(beta)
//
//	Revision 1.5  2007/04/18 01:36:18  rainer
//	Regler angepasst, zum Testen RS232 Konsole eingebaut um Regler-Parameter zu variieren. Rainer.
//
//	Revision 1.4  2006/11/24 22:04:28  rainer
//	Player around with real sensors and stabilization algorithm. Basic function seems to be ok, finetuning of factors to do.
//
//	Revision 1.3  2006/11/19 19:15:29  rainer
//	formatting + fixed a little bug
//
//	Revision 1.2  2006/11/19 18:53:43  rainer
//	completed monitor-routine by adding gyro temperatures and PWM parameters
//
//	Revision 1.1  2006/11/12 22:44:17  rainer
//	Added serial debug monitor and support for IR-LED PWM-Signals.
//
//
//
//
/////////////////////////////////////////////////////////////////

#include <stdlib.h>
#include <stdio.h>


#include "main.h" /* for SYS_CLK */
#include "motor_pwm.h" /* for Motor-values */
#include "monitor.h"
#include "adc.h"
#include "globals.h"
#include "time_ref.h" 

void monitor()
{
		int i;
		int val;
		char buf[BUFLEN];
		
		
		/* read temperature */
		//read_gyro_temp = 1;
		/* wait for gyro temp to be read */
		//while( read_gyro_temp )
		//	;
		
		if( gyro1_temp & 0x0800 )
			{
					gyro1_temp |= 0xF000;
					gyro1_temp = gyro1_temp-1;
					gyro1_temp = ~gyro1_temp;
			}
			else
				gyro1_temp &= 0x0FFF;
		if( gyro2_temp & 0x0800 )
			{
					gyro2_temp |= 0xF000;
					gyro2_temp = gyro2_temp-1;
					gyro2_temp = ~gyro2_temp;
			}
		else
			gyro2_temp &= 0x0FFF;
			
			
		/* clear VT100 screen */
		snprintf(buf,BUFLEN, "%c[2J", 27); 
		uart_puts_P("\n\r\n\r ############################Scooter Controllerboard Debug Monitor##########################\n\r");
		
		/* print gyro temps */
		uart_puts_P("\n\r------- Gyro temperature -------\n\r");
		snprintf(buf, BUFLEN, "Temp: Gyro1 %u\tGyro2 %u\n\r", gyro1_temp, gyro2_temp );
		uart_puts(buf);
		/* printf Gyro1 Ringbuffer */
		
		uart_puts_P("\n\r----------RINGBUFFERS------------\n\r");
		uart_puts_P("Index\tGyro 1\tGyro 2\tAccel.\n\r");
		
		/*
		for(i=0; i<GYRO1_SAMPLES_BUFSIZE; i++)
		{
			if( gyro1_samples[i] & 0x0800 )
			{
					gyro1_samples[i] |= 0xF000;
					gyro1_samples[i] = gyro1_samples[i]-1;
					gyro1_samples[i] = ~gyro1_samples[i];
			}
			else
				gyro1_samples[i] &= 0x0FFF;
		}
		for(i=0; i<GYRO2_SAMPLES_BUFSIZE; i++)
		{
			if( gyro2_samples[i] & 0x0800 )
			{
					gyro2_samples[i] |= 0xF000;
					gyro2_samples[i] = gyro2_samples[i]-1;
					gyro2_samples[i] = ~gyro2_samples[i];
			}
			else
				gyro2_samples[i] &= 0x0FFF;
		}
		*/
		for(i=0; i< GYRO1_SAMPLES_BUFSIZE; i++)
		{
			snprintf(buf, BUFLEN, "%u\t%d\t%d\t%d\n\r", \
					i, gyro1_samples[i], gyro2_samples[i], (accl_high_samples[i]-accl_low_samples[i]) );
			uart_puts(buf);
		}
		uart_puts_P("\n\r---------ADC-Channels------------\n\r");
		uart_puts_P("Channel\tValue\tVoltage\n\r");
		for(i=0; i<8; i++)
		{
				adc_sel_channel(i);
				val = read_adc();
				snprintf(buf, BUFLEN, "%d\t%u\t%d mV\n\r", i, val, (val*5) );
				uart_puts(buf);
		}
		uart_puts_P("\n\r---------Motor PWM------------\n\r");
		snprintf(buf, BUFLEN, "Motor Frequency: %ld Hz\n\r", (SYS_CLK/ICR1) );
		uart_puts( buf );
		snprintf(buf, BUFLEN, "Motor 1: %d/%d (%d %%)\tDIR %d\t\n\r", \
			__motor1_duty, PWM_RESOLUTION, (__motor1_duty*100)/PWM_RESOLUTION, PORTD & (1<<6)  );
		uart_puts( buf );
		snprintf(buf, BUFLEN, "Motor 2: %d/%d (%d %%)\tDIR %d\n\r", \
			__motor2_duty, PWM_RESOLUTION, (__motor2_duty*100)/PWM_RESOLUTION , PORTD & (1<<7) );
		uart_puts( buf ); 
		uart_puts_P("\n\r--------- Systime ------------\n\r");
		snprintf(buf, BUFLEN, "%d:%d:%d:%d", \
					systime_h, systime_min, systime_sec, systime_millisec );
		uart_puts( buf );
}

