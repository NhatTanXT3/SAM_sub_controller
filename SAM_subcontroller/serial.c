/*-----system include----------------*/
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>

#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "inc/tm4c123gh6pm.h"
#include "driverlib/pin_map.h"
#include "driverlib/uart.h"
#include "driverlib/interrupt.h"
#include "utils/ringbuf.h"


/*-----my include-------------------*/
#include "serial.h"
#include "myIO.h"
#include "myFIFO.h"




/* -------define variables----------*/
MyUart UartPC;
MyUart Uart;
MyUart UartCamera;

FIFO communicationFIFO;
FIFO cameraFIFO;

float data_buffer_cn[10];
/*
 * ====================UART6_INIT=====================================
 */
void set_position (char *string, int16_t *position)
{
	while(*string!=STR_TERMINATOR_)
	{
		//		led(LED_BLUE,1);
		switch(*string){
		case SENSOR2CTL_X_:
			*position=atoi(string+1);
			break;
		case SENSOR2CTL_Y_:
			*(position+1)=atoi(string+1);
			break;
		default:
			break;
		}
		string++;
	}
}

//void  UART6_Interrupt_Handler(void)
//{
//#ifndef USE_FIFO_UART_
//	UARTIntClear(UART6_BASE,UART_INT_RT|UART_INT_RX);//|UART_INT_RX
//	SerialGetStr(UART6_BASE,Uart.Command_Data);
//	Uart.Flag_receive=1;
//#else
//	// return: true-the masked interrupt status | false -the current interrupt status
//	uint32_t interrupt_status;
//	interrupt_status=UARTIntStatus(UART6_BASE,true);
//
//	if((((interrupt_status&UART_INT_RT)==UART_INT_RT)|((interrupt_status&UART_INT_RX)==UART_INT_RX))==1)
//	{
//		UARTIntClear(UART6_BASE,UART_INT_RT|UART_INT_RX);//|UART_INT_RX
//		unsigned char flag_valid_data=0;
//		char charData;
//
//		while(UARTCharsAvail(UART6_BASE))
//		{
//			charData=(char)UARTCharGet(UART6_BASE);
//			if(charData==COM_TERMINATOR_)
//			{
//				flag_valid_data=1;
//				FIFO_Rx_CharPut(&communicationFIFO,STR_TERMINATOR_);
//			}
//			else
//				FIFO_Rx_CharPut(&communicationFIFO,charData);
//		}
//
//		if(flag_valid_data==1)
//		{
//			flag_valid_data=0;
//			FIFO_Rx_StrGet(&communicationFIFO,Uart.Command_Data);
//			Uart.Flag_receive=1;
//		}
//	}
//	else if((interrupt_status&UART_INT_TX)==UART_INT_TX)
//	{
//		UARTIntClear(UART6_BASE,UART_INT_TX);
//		update_hardwareFIFO(&communicationFIFO,UART6_BASE);
//	}
//#endif
//}

//void UART1_Interrupt_Handler(void)
//{
//#ifndef USE_FIFO_UART_
//	//	UARTIntClear(UART1_BASE,UART_INT_RT|UART_INT_RX);//|UART_INT_RX
//	//	SerialGetStr(UART1_BASE,Uart.Command_Data);
//	//	Uart.Flag_receive=1;
//	uint32_t interrupt_status;
//	interrupt_status=UARTIntStatus(UART1_BASE,true);
//	interrupt_status=UARTIntStatus(UART1_BASE,true);
//	if((((interrupt_status&UART_INT_RT)==UART_INT_RT)|((interrupt_status&UART_INT_RX)==UART_INT_RX))==1)
//	{
//		interrupt_status=UARTIntStatus(UART1_BASE,true);
//		UARTIntClear(UART1_BASE,UART_INT_RT|UART_INT_RX);//|UART_INT_RX
//		SerialGetStr(UART1_BASE,Uart.Command_Data);
//		Uart.Flag_receive=1;
//	}
//#else
//	// return: true-the masked interrupt status | false -the current interrupt status
//	uint32_t interrupt_status;
//	interrupt_status=UARTIntStatus(UART1_BASE,true);
//
//	if((((interrupt_status&UART_INT_RT)==UART_INT_RT)|((interrupt_status&UART_INT_RX)==UART_INT_RX))==1)
//	{
//		UARTIntClear(UART1_BASE,UART_INT_RT|UART_INT_RX);//|UART_INT_RX
//		unsigned char flag_valid_data=0;
//		char charData;
//
//		while(UARTCharsAvail(UART1_BASE))
//		{
//			charData=(char)UARTCharGet(UART1_BASE);
//			if(charData==COM_TERMINATOR_)
//			{
//				flag_valid_data=1;
//				FIFO_Rx_CharPut(&communicationFIFO,STR_TERMINATOR_);
//			}
//			else
//				FIFO_Rx_CharPut(&communicationFIFO,charData);
//		}
//
//		if(flag_valid_data==1)
//		{
//			flag_valid_data=0;
//			FIFO_Rx_StrGet(&communicationFIFO,Uart.Command_Data);
//			Uart.Flag_receive=1;
//		}
//	}
//
//	else if((interrupt_status&UART_INT_TX)==UART_INT_TX)
//	{
//		UARTIntClear(UART1_BASE,UART_INT_TX);
//		update_hardwareFIFO(&communicationFIFO,UART1_BASE);
//	}
//#endif
//}


