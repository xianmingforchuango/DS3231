/*******************************************************************************
* File Name          : i2c_fram.h
* Author             : MCD Application Team
* Version            : V2.0.1
* Date               : 06/13/2008
* Description        : Header for i2c_ee.c module
*******************************************************************************/
/* Define to prevent recursive inclusion ------------------------------------ */
#ifndef __IIC_H
#define __IIC_H
#include "sys.h"
#include <stdbool.h>
/* Includes ------------------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/


/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void IIC_Init(void);
bool IIC_Start(void);
void IIC_Stop(void);
void IIC_Ack(void);
void IIC_NoAck(void);
bool IIC_WaitAck(void);
void IIC_SendByte(u8 SendByte);
u8 IIC_ReceiveByte(void) ;
bool IIC_FRAM_BufferWrite(u8* pBuffer, u16 WriteAddr, u16 NumByteToWrite);
bool IIC_FRAM_BufferRead(u8* pBuffer, u16 ReadAddr, u16 NumByteToRead);

#endif /* __I2C_FRAM_H */
