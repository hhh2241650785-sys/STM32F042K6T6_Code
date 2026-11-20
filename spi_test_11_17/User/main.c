#include "stm32f0xx.h"
#include "stm32f0xx_conf.h"
#include "stm32f0xx_rcc.h"
#include "stm32f0xx_gpio.h"

uint8_t buffer[10];
Button_TypeDef button1;

void My_USART_Init(void);
void My_GPIO_Init(void);
void My_Board_Led_GPIO_Init(void);
void My_I2C_Init(void);
void My_SPI_init(void);
void App_SPI_MasterTransmidReceive(SPI_TypeDef *SPIx,const uint8_t* pDataTx,uint8_t *pDataRx,uint16_t Size);
void App_W25Q64_SaveByte(uint8_t Byte);
uint8_t App_W25Q64_LoadByte(void);
void App_Button_Init(void);
void button_clicked_cb(uint8_t clicks);

int main(void)
{
	My_Board_Led_GPIO_Init();
	
	App_Button_Init();
	
	My_SPI_init();
	
	BitAction status = (BitAction)App_W25Q64_LoadByte();
	GPIO_WriteBit(GPIOB,GPIO_Pin_3,status);
	
	while (1)
	{
			/* Main loop */
		My_Button_Proc(&button1);
	}
}

void App_Button_Init(void)
{
	Button_InitTypeDef Button_InitStruct = {0};
	
	Button_InitStruct.button_clicked_cb = button_clicked_cb;
	Button_InitStruct.button_pressed_cb = 0;
	Button_InitStruct.button_long_pressed_cb = 0;
	Button_InitStruct.button_released_cb = 0;
	Button_InitStruct.ClickInterval = 0;
	Button_InitStruct.GPIOx = GPIOB;
	Button_InitStruct.GPIO_Pin = GPIO_Pin_5;
	Button_InitStruct.LongPressTickInterval  = 0;
	Button_InitStruct.LongPressTime = 0;
	
	My_Button_Init(&button1,&Button_InitStruct);
	
}

void button_clicked_cb(uint8_t clicks)
{
	if(clicks == 1)
	{
		if(GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_3) == Bit_SET)
		{
			GPIO_WriteBit(GPIOB,GPIO_Pin_3,Bit_RESET);
			App_W25Q64_SaveByte(Bit_RESET);
		}
		else
		{
			GPIO_WriteBit(GPIOB,GPIO_Pin_3,Bit_SET);
			App_W25Q64_SaveByte(Bit_SET);
		}
	}
	else if(clicks == 2)
	{
		GPIO_WriteBit(GPIOB,GPIO_Pin_3,Bit_RESET);
		App_W25Q64_SaveByte(Bit_RESET);
	}
}

uint8_t App_W25Q64_LoadByte(void)
{
	buffer[0] = 0x03;
	buffer[1] = 0x00;
	buffer[2] = 0x00;
	buffer[3] = 0x00;
	
	GPIO_WriteBit(GPIOA,GPIO_Pin_3,Bit_RESET);
	
	App_SPI_MasterTransmidReceive(SPI1,buffer,buffer,4);
	
	buffer[0] = 0xff;
	
	App_SPI_MasterTransmidReceive(SPI1,buffer,buffer,1);
	
	GPIO_WriteBit(GPIOA,GPIO_Pin_3,Bit_SET);
	
	return buffer[0];
}

