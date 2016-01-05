/*********************************************************************
* �ļ����ƣ�LCD1602.C
* ���ܣ�Ϊ51��Ƭ���ṩLCD1602�������򣬽��LCD1602���ϲ�����ĸ�������
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
*��    �ƣ�LCD_busy()
*��    �ܣ���LCDæµ״̬,ֱ��LCD1602��æΪֹ
*��ڲ�������
*���ڲ������޷����ֽ����ͣ�����0��ʾLCD�Ѿ�������ϣ����С�
*˵    ����һ����д�������������֮ǰ��while(LCD_busy())���ȴ�LCD����
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
*��    �ܣ�һֱ�ȴ���LCD1602����
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
*��    �ܣ�д��LCD��ʼ��ʱ����������Լ��æ״̬
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
*��    �ƣ�LCD_Writecmd()
*��    �ܣ�дָ�LCD1602,ָ������ռһ���ֽ�
*��ڲ�����cmd:��д���ָ��
*���ڲ�������
*********************************************************************/
void LCD_Writecmd(uchar cmd)
{                          
//while(LCD_busy());
GPIO_ResetBits(LCD_CMD,LCD_RS_PIN);    //��ͬһ���Ĵ���������д�룬�м���ʱһ��
delay_us(1);
delay_us(10);
GPIO_ResetBits(LCD_CMD,LCD_EN_PIN);  
delay_us(10);//��������
delay_us(2);//��������


Gpio_data(cmd,1);  //?????? cmd;
delay_us(10);
delay_us(1);
//GPIO_SetBits(LCD_CMD,LCD_EN_PIN); 
//delay_us(300);//��Ҫ����ʱ
//GPIO_ResetBits(LCD_CMD,LCD_EN_PIN);      //�½��أ�LCD1602��ʼ����
}
/*********************************************************************
*��    �ƣ�LCD_Writedata()
*��    �ܣ�дһ�ֽ����ݵ�LCD1602
*��ڲ�����dat���޷����ֽ����ͣ�0~255  ��������ASCII���ַ�
*���ڲ�������
*********************************************************************/
void LCD_Writedata(uchar dat) 
{                         
//while(LCD_busy());//�ȴ�LCD1602����

GPIO_SetBits(LCD_CMD,LCD_RS_PIN);
delay_us(10);
delay_us(1);

GPIO_ResetBits(LCD_CMD,LCD_EN_PIN);  
delay_us(10);
delay_us(1);

Gpio_data(dat,0);  //?????? cmd;
delay_us(10);
delay_us(1);
//GPIO_SetBits(LCD_CMD,LCD_EN_PIN);   //������
//delay_us(300); //����Ҫ����ʱ�������ԣ���ʱ300us���ϲſ���
//GPIO_ResetBits(LCD_CMD,LCD_EN_PIN);         //�½��أ���ʼд����Ч����
}
/*********************************************************************
*��    �ƣ�LCD_pos()
*��    �ܣ��趨��ʾλ��
*��ڲ�����pos:��ʾλ��,ֵ�ķ�Χ���£�
*    0x00----------------0x0f 0x10-------0x27 ��һ�У�һ����ʾ16���ַ���
*    0x40----------------0x4f 0x50-------0x67 �ڶ���
*���ڲ�������
*********************************************************************/
void LCD_pos(uchar pos)
{                         
LCD_Writecmd(pos | 0x80);
}
/*********************************************************************
*��    �ƣ�LCD_Setpos()
*��    �ܣ�����ϰ���趨��ʾλ��
*��ڲ�����row:�У�row=1��ʾ��һ�У�row=2��ʾ�ڶ���
*          col:�У�0~15������ָ����ʾ���У���Χ������0~40
*���ڲ�������
*********************************************************************/
void LCD_Setpos(uchar row,uchar col)
{
if(row==1) LCD_Writecmd(col | 0x80);
else LCD_Writecmd(col | 0xC0);
}
/*********************************************************************
*��  �ܣ���ʾһ���ַ�   
*��  �ڣ�ch������ʾ���ַ�
*********************************************************************/
void LCD_DispChar(char ch)
{
LCD_Writedata(ch);
}
/*********************************************************************
*��  �ܣ���ָ��λ����ʾһ���ַ�   
*��  �ڣ�row:�� 1��2        col:�У�0~15
*        ch������ʾ���ַ�
*********************************************************************/
void LCD_Setpos_DispChar(uchar row,uchar col,char ch)
{
LCD_Setpos(row,col);
LCD_Writedata(ch);
}
/*********************************************************************
*��    �ƣ�LCD_DispString()
*��    �ܣ�ʹLCD1602��ʾһ���ַ�������ʾλ������ǰ�趨
*��ڲ�����str[]:����ʾ���ַ���
*���ڲ�������
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
*��    �ƣ�LCD_Setpos_DispString()
*��    �ܣ�ʹLCD1602��ָ��λ�ÿ�ʼ��ʾһ���ַ���
*��ڲ�����row:��ʾ�У�1��2
*          col:��ʾ�У�0~15 ��0~40�� 
*          str[]:����ʾ���ַ���
*���ڲ�������
*˵    ����ָ��λ������ʾ�ĳ�ʼλ�ã���һ���ַ���ʾ��λ��
*********************************************************************/
void LCD_Setpos_DispString(uchar row,uchar col,char str[])
{
LCD_Setpos(row,col);
LCD_DispString(str);
}
/*********************************************************************
*��    �ƣ�LCD_Dispnum()
*��    �ܣ���ʾһ��������8λ����������ʾλ������ǰ����
*��ڲ�����num:����ʾ��������������8λ
*���ڲ�������
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
LCD_DispString(c);//��ʾ�ַ���
}
/*********************************************************************
*��    �ƣ�LCD_Setpos_Dispnum()
*��    �ܣ���ָ��λ����ʾһ��������8λ������
*��ڲ�����num:����ʾ��������������8λ
*���ڲ�������
*********************************************************************/
void LCD_Setpos_Dispnum(uchar row,uchar col,uint32_t num)
{
   LCD_Setpos(row,col); 
   LCD_Dispnum(num);
}
/*********************************************************************
*��    �ƣ�LCD_DispDecimal()
*��    �ܣ���ʾһ����Чλ������8λ�ĸ�����,��ʾλ����Ҫ��ǰ�趨
*��ڲ�����num:����ʾ�ĸ������Ŵ��õ���������������8λ
*          dot:С�����λ��
*���ڲ�������
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
		LCD_DispString(str);//��ʾ����С��
}

/*********************************************************************
*��    �ƣ�LCD_Setpos_DispDecimal()
*��    �ܣ���ָ��λ����ʾһ����Чλ������8λ�ĸ�����
*��ڲ�����row:����ʾ���� 1,2
*          col:����ʾ���� 0~15
*          num:����ʾ�ĸ������Ŵ�10����������õ���������������8λ
*          dot:С�����λ��
*���ڲ�������
*********************************************************************/
void LCD_Setpos_DispDecimal(uchar row,uchar col,uint32_t num,uchar dot)
{
  LCD_Setpos(row,col);
  LCD_DispDecimal(num,dot);
}
/*********************************************************************
*��    �ƣ�LCD_DispDateTime()
*��    �ܣ���ʾ��������ʾ������ʱ��
*��ڲ�����year:��,����������ֵ
*          month:�£�1~12
*          day:�գ�1~31
*          hour:ʱ��1~24 ��  1~12
*          min:�֣�0~59
*          sec:�룬0~59
*���ڲ�������
*˵    ����ʹ��ǰ�����Ⱥ궨��USE_LCD_DISP_DATE���ڰ����ļ�֮ǰ������Ч
*��ʾ��ʽ��Date:yyyy-mm-dd  *
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
LCD_DispChar('*'); //��һ�н�������ʾ
LCD_Setpos(2,0);
LCD_DispString("Time:");
LCD_Dispnum((uint32_t)hour);
LCD_DispChar(':');
LCD_Dispnum((uint32_t)min);
LCD_DispChar(':');
LCD_Dispnum((uint32_t)sec);
LCD_Setpos(2,15);
LCD_DispChar('*'); //�ڶ��н�������ʾ
}
/*********************************************************************
*��    �ƣ�LCD_DispStopWatch()
*��    �ܣ���ʾ�����ʾʱ���֣��룬10���룬��ȷ��10ms
*��ڲ�����hour:ʱ��1~24 ��  1~12
*          min:�֣�0~59
*          sec:�룬0~59
*          tenms:10ms����ֵ����3��ʾ30ms
*���ڲ�������
*��ʾ��ʽ��Current Time:   *
*            hh:mm:ss:tt   *
*********************************************************************/
void LCD_DispStopWatch(uchar hour,uchar min,uchar sec,uchar tenms)
{
LCD_Setpos(1,0);
LCD_DispString("Current Time:");
LCD_Setpos(1,15);
LCD_DispChar('*'); //��һ�н�������ʾ
LCD_Setpos(2,2);
LCD_Dispnum((uint32_t)hour);
LCD_DispChar(':');
LCD_Dispnum((uint32_t)min);
LCD_DispChar(':');
LCD_Dispnum((uint32_t)sec);
LCD_DispChar(':');
LCD_Dispnum((uint32_t)tenms);
LCD_Setpos(2,15);
LCD_DispChar('*'); //�ڶ��н�������ʾ
}
/*********************************************************************
*��    �ƣ�LCD_DispFreq()
*��    �ܣ���ʾƵ�ʼƣ���̬�Զ�����Ƶ����ʾ
*��ڲ�����freq:������ʾ8λ������ʾ��Ƶ��ֵ��Ƶ�ʷ�Χ1Hz~99MHz
*���ڲ�������
*˵    ������ڵ�Ƶ��ֵ��λ��Hz����ʾ�����Զ�ѡ����ʵ���ʾ��ʽ
*          freq>=1000000��ʾ��6λС����MHz
*    else  freq>=1000   ��ʾ��3λС����KHz
*    else  freq>=0      Ĭ����ʾ��λ��Hz  ���������以�⣩
*********************************************************************/
void LCD_DispFreq(uint32_t freq)
{
LCD_Setpos(1,0);
LCD_DispString("Current Freq:");
LCD_Setpos(2,1); //��ʾλ��
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
//��Ļ��������һ�����ڹ�����ʾ
void LCD_ShiftLeft()
{
LCD_Writecmd(0x18);
}
//��Ļ��������һ�����ڹ�����ʾ
void LCD_ShiftRight(void)
{
LCD_Writecmd(0x1C);
}
//�����������ʾ
void LCD_Clear(void)
{
LCD_Writecmd(0x01);
}
//ʹ��껹��ԭ��λ��
void LCD_Return(void)
{
LCD_Writecmd(0x02);
}
//����ʾ
void LCD_Close(void)
{
LCD_Writecmd(0x08);
}
//����ʾ
void LCD_Open(void)
{
LCD_Writecmd(0x0C);
}
/*********************************************************************
*��    �ƣ�LCD_FlickerChar()
*��    �ܣ�ʹָ��λ���ַ���˸������ʾ���
*��ڲ�����row:�� 1,2  
*          col:�� 0~15
*���ڲ�������
*˵    ����д����0x0D����ʾ������˸��д����0x0F����ʾ������˸
*          һ���趨��˸�󣬻����λ�ñ仯��˸������˸д����0x0C
*********************************************************************/
void LCD_FlickerChar(uchar row,uchar col)
{
LCD_Writecmd(0x0D);
LCD_Setpos(row,col);
}
//�ر��ַ���˸
void LCD_CloseFlicker(void)
{
LCD_Writecmd(0x0C);
}
//��Ļ����˸һ��
void LCD_FlickerScreen(void)
{
LCD_Writecmd(0x08);//����ʾ
delay_ms(500);
LCD_Writecmd(0x0C);//����ʾ
delay_ms(500);
}
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
void LCD_Init(void)
{   
	  Lcd_GPIO_init();
   /* delay_ms(200);           //��ʱ20ms         
LCD_WriteInitcmd(0x38);  //16*2��ʾ��5*7����8λ����
delay_ms(10);
LCD_WriteInitcmd(0x38);  //16*2��ʾ��5*7����8λ����
delay_ms(5);
LCD_WriteInitcmd(0x38);  //16*2��ʾ��5*7����8λ����
delay_ms(5);
LCD_WriteInitcmd(0x08);  //�ȹ���ʾ������ʾ
delay_ms(5);
LCD_WriteInitcmd(0x06);  //�Զ����ƹ��,0x04Ϊ���ƹ��
delay_ms(5);
LCD_WriteInitcmd(0x01);  //���LCD����ʾ����
delay_ms(6);
LCD_WriteInitcmd(0x0c);  //��ʾ�����ع��;0x08Ϊ����ʾ
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
