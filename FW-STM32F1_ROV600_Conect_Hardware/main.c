/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include <stdio.h>
#include "string.h"
#include <stdlib.h>

#include "peripheral.h"
#include "variable.h"
#include "gripper.h"
#include "current.h"
#include "relay.h"
#include "leak.h"

/*
delay 					TIM4

UART3 						PB10		//PB11 roi//pix

Gripper PWM 			PA2	 		PA3			||		Igripper
									5				6				||			PA0

Relay							1				 2			3			4		
									PB6			PB7			PB8		PB9
kenh							1					4			3 		2

servo PWM				PA8				PA11
	
uart1		PA9 PA10 // RAS
	
LEAK 						1 		2			3
							PA5		PA4		PA1
	
*/	
/* Private typedef -----------------------------------------------------------*/
void USART3_Config(uint32_t Baudrate);
void USART1_Config(uint32_t Baudrate) ;
void TIM3_Config(void);
#define DMABufferSize1_Rx 27
void USART1_DMA_Config(uint8_t DMA_BufferSize_Rx,uint8_t DMA_BufferSize_Tx);
char USART1_Rx_Data[DMABufferSize1_Rx];
#define DMABufferSize1_Tx 27
char USART1_Tx_Data[DMABufferSize1_Tx];

#define DMABufferSize3_Rx 27
void USART3_DMA_Config(uint8_t DMA_BufferSize_Rx,uint8_t DMA_BufferSize_Tx);
char USART3_Rx_Data[DMABufferSize3_Rx];
#define DMABufferSize3_Tx 27
char USART3_Tx_Data[DMABufferSize3_Tx];
int cnt_leaking = 0;
int cnt_test;


int h=0;
float mean;
float max=0.15;
char m = 7;
int cnt_tx_uart1= 0;
int cnt_tx_uart3= 0;

void DMA1_Channel5_IRQHandler(void);
void		USART3_Request(void);
void		USART1_Request(void);
int	main(void)
{
	Config_Gripper();
	ADC1DMA_Config();
	USART3_Config(9600);
	USART1_Config(9600);
	ACS712_Cab();
	Config_Relay();
	Config_PWMGripper(0,0);	
	USART1_DMA_Config(DMABufferSize1_Rx,DMABufferSize1_Tx);
	USART3_DMA_Config(DMABufferSize3_Rx,DMABufferSize3_Tx);
		Flag_Heading = 0;
		Flag_Rmin = 0;
		Flag_Light = 0;
		Flag_Niskin = 0;
//	Config_PWMGripper(90,90);
//	delay_ms(3000);
//	Config_PWMGripper(0,0);	
//	delay_ms(3000);
//	Config_PWMGripper(50,50);	
	//		TIM2->CCR3 = 50*4799/100.0;
//		TIM2->CCR4 = 50*4799/100.0;
//	Config_PWMGripper(0,90);
//	TIM2->CCR3 = 50*2399/100.0;
	TIM3_Config();
  while (1)
	{	
//		Config_PWMGripper(90,90);

//		mean = test_I;
	}
}

void TIM3_Config(void) //TIM3, 20MS
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure; 
  TIM_TimeBaseStructure.TIM_Prescaler = 48000 -1;// 
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseStructure.TIM_Period = 100-1;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	
	TIM_ClearFlag(TIM3, TIM_FLAG_Update);
	/*Enable Interrupt TIM 3 */
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM3, ENABLE);
	// Interrupt
	NVIC_InitTypeDef NVICStructure;
  NVICStructure.NVIC_IRQChannel = TIM3_IRQn;
  NVICStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVICStructure.NVIC_IRQChannelSubPriority = 0;
  NVICStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVICStructure);
}

//void USART3_IRQHandler()
//{
//	if(USART_GetITStatus(USART3,USART_IT_RXNE)!= RESET)
//	{
//		m=USART_ReceiveData(USART3);
//		USART_SendData(USART3,m);
//		m=m-48;
//	}	
//	USART_ClearITPendingBit(USART3,USART_IT_RXNE);

//}
//void USART1_IRQHandler()
//{
//	if(USART_GetITStatus(USART1,USART_IT_RXNE)!= RESET)
//	{
//		m=USART_ReceiveData(USART1);
//		USART_SendData(USART1,m);
//	}	
//	USART_ClearITPendingBit(USART1,USART_IT_RXNE);

//}

