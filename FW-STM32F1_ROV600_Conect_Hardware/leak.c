#include "leak.h"
#include "variable.h"
/*

LEAK 						1 		2			3
							PA5		PA4		PA1
							
*/
void Config_leak(void){
	GPIO_InitTypeDef GPIO_InitStructure;

	/*Enable GPIOA */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	/*Config GPIO PA5 & PA4*/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_1	|	GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
}


void Caculate_Leak(void){
	char m;
	Vleak1 = ((float)ADC_value[1])/4095.0*3.3;
	Vleak2 = ((float)ADC_value[2])/4095.0*3.3;
	Vleak3 = ((float)ADC_value[3])/4095.0*3.3;
	
	if( Vleak1 >= 2.7 || Vleak1 < 0.5 )
	{
		Warning_Leak1=1;

	}
	if( Vleak2 >= 2.7 || Vleak2 < 0.5)
	{
		Warning_Leak2=1;

	}
	if( Vleak3 >= 2.7 || Vleak3 < 0.5)
	{
		Warning_Leak3=1;
	}
// cnt++;
//	if(cnt == 50)
//	{
//	if (Warning_Leak1 == 1){
//			m = 'h';
//		USART_SendData(USART3,m);
//	}
//	
//	if (Warning_Leak2 == 1){
//			m = 'h';
//		USART_SendData(USART3,m);
//	}
//	
//	if (Warning_Leak3 == 1){
//			m = 'h';
//		USART_SendData(USART3,m);
//	}
	//cnt = 0;}
}