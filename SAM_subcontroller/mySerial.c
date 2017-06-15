/*
 * mySerial.c
 *
 *  Created on: Mar 1, 2017
 *      Author: Nhat Tan
 */

/*-----system include----------------*/
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>

#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_gpio.h"

#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "inc/tm4c123gh6pm.h"
#include "driverlib/pin_map.h"
#include "driverlib/uart.h"
#include "driverlib/interrupt.h"
#include "utils/ringbuf.h"


/*-----my include-------------------*/
#include "mySerial.h"
#include "myIO.h"
#include "myFIFO.h"

MySerial serialPC;
unsigned char dataSendBuffer[150];
//FIFO serialPcFIFO;

//void UART_PC_Init(){
//	UART3_Init();
//}

void UART2_Init(){
	//
	// Enable Peripheral Clocks
	//
	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART2);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
	//
	// Enable pin PD6 for UART2 U2RX
	//
	GPIOPinConfigure(GPIO_PD6_U2RX);
	GPIOPinTypeUART(GPIO_PORTD_BASE, GPIO_PIN_6);

	//
	// Enable pin PD7 for UART2 U2TX
	// First open the lock and select the bits we want to modify in the GPIO commit register.
	//
	HWREG(GPIO_PORTD_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
	HWREG(GPIO_PORTD_BASE + GPIO_O_CR) = 0x80;
	//
	// Now modify the configuration of the pins that we unlocked.
	//
	GPIOPinConfigure(GPIO_PD7_U2TX);
	GPIOPinTypeUART(GPIO_PORTD_BASE, GPIO_PIN_7);

	UARTConfigSetExpClk(UART2_BASE, SysCtlClockGet(), 115200,
			(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));

	IntMasterEnable(); //enable processor interrupts
	IntEnable(INT_UART2); //enable the UART interrupt

	UARTFIFOLevelSet(UART2_BASE,UART_FIFO_TX1_8,UART_FIFO_RX4_8);
	UARTIntEnable(UART2_BASE, UART_INT_RX|UART_INT_RT);
}


void UART5_Init()
{
	//
	// Enable Peripheral Clocks
	//
	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART5);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);

	//
	// Enable pin PE4 for UART5 U5RX
	//
	GPIOPinConfigure(GPIO_PE4_U5RX);
	GPIOPinTypeUART(GPIO_PORTE_BASE, GPIO_PIN_4);

	//
	// Enable pin PE5 for UART5 U5TX
	//
	GPIOPinConfigure(GPIO_PE5_U5TX);
	GPIOPinTypeUART(GPIO_PORTE_BASE, GPIO_PIN_5);

	UARTConfigSetExpClk(UART5_BASE, SysCtlClockGet(), 1500000,
			(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));

	IntMasterEnable(); //enable processor interrupts
	IntEnable(INT_UART5); //enable the UART interrup

	UARTFIFOLevelSet(UART5_BASE,UART_FIFO_TX1_8,UART_FIFO_RX4_8);
	UARTIntEnable(UART5_BASE, UART_INT_RX|UART_INT_RT);
	//	myFIFO_init(&serialPcFIFO);
}

void UART5_Interrupt_Handler(void)
{
	uint32_t interrupt_status = UARTIntStatus(UART5_BASE,true);
	if(interrupt_status&(UART_INT_RX|UART_INT_RT))
	{
		UARTIntClear(UART5_BASE,UART_INT_RX|UART_INT_RT);
		while(UARTCharsAvail(UART5_BASE))
		{
			char charData=(char)UARTCharGet(UART5_BASE);
			if(charData==PC2MCU_HEADER_){
				serialPC.dataIndex=0;
				serialPC.Flag_capture=1;
				serialPC.Command_Data[serialPC.dataIndex++]=charData;
				//				serialPC.dataCount=0;
				//					FIFO_Rx_clear(&serialPcFIFO);
			}
			else if((charData==PC2MCU_TERMINATOR_)&&(serialPC.Flag_capture)){
				serialPC.Flag_receive=1;
				serialPC.Flag_capture=0;
				serialPC.Command_Data[serialPC.dataIndex++]=charData;
			}else if((serialPC.Flag_capture)&&(serialPC.dataIndex<SERIAL_BUFFER_SIZE_))
			{
			    serialPC.Command_Data[serialPC.dataIndex++]=charData;
			}else if(serialPC.dataIndex>=SERIAL_BUFFER_SIZE_){
			    serialPC.dataIndex=0;
			}

//			else
//			{
//			    serialPC.dataIndex=0;
//			}

//			if((serialPC.dataIndex<SERIAL_BUFFER_SIZE_)&&(serialPC.Flag_capture))
//				serialPC.Command_Data[serialPC.dataIndex++]=charData;
//			else
//				serialPC.dataIndex=0;
			//				FIFO_Rx_CharPut(&serialPcFIFO,charData);
		}
		//		serialGetData(UART3_BASE,serialPC.Command_Data);
	}
}
//
//void UART7_Init(){
//	//
//	// Enable Peripheral Clocks
//	//
//	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART7);
//	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
//	//
//	// Enable pin PE0 for UART7 U7RX
//	//
//	GPIOPinConfigure(GPIO_PE0_U7RX);
//	GPIOPinTypeUART(GPIO_PORTE_BASE, GPIO_PIN_0);
//
//	//
//	// Enable pin PE1 for UART7 U7TX
//	//
//	GPIOPinConfigure(GPIO_PE1_U7TX);
//	GPIOPinTypeUART(GPIO_PORTE_BASE, GPIO_PIN_1);
//
//	UARTConfigSetExpClk(UART7_BASE, SysCtlClockGet(), 115200,
//			(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));
//
//	IntMasterEnable(); //enable processor interrupts
//	IntEnable(INT_UART7); //enable the UART interrupt
//
//	UARTFIFOLevelSet(UART7_BASE,UART_FIFO_TX1_8,UART_FIFO_RX4_8);
//	UARTIntEnable(UART7_BASE, UART_INT_RX|UART_INT_RT);
//}


