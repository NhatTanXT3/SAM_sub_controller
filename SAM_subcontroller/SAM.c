/*
 * SAM.c
 *
 *  Created on: Feb 27, 2017
 *      Author: Nhat Tan
 */
#include <stdint.h>
#include <stdbool.h>

#include "inc/tm4c123gh6pm.h"
#include "inc/hw_memmap.h"
#include "driverlib/pin_map.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/uart.h"
#include "driverlib/interrupt.h"
#include "driverlib/systick.h"
#include "utils/ringbuf.h"

#include "SAM.h"
#include "myRS485.h"

SAM sam1;
volatile unsigned int samPosition12[32];
volatile unsigned char samDataAvail[32];
volatile unsigned char  samReadBusy;
volatile unsigned char samReadCurrentID_C4;
volatile unsigned char samReadCurrentID_C2;
//RS485_Channel_State channel_1, channel_2, channel3;

SAM sam1;

//FlagDataAvailSAM flagDataAvailSAM;
////FlagBusySAM flagBusySAM;
//
//volatile unsigned char Channel_1;
//volatile unsigned char flagReadSuccess;
//volatile unsigned char Channel_1_ReadID;


void SAM_init(){

}

void SAM_get_jointAngle10bit(unsigned char ID){
	uint32_t ui32Base;
	char data[7];
	data[0]=0xFF;
	data[1]=0xE0;
	//			0x22;
	data[2]=173;
	data[3]=ID;
	data[4]=0;
	data[5]=0;
	data[6]=(data[1]^data[2]^data[3]^data[4]^data[5])&0x7F;

	switch (ID){
	case 0:
		ui32Base=UART_RS485_ID_0_;
		break;
	case 1:
		ui32Base=UART_RS485_ID_1_;
		break;
	case 2:
		ui32Base=UART_RS485_ID_2_;
		break;
	case 3:
		ui32Base=UART_RS485_ID_3_;
		break;
	case 4:
		ui32Base=UART_RS485_ID_4_;
		break;
	case 5:
		ui32Base=UART_RS485_ID_5_;
		break;
	case 6:
		ui32Base=UART_RS485_ID_6_;
		break;
	case 7:
		ui32Base=UART_RS485_ID_7_;
		break;
	case 8:
		ui32Base=UART_RS485_ID_8_;
		break;
	case 9:
		ui32Base=UART_RS485_ID_9_;
		break;
	case 10:
		ui32Base=UART_RS485_ID_10_;
		break;
	case 11:
		ui32Base=UART_RS485_ID_11_;
		break;
	case 12:
		ui32Base=UART_RS485_ID_12_;
		break;
	case 13:
		ui32Base=UART_RS485_ID_13_;
		break;
	case 14:
		ui32Base=UART_RS485_ID_14_;
		break;
	case 15:
		ui32Base=UART_RS485_ID_15_;
		break;
	case 16:
		ui32Base=UART_RS485_ID_16_;
		break;
	case 17:
		ui32Base=UART_RS485_ID_17_;
		break;
	case 18:
		ui32Base=UART_RS485_ID_18_;
		break;
	case 19:
		ui32Base=UART_RS485_ID_19_;
		break;
	case 20:
		ui32Base=UART_RS485_ID_20_;
		break;
	case 21:
		ui32Base=UART_RS485_ID_21_;
		break;
	case 22:
		ui32Base=UART_RS485_ID_22_;
		break;
	case 23:
		ui32Base=UART_RS485_ID_23_;
		break;
	case 24:
		ui32Base=UART_RS485_ID_24_;
		break;
	case 25:
		ui32Base=UART_RS485_ID_25_;
		break;
	case 26:
		ui32Base=UART_RS485_ID_26_;
		break;
	case 27:
		ui32Base=UART_RS485_ID_27_;
		break;
	default:
		break;

	}

	switch(ui32Base){
	case UART_RS485_4_:
		samReadCurrentID_C4=ID;
		GPIOPinWrite(GPIO_PORTA_BASE,GPIO_PIN_3,GPIO_PIN_3);
		break;
	case UART_RS485_2_:
		samReadCurrentID_C2=ID;
		GPIOPinWrite(GPIO_PORTA_BASE,GPIO_PIN_2,GPIO_PIN_2);
		break;
	case UART_RS485_3_:
		GPIOPinWrite(GPIO_PORTA_BASE,GPIO_PIN_4,GPIO_PIN_4);
		break;
	default:
		break;
	}


	//	flagBusyChannel_1=1;
	//	samReadBusy=1;

	UARTCharPut(ui32Base,data[0]);
	UARTCharPut(ui32Base,data[1]);
	UARTCharPut(ui32Base,data[2]);
	UARTCharPut(ui32Base,data[3]);
	UARTCharPut(ui32Base,data[4]);
	UARTCharPut(ui32Base,data[5]);
	UARTCharPut(ui32Base,data[6]);
	//	Timer1_Reset();
}

