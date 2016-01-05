/*********************************************************************
* 文件名称：LCD1602.C
* 功能：为51单片机提供LCD1602驱动程序，解决LCD1602与上层软件的隔离问题
*********************************************************************/
#include "stm32f10x.h"
#include "LCD1602.h"
#include "delay.h"

#ifndef  LCD1602_STM32_C
#define  LCD1602_STM32_C

/************************?????*******************************/
/*
static void delay_us(u16 time)
{
		u16 i=0;
	while(time--)
	{
		i=30;
		while(i--);
	}
}

static void delay_ms(u16 time)
{
		u16 i=0;
	while(time--)
	{
		i=12000;
		while(i--);
	}
}

*/
void Lcd_GPIO_init(void)
{
      GPIO_InitTypeDef GPIO_InitStructure;   //?????
	   GPIO_InitTypeDef GPIO_InitStructure_BL;   //?????
     
    /********Data????*************/
	   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);  //????B?? 
     GPIO_InitStructure.GPIO_Pin  = GPIO_DATA_4_PIN|GPIO_DATA_5_PIN|GPIO_DATA_6_PIN|GPIO_DATA_7_PIN; //  DB4~DB7
     GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //??????
     GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz; //50M????
     GPIO_Init(GPIO_DATA, &GPIO_InitStructure);      //?????
	
	    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);  //????B?? 
	       
     GPIO_InitStructure_BL.GPIO_Pin  = LCD_BL_PIN|LCD_EN_PIN|LCD_RS_PIN; 
     GPIO_InitStructure_BL.GPIO_Mode = GPIO_Mode_Out_PP;  //??????
     GPIO_InitStructure_BL.GPIO_Speed = GPIO_Speed_10MHz; //50M????
     GPIO_Init(LCD_CMD, &GPIO_InitStructure_BL);      //?????
		 GPIO_SetBits(LCD_CMD,LCD_BL_PIN); 
	

}
/******************************************************************/
/*********************************************************************
*名    称：LCD_busy()
*功    能：读LCD忙碌状态,直到LCD1602不忙为止
*入口参数：无
*出口参数：无符号字节类型，返回0表示LCD已经操作完毕，空闲。
*说    明：一般在写入新命令或数据之前用while(LCD_busy())语句等待LCD就绪
*********************************************************************/