/*Interrupt Timer 3 100ms */
void TIM3_IRQHandler()
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
	{	
		
//		Current_Now=Gripper();
		/*relay*/
		if(Flag_Light == 3)
		{	
			relay_channel = 1 ;
		}
		if(Flag_Niskin == 3)
		{
			relay_channel = 2;
			Flag_Niskin = 1;
		}

		Receive_Relay();
		m='A';
		
		/*Gripper*/
			Gripper();
		
			/*Leak Sensor*/
		Caculate_Leak();
				
		USART3_Request();
		USART1_Request();		

	
////		//test = 10;
////		if (cnt_tx_uart1 == 50){
////		DMA_ClearFlag(DMA1_FLAG_TC4);
////						//DMA_Cmd(DMA1_Channel4, DISABLE);
////						//Frame truyen : [Y][speed][E]

////						USART1_Tx_Data[0] = 'A';
////						//memcpy(USART1_Tx_Data + 1, &test, 4);
////						USART1_Tx_Data[1] = 'B';
////						USART1_Tx_Data[2] = 'C';
////						USART1_Tx_Data[3] = 'D';
////						USART1_Tx_Data[4] = 'E';
////						//USART1_Tx_Data[5] = 'n';
////						DMA1_Channel4->CNDTR = DMABufferSize1_Tx;	// so byte can truyen
////						DMA_Cmd(DMA1_Channel4, ENABLE);		// cho phep truyen
////						
////			
////				while(DMA_GetFlagStatus(DMA1_FLAG_TC4)  == RESET){
////				}
////			DMA_Cmd(DMA1_Channel4, DISABLE);
////	
////		cnt_tx_uart1 = 0;
////		}
////		cnt_tx_uart1++;

	
	}

		TIM_ClearFlag(TIM3, TIM_FLAG_Update);
}

