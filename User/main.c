#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "usmart.h"
#include "DS3231.h"
#include "LCD1602.h"

#define SIZE sizeof(TEXT_Buffer)
 int main(void)
 {	 
	u8  t=0;	
	delay_init();	    	 //��ʱ������ʼ��	  
	NVIC_Configuration(); 	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);	 	//���ڳ�ʼ��Ϊ9600
	LCD_Init();
	DS3231_Init();
	DS3231_Set(16,1,5,17,23,01);
	usmart_dev.init(72);	//��ʼ��USMART	
   
	while(1)
	{
		get_show_time();
		//delay_ms(500);
		if(t!=calendar.sec)
		{
			LCD_DispDateTime(calendar.w_year,calendar.w_month,calendar.w_date,calendar.hour,calendar.min,calendar.sec);
			t=calendar.sec;
		}
	}
}
