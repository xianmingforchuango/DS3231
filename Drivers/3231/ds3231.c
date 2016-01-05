/******************************************************************************
* @ File name --> ds3231.c
* @ Author    --> By@ Sam Chan
* @ Version   --> V1.0
* @ Date      --> 02 - 01 - 2014
* @ Brief     --> �߾���ʼ��оƬDS3231��������
*
* @ Copyright (C) 20**
* @ All rights reserved
*******************************************************************************
*
*                                  File Update
* @ Version   --> V1.
* @ Author    -->
* @ Date      -->
* @ Revise    -->
*
******************************************************************************/

#include "ds3231.h"
#include "myiic.h"
/******************************************************************************
                               ������ʾʱ���ʽ
                         Ҫ�ı���ʾ�ĸ�ʽ���޸Ĵ�����
******************************************************************************/

u8 Display_Time[8] = {0x30,0x30,0x3a,0x30,0x30,0x3a,0x30,0x30};	
					//ʱ����ʾ����   ��ʽ  00:00:00

u8 Display_Date[13] = {0x32,0x30,0x31,0x33,0x2f,0x31,0x30,0x2f,0x32,0x30,0x20,0x37,0x57};
					//������ʾ����   ��ʽ  2013/10/20 7W

/******************************************************************************
                               ������صı�������
******************************************************************************/

Time_Typedef TimeValue;	//����ʱ������ָ��

u8 Time_Buffer[7];	//ʱ���������ݻ���

