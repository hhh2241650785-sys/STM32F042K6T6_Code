#include "stm32f0xx.h"
#include "stm32f0xx_conf.h"
#include "stm32f0xx_rcc.h"
#include "stm32f0xx_gpio.h"

void My_USART_Init(void);
void My_Button_Init(void);
void My_Board_Led_GPIO_Init(void);
void My_I2C_Init(void);

int main(void)
{
	My_Button_Init();
	My_Board_Led_GPIO_Init();
	uint8_t previous, current = Bit_SET;
	while (1)
	{
			/* Main loop */
		previous = current;
		current = GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_8);
		if(previous != current)
		{
			Delay(10);
			if(current == Bit_SET)
			{
			
//				if(GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_3) == Bit_SET)
//				{
//					GPIO_WriteBit(GPIOB,GPIO_Pin_3,Bit_RESET);
//				}
//				else if(GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_3) == Bit_RESET)
//				{
//					GPIO_WriteBit(GPIOB,GPIO_Pin_3,Bit_SET);
//				}
		
				uint8_t status = !GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_3);
				GPIO_WriteBit(GPIOB,GPIO_Pin_3,(BitAction)status);
			}
		}
	}
}

void My_I2C_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1,ENABLE);
	RCC_I2CCLKConfig(RCC_I2C1CLK_HSI);
	I2C_InitTypeDef I2C_InitStruct = {0};
	
	I2C_InitStruct.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStruct.I2C_Timing = 0x0010020A;
	
	I2C_Init(I2C1,&I2C_InitStruct);
	
	I2C_Cmd(I2C1,ENABLE);
}

void My_USART_Init(void)
{	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);   

	USART_InitTypeDef USART_InitStruct = { 0 };
	
	USART_InitStruct.USART_BaudRate = 115200;
	USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	
	USART_Init(USART2,&USART_InitStruct); 
	
	USART_Cmd(USART2,ENABLE);
}

void My_Button_Init(void)
{
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA,ENABLE);

	GPIO_InitTypeDef GPIO_InitStruct = { 0 };
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_Level_1;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;

	GPIO_Init(GPIOA,&GPIO_InitStruct);                                       
}

void My_Board_Led_GPIO_Init(void)
{
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB,ENABLE);
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3;
	
	GPIO_Init(GPIOB,&GPIO_InitStruct);
}
