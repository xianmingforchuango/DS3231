#include "DS3231.h"
#include "iic.h" 
#include "delay.h"   
#include "sys.h"  
     
_calendar_obj calendar;

#define DS3231_WriteAddress 0xD0   
#define DS3231_ReadAddress  0xD1

u8 BCD2HEX(u8 val)
{
        u8 L;
L=(val/16)*10+(val%16);
        return L;
}

u16 B_BCD(u8 val)
{
   u8 L;
        L=(val/10)*16|(val%10);    
        return L;	
}


void I2cByteWrite(u8 addr,u8 bytedata)
{
  IIC_Start();
  delay_us(5);
  IIC_SendByte(DS3231_WriteAddress);
  IIC_WaitAck();
  delay_us(5);
  IIC_SendByte(addr);
  IIC_WaitAck();
  delay_us(5);
  IIC_SendByte(bytedata);
  IIC_WaitAck();
  delay_us(5);
  IIC_Stop();
}	


u8 I2cByteRead(u8 addr)
{
  u8 Dat=0;
  
  IIC_Start();
  IIC_SendByte(DS3231_WriteAddress);
  IIC_WaitAck();
  delay_us(5);
  IIC_SendByte(addr);
  IIC_WaitAck();
  delay_us(5);
  IIC_Start();
  IIC_SendByte(DS3231_ReadAddress);
  IIC_WaitAck();
  delay_us(5);
  Dat=IIC_ReceiveByte();
	//IIC_NoAck();
  IIC_Stop();
  return Dat;
} 



void DS3231_Init(void)
{
	IIC_Init();
	 I2cByteWrite(0x0E,0x00);
  delay_ms(2); 
	I2cByteWrite(0x0f,0x00);
	delay_ms(2); 

}
void DS3231_Get(void)
{
  calendar.w_year=I2cByteRead(0x06);  
  calendar.w_month=I2cByteRead(0x05);  
  calendar.w_date=I2cByteRead(0x04); 
  calendar.hour=I2cByteRead(0x02);  
  calendar.min=I2cByteRead(0x01);
  calendar.sec=I2cByteRead(0x00);
}

void DS3231_Set(u8 yea,u8 mon,u8 da,u8 hou,u8 min,u8 sec)
{
  u8 temp=0;
  
  temp=B_BCD(yea);
  I2cByteWrite(0x06,temp);
  
  temp=B_BCD(mon);
  I2cByteWrite(0x05,temp);
   
  temp=B_BCD(da);
  I2cByteWrite(0x04,temp);
  
  temp=B_BCD(hou);
  I2cByteWrite(0x02,temp);
  
  temp=B_BCD(min);
  I2cByteWrite(0x01,temp);
  
  temp=B_BCD(sec);
  I2cByteWrite(0x00,temp);
}


void get_show_time(void)
{


calendar.w_year=I2cByteRead(0x06);  
calendar.w_year=BCD2HEX(calendar.w_year);


calendar.w_month=I2cByteRead(0x05); 
calendar.w_month=BCD2HEX(calendar.w_month);


calendar.w_date=I2cByteRead(0x04);  
calendar.w_date=BCD2HEX(calendar.w_date);
 

calendar.hour=I2cByteRead(0x02); 
calendar.hour&=0x3f;                   
calendar.hour=BCD2HEX(calendar.hour);


calendar.min=I2cByteRead(0x01);
calendar.min=BCD2HEX(calendar.min);
  
  
calendar.sec=I2cByteRead(0x00);
calendar.sec=BCD2HEX(calendar.sec);
}


