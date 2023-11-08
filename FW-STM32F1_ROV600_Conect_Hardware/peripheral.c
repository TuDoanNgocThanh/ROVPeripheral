#include "peripheral.h"

uint8_t USART2_Pressuse_Rx_Data[10];

void ADC1DMA_Config(void)
{
	ADC_InitTypeDef 			ADC_InitStructure;
	DMA_InitTypeDef       DMA_InitStructure;
	/*Enable Clock DMA1 & ADC1*/
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 , ENABLE);
	
	/*Config DMA1 for ADC 1*/
	DMA_DeInit(DMA1_Channel1);
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(ADC1->DR); // source register
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&ADC_value;  // destination register
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize = 16;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; //16 bit
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord; // 16bit
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;

	DMA_Init(DMA1_Channel1, &DMA_InitStructure);
	/*ADC for Analog Input*/
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfChannel = 4;
	
	/*Enable ADC DMA*/
	ADC_DMACmd(ADC1, ENABLE);	
	
	/*Time sample*/	
	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_28Cycles5); // I GRIPPER
	ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 2, ADC_SampleTime_28Cycles5); // LEAK1
	ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 3, ADC_SampleTime_28Cycles5); // LEAK2
	ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 4, ADC_SampleTime_28Cycles5); // LEAK3
	
	ADC_Init(ADC1, &ADC_InitStructure);
	
	/* Enable DMA1 Channel 1*/
	DMA_Cmd(DMA1_Channel1, ENABLE);
	/* Enable ADC */
	ADC_Cmd(ADC1, ENABLE);

	/* Reset Cablib register  */   
	ADC_ResetCalibration(ADC1);
	/* waiting */
	while(ADC_GetResetCalibrationStatus(ADC1));
	/* Start ADC Calib */
	ADC_StartCalibration(ADC1);	
	/* waiting*/
	while(ADC_GetCalibrationStatus(ADC1));
	/* Start Convert ADC */ 
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}




/*delay ms*/
void delay_ms(uint16_t ms)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

  
	TIM_TimeBaseStructure.TIM_Period = 65535;
	TIM_TimeBaseStructure.TIM_Prescaler = 48000-1;
	TIM_TimeBaseStructure.TIM_ClockDivision =0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseStructure);
	
	TIM_SetCounter(TIM4,0);
	TIM_Cmd(TIM4,ENABLE);
	
	while(TIM_GetCounter(TIM4)<ms);
	TIM_Cmd(TIM4,DISABLE);
}

/*delay us*/
void delay_us(uint16_t us)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
  // tim4 clock 48MHz
	TIM_TimeBaseStructure.TIM_Period = 65535; 
	TIM_TimeBaseStructure.TIM_Prescaler = 48-1; // 1Mhz
	TIM_TimeBaseStructure.TIM_ClockDivision =0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseStructure);
	
	TIM_SetCounter(TIM4,0);
	TIM_Cmd(TIM4,ENABLE);
	
	while(TIM_GetCounter(TIM4)<us);
	TIM_Cmd(TIM4,DISABLE);
}
