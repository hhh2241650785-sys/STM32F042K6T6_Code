#include "stm32f0xx.h"
#include "stm32f0xx_conf.h"
#include "stm32f0xx_rcc.h"
#include "stm32f0xx_gpio.h"

void delay(volatile uint32_t count)
{
	while(count--)
	{
	}
}
int main(void)
{
  
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB,ENABLE);  //开启时钟
	
	GPIO_InitTypeDef GPIO_InitStruct  = {0};

  GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_3;	
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	
  GPIO_Init(GPIOB,&GPIO_InitStruct);//初始化I0引脚，通用输出推挽模式
	
	
	while (1)
    {
        /* Main loop */
		  GPIO_WriteBit(GPIOB,GPIO_Pin_3,Bit_SET);
      delay(300000);
			GPIO_WriteBit(GPIOB,GPIO_Pin_3,Bit_RESET);
			delay(30000);
			
    }
}
