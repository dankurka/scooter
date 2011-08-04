//////////////////////////////////////////////////////////////////
//
//   $Header: /cvs/Controller\040Board/AVR-Code/Testsuite/transceiver_spi.h,v 1.1 2006/12/18 22:52:55 rainer Exp $
//   $Source: /cvs/Controller\040Board/AVR-Code/Testsuite/transceiver_spi.h,v $
//   $Revision: 1.1 $
//
//////////////////////////////////////////////////////////////////
//
// Project: Controllerboard of balancing Scooter, Prototype-Code
//
// Author: Rainer Ostendorf, based on 8051 code by Dennis Meiering, 2004
//
// Description: low level functions for accessing the Freescale MC13192 2.4Ghz Transceiver
//
//	$Log: transceiver_spi.h,v $
//	Revision 1.1  2006/12/18 22:52:55  rainer
//	Initial checkin. Functions for using the 2.4GHz Transceiver.
//
//
//
//
//
/////////////////////////////////////////////////////////////////

/** 
 *  @defgroup ro_transceiver_spi  2.4 GHz Transceiver SPI Interface
 *  @code #include <transceiver_spi.h> @endcode
 * 
 *  @brief Routines for accessing the Freescale MC13192 2.4GHz Transceiver
 *
 * 
 * This Routines are for reading and writing to the 802.14.1 Transceiver 
 * using the SPI interface
 *   
 *  @note Based on Atmel Datasheet for Atmega128, Nov. 2006 and 
 *        on the Freescale reference manual v1.5, 10/2005 for MC13192
 * 		  
 *  @author Rainer Ostendorf
 */

/**@{*/

#ifndef TRANSCEIVER_SPI_H_
#define TRANSCEIVER_SPI_H_

#include <inttypes.h>

/* which CS the transceiver is connected to */ 
#define TRANSCEIVER_CS 2

typedef struct
{
	int dataLength;
	unsigned char datan[130];
} tx_packet_t;


typedef struct
{
	int dataLength;
	unsigned char datan[130];
} rx_packet_t;



void write_to_spi( uint8_t spi_adress, uint16_t spi_data );
void write_to_ram( uint8_t spi_adress, tx_packet_t *tx_packet );

uint8_t read_from_spi( uint8_t spi_addr );
uint8_t read_from_ram( uint8_t spi_adress, rx_packet_t *rx_packet );


#endif /*TRANSCEIVER_SPI_H_*/
