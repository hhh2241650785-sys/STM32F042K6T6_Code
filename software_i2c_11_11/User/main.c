#include "stm32f0xx.h"
#include "stm32f0xx_conf.h"
#include "stm32f0xx_rcc.h"
#include "stm32f0xx_gpio.h"


void My_SI2C_Init(void);
void SCL_Write(uint8_t level);
void SDA_Write(uint8_t level);
uint8_t SDA_Read(void);
void delay_us(uint32_t us);
void SendStart(void);
void SendStop(void);
uint8_t Send_Byte(uint8_t byte);
uint8_t Receive_Byte(uint8_t ack);
int My_SI2C_SendBytes(uint8_t Addr, const uint8_t *pData, uint16_t Size);
int My_SI2C_ReceiveBytes(uint8_t Addr, uint8_t *pBuffer, uint16_t Size);



int main(void)
{
	My_SI2C_Init();
	
	uint8_t command[] = {0x00, 0x8d, 0x14, 0xaf, 0xa5};
	My_SI2C_SendBytes(0x78,command,5);
	while (1)
    {
        /* Main loop */
    }
}


void My_SI2C_Init(void)
{
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA,ENABLE);     	
		
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };
		
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct .GPIO_OType = GPIO_OType_OD;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_11;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	
	GPIO_Init(GPIOA,&GPIO_InitStruct); 

	GPIO_WriteBit(GPIOA,GPIO_Pin_11 | GPIO_Pin_8,Bit_SET);
}

void SCL_Write(uint8_t level)
{
	if(1 == level)
	{
		GPIO_WriteBit(GPIOA,GPIO_Pin_11,Bit_SET);//PA11
	}
	else
	{
		GPIO_WriteBit(GPIOA,GPIO_Pin_11,Bit_RESET);
	}
}

void SDA_Write(uint8_t level)
{
	if(1 == level)
	{
		GPIO_WriteBit(GPIOA,GPIO_Pin_8,Bit_SET);//PA8
	}
	else
	{
		GPIO_WriteBit(GPIOA,GPIO_Pin_8,Bit_RESET);
	}
}

uint8_t SDA_Read(void)
{
	if( GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_8) == 1)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

void SendStart(void)
{
	SDA_Write(0);
	DelayUs(1);
}

void SendStop(void)
{
	SDA_Write(0);
	SCL_Write(0);
	DelayUs(1);
	SCL_Write(1);
	DelayUs(1);
	SDA_Write(1);
	DelayUs(1);

}

uint8_t Send_Byte(uint8_t byte)
{
	for(int8_t i = 7;i >= 0;i--)
	{
		SCL_Write(0);
		if((byte & (0x01 << i)) != 0)
		{
			SDA_Write(1);
		}
		else
		{
			SDA_Write(0);
		}		
		DelayUs(1);
		SCL_Write(1);
		DelayUs(1);
	}
		SCL_Write(0);
		SDA_Write(1);
		DelayUs(1);
		SCL_Write(1);
		DelayUs(1);
		
		return SDA_Read();
}

uint8_t Receive_Byte(uint8_t ack)
{
	uint8_t byte = 0;
	
	for(int8_t i = 7;i>=0;i--)
	{
		SCL_Write(0);
		SDA_Write(1);
		DelayUs(1);
		SCL_Write(1);
		DelayUs(1);
		
		if(SDA_Read() != 0)
		{
			byte |= (0x01 << i);
		}
	}
		SCL_Write(0);
		SDA_Write(!ack);
		DelayUs(1);
		SCL_Write(1);
		DelayUs(1);
		
		return byte;
}

int My_SI2C_SendBytes(uint8_t Addr, const uint8_t *pData, uint16_t Size)
{
	SendStart();
	if(Send_Byte(Addr &0xfe) != 0)
	{
		SendStop();
		return -1;
	}
	for(uint32_t i = 0;i < Size;i++)
	{
		if(Send_Byte(pData[i]) != 0)
		{
			SendStop();
			return -2;
		}
	}
	SendStop();
	return 0;
}

int My_SI2C_ReceiveBytes(uint8_t Addr, uint8_t *pBuffer, uint16_t Size)
{
	SendStart();
	if(Send_Byte(Addr | 0x01) != 0)
	{
		SendStop();
		return -1;
	}
	for(uint32_t i = 0;i < Size;i++)
	{
		if(i < Size - 1)
		{
			pBuffer[i] = Receive_Byte(1);
		}
		else
		{
			pBuffer[i] = Receive_Byte(0);
		}
	}
	SendStop();
	return 0;
}
