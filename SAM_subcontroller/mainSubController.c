/*
 * main.c
 * Readversion
 * Test
 */
#include <stdint.h>
#include <stdbool.h>
//#include <math.h>
//#include <stdlib.h>




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
#include "SAM.h"

#include "numManipulate.h"

void task_100Hz();
void task_50Hz();
void task_20Hz();
void communication();
void display_com();


void main(){
    SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ); //40Mhz
    myIO_init();
    SAM_Power_enable(0);
    led(LED_RED,0);
    led(LED_BLUE,0);
    led(LED_GREEN,0);


    Timer0_init();
    Timer1_init();
    SysTick_Init();

    RS485_4_Init();
    RS485_2_Init();
    //    RS485_3_Init();
    RS485_1_Init();
    RS485_6_Init();
    RS485_7_Init();

    UART5_Init();

    //  UART1_Init();
    UARTCharPut(UART_PC_,128);
    SerialPutStrLn(UART_PC_,"config done!");
    SysCtlDelay(SysCtlClockGet()/300);
    SAM_Power_enable(1);


    //  SerialPutStrLn(UART_PC_,"2");
    //  char buffer[20];
    //  int2num((SysCtlClockGet()/1000),buffer);
    //  SerialPutStrLn(UART_PC_,buffer);

    //  SerialPutStr_NonTer(UART_PC_,"config done2");
    //  UARTCharPut(UART_PC_,127);


    //  unsigned char test=6;
    //  int2num(test%4,buffer);
    //  SerialPutStrLn(UART_PC_,buffer);
    //  int2num(test/4,buffer);
    //  SerialPutStrLn(UART_PC_,buffer);
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
        //      toggle_led[0]^=1;
        //      led(LED_RED,toggle_led[0]);
        //      RS485PutData();
        //      if (count>UPPER_10BIT_BOUNDARY_)
        //          count=LOWER_10BIT_BOUNDARY_;
        //      else count=count+5;
        //      //      if (count==255)
        //      //              count=1;
        //      //          else count=count+1;
        //              SAM_set_jointAngle10bit(0,count);
        //      SysCtlDelay(SysCtlClockGet()/3/20000);
        //      //      toggle_led[0]^=1;
        //      //      led(LED_RED,toggle_led[0]);
        //      //      SAM_get_jointAngle10bit(0);



        //      SAM_get_jointAngle10bit(0);
        //      RS485PutStr("hello friend");
        //      SerialPutStrLn(UART_BASE_USE,"config done!")
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
        //      toggle_led[1]^=1;
        //      led(LED_BLUE,toggle_led[1]);
        //      Timer1_Reset();

    }
}

struct{
    unsigned char display:1;
}Flag;