void	USART3_Request(void)
		{
				//USART 3																																25
	//	REQ light					A					L				16# 0000 0000 0000 0000 0000 0001	 OFF								B
			// ---- light
				if (Flag_Light == 1) //off
						{Flag_Light = 0;
										DMA_ClearFlag(DMA1_FLAG_TC2);
														USART3_Tx_Data[0] = 'A';
														USART3_Tx_Data[1] = 'L';
													for(int cntuart = 2; cntuart < 27; cntuart++)
													{
															USART3_Tx_Data[cntuart] = 0;
													}
														USART3_Tx_Data[25] = '1';
														USART3_Tx_Data[26] = 'B';
													
														DMA1_Channel2->CNDTR = DMABufferSize3_Tx;	// so byte can truyen
														DMA_Cmd(DMA1_Channel2, ENABLE);		// cho phep truyen
														
												while(DMA_GetFlagStatus(DMA1_FLAG_TC2)  == RESET){
												}
											DMA_Cmd(DMA1_Channel2, DISABLE);
						}
						
					if (Flag_Light == 2) //on
						{Flag_Light = 0;
										DMA_ClearFlag(DMA1_FLAG_TC2);
														USART3_Tx_Data[0] = 'A';
														USART3_Tx_Data[1] = 'L';
													for(int cntuart = 2; cntuart < 27; cntuart++)
													{
															USART3_Tx_Data[cntuart] = 0;
													}
														USART3_Tx_Data[24] = '2';
														USART3_Tx_Data[26] = 'B';
													
														DMA1_Channel2->CNDTR = DMABufferSize3_Tx;	// so byte can truyen
														DMA_Cmd(DMA1_Channel2, ENABLE);		// cho phep truyen
														
												while(DMA_GetFlagStatus(DMA1_FLAG_TC2)  == RESET){
												}
											DMA_Cmd(DMA1_Channel2, DISABLE);
						}
						
						
					// niskin	-----------
					if (Flag_Niskin == 1)
						{Flag_Niskin = 0;
										DMA_ClearFlag(DMA1_FLAG_TC2);
														USART3_Tx_Data[0] = 'A';
														USART3_Tx_Data[1] = 'N';
													for(int cntuart = 2; cntuart < 27; cntuart++)
													{
															USART3_Tx_Data[cntuart] = 0;
													}
														USART3_Tx_Data[25] = '1';
														USART3_Tx_Data[26] = 'B';
													
														DMA1_Channel2->CNDTR = DMABufferSize3_Tx;	// so byte can truyen
														DMA_Cmd(DMA1_Channel2, ENABLE);		// cho phep truyen
														
												while(DMA_GetFlagStatus(DMA1_FLAG_TC2)  == RESET){
												}
											DMA_Cmd(DMA1_Channel2, DISABLE);
						}
						// leaking	-----------
						cnt_leaking++;
						if(cnt_leaking == 30){
					if (Warning_Leak1 == 1 || Warning_Leak2 == 1 || Warning_Leak3 == 1)
						{
										DMA_ClearFlag(DMA1_FLAG_TC2);
														USART3_Tx_Data[0] = 'A';
														USART3_Tx_Data[1] = 'R';
													for(int cntuart = 2; cntuart < 27; cntuart++)
													{
															USART3_Tx_Data[cntuart] = 0;
													}
														USART3_Tx_Data[25] = '1';
														USART3_Tx_Data[26] = 'B';
													
														DMA1_Channel2->CNDTR = DMABufferSize3_Tx;	// so byte can truyen
														DMA_Cmd(DMA1_Channel2, ENABLE);		// cho phep truyen
														
												while(DMA_GetFlagStatus(DMA1_FLAG_TC2)  == RESET){
												}
											DMA_Cmd(DMA1_Channel2, DISABLE);
						}
						cnt_leaking = 0;
						
						}
						
																			// reset	-----------
					if (Flag_Reset == 1)
						{Flag_Reset = 0;
										DMA_ClearFlag(DMA1_FLAG_TC2);

													USART3_Tx_Data[0] = 'A';
														USART3_Tx_Data[1] = 'R';
													for(int cntuart = 2; cntuart < 27; cntuart++)
													{
															USART3_Tx_Data[cntuart] = 0;
													}
														USART3_Tx_Data[24] = '2';
														USART3_Tx_Data[26] = 'B';
														DMA1_Channel2->CNDTR = DMABufferSize3_Tx;	// so byte can truyen
														DMA_Cmd(DMA1_Channel2, ENABLE);		// cho phep truyen
														
												while(DMA_GetFlagStatus(DMA1_FLAG_TC2)  == RESET){
												}
											DMA_Cmd(DMA1_Channel2, DISABLE);
						}
						
						
													// rmin	-----------
					if (Flag_Rmin == 1)
						{Flag_Rmin = 0;
										DMA_ClearFlag(DMA1_FLAG_TC2);
														USART3_Tx_Data[0] = 'A';
														USART3_Tx_Data[1] = 'T';
										memcpy(USART3_Tx_Data + 2 ,&rmin1 ,  8);
										memcpy(USART3_Tx_Data + 10,&rmin2 ,  8);
										memcpy(USART3_Tx_Data + 18,&rmin3 , 8);
														USART3_Tx_Data[26] = 'B';
														DMA1_Channel2->CNDTR = DMABufferSize3_Tx;	// so byte can truyen
														DMA_Cmd(DMA1_Channel2, ENABLE);		// cho phep truyen
														
												while(DMA_GetFlagStatus(DMA1_FLAG_TC2)  == RESET){
												}
											DMA_Cmd(DMA1_Channel2, DISABLE);
						}
						
					if (Flag_Rmin == 2)
						{Flag_Rmin = 0;
										DMA_ClearFlag(DMA1_FLAG_TC2);
														USART3_Tx_Data[0] = 'A';
														USART3_Tx_Data[1] = 'P';
										memcpy(USART3_Tx_Data + 2	,&rmin3 ,  8);
										memcpy(USART3_Tx_Data + 10,&rmin4 , 8);
										memcpy(USART3_Tx_Data + 18,&rmin5 ,  8);
														USART3_Tx_Data[26] = 'B';
													
														DMA1_Channel2->CNDTR = DMABufferSize3_Tx;	// so byte can truyen
														DMA_Cmd(DMA1_Channel2, ENABLE);		// cho phep truyen
														
												while(DMA_GetFlagStatus(DMA1_FLAG_TC2)  == RESET){
												}
											DMA_Cmd(DMA1_Channel2, DISABLE);
						}
						
						
						


		}
		
		void	USART1_Request(void)
{
													// heading	-----------
					if (Flag_Heading == 1)
						{Flag_Heading = 0;
										DMA_ClearFlag(DMA1_FLAG_TC4);
														USART1_Tx_Data[0] = 'A';
														USART1_Tx_Data[1] = 'T';
										memcpy(USART1_Tx_Data + 2, &heading1, 8);
										memcpy(USART1_Tx_Data + 10, &heading2, 8);
										memcpy(USART1_Tx_Data + 18, &heading3, 8);
														USART1_Tx_Data[26] = 'B';
														DMA1_Channel4->CNDTR = DMABufferSize3_Tx;	// so byte can truyen
														DMA_Cmd(DMA1_Channel4, ENABLE);		// cho phep truyen
														
												while(DMA_GetFlagStatus(DMA1_FLAG_TC4)  == RESET){
												}
											DMA_Cmd(DMA1_Channel4, DISABLE);
						}
						
					if (Flag_Heading == 2)
						{Flag_Heading = 0;
										DMA_ClearFlag(DMA1_FLAG_TC4);
														USART1_Tx_Data[0] = 'A';
														USART1_Tx_Data[1] = 'P';
										memcpy(USART1_Tx_Data + 2, &heading3, 8);
										memcpy(USART1_Tx_Data + 10, &heading4, 8);
										memcpy(USART1_Tx_Data + 18, &heading5, 8);
														USART1_Tx_Data[26] = 'B';
													
														DMA1_Channel4->CNDTR = DMABufferSize3_Tx;	// so byte can truyen
														DMA_Cmd(DMA1_Channel4, ENABLE);		// cho phep truyen
														
												while(DMA_GetFlagStatus(DMA1_FLAG_TC4)  == RESET){
												}
											DMA_Cmd(DMA1_Channel4, DISABLE);
						}
}
void USART3_Config(uint32_t Baudrate)
{
	GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;
	/*Enable Clock*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA| RCC_APB2Periph_AFIO |RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	/* Configure PB10 and PB11 as USART3 TX/RX */
    
  /* PB10 = alternate function push/pull output */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
    
  /* PB11 = floating input */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
    
  /* Configure and initialize usart... */
  USART_InitStructure.USART_BaudRate = Baudrate;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	/*Enable Interrupt Rx USART3*/
	USART_ITConfig(USART3,USART_IT_RXNE,ENABLE);
	
	 USART_Init(USART3, &USART_InitStructure);
	/* Enable USART3 */
  USART_Cmd(USART3, ENABLE);
	
	
//	/*Interrupt Config*/
//	NVIC_InitTypeDef NVICStructure;
//		
//	NVICStructure.NVIC_IRQChannel = USART3_IRQn;
//	NVICStructure.NVIC_IRQChannelPreemptionPriority = 2;
//	NVICStructure.NVIC_IRQChannelSubPriority = 2;
//	NVICStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&NVICStructure);
}

