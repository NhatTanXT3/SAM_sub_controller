/*
 * mySerial.h
 *
 *  Created on: Mar 1, 2017
 *      Author: Nhat Tan
 */

#ifndef MYSERIAL_H_
#define MYSERIAL_H_

/*
 * Define uart for hardware
 */
#define UART_PC_	UART5_BASE
#define PC2MCU_TERMINATOR_ 	0xFE
#define PC2MCU_HEADER_		0xFF
#define MCU2PC_HEADER_		0xFF
#define MCU2PC_TERMINATOR_ 	0xFE
//=====QUICK MODE DEFINE=====
#define PC_SAM_MODE_0_ 0
#define PC_SAM_MODE_1_ 1
#define PC_SAM_MODE_2_ 2
#define PC_SAM_MODE_3_ 3
#define PC_SAM_MODE_4_ 4
#define PC_SAM_MODE_5_ 5

#define PC_SAM_MODE_1_DATALENGTH_ 6
#define PC_SAM_MODE_2_DATALENGTH_
#define PC_SAM_MODE_3_DATALENGTH_
#define PC_SAM_MODE_4_DATALENGTH_

//=====SPECIAL MODE DEFINE
#define PC_SAM_SP_MODE_1_ 0xF0
#define PC_SAM_SP_MODE_2_ 0xCC
#define PC_SAM_SP_MODE_3_ 0xAA
#define PC_SAM_SP_MODE_4_ 0x95

//#define PC_SAM_READ_1_ 2
//#define

#define SERIAL_BUFFER_SIZE_ 200
typedef volatile struct {
	unsigned char Flag_receive;
	unsigned char dataIndex;
	unsigned char dataCount;
	unsigned char Command_Data[SERIAL_BUFFER_SIZE_];
} MySerial;
extern unsigned char dataSendBuffer[];

extern MySerial serialPC;


extern void UART5_Init();

extern void UART2_Init();

extern void UART4_Init();
extern void UART7_Init();

extern void SerialGetData(uint32_t ui32Base,char *uart_str);
extern void SerialSendData(uint32_t ui32Base,unsigned char *uart_str);
extern void SerialSend_1_Position(uint32_t ui32Base,unsigned char ID,unsigned int value);
extern void SerialSend_All_Position(uint32_t ui32Base,unsigned char ID,unsigned int *SamPos);
extern void SerialSend_PID(uint32_t ui32Base,unsigned char ID,unsigned char P, unsigned char I, unsigned char D);

#endif /* MYSERIAL_H_ */
