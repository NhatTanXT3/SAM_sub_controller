/*
 * main.c
 * Readversion
 * Test
 */
#include <stdint.h>
#include <stdbool.h>
//#include <math.h>
//#include <stdlib.h>
#include "SAM.h"



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

#include "mySerial.h"
#include "serial.h"
#include "myFIFO.h"
#include "myTimer.h"
#include "myIO.h"
#include "myRS485.h"

#include "numManipulate.h"

void task_100Hz();
void task_50Hz();
void task_20Hz();
void communication();
void display_com();

#define UPPER_10BIT_BOUNDARY_  3700
#define LOWER_10BIT_BOUNDARY_  401

//void RS485PutChar(unsigned char uart_char){
//	led(LED_GREEN,1);
//	SerialPutChar(UART_RS485,uart_char);
//}
//
//void RS485PutStr(char *uart_str){
//	led(LED_GREEN,1);
//	SerialPutStrLn(UART_RS485,uart_str);
//}

unsigned int count =1;

void uax_get_jointAngle10bit(uint32_t ui32Base,unsigned char ID){
	char data[7];
	data[0]=0xFF;
	data[1]=0xE0;
	//			0x22;
	data[2]=173;
	data[3]=ID;
	data[4]=0;
	data[5]=0;
	data[6]=(data[1]^data[2]^data[3]^data[4]^data[5])&0x7F;

	switch(ui32Base){
	case UART_RS485_4_:
		GPIOPinWrite(GPIO_PORTA_BASE,GPIO_PIN_3,GPIO_PIN_3);
		break;
	case UART_RS485_3_:
		GPIOPinWrite(GPIO_PORTA_BASE,GPIO_PIN_4,GPIO_PIN_4);
		break;
	default:
		break;
	}

	toggle_led[1]^=1;
	led(LED_BLUE,toggle_led[1]);

	flagReadBusy=1;

	UARTCharPut(ui32Base,data[0]);
	UARTCharPut(ui32Base,data[1]);
	UARTCharPut(ui32Base,data[2]);
	UARTCharPut(ui32Base,data[3]);
	UARTCharPut(ui32Base,data[4]);
	UARTCharPut(ui32Base,data[5]);
	UARTCharPut(ui32Base,data[6]);
	Timer1_Reset();
}