void USART1_Config(uint32_t Baudrate) 
{
	GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;
	/*Enable Clock*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA| RCC_APB2Periph_AFIO, ENABLE);
	/* Configure PA9 and PA10 as USART1 TX/RX */
    
  /* PA9 = alternate function push/pull output */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
    
  /* PA10 = floating input */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
    
  /* Configure and initialize usart... */
  USART_InitStructure.USART_BaudRate = Baudrate;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	
//	/*Enable DMA Tx USART1 */
//	USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);
	/*Enable Interrupt Rx USART1*/
	//USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
	
	  USART_Init(USART1, &USART_InitStructure);
	/* Enable USART1 */
  USART_Cmd(USART1, ENABLE);
		/*Enable interrupt DMA Rx */
	USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);
	
//	/*Interrupt Config*/
//	NVIC_InitTypeDef NVICStructure;
//		
//	NVICStructure.NVIC_IRQChannel = USART1_IRQn;
//	NVICStructure.NVIC_IRQChannelPreemptionPriority = 2;
//	NVICStructure.NVIC_IRQChannelSubPriority = 1;
//	NVICStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&NVICStructure);
}
/*Config interrupt DMA Tx - Rx USART1*/
void USART1_DMA_Config(uint8_t DMA_BufferSize_Rx, uint8_t DMA_BufferSize_Tx) // 
{
	DMA_InitTypeDef DMA_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);
		/*Config DMA1 for USART1 Rx*/
	DMA_DeInit(DMA1_Channel5);
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(USART1->DR); // source register
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)USART1_Rx_Data;  // destination register
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize =DMA_BufferSize_Rx ;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; //8 bit
	DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_Byte; // 8bit
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
		
	DMA_ITConfig(DMA1_Channel5, DMA_IT_TC, ENABLE);
	
	
		
	DMA_Init(DMA1_Channel5, &DMA_InitStructure);	
	/* Enable DMA1 Channel 5*/
	DMA_Cmd(DMA1_Channel5, ENABLE);
	
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel5_IRQn; //USART 1 Rx - DMA1 Channel 5
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	

	
	//------------------------------------------------------
	USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);
		/*Config DMA1 for USART1 Tx*/
	DMA_DeInit(DMA1_Channel4);
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(USART1->DR); 
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)USART1_Tx_Data;  
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
	DMA_InitStructure.DMA_BufferSize =DMA_BufferSize_Tx ;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; //8 bit
	DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_Byte; // 8bit
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
		
	DMA_ITConfig(DMA1_Channel4, DMA_IT_TC, ENABLE);
	DMA_Init(DMA1_Channel4, &DMA_InitStructure);	
	
	
	/* Enable DMA1 Channel 4*/
	//DMA_Cmd(DMA1_Channel4, ENABLE);
		

}