void App_W25Q64_SaveByte(uint8_t Byte)
{

	buffer[0] = 0x06;
	
	GPIO_WriteBit(GPIOA,GPIO_Pin_3,Bit_RESET);
	
	App_SPI_MasterTransmidReceive(SPI1,buffer,buffer,1);
	
	GPIO_WriteBit(GPIOA,GPIO_Pin_3,Bit_SET);
	
	buffer[0] = 0x20;
	buffer[1] = 0x00;
	buffer[2] = 0x00;
	buffer[3] = 0x00;
	
	GPIO_WriteBit(GPIOA,GPIO_Pin_3,Bit_RESET);
	
	App_SPI_MasterTransmidReceive(SPI1,buffer,buffer,4);
	
	GPIO_WriteBit(GPIOA,GPIO_Pin_3,Bit_SET);
	
	while(1)
	{
	
	GPIO_WriteBit(GPIOA,GPIO_Pin_3,Bit_RESET);
	
	buffer[0] = 0x05;
		
	App_SPI_MasterTransmidReceive(SPI1,buffer,buffer,1);
	
	buffer[0] = 0xff;
	App_SPI_MasterTransmidReceive(SPI1,buffer,buffer,1);
		
	GPIO_WriteBit(GPIOA,GPIO_Pin_3,Bit_SET);
		
	if((buffer[0]&0x01) == 0)
		break;
	
	}
	
	buffer[0] = 0x06;
	
	GPIO_WriteBit(GPIOA,GPIO_Pin_3,Bit_RESET);
	
	App_SPI_MasterTransmidReceive(SPI1,buffer,buffer,1);
	
	GPIO_WriteBit(GPIOA,GPIO_Pin_3,Bit_SET);
	
	buffer[0] = 0x02;
	buffer[1] = 0x00;
	buffer[2] = 0x00;
	buffer[3] = 0x00;
	buffer[4] = Byte;
	
	GPIO_WriteBit(GPIOA,GPIO_Pin_3,Bit_RESET);
	
	App_SPI_MasterTransmidReceive(SPI1,buffer,buffer,5);
	
	GPIO_WriteBit(GPIOA,GPIO_Pin_3,Bit_SET);
	
		while(1)
	{
	
	GPIO_WriteBit(GPIOA,GPIO_Pin_3,Bit_RESET);
	
	buffer[0] = 0x05;
		
	App_SPI_MasterTransmidReceive(SPI1,buffer,buffer,1);
	
	buffer[0] = 0xff;
	App_SPI_MasterTransmidReceive(SPI1,buffer,buffer,1);
		
	GPIO_WriteBit(GPIOA,GPIO_Pin_3,Bit_SET);
		
	if((buffer[0]&0x01) == 0)
		break;
	
	}
}
	
void App_SPI_MasterTransmidReceive(SPI_TypeDef *SPIx,const uint8_t* pDataTx,uint8_t *pDataRx,uint16_t Size)
{
	SPI_Cmd(SPI1,ENABLE);
	
	SPI_SendData8(SPI1,pDataTx[0]);
	for(uint8_t i = 1;i<Size;i++)
	{
		while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE) == RESET);
		SPI_SendData8(SPI1,pDataTx[i]);
		while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_RXNE) == RESET);
		pDataRx[i-1] = SPI_ReceiveData8(SPI1);
	}	
	while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_RXNE) == RESET);
	pDataRx[Size - 1] = SPI_ReceiveData8(SPI1);
	
	SPI_Cmd(SPI1,DISABLE);
	
}
	
void My_SPI_init(void)
{
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5 |GPIO_Pin_7;//MOSI SCK
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_Level_1;
	
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;//MISO
	
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;//CS
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3;
	
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	GPIO_WriteBit(GPIOA,GPIO_Pin_3,Bit_RESET);
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource5,GPIO_AF_0);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource7,GPIO_AF_0);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource6,GPIO_AF_0);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);
	
	SPI_InitTypeDef SPI_InitStruct = {0};
	SPI_InitStruct.SPI_Mode = SPI_Mode_Master;
	SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64;
	SPI_InitStruct.SPI_NSS = SPI_NSS_Soft;
	
	SPI_Init(SPI1,&SPI_InitStruct);
	
	SPI_RxFIFOThresholdConfig(SPI1, SPI_RxFIFOThreshold_QF);
	
	SPI_NSSInternalSoftwareConfig(SPI1,SPI_NSSInternalSoft_Set);
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
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB,ENABLE);
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3;
	
	GPIO_Init(GPIOB,&GPIO_InitStruct);
}
