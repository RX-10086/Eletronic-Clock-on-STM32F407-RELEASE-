#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "rtc.h"
#include "lcd.h"
#include "lcd_init.h"
#include "Reimu.h"
#include "Marisa.h"

#define TRUE 1
#define FALSE 0

RTC_TimeTypeDef RTC_TimeStruct;     //RTC时间
RTC_DateTypeDef RTC_DateStruct;     //RTC日期

RTC_TimeTypeDef time_data;    //存储时间
RTC_DateTypeDef date_data;    //存储日期

u8 tbuf_1[20];       //24H显示模式
u8 tbuf_2[20];       //显示日期

char WEEK[7][4] = {"MON","TUE","WEN","THU","FRI","SAT","SUN"};      //输出星期（以英语缩写形式）

int Week_Day[12] = {31,28,31,30,31,30,31,31,30,31,30,31};
int Week_Day_2[12] = {31,29,31,30,31,30,31,31,30,31,30,31};

void DISPLAY_MODE()   //主界面显示
{
	LCD_Init();
	LCD_Fill(0,0,LCD_W,LCD_H,GRAYBLUE);
	LCD_ShowPicture(2,36,120,120,gImage_Reimu);     //显示”城管“的照片
}
void DISPLAY_MODE__FRESH()     //时间、日期显示
{
	RTC_GetTime(RTC_Format_BIN,&RTC_TimeStruct);
	sprintf((char*)tbuf_1,"%02d:%02d:%02d",RTC_TimeStruct.RTC_Hours,RTC_TimeStruct.RTC_Minutes,RTC_TimeStruct.RTC_Seconds); 
	LCD_ShowString(10,1,tbuf_1,WHITE,GRAYBLUE,16,0);
	RTC_GetDate(RTC_Format_BIN, &RTC_DateStruct);
	sprintf((char*)tbuf_2,"20%02d-%02d-%02d",RTC_DateStruct.RTC_Year,RTC_DateStruct.RTC_Month,RTC_DateStruct.RTC_Date); 
	LCD_ShowString(10,21,tbuf_2,WHITE,GRAYBLUE,12,0); 
	LCD_ShowString(80,21,WEEK[RTC_DateStruct.RTC_WeekDay - 1],YELLOW,GRAYBLUE,12,0);
}
void flag_CHG_DISPLAY()        //切换至12H模式时，显示AM/PM标志
{
	LCD_DrawRectangle(96,1,116,21,RED);
	LCD_ShowString(98,2,(RTC_TimeStruct.RTC_Hours <= 12)?"AM":"PM",RED,GRAYBLUE,16,0);
	LCD_ShowPicture(2,36,120,120,gImage_Marisa);     //魔理沙偷走了重要的12H（doge）
}
void DISPLAY_MODE__FRESH_12()  //时间、日期显示，时间以12H模式显示
{
	RTC_GetTime(RTC_Format_BIN,&RTC_TimeStruct);
	if(RTC_TimeStruct.RTC_Hours > 12)
	{
		sprintf((char*)tbuf_1,"%02d:%02d:%02d",RTC_TimeStruct.RTC_Hours - 12,RTC_TimeStruct.RTC_Minutes,RTC_TimeStruct.RTC_Seconds); 
	}
	else
	{
		sprintf((char*)tbuf_1,"%02d:%02d:%02d",RTC_TimeStruct.RTC_Hours,RTC_TimeStruct.RTC_Minutes,RTC_TimeStruct.RTC_Seconds); 
	}
	LCD_ShowString(10,1,tbuf_1,WHITE,GRAYBLUE,16,0);
	RTC_GetDate(RTC_Format_BIN, &RTC_DateStruct);
	sprintf((char*)tbuf_2,"20%02d-%02d-%02d",RTC_DateStruct.RTC_Year,RTC_DateStruct.RTC_Month,RTC_DateStruct.RTC_Date); 
	LCD_ShowString(10,20,tbuf_2,WHITE,GRAYBLUE,12,0); 
	LCD_ShowString(80,20,WEEK[RTC_DateStruct.RTC_WeekDay - 1],YELLOW,GRAYBLUE,12,0);
}
void REAL_TIME_CHG(int rtc_flag_adj)   //修改当前时间（增加）
{
	time_data = RTC_TimeStruct;
	switch(rtc_flag_adj)
	{
		case 3:
		{
			time_data.RTC_Hours++;
			if(time_data.RTC_Hours == 24){time_data.RTC_Hours = 0;}
			break;
		}
		case 2:
		{
			time_data.RTC_Minutes++;
			if(time_data.RTC_Minutes == 60){time_data.RTC_Minutes = 0;}
			break;
		}
		case 1:
		{
			time_data.RTC_Seconds = 0;
			break;
		}
	}
	RTC_Set_Time(time_data.RTC_Hours,time_data.RTC_Minutes,time_data.RTC_Seconds,RTC_H12_AM);
}
void REAL_TIME_CHG_2(int rtc_flag_adj)   //减少
{
	time_data = RTC_TimeStruct;
	switch(rtc_flag_adj)
	{
		case 3:
		{
			if(time_data.RTC_Hours > 0){time_data.RTC_Hours--;}
			else {time_data.RTC_Hours = 23;}
			break;
		}
		case 2:
		{
			if(time_data.RTC_Minutes > 0){time_data.RTC_Minutes--;}
			else {time_data.RTC_Minutes = 59;}
			break;
		}
		case 1:
		{
			time_data.RTC_Seconds = 0;
			break;
		}
	}
	RTC_Set_Time(time_data.RTC_Hours,time_data.RTC_Minutes,time_data.RTC_Seconds,RTC_H12_AM);
}
void REAL_DATE_CHG(int rtc_flag_adj)    //修改日期（增加）
{
	date_data = RTC_DateStruct;
	switch(rtc_flag_adj)
	{
		case 4:
		{
			date_data.RTC_Year++;
			if(date_data.RTC_Year == 99){date_data.RTC_Year = 0;}
			break;
		}
		case 5:
		{
			date_data.RTC_Month++;
			if(date_data.RTC_Month == 13){date_data.RTC_Month = 1;}
			break;
		}
		case 6:
		{
			date_data.RTC_Date++;
			if(date_data.RTC_Year / 4 == 0)
			{
        if(date_data.RTC_Date == Week_Day_2[date_data.RTC_Month - 1] + 1){date_data.RTC_Date = 1;}
		  }
			else
			{
				if(date_data.RTC_Date == Week_Day[date_data.RTC_Month - 1] + 1){date_data.RTC_Date = 1;}
			}
			break;
		}
		case 7:
		{
			date_data.RTC_WeekDay++;
			if(date_data.RTC_WeekDay == 8){date_data.RTC_WeekDay = 1;}
			break;
		}
	}
	if(date_data.RTC_Year / 4 == 0)
	{
    if(date_data.RTC_Date > Week_Day_2[date_data.RTC_Month - 1]){date_data.RTC_Date = Week_Day_2[date_data.RTC_Month - 1];}
	}
	else
	{
		if(date_data.RTC_Date > Week_Day[date_data.RTC_Month - 1]){date_data.RTC_Date = Week_Day[date_data.RTC_Month - 1];}
	}
	RTC_Set_Date(date_data.RTC_Year,date_data.RTC_Month,date_data.RTC_Date,date_data.RTC_WeekDay);
}
void REAL_DATE_CHG_2(int rtc_flag_adj)      //减少
{
	date_data = RTC_DateStruct;
	switch(rtc_flag_adj)
	{
		case 4:
		{
			if(date_data.RTC_Year > 0){date_data.RTC_Year--;}
			else {date_data.RTC_Year = 99;}
			break;
		}
		case 5:
		{
			if(date_data.RTC_Month > 1){date_data.RTC_Month--;}
			else {date_data.RTC_Month = 12;}
			break;
		}
		case 6:
		{
			if(date_data.RTC_Year / 4 == 0)
			{
        if(date_data.RTC_Date > 1) {date_data.RTC_Date--;}
        else {date_data.RTC_Date = Week_Day_2[date_data.RTC_Month - 1];}
		  }
			else
			{
				if(date_data.RTC_Date > 1) {date_data.RTC_Date--;}
        else {date_data.RTC_Date = Week_Day[date_data.RTC_Month - 1];}
			}
			break;
		}
		case 7:
		{
			if(date_data.RTC_WeekDay > 0){date_data.RTC_WeekDay--;}
			else {date_data.RTC_WeekDay = 7;}
			break;
		}
	}
	if(date_data.RTC_Year / 4 == 0)
	{
    if(date_data.RTC_Date > Week_Day_2[date_data.RTC_Month - 1]){date_data.RTC_Date = Week_Day_2[date_data.RTC_Month - 1];}
	}
	else
	{
		if(date_data.RTC_Date > Week_Day[date_data.RTC_Month - 1]){date_data.RTC_Date = Week_Day[date_data.RTC_Month - 1];}
	}
	RTC_Set_Date(date_data.RTC_Year,date_data.RTC_Month,date_data.RTC_Date,date_data.RTC_WeekDay);
}
void USART_REC_TIME()     //通过串口修改当前时间、日期
{
	  u16 usart_len;
	  int i, j, k, l, m, n, q, ju_01, ju_02;
	  const char *JD1 = "TIME:";     //"TIME:XX:XX:XX"
	  const char *JD2 = "DATE:";     //"DATE:XX-XX-XX-X"
    u16 ch = 0;
	  time_data = RTC_TimeStruct;
	  date_data = RTC_DateStruct;
	  if(USART_RX_STA&0x8000)
		{
			u16 t = 0;
			for(t = 0;t < 4;t++)
			{
				if(*(USART_RX_BUF + t) != *(JD1 + t) && *(USART_RX_BUF + t) != *(JD2 + t))
				{
					printf("\r\nERROR!\r\n");
					delay_ms(50);
					return;
				}
			}
			ju_01 = TRUE;
		  for(t = 0;t < 4;t++)
			{
				if(*(USART_RX_BUF + t) != *(JD1 + t))
				{
					ju_01 = FALSE;
					break;
				}
			}
			ju_02 = TRUE;
			for(t = 0;t < 4;t++)
			{
				if(*(USART_RX_BUF + t) != *(JD2 + t))
				{
					ju_02 = FALSE;
					break;
				}
			}
      if(ju_01 == TRUE)
			{				
			  usart_len = USART_RX_STA&0x3fff;  			//得到此次接收到的数据长度
		    i = (int)(*(USART_RX_BUF + 5)) - 48, j = (int)(*(USART_RX_BUF + 6)) - 48;        //字符串转数字，下面同理
        time_data.RTC_Hours = i * 10 + j;
        k = (int)(*(USART_RX_BUF + 8)) - 48, l = (int)(*(USART_RX_BUF + 9)) - 48;
        time_data.RTC_Minutes = k * 10 + l;
        m = (int)(*(USART_RX_BUF + 11)) - 48, n = (int)(*(USART_RX_BUF + 12)) - 48;
        time_data.RTC_Seconds = m * 10 + n;
			  RTC_Set_Time(time_data.RTC_Hours,time_data.RTC_Minutes,time_data.RTC_Seconds,RTC_H12_AM);
			}
			if(ju_02 == TRUE)
			{
        usart_len = USART_RX_STA&0x3fff;
        i = (int)(*(USART_RX_BUF + 5)) - 48, j = (int)(*(USART_RX_BUF + 6)) - 48;   
        date_data.RTC_Year = i * 10 + j;
        k = (int)(*(USART_RX_BUF + 8)) - 48, l = (int)(*(USART_RX_BUF + 9)) - 48;
        date_data.RTC_Month = k * 10 + l;
        m = (int)(*(USART_RX_BUF + 11)) - 48, n = (int)(*(USART_RX_BUF + 12)) - 48;
        date_data.RTC_Date = m * 10 + n;
			  q = (int)(*(USART_RX_BUF + 14)) - 48;
			  date_data.RTC_WeekDay = q;
			  RTC_Set_Date(date_data.RTC_Year,date_data.RTC_Month,date_data.RTC_Date,date_data.RTC_WeekDay);
			}
		}
		USART_RX_STA = 0;
    delay_ms(50);		
}