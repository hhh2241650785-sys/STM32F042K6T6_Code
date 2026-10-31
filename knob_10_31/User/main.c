#include "stm32f0xx.h"
#include "stm32f0xx_conf.h"
#include "stm32f0xx_rcc.h"
#include "stm32f0xx_gpio.h"

int main(void)
{
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB,ENABLE);
	  
		GPIO_InitTypeDef GPIO_InitStruct = { 0 };
		
		GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_OUT;
		GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7;
		GPIO_Init(GPIOB,&GPIO_InitStruct);
		
		GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
		GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;
		
		GPIO_Init(GPIOB,&GPIO_InitStruct);

		
	  while (1)
    {
        /* Main loop */
			  if(	GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6) == Bit_RESET)
				{
					
				GPIO_WriteBit(GPIOB,GPIO_Pin_7,Bit_SET);
					
				}
				else
				{
				
				GPIO_WriteBit(GPIOB,GPIO_Pin_7,Bit_RESET);
					
				}
    }
}
