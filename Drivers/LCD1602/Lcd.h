#ifndef	___Lcd_H__
#define	___Lcd_H__

#define LCD_BUFFER_EN                   1
#include <stdbool.h>
#include "sys.h"
#define	LCD_LINE_DISP_CHAR_MAX		(u8)16
#define	LCD_LINE_MAX					(u8)2


#define	PinLcdBackLightPort		GPIOC
#define	PinLcdBackLight		GPIO_Pin_8
#define	PinLcdRSPort			GPIOC
#define	PinLcdRS			GPIO_Pin_7
#define	PinLcdEnPort			GPIOC
#define	PinLcdEn			GPIO_Pin_6
#define	PinLcdD0Port			GPIOD
#define	PinLcdD0			GPIO_Pin_15
#define	PinLcdD1Port			GPIOD
#define	PinLcdD1			GPIO_Pin_14
#define	PinLcdD2Port			GPIOD
#define	PinLcdD2			GPIO_Pin_13
#define	PinLcdD3Port			GPIOD
#define	PinLcdD3			GPIO_Pin_12
   


void LCD_Init(void);
void  LCD_Writecmd(u8 dat);
void LCD_Writedata(u8 dat) ;
#define FbLCDShowLock  GPIOC->IDR


extern void delay_ms(u16 nms);




#endif






