//void UART1_Interrupt_Handler(void)
//{
//	UARTIntClear(UART1_BASE,UART_INT_RT|UART_INT_RX);//|UART_INT_RX
//	SerialGetStr(UART1_BASE,Uart.Command_Data);
//
//	if(!(UARTBusy(UART1_BASE))){
//		UartPutStr(UART1_BASE,Uart.Command_Data);
//	}
//	Uart.Flag_receive=1;
//}

void UART0_Interrupt_Handler(void)
{
#ifndef USE_FIFO_UART_
	uint32_t interrupt_status = UARTIntStatus(UART0_BASE,true);
	if(interrupt_status&UART_INT_RX)
	{
		UARTIntClear(UART0_BASE,UART_INT_RX);//|UART_INT_RX
		//		SerialGetStr(UART0_BASE,Uart.Command_Data);
		toggle_led[0]^=1;
		led(LED_RED,toggle_led[0]);
	}
	else if(interrupt_status&UART_INT_RT)
	{
		UARTIntClear(UART0_BASE,UART_INT_RT);//|UART_INT_RX
		//		PCgetData(UART0_BASE,UartPC.Command_Data);
		Uart.Flag_receive=1;

		toggle_led[1]^=1;
		led(LED_BLUE,toggle_led[1]);
	}
#else

	// return: true-the masked interrupt status | false -the current interrupt status

	uint32_t interrupt_status;
	interrupt_status=UARTIntStatus(UART0_BASE,true);

	if((((interrupt_status&UART_INT_RT)==UART_INT_RT)|((interrupt_status&UART_INT_RX)==UART_INT_RX))==1)
	{
		UARTIntClear(UART0_BASE,UART_INT_RT|UART_INT_RX);//|UART_INT_RX
		unsigned char flag_valid_data=0;
		char charData;

		while(UARTCharsAvail(UART0_BASE))
		{
			charData=(char)UARTCharGet(UART0_BASE);
			if(charData==COM_TERMINATOR_)
			{
				flag_valid_data=1;
				FIFO_Rx_CharPut(&communicationFIFO,STR_TERMINATOR_);
			}
			else
				FIFO_Rx_CharPut(&communicationFIFO,charData);
		}

		if(flag_valid_data==1)
		{
			flag_valid_data=0;
			FIFO_Rx_StrGet(&communicationFIFO,Uart.Command_Data);
			Uart.Flag_receive=1;
			//			UartPutStr(UART1_BASE,Uart.Command_Data);
		}

		//		SerialGetStr(UART1_BASE,Uart.Command_Data);
		//		Uart.Flag_receive=1;
	}
	else if((interrupt_status&UART_INT_TX)==UART_INT_TX)
	{
		UARTIntClear(UART0_BASE,UART_INT_TX);
		update_hardwareFIFO(&communicationFIFO,UART0_BASE);
	}
#endif
}


//void UART6_Init()
//{
//	//
//	// Enable Peripheral Clocks
//	//
//	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART6);
//	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
//
//	//
//	// Enable pin PD5 for UART6 U6TX
//	//
//	GPIOPinConfigure(GPIO_PD5_U6TX);
//	GPIOPinTypeUART(GPIO_PORTD_BASE, GPIO_PIN_5);
//
//	//
//	// Enable pin PD4 for UART6 U6RX
//	//
//	GPIOPinConfigure(GPIO_PD4_U6RX);
//	GPIOPinTypeUART(GPIO_PORTD_BASE, GPIO_PIN_4);
//
//	UARTConfigSetExpClk(UART6_BASE, SysCtlClockGet(), 115200,
//			(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));
//
//
//	IntMasterEnable(); //enable processor interrupts
//	IntEnable(INT_UART6); //enable the UART interrupt
//
//	IntPrioritySet(INT_UART6,0xE0);
//
//#ifdef USE_FIFO_UART_
//	UARTTxIntModeSet(UART6_BASE,UART_TXINT_MODE_EOT);
//	UARTIntEnable(UART6_BASE,  UART_INT_RT|UART_INT_RX|UART_INT_TX); //only enable RX and RTinterrupts
//	myFIFO_init(&communicationFIFO);
//#else
//	UARTIntEnable(UART6_BASE, UART_INT_RX|UART_INT_RT);
//#endif
//
//}