void communication(){
    if(serialPC.Flag_receive){

        serialPC.Flag_receive=0;

        //      FIFO_Rx_DataGet(&serialPcFIFO,serialPC.Command_Data);
        //      serialPC.Command_Data=
        if(serialPC.Command_Data[0]==PC2MCU_HEADER_)
        {
            if(serialPC.Command_Data[1]&0x80) // special command
            {
                switch(serialPC.Command_Data[1]){
                case  PC_SAM_SP_MODE_1_: // set position of SAM
                    //                  SerialPutStrLn(UART_PC_,"sm1");
                    if(((serialPC.dataIndex-3)%4)==0){ // check length of data
                        unsigned char numOfSam=serialPC.dataIndex/4;
                        unsigned char i;
                        unsigned char refIndex;
                        for(i=0;i<numOfSam;i++)
                        {
                            refIndex=4*i+2;
                            if(((serialPC.Command_Data[refIndex]^serialPC.Command_Data[refIndex+1]^serialPC.Command_Data[refIndex+2])&0x7F)==serialPC.Command_Data[refIndex+3])
                            {
                                sam1.id=serialPC.Command_Data[refIndex]&0x1F;
                                //sam1.mode=serialPC.Command_Data[refIndex]>>5;
                                sam1.SAMmode=((serialPC.Command_Data[refIndex]&0x60)>>3)+((serialPC.Command_Data[refIndex+1]&0x60)>>5);
                                sam1.position12=((serialPC.Command_Data[refIndex+1]&0x1F)<<7)+(serialPC.Command_Data[refIndex+2]&0X7F);
                                if(sam1.SAMmode==1){
                                    SAM_set_jointAngle12bit(sam1.id,sam1.position12);
                                    samEnableRead[sam1.id]=1;;
                                }
                                else if(sam1.SAMmode==2){
                                    SAM_set_passiveMode(sam1.id);
                                    samEnableRead[sam1.id]=0;
                                }

                                //                          display_com();
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
                    //                    SerialPutStrLn(UART_PC_,"sm2");
                    if(serialPC.dataIndex==3){//check length of data
                        while(samReadBusy);
                        samReadBusy=1;
                        toggle_led[1]^=1;
                        led(LED_BLUE,toggle_led[1]);

                        samReadCurrentID_C1=1;
                        if(samEnableRead[samReadCurrentID_C1])
                            SAM_get_jointAngle12bit(samReadCurrentID_C1);
                        samReadCurrentID_C6=0;
                        if(samEnableRead[samReadCurrentID_C6])
                            SAM_get_jointAngle12bit(samReadCurrentID_C6);
                        samReadCurrentID_C7=10;
                        if(samEnableRead[samReadCurrentID_C7])
                            SAM_get_jointAngle12bit(samReadCurrentID_C7);
                        Timer1_Reset();

                        while(samReadBusy);
                        samReadBusy=1;
                        samReadCurrentID_C1=3;
                        if(samEnableRead[samReadCurrentID_C1])
                            SAM_get_jointAngle12bit(samReadCurrentID_C1);
                        samReadCurrentID_C6=2;
                        if(samEnableRead[samReadCurrentID_C6])
                            SAM_get_jointAngle12bit(samReadCurrentID_C6);
                        samReadCurrentID_C7=11;
                        if(samEnableRead[samReadCurrentID_C7])
                            SAM_get_jointAngle12bit(samReadCurrentID_C7);
                        Timer1_Reset();

                        while(samReadBusy);
                        samReadBusy=1;
                        samReadCurrentID_C1=5;
                        if(samEnableRead[samReadCurrentID_C1])
                            SAM_get_jointAngle12bit(samReadCurrentID_C1);
                        samReadCurrentID_C6=4;
                        if(samEnableRead[samReadCurrentID_C6])
                            SAM_get_jointAngle12bit(samReadCurrentID_C6);

                        samReadCurrentID_C7=22;
                        if(samEnableRead[samReadCurrentID_C7])
                            SAM_get_jointAngle12bit(samReadCurrentID_C7);
                        Timer1_Reset();

                        while(samReadBusy);
                        samReadBusy=1;
                        samReadCurrentID_C1=7;
                        if(samEnableRead[samReadCurrentID_C1])
                            SAM_get_jointAngle12bit(samReadCurrentID_C1);
                        samReadCurrentID_C6=6;
                        if(samEnableRead[samReadCurrentID_C6])
                            SAM_get_jointAngle12bit(samReadCurrentID_C6);
                        Timer1_Reset();

                        while(samReadBusy);
                        samReadBusy=1;
                        samReadCurrentID_C1=9;
                        if(samEnableRead[samReadCurrentID_C1])
                            SAM_get_jointAngle12bit(samReadCurrentID_C1);
                        samReadCurrentID_C6=8;
                        if(samEnableRead[samReadCurrentID_C6])
                            SAM_get_jointAngle12bit(samReadCurrentID_C6);
                        Timer1_Reset();


                        while(samReadBusy);
                        unsigned char i;
                        unsigned char refIndex=2;
                        dataSendBuffer[0]=MCU2PC_HEADER_;
                        dataSendBuffer[1]=PC_SAM_SP_MODE_2_;
                        for (i=0;i<NUM_OF_SAM_;i++)
                        {
                            if(samDataAvail[i])
                            {
                                samDataAvail[i]=0;
                                dataSendBuffer[refIndex++]=i&0x1F;
                                dataSendBuffer[refIndex++]=(samPosition12[i]>>7)&0x1F;
                                dataSendBuffer[refIndex++]=(samPosition12[i])&0x7F;
                                dataSendBuffer[refIndex++]=(dataSendBuffer[refIndex-3]^dataSendBuffer[refIndex-2]^dataSendBuffer[refIndex-1])&0x7F;
                            }
                        }
                        dataSendBuffer[refIndex]=MCU2PC_TERMINATOR_;
                        SerialSendData(UART_PC_,dataSendBuffer);
                    }
                    else{
                        SerialPutStrLn(UART_PC_,"e_l_3");
                    }
                    break;
                case PC_SAM_SP_MODE_3_:// set PID of 1 SAM
                    SerialPutStrLn(UART_PC_,"sm3");
                    if(serialPC.dataIndex==9){ //check length of data
                        if(((serialPC.Command_Data[2]^serialPC.Command_Data[3]^serialPC.Command_Data[4]^serialPC.Command_Data[5]^serialPC.Command_Data[6])&0x7F)==serialPC.Command_Data[7]){
                            sam1.id=serialPC.Command_Data[2]&0x1F;
                            sam1.P=serialPC.Command_Data[4]+((serialPC.Command_Data[3]<<5)&0x80);
                            sam1.I=serialPC.Command_Data[5]+((serialPC.Command_Data[3]<<6)&0x80);
                            sam1.D=serialPC.Command_Data[6]+((serialPC.Command_Data[3]<<7)&0x80);

                            while(samReadBusy);
                            samReadBusy=1;
                            SAM_set_PD_Runtime(sam1.id, sam1.P, sam1.D);
                            Timer1_Reset();

                            while(samReadBusy);
                            samReadBusy=1;
                            SAM_set_I_Runtime(sam1.id, sam1.I);
                            Timer1_Reset();
                            while(samReadBusy);

                        }
                        else{
                            SerialPutStrLn(UART_PC_,"e_cs");
                        }
                    }
                    else{
                        SerialPutStrLn(UART_PC_,"e_l");
                    }
                    break;
                case PC_SAM_SP_MODE_4_:// get PID of 1 SAM
                    SerialPutStrLn(UART_PC_,"sm4");
                    if(serialPC.dataIndex==4){ //check length of data
                        unsigned char update_count=0;
                        sam1.id=serialPC.Command_Data[2];
                        while(samReadBusy);
                        samReadBusy=1;
                        SAM_get_PD(sam1.id);
                        Timer1_Reset();

                        while(samReadBusy);
                        if(samDataAvail[sam1.id])
                        {
                            update_count++;
                            samDataAvail[sam1.id]=0;
                        }

                        samReadBusy=1;
                        SAM_get_I(sam1.id);
                        Timer1_Reset();

                        while(samReadBusy);
                        if(samDataAvail[sam1.id])
                        {
                            update_count++;
                            samDataAvail[sam1.id]=0;
                        }

                        if(update_count==2)
                        {
                            sam1.P=samP[sam1.id];
                            sam1.I=samI[sam1.id];
                            sam1.D=samD[sam1.id];
                            SerialSend_PID(UART_PC_,sam1.id,sam1.P, sam1.I,sam1.D);
                        }
                        else{
                            SerialPutStrLn(UART_PC_,"e_u");
                        }



                    }
                    else{
                        SerialPutStrLn(UART_PC_,"e_l");
                    }
                    break;

                case PC_SAM_SP_MODE_5_: // read all pos8
                    SerialPutStrLn(UART_PC_,"sm5");
                    if(serialPC.dataIndex==3){//check length of data
                        while(samReadBusy);
                        samReadBusy=1;
                        toggle_led[1]^=1;
                        led(LED_BLUE,toggle_led[1]);
                        samReadCurrentID_C1=1;
                        SAM_get_jointAngle8bit(samReadCurrentID_C1);
                        samReadCurrentID_C6=0;
                        SAM_get_jointAngle8bit(samReadCurrentID_C6);
                        samReadCurrentID_C7=10;
                        SAM_get_jointAngle8bit(samReadCurrentID_C7);
                        Timer1_Reset();

                        while(samReadBusy);
                        samReadBusy=1;
                        samReadCurrentID_C1=3;
                        SAM_get_jointAngle8bit(samReadCurrentID_C1);
                        samReadCurrentID_C6=2;
                        SAM_get_jointAngle8bit(samReadCurrentID_C6);
                        samReadCurrentID_C7=11;
                        SAM_get_jointAngle8bit(samReadCurrentID_C7);
                        Timer1_Reset();

                        while(samReadBusy);
                        samReadBusy=1;
                        samReadCurrentID_C1=5;
                        SAM_get_jointAngle8bit(samReadCurrentID_C1);
                        samReadCurrentID_C6=4;
                        SAM_get_jointAngle8bit(samReadCurrentID_C6);
                        samReadCurrentID_C7=22;
                        SAM_get_jointAngle8bit(samReadCurrentID_C7);
                        Timer1_Reset();

                        while(samReadBusy);
                        samReadBusy=1;
                        samReadCurrentID_C1=7;
                        SAM_get_jointAngle8bit(samReadCurrentID_C1);
                        samReadCurrentID_C6=6;
                        SAM_get_jointAngle8bit(samReadCurrentID_C6);
                        Timer1_Reset();

                        while(samReadBusy);
                        samReadBusy=1;
                        samReadCurrentID_C1=9;
                        SAM_get_jointAngle8bit(samReadCurrentID_C1);
                        samReadCurrentID_C6=8;
                        SAM_get_jointAngle8bit(samReadCurrentID_C6);
                        Timer1_Reset();


                        while(samReadBusy);
                        unsigned char i;
                        unsigned char refIndex=2;
                        dataSendBuffer[0]=MCU2PC_HEADER_;
                        dataSendBuffer[1]=PC_SAM_SP_MODE_5_;
                        for (i=0;i<NUM_OF_SAM_;i++)
                        {
                            if(samDataAvail[i])
                            {
                                samDataAvail[i]=0;
                                dataSendBuffer[refIndex++]=((samLoad8[i]&0x80)>>1)+((samPosition8[i]&0x80)>>2)+(i&0x1F);
                                dataSendBuffer[refIndex++]=samLoad8[i]&0x7F;
                                dataSendBuffer[refIndex++]=samPosition8[i]&0x7F;
                                dataSendBuffer[refIndex++]=(dataSendBuffer[refIndex-3]^dataSendBuffer[refIndex-2]^dataSendBuffer[refIndex-1])&0x7F;
                            }
                        }
                        dataSendBuffer[refIndex]=MCU2PC_TERMINATOR_;
                        SerialSendData(UART_PC_,dataSendBuffer);
                    }
                    else{
                        SerialPutStrLn(UART_PC_,"e_l_3");
                    }
                    break;
                case PC_SAM_SP_MODE_6_:// set all passive lower link
                    SerialPutStrLn(UART_PC_,"sm6");
                    if(serialPC.dataIndex==3){//check length of data
                        unsigned char i;
                        for(i=0;i<12;i++)
                        {
                            SAM_set_passiveMode(i);
                        }
                    }
                    else{
                        SerialPutStrLn(UART_PC_,"e_l_3");
                    }
                    break;
                case PC_SAM_SP_MODE_7_:// enable/disable power
                    SerialPutStrLn(UART_PC_,"sm7");
                    if(serialPC.dataIndex==4){//check length of data
                        SAM_Power_enable(serialPC.Command_Data[2]);
                    }
                    else{
                        SerialPutStrLn(UART_PC_,"e_l_4");
                    }
                    break;
                case PC_SAM_SP_MODE_8_:// set quick PD
                    SerialPutStrLn(UART_PC_,"sm8");
                    if(serialPC.dataIndex==7){ //check length of data
                        if(((serialPC.Command_Data[2]^serialPC.Command_Data[3]^serialPC.Command_Data[4])&0x7F)==serialPC.Command_Data[5]){
                            sam1.id=serialPC.Command_Data[2]&0x1F;
                            sam1.P=serialPC.Command_Data[3]+((serialPC.Command_Data[2]<<1)&0x80);
                            sam1.D=serialPC.Command_Data[4]+((serialPC.Command_Data[2]<<2)&0x80);
                            SAM_set_PD_RuntimeQuick(sam1.id, sam1.P, sam1.D);
                        }
                        else{
                            SerialPutStrLn(UART_PC_,"e_cs");
                        }
                    }
                    else{
                        SerialPutStrLn(UART_PC_,"e_l");
                    }
                    break;
                case  PC_SAM_SP_MODE_9_:
                    SerialPutStrLn(UART_PC_,"sm9");
                    if(((serialPC.dataIndex-3)%4)==0){ // check length of data
                        unsigned char numOfSam=serialPC.dataIndex/4;
                        unsigned char i;
                        unsigned char refIndex;
                        for(i=0;i<numOfSam;i++)
                        {
                            refIndex=4*i+2;
                            if(((serialPC.Command_Data[refIndex]^serialPC.Command_Data[refIndex+1]^serialPC.Command_Data[refIndex+2])&0x7F)==serialPC.Command_Data[refIndex+3])
                            {
                                sam1.id=serialPC.Command_Data[refIndex]&0x1F;
                                //sam1.mode=serialPC.Command_Data[refIndex]>>5;
                                //sam1.SAMmode=((serialPC.Command_Data[refIndex]&0x60)>>5)+((serialPC.Command_Data[refIndex+1]&0x60)>>3);
                                sam1.averageTorq=((serialPC.Command_Data[refIndex+1]&0x1F)<<7)+(serialPC.Command_Data[refIndex+2]&0X7F);
                                SAM_set_avergTorque(sam1.id,sam1.averageTorq);
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
                case  PC_SAM_SP_MODE_10_:
                    if(serialPC.dataIndex==3){//check length of data
                        while(samReadBusy);
                        samReadBusy=1;
                        samReadCurrentID_C1=1;
                        SAM_get_avergTorque(samReadCurrentID_C1);
                        samReadCurrentID_C6=0;
                        SAM_get_avergTorque(samReadCurrentID_C6);
                        samReadCurrentID_C7=10;
                        SAM_get_avergTorque(samReadCurrentID_C7);
                        Timer1_Reset();

                        while(samReadBusy);
                        samReadBusy=1;
                        samReadCurrentID_C1=3;
                        SAM_get_avergTorque(samReadCurrentID_C1);
                        samReadCurrentID_C6=2;
                        SAM_get_avergTorque(samReadCurrentID_C6);
                        samReadCurrentID_C7=11;
                        SAM_get_avergTorque(samReadCurrentID_C7);
                        Timer1_Reset();

                        while(samReadBusy);
                        samReadBusy=1;
                        samReadCurrentID_C1=5;
                        SAM_get_avergTorque(samReadCurrentID_C1);
                        samReadCurrentID_C6=4;
                        SAM_get_avergTorque(samReadCurrentID_C6);
                        samReadCurrentID_C7=22;
                        SAM_get_avergTorque(samReadCurrentID_C7);
                        Timer1_Reset();

                        while(samReadBusy);
                        samReadBusy=1;
                        samReadCurrentID_C1=7;
                        SAM_get_avergTorque(samReadCurrentID_C1);
                        samReadCurrentID_C6=6;
                        SAM_get_avergTorque(samReadCurrentID_C6);
                        Timer1_Reset();

                        while(samReadBusy);
                        samReadBusy=1;
                        samReadCurrentID_C1=9;
                        SAM_get_avergTorque(samReadCurrentID_C1);
                        samReadCurrentID_C6=8;
                        SAM_get_avergTorque(samReadCurrentID_C6);
                        Timer1_Reset();


                        while(samReadBusy);
                        unsigned char i;
                        unsigned char refIndex=2;
                        dataSendBuffer[0]=MCU2PC_HEADER_;
                        dataSendBuffer[1]=PC_SAM_SP_MODE_10_;
                        for (i=0;i<NUM_OF_SAM_;i++)
                        {
                            if(samDataAvail[i])
                            {
                                samDataAvail[i]=0;
                                dataSendBuffer[refIndex++]=i&0x1F;
                                dataSendBuffer[refIndex++]=(samAverageTorq[i]>>7)&0x1F;
                                dataSendBuffer[refIndex++]=(samAverageTorq[i])&0x7F;
                                dataSendBuffer[refIndex++]=(dataSendBuffer[refIndex-3]^dataSendBuffer[refIndex-2]^dataSendBuffer[refIndex-1])&0x7F;
                            }
                        }
                        dataSendBuffer[refIndex]=MCU2PC_TERMINATOR_;
                        SerialSendData(UART_PC_,dataSendBuffer);
                    }
                    else{
                        SerialPutStrLn(UART_PC_,"e_l_3");
                    }
                    break;
                case  PC_SAM_SP_MODE_11_:
                    SerialPutStrLn(UART_PC_,"sm11");
                    if(((serialPC.dataIndex-3)%4)==0){ // check length of data
                        unsigned char numOfSam=serialPC.dataIndex/4;
                        unsigned char i;
                        unsigned char refIndex;
                        for(i=0;i<numOfSam;i++)
                        {
                            refIndex=4*i+2;
                            if(((serialPC.Command_Data[refIndex]^serialPC.Command_Data[refIndex+1]^serialPC.Command_Data[refIndex+2])&0x7F)==serialPC.Command_Data[refIndex+3])
                            {
                                sam1.id=serialPC.Command_Data[refIndex]&0x1F;
                                sam1.P=serialPC.Command_Data[refIndex+1]+((serialPC.Command_Data[refIndex]<<1)&0x80);
                                sam1.D=serialPC.Command_Data[refIndex+2]+((serialPC.Command_Data[refIndex]<<2)&0x80);
                                SAM_set_PD_RuntimeQuick(sam1.id, sam1.P, sam1.D);
                                samReadBusy=1;
                                Timer1_Reset();
                                while(samReadBusy);


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
                case  PC_SAM_SP_MODE_14_:
                    SerialPutStrLn(UART_PC_,"sm14");
                    if(((serialPC.dataIndex-3)%5)==0){ // check length of data
                        unsigned char numOfSam=serialPC.dataIndex/5;
                        unsigned char i;
                        unsigned char refIndex;
                        for(i=0;i<numOfSam;i++)
                        {
                            refIndex=5*i+2;
                            if(((serialPC.Command_Data[refIndex]^serialPC.Command_Data[refIndex+1]^serialPC.Command_Data[refIndex+2]^serialPC.Command_Data[refIndex+3])&0x7F)==serialPC.Command_Data[refIndex+4])
                            {
                                sam1.id=serialPC.Command_Data[refIndex]&0x1F;
                                sam1.P=serialPC.Command_Data[refIndex+1]+((serialPC.Command_Data[refIndex]<<1)&0x80);
                                sam1.D=serialPC.Command_Data[refIndex+2]+((serialPC.Command_Data[refIndex]<<2)&0x80);
                                sam1.I=serialPC.Command_Data[refIndex+3]&0x7F;
                                SAM_set_PD_RuntimeQuick(sam1.id, sam1.P, sam1.D);
                                samReadBusy=1;
                                Timer1_Reset();
                                while(samReadBusy);
                                SAM_set_I_Runtime(sam1.id, sam1.I);
                                samReadBusy=1;
                                Timer1_Reset();
                                while(samReadBusy);


                            }
                            else
                            {
                                UARTCharPut(UART_PC_,i+48);
                                SerialPutStrLn(UART_PC_,"e_cs_14");
                            }
                        }
                    }
                    else{
                        SerialPutStrLn(UART_PC_,"e_l_14");
                    }
                    break;
                case  PC_SAM_SP_MODE_12_:
                    if(serialPC.dataIndex==3){//check length of data
                        while(samReadBusy);
                        samReadBusy=1;
                        samReadCurrentID_C1=1;
                        SAM_get_PD(samReadCurrentID_C1);
                        samReadCurrentID_C6=0;
                        SAM_get_PD(samReadCurrentID_C6);
                        samReadCurrentID_C7=10;
                        SAM_get_PD(samReadCurrentID_C7);
                        Timer1_Reset();

                        while(samReadBusy);
                        samReadBusy=1;
                        samReadCurrentID_C1=3;
                        SAM_get_PD(samReadCurrentID_C1);
                        samReadCurrentID_C6=2;
                        SAM_get_PD(samReadCurrentID_C6);
                        samReadCurrentID_C7=11;
                        SAM_get_PD(samReadCurrentID_C7);
                        Timer1_Reset();

                        while(samReadBusy);
                        samReadBusy=1;
                        samReadCurrentID_C1=5;
                        SAM_get_PD(samReadCurrentID_C1);
                        samReadCurrentID_C6=4;
                        SAM_get_PD(samReadCurrentID_C6);
                        samReadCurrentID_C7=22;
                        SAM_get_PD(samReadCurrentID_C7);
                        Timer1_Reset();

                        while(samReadBusy);
                        samReadBusy=1;
                        samReadCurrentID_C1=7;
                        SAM_get_PD(samReadCurrentID_C1);
                        samReadCurrentID_C6=6;
                        SAM_get_PD(samReadCurrentID_C6);
                        Timer1_Reset();

                        while(samReadBusy);
                        samReadBusy=1;
                        samReadCurrentID_C1=9;
                        SAM_get_PD(samReadCurrentID_C1);
                        samReadCurrentID_C6=8;
                        SAM_get_PD(samReadCurrentID_C6);
                        Timer1_Reset();


                        while(samReadBusy);
                        unsigned char i;
                        unsigned char refIndex=2;
                        dataSendBuffer[0]=MCU2PC_HEADER_;
                        dataSendBuffer[1]=PC_SAM_SP_MODE_12_;
                        for (i=0;i<NUM_OF_SAM_;i++)
                        {
                            if(samDataAvail[i])
                            {
                                samDataAvail[i]=0;
                                dataSendBuffer[refIndex++]=(i&0x1F)+((samP[i]&0x80)>>1)+((samD[i]&0x80)>>2);
                                dataSendBuffer[refIndex++]=samP[i]&0x7F;
                                dataSendBuffer[refIndex++]=samD[i]&0x7F;
                                dataSendBuffer[refIndex++]=(dataSendBuffer[refIndex-3]^dataSendBuffer[refIndex-2]^dataSendBuffer[refIndex-1])&0x7F;
                            }
                        }
                        dataSendBuffer[refIndex]=MCU2PC_TERMINATOR_;
                        SerialSendData(UART_PC_,dataSendBuffer);
                    }
                    else{
                        SerialPutStrLn(UART_PC_,"e_l_3");
                    }
                    break;
                case PC_SAM_SP_MODE_13_:
                    //                    SerialPutStrLn(UART_PC_,"sm2");
                    if(serialPC.dataIndex==3){//check length of data
                        while(samReadBusy);
                        samReadBusy=1;
                        toggle_led[1]^=1;
                        led(LED_BLUE,toggle_led[1]);
                        samReadCurrentID_C1=1;
                        SAM_get_jointAngle12bit(samReadCurrentID_C1);
                        samReadCurrentID_C6=0;
                        SAM_get_jointAngle12bit(samReadCurrentID_C6);
                        samReadCurrentID_C7=10;
                        SAM_get_jointAngle12bit(samReadCurrentID_C7);
                        samReadCurrentID_C2=13;
                        SAM_get_jointAngle12bit(samReadCurrentID_C2);
                        samReadCurrentID_C4=12;
                        SAM_get_jointAngle12bit(samReadCurrentID_C4);

                        Timer1_Reset();

                        while(samReadBusy);
                        samReadBusy=1;
                        samReadCurrentID_C1=3;
                        SAM_get_jointAngle12bit(samReadCurrentID_C1);
                        samReadCurrentID_C6=2;
                        SAM_get_jointAngle12bit(samReadCurrentID_C6);
                        samReadCurrentID_C7=11;
                        SAM_get_jointAngle12bit(samReadCurrentID_C7);

                        samReadCurrentID_C2=15;
                        SAM_get_jointAngle12bit(samReadCurrentID_C2);
                        samReadCurrentID_C4=14;
                        SAM_get_jointAngle12bit(samReadCurrentID_C4);
                        Timer1_Reset();

                        while(samReadBusy);
                        samReadBusy=1;
                        samReadCurrentID_C1=5;
                        SAM_get_jointAngle12bit(samReadCurrentID_C1);
                        samReadCurrentID_C6=4;
                        SAM_get_jointAngle12bit(samReadCurrentID_C6);

                        samReadCurrentID_C7=22;
                        SAM_get_jointAngle12bit(samReadCurrentID_C7);
                        samReadCurrentID_C2=17;
                        SAM_get_jointAngle12bit(samReadCurrentID_C2);
                        samReadCurrentID_C4=16;
                        SAM_get_jointAngle12bit(samReadCurrentID_C4);
                        Timer1_Reset();

                        while(samReadBusy);
                        samReadBusy=1;
                        samReadCurrentID_C1=7;
                        SAM_get_jointAngle12bit(samReadCurrentID_C1);
                        samReadCurrentID_C6=6;
                        SAM_get_jointAngle12bit(samReadCurrentID_C6);
                        samReadCurrentID_C2=19;
                        SAM_get_jointAngle12bit(samReadCurrentID_C2);
                        samReadCurrentID_C4=18;
                        SAM_get_jointAngle12bit(samReadCurrentID_C4);
                        Timer1_Reset();

                        while(samReadBusy);
                        samReadBusy=1;
                        samReadCurrentID_C1=9;
                        SAM_get_jointAngle12bit(samReadCurrentID_C1);
                        samReadCurrentID_C6=8;
                        SAM_get_jointAngle12bit(samReadCurrentID_C6);
                        samReadCurrentID_C2=24;
                        SAM_get_jointAngle12bit(samReadCurrentID_C2);
                        samReadCurrentID_C4=23;
                        SAM_get_jointAngle12bit(samReadCurrentID_C4);
                        Timer1_Reset();


                        while(samReadBusy);
                        unsigned char i;
                        unsigned char refIndex=2;
                        dataSendBuffer[0]=MCU2PC_HEADER_;
                        dataSendBuffer[1]=PC_SAM_SP_MODE_13_;
                        for (i=0;i<NUM_OF_SAM_;i++)
                        {
                            if(samDataAvail[i])
                            {
                                samDataAvail[i]=0;
                                dataSendBuffer[refIndex++]=i&0x1F;
                                dataSendBuffer[refIndex++]=(samPosition12[i]>>7)&0x1F;
                                dataSendBuffer[refIndex++]=(samPosition12[i])&0x7F;
                                dataSendBuffer[refIndex++]=(dataSendBuffer[refIndex-3]^dataSendBuffer[refIndex-2]^dataSendBuffer[refIndex-1])&0x7F;
                            }
                        }
                        dataSendBuffer[refIndex]=MCU2PC_TERMINATOR_;
                        SerialSendData(UART_PC_,dataSendBuffer);
                    }
                    else{
                        SerialPutStrLn(UART_PC_,"e_l_3");
                    }
                    break;
                default:
                    break;
                }
            }
            else{
                if(serialPC.dataIndex==PC_SAM_MODE_1_DATALENGTH_)
                {
                    unsigned char mode=((serialPC.Command_Data[1]&0x60)>>3)+((serialPC.Command_Data[2]&0x60)>>5);
                    sam1.id=serialPC.Command_Data[1]&0x1F;
                    switch(mode)
                    {
                    case PC_SAM_MODE_0_:
                        SerialPutStrLn(UART_PC_,"M0");
                        if(((serialPC.Command_Data[1]^serialPC.Command_Data[2]^serialPC.Command_Data[3])&0x7F)==serialPC.Command_Data[4])
                        {
                            sam1.mode=PC_SAM_MODE_0_;
                            sam1.position8=((serialPC.Command_Data[2]&0x01)<<7)+(serialPC.Command_Data[3]&0X7F);
                            SAM_set_jointAngle8bit(sam1.id,sam1.mode,sam1.position8);
                        }
                        else
                        {
                            SerialPutStrLn(UART_PC_,"e_cs");
                        }
                        break;
                    case PC_SAM_MODE_1_:
                        SerialPutStrLn(UART_PC_,"M1");
                        if(((serialPC.Command_Data[1]^serialPC.Command_Data[2]^serialPC.Command_Data[3])&0x7F)==serialPC.Command_Data[4])
                        {
                            sam1.mode=PC_SAM_MODE_1_;
                            sam1.position8=((serialPC.Command_Data[2]&0x01)<<7)+(serialPC.Command_Data[3]&0X7F);
                            SAM_set_jointAngle8bit(sam1.id,sam1.mode,sam1.position8);
                        }
                        else
                        {
                            SerialPutStrLn(UART_PC_,"e_cs");
                        }
                        break;
                    case PC_SAM_MODE_2_:
                        SerialPutStrLn(UART_PC_,"M2");
                        if(((serialPC.Command_Data[1]^serialPC.Command_Data[2]^serialPC.Command_Data[3])&0x7F)==serialPC.Command_Data[4])
                        {
                            sam1.mode=PC_SAM_MODE_2_;
                            sam1.position8=((serialPC.Command_Data[2]&0x01)<<7)+(serialPC.Command_Data[3]&0X7F);
                            SAM_set_jointAngle8bit(sam1.id,sam1.mode,sam1.position8);
                        }
                        else
                        {
                            SerialPutStrLn(UART_PC_,"e_cs");
                        }
                        break;
                    case PC_SAM_MODE_3_:
                        SerialPutStrLn(UART_PC_,"M3");
                        if(((serialPC.Command_Data[1]^serialPC.Command_Data[2]^serialPC.Command_Data[3])&0x7F)==serialPC.Command_Data[4])
                        {
                            sam1.mode=PC_SAM_MODE_3_;
                            sam1.position8=((serialPC.Command_Data[2]&0x01)<<7)+(serialPC.Command_Data[3]&0X7F);
                            SAM_set_jointAngle8bit(sam1.id,sam1.mode,sam1.position8);
                        }
                        else
                        {
                            SerialPutStrLn(UART_PC_,"e_cs");
                        }
                        break;
                    case PC_SAM_MODE_4_:
                        SerialPutStrLn(UART_PC_,"M4");
                        if(((serialPC.Command_Data[1]^serialPC.Command_Data[2]^serialPC.Command_Data[3])&0x7F)==serialPC.Command_Data[4])
                        {
                            sam1.mode=PC_SAM_MODE_4_;
                            sam1.position8=((serialPC.Command_Data[2]&0x01)<<7)+(serialPC.Command_Data[3]&0X7F);
                            SAM_set_jointAngle8bit(sam1.id,sam1.mode,sam1.position8);
                        }
                        else
                        {
                            SerialPutStrLn(UART_PC_,"e_cs");
                        }
                        break;

                    case PC_SAM_MODE_5_:
                        //                        SerialPutStrLn(UART_PC_,"M5");
                        while(samReadBusy);
                        samReadBusy=1;
                        samReadCurrentID_C4=sam1.id;
                        SAM_get_jointAngle8bit(samReadCurrentID_C4);
                        Timer1_Reset();

                        while(samReadBusy);

                        if(samDataAvail[samReadCurrentID_C4])
                        {
                            samDataAvail[samReadCurrentID_C4]=0;
                            SerialSend_1_Position8(UART_PC_,sam1.id,samPosition8[sam1.id],samLoad8[sam1.id]);
                        }
                        else
                        {
                            SerialPutStrLn(UART_PC_,"idNA");
                        }

                        break;
                    case PC_SAM_MODE_6_:
                        SerialPutStrLn(UART_PC_,"M6"); // check if it have feedback or not.
                        if(((serialPC.Command_Data[1]^serialPC.Command_Data[2]^serialPC.Command_Data[3])&0x7F)==serialPC.Command_Data[4])
                        {
                            SAM_set_passiveMode(sam1.id);
                        }
                        else
                        {
                            SerialPutStrLn(UART_PC_,"e_cs");
                        }
                        break;
                    case PC_SAM_MODE_7_:
                        //                        SerialPutStrLn(UART_PC_,"M7");
                        while(samReadBusy);
                        samReadBusy=1;
                        samReadCurrentID_C4=sam1.id;
                        SAM_get_jointAngle12bit(samReadCurrentID_C4);
                        Timer1_Reset();

                        while(samReadBusy);
                        if(samDataAvail[samReadCurrentID_C4])
                        {
                            samDataAvail[samReadCurrentID_C4]=0;
                            SerialSend_1_Position12(UART_PC_,samReadCurrentID_C4,samPosition12[sam1.id]);
                        }
                        else
                        {
                            SerialPutStrLn(UART_PC_,"idNA");
                        }
                        break;
                    case PC_SAM_MODE_8_:
                        SerialPutStrLn(UART_PC_,"M8");
                        if(((serialPC.Command_Data[1]^serialPC.Command_Data[2]^serialPC.Command_Data[3])&0x7F)==serialPC.Command_Data[4])
                        {
                            sam1.position12=((serialPC.Command_Data[2]&0x1F)<<7)+(serialPC.Command_Data[3]&0X7F);
                            SAM_set_jointAngle12bit(sam1.id,sam1.position12);
                        }
                        else
                        {
                            SerialPutStrLn(UART_PC_,"e_cs");
                        }
                        break;
                    case PC_SAM_MODE_9_:
                        SerialPutStrLn(UART_PC_,"M9");
                        if(((serialPC.Command_Data[1]^serialPC.Command_Data[2]^serialPC.Command_Data[3])&0x7F)==serialPC.Command_Data[4])
                        {
                            sam1.averageTorq=((serialPC.Command_Data[2]&0x1F)<<7)+(serialPC.Command_Data[3]&0X7F);
                            SAM_set_avergTorque(sam1.id,sam1.averageTorq);
                        }
                        else
                        {
                            SerialPutStrLn(UART_PC_,"e_cs");
                        }
                        break;

                    case PC_SAM_MODE_10_:
                        SerialPutStrLn(UART_PC_,"M10");
                        while(samReadBusy);
                        samReadBusy=1;
                        samReadCurrentID_C4=sam1.id;
                        SAM_get_avergTorque(samReadCurrentID_C4);
                        Timer1_Reset();

                        while(samReadBusy);
                        if(samDataAvail[samReadCurrentID_C4])
                        {
                            samDataAvail[samReadCurrentID_C4]=0;
                            SerialSend_1_AverageTorq(UART_PC_,samReadCurrentID_C4,samAverageTorq[sam1.id]);
                        }
                        else
                        {
                            SerialPutStrLn(UART_PC_,"idNA");
                        }
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

        //      float2str(Sonar_module.attitude,buffer);
        //      SerialPutChar(UART_COM_2_CONTROLLER_ ,CN_1_);
        //      SerialPutStr_NonTer(UART_COM_2_CONTROLLER_,buffer);
        //
        //      float2str(PID_z.set_point,buffer);
        //      SerialPutChar(UART_COM_2_CONTROLLER_ ,CN_2_);
        //      SerialPutStr_NonTer(UART_COM_2_CONTROLLER_,buffer);
        //
        //      float2str(Socket.attitude_hold_throttle,buffer);
        //      SerialPutChar(UART_COM_2_CONTROLLER_ ,CN_3_);
        //      SerialPutStr_NonTer(UART_COM_2_CONTROLLER_,buffer);
        //      //Dte
        //      float2str(PID_z.P_term,buffer);
        //      SerialPutChar(UART_COM_2_CONTROLLER_ ,CN_4_);
        //      SerialPutStr_NonTer(UART_COM_2_CONTROLLER_,buffer);
        //      //
        //      float2str(PID_z.I_term,buffer); //acc_term
        //      SerialPutChar(UART_COM_2_CONTROLLER_ ,CN_5_);
        //      SerialPutStr_NonTer(UART_COM_2_CONTROLLER_,buffer);
        //
        //      float2str(PID_z.D_term,buffer);
        //      SerialPutChar(UART_COM_2_CONTROLLER_ ,CN_6_);
        //      SerialPutStr_NonTer(UART_COM_2_CONTROLLER_,buffer);
        //      //
        //      float2str(PID_z.KP*100.0,buffer);
        //      SerialPutChar(UART_COM_2_CONTROLLER_ ,CN_7_);
        //      SerialPutStr_NonTer(UART_COM_2_CONTROLLER_,buffer);
        //
        //      float2str(PID_z.KI*100.0,buffer);
        //      SerialPutChar(UART_COM_2_CONTROLLER_ ,CN_8_);
        //      SerialPutStr_NonTer(UART_COM_2_CONTROLLER_,buffer);
        //      //
        //      float2str(PID_z.KD*100.0,buffer);
        //      SerialPutChar(UART_COM_2_CONTROLLER_ ,CN_9_);
        //      SerialPutStr_NonTer(UART_COM_2_CONTROLLER_,buffer);

        //      float2str(-12343.23,buffer);
        //      SerialPutChar(UART_COM_2_CONTROLLER_ ,CN_10_);
        //      SerialPutStrLn(UART_COM_2_CONTROLLER_,buffer);
    }
    SerialTerminator(UART_PC_);
}
