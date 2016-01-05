# DS3231+LCD1602+STM32

 - 作者：黄贤明
 - 时间：2016-1-5 17:37:30
 - 备注：配合公司验证ds3231可靠性检验。（跑一个星期看看误差多少。）
 --------------------------------------------------------------------------------
## 第一步：STM32+LCD1602跑通 

 - 这块板子用的是4根数据线，所以得先发送高4位，再发送低四位。
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
	
	- 初始化必须选择4总线模式
		LCD_Writecmd(0x28);
	- 搞清楚lcd1602的时序，和相关寄存器的配置。
	
## 第二步：加入DS3231，用1602显示。
  - ds3231使用iic总线，先移植好iic
  - ds3231良率不高，在可读写，时钟不走的情况下建议换一块。
  
  -----------------------------------------------------------------------
 #结束！
 ------------------------------------------------------------------------