void UART0_Init()
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

	GPIOPinConfigure(GPIO_PA0_U0RX);
	GPIOPinConfigure(GPIO_PA1_U0TX);
	GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

	UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 115200,
			(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));

	IntMasterEnable(); //enable processor interrupts
	IntEnable(INT_UART0); //enable the UART interrupt
	//	IntPrioritySet(INT_UART0,0xE0);

#ifdef USE_FIFO_UART_
	UARTTxIntModeSet(UART0_BASE,UART_TXINT_MODE_EOT);
	UARTIntEnable(UART0_BASE, UART_INT_RX|UART_INT_RT|UART_INT_TX);
	//	myFIFO_init(&communicationFIFO);
	myFIFO_init(&cameraFIFO);
#else
	UARTFIFOLevelSet(UART0_BASE,UART_FIFO_TX1_8,UART_FIFO_RX2_8);
	//	UARTTxIntModeSet(UART0_BASE,UART_TXINT_MODE_EOT);
	UARTIntEnable(UART0_BASE, UART_INT_RX|UART_INT_RT);
#endif



}

//uart5: PE4-Rx PE5 -Tx
//void UART5_Init()
//{
//	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART5);
//	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
//
//	GPIOPinConfigure(GPIO_PE4_U5RX);
//	GPIOPinTypeUART(GPIO_PORTE_BASE, GPIO_PIN_4);
//
//	GPIOPinConfigure(GPIO_PE5_U5TX);
//	GPIOPinTypeUART(GPIO_PORTE_BASE, GPIO_PIN_5);
//
//	UARTConfigSetExpClk(UART5_BASE, SysCtlClockGet(), 115200,
//			(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));
//}

// UART1: PB0-RX | PB1-TX
//void UART1_Init()
//{
//	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART1);
//	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
//
//	GPIOPinConfigure(GPIO_PB0_U1RX);
//	GPIOPinTypeUART(GPIO_PORTB_BASE, GPIO_PIN_0);
//
//	GPIOPinConfigure(GPIO_PB1_U1TX);
//	GPIOPinTypeUART(GPIO_PORTB_BASE, GPIO_PIN_1);
//
//	/*
//	 * setting the baud clock source from the system clock
//	 */
//	UARTClockSourceSet(UART1_BASE,UART_CLOCK_SYSTEM);
//
//	UARTConfigSetExpClk(UART1_BASE, SysCtlClockGet(), 115200,
//			(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));
//
//	/*
//	 * Enable the UART.
//	 */
//	UARTEnable(UART1_BASE);
//
//	IntMasterEnable(); //enable processor interrupts
//	IntEnable(INT_UART1); //enable the UART interrupt
//
//	IntPrioritySet(INT_UART1,0xE0);
//	UARTFIFOLevelSet(UART1_BASE,UART_FIFO_TX1_8,UART_FIFO_RX4_8);
//
//	//	UARTTxIntModeSet(UART1_BASE,UART_TXINT_MODE_EOT);
//	//	UARTIntEnable(UART1_BASE,  UART_INT_RT|UART_INT_RX|UART_INT_TX); //only enable time out interrupts
//	//	UARTIntEnable(UART1_BASE,  UART_INT_RT|UART_INT_RX);
//
//#ifdef USE_FIFO_UART_
//	UARTTxIntModeSet(UART1_BASE,UART_TXINT_MODE_EOT);
//	UARTIntEnable(UART1_BASE, UART_INT_RX|UART_INT_RT|UART_INT_TX);
//	myFIFO_init(&communicationFIFO);
//#else
//	//	UARTIntEnable(UART1_BASE, UART_INT_RX|UART_INT_RT);
//	UARTIntEnable(UART1_BASE, UART_INT_RX);
//#endif
//}


#ifndef USE_FIFO_UART_

void SerialPutData(uint32_t ui32Base,char *uart_data,unsigned char dataSize){
	unsigned char i;
	for(i=0;i<dataSize;i++)
	{
		//		RingBufWriteOne(&(communicationFIFO.Tx),*uart_data++);
		UARTCharPut(ui32Base,*uart_data++ );
	}
	//	RingBufWriteOne(&(communicationFIFO.Tx),'\n');
	//	update_hardwareFIFO(&communicationFIFO,ui32Base);


}

void SerialPutChar(uint32_t ui32Base,unsigned char uart_char)
{
	UARTCharPut(ui32Base,uart_char);
}

