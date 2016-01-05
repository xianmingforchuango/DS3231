#ifndef __LCD1602_H
#define	__LCD1602_H

#include "stm32f10x.h"
#include <stdio.h>

void Lcd_GPIO_init(void);  //端口初始化
void Lcd_Init( void );     //LCD初始化
void Lcd_En_Toggle(void);  //发使能脉冲
void Lcd_Busy(void);       //判断忙
void Gpio_data(unsigned char x);  //端口写数据
void Lcd_Write_Command(unsigned char x,char Busy);  //写命令
void Lcd_Write_Data(unsigned char x);               //写数据 
void Lcd_SetXY(unsigned char x,unsigned char y);   //位置设定，x表示列，y表示行 
void Lcd_Puts(unsigned char x,unsigned char y, unsigned char *string); //向1602写一个字符串 
unsigned char *tr_2(unsigned int outData_2);	 //把变量转换为2位字符显示
unsigned char *tr_3(unsigned int outData_3);	 //把变量转换为3位字符显示
unsigned char *tr_4(unsigned int outData_4);	 //把变量转换为4位字符显示
unsigned char *tr_5(unsigned int outData_5);	 //把变量转换为5位字符显示
unsigned char *tr_6(unsigned int outData_6);	 //把变量转换为6位字符显示
unsigned char *tr_7(unsigned int outData_7);	 //把变量转换为7位字符显示

#endif /* __LCD1602_H */
