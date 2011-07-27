//////////////////////////////////////////////////////////////////
//
//   $Header: /cvs/Controller\040Board/AVR-Code/Testsuite/transceiver_spi.c,v 1.1 2006/12/18 22:52:55 rainer Exp $
//   $Source: /cvs/Controller\040Board/AVR-Code/Testsuite/transceiver_spi.c,v $
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
//	$Log: transceiver_spi.c,v $
//	Revision 1.1  2006/12/18 22:52:55  rainer
//	Initial checkin. Functions for using the 2.4GHz Transceiver.
//
//
//
//
//
/////////////////////////////////////////////////////////////////

#include <avr/io.h>
#include <inttypes.h>
#include "transceiver_spi.h"
#include "gyro_spi.h"


void write_to_spi( uint8_t spi_adress, uint16_t spi_data )
{
	spi_cs( TRANSCEIVER_CS );
 	PORTB = PORTB & 0xFE;/* activate chipselect (/SS) for SPI: */
	spi_adress &= 0x3F; /* mask 2 MSBs of adress to zero */
	spi_write_8_nocs( spi_adress ); /* write adress to transceiver */
	spi_write_16_nocs( spi_data ); /* write data to transceiver */
	PORTB = PORTB | 0x01; /* deactivate /SS, set lowest bit */
}

uint8_t read_from_spi( uint8_t spi_addr )
{
		uint16_t buf;
		spi_cs( TRANSCEIVER_CS );
		PORTB = PORTB & 0xFE;/* activate chipselect (/SS) for SPI: */
		spi_addr &= 0x3F; /* mask 2 MSBs of adress to zero */
		spi_write_8_nocs( spi_addr );
		buf = spi_write_16_nocs( 0x0000 );
		PORTB = PORTB | 0x01; /* deactivate /SS, set lowest bit */
		return buf;
}

void write_to_ram( uint8_t spi_addr, tx_packet_t *tx_packet )
{
	int n;	
	
	spi_cs( TRANSCEIVER_CS );
	PORTB = PORTB & 0xFE;/* activate chipselect (/SS) for SPI: */
	spi_addr &= 0x3F; /* mask 2 MSBs of adress to zero */
	
	spi_write_8_nocs( spi_addr ); /* write address */

	for( n = 0; n < tx_packet->dataLength; n+=2 )
	{
		spi_write_16_nocs( (uint16_t) ((tx_packet->datan[n+1]<<8)|(tx_packet->datan[n])) );			
	}
	PORTB = PORTB | 0x01; /* deactivate /SS, set lowest bit */
}

uint8_t read_from_ram( uint8_t spi_addr, rx_packet_t *rx_packet)
{
	int n;
	uint16_t buf;
		
	spi_cs( TRANSCEIVER_CS );
	PORTB = PORTB & 0xFE;/* activate chipselect (/SS) for SPI: */
	spi_addr &= 0x3F; /* mask 2 MSBs of adress to zero */
	spi_write_8_nocs( spi_addr ); /* write address */
	
	n = spi_write_8_nocs( 0x00 ); /* discard first byte, as its garbage */
	for( n = 0; n < rx_packet->dataLength; n+=2 )
	{
		buf = spi_write_16_nocs(0x00);
		rx_packet->datan[n] =   (char)(buf & 0xFF);
		rx_packet->datan[n+1] = (char)(buf>>8);
	}
	PORTB = PORTB | 0x01; /* deactivate /SS, set lowest bit */
	return 1;
}
