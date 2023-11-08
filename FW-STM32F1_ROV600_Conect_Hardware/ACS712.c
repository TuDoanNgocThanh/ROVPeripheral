#include "ACS712.h"

/*for test (DONE)*/
void ACS712_setADCzero()
{
	for(channel = 0; channel <16;channel++)
	{
		uint32_t acc=0;
		SetChannel_MUX(channel);
		 delay_us(10);
		for(int i =0; i<10; i++)
		{
			acc+= ADC_value[7];
		}
		ADC_zero[channel]= (float)acc/10;
//		sprintf(send_data,"%d%d%d%d  ADC_zero[%d] %0.1f \n",(channel>>3)&0x01,(channel>>2)&0x01,(channel>>1)&0x01,(channel)&0x01,channel,ADC_zero[channel]);
//		USART1_Send_String(send_data);
	}
}

void ACS712_getADCvalue()
{
	for(channel = 0; channel <16; channel++)
	{
		uint32_t acc=0;
		SetChannel_MUX(channel);
		delay_us(50);
		RawValue[channel] = ADC_value[7];
//	sprintf(send_data,"%d%d%d%d  RAW_Value[%d] %0.1f \n",(channel>>3)&0x01,(channel>>2)&0x01,(channel>>1)&0x01,(channel)&0x01,channel,RawValue[channel]);
//	USART1_Send_String(send_data);
	}
}

void Caculate_ACS712()
{
	/*channel 1 Voltage: Y6 | Current: Y7	*/
	CurrentDC[1] = (ADC_CurrentZero - RawValue[7])/ADC_SCALE*VREF/sensitivity;
	VoltageDC[1] = (RawValue[6]-ADC_VoltageZero)/ADC_SCALE*VREF*(1.5+7.5)/1.5;
//	sprintf(send_data,"Ch1 Amp = %0.2f  Vol = %0.2f \n",CurrentDC[1],VoltageDC[1]);
//	USART1_Send_String(send_data);
	
	/*channel 2 Voltage: Y4 | Current: Y5*/
	CurrentDC[2] = (ADC_CurrentZero - RawValue[5])/ADC_SCALE*VREF/sensitivity;
	VoltageDC[2] = (RawValue[4]-ADC_VoltageZero)/ADC_SCALE*VREF*(1.5+7.5)/1.5;
//	sprintf(send_data,"Ch2 Amp = %0.2f  Vol = %0.2f \n",CurrentDC[2],VoltageDC[2]);
//	USART1_Send_String(send_data);
	
	/*channel 3 Voltage: Y2 | Current: Y3*/
	CurrentDC[3] = (ADC_CurrentZero - RawValue[3])/ADC_SCALE*VREF/sensitivity;
	VoltageDC[3] = (RawValue[2]-ADC_VoltageZero)/ADC_SCALE*VREF*(1.5+7.5)/1.5;
//	sprintf(send_data,"Ch3 Amp = %0.2f  Vol = %0.2f \n",CurrentDC[3],VoltageDC[3]);
//	USART1_Send_String(send_data);
	
	/*channel 4 Voltage: Y0 | Current: Y1*/
	CurrentDC[4] = (ADC_CurrentZero - RawValue[1])/ADC_SCALE*VREF/sensitivity;
	VoltageDC[4] = (RawValue[0]-ADC_VoltageZero)/ADC_SCALE*VREF*(1.5+1.5)/1.5;
//	sprintf(send_data,"Ch4 Amp = %0.2f  Vol = %0.2f \n",CurrentDC[4],VoltageDC[4]);
//	USART1_Send_String(send_data);
	
	/*channel 5 Voltage: Y9 | Current: Y8*/
	CurrentDC[5] = (ADC_CurrentZero - RawValue[8])/ADC_SCALE*VREF/sensitivity;
	
	VoltageDC[5] = (RawValue[9]-ADC_VoltageZero)/ADC_SCALE*VREF*(1.5+7.5)/1.5;
//	sprintf(send_data,"Ch5 Amp = %0.2f  Vol = %0.2f \n",CurrentDC[5],VoltageDC[5]);
//	USART1_Send_String(send_data);
	
	/*channel 6 Voltage: Y11 | Current: Y10*/
	CurrentDC[6] = (ADC_CurrentZero - RawValue[10])/ADC_SCALE*VREF/sensitivity;
	VoltageDC[6] = (RawValue[11]-ADC_VoltageZero)/ADC_SCALE*VREF*(1.5+7.5)/1.5;
//	sprintf(send_data,"Ch6 Amp = %0.2f  Vol = %0.2f \n",CurrentDC[6],VoltageDC[6]);
//	USART1_Send_String(send_data);

/*channel 7 Voltage: Y13 | Current: Y12*/
	CurrentDC[7] = (ADC_CurrentZero - RawValue[12])/ADC_SCALE*VREF/sensitivity;
	VoltageDC[7] = (RawValue[13]-ADC_VoltageZero)/ADC_SCALE*VREF*(1.5+7.5)/1.5;
//	sprintf(send_data,"Ch7 Amp = %0.2f  Vol = %0.2f \n",CurrentDC[7],VoltageDC[7]);
//	USART1_Send_String(send_data);
	
	/*channel 8 Voltage: Y15 | Current: Y14*/
	CurrentDC[8] = (ADC_CurrentZero - RawValue[14])/ADC_SCALE*VREF/sensitivity;
	VoltageDC[8] = (RawValue[15]-ADC_VoltageZero)/ADC_SCALE*VREF*(1.5+7.5)/1.5;
//	sprintf(send_data,"Ch8 Amp = %0.2f  Vol = %0.2f \n",CurrentDC[8],VoltageDC[8]);
//	USART1_Send_String(send_data);
}
