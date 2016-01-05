#include "stm32f10x_conf.h"

#include "Lcd.h"
#include "sys.h"
#include <stdbool.h>
#include "stm32f10x.h"
/*
 *IO�˿ڲ������ǾͿ��Խӳ�6�߽ӷ���RS��E��DB4��DB5��DB6��DB7
 *WR:ֻ����LCDд�����ݿ���ֱ�ӽӵ�
 *����������6��������IO�ߣ�
 *�����Ϳ��Խ�ʡ��5��IO��
 *��������ʱ�ȸ�4bit,�ٵ�4bit
*/

/**
 *Function:
 *	lcd 1602 Write Command and DAta 
 *Input:
 *	dat		:write data
 *	CmdDatOpt		:=true	is write Command	,= FALSE is Write Dat
 *Output:
 *	none
 */
 /**
 *Function:
 *	?????us
 *Input:
 *	ms	:	???us
 *Oputput:
 *	none
 *Note:
 */
static void DelayUS(u16 us)
{
	u16 t;

	do
	{
		for(t = 15 ; t > 0 ; t--);
	}while(--us);

}
static void LCD1602_Write(u8 dat , bool CmdDatOpt)
{
	u8 i;

	if(FbLCDShowLock == true)return;
	
	if(CmdDatOpt == true)
	{
		GPIO_ResetBits(PinLcdRSPort , PinLcdRS);		/* RS = 0 : Is Write Command */
	}
	else
	{
		GPIO_SetBits(PinLcdRSPort , PinLcdRS);			/* RS = 1 : Is Write Dat */
	}
	DelayUS(2);
	GPIO_ResetBits(PinLcdEnPort , PinLcdEn);
	DelayUS(2);
	for(i = 0 ; i < 2 ; i++)
	{
		/* ��д��4λ */
		GPIO_ResetBits(PinLcdD0Port , PinLcdD0);
		GPIO_ResetBits(PinLcdD1Port , PinLcdD1);
		GPIO_ResetBits(PinLcdD2Port , PinLcdD2);
		GPIO_ResetBits(PinLcdD3Port , PinLcdD3);
		if(dat & 0x80)GPIO_SetBits(PinLcdD3Port , PinLcdD3);
		if(dat & 0x40)GPIO_SetBits(PinLcdD2Port , PinLcdD2);
		if(dat & 0x20)GPIO_SetBits(PinLcdD1Port , PinLcdD1);
		if(dat & 0x10)GPIO_SetBits(PinLcdD0Port , PinLcdD0);		
		DelayUS(2);
		GPIO_SetBits(PinLcdEnPort , PinLcdEn);				
		DelayUS(5);
		GPIO_ResetBits(PinLcdEnPort , PinLcdEn);			/* 1 -> 0 :ִ�в��� */

		dat <<= 4;
	}
	if(CmdDatOpt == true)
	{
		delay_ms(5);
	}
	else
	{
		DelayUS(300);
	}
}


/**
;ģ������:LCD_write_command(); 
;��    ��:LCD1602дָ��� 
;ռ����Դ: P2.0--RS(LCD_RS),P2.1--RW(LCD_RW),P2.2--E(LCD_E).
;����˵��:datΪд�������
;��������:2008.08.15 
;��    ��:FV1.0(�����汾Function Version)
;�޸�����:--
;�޸�˵��:--
;*/ 
void  LCD_Writecmd(u8 dat)
{
	LCD1602_Write(dat, true);
	//LCD1602_Write(dat, true);
}

/*-------------------------------------- 
;ģ������:LCD_write_data(); 
;��    ��:LCD1602д���ݺ��� 
;-------------------------------------*/
void LCD_Writedata(u8 dat)
{ 
	LCD1602_Write(dat, false);
}




void LCD_Init(void)
{  
	GPIO_InitTypeDef GPIO_InitS; 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD, ENABLE);
	/* configure LCD Pin is Input Push-up */
	GPIO_InitS.GPIO_Pin = PinLcdRS;
	GPIO_InitS.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitS.GPIO_Mode = GPIO_Mode_Out_PP;	
	GPIO_Init(PinLcdRSPort, &GPIO_InitS);
	
	GPIO_SetBits(PinLcdBackLightPort , PinLcdBackLight);
	GPIO_InitS.GPIO_Pin = PinLcdEn;
	GPIO_Init(PinLcdEnPort, &GPIO_InitS);
	GPIO_SetBits(PinLcdEnPort , PinLcdEn);	/* ��ʹ�� */
	
	GPIO_InitS.GPIO_Pin = PinLcdD0 |PinLcdD1|PinLcdD2|PinLcdD3;
	GPIO_Init(PinLcdD3Port, &GPIO_InitS);

	
	GPIO_InitS.GPIO_Pin = PinLcdBackLight;
	GPIO_Init(PinLcdBackLightPort, &GPIO_InitS);
	//GPIO_SetBits(PinLcdBackLightPort , PinLcdBackLight);

	
	delay_ms(200);    //����8λ��ʽ��2�У�5x7
	LCD_Writecmd(0x38);		//����4λ��ʽ��2�У�5x7
	
	LCD_Writecmd(0x01);		//�����Ļ��ʾ
	delay_ms(100);  ;       	
	LCD_Writecmd(0x01);		//�����Ļ��ʾ
	delay_ms(100);  ;       			//��ʱ�		//��ʱ��������ʱ��������ʱԼn��10us
	LCD_Writecmd(0x06);		//�趨���뷽ʽ����������λ
	LCD_Writecmd(0x80);		//
	//LCD1602_WriteCMD(0x0c);			//������ʾ���ع�꣬����˸
	LCD_Writecmd(0x0c);		//������ʾ���ع�꣬����˸


#if LCD_BUFFER_EN
    //LCD_screen.updateIntervalTime = 0; /* ����ˢ����ʾ */
#endif

    /* �����Զ����ַ� */
  //  LCD_SetUserChar();
}