/******************************************************************************
* Function Name --> DS3231ĳ�Ĵ���д��һ���ֽ�����
* Description   --> none
* Input         --> REG_ADD��Ҫ�����Ĵ�����ַ
*                   dat��Ҫд�������
* Output        --> none
* Reaturn       --> none 
******************************************************************************/
void DS3231_Write_Byte(u8 REG_ADD,u8 dat)
{
	IIC_Start();
IIC_Send_Byte(DS3231_Write_ADD);	//����д������Ӧ��λ

		IIC_Send_Byte(REG_ADD);
		IIC_Send_Byte(dat);	//��������
	IIC_Stop();
}
/******************************************************************************
* Function Name --> DS3231ĳ�Ĵ�����ȡһ���ֽ�����
* Description   --> none
* Input         --> REG_ADD��Ҫ�����Ĵ�����ַ
* Output        --> none
* Reaturn       --> ��ȡ���ļĴ�������ֵ 
******************************************************************************/
u8 DS3231_Read_Byte(u8 REG_ADD)
{
	u8 ReData;
	IIC_Start();
	IIC_Send_Byte(DS3231_Write_ADD);//����д������Ӧ��λ
IIC_Send_Byte(REG_ADD);	//ȷ��Ҫ�����ļĴ���
		IIC_Start();	//��������
		IIC_Send_Byte(DS3231_Read_ADD);	//���Ͷ�ȡ����
		ReData = IIC_Read_Byte(0);	//��ȡ����
		IIC_NAck();	//���ͷ�Ӧ���źŽ������ݴ���
	IIC_Stop();
	return ReData;
}
/******************************************************************************
* Function Name --> DS3231��ʱ�������Ĵ���������д�����ݻ��߶�ȡ����
* Description   --> ����д��n�ֽڻ���������ȡn�ֽ�����
* Input         --> REG_ADD��Ҫ�����Ĵ�����ʼ��ַ
*                   *WBuff��д�����ݻ���
*                   num��д����������
*                   mode������ģʽ��0��д�����ݲ�����1����ȡ���ݲ���
* Output        --> none
* Reaturn       --> none
******************************************************************************/
void DS3231_Operate_Register(u8 REG_ADD,u8 *pBuff,u8 num,u8 mode)
{
	u8 i;
	if(mode)	//��ȡ����
	{
		IIC_Start();
		if(!(IIC_Send_Byte(DS3231_Write_ADD)))	//����д������Ӧ��λ
		{
			IIC_Send_Byte(REG_ADD);	//��λ��ʼ�Ĵ�����ַ
			IIC_Start();	//��������
			IIC_Send_Byte(DS3231_Read_ADD);	//���Ͷ�ȡ����
			for(i = 0;i < num;i++)
			{
				*pBuff = IIC_Read_Byte(1);	//��ȡ����
				if(i == (num - 1))	IIC_NAck();	//���ͷ�Ӧ���ź�
				else IIC_Ack();	//����Ӧ���ź�
				pBuff++;
			}
		}
		IIC_Stop();	
	}
	else	//д������
	{		 	
		IIC_Start();
		if(!(IIC_Send_Byte(DS3231_Write_ADD)))	//����д������Ӧ��λ
		{
			IIC_Send_Byte(REG_ADD);	//��λ��ʼ�Ĵ�����ַ
			for(i = 0;i < num;i++)
			{
				IIC_Send_Byte(*pBuff);	//д������
				pBuff++;
			}
		}
		IIC_Stop();
	}
}
/******************************************************************************
* Function Name --> DS3231��ȡ����д��ʱ����Ϣ
* Description   --> ����д��n�ֽڻ���������ȡn�ֽ�����
* Input         --> *pBuff��д�����ݻ���
*                   mode������ģʽ��0��д�����ݲ�����1����ȡ���ݲ���
* Output        --> none
* Reaturn       --> none
******************************************************************************/
void DS3231_ReadWrite_Time(u8 mode)
{
	u8 Time_Register[8];	//����ʱ�仺��
	
	if(mode)	//��ȡʱ����Ϣ
	{
		DS3231_Operate_Register(Address_second,Time_Register,7,1);	//�����ַ��0x00����ʼ��ȡʱ����������
		
		/******�����ݸ��Ƶ�ʱ��ṹ���У��������������******/
		TimeValue.second = Time_Register[0] & Shield_secondBit;	//������
		TimeValue.minute = Time_Register[1] & Shield_minuteBit;	//��������
		TimeValue.hour   = Time_Register[2] & Shield_hourBit;	//Сʱ����
		TimeValue.week   = Time_Register[3] & Shield_weekBit;	//��������
		TimeValue.date   = Time_Register[4] & Shield_dateBit;	//������
		TimeValue.month  = Time_Register[5] & Shield_monthBit;	//������
		TimeValue.year   = Time_Register[6] | DS3231_YEARDATA;	//������
	}
	else
	{
		/******��ʱ��ṹ���и������ݽ���******/
		Time_Register[0] = TimeValue.second;	//��
		Time_Register[1] = TimeValue.minute;	//����
		Time_Register[2] = TimeValue.hour | Hour_Mode24;	//Сʱ
		Time_Register[3] = TimeValue.week;	//����
		Time_Register[4] = TimeValue.date;	//��
		Time_Register[5] = TimeValue.month;	//��
		Time_Register[6] = (u8)TimeValue.year;	//��
		
		DS3231_Operate_Register(Address_second,Time_Register,7,0);	//�����ַ��0x00����ʼд��ʱ����������
	}
}
/******************************************************************************
* Function Name --> ʱ��������ʼ��
* Description   --> none
* Input         --> *TimeVAL��RTCоƬ�Ĵ���ֵָ��
* Output        --> none
* Reaturn       --> none
******************************************************************************/
void DS3231_Time_Init(Time_Typedef *TimeVAL)
{	
	//ʱ����������
	Time_Buffer[0] = TimeVAL->second;
	Time_Buffer[1] = TimeVAL->minute;
	Time_Buffer[2] = TimeVAL->hour;
	Time_Buffer[3] = TimeVAL->week;
	Time_Buffer[4] = TimeVAL->date;
	Time_Buffer[5] = TimeVAL->month;
	Time_Buffer[6] = (u8)TimeVAL->year;
	
	DS3231_Operate_Register(Address_second,Time_Buffer,7,0);	//���루0x00����ʼд��7������
	DS3231_Write_Byte(Address_control, OSC_Enable);
	DS3231_Write_Byte(Address_control_status, Clear_OSF_Flag);
}
/******************************************************************************
* Function Name --> DS3231��⺯��
* Description   --> ����ȡ����ʱ��������Ϣת����ASCII�󱣴浽ʱ���ʽ������
* Input         --> none
* Output        --> none
* Reaturn       --> 0: ����
*                   1: ������������Ҫ��ʼ��ʱ����Ϣ
******************************************************************************/
u8 DS3231_Check(void)
{
	if(DS3231_Read_Byte(Address_control_status) & 0x80)  //����ֹͣ������
	{
		return 1;  //�쳣
	}
	else if(DS3231_Read_Byte(Address_control) & 0x80)  //���� EOSC����ֹ��
	{
		return 1;  //�쳣
	}
	else	return 0;  //����
}
/******************************************************************************
* Function Name --> ʱ���������ݴ�����
* Description   --> ����ȡ����ʱ��������Ϣת����ASCII�󱣴浽ʱ���ʽ������
* Input         --> none
* Output        --> none
* Reaturn       --> none
******************************************************************************/
void Time_Handle(void)
{
	/******************************************************
	                   ��ȡʱ��������Ϣ
	******************************************************/
	
	DS3231_ReadWrite_Time(1);	//��ȡʱ����������
	
	/******************************************************
	            ʱ����Ϣת��ΪASCII������ַ�
	******************************************************/
	
	Display_Time[6] = (TimeValue.second >> 4) + 0x30;
	Display_Time[7] = (TimeValue.second & 0x0f) + 0x30;	//Second

	Display_Time[3] = (TimeValue.minute >> 4) + 0x30;
	Display_Time[4] = (TimeValue.minute & 0x0f) + 0x30;	//Minute

	Display_Time[0] = (TimeValue.hour >> 4) + 0x30;
	Display_Time[1] = (TimeValue.hour & 0x0f) + 0x30;	//Hour 

	Display_Date[8] = (TimeValue.date >> 4) + 0x30;
	Display_Date[9] = (TimeValue.date & 0x0f) + 0x30;	//Date

	Display_Date[5] = (TimeValue.month >> 4) + 0x30;
	Display_Date[6] = (TimeValue.month & 0x0f) + 0x30;	//Month

	Display_Date[0] = (u8)(TimeValue.year >> 12) + 0x30;
	Display_Date[1] = (u8)((TimeValue.year & 0x0f00) >> 8) + 0x30;
	Display_Date[2] = (TimeValue.year >> 4) + 0x30;
	Display_Date[3] = (TimeValue.year & 0x0f) + 0x30;	//Year

	Display_Date[11] = (TimeValue.week & 0x0f) + 0x30;	//week

}
/******************************************************************************
* Function Name --> ��ȡоƬ�¶ȼĴ���
* Description   --> �¶ȼĴ�����ַΪ0x11��0x12�������Ĵ���Ϊֻ��
* Input         --> none
* Output        --> *Temp�������¶���ʾ�ַ�����
* Reaturn       --> none
******************************************************************************/
void DS3231_Read_Temp(u8 *Temp)
{
	u8 temph,templ;
	float temp_dec;

	temph = DS3231_Read_Byte(Address_temp_MSB);	//��ȡ�¶ȸ�8bits
	templ = DS3231_Read_Byte(Address_temp_LSB) >> 6;	//��ȡ�¶ȵ�2bits

	//�¶�ֵת��
	if(temph & 0x80)	//�ж��¶�ֵ������
	{	//���¶�ֵ
		temph = ~temph;	//��λȡ��
		templ = ~templ + 0x01;	//��λȡ����1
		Temp[0] = 0x2d;	//��ʾ��-��
	}
	else	Temp[0] = 0x20;	//���¶Ȳ���ʾ���ţ���ʾ������0x2b

	//С�����ּ��㴦��
	temp_dec = (float)templ * (float)0.25;	//0.25��ֱ���

	//�������ּ��㴦��
	temph = temph & 0x70;	//ȥ������λ
	Temp[1] = temph % 1000 / 100 + 0x30;	//��λ
	Temp[2] = temph % 100 / 10 + 0x30;	//ʮλ
	Temp[3] = temph % 10 + 0x30;	//��λ
	Temp[4] = 0x2e;	//.

	//С�����ִ���
	Temp[5] = (u8)(temp_dec * 10) + 0x30;	//С�����һλ
	Temp[6] = (u8)(temp_dec * 100) % 10 + 0x30;	//С������λ

	if(Temp[1] == 0x30)	Temp[1] = 0x20;	//��λΪ0ʱ����ʾ
	if(Temp[2] == 0x30)	Temp[2] = 0x20;	//ʮλΪ0ʱ����ʾ
}

