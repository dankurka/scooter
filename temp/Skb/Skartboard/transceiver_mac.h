//////////////////////////////////////////////////////////////////
//
//   $Header: /cvs/Controller\040Board/AVR-Code/Testsuite/transceiver_mac.h,v 1.1 2006/12/18 22:52:55 rainer Exp $
//   $Source: /cvs/Controller\040Board/AVR-Code/Testsuite/transceiver_mac.h,v $
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
//	$Log: transceiver_mac.h,v $
//	Revision 1.1  2006/12/18 22:52:55  rainer
//	Initial checkin. Functions for using the 2.4GHz Transceiver.
//
//
//
//
/////////////////////////////////////////////////////////////////

/** 
 *  @defgroup ro_transceiver_mac  2.4 GHz Transceiver MAC layer
 *  @code #include <helpers.h> @endcode
 * 
 *  @brief MAC layer code for the MC13192 2.4GHz Transceiver 
 *
 * 
 * This contains functions for intializing the chip and switching its
 * internal states and modes of operation
 *   
 *  @author Original by Dennis Meiering 2004, ported to AVR by Rainer Ostendorf 2006
 */

/**@{*/


#ifndef TRANSCEIVER_MAC_H_
#define TRANSCEIVER_MAC_H_

#include <inttypes.h>
#include "transceiver_spi.h"

#define MC13192_IDLE 0
#define MC13192_RX 1
#define MC13192_TX 2

#define NO_PACKET 0
#define RX_PACKET 1
#define TX_PACKET 2
#define DROP_PACKET 3


extern uint8_t mc13192_mode;
extern uint8_t mc13192_state;

/** @brief Initialize the MC13192 
 *  TODO: write detailed code description! 
 */
void MC13192_init();

/** @brief switch MC13192 to receive mode 
 * TODO: write detailed code description! 
 */
void init_rx_pkt_mode(void);

/** r@brief read a data paket from MC13192 
*
* TODO: write detailed code description! 
* @param rx_packet Datastructure to save the received data to
* @return 1 in every case
*/
int get_rx_pkt(rx_packet_t *rx_packet);

/** @brief Update the state variable to reflect the state of MC13192
 * 
 * * TODO: write detailed code description!
 */
void MC13192_update_state(void);

/** @brief send a data packet
* TODO: write detailed code description!
* @param tx_packet
 */
int tx_pkt_mode(tx_packet_t *tx_packet);


void cca_modus(void);

#endif /*TRANSCEIVER_MAC_H_*/


/**@}*/
