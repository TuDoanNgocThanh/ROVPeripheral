#include "stm32f10x.h"
#include <stdio.h>
#include "string.h"
#include "stdlib.h"
#include "stm32f10x_conf.h"
#include "variable.h"

void USART1_Pixhawk_Config(uint32_t Baudrate);
void USART2_Pressure_Config(uint32_t Baudrate);
void USART1_DMA_Rx_Config(uint8_t DMABufferSize);
void USART2_DMA_Rx_Config(uint8_t DMABufferSize);
void USART1_Send_String(volatile char *String);
void InterruptTimer_Config();
void ADC1DMA_Config(void);

void delay_ms(uint16_t ms);
void delay_us(uint16_t us);
void user_USART_SendFrame(USART_TypeDef* USARTx,char Frame[], uint16_t Length_Frame);
void user_USART_SendByte(USART_TypeDef* USARTx,uint16_t ch);
void user_USART_Send_String(USART_TypeDef *USARTx,volatile char *String);

void USART1_DMA_Tx_Config(uint8_t DMABufferSize);
extern uint8_t USART2_Pressuse_Rx_Data[10];