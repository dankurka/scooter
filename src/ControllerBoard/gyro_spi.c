//////////////////////////////////////////////////////////////////
//
//   $Header: /cvs/Controller\040Board/AVR-Code/Testsuite/gyro_spi.c,v 1.9 2006/12/18 22:51:56 rainer Exp $
//   $Source: /cvs/Controller\040Board/AVR-Code/Testsuite/gyro_spi.c,v $
//   $Revision: 1.9 $
//
//////////////////////////////////////////////////////////////////
//
// Project: Controllerboard of balancing Scooter, Testsuite
//
// Author: Rainer Ostendorf
//
// Description: Routines for reading the two onboard plugged AD ADIS16100 Gyroscopes
//
//	$Log: gyro_spi.c,v $
//	Revision 1.9  2006/12/18 22:51:56  rainer
//	Added functions for writing to SPI without switching the /CS signal of the SPI bus. This is needed by the MC13192 2.4GHz transceiver.
//
//	Revision 1.8  2006/11/24 22:04:28  rainer
//	Player around with real sensors and stabilization algorithm. Basic function seems to be ok, finetuning of factors to do.
//
//	Revision 1.7  2006/11/18 18:47:19  rainer
//	Added support for reading the temerature of the Gyros.
//
//	Revision 1.6  2006/11/12 22:44:17  rainer
//	Added serial debug monitor and support for IR-LED PWM-Signals.
//
//	Revision 1.5  2006/11/12 17:47:15  rainer
//	Added support for PWM-Duty-cycle measurement with Timer3. Tuned SPI-Code.
//
//	Revision 1.4  2006/11/12 00:59:42  rainer
//	Added support for interrupt-driven use of the Gyros. Cleaned up main() a bit.
//
//	Revision 1.3  2006/11/11 00:03:55  rainer
//	Successfully tested support for gyroscopes
//
//	Revision 1.2  2006/11/08 23:44:41  rainer
//	Verpeilung beim checkin behoben.
//
//	Revision 1.1  2006/11/07 23:12:25  rainer
//	Initial checkin. SPI Routines for Gyros.
//
//
//
//
/////////////////////////////////////////////////////////////////

#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h> 

#include "gyro_spi.h"
#include "globals.h"

/* temporary buffer for the high byte, to avoid that
 * the ringbuffer contains a value, where high-byte and low byte
 * belong to two different samples (16bit write should be atomic) */
uint8_t highbyte_temp;

/** The SPI interrupt service routine. This
 * route is called everytime, a byte has been completly 
 * transmitted over the SPI-Interface. According to the current state
 * of the transmission, either the low-byte of the 16bit word has to be
 * send, or a new transfer needs to be started
 */
