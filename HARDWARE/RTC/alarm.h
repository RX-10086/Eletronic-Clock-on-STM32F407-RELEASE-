#include "sys.h"
#include "usart.h"
#include "rtc.h"
#include "lcd.h"
#include "lcd_init.h"
#include "delay.h"
#include "Sakuya.h"        //ʮ��ҹ�Dҹ

#define TRUE 1
#define FALSE 0

RTC_TimeTypeDef RTC_TimeStruct1;     //RTCʱ�� ��׺������1
RTC_DateTypeDef RTC_DateStruct1;     //RTC����

RTC_TimeTypeDef time_datal;    //�洢ʱ�� ��׺Ϊ��ĸl
RTC_DateTypeDef date_datal;    //�洢����

int Week_Day_A[12] = {31,28,31,30,31,30,31,31,30,31,30,31};
int Week_Day_A2[12] = {31,29,31,30,31,30,31,31,30,31,30,31};

u8 tbuf_time[28];    //��ǰʱ��
u8 tbuf_date[20];
u8 tbuf_time_2[28];  //�趨ʱ��
u8 tbuf_date_2[20];

int flag_alarm_on = FALSE;   //�����Ƿ��

int sum = 0;
long int sta = 0;
int i = 0;

void ALARM_PRE(void)        //��ʼ��
{
	RTC_GetTime(RTC_Format_BIN,&RTC_TimeStruct1);
  RTC_GetDate(RTC_Format_BIN,&RTC_DateStruct1);
	LCD_Init();
	LCD_Fill(0,0,LCD_W,LCD_H,GRAYBLUE);
	LCD_ShowPicture(45,70,80,80,gImage_Sakuya); 
}
void ALARM_PRE_2(void)      //�趨ʱ���޸ĳɵ�ǰʱ�䣬��֮�󲻻��浱ǰʱ��仯���仯
{
	time_datal = RTC_TimeStruct1;
	date_datal = RTC_DateStruct1;
}
void SPE_ADJ_RTC(void)     //���µ�ǰʱ����ʾ
{
	RTC_GetTime(RTC_Format_BIN,&RTC_TimeStruct1);
  RTC_GetDate(RTC_Format_BIN,&RTC_DateStruct1);
}
void ALARM_DISPLAY(void)  //��ǰʱ��/�趨ʱ����ʾ
{
	sprintf((char*)tbuf_time,"Time = %02d:%02d",RTC_TimeStruct1.RTC_Hours,RTC_TimeStruct1.RTC_Minutes);
  sprintf((char*)tbuf_date,"Date = %02d-%02d",RTC_DateStruct1.RTC_Month,RTC_DateStruct1.RTC_Date);
  LCD_ShowString(5,5,tbuf_time,WHITE,GRAYBLUE,12,0);
  LCD_ShowString(5,16,tbuf_date,WHITE,GRAYBLUE,12,0);
	sprintf((char*)tbuf_time_2,"Set Time = %02d:%02d",time_datal.RTC_Hours,time_datal.RTC_Minutes);
  sprintf((char*)tbuf_date_2,"Set Date = %02d-%02d",date_datal.RTC_Month,date_datal.RTC_Date);
	LCD_ShowString(5,30,tbuf_time_2,WHITE,GRAYBLUE,12,0);
  LCD_ShowString(5,45,tbuf_date_2,WHITE,GRAYBLUE,12,0);
}
void PRE_SET_TIME(int alarm_flag_adj)   //����ʱ�䣨���ӣ�
{
	switch(alarm_flag_adj)
	{
		case 1:
		{
			time_datal.RTC_Hours++;
			if(time_datal.RTC_Hours == 24){time_datal.RTC_Hours = 0;}
			break;
		}
		case 2:
		{
			time_datal.RTC_Minutes++;
			if(time_datal.RTC_Minutes == 60){time_datal.RTC_Minutes = 0;}
			break;
		}
		case 3:
		{
			date_datal.RTC_Month++;
			if(date_datal.RTC_Month == 13){date_datal.RTC_Month = 1;}
			break;
		}
		case 4:
		{
			date_datal.RTC_Date++;
			if(date_datal.RTC_Year / 4 == 0)
			{
        if(date_datal.RTC_Date == Week_Day_A2[date_datal.RTC_Month - 1] + 1){date_datal.RTC_Date = 1;}
		  }
			else
			{
				if(date_datal.RTC_Date == Week_Day_A[date_datal.RTC_Month - 1] + 1){date_datal.RTC_Date = 1;}
			}
			break;
		}
	}
	if(date_datal.RTC_Year / 4 == 0)
	{
    if(date_datal.RTC_Date > Week_Day_A2[date_datal.RTC_Month - 1]){date_datal.RTC_Date = Week_Day_A2[date_datal.RTC_Month - 1];}
	}
	else
	{
		if(date_datal.RTC_Date > Week_Day_A[date_datal.RTC_Month - 1]){date_datal.RTC_Date = Week_Day_A[date_datal.RTC_Month - 1];}
	}
}
void PRE_SET_TIME_2(int alarm_flag_adj)    //����
{
	switch(alarm_flag_adj)
	{
		case 1:
		{
			if(time_datal.RTC_Hours > 0){time_datal.RTC_Hours--;}
			else {time_datal.RTC_Hours = 23;}
			break;
		}
		case 2:
		{
			if(time_datal.RTC_Minutes > 0){time_datal.RTC_Minutes--;}
			else {time_datal.RTC_Minutes = 59;}
			break;
		}
		case 3:
		{
			if(date_datal.RTC_Month > 1){date_datal.RTC_Month--;}
			else {date_datal.RTC_Month = 12;}
			break;
		}
		case 4:
		{
			if(date_datal.RTC_Year / 4 == 0)
			{
        if(date_datal.RTC_Date > 1) {date_datal.RTC_Date--;}
        else {date_datal.RTC_Date = Week_Day_A2[date_datal.RTC_Month - 1];}
		  }
			else
			{
				if(date_datal.RTC_Date > 1) {date_datal.RTC_Date--;}
        else {date_datal.RTC_Date = Week_Day_A[date_datal.RTC_Month - 1];}
			}
			break;
		}
	}
	if(date_datal.RTC_Year / 4 == 0)
	{
    if(date_datal.RTC_Date > Week_Day_A2[date_datal.RTC_Month - 1]){date_datal.RTC_Date = Week_Day_A2[date_datal.RTC_Month - 1];}
	}
	else
	{
		if(date_datal.RTC_Date > Week_Day_A[date_datal.RTC_Month - 1]){date_datal.RTC_Date = Week_Day_A[date_datal.RTC_Month - 1];}
	}
}
void ALARM_SET_FIN(void)     //ѡ����������ģʽ
{
	for(i = RTC_DateStruct1.RTC_Month + 1; i <= date_datal.RTC_Month - 1; i++)
	{
		if(date_datal.RTC_Year / 4 == 0) {sum = sum + Week_Day_A2[i - 1];}
		else {sum = sum + Week_Day_A[i - 1];}
	}
	i = (date_datal.RTC_Year / 4 == 0)?(Week_Day_A2[RTC_DateStruct1.RTC_Month]):(Week_Day_A[RTC_DateStruct1.RTC_Month]);
	if(RTC_DateStruct1.RTC_Month != date_datal.RTC_Month)
	{
		sta = (sum + (date_datal.RTC_Date + i - RTC_DateStruct1.RTC_Date)) * 24 * 60 + (time_datal.RTC_Hours - RTC_TimeStruct1.RTC_Hours) * 60 + (time_datal.RTC_Minutes - RTC_TimeStruct1.RTC_Minutes);
	}
	else
	{
		sta = (date_datal.RTC_Date - RTC_DateStruct1.RTC_Date) * 24 * 60 + (time_datal.RTC_Hours - RTC_TimeStruct1.RTC_Hours) * 60 + (time_datal.RTC_Minutes - RTC_TimeStruct1.RTC_Minutes);
	}
	if(sta > 0 && flag_alarm_on == FALSE)
	{
		LCD_ShowString(10,120,"SET!",YELLOW,GRAYBLUE,12,0);
		flag_alarm_on = TRUE;
	}
	else if(sta > 0 && flag_alarm_on == TRUE)
	{
		LCD_ShowString(10,120,"     ",YELLOW,GRAYBLUE,12,0);
		flag_alarm_on = FALSE;
		return;
	}
	else if(sta <= 0 && flag_alarm_on == FALSE)
	{
		LCD_ShowString(10,120,"ERROR",YELLOW,GRAYBLUE,12,0);
		delay_ms(2000);
		LCD_ShowString(10,120,"     ",YELLOW,GRAYBLUE,12,0);
		flag_alarm_on = FALSE;
	}
}
void ALARM_CON_ST(void)    //�ж������Ƿ�����
{
	for(i = RTC_DateStruct1.RTC_Month + 1; i <= date_datal.RTC_Month - 1; i++)
	{
		if(date_datal.RTC_Year / 4 == 0) {sum = sum + Week_Day_A2[i - 1];}
		else {sum = sum + Week_Day_A[i - 1];}
	}
	i = (date_datal.RTC_Year / 4 == 0)?(Week_Day_A2[RTC_DateStruct1.RTC_Month]):(Week_Day_A[RTC_DateStruct1.RTC_Month]);
	if(RTC_DateStruct1.RTC_Month != date_datal.RTC_Month)
	{
		sta = (sum + (date_datal.RTC_Date + i - RTC_DateStruct1.RTC_Date)) * 24 * 60 + (time_datal.RTC_Hours - RTC_TimeStruct1.RTC_Hours) * 60 + (time_datal.RTC_Minutes - RTC_TimeStruct1.RTC_Minutes);   //�����趨ʱ����ʵ��ʱ��Ĳ�
	}
	else
	{
		sta = (date_datal.RTC_Date - RTC_DateStruct1.RTC_Date) * 24 * 60 + (time_datal.RTC_Hours - RTC_TimeStruct1.RTC_Hours) * 60 + (time_datal.RTC_Minutes - RTC_TimeStruct1.RTC_Minutes);
	}
	if(sta <= 0 && flag_alarm_on == TRUE)
	{
		LCD_ShowString(10,120,"TIME!",YELLOW,GRAYBLUE,12,0);
		delay_ms(3500);
		LCD_ShowString(10,120,"      ",YELLOW,GRAYBLUE,12,0);
		flag_alarm_on = FALSE;
	}
} 
void ALARM_RESET(void)    //�����趨ʱ��
{
	flag_alarm_on = FALSE;
	LCD_ShowString(10,120,"    ",YELLOW,GRAYBLUE,16,0);
	time_datal = RTC_TimeStruct1;
	date_datal = RTC_DateStruct1;
}