/**************************************************************************
**�ļ����ƣ�LCD1602.h  for  STM32
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

uint8_t LCD_busy(void); //��LCDæµ״̬,ֱ��LCD1602��æΪֹ
void Wait_Leisure(void);//һֱ�ȴ���LCD1602����
void LCD_WriteInitcmd(uchar cmd);//д��LCD��ʼ��ʱ����������Լ��æ״̬
void LCD_Writecmd(uchar cmd);//дָ�LCD1602,ָ������ռһ���ֽ�
void LCD_Writedata(uchar dat);//дһ�ֽ����ݵ�LCD1602
void LCD_pos(uchar pos);//�趨��ʾλ�� 00h~27h,40h~47h
void LCD_Setpos(uchar row,uchar col);//����ϰ���趨��ʾλ��
void LCD_DispChar(char ch);//��ʾһ���ַ�
void LCD_Setpos_DispChar(uchar row,uchar col,char ch);//��ָ��λ����ʾһ���ַ�
void LCD_DispString(char str[]);//ʹLCD1602��ʾһ���ַ�������ʾλ������ǰ�趨
void LCD_Setpos_DispString(uchar row,uchar col,char str[]);//ʹLCD1602��ָ��λ�ÿ�ʼ��ʾһ���ַ���
void LCD_Dispnum(uint32_t num);//��ʾһ��������8λ����������ʾλ������ǰ����
void LCD_Setpos_Dispnum(uchar row,uchar col,uint32_t num);//��ָ��λ����ʾһ��������8λ������
void LCD_DispDecimal(uint32_t num,uchar dot);//��ʾһ����Чλ������8λ�ĸ�����,��ʾλ����Ҫ��ǰ�趨
//��ָ��λ����ʾһ����Чλ������8λ�ĸ�����
void LCD_Setpos_DispDecimal(uchar row,uchar col,uint32_t num,uchar dot);
//��ʾ��������ʾ������ʱ��
void LCD_DispDateTime(uint32_t year,uchar month,uchar day,uchar hour,uchar min,uchar sec);
//��ʾ�����ʾʱ���֣��룬10���룬��ȷ��10ms
void LCD_DispStopWatch(uchar hour,uchar min,uchar sec,uchar tenms);
//��ʾƵ�ʼƣ���̬�Զ�����Ƶ����ʾ
void LCD_DispFreq(uint32_t freq);
void LCD_ShiftLeft(void);//��Ļ��������һ�����ڹ�����ʾ
void LCD_ShiftRight(void);//��Ļ��������һ�����ڹ�����ʾ
void LCD_Clear(void);//�����������ʾ
void LCD_Return(void);//ʹ��껹��ԭ��λ��
void LCD_Close(void);//����ʾ
void LCD_Open(void);//����ʾ
void LCD_FlickerChar(uchar row,uchar col);//ʹָ��λ���ַ���˸������ʾ���
void LCD_CloseFlicker(void);//�ر��ַ���˸
void LCD_FlickerScreen(void);//��Ļ����˸һ��
void Gpio_data(unsigned char x, unsigned char CmdDatOpt);  
/*********************************************************************
*��    �ƣ�LCD_Init()
*��    �ܣ���ʼ��LCD1602
*��ڲ�������
*���ڲ�������
*�������0x38:16*2��ʾ��5*&������ʾ�ַ���8λ����;ָ��ִ��ʱ��40us
*          0x0C:����ʾ���ع��   40us
*          0x08:����ʾ���ع��   40us
*          0x0D:�ַ���˸���ع�꣬�����趨λ�ã���˸����0.4ms����
*          0x0F:�ַ���˸�������
*          0x06:д�������Զ����ƹ�꣬��ͨ���Σ�����������ʾ  40us
*          0x04:д�������Զ����ƹ�꣬���Դ���������ʾ  40us
*          0x01:�����ʾ���ݣ�������  1.64ms
*          0x02:ʹ��껹����ʼλ��   1.64ms
*          0x18:��Ļ�������ַ�ͬʱ����һ���ʺϹ�����ʾ 40us
*          0x1C:��Ļ�������ַ�ͬʱ����һ���ʺϹ�����ʾ 40us
*********************************************************************/
void LCD_Init(void);

#endif 
