/**************************************************************************
**文件名称：LCD1602.h  for  STM32
**************************************************************************/
#include "stdint.h"
#ifndef  LCD1602_STM32_H
#define  LCD1602_STM32_H
#define LCD_RS_1 GPIOC->BSRR=GPIO_Pin_7 //PE.0--(LCD)RS
#define LCD_RS_0 GPIOC->BRR =GPIO_Pin_7
#define LCD_EN_1 GPIOC->BSRR=GPIO_Pin_6 //PE.2--(LCD)E
#define LCD_EN_0 GPIOC->BRR =GPIO_Pin_6
#define LCD_CMD GPIOC   
#define LCD_RS_PIN  GPIO_Pin_7
#define LCD_EN_PIN  GPIO_Pin_6
#define LCD_BL_PIN  GPIO_Pin_8
#define GPIO_DATA      GPIOD         //  ???4_???
#define GPIO_DATA_4_PIN   GPIO_Pin_15   //  ???4_???
#define GPIO_DATA_5_PIN   GPIO_Pin_14   //  ???5_???
#define GPIO_DATA_6_PIN   GPIO_Pin_13   //  ???6_???
#define GPIO_DATA_7_PIN   GPIO_Pin_12   //  ???7_???
#ifndef uchar
#define uchar unsigned char
#endif
#ifndef uint 
#define uint unsigned int
#endif

uint8_t LCD_busy(void); //读LCD忙碌状态,直到LCD1602不忙为止
void Wait_Leisure(void);//一直等待到LCD1602空闲
void LCD_WriteInitcmd(uchar cmd);//写入LCD初始化时的命令，不可以检测忙状态
void LCD_Writecmd(uchar cmd);//写指令到LCD1602,指令数据占一个字节
void LCD_Writedata(uchar dat);//写一字节数据到LCD1602
void LCD_pos(uchar pos);//设定显示位置 00h~27h,40h~47h
void LCD_Setpos(uchar row,uchar col);//根据习惯设定显示位置
void LCD_DispChar(char ch);//显示一个字符
void LCD_Setpos_DispChar(uchar row,uchar col,char ch);//在指定位置显示一个字符
void LCD_DispString(char str[]);//使LCD1602显示一个字符串，显示位置需提前设定
void LCD_Setpos_DispString(uchar row,uchar col,char str[]);//使LCD1602从指定位置开始显示一个字符串
void LCD_Dispnum(uint32_t num);//显示一个不超过8位的整数，显示位置需提前设置
void LCD_Setpos_Dispnum(uchar row,uchar col,uint32_t num);//在指定位置显示一个不超过8位的整数
void LCD_DispDecimal(uint32_t num,uchar dot);//显示一个有效位不超过8位的浮点数,显示位置需要提前设定
//在指定位置显示一个有效位不超过8位的浮点数
void LCD_Setpos_DispDecimal(uchar row,uchar col,uint32_t num,uchar dot);
//显示日历，显示日期与时间
void LCD_DispDateTime(uint32_t year,uchar month,uchar day,uchar hour,uchar min,uchar sec);
//显示秒表，显示时，分，秒，10毫秒，精确到10ms
void LCD_DispStopWatch(uchar hour,uchar min,uchar sec,uchar tenms);
//显示频率计，动态自动调整频率显示
void LCD_DispFreq(uint32_t freq);
void LCD_ShiftLeft(void);//屏幕整体左移一格，用于滚动显示
void LCD_ShiftRight(void);//屏幕整体右移一格，用于滚动显示
void LCD_Clear(void);//清屏，清除显示
void LCD_Return(void);//使光标还回原点位置
void LCD_Close(void);//关显示
void LCD_Open(void);//开显示
void LCD_FlickerChar(uchar row,uchar col);//使指定位置字符闪烁，不显示光标
void LCD_CloseFlicker(void);//关闭字符闪烁
void LCD_FlickerScreen(void);//屏幕秒闪烁一次
void Gpio_data(unsigned char x, unsigned char CmdDatOpt);  
/*********************************************************************
*名    称：LCD_Init()
*功    能：初始化LCD1602
*入口参数：无
*出口参数：无
*常用命令：0x38:16*2显示，5*&点阵显示字符，8位数据;指令执行时间40us
*          0x0C:开显示，关光标   40us
*          0x08:关显示，关光标   40us
*          0x0D:字符闪烁，关光标，接着设定位置，闪烁周期0.4ms左右
*          0x0F:字符闪烁，开光标
*          0x06:写完数据自动右移光标，普通情形，从左向右显示  40us
*          0x04:写完数据自动左移光标，可以从右向左显示  40us
*          0x01:清除显示内容，即清屏  1.64ms
*          0x02:使光标还回起始位置   1.64ms
*          0x18:屏幕上所有字符同时左移一格，适合滚动显示 40us
*          0x1C:屏幕上所有字符同时右移一格，适合滚动显示 40us
*********************************************************************/
void LCD_Init(void);

#endif 