void serialGetData(uint32_t ui32Base,char *uart_str)
{
	while(UARTCharsAvail(ui32Base))
	{
		*uart_str++=(char)UARTCharGet(ui32Base);
	}
}

void SerialSendData(uint32_t ui32Base,unsigned char *uart_str)
{
	while(*uart_str != MCU2PC_TERMINATOR_) {UARTCharPut(ui32Base,*uart_str++ );}
	UARTCharPut(ui32Base,MCU2PC_TERMINATOR_);
}

void SerialSend_1_Position12(uint32_t ui32Base,unsigned char ID,unsigned int value){
    unsigned char data[6];
    data[0]=MCU2PC_HEADER_;
    data[1]=ID&0x1F;
    data[2]=(unsigned char)((value>>7)&0x1F); // target position: upper 5bits
    data[3]=(unsigned char)(value&0x7F); //target position: lower 7 bits
    data[4]=(data[1]^data[2]^data[3])&0x7F;//check sum
    data[5]=MCU2PC_TERMINATOR_;
    SerialSendData(ui32Base,data);
}
void SerialSend_1_AverageTorq(uint32_t ui32Base,unsigned char ID,unsigned int value){
    unsigned char data[6];
    unsigned char Mode=10;
        data[0]=MCU2PC_HEADER_;
        data[1]=(ID&0x1F)+((Mode&0x0C)<<3);
        data[2]=(unsigned char)((value>>7)&0x1F)+((Mode&0x03)<<5); // target position: upper 5bits
        data[3]=(unsigned char)(value&0x7F); //target position: lower 7 bits
        data[4]=(data[1]^data[2]^data[3])&0x7F;//check sum
        data[5]=MCU2PC_TERMINATOR_;
        SerialSendData(ui32Base,data);
}

void SerialSend_1_Position8(uint32_t ui32Base,unsigned char ID,unsigned char pos,unsigned char load){
    unsigned char data[7];
    data[0]=MCU2PC_HEADER_;
    data[1]=MCU2PC_MODE_1_;
    data[2]=((load&0x08)>>1)+((pos&0x80)>>2)+(ID&0x1F);
    data[3]=load&0x7F; // target position: upper 5bits
    data[4]=pos&0x7F; //target position: lower 7 bits
    data[5]=(data[1]^data[2]^data[3]^data[4])&0x7F;//check sum
    data[6]=MCU2PC_TERMINATOR_;
    SerialSendData(ui32Base,data);
}
void SerialSend_All_Position(uint32_t ui32Base,unsigned char ID,unsigned int *SamPos){

}

void SerialSend_PID(uint32_t ui32Base,unsigned char ID,unsigned char P, unsigned char I, unsigned char D){
    unsigned char data[9];
    data[0]=MCU2PC_HEADER_;
    data[1]=0x95;
    data[2]=ID; // target position: upper 5bits
    data[3]=((P&0x80)>>5)+((I&0x80)>>6)+((D&0x80)>>7); //target position: lower 7 bits
    data[4]=P&0x7F;
    data[5]=I&0x7F;
    data[6]=D&0x7F;
    data[7]=(data[2]^data[3]^data[4]^data[5]^data[6])&0x7F;//check sum
    data[8]=MCU2PC_TERMINATOR_;

    SerialSendData(ui32Base,data);
}