SIGNAL( SIG_SPI )
{	
	cli(); /* disable any interrupt */
	 switch ( gyro_state )
	 {
	 	case (first_run): /* first entry of isr */
	 		gyro1_idx = 0; /* set ringbuffer-idice to 0 */
	 		gyro2_idx = 0;
	 		PORTD = PORTD & 0xCF; /* clear both CS-lines, CS0 */
	 		PORTB = PORTB & 0xFE; /* activate /CS */
	 		SPDR = ACC_HB; /* transmit high byte of read-accl. cmd */
	 		gyro_state = g1_hacc;
	 	break;
	 	case (g1_hacc):
	 		highbyte_temp = SPDR; /* read in high-byte of accl-value */
	 		if(highbyte_temp & 0x08)
				highbyte_temp |= 0xF0;
			else 
				highbyte_temp &= 0x0F;
	 		SPDR = ACC_LB; /* transmit low-byte of read-accl. cmd */
	 		gyro_state = g1_lacc;
	 	break;
	 	case (g1_lacc):
	 		PORTB = PORTB | 0x01; /* deactivate /CS */
	 		gyro1_samples[ gyro1_idx ] = (uint16_t)((highbyte_temp<<8) | SPDR) ; /* save accl-value */
	 		gyro1_idx =  (gyro1_idx+1) & (GYRO1_SAMPLES_BUFSIZE-1); /* increase index to ringbuffer */
	 		#ifndef READ_GYRO1_ONLY
	 		PORTD = PORTD | (CS_GYRO2 << 4); /* switch CS to Gyro2 */
	 		#endif
	 		PORTB = PORTB & 0xFE; /* activate /CS */
	 		SPDR = ACC_HB; /* send high-byte to Gyro 2 */
	 		#ifndef READ_GYRO1_ONLY
	 		gyro_state = g2_hacc;
	 		#else
	 		gyro_state = g1_hacc;
	 		#endif
	 	break;
	 	case (g2_hacc):
	 		highbyte_temp = SPDR; /* save high byte of Gyro 2 */
	 		if(highbyte_temp & 0x08)
				highbyte_temp |= 0xF0;
			else 
				highbyte_temp &= 0x0F;
	 		SPDR = ACC_LB;
	 		gyro_state = g2_lacc;
	 	break;
	 	case (g2_lacc):
	 		PORTB = PORTB | 0x01; /* deactivate /CS */
	 		gyro2_samples[ gyro2_idx ] =  (uint16_t)((highbyte_temp << 8 ) | SPDR); /* save low-byte of Gyro 2 */
	 		gyro2_idx =  (gyro2_idx+1) & (GYRO2_SAMPLES_BUFSIZE-1);
	 		PORTD = PORTD & 0xCF; /* switch CS to Gyro1 */
	 		PORTB = PORTB & 0xFE; /* activate /CS */
	 		/*
	 		 *  when the temperature-read-flag is set, the FSM forks at this point and
	 		 *  reads both temperatures before returning to normal (read accleration) operation
	 		 */	 		
	 		if( !read_gyro_temp ) /* check if the temperature shall be read */
	 		{
	 			/* don't read temperature */
	 			SPDR = ACC_HB; /* transmit high byte of "get accl." */
	 			gyro_state = g1_hacc;
	 		}
	 		else
	 		{
	 			/* read temperature */ 
	 			SPDR = TEMP_HB; /* transmit high byte of "get temp." */
	 			gyro_state = g1_htmp;
	 		}
	 	break;
	 	/* 
	 	 *  the following states are for reading the temperature values and
	 	 *  get skipped in normal operation 
	 	 * */
	 	case g1_htmp:
	 		highbyte_temp = SPDR;
	 		SPDR = TEMP_LB;
	 		gyro_state = g1_ltmp;
	 	break;
	 	case g1_ltmp:
	 		PORTB = PORTB | 0x01; /* deactivate /CS */
	 		gyro1_temp = (uint16_t)((highbyte_temp << 8 ) | SPDR); /* save temp. to global variable */
	 		PORTD = PORTD | (CS_GYRO2 << 4); /* switch CS to Gyro2 */
	 		PORTB = PORTB & 0xFE; /* activate /CS */
	 		SPDR = TEMP_HB; /* send  "read temp" high-byte to Gyro 2 */
	 		gyro_state = g2_htmp;
		case g2_htmp:
			highbyte_temp = SPDR;
	 		SPDR = TEMP_LB;
	 		gyro_state = g2_ltmp;
		break;
		case g2_ltmp:
	 		PORTB = PORTB | 0x01; /* deactivate /CS */
	 		gyro2_temp = (uint16_t)((highbyte_temp << 8 ) | SPDR); /* save temp. to global variable */
	 		read_gyro_temp = 0; /* clear the flag indicating that the temperature shall be read */
	 		/* now reading both temperature is finished and the FSM goes back to its normal mode of operation */
	 		PORTD = PORTD & 0xCF; /* switch CS to Gyro1 */
	 		PORTB = PORTB & 0xFE; /* activate /CS */
	 		SPDR = ACC_HB; /* send  "read accl" high-byte to Gyro 1 */
	 		gyro_state = g1_hacc;
	 	break; 
	 	default: /* this should never happen */
	 		gyro_state = first_run; 
	 	break;
	 }
	 sei();/* enable interrupts */
}


