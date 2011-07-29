//////////////////////////////////////////////////////////////////
//
//   $Header: /cvs/Controller\040Board/AVR-Code/Testsuite/adc.h,v 1.4 2006/11/20 09:45:46 rainer Exp $
//   $Source: /cvs/Controller\040Board/AVR-Code/Testsuite/adc.h,v $
//   $Revision: 1.4 $
//
//////////////////////////////////////////////////////////////////
//
// Project: Controllerboard of balancing Scooter, Testsuite
//
// Author: Rainer Ostendorf
//
// Description: Routines for using the onchip Analog-Digital-Converter
//
//	$Log: adc.h,v $
//	Revision 1.4  2006/11/20 09:45:46  rainer
//	Improved doxygen documentation, killed some bugs there.
//
//	Revision 1.3  2006/11/20 00:03:47  rainer
//	added defines for each ADC channel
//
//	Revision 1.2  2006/11/12 00:59:42  rainer
//	Added support for interrupt-driven use of the Gyros. Cleaned up main() a bit.
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
 *  @defgroup ro_adc ADC Routines
 *  @code #include <adc.h> @endcode
 * 
 *  @brief Routines for controlling the onboard Analog-Digital converter
 *
 * 
 * This Routines are for initializing the ADC and selecting the 
 * analog input channel of the multiplexer.
 * After a channel has be activated, a single conversion may be performed by calling
 * read_adc().
 *   
 *  @note Based on Atmel Datasheet for Atmega128, Nov. 2006
 *  @author Rainer Ostendorf
 */

/**@{*/

#ifndef ADC_H_
#define ADC_H_

/** the ADC-channels : 
 * 0: connected to Vref of Gyro 1 */
#define ADC_GYRO1 0 
/**1: connected to Vref of Gyro 2 */
#define ADC_GYRO2 1
/**2: current unused, pin header */ 
#define ADC_CH2 2 
/**3: current unused, pin header */
#define ADC_CH3 3 
/**4: steering potetiometer, X-Axis */
#define ADC_STEERING_1 4 
/**5: steering potetiometer, Y-Axis */
#define ADC_STEERING_2 5
/**6: Tacho 1, (to be defined) */ 
#define ADC_TACHO_1 6 
/**7: Tacho 2  (to be defined) */
#define ADC_TACHO_2 7 

/* Initialize ADC, but not yet start a conversion
 */
void init_adc();

/* Initialize ADC, but not yet start a conversion
 */
void init_adc();

/** select the active ADC-Channel from input multiplexer
 * @param channel ADC-Channel to read, 0-7
 */
void adc_sel_channel(char channel);

/** start an ADConversion, busy-wait for the result and return it
 * @return the conversion result
 */
unsigned int read_adc();

/** demo-code: read in all ADC channels and print their
 * result to the first uart.
 */
void print_all_adc( void );

#endif /* ADC_H_ */

/**@}*/