void SAM_set_jointAngle10bit(unsigned char ID,unsigned int value){
	uint32_t ui32Base;

	if((value>=LOWER_10BIT_BOUNDARY_)&&(value<=UPPER_10BIT_BOUNDARY_))
	{
		unsigned char data[7];
		data[0]=0xFF;
		data[1]=0xE0;
		//			0x22;
		data[2]=200;
		data[3]=ID;

		data[4]=(unsigned char)((value>>7)&0x1F); // target position: upper 5bits
		data[5]=(unsigned char)(value&0x7F); //target position: lower 7 bits

		data[6]=(data[1]^data[2]^data[3]^data[4]^data[5])&0x7F;//check sum

		switch (ID){
		case 0:
			ui32Base=UART_RS485_ID_0_;
			break;
		case 1:
			ui32Base=UART_RS485_ID_1_;
			break;
		case 2:
			ui32Base=UART_RS485_ID_2_;
			break;
		case 3:
			ui32Base=UART_RS485_ID_3_;
			break;
		case 4:
			ui32Base=UART_RS485_ID_4_;
			break;
		case 5:
			ui32Base=UART_RS485_ID_5_;
			break;
		case 6:
			ui32Base=UART_RS485_ID_6_;
			break;
		case 7:
			ui32Base=UART_RS485_ID_7_;
			break;
		case 8:
			ui32Base=UART_RS485_ID_8_;
			break;
		case 9:
			ui32Base=UART_RS485_ID_9_;
			break;
		case 10:
			ui32Base=UART_RS485_ID_10_;
			break;
		case 11:
			ui32Base=UART_RS485_ID_11_;
			break;
		case 12:
			ui32Base=UART_RS485_ID_12_;
			break;
		case 13:
			ui32Base=UART_RS485_ID_13_;
			break;
		case 14:
			ui32Base=UART_RS485_ID_14_;
			break;
		case 15:
			ui32Base=UART_RS485_ID_15_;
			break;
		case 16:
			ui32Base=UART_RS485_ID_16_;
			break;
		case 17:
			ui32Base=UART_RS485_ID_17_;
			break;
		case 18:
			ui32Base=UART_RS485_ID_18_;
			break;
		case 19:
			ui32Base=UART_RS485_ID_19_;
			break;
		case 20:
			ui32Base=UART_RS485_ID_20_;
			break;
		case 21:
			ui32Base=UART_RS485_ID_21_;
			break;
		case 22:
			ui32Base=UART_RS485_ID_22_;
			break;
		case 23:
			ui32Base=UART_RS485_ID_23_;
			break;
		case 24:
			ui32Base=UART_RS485_ID_24_;
			break;
		case 25:
			ui32Base=UART_RS485_ID_25_;
			break;
		case 26:
			ui32Base=UART_RS485_ID_26_;
			break;
		case 27:
			ui32Base=UART_RS485_ID_27_;
			break;
		default:
			break;

		}


		switch(ui32Base){
		case UART_RS485_4_:
			GPIOPinWrite(GPIO_PORTA_BASE,GPIO_PIN_3,GPIO_PIN_3);
			break;
		case UART_RS485_2_:
			GPIOPinWrite(GPIO_PORTA_BASE,GPIO_PIN_2,GPIO_PIN_2);
			break;
		case UART_RS485_3_:
			GPIOPinWrite(GPIO_PORTA_BASE,GPIO_PIN_4,GPIO_PIN_4);
			break;
		default:
			break;
		}

		UARTCharPut(ui32Base,data[0]);
		UARTCharPut(ui32Base,data[1]);
		UARTCharPut(ui32Base,data[2]);
		UARTCharPut(ui32Base,data[3]);
		UARTCharPut(ui32Base,data[4]);
		UARTCharPut(ui32Base,data[5]);
		UARTCharPut(ui32Base,data[6]);
	}
}

void SAM_get_jointAngle8bit(uint32_t ui32Base,unsigned char ID)
{
	unsigned char data[4];
	data[0]=0xFF;
	data[1]=(5<<5)|(ID);
	//			0x22;
	data[2]=0;
	data[3]=(data[1]^data[2])&0x7F;

	//	switch(ui32Base){
	//	case UART_RS485_4_:
	//		GPIOPinWrite(GPIO_PORTA_BASE,GPIO_PIN_3,GPIO_PIN_3);
	//		break;
	//	case UART_RS485_3_:
	//		GPIOPinWrite(GPIO_PORTA_BASE,GPIO_PIN_4,GPIO_PIN_4);
	//		break;
	//	default:
	//		break;
	//	}
	UARTCharPut(ui32Base,data[0]);
	UARTCharPut(ui32Base,data[1]);
	UARTCharPut(ui32Base,data[2]);
	UARTCharPut(ui32Base,data[3]);
}

void SAM_set_jointAngle8bit(uint32_t ui32Base,unsigned char ID,unsigned char value)
{
	unsigned char data[4];
	data[0]=0xFF;
	data[1]=(4<<5)|(ID);
	data[2]=value;
	data[3]=(data[1]^data[2])&0x7F;

	//	switch(ui32Base){
	//	case UART_RS485_4_:
	//		GPIOPinWrite(GPIO_PORTA_BASE,GPIO_PIN_3,GPIO_PIN_3);
	//		break;
	//	case UART_RS485_3_:
	//		GPIOPinWrite(GPIO_PORTA_BASE,GPIO_PIN_4,GPIO_PIN_4);
	//		break;
	//	default:
	//		break;
	//	}

	UARTCharPut(ui32Base,data[0]);
	UARTCharPut(ui32Base,data[1]);
	UARTCharPut(ui32Base,data[2]);
	UARTCharPut(ui32Base,data[3]);
}