void SerialPutStr(uint32_t ui32Base,char *uart_str)
{
	while(*uart_str != '\0') {UARTCharPut(ui32Base,*uart_str++ );}
	UARTCharPut(ui32Base,STR_TERMINATOR_);
}

void SerialPutStrLn(uint32_t ui32Base,char *uart_str)
{
	while(*uart_str != '\0') {UARTCharPut(ui32Base,*uart_str++ );}
//	UARTCharPut(ui32Base,127);
	UARTCharPut(ui32Base,'\r');
	UARTCharPut(ui32Base,'\n');
}

void  SerialPutStr_NonTer(uint32_t ui32Base,char *uart_str)
{
	while(*uart_str != '\0') {UARTCharPut(ui32Base,*uart_str++ );}
}

void SerialGetStr(uint32_t ui32Base,char *uart_str)
{
	char charData;
	charData=(char)UARTCharGet(ui32Base);
	//	*uart_str= (char)UARTCharGet(ui32Base);

	// dangerous!!! it will halt if there is no comunication.
	while (charData != COM_TERMINATOR_)
	{
		*uart_str=charData;
		uart_str++;
		charData=(char)UARTCharGet(ui32Base);
	}
	*(uart_str)=STR_TERMINATOR_;
}

void SerialTerminator(uint32_t ui32Base)
{
	UARTCharPut(ui32Base,'\r');
	UARTCharPut(ui32Base,'\n');
}

#else
void SerialPutData(uint32_t ui32Base,char *uart_data,unsigned char dataSize){
	unsigned char i;
	for(i=0;i<dataSize;i++)
	{
		RingBufWriteOne(&(communicationFIFO.Tx),*uart_data++);
	}
	RingBufWriteOne(&(communicationFIFO.Tx),'\n');
	update_hardwareFIFO(&communicationFIFO,ui32Base);
}

void SerialPutChar(uint32_t ui32Base,unsigned char uart_char)
{
	RingBufWriteOne(&(communicationFIFO.Tx),uart_char);

}

void SerialPutStr(uint32_t ui32Base,char *uart_str)
{
	while(*uart_str != '\0') {RingBufWriteOne(&(communicationFIFO.Tx),*uart_str++);}
	RingBufWriteOne(&(communicationFIFO.Tx),STR_TERMINATOR_);
}

// put data and update
void SerialPutStrLn(uint32_t ui32Base,char *uart_str)
{
	toggle_led[1]^=1;
	led(LED_BLUE,toggle_led[1]);
	while(*uart_str != '\0') {RingBufWriteOne(&(communicationFIFO.Tx),*uart_str++);}
	RingBufWriteOne(&(communicationFIFO.Tx),'\r');
	RingBufWriteOne(&(communicationFIFO.Tx),'\n');
	toggle_led[1]^=1;
	led(LED_BLUE,toggle_led[1]);

	update_hardwareFIFO(&communicationFIFO,ui32Base);
	toggle_led[1]^=1;
	led(LED_BLUE,toggle_led[1]);
}

void  SerialPutStr_NonTer(uint32_t ui32Base,char *uart_str)
{
	while(*uart_str != '\0') {RingBufWriteOne(&(communicationFIFO.Tx),*uart_str++);}
}

void SerialTerminator(uint32_t ui32Base)
{
	RingBufWriteOne(&(communicationFIFO.Tx),'\r');
	RingBufWriteOne(&(communicationFIFO.Tx),'\n');
	update_hardwareFIFO(&communicationFIFO,ui32Base);
}

//void forward_message(uint32_t ui32Base,char *uart_str)
//{
//	while(*uart_str != '\n') {RingBufWriteOne(&(communicationFIFO.Tx),*uart_str++);}
//	RingBufWriteOne(&(communicationFIFO.Tx),'\r');
//	RingBufWriteOne(&(communicationFIFO.Tx),'\n');
//	update_hardwareFIFO(&communicationFIFO,UART_COM_2_CONTROLLER_);
//}

//void SerialGetStr(uint32_t ui32Base,char *uart_str)
//{
//	*uart_str= (char)UARTCharGet(ui32Base);
//	while (*uart_str != '\n')
//	{
//		uart_str++;
//		*uart_str= (char)UARTCharGet(ui32Base);
//	}
//	*(uart_str+1)='\0';
//}

//void Serial_Sendcommand_Camera(char *uart_str)
//{
//	while(*uart_str != '\0') {RingBufWriteOne(&(cameraFIFO.Tx),*uart_str++);}
//	RingBufWriteOne(&(cameraFIFO.Tx),'\n');
//	update_hardwareFIFO(&cameraFIFO, UART_IMAGE_SENSOR_2_CONTROLLER_);
//}

#endif






