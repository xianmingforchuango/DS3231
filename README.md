# DS3231+LCD1602+STM32

 - ���ߣ�������
 - ʱ�䣺2016-1-5 17:37:30
 - ��ע����Ϲ�˾��֤ds3231�ɿ��Լ��顣����һ�����ڿ��������١���
 --------------------------------------------------------------------------------
## ��һ����STM32+LCD1602��ͨ 

 - �������õ���4�������ߣ����Ե��ȷ��͸�4λ���ٷ��͵���λ��
	 void Gpio_data(unsigned char x, unsigned char CmdDatOpt)    
	{  
		 int i;
	
		for(i=0;i<2;i++)
		{
			GPIO_ResetBits(GPIO_DATA, GPIO_DATA_4_PIN);  //DB4
			GPIO_ResetBits(GPIO_DATA, GPIO_DATA_5_PIN);  //DB5
			GPIO_ResetBits(GPIO_DATA, GPIO_DATA_6_PIN);  //DB6
			GPIO_ResetBits(GPIO_DATA, GPIO_DATA_7_PIN);  //DB7
			if(x&0X10)GPIO_SetBits(GPIO_DATA, GPIO_DATA_4_PIN);//DB4
			if(x&0X20)GPIO_SetBits(GPIO_DATA, GPIO_DATA_5_PIN);//DB5
			if(x&0X40)GPIO_SetBits(GPIO_DATA, GPIO_DATA_6_PIN);//DB6
			if(x&0X80)GPIO_SetBits(GPIO_DATA, GPIO_DATA_7_PIN);//DB7
			
	    delay_us(5);
			GPIO_SetBits(LCD_CMD,LCD_EN_PIN); 
			delay_us(10);
			GPIO_ResetBits(LCD_CMD,LCD_EN_PIN);  
			x <<= 4;
		}	
		if(CmdDatOpt == 1)
		{
			delay_us(5);
		}
		else
		{
			delay_us(300);
		}
	}
	
	- ��ʼ������ѡ��4����ģʽ
		LCD_Writecmd(0x28);
	- �����lcd1602��ʱ�򣬺���ؼĴ��������á�
	
## �ڶ���������DS3231����1602��ʾ��
  - ds3231ʹ��iic���ߣ�����ֲ��iic
  - ds3231���ʲ��ߣ��ڿɶ�д��ʱ�Ӳ��ߵ�����½��黻һ�顣
  
  -----------------------------------------------------------------------
 #������
 ------------------------------------------------------------------------