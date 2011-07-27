//////////////////////////////////////////////////////////////////
//
//   $Header: /cvs/Controller\040Board/AVR-Code/Testsuite/display_com.h,v 1.3 2007/09/23 22:56:59 rainer Exp $
//   $Source: /cvs/Controller\040Board/AVR-Code/Testsuite/display_com.h,v $
//   $Revision: 1.3 $
//
//////////////////////////////////////////////////////////////////
//
// Project: Controllerboard of balancing Scooter, Testsuite
//
// Author: Rainer Ostendorf
//
// Description: Protocol for communication with ther display unit
//
//	$Log: display_com.h,v $
//	Revision 1.3  2007/09/23 22:56:59  rainer
//	Rainer: added beeper, added some display com channels, modified tiltback, modified steering
//
//	Revision 1.2  2007/09/12 04:17:02  rainer
//	Rainer: Worked the weekend on software-features: Implemented display-communication, battery-monitoring, beeper support
//
//	Revision 1.1  2007/09/04 02:40:33  rainer
//	Added display communication protocol
//
//
//
//
/////////////////////////////////////////////////////////////////


#ifndef DISPLAY_COM_H_
#define DISPLAY_COM_H_


#define CH_MOT1_PWM 1
#define CH_MOT1_DIR 2
#define CH_MOT1_STATUS 3
#define CH_MOT2_PWM 4
#define CH_MOT2_DIR 5
#define CH_MOT2_STATUS 6

#define CH_ACCL 21
#define CH_GYR1_VAL 22
#define CH_GYR2_VAL 23
#define CH_GYR1_TEMP 24
#define CH_GYR2_TEMP 25
#define CH_ADC1 26
#define CH_ADC2 27
#define CH_ADC3 28
#define CH_ADC4 29
#define CH_ADC5 30
#define CH_ADC6 31
#define CH_ADC7 32
#define CH_ADC8 33
#define CH_ADC9 34
#define CH_ADC10 35

#define CH_STEER_ZERO 41
#define CH_GYR1_ZERO 42
#define CH_GYR_TO_OMEGA 43
#define CH_ACCL_TO_DEGR 44
#define CH_STEER_RESP 45
#define CH_MAX_STEER 46
#define CH_K_ANGLE 47
#define CH_K_OMEGA 48
#define CH_K_CORR 49
#define CH_SET_POINT 50
#define CH_CURSPEED_FAC 51
#define CH_STEER_CMD 52

#define CH_CUR_SPEED 70
#define CH_PHI 71
#define CH_BEEPER 79

#define CH_FWVER 91
#define CH_SERNR 92
#define CH_SYSTIME_MIL 93
#define CH_SYSTIME_SEC 94
#define CH_SYSTIME_MIN 95
#define CH_SYSTIME_H 96


/** the current state of FSM handling the communication protocol*/ 
typedef enum protocol_fsm_state_enum {
		/** wait for incoming characters */
		idle,
		/* wait for SOT-character */
		got_sot,
		/* wait for command */
		got_command,
		/* wait for first seperator (between cmd and ch)  */
		got_sep1,
		/* wait for channel number, byte 1/3 */
		got_channel_1,
		/* wait for channel number, byte 2/3 */
		got_channel_2,
		/* wait for channel number, byte 3/3 */
		got_channel_3,
		/* wait for second seperator (between ch and val)  */
		got_sep2,
		/* wait for value */
		got_value,
		/* wait for eot */
		got_eot
} protocol_fsm_state;

/** the state variable for the ISRs finite state maschine */ 
protocol_fsm_state com_state;

uint8_t com_cmd;
uint8_t com_ch;


/** handle the communication with the display unit via serial protocol */
void handle_display_com(void);

void com_response_read(uint8_t com_ch);

void com_response_write(uint8_t com_ch);


#endif /*DISPLAY_COM_H_*/
