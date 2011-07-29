//////////////////////////////////////////////////////////////////
//
//   $Header: /cvs/Controller\040Board/AVR-Code/Testsuite/transceiver_mac.c,v 1.1 2006/12/18 22:52:55 rainer Exp $
//   $Source: /cvs/Controller\040Board/AVR-Code/Testsuite/transceiver_mac.c,v $
//   $Revision: 1.1 $
//
//////////////////////////////////////////////////////////////////
//
// Project: Controllerboard of balancing Scooter, Testsuite
//
// Author: Rainer Ostendorf
//
// Description: Routines for handling the MAC layer of the zigbee transceiver
//
//	$Log: transceiver_mac.c,v $
//	Revision 1.1  2006/12/18 22:52:55  rainer
//	Initial checkin. Functions for using the 2.4GHz Transceiver.
//
//
//
//
/////////////////////////////////////////////////////////////////


#include <inttypes.h>
#include "transceiver_spi.h"
#include "transceiver_mac.h"

#ifdef DEBUG_MAC
#include "uart.h"
#endif

unsigned char mc13192_mode;
unsigned char mc13192_state;

void MC13192_init()
{
	write_to_spi(0x00, 0x0000); //RESET
	
	/* MC13192 v2.2 register settings */
	write_to_spi(0x1B,0x8000); 	/* Disable TC1. */
	write_to_spi(0x1D,0x8000); 	/* Disable TC2. */
	write_to_spi(0x1F,0x8000); 	/* Disable TC3. */
	write_to_spi(0x21,0x8000); 	/* Disable TC4. */
	write_to_spi(0x07,0x0E00); 	/* Enable CLKo in Doze */
	write_to_spi(0x0C,0x0300); 	/* IRQ pull-up disable. */
	read_from_spi(0x25); 		/* Sets the reset indicator bit */
	write_to_spi(0x04,0xA08D); 	/* LR ADDED New cal value */
	write_to_spi(0x08,0xFFF7); 	/* Preferred injection */
	write_to_spi(0x05,0x8351); 	/* Acoma, TC1, Doze, ATTN masks, LO1, CRC */
	write_to_spi(0x06,0x4720); 	/* CCA, TX, RX, energy detect */
	/* Read the status register to clear any undesired IRQs. */
	read_from_spi(0x24); 		/* Clear the status register, if set */
	mc13192_mode = MC13192_IDLE; /* Update global variable to reflect MC13192 status */
	mc13192_state = NO_PACKET; 	/* Update global variable to reflect MC13192 status */
}


void MC13192_update_state(void)
	{

	int buffer;
			  	
	buffer = read_from_spi(0x24);
	
	switch (buffer)
		{
		case 0x81 :
			mc13192_state = RX_PACKET;
			break;
		case 0x40 :
			mc13192_state = TX_PACKET;
			break;
		case 0x00 :
			mc13192_state = NO_PACKET;
			break;
		default :
			mc13192_state = DROP_PACKET;
			break;
		}
	}


void cca_modus(void)
	{
	uint16_t buffer;
	
	#ifdef DEBUG_MAC
		char buf[64]; /* buffer for debug output */
	#endif
	
	/**************************** CCA MODUS ************************************/

	buffer = read_from_spi( 0x06); 	// CCA MODE Aktivieren
	buffer = buffer|(1);
	buffer = buffer&~(2);
	write_to_spi(0x06, buffer); 	//Frequency

	buffer = read_from_spi( 0x06); 	// CCA TYPE CLEAR CHANNEL ASSESSMENT
	buffer = buffer|(1<<4);
	buffer = buffer&~(1<<5);
	write_to_spi(0x06, buffer); 

	buffer = read_from_spi( 0x29); 	//read signal strenght
	buffer = buffer & (0x00FF);
	buffer = buffer / (-2);
	
	#ifdef DEBUG_MAC	
		snprintf(buf,64,"Read Register 0x29 %d\n",buffer ); 
		uart_puts(buf);
	#endif	
	/**************************** CCA MODUS ENDE ********************************/
	}

 
void init_rx_pkt_mode(void)
	{
	write_to_spi(0x06, 0x0122);
	}
 

int get_rx_pkt(rx_packet_t *rx_packet)
	{
	rx_packet->dataLength = read_from_spi( 0x25 )-2;
	rx_packet->dataLength = rx_packet->dataLength&0x007F;
	read_from_ram( 0x01, rx_packet);
	
	return 1;
	}
	

int tx_pkt_mode( tx_packet_t *tx_packet)
	{
	int buffer;

	if( tx_packet->dataLength > 125 )
		{
		return 0;
		}

	buffer = tx_packet->dataLength+2; //Paket Laenge +2 Bytes fuer Checksumme 
	write_to_spi(0x03, buffer);
	
	write_to_ram(0x02, tx_packet); 	//Daten ins Packet RAM laden
	write_to_spi(0x06, 0x0203); 	//Sende Modus + Interrupt aktivieren
	
	return 1;	
	}
