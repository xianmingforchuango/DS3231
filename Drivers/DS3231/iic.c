/*******************************************************************************
* File Name          : i2c_fram.c
* Author             : Appcat
* Version            : V0.0.1
* Date               : 07/11/2009
* Description        : This file provides a set of functions needed to manage the
*                      communication between I2C peripheral and I2C FM24CL16 FRAM.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "iic.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define I2C_Speed              100000
#define I2C1_SLAVE_ADDRESS7    0xD0
#define I2C_PageSize           256

#define SCL_H         GPIOB->BSRR = GPIO_Pin_10
#define SCL_L         GPIOB->BRR  = GPIO_Pin_10 
   
#define SDA_H         GPIOB->BSRR = GPIO_Pin_11
#define SDA_L         GPIOB->BRR  = GPIO_Pin_11

#define SCL_read      GPIOB->IDR  & GPIO_Pin_10
#define SDA_read      GPIOB->IDR  & GPIO_Pin_11
#define FALSE 0
#define TRUE  1

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
vu8 FRAM_ADDRESS;

/* Private function prototypes -----------------------------------------------*/

/**/
void I2C_delay(void)
{        
   u8 i=100; //这里可以优化速度        ，经测试最低到5还能写入
   while(i) 
   { 
     i--; 
   } 
}
//???IIC
void IIC_Init(void)
{					     
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );	
	   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD ;   //????
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB,GPIO_Pin_10|GPIO_Pin_11); 	//PB10,PB11 ???
}

bool IIC_Start(void)
{
        SDA_H;
        SCL_H;
        I2C_delay();
        if(!SDA_read)return FALSE;        //SDA线为低电平则总线忙,退出
        SDA_L;
        I2C_delay();
        if(SDA_read) return FALSE;        //SDA线为高电平则总线出错,退出
        SDA_L;
        I2C_delay();
        return TRUE;
}

void IIC_Stop(void)
{
        SCL_L;
        I2C_delay();
        SDA_L;
        I2C_delay();
        SCL_H;
        I2C_delay();
        SDA_H;
        I2C_delay();
}

void IIC_Ack(void)
{        
        SCL_L;
        I2C_delay();
        SDA_L;
        I2C_delay();
        SCL_H;
        I2C_delay();
        SCL_L;
        I2C_delay();
}

void IIC_NoAck(void)
{        
        SCL_L;
        I2C_delay();
        SDA_H;
        I2C_delay();
        SCL_H;
        I2C_delay();
        SCL_L;
        I2C_delay();
}

bool IIC_WaitAck(void)          //返回为:=1有ACK,=0无ACK
{
        SCL_L;
        I2C_delay();
        SDA_H;                        
        I2C_delay();
        SCL_H;
        I2C_delay();
        if(SDA_read)
        {
      SCL_L;
      return FALSE;
        }
        SCL_L;
        return TRUE;
}

void IIC_SendByte(u8 SendByte) //数据从高位到低位//
{
    u8 i=8;
    while(i--)
    {
        SCL_L;
        I2C_delay();
      if(SendByte&0x80)
        SDA_H;  
      else 
        SDA_L;   
        SendByte<<=1;
        I2C_delay();
                SCL_H;
        I2C_delay();
    }
    SCL_L;
}

u8 IIC_ReceiveByte(void)  //数据从高位到低位//
{ 
    u8 i=8;
    u8 ReceiveByte=0;

    SDA_H;                                
    while(i--)
    {
      ReceiveByte<<=1;      
      SCL_L;
      I2C_delay();
          SCL_H;
      I2C_delay();        
      if(SDA_read)
      {
        ReceiveByte|=0x01;
      }
    }
    SCL_L;
    return ReceiveByte;
}

bool IIC_FRAM_BufferWrite(u8* pBuffer, u16 WriteAddr, u16 NumByteToWrite)
{
        u8 Addr = 0, count = 0;

        Addr = WriteAddr / I2C_PageSize;

        count = WriteAddr % I2C_PageSize;

        Addr = Addr << 1;

        Addr = Addr & 0x0F;  

        FRAM_ADDRESS = I2C1_SLAVE_ADDRESS7 | Addr;

    if (!IIC_Start()) return FALSE;
    IIC_SendByte(FRAM_ADDRESS);//设置器件地址+段地址 
    if (!IIC_WaitAck())
        {
                IIC_Stop(); 
                return FALSE;
        }
    IIC_SendByte(count);   //设置段内地址      
        IIC_WaitAck();        
          
        while(NumByteToWrite--)
        {
          IIC_SendByte(* pBuffer);
          IIC_WaitAck();
      pBuffer++;
        }
        IIC_Stop();
          //注意：因为这里要等待EEPROM写完，可以采用查询或延时方式(10ms)
          //Systick_Delay_1ms(10);
        return TRUE;
}


//读出1串数据         
bool IIC_FRAM_BufferRead(u8* pBuffer, u16 WriteAddr, u16 NumByteToRead)
{                
        u8 Addr = 0, count = 0;

    Addr = WriteAddr / I2C_PageSize;

        count = WriteAddr % I2C_PageSize;

        Addr = Addr << 1;

        Addr = Addr & 0x0F;  

        FRAM_ADDRESS = I2C1_SLAVE_ADDRESS7 | Addr;
        
        if (!IIC_Start()) return FALSE;

    IIC_SendByte(FRAM_ADDRESS);//设置器件地址+段地址 

    if (!IIC_WaitAck()) 
        {
                IIC_Stop(); 
                return FALSE;
        }

    IIC_SendByte(count);   //设置低起始地址      
    IIC_WaitAck();
    IIC_Start();
    IIC_SendByte(FRAM_ADDRESS | 0x01);
    IIC_WaitAck();
    while(NumByteToRead)
    {
      *pBuffer = IIC_ReceiveByte();
      if(NumByteToRead == 1)IIC_NoAck();
      else IIC_Ack(); 
      pBuffer++;
      NumByteToRead--;
    }
    IIC_Stop();
    return TRUE;
}
