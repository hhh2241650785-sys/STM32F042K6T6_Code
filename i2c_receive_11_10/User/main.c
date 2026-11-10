#include "stm32f0xx.h"
#include "stm32f0xx_conf.h"
#include "stm32f0xx_rcc.h"
#include "stm32f0xx_gpio.h"


void My_USART2_Init(void);
void My_GPIO_Init(void);
void My_Board_Led_GPIO_Init(void);
void My_I2C_Init(void);

int main(void)
{
    My_GPIO_Init();
		My_Board_Led_GPIO_Init();
	
		GPIO_PinAFConfig(GPIOB,GPIO_PinSource6,GPIO_AF_1);//CLOCK
	  GPIO_PinAFConfig(GPIOB,GPIO_PinSource7,GPIO_AF_1);//DATA
	
		My_I2C_Init();
	
		uint8_t command[] = {0x00, 0x8d, 0x14, 0xaf, 0xa5};
		
		My_I2C_SendBytes(I2C1,0x78,command,5);
		
		uint8_t rcdata;
		
		My_I2C_ReceiveBytes(I2C1,0x78,&rcdata,1);
	
		while (1)
    {
        /* Main loop */
			if((rcdata & (0x01<<6)) == 0)
			{
				GPIO_WriteBit(GPIOB,GPIO_Pin_3,Bit_SET);
			}
			else
			{
				GPIO_WriteBit(GPIOB,GPIO_Pin_3,Bit_RESET);
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

void My_USART2_Init(void)                                          //³õÊ¼»¯USART2
{
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);   //USART2Ê±ÖÓ
	
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
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA,ENABLE);     
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB,ENABLE);
	
		GPIO_InitTypeDef GPIO_InitStruct = { 0 };
		
		GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
		GPIO_InitStruct .GPIO_OType = GPIO_OType_OD;
		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
		GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	
		GPIO_Init(GPIOB,&GPIO_InitStruct);                                       
}

void My_Board_Led_GPIO_Init(void)
{
		GPIO_InitTypeDef GPIO_InitStruct = { 0 };
		
//		GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_1);
//		GPIO_PinAFConfig(GPIOA,GPIO_PinSource15,GPIO_AF_1);
		
		GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
		GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3;
		
		GPIO_Init(GPIOB,&GPIO_InitStruct);
}
