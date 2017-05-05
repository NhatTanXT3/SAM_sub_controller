/*
 * SAM.h
 *
 *  Created on: Feb 27, 2017
 *      Author: Nhat Tan
 */

#ifndef SAM_H_
#define SAM_H_
#define NUM_OF_SAM_ 24

#define SAM_WRITE_POSITION_ 0
#define UPPER_10BIT_BOUNDARY_  3700
#define LOWER_10BIT_BOUNDARY_  401

/*
 * define for hardware
 */
#define UART_RS485_ID_0_ 	UART_RS485_1_
#define UART_RS485_ID_1_ 	UART_RS485_4_
#define UART_RS485_ID_2_ 	UART_RS485_2_
#define UART_RS485_ID_3_ 	UART_RS485_4_
#define UART_RS485_ID_4_ 	UART_RS485_1_
#define UART_RS485_ID_5_ 	UART_RS485_4_
#define UART_RS485_ID_6_ UART_RS485_2_
#define UART_RS485_ID_7_ UART_RS485_4_
#define UART_RS485_ID_8_ UART_RS485_4_
#define UART_RS485_ID_9_ UART_RS485_4_
#define UART_RS485_ID_10_ UART_RS485_4_
#define UART_RS485_ID_11_ UART_RS485_4_
#define UART_RS485_ID_12_ UART_RS485_4_
#define UART_RS485_ID_13_ UART_RS485_4_
#define UART_RS485_ID_14_ UART_RS485_4_
#define UART_RS485_ID_15_ UART_RS485_4_
#define UART_RS485_ID_16_ UART_RS485_4_
#define UART_RS485_ID_17_ UART_RS485_4_
#define UART_RS485_ID_18_ UART_RS485_4_
#define UART_RS485_ID_19_ UART_RS485_4_
#define UART_RS485_ID_20_ UART_RS485_4_
#define UART_RS485_ID_21_ UART_RS485_4_
#define UART_RS485_ID_22_ UART_RS485_4_
#define UART_RS485_ID_23_ UART_RS485_4_
#define UART_RS485_ID_24_ UART_RS485_4_
#define UART_RS485_ID_25_ UART_RS485_4_
#define UART_RS485_ID_26_ UART_RS485_4_
#define UART_RS485_ID_27_ UART_RS485_4_

typedef struct {
	unsigned char id;
	unsigned char SAMmode;
	unsigned char mode;
	unsigned char position8;
	unsigned int position12;
	unsigned char P;
	unsigned char I;
	unsigned char D;
} SAM;

//typedef volatile struct {
//	unsigned int s1;
//	unsigned int s2;
//	unsigned int s3;
//	unsigned int s4;
//}SAM_position12;

//typedef volatile struct{
//	unsigned char busy;
//	unsigned char currentID;
//}RS485_Channel_State;
#define SAMREAD_POS12_	0
#define SAMREAD_PD_		1
#define SAMREAD_I_		2
#define SAMREAD_POS8_	3

extern volatile unsigned char samReadBusy;
extern unsigned char samReadMode;
extern volatile unsigned int samPosition12[];
extern volatile unsigned char samDataAvail[];
extern volatile unsigned char samP[];
extern volatile unsigned char samI[];
extern volatile unsigned char samD[];
extern volatile unsigned char samReadCurrentID_C4;
extern volatile unsigned char samReadCurrentID_C2;
extern volatile unsigned char samReadCurrentID_C1;
extern SAM sam1;

extern void SAM_set_PD_Runtime(unsigned char ID, unsigned char P, unsigned char D);
extern void SAM_set_I_Runtime(unsigned char ID, unsigned char I);
extern void SAM_get_PD(unsigned char ID);
extern void SAM_get_I(unsigned char ID);

extern void SAM_get_jointAngle10bit(unsigned char ID);
extern void SAM_set_jointAngle10bit(unsigned char ID,unsigned int value);
extern void SAM_get_jointAngle8bit(uint32_t ui32Base,unsigned char ID);
extern void SAM_set_jointAngle8bit(uint32_t ui32Base,unsigned char ID,unsigned char value);

#endif /* SAM_H_ */
