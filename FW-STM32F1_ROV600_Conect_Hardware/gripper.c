#include "gripper.h"
#include "current.h"
#include "variable.h"
/*
	Config PWM for Gripper 
	PA2			PA3			PA0
	PWM1		PWM2		I_Gripper
	Timer 2 channel 3 & 4 are using for PWM
*/
void Config_Gripper(void)
{
	/*Enable clock*/	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	/*Config GPIO*/
	GPIO_InitTypeDef GPIO_InitStructure;
	/*GPIO PA2 & PA3*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz ; 
	
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/*GPIO PA0*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	
	GPIO_Init(GPIOA, &GPIO_InitStructure); 
		
	/*Config Timer*/
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

	TIM_TimeBaseStructure.TIM_Prescaler = 48 - 1; // Fclock = 1Mhz , T=1 uS
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_Period = 100- 1; // t = 1*100uS => f=10kHz
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	TIM_Cmd(TIM2, ENABLE);
}
void Config_PWMGripper(int duty1,int duty2)
{
		/*Config Output Capture*/
	TIM_OCInitTypeDef TIM_OCInitStructure;
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	/*PWM1*/
	TIM_OCInitStructure.TIM_Pulse = duty1;
	TIM_OC3Init(TIM2, &TIM_OCInitStructure);
	TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);
	/*PWM2*/
	TIM_OCInitStructure.TIM_Pulse = duty2;
	TIM_OC4Init(TIM2, &TIM_OCInitStructure);
	TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);
}
void Gripper(void)
	{
//		int h=0;

			//IGripper = (float)(ACS712_mean() /*- ADC_CurrentZero*/ )/4095*3.3/0.1;
			IGripper = (float)(ACS712_mean() - ADC_CurrentZero )/ADC_SCALE*VREF/sensitivity;
		if(home==5)
		{
//			h=1;
			Config_PWMGripper(90,0);
			home=3;
		}
		else if(home==6 )
		{ 
//			h=2;
				Config_PWMGripper(0,90);
			home=3;
		}
		 if (home = 3 && (IGripper > Imax || IGripper < -Imax))
		{
			Config_PWMGripper(0,0);
			home=0;
		}
	}