/*Config interrupt DMA Tx - Rx USART3*/
void USART3_DMA_Config(uint8_t DMA_BufferSize_Rx, uint8_t DMA_BufferSize_Tx) // 
{
	DMA_InitTypeDef DMA_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	USART_DMACmd(USART3, USART_DMAReq_Rx, ENABLE);
		/*Config DMA1 for USART3 Rx*/
	DMA_DeInit(DMA1_Channel3);
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(USART3->DR); // source register
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)USART3_Rx_Data;  // destination register
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize =DMA_BufferSize_Rx ;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; //8 bit
	DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_Byte; // 8bit
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
		
	DMA_ITConfig(DMA1_Channel3, DMA_IT_TC, ENABLE);
	
	
		
	DMA_Init(DMA1_Channel3, &DMA_InitStructure);	
	/* Enable DMA1 Channel 3*/
	DMA_Cmd(DMA1_Channel3, ENABLE);
	
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel3_IRQn; //USART 3 Rx - DMA1 Channel 3
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	

	
	//------------------------------------------------------
	USART_DMACmd(USART3, USART_DMAReq_Tx, ENABLE);
		/*Config DMA1 for USART3 Tx*/
	DMA_DeInit(DMA1_Channel2);
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(USART3->DR); 
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)USART3_Tx_Data;  
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
	DMA_InitStructure.DMA_BufferSize =DMA_BufferSize_Tx ;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; //8 bit
	DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_Byte; // 8bit
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
		
	DMA_ITConfig(DMA1_Channel2, DMA_IT_TC, ENABLE);
	DMA_Init(DMA1_Channel2, &DMA_InitStructure);	
	
	
	/* Enable DMA1 Channel 2*/
	//DMA_Cmd(DMA1_Channel2, ENABLE);
		
}
/* 
								FRAME LEN USART1 tu RASPERRY xuong STM32F1
								[Start] [FUNCTION] 							[DATA]x24 															[Stop]  ==> 27 bytes

receive Rmin			A					T				byte Rmin -10 / Rmin -5 / Rmin 0    										B
									A					P				byte Rmin 0 / Rmin 5 / Rmin 10													B

	-------------------------------------------------------------------------------------------------------------

										1 T  , 2 P
								FRAME LEN USART3 tu STM32F1 len RASPERRY
								[Start] [FUNCTION] 							[DATA]x24 					25									[Stop]  ==> 27

Send heading			A					T				byte Heading -10 / Heading -5 / Heading 0    					B
									A					P				byte Heading 0 / Heading 5 / Heading 10								B
									

*/
void DMA1_Channel5_IRQHandler(void)
{
	if(DMA_GetITStatus(DMA1_IT_TC5))
	{	test++;
	DMA_ClearITPendingBit(DMA1_IT_TC5);
	if(USART1_Rx_Data[0] == 'A' & USART1_Rx_Data[26] == 'B'){
		if(USART1_Rx_Data[1] == 'T'){
			Flag_Rmin = 1;
	memcpy(&rmin1, USART1_Rx_Data + 2,  8);
	memcpy(&rmin2, USART1_Rx_Data + 10,  8);
	memcpy(&rmin3, USART1_Rx_Data + 18,  8);}
		else if(USART1_Rx_Data[1] == 'P'){
			Flag_Rmin = 2;
	memcpy(&rmin3, USART1_Rx_Data + 2,  8);
	memcpy(&rmin4, USART1_Rx_Data + 10,  8);
	memcpy(&rmin5, USART1_Rx_Data + 18, 8);}
	}
	DMA_Cmd(DMA1_Channel5, ENABLE);
	}
	USART1_DMA_Config(DMABufferSize1_Rx,DMABufferSize1_Tx);
	  
}

