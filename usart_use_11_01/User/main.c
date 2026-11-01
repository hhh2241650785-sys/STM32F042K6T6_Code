#include "stm32f0xx.h"
#include "stm32f0xx_conf.h"
#include "stm32f0xx_rcc.h"
#include "stm32f0xx_gpio.h"
#include <string.h>
#include <stdio.h>

void My_USART_SendBytes(USART_TypeDef* USARTx,uint8_t* pData,uint16_t Size);
void My_USART2_Init(void);
void My_GPIO_Init(void);

int main(void)
{
		My_GPIO_Init();
		
		My_USART2_Init();    
	
//		uint8_t Str[] = "hello world";
//		uint16_t Size = (uint16_t)strlen((char*)Str);
//		My_USART_SendBytes(USART2,Str,Size);
	
		printf("hello world");
		
		while (1)
		{
				/* Main loop */
//			fputc(5);
			for(int i = 0;i<1000000;i++);
			
		}
}

void My_USART_SendBytes(USART_TypeDef* USARTx,uint8_t* pData,uint16_t Size)
{
		for(uint32_t i = 0;i < Size;i++)
	{
			while(USART_GetFlagStatus(USARTx,USART_FLAG_TXE) == RESET);
			USART_SendData(USARTx,pData[i]);	
	}	
		while(USART_GetFlagStatus(USARTx,USART_FLAG_TC) == RESET);
}

void My_USART2_Init(void)                                          //初始化USART2
{
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);   //USART2时钟
	
		USART_InitTypeDef USART_InitStruct = { 0 };
		
		USART_InitStruct.USART_BaudRate = 115200;
		USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
		USART_InitStruct.USART_Parity = USART_Parity_No;
		USART_InitStruct.USART_StopBits = USART_StopBits_1;
		USART_InitStruct.USART_WordLength = USART_WordLength_8b;
		
		USART_Init(USART2,&USART_InitStruct); 
		
		USART_Cmd(USART2,ENABLE);
}

void My_GPIO_Init(void)
{
	
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA,ENABLE);      //GPIOA时钟
		
		//RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG,ENABLE); //SYSCFG时钟
	
		GPIO_InitTypeDef GPIO_InitStruct = { 0 };
		
		GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
		GPIO_InitStruct .GPIO_OType = GPIO_OType_PP;
		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2;
		GPIO_InitStruct.GPIO_Speed = GPIO_Speed_10MHz;
	
		GPIO_Init(GPIOA,&GPIO_InitStruct);                       //初始化PA9,TX
	
		GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_15;
		GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	
		GPIO_Init(GPIOA,&GPIO_InitStruct);                       //初始化PA10,RX
	
		GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_1);
		GPIO_PinAFConfig(GPIOA,GPIO_PinSource15,GPIO_AF_1);
}

int fputc(int str,FILE *f)
{
		while(USART_GetFlagStatus(USART2,USART_FLAG_TXE) == RESET);
		
		USART_SendData(USART2,(uint8_t)str);
	
		return str;
}
