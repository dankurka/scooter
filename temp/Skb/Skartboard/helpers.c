//////////////////////////////////////////////////////////////////
//
//   $Header: /cvs/Controller\040Board/AVR-Code/Testsuite/helpers.c,v 1.2 2006/11/18 18:50:44 rainer Exp $
//   $Source: /cvs/Controller\040Board/AVR-Code/Testsuite/helpers.c,v $
//   $Revision: 1.2 $
//
//////////////////////////////////////////////////////////////////
//
// Project: Controllerboard of balancing Scooter, Testsuite
//
// Author: Rainer Ostendorf
//
// Description: Small helper-routines, eg. delays, conversions..
//
//	$Log: helpers.c,v $
//	Revision 1.2  2006/11/18 18:50:44  rainer
//	Changed PWM-Mode from dual-slope-mode to fast, single-slope mode. Added security checks for PWM-timings (max frequence, min pulse-width) to protect the h-bridges.
//
//	Revision 1.1  2006/11/08 12:49:54  rainer
//	Umsortiert. VHDL-Daten bitte jetzt nach VHDL-Code
//
//	Revision 1.1  2006/11/06 22:38:42  rainer
//	Initial checkin.
//	Supported features: LEDs, UART1, Motor-PWM, ADC
//	Rainer. 11/06/2006
//
//
//
/////////////////////////////////////////////////////////////////





/* delay program executing for ms milliseconds by doing busy waiting
 * 
 * @param ms delay in ms
 * @return void
 */
void delay_ms( unsigned int ms ) 
{
    unsigned int  i;
    
    while(ms)
    { 
	    for ( i = 0x37e;  i;  i-- )
    	    { 
        	}
        ms--;
    }
   
}
