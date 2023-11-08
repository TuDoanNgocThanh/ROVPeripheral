#include "variable.h"


float test; // for test
/*DMA ADC*/
 uint16_t ADC_value[4];

float ADC_SCALE = 4095;
float VREF = 3.3;
float sensitivity=0.1;
//float ADC_CurrentZero=3100;  // zero = 2.52V
float ADC_CurrentZero=3008;	
//float Current_Now;

/*I Gripper*/
float IGripper;
int duty1=0,duty2=0,home=0;
float Imax=0.3;

/*Relay*/
int relay_channel=0;

/*LEAK */
float 	Vleak1, Vleak2, Vleak3;
bool	 Warning_Leak1=0,Warning_Leak2=0,Warning_Leak3=0;
int cnt =0;
double rmin1,rmin2,rmin3,rmin4,rmin5,heading1,heading2,heading3,heading4,heading5;
int Flag_Light, Flag_Niskin, Flag_Heading, Flag_Rmin, Flag_Reset;
