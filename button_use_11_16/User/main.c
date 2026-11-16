#include "stm32f0xx.h"
#include "stm32f0xx_conf.h"
#include "stm32f0xx_rcc.h"
#include "stm32f0xx_gpio.h"

Button_TypeDef button;
uint32_t number = 0;

void My_USART_Init(void);
void My_GPIO_Init(void);
void My_Board_Led_GPIO_Init(void);
void My_I2C_Init(void);
void My_TotalButton_Init(void);
void clicked_cb(uint8_t clicks);
void long_pressed_cb(uint8_t ticks);

int main(void)
{
	My_GPIO_Init();
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource15,GPIO_AF_1);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_1);
	My_USART_Init();
	My_TotalButton_Init();

	while (1)
	{
			/* Main loop */
		My_Button_Proc(&button);
	}
}

void My_TotalButton_Init(void)
{
	Button_InitTypeDef Button_InitStruct = {0};
	Button_InitStruct.GPIOx = GPIOA;
	Button_InitStruct.GPIO_Pin = GPIO_Pin_8;
	Button_InitStruct.button_clicked_cb = clicked_cb;
	Button_InitStruct.button_long_pressed_cb = long_pressed_cb;
	Button_InitStruct.button_pressed_cb = 0;
	Button_InitStruct.button_released_cb = 0;
	Button_InitStruct.ClickInterval = 100;
	Button_InitStruct.LongPressTickInterval = 0;
	Button_InitStruct.LongPressTime = 0;
	
	My_Button_Init(&button,&Button_InitStruct);
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

void My_GPIO_Init(void)
{
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA,ENABLE);

	GPIO_InitTypeDef GPIO_InitStruct = { 0 };
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct .GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_15;
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

void clicked_cb(uint8_t clicks)
{
	if(clicks == 1)
	{
		number++;
		My_USART_Printf(USART2,"%d",number);
	}
	else if(clicks == 2)
	{
		number = 0;
		My_USART_Printf(USART2,"%d",number);
	}
}

void long_pressed_cb(uint8_t ticks)
{
	number++;
	My_USART_Printf(USART2,"%d",number);
}
