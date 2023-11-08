#include "stm32f10x.h"
#include <stdio.h>
#include "string.h"
#include "stdlib.h"
#include "stdbool.h"

extern float test;

/*DMA ADC*/
extern  uint16_t ADC_value[4];
	
/*For ACS712*/
extern float CurrentDC[9];

extern float ADC_SCALE;
extern float VREF;
extern float sensitivity;
extern float ADC_CurrentZero;
extern float ADC_VoltageZero;
extern float Current_Now;

/*Send data*/
extern char send_data[64];
extern char receive_data[10];
extern char *receive_data1;
extern int count,start_text;
/*Select channel*/
extern int8_t channel;


/*I Gripper*/
extern float IGripper;
extern int duty1,duty2,home;
extern float Imax;

/*Relay*/
extern int relay_channel;
extern uint8_t USART1_Pixhawk_Tx_Data[29];

/*LEAK*/
extern float Vleak1, Vleak2, Vleak3;
extern bool	 Warning_Leak1,Warning_Leak2,Warning_Leak3;
extern int cnt;
extern double rmin1,rmin2,rmin3,rmin4,rmin5,heading1,heading2,heading3,heading4,heading5;
extern int Flag_Light, Flag_Niskin, Flag_Heading, Flag_Rmin, Flag_Reset;