void uax_set_jointAngle10bit(uint32_t ui32Base,unsigned char ID,unsigned int value){
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

		switch(ui32Base){
		case UART_RS485_4_:
			GPIOPinWrite(GPIO_PORTA_BASE,GPIO_PIN_3,GPIO_PIN_3);
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

void uax_get_jointAngle8bit(uint32_t ui32Base,unsigned char ID)
{
	unsigned char data[4];
	data[0]=0xFF;
	data[1]=(5<<5)|(ID);
	//			0x22;
	data[2]=255-count;
	data[3]=(data[1]^data[2])&0x7F;

	switch(ui32Base){
	case UART_RS485_4_:
		GPIOPinWrite(GPIO_PORTA_BASE,GPIO_PIN_3,GPIO_PIN_3);
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
}

void uax_set_jointAngle8bit(uint32_t ui32Base,unsigned char ID,unsigned char value)
{
	unsigned char data[4];
	data[0]=0xFF;
	data[1]=(4<<5)|(ID);
	data[2]=value;
	data[3]=(data[1]^data[2])&0x7F;

	switch(ui32Base){
	case UART_RS485_4_:
		GPIOPinWrite(GPIO_PORTA_BASE,GPIO_PIN_3,GPIO_PIN_3);
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
}

void RS485PutData(){
	char data[10];
	if (count==255)
		count=1;
	else count=count+1;

	data[0]=0xFF;
	data[1]=(4<<5)|(0);
	//			0x22;
	data[2]=count;
	data[3]=(data[1]^data[2])&0x7F;
	//	led(LED_GREEN,1);
	//	GPIOPinWrite(GPIO_PORTA_BASE,GPIO_PIN_4,GPIO_PIN_4);
	//	UARTCharPut(UART_RS485,data[0]);
	//	UARTCharPut(UART_RS485,data[1]);
	//	UARTCharPut(UART_RS485,data[2]);
	//	UARTCharPut(UART_RS485,data[3]);

}

void main(){
	SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ); //40Mhz
	myIO_init();
	led(LED_RED,0);
	led(LED_BLUE,0);
	led(LED_GREEN,0);


	Timer0_init();
	Timer1_init();
	SysTick_Init();

	//	UART0_Init();
	//	RS485_3_Init();
	RS485_4_Init();

	UART5_Init();

	//	UART1_Init();

	SerialPutStrLn(UART_PC_,"config done!");
	//	char buffer[20];
	//	int2num((SysCtlClockGet()/1000),buffer);
	//	SerialPutStrLn(UART_PC_,buffer);

	//	SerialPutStr_NonTer(UART_PC_,"config done2");
	UARTCharPut(UART_PC_,127);

	//	unsigned char test=6;
	//	int2num(test%4,buffer);
	//	SerialPutStrLn(UART_PC_,buffer);
	//	int2num(test/4,buffer);
	//	SerialPutStrLn(UART_PC_,buffer);
	while(1)
	{
		communication();
		task_20Hz();
		task_50Hz();
		task_100Hz();
	}
}
void task_20Hz(){
	if(FlagTimer.Hz_20)
	{
		FlagTimer.Hz_20=0;
		/*
		 * Your code begin from here
		 */
		//		toggle_led[0]^=1;
		//		led(LED_RED,toggle_led[0]);
		//		RS485PutData();
		//		if (count>UPPER_10BIT_BOUNDARY_)
		//			count=LOWER_10BIT_BOUNDARY_;
		//		else count=count+5;
		//		//		if (count==255)
		//		//				count=1;
		//		//			else count=count+1;
		//				uax_set_jointAngle10bit(0,count);
		//		SysCtlDelay(SysCtlClockGet()/3/20000);
		//		//		toggle_led[0]^=1;
		//		//		led(LED_RED,toggle_led[0]);
		//		//		uax_get_jointAngle10bit(0);



		//		uax_get_jointAngle10bit(0);
		//		RS485PutStr("hello friend");
		//		SerialPutStrLn(UART_BASE_USE,"config done!")
	}


}

void task_100Hz(){
	if(FlagTimer.Hz_100)
	{
		FlagTimer.Hz_100=0;
		/*
		 * Your code begin from here
		 */
	}
}
void task_50Hz(){
	if(FlagTimer.Hz_50)
	{
		FlagTimer.Hz_50=0;
		/*
		 * Your code begin from here
		 */
		//		toggle_led[1]^=1;
		//		led(LED_BLUE,toggle_led[1]);
		//		Timer1_Reset();

	}
}

struct{
	unsigned char display:1;
}Flag;

void communication(){
	if(serialPC.Flag_receive){
		serialPC.Flag_receive=0;
		//		FIFO_Rx_DataGet(&serialPcFIFO,serialPC.Command_Data);
		//		serialPC.Command_Data=
		if(serialPC.Command_Data[0]==PC_HEADER_)
		{
			if(serialPC.Command_Data[1]&0x80) // special command
			{
				switch(serialPC.Command_Data[1]){
				case  PC_SAM_SP_MODE_1_:
					SerialPutStrLn(UART_PC_,"sm1");
					if(((serialPC.dataIndex-3)%4)==0){
						unsigned char numOfSam=serialPC.dataIndex/4;
						unsigned char i;
						unsigned char refIndex;
						for(i=0;i<numOfSam;i++)
						{
							refIndex=4*i+2;
							if(((serialPC.Command_Data[refIndex]^serialPC.Command_Data[refIndex+1]^serialPC.Command_Data[refIndex+2])&0x7F)==serialPC.Command_Data[refIndex+3])
							{
								sam1.id=serialPC.Command_Data[refIndex]&0x1F;
								sam1.mode=serialPC.Command_Data[refIndex]>>5;
								sam1.SAMmode=((serialPC.Command_Data[refIndex]&0x60)>>5)+((serialPC.Command_Data[refIndex+1]&0x60)>>3);
								sam1.position12=((serialPC.Command_Data[refIndex+1]&0x1F)<<7)+(serialPC.Command_Data[refIndex+2]&0X7F);
								uax_set_jointAngle10bit(UART_RS485_4_,sam1.id,sam1.position12);
								//							display_com();
							}
							else
							{
								UARTCharPut(UART_PC_,i+48);
								SerialPutStrLn(UART_PC_,"e_cs");
							}
						}
					}
					else{
						SerialPutStrLn(UART_PC_,"e_l_2");
					}

					break;
				case PC_SAM_SP_MODE_2_:
					SerialPutStrLn(UART_PC_,"sm2");
					break;
				default:
					break;

				}
			}
			else{
				if(serialPC.dataIndex==PC_SAM_MODE_1_DATALENGTH_)
				{
					unsigned char mode=((serialPC.Command_Data[1]&0x60)>>5)+((serialPC.Command_Data[2]&0x60)>>3);
					switch(mode)
					{
					case PC_SAM_MODE_1_:
						SerialPutStrLn(UART_PC_,"M1");
						if(((serialPC.Command_Data[1]^serialPC.Command_Data[2]^serialPC.Command_Data[3])&0x7F)==serialPC.Command_Data[4])
						{
							sam1.id=serialPC.Command_Data[1]&0x1F;
							sam1.mode=PC_SAM_MODE_1_;
//							sam1.SAMmode=((serialPC.Command_Data[1]&0x60)>>5)+((serialPC.Command_Data[2]&0x60)>>3);
							sam1.position12=((serialPC.Command_Data[2]&0x1F)<<7)+(serialPC.Command_Data[3]&0X7F);
							uax_set_jointAngle10bit(UART_RS485_4_,sam1.id,sam1.position12);
							//display_com();
						}
						else
						{
							SerialPutStrLn(UART_PC_,"e_cs");
						}
						break;
					case PC_SAM_MODE_2_:
						SerialPutStrLn(UART_PC_,"M2");

						while(flagReadBusy);
						uax_get_jointAngle10bit(UART_RS485_4_,sam1.id);



						break;
					case PC_SAM_MODE_3_:
						SerialPutStrLn(UART_PC_,"M3");
						break;
					case PC_SAM_MODE_4_:
						SerialPutStrLn(UART_PC_,"M4");
						break;
					default:
						SerialPutStrLn(UART_PC_,"M?");
						break;
					}

				}
				else
				{
					SerialPutStrLn(UART_PC_,"e_l_1");
				}

			}


			//			if(((serialPC.Command_Data[1]^serialPC.Command_Data[2]^serialPC.Command_Data[3])&0x7F)==serialPC.Command_Data[4])
			//			{
			//				sam1.id=serialPC.Command_Data[1]&0x1F;
			//				sam1.mode=serialPC.Command_Data[1]>>5;
			//				sam1.position12=(serialPC.Command_Data[2]<<7)+(serialPC.Command_Data[3]&0X7F);
			//				uax_set_jointAngle10bit(sam1.id,sam1.position12);
			//				display_com();
			//			}
			//			else
			//			{
			//				SerialPutStrLn(UART_PC_,"error checksum");
			//			}
			//			switch(UartPC.Command_Data[1])
			//			{
			//			case COM2CTL_DISPLAY_ON_:
			//				Flag.display=1;
			//				//			display_mode=UartPC.Command_Data[1];
			//				break;
			//			case COM2CTL_DISPLAY_OFF_:
			//				Flag.display=0;
			//				break;
			//			}

		}
		else
		{
			serialPC.dataIndex=0;
			SerialPutStrLn(UART_PC_,"e_h");
		}
	}
}

unsigned char display_mode=COM2CTL_DISPLAY_MODE_1_;

void display_com(){
	char buffer[20];

	if(display_mode==COM2CTL_DISPLAY_MODE_0_)
	{

	}
	else if(display_mode==COM2CTL_DISPLAY_MODE_1_)
	{

		/*
		 * khao sat PID_z
		 */
		int2num(sam1.id,buffer);
		SerialPutStr_NonTer(UART_PC_,"id = ");
		SerialPutStr_NonTer(UART_PC_,buffer);

		int2num(sam1.mode,buffer);
		SerialPutStr_NonTer(UART_PC_,"mode = ");
		SerialPutStr_NonTer(UART_PC_,buffer);

		int2num(sam1.position12,buffer);
		SerialPutStr_NonTer(UART_PC_,"position12 = ");
		SerialPutStr_NonTer(UART_PC_,buffer);

		//		float2str(Sonar_module.attitude,buffer);
		//		SerialPutChar(UART_COM_2_CONTROLLER_ ,CN_1_);
		//		SerialPutStr_NonTer(UART_COM_2_CONTROLLER_,buffer);
		//
		//		float2str(PID_z.set_point,buffer);
		//		SerialPutChar(UART_COM_2_CONTROLLER_ ,CN_2_);
		//		SerialPutStr_NonTer(UART_COM_2_CONTROLLER_,buffer);
		//
		//		float2str(Socket.attitude_hold_throttle,buffer);
		//		SerialPutChar(UART_COM_2_CONTROLLER_ ,CN_3_);
		//		SerialPutStr_NonTer(UART_COM_2_CONTROLLER_,buffer);
		//		//Dte
		//		float2str(PID_z.P_term,buffer);
		//		SerialPutChar(UART_COM_2_CONTROLLER_ ,CN_4_);
		//		SerialPutStr_NonTer(UART_COM_2_CONTROLLER_,buffer);
		//		//
		//		float2str(PID_z.I_term,buffer); //acc_term
		//		SerialPutChar(UART_COM_2_CONTROLLER_ ,CN_5_);
		//		SerialPutStr_NonTer(UART_COM_2_CONTROLLER_,buffer);
		//
		//		float2str(PID_z.D_term,buffer);
		//		SerialPutChar(UART_COM_2_CONTROLLER_ ,CN_6_);
		//		SerialPutStr_NonTer(UART_COM_2_CONTROLLER_,buffer);
		//		//
		//		float2str(PID_z.KP*100.0,buffer);
		//		SerialPutChar(UART_COM_2_CONTROLLER_ ,CN_7_);
		//		SerialPutStr_NonTer(UART_COM_2_CONTROLLER_,buffer);
		//
		//		float2str(PID_z.KI*100.0,buffer);
		//		SerialPutChar(UART_COM_2_CONTROLLER_ ,CN_8_);
		//		SerialPutStr_NonTer(UART_COM_2_CONTROLLER_,buffer);
		//		//
		//		float2str(PID_z.KD*100.0,buffer);
		//		SerialPutChar(UART_COM_2_CONTROLLER_ ,CN_9_);
		//		SerialPutStr_NonTer(UART_COM_2_CONTROLLER_,buffer);

		//		float2str(-12343.23,buffer);
		//		SerialPutChar(UART_COM_2_CONTROLLER_ ,CN_10_);
		//		SerialPutStrLn(UART_COM_2_CONTROLLER_,buffer);
	}
	SerialTerminator(UART_PC_);
}