/** Initialize the onboard SPInterface to match the needs
 * of the AD ADIS16100 Gyroskope. This means setting the clock,
 * setting the ports directions, setting mode etc.
 * @param interrupt_mode flag wether to enable the interrupt mode
 * 			in irq-mode, the ISR takes care of reading the Gyros
 * 			as fast as possible and fills a global array used as 
 * 			ringbuffer of sample-values. Either the Gyros have
 * 			to be read manually by calling the spi_write-functions.
 */
 void init_spi_gyro( uint8_t interrupt_mode)
 {
 	
 	uint8_t ie;
 	
 	if( 1==interrupt_mode)
 	{
 		ie = 1;
 	}
 	else
 		ie = 0;
 		
	/** Set the SPI-port-Pin directions:
	 * B0 - /SS -> Output, 1
	 * B1 - SCLK -> Output, 1
	 * B2 - MOSI -> Output, 1
	 * B3 - MISO -> Input, 0
	*/
	DDRB = DDRB & 0xF0; /* clear SPI-Pins */
	DDRB = DDRB | 0x07; /* set SPI-Direction Pins 0111b (MISO input, SCLK, MOSI, SS output) */
	
	/* set the CS1 and CS2 signals on Port D to act as output */
	DDRD = DDRD | (1<<4) | (1<<5);
	/* set CS1 = 0 and CS2 = 0 -> activate Chipsel 1 */
    PORTD = PORTD & 0xCF;
	/* Set /SS to high -> inactive */
	PORTB = PORTB | 0x01;  
	
	/** Set SPI Control register:
	 * 7 - SPIE, Interrup Enable -> 1, enable IRQs
	 * 6 - SPE, SPI-Enbale -> 1, enable SPI-System
	 * 5 - DORD, DataOrder -> 0, MSB first (leftshift)
	 * 4 - MSTR, Master/Slave ->  1, MasterMode
	 * 3 - CPOL, ClkPolarity -> 0, begin with leading edge rising,trailing edge falling
	 * 2 - CPHA, ClkPhase -> 1, sample on leading edge (0-1), setup on trailing(1-0)
	 * 1,0 SPR1,0, ClockRate, 1,1 - for 125 kHz CLK with SPI2x not set	 */
	 SPCR = 0x00 | (ie<<SPIE) | (1<<SPE) | (1<<MSTR) | (1<<CPHA) | (1<<SPR1) | (0<<SPR0); /* 0b1101 0100 */
	 
	 /** Set SPI  Status Register:
	  * 0, SPI2X -> 1,clk-doubling for 0.25 Mhz  */
	  SPSR = 0x00 | (0<<SPI2X);
	  
	  /** Set the selftest-signals to be outputs */
	  DDRA = DDRA | (1<<PA5) | (1<<PA6);
	  
	  /** deactive self-tests */
	  PORTA = PORTA & ~( (1<<PA5) | (1<<PA6)) ;
	  
	  if( 1==interrupt_mode )
	  	SPDR = 0x00; /* start first transmission, rest is handled by ISR */
 }
 
 /** choose a chip by setting the CS1 and CS2 signal
  * towards the GAL according
  * @param chip Nr of the /CS-Signal to choose (0-3)
  */
 void spi_cs( uint8_t chip )
 {
 		/* clear CS1 and CS2 */
 		PORTD = PORTD & 0xCF;
 		PORTD = PORTD |  ((chip & 0x03)<<4);
 		return;
 }
 
  /* Transfer 1 Byte given in data via the SPI,
  * return the 1 Byte sampled during transfer.
  * @param data 8bit word that is transferred
  * @return data sampled during transfer
  */  
 uint8_t spi_write_8( uint8_t data )
 {
 		uint8_t indata=0;
 		/* activate chipselect (/SS) for SPI: */
 		PORTB = PORTB & 0xFE; /* clear lowest bit */
 		/* send high byte of output */
 		SPDR = data;
 		/* wait for transfer to finish (IRQ-Flag set) 
 		 * the flag is clear automatically by reading the
 		 * status register and then the data-register, 
 		 * see datasheet 10/06, p. 170 */
 		while( !(SPSR & (1<<SPIF)) )
 			;
 		/* save the high byte of input */
 		indata = SPDR;
 		PORTB = PORTB | 0x01; /* deactivate /SS, set lowest bit */
 		return indata; 
 } 
 
  /* @brief Transfer 1 Byte given in data via the SPI, no chipselection
  * Returns the 1 Byte sampled during transfer.
  * DOES NOT switch CS
  * @param data 8bit word that is transferred
  * @return data sampled during transfer
  */  
 uint8_t spi_write_8_nocs( uint8_t data )
 {
 		uint8_t indata=0;
 		/* send high byte of output */
 		SPDR = data;
 		/* wait for transfer to finish (IRQ-Flag set) 
 		 * the flag is clear automatically by reading the
 		 * status register and then the data-register, 
 		 * see datasheet 10/06, p. 170 */
 		while( !(SPSR & (1<<SPIF)) )
 			;
 		/* save the high byte of input */
 		indata = SPDR;
 		return indata; 
 }
 
 /* Tranfer 2 Bytes given in data via the SPI,
  * return the 2 Bytes sampled during transfer.
  * @param data 16bit word that is transferred
  * @return data sampled during transfer
  */  
 uint16_t spi_write_16( uint16_t data )
 {
 		uint16_t indata=0;
 		/* activate chipselect (/SS) for SPI: */
 		PORTB = PORTB & 0xFE; /* clear lowest bit */
 		/* send high byte of output */
 		SPDR = (uint8_t) ((data & 0xFF00)>>8); 
 		/* wait for transfer to finish (IRQ-Flag set) 
 		 * the flag is clear automatically by reading the
 		 * status register and then the data-register, 
 		 * see datasheet 10/06, p. 170 */
 		while( !(SPSR & (1<<SPIF)) )
 			;
 		/* save the high byte of input */
 		indata = (uint16_t) (SPDR << 8 );
 		/* send low-byte of output */
 		SPDR = (uint8_t) (data & 0x00FF );
 		while( !(SPSR & (1<<SPIF)) )
 			;
 		indata = indata | SPDR ;
 		PORTB = PORTB | 0x01; /* deactivate /SS, set lowest bit */
 		return indata; 
} 


/* Tranfer 2 Bytes given in data via the SPI, no chipselection
  * return the 2 Bytes sampled during transfer.
  * DO NOT switch CS
  * @param data 16bit word that is transferred
  * @return data sampled during transfer
  */  
 uint16_t spi_write_16_nocs( uint16_t data )
 {
 		uint16_t indata=0;
 		/* send high byte of output */
 		SPDR = (uint8_t) ((data & 0xFF00)>>8); 
 		/* wait for transfer to finish (IRQ-Flag set) 
 		 * the flag is clear automatically by reading the
 		 * status register and then the data-register, 
 		 * see datasheet 10/06, p. 170 */
 		while( !(SPSR & (1<<SPIF)) )
 			;
 		/* save the high byte of input */
 		indata = (uint16_t) (SPDR << 8 );
 		/* send low-byte of output */
 		SPDR = (uint8_t) (data & 0x00FF );
 		while( !(SPSR & (1<<SPIF)) )
 			;
 		indata = indata | SPDR ;
 		return indata; 
} 

