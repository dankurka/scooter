//////////////////////////////////////////////////////////////////
//
//   $Header: /cvs/Controller\040Board/AVR-Code/Testsuite/helpers.h,v 1.4 2006/11/20 09:45:46 rainer Exp $
//   $Source: /cvs/Controller\040Board/AVR-Code/Testsuite/helpers.h,v $
//   $Revision: 1.4 $
//
//////////////////////////////////////////////////////////////////
//
// Project: Controllerboard of balancing Scooter, Testsuite
//
// Author: Rainer Ostendorf
//
// Description: Small helper-routines, eg. delays, conversions..
//
//	$Log: helpers.h,v $
//	Revision 1.4  2006/11/20 09:45:46  rainer
//	Improved doxygen documentation, killed some bugs there.
//
//	Revision 1.3  2006/11/18 18:50:44  rainer
//	Changed PWM-Mode from dual-slope-mode to fast, single-slope mode. Added security checks for PWM-timings (max frequence, min pulse-width) to protect the h-bridges.
//
//	Revision 1.2  2006/11/17 13:30:48  rainer
//	Moved bunch of entropy from main.h to helpers.h. See random[] there
//
//	Revision 1.1  2006/11/08 12:49:54  rainer
//	Umsortiert. VHDL-Daten bitte jetzt nach VHDL-Code
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
 *  @defgroup ro_helpers  Auxiliary functions
 *  @code #include <helpers.h> @endcode
 * 
 *  @brief Small helper utils
 *
 * 
 *	This is the place for some small helpers, eg. for delays 
 *  or data-conversion. Do not put extensive code in here.
 *   
 *  @author Rainer Ostendorf
 */

/**@{*/

#ifndef HELPERS_H_
#define HELPERS_H_




/** delay program executing for ms milliseconds by doing busy waiting
 * 
 * @param ms delay in ms
 * @return void
 */
void delay_ms( unsigned int ms );

#endif /*HELPERS_H_*/


/**@}*/