/* 
								FRAME LEN USART3 tu STM32F1 len pixhawk
								[Start] [FUNCTION] 							[DATA]x24 															[Stop]  ==> 27 bytes
ACK light					A					L				byte 0000 0000 0000 0000 0000 0001	 OFF								B
																		byte 0000 0000 0000 0000 0000 0020	 ON									B

ACK Niskin				A					N				byte 0000 0000 0000 0000 0000 0001	 successfull				B


Send Rmin					A					T				byte Rmin -10 / Rmin -5 / Rmin 0    										B
									A					P				byte Rmin 0 / Rmin 5 / Rmin 10													B
leaking 					A					R				byte 0000 0000 0000 0000 0000 0001	leaking							B

reset Success			A					R				byte 0000 0000 0000 0000 0000 0020	reset								B
	-------------------------------------------------------------------------------------------------------------
Flag 0 don't send, 1 off , 2 on
										1 T  , 2 P
								FRAME LEN USART3 tu Pixhawk xuong STM32F1
								[Start] [FUNCTION] 							[DATA]x24 					25									[Stop]  ==> 27
REQ light					A					L				byte 0000 0000 0000 0000 0000 0001	 toggle							B


REQ Niskin				A					N				byte 0000 0000 0000 0000 0000 0001	 kick								B


Send heading			A					T				byte Heading -10 / Heading -5 / Heading 0    						B
									A					P				byte Heading 0 / Heading 5 / Heading 10									B
									
reset leak   			A					R				byte 0000 0000 0000 0000 0000 0000	reset								B
*/
void DMA1_Channel3_IRQHandler(void)
{
	if(DMA_GetITStatus(DMA1_IT_TC3))
	{	test++;
	DMA_ClearITPendingBit(DMA1_IT_TC3);
	if(USART3_Rx_Data[0] == 'A' && USART3_Rx_Data[26] == 'B'){
		if (USART3_Rx_Data[1] == 'L')
		{
				if(USART3_Rx_Data[25] == '1')
					{Flag_Light = 3;
					}
		}
		
		if (USART3_Rx_Data[1] == 'N')
		{
				if(USART3_Rx_Data[25] == '1')
					{Flag_Niskin = 3;}
		}
		if (USART3_Rx_Data[1] == 'R')
		{
			Flag_Reset = 1;
			Warning_Leak1 = 0;
			Warning_Leak2 = 0;
			Warning_Leak3 = 0;
			for( int cnt_reset=0; cnt_reset < 30; cnt_reset++){
				USART3_Rx_Data[cnt_reset] = 0;
				USART1_Rx_Data[cnt_reset] = 0;
				USART3_Tx_Data[cnt_reset] = 0;
				USART1_Tx_Data[cnt_reset] = 0;
			}
			
		}
		
		if (USART3_Rx_Data[1] == 'T')
					{Flag_Heading = 1;
						memcpy(&heading1,USART3_Rx_Data + 2,  8);
						memcpy(&heading2,USART3_Rx_Data + 10, 8);
						memcpy(&heading3,USART3_Rx_Data + 18, 8);
					}
		else if(USART3_Rx_Data[1] == 'P')
					{Flag_Heading = 2;
						memcpy(&heading3, USART3_Rx_Data + 2, 8);
						memcpy(&heading4, USART3_Rx_Data + 10, 8);
						memcpy(&heading5,USART3_Rx_Data + 18, 8);
						//memcpy(&heading1,USART3_Rx_Data + 2,  4);
						//memcpy(&rmin1,USART3_Rx_Data + 6, 4);
						//memcpy(&heading2,USART3_Rx_Data + 10, 4);
						//memcpy(&rmin2,USART3_Rx_Data + 14, 4);
						//memcpy(&heading3,USART3_Rx_Data + 18, 4);
						//memcpy(&rmin3,USART3_Rx_Data + 22, 4);
					}
		
	}
	DMA_Cmd(DMA1_Channel3, ENABLE);
	}
	USART1_DMA_Config(DMABufferSize3_Rx,DMABufferSize3_Tx);
}