uint8_t LCD_busy(void)
{                         
GPIO_ResetBits(LCD_CMD,LCD_RS_PIN);     
delay_us(1);
delay_us(1);
do
{
  GPIO_ResetBits(LCD_CMD,LCD_EN_PIN); 	
  delay_us(200);
  GPIO_SetBits(LCD_CMD,LCD_EN_PIN); 
  delay_us(200);
}
while(GPIOD->IDR & 0x80);
GPIO_ResetBits(LCD_CMD,LCD_EN_PIN);  
return (uint8_t)0; 
}
/*********************************************************************
*功    能：一直等待到LCD1602空闲
*********************************************************************/
void Wait_Leisure(void)
{
LCD_busy();
}

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
/*********************************************************************
*功    能：写入LCD初始化时的命令，不可以检测忙状态
*********************************************************************/
void LCD_WriteInitcmd(uchar cmd)
{    

GPIO_ResetBits(LCD_CMD,LCD_RS_PIN);
delay_us(1);
delay_us(2);
GPIO_ResetBits(LCD_CMD,LCD_EN_PIN);
delay_us(1);
delay_us(2);	

Gpio_data(cmd,1);  //?????? cmd;
	/*
GPIO_ResetBits(LCD_CMD,LCD_EN_PIN);  
delay_us(300);
GPIO_SetBits(LCD_CMD,LCD_EN_PIN); 
delay_us(300);
GPIO_ResetBits(LCD_CMD,LCD_EN_PIN);  
delay_ms(2);
	*/
}
/*********************************************************************
*名    称：LCD_Writecmd()
*功    能：写指令到LCD1602,指令数据占一个字节
*入口参数：cmd:待写入的指令
*出口参数：无
*********************************************************************/
void LCD_Writecmd(uchar cmd)
{                          
//while(LCD_busy());
GPIO_ResetBits(LCD_CMD,LCD_RS_PIN);    //对同一个寄存器的两次写入，中间延时一会
delay_us(1);
delay_us(10);
GPIO_ResetBits(LCD_CMD,LCD_EN_PIN);  
delay_us(10);//产生脉冲
delay_us(2);//产生脉冲


Gpio_data(cmd,1);  //?????? cmd;
delay_us(10);
delay_us(1);
//GPIO_SetBits(LCD_CMD,LCD_EN_PIN); 
//delay_us(300);//必要的延时
//GPIO_ResetBits(LCD_CMD,LCD_EN_PIN);      //下降沿，LCD1602开始工作
}
/*********************************************************************
*名    称：LCD_Writedata()
*功    能：写一字节数据到LCD1602
*入口参数：dat：无符号字节类型，0~255  包括各个ASCII码字符
*出口参数：无
*********************************************************************/
void LCD_Writedata(uchar dat) 
{                         
//while(LCD_busy());//等待LCD1602空闲

GPIO_SetBits(LCD_CMD,LCD_RS_PIN);
delay_us(10);
delay_us(1);

GPIO_ResetBits(LCD_CMD,LCD_EN_PIN);  
delay_us(10);
delay_us(1);

Gpio_data(dat,0);  //?????? cmd;
delay_us(10);
delay_us(1);
//GPIO_SetBits(LCD_CMD,LCD_EN_PIN);   //先拉高
//delay_us(300); //很重要的延时，经调试，延时300us以上才可以
//GPIO_ResetBits(LCD_CMD,LCD_EN_PIN);         //下降沿，开始写入有效数据
}
/*********************************************************************
*名    称：LCD_pos()
*功    能：设定显示位置
*入口参数：pos:显示位置,值的范围如下：
*    0x00----------------0x0f 0x10-------0x27 第一行（一次显示16个字符）
*    0x40----------------0x4f 0x50-------0x67 第二行
*出口参数：无
*********************************************************************/
void LCD_pos(uchar pos)
{                         
LCD_Writecmd(pos | 0x80);
}
/*********************************************************************
*名    称：LCD_Setpos()
*功    能：根据习惯设定显示位置
*入口参数：row:行，row=1表示第一行，row=2表示第二行
*          col:列，0~15，用于指定显示的列，范围可以是0~40
*出口参数：无
*********************************************************************/
void LCD_Setpos(uchar row,uchar col)
{
if(row==1) LCD_Writecmd(col | 0x80);
else LCD_Writecmd(col | 0xC0);
}
/*********************************************************************
*功  能：显示一个字符   
*入  口：ch：待显示的字符
*********************************************************************/
void LCD_DispChar(char ch)
{
LCD_Writedata(ch);
}
/*********************************************************************
*功  能：在指定位置显示一个字符   
*入  口：row:行 1或2        col:列，0~15
*        ch：待显示的字符
*********************************************************************/
void LCD_Setpos_DispChar(uchar row,uchar col,char ch)
{
LCD_Setpos(row,col);
LCD_Writedata(ch);
}
/*********************************************************************
*名    称：LCD_DispString()
*功    能：使LCD1602显示一个字符串，显示位置需提前设定
*入口参数：str[]:待显示的字符串
*出口参数：无
*********************************************************************/
void LCD_DispString(char str[])
{
    uchar i=0;
while(str[i] != '\0')
{
    LCD_Writedata(str[i]);
    ++i;
}
}
/*********************************************************************
*名    称：LCD_Setpos_DispString()
*功    能：使LCD1602从指定位置开始显示一个字符串
*入口参数：row:显示行，1或2
*          col:显示列，0~15 （0~40） 
*          str[]:待显示的字符串
*出口参数：无
*说    明：指定位置是显示的初始位置，第一个字符显示的位置
*********************************************************************/
void LCD_Setpos_DispString(uchar row,uchar col,char str[])
{
LCD_Setpos(row,col);
LCD_DispString(str);
}
/*********************************************************************
*名    称：LCD_Dispnum()
*功    能：显示一个不超过8位的整数，显示位置需提前设置
*入口参数：num:待显示的整数，不超过8位
*出口参数：无
*********************************************************************/
void LCD_Dispnum(uint32_t num)
{
   /*	char *str;
	  uchar dot = 2;
	  char table[]={'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
    uint8_t i;
    int temp = num;
    for (i = 1; i<=dot; i++)
    {
        if (temp==0)
            str[dot-i] = table[0];
        else
            str[dot-i] = table[temp%10];
        temp = temp/10;
    }
	 str[dot] = '\0';
	*/
	int i,k;
int b[10];
char c[10];
uint32_t temp = num;
for(i=0;num;i++,num=num/10) 
b[i]=num%10;
for(k=0;k<i;k++) 
c[k]=b[i-1-k]+'0';

c[i]='\0';
if(temp==0)
{
	c[0]='0';
	c[1]='\0';
	
}
LCD_DispString(c);//显示字符串
}
/*********************************************************************
*名    称：LCD_Setpos_Dispnum()
*功    能：在指定位置显示一个不超过8位的整数
*入口参数：num:待显示的整数，不超过8位
*出口参数：无
*********************************************************************/
void LCD_Setpos_Dispnum(uchar row,uchar col,uint32_t num)
{
   LCD_Setpos(row,col); 
   LCD_Dispnum(num);
}
/*********************************************************************
*名    称：LCD_DispDecimal()
*功    能：显示一个有效位不超过8位的浮点数,显示位置需要提前设定
*入口参数：num:待显示的浮点数放大后得到的整数，不超过8位
*          dot:小数点的位数
*出口参数：无
*********************************************************************/
void LCD_DispDecimal(uint32_t num,uchar dot)
{
		char *str;
		static char table[]={'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
    uint8_t i;
    int temp = num/1;
    double t2 = 0.0;
    for (i = 1; i<=dot; i++)
    {
        if (temp==0)
            str[dot-i] = table[0];
        else
            str[dot-i] = table[temp%10];
        temp = temp/10;
    }
    *(str+dot) = '.';
    temp = 0;
    t2 = num;
    for(i=1; i<=8; i++)
    {
        temp = t2*10;
        str[dot+i] = table[temp%10];
        t2 = t2*10;
    }
    *(str+dot+i+1) = '\0';
		LCD_DispString(str);//显示浮点小数
}

/*********************************************************************
*名    称：LCD_Setpos_DispDecimal()
*功    能：在指定位置显示一个有效位不超过8位的浮点数
*入口参数：row:待显示的行 1,2
*          col:待显示的列 0~15
*          num:待显示的浮点数放大10的整数倍后得到的整数，不超过8位
*          dot:小数点的位数
*出口参数：无
*********************************************************************/
void LCD_Setpos_DispDecimal(uchar row,uchar col,uint32_t num,uchar dot)
{
  LCD_Setpos(row,col);
  LCD_DispDecimal(num,dot);
}
/*********************************************************************
*名    称：LCD_DispDateTime()
*功    能：显示日历，显示日期与时间
*入口参数：year:年,可以是任意值
*          month:月，1~12
*          day:日，1~31
*          hour:时，1~24 或  1~12
*          min:分，0~59
*          sec:秒，0~59
*出口参数：无
*说    明：使用前必须先宏定义USE_LCD_DISP_DATE，在包含文件之前定义有效
*显示格式：Date:yyyy-mm-dd  *
*          Time:hh:mm:ss    *
*********************************************************************/
void LCD_DispDateTime(uint32_t year,uchar month,uchar day,uchar hour,uchar min,uchar sec)
{
LCD_Writecmd(0x01);	
delay_ms(5);
LCD_Setpos(1,0);
LCD_DispString("Date:");
LCD_Dispnum(year);
LCD_DispChar('-');
LCD_Dispnum((uint32_t)month);
LCD_DispChar('-');
LCD_Dispnum((uint32_t)day);
LCD_Setpos(1,15);
LCD_DispChar('*'); //第一行结束符显示
LCD_Setpos(2,0);
LCD_DispString("Time:");
LCD_Dispnum((uint32_t)hour);
LCD_DispChar(':');
LCD_Dispnum((uint32_t)min);
LCD_DispChar(':');
LCD_Dispnum((uint32_t)sec);
LCD_Setpos(2,15);
LCD_DispChar('*'); //第二行结束符显示
}
/*********************************************************************
*名    称：LCD_DispStopWatch()
*功    能：显示秒表，显示时，分，秒，10毫秒，精确到10ms
*入口参数：hour:时，1~24 或  1~12
*          min:分，0~59
*          sec:秒，0~59
*          tenms:10ms计数值，如3表示30ms
*出口参数：无
*显示格式：Current Time:   *
*            hh:mm:ss:tt   *
*********************************************************************/
void LCD_DispStopWatch(uchar hour,uchar min,uchar sec,uchar tenms)
{
LCD_Setpos(1,0);
LCD_DispString("Current Time:");
LCD_Setpos(1,15);
LCD_DispChar('*'); //第一行结束符显示
LCD_Setpos(2,2);
LCD_Dispnum((uint32_t)hour);
LCD_DispChar(':');
LCD_Dispnum((uint32_t)min);
LCD_DispChar(':');
LCD_Dispnum((uint32_t)sec);
LCD_DispChar(':');
LCD_Dispnum((uint32_t)tenms);
LCD_Setpos(2,15);
LCD_DispChar('*'); //第二行结束符显示
}
/*********************************************************************
*名    称：LCD_DispFreq()
*功    能：显示频率计，动态自动调整频率显示
*入口参数：freq:可以显示8位整数表示的频率值，频率范围1Hz~99MHz
*出口参数：无
*说    明：入口的频率值单位是Hz，显示函数自动选择合适的显示方式
*          freq>=1000000显示成6位小数的MHz
*    else  freq>=1000   显示成3位小数的KHz
*    else  freq>=0      默认显示单位是Hz  （三个区间互斥）
*********************************************************************/
void LCD_DispFreq(uint32_t freq)
{
LCD_Setpos(1,0);
LCD_DispString("Current Freq:");
LCD_Setpos(2,1); //显示位置
if(freq>=1000000)
{
  LCD_DispDecimal(freq,6);
  LCD_DispString("MHz");
}
else if(freq>=1000) 
{ 
  LCD_DispDecimal(freq,3);
  LCD_DispString("KHz");
}
else { LCD_Dispnum(freq);
     LCD_DispString("Hz");
}
LCD_Setpos(2,14);
LCD_DispString("OK");
}
//屏幕整体左移一格，用于滚动显示
void LCD_ShiftLeft()
{
LCD_Writecmd(0x18);
}
//屏幕整体右移一格，用于滚动显示
void LCD_ShiftRight(void)
{
LCD_Writecmd(0x1C);
}
//清屏，清除显示
void LCD_Clear(void)
{
LCD_Writecmd(0x01);
}
//使光标还回原点位置
void LCD_Return(void)
{
LCD_Writecmd(0x02);
}
//关显示
void LCD_Close(void)
{
LCD_Writecmd(0x08);
}
//开显示
void LCD_Open(void)
{
LCD_Writecmd(0x0C);
}
/*********************************************************************
*名    称：LCD_FlickerChar()
*功    能：使指定位置字符闪烁，不显示光标
*入口参数：row:行 1,2  
*          col:列 0~15
*出口参数：无
*说    明：写命令0x0D不显示光标的闪烁，写命令0x0F是显示光标的闪烁
*          一旦设定闪烁后，会根据位置变化闪烁，关闪烁写命令0x0C
*********************************************************************/
void LCD_FlickerChar(uchar row,uchar col)
{
LCD_Writecmd(0x0D);
LCD_Setpos(row,col);
}
//关闭字符闪烁
void LCD_CloseFlicker(void)
{
LCD_Writecmd(0x0C);
}
//屏幕秒闪烁一次
void LCD_FlickerScreen(void)
{
LCD_Writecmd(0x08);//关显示
delay_ms(500);
LCD_Writecmd(0x0C);//开显示
delay_ms(500);
}
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
void LCD_Init(void)
{   
	  Lcd_GPIO_init();
   /* delay_ms(200);           //延时20ms         
LCD_WriteInitcmd(0x38);  //16*2显示，5*7点阵，8位数据
delay_ms(10);
LCD_WriteInitcmd(0x38);  //16*2显示，5*7点阵，8位数据
delay_ms(5);
LCD_WriteInitcmd(0x38);  //16*2显示，5*7点阵，8位数据
delay_ms(5);
LCD_WriteInitcmd(0x08);  //先关显示，后开显示
delay_ms(5);
LCD_WriteInitcmd(0x06);  //自动右移光标,0x04为左移光标
delay_ms(5);
LCD_WriteInitcmd(0x01);  //清除LCD的显示内容
delay_ms(6);
LCD_WriteInitcmd(0x0c);  //显示开，关光标;0x08为关显示
delay_ms(5);*/
	
  delay_ms(200);    //??8???,2?,5x7
		
	LCD_Writecmd(0x28);		//??4???,2?,5x7
	delay_ms(5);
	LCD_Writecmd(0x28);
	delay_ms(5);
	LCD_Writecmd(0x28);
	delay_ms(5);
	LCD_Writecmd(0x28);
	LCD_Writecmd(0x28);
	LCD_Writecmd(0x28);
	LCD_Writecmd(0x28);
	LCD_Writecmd(0x28);
  LCD_Writecmd(0x08);
	delay_ms(5);
	LCD_Writecmd(0x01);		//??????
	delay_ms(5);
	LCD_Writecmd(0x06);
  delay_ms(5);
	LCD_Writecmd(0x0c);		//????,???,???
	delay_ms(5);

}
#endif  
