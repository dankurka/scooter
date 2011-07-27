//////////////////////////////////////////////////////////////////
//
//   $Header: /cvs/Controller\040Board/AVR-Code/Testsuite/gyro_spi.h,v 1.10 2007/04/18 01:36:18 rainer Exp $
//   $Source: /cvs/Controller\040Board/AVR-Code/Testsuite/gyro_spi.h,v $
//   $Revision: 1.10 $
//
//////////////////////////////////////////////////////////////////
//
// Project: Controllerboard of balancing Scooter, Testsuite
//
// Author: Rainer Ostendorf
//
// Description: Routines for reading the two onboard plugged AD ADIS16100 Gyroscopes
//
//  $Log: gyro_spi.h,v $
//  Revision 1.10  2007/04/18 01:36:18  rainer
//  Regler angepasst, zum Testen RS232 Konsole eingebaut um Regler-Parameter zu variieren. Rainer.
//
//  Revision 1.9  2006/12/18 22:51:56  rainer
//  Added functions for writing to SPI without switching the /CS signal of the SPI bus. This is needed by the MC13192 2.4GHz transceiver.
//
//  Revision 1.8  2006/11/24 22:04:28  rainer
//  Player around with real sensors and stabilization algorithm. Basic function seems to be ok, finetuning of factors to do.
//
//  Revision 1.7  2006/11/20 09:45:46  rainer
//  Improved doxygen documentation, killed some bugs there.
//
//  Revision 1.6  2006/11/18 18:47:20  rainer
//  Added support for reading the temerature of the Gyros.
//
//  Revision 1.5  2006/11/12 22:44:17  rainer
//  Added serial debug monitor and support for IR-LED PWM-Signals.
//
//  Revision 1.4  2006/11/12 17:47:15  rainer
//  Added support for PWM-Duty-cycle measurement with Timer3. Tuned SPI-Code.
//
//  Revision 1.3  2006/11/12 00:59:42  rainer
//  Added support for interrupt-driven use of the Gyros. Cleaned up main() a bit.
//
//  Revision 1.2  2006/11/08 23:44:41  rainer
//  Verpeilung beim checkin behoben.
//
//  Revision 1.4  2006/11/08 23:31:36  rainer
//  Changes to PWM and SPI
//
//  Revision 1.2  2006/11/07 23:37:16  rainer
//  Updated Doxygen documentation.
//
//  Revision 1.1  2006/11/07 23:12:25  rainer
//  Initial checkin. SPI Routines for Gyros.
//
//
//
//
/////////////////////////////////////////////////////////////////

/** 
 *  @defgroup ro_spi  SPI for Gyros
 *  @code #include <gyro_spi.h> @endcode
 * 
 *  @brief Code for accessing the Gyroscopes
 *
 * 
 *	These are the functions for initializing and using the SPI-
 *  Interface. Please be aware, that the Code currently is 
 *  optimized for use with the Gyroscopes. One will need to modify
 *  it for reusing with other purpose.
 *  
 *  @note Based on Atmel Datasheet for Atmega128, Nov. 2006
 *  @author Rainer Ostendorf
 */

/**@{*/


#ifndef GYRO_SPI_H_
#define GYRO_SPI_H_

#include <inttypes.h>

/** set this define to read gyrosope 1 only */ 
//#define READ_GYRO1_ONLY 1

/** chipselect for gyro1 */
#define CS_GYRO1 0
/** chipselect for gyro2 */
#define CS_GYRO2 2 

/** Din of Gyro for Acc:  0b1000 0011 0001 0000 = 0x8300 */
/** high-byte of "read acceleration" control word for ADIS16100 */
#define ACC_HB 0x83 
/** low-byte of "read acceleration" control word for ADIS16100  -> 2s-complement */
#define ACC_LB 0x00 
/** Din of Gyro for Temp:  0b1000 0111 0000 0000 = 0x8700 */
/** high-byte of "read tempereature" control word for ADIS16100 */
#define TEMP_HB 0x87 
/** low-byte of "read tempereature" control word for ADIS16100 */
#define TEMP_LB 0x01 

/** state definition for the finite state maschine running inside the SPI-ISR */ 
typedef enum gyro_fsm_state_enum {
		/** first run of the ISR, not initialized */
		first_run,
		/** high-byte of "read-acceleration" word was send to gyro 1 */	 
		g1_hacc,
		/** low-byte of "read-acceleration" word was send to gyro 1 */ 
		g1_lacc,
		/** high-byte of "read-acceleration" word was send to gyro 2 */
		g2_hacc,
		/** low-byte of "read-acceleration" word was send to gyro1 */
		g2_lacc,
		/** high-byte of "read-temperature" word was send to gyro1 */	
		g1_htmp,
		/** low-byte of "read-temperature" word was send to gyro1 */
		g1_ltmp,
		/** high-byte of "read-temperature" word was send to gyro2 */
		g2_htmp,
		/** high-byte of "read-temperature" word was send to gyro2 */
		g2_ltmp	
} gyro_fsm_state;

/** the state variable for the ISRs finite state maschine */ 
gyro_fsm_state gyro_state;

/**Initialize the onboard SPInterface to match the needs
 * of the AD ADIS16100 Gyroskope. This means setting the clock,
 * setting the ports directions, setting mode etc.
 * @param interrupt_mode flag wether to enable the interrupt mode.
 * 			In irq-mode, the ISR takes care of reading the Gyros
 * 			as fast as possible and fills a global array used as 
 * 			ringbuffer of sample-values. Either the Gyros have
 * 			to be read manually by calling the spi_write-functions.
 */
 extern void init_spi_gyro( uint8_t interrupt_mode );
 
 /** choose a chip by setting the CS1 and CS2 signal
  * towards the GAL according
  * @param chip Nr of the /CS-Signal to choose (0-3)
  */
 void spi_cs( uint8_t chip );
 
 /** @brief Transfer 1 Byte given in data via the SPI
  * return the 1 Byte sampled during transfer.
  * @param data 8bit word that is transferred
  * @return data sampled during transfer
  */  
 uint8_t spi_write_8( uint8_t data );
 
  /* @brief Transfer 1 Byte given in data via the SPI, no chipselection
  * Returns the 1 Byte sampled during transfer.
  * DOES NOT switch CS
  * @param data 8bit word that is transferred
  * @return data sampled during transfer
  */  
 uint8_t spi_write_8_nocs( uint8_t data );
 
 
 /** @brief Tranfer 2 Bytes given in data via the SPI,
  * return the 2 Bytes sampled during transfer.
  * @param data 16bit word that is transferred
  * @return data sampled during transfer
  */  
 uint16_t spi_write_16( uint16_t data );
 
 /* @brief Transfer 2 Bytes given in data via the SPI, no chipselection
  * return the 2 Bytes sampled during transfer.
  * DO NOT switch CS
  * @param data 16bit word that is transferred
  * @return data sampled during transfer
  */  
 uint16_t spi_write_16_nocs( uint16_t data );

#endif /*GYRO_SPI_H_*/

/**@}*/
