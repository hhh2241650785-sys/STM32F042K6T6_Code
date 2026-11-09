#include "stm32f0xx.h"
#include "stm32f0xx_conf.h"
#include "stm32f0xx_rcc.h"
#include "stm32f0xx_gpio.h"


void My_USART2_Init(void);
void My_GPIO_Init(void);
void My_Board_Led_GPIO_Init(void);
void My_I2C_Init(void);
//int My_I2C_SendBytes(I2C_TypeDef* I2Cx,uint8_t Addr,uint8_t *oData,uint16_t Size);

int main(void)
{
    My_GPIO_Init();
	
		GPIO_PinAFConfig(GPIOB,GPIO_PinSource6,GPIO_AF_1);//CLOCK
	  GPIO_PinAFConfig(GPIOB,GPIO_PinSource7,GPIO_AF_1);//DATA
	
		My_I2C_Init();
	
		uint8_t command[] = {0x00, 0x8d, 0x14, 0xaf, 0xa5};
		
		My_I2C_SendBytes(I2C1,0x78,command,5);
	
		while (1)
    {
        /* Main loop */
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
		
		GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_1);
		GPIO_PinAFConfig(GPIOA,GPIO_PinSource15,GPIO_AF_1);
		
		GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
		GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3;
		
		GPIO_Init(GPIOB,&GPIO_InitStruct);
}

//int My_I2C_SendBytes(I2C_TypeDef* I2Cx,uint8_t Addr,uint8_t *pData,uint16_t Size)
//{
//	while(I2C_GetFlagStatus(I2Cx,I2C_FLAG_BUSY) == SET);//等待标志空闲
//	
//	I2C_TransferHandling(I2Cx,Addr & 0xfe,Size,I2C_AutoEnd_Mode,I2C_Generate_Start_Write);
//	
//	for(uint16_t i = 0;i<Size;i++)
//	{
//		while(1)
//		{
//			if(I2C_GetFlagStatus(I2Cx,I2C_FLAG_NACKF) == SET)
//			{
//				I2C_GenerateSTOP(I2Cx,ENABLE);
//				return -2;
//			}
//			if(I2C_GetFlagStatus(I2Cx,I2C_FLAG_TXIS) == SET)
//				break;
//		}
//		I2C_SendData(I2Cx,pData[i]);
//	}
//	while(1)
//		{
//			if(I2C_GetFlagStatus(I2Cx,I2C_FLAG_NACKF) == SET)
//			{
//				I2C_GenerateSTOP(I2Cx,ENABLE);
//				return -2;
//			}
//			if(I2C_GetFlagStatus(I2Cx, I2C_FLAG_STOPF) == SET)
//        {
//            break;
//        }
//		}
//	I2C_ClearFlag(I2Cx, I2C_FLAG_STOPF);

//  return 0;
//}
