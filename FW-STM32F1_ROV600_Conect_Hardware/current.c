#include "current.h"

float ACS712_getADCvalue(void)
{	 
		return ADC_value[0];
}
float ACS712_mean(void)
{
		float x=0;
	for(int i=1;i<=20;i++)
	{	
			x+=ACS712_getADCvalue();
		delay_us(500);
	}
	return x/20.0;
}
void ACS712_Cab(void)
{
		ADC_CurrentZero = 0;
	for(int b=1;b<=10;b++)
	{
	ADC_CurrentZero+= ACS712_mean();
	}
	ADC_CurrentZero=ADC_CurrentZero/10;
}