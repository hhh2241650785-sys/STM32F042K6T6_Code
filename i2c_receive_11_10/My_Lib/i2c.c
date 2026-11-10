/**
  ******************************************************************************
  * @file    i2c.c
  * @author  铁头山羊
  * @version V 1.0.0
  * @date    2024年9月3日
  * @brief   i2c驱动源文件
  ******************************************************************************
  */
#include "i2c.h"

  //
  // @简介：通过I2C向从机写入多个字节
  //
  // @参数 I2Cx：填写要操作的I2C的名称，可以是I2C1或I2C2
  // @参数 Addr：填写从机的地址，左对齐 - A6 A5 A4 A3 A2 A1 A0 0
  // @参数 pData：要发送的数据（数组）
  // @参数 Size：要发送的数据的数量，以字节为单位
  //
  // @返回值：0 - 发送成功，-1 - 寻址失败，-2 - 数据被拒收
  //
__weak int My_I2C_SendBytes(I2C_TypeDef* I2Cx, uint8_t Addr, const uint8_t* pData, uint16_t Size)
{
	// #1. 检查参数
	if (Size == 0 || Size > 255 || pData == 0)
	{
		return -3;
	}

	// #2. 等待总线空闲
	while (I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY) == SET);

	I2C_ClearFlag(I2Cx, I2C_FLAG_NACKF | I2C_FLAG_STOPF);

	// #3. 配置地址、数据数量并发送起始位
	I2C_TransferHandling(I2Cx, Addr & 0xfe, Size,
		I2C_AutoEnd_Mode,
		I2C_Generate_Start_Write);

	// #4. 发送数据
	for (uint16_t i = 0; i < Size; i++)
	{
		while (1)
		{
			if (I2C_GetFlagStatus(I2Cx, I2C_FLAG_NACKF) == SET)
			{
				while (I2C_GetFlagStatus(I2Cx, I2C_FLAG_STOPF) == RESET);

				I2C_ClearFlag(I2Cx, I2C_FLAG_NACKF | I2C_FLAG_STOPF);

				if (i == 0)
					return -1;
				else
					return -2;
			}

			if (I2C_GetFlagStatus(I2Cx, I2C_FLAG_TXIS) == SET)
			{
				break;
			}
		}

		I2C_SendData(I2Cx, pData[i]);
	}

	// #5. 等待发送完成
	
	while (I2C_GetFlagStatus(I2Cx, I2C_FLAG_STOPF) == RESET);

	if (I2C_GetFlagStatus(I2Cx, I2C_FLAG_NACKF) == SET)
	{
		I2C_ClearFlag(I2Cx, I2C_FLAG_NACKF | I2C_FLAG_STOPF);
		return -2;
	}

	// #6. 清除STOP标志
	I2C_ClearFlag(I2Cx, I2C_FLAG_STOPF);

	return 0;
}


//
// @简介：通过I2C从从机读多个字节
// 
// @参数 I2Cx：填写要操作的I2C的名称
// @参数 Addr：填写从机的地址，左对齐 - A6 A5 A4 A3 A2 A1 A0 0
// @参数 pBuffer：接收缓冲区（数组）
// @参数 Size：要读取的数据的数量，以字节为单位
//
// @返回值：0 - 接收成功， -1 - 寻址失败
//
__weak int My_I2C_ReceiveBytes(I2C_TypeDef *I2Cx, uint8_t Addr, uint8_t *pBuffer, uint16_t Size)
{
    if(Size == 0) return 0;
    if(Size > 255 || pBuffer == 0) return -1;

    // #1. 等待总线空闲
    while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY) == SET);

    // #2. 清除标志位
    I2C_ClearFlag(I2Cx, I2C_FLAG_NACKF | I2C_FLAG_STOPF);

    // #3. 配置从机地址，接收数量，发送起始位
    I2C_TransferHandling(I2Cx, Addr & 0xFE, Size,
                         I2C_AutoEnd_Mode,
												 I2C_Generate_Start_Read);

    // #4. 数据读取
    for(uint16_t i = 0; i < Size; i++)
    {
        // 等待RXNE置位
        while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_RXNE) == RESET)
        {
            if(I2C_GetFlagStatus(I2Cx, I2C_FLAG_NACKF) == SET)
            {
                while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_STOPF) == RESET);
                I2C_ClearFlag(I2Cx, I2C_FLAG_NACKF);
                I2C_ClearFlag(I2Cx, I2C_FLAG_STOPF);
                return -1;
            }
        }

        // 读取数据
        pBuffer[i] = I2C_ReceiveData(I2Cx);
    }

    // #5. 等待停止位
    while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_STOPF) == RESET);

    // #6. 清除停止标志
    I2C_ClearFlag(I2Cx, I2C_FLAG_STOPF);

    return 0;
}
