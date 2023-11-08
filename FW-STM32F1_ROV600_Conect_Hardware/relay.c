#include "relay.h"
#include "variable.h"
#include "peripheral.h"
/*
	Relay 	1 		2			3			4		OPTO
				PB6			PB7		PB8		PB8 PA15
	kenh		1			4			3 		2
*/
void Config_Relay(void)
{
		/*Enable Clock*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
	/*Disable JTAG*/
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, DISABLE); 

	GPIO_AFIODeInit();
	/*Config GPIO*/	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	/*Config GPIOA*/
	/*Relay OPTO*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
	/*Enable JTAG*/
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE); 
	
	GPIO_SetBits(GPIOB,GPIO_Pin_6);
	GPIO_SetBits(GPIOB,GPIO_Pin_7);
	GPIO_SetBits(GPIOB,GPIO_Pin_8);
	GPIO_SetBits(GPIOB,GPIO_Pin_9);
	GPIO_SetBits(GPIOA,GPIO_Pin_15);
}
void Receive_Relay(void)
{
	char m;
						switch(relay_channel)
						{
							case 1:
											GPIOB->ODR ^= GPIO_Pin_6;
										if (GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6) == 0x00 )
										{
											Flag_Light = 2;
										}
										else
										{
											Flag_Light = 1;
										}

										
//										GPIOB->ODR ^= GPIO_Pin_7;
//										if (GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7) == 0x00 )
//										{
//											m = 'b' ;
//										USART_SendData(USART3,m);}
//										else
//										{
//											m = 't' ;
//											USART_SendData(USART3,m);
//										}
//										test=2;
							break;
							case 2:
										GPIO_ResetBits(GPIOB,GPIO_Pin_8);
										delay_ms(2000);
										GPIO_SetBits(GPIOB,GPIO_Pin_8);
//										m = 'k' ;
//										USART_SendData(USART3,m);
//										

//										GPIO_ResetBits(GPIOB,GPIO_Pin_9);
//										delay_ms(2000);
//										GPIO_SetBits(GPIOB,GPIO_Pin_9);
//										m = 'k' ;
//										USART_SendData(USART3,m);
//										test =4;
								break;
						}
							relay_channel=0; 	
					}