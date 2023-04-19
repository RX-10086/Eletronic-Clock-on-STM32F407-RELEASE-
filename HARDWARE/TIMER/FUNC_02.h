#include "sys.h"
#include "lcd.h"
#include "lcd_init.h"
#include "background.h"      //背景

#define TRUE 1
#define FALSE 0

char str_output[13] = "00:00:00:000";     //输出字符串
int sec=0;
int min=0;
int hour=0;
int micro_sec = 0;

u16 STATE = 1;   //判断暂停状态

char rec[4][20] = {"rec1=00:00:00:000","rec2=00:00:00:000","rec3=00:00:00:000","rec4=00:00:00:000"};
int flag_rec = 0;

void TIME_SET_FRESH(void)    //TIM3中断发生事件，每中断一次，时间加1ms
{
	  if(micro_sec == 1000)
	  {
		  micro_sec = 0;
		  sec++;
	  }
		if(sec==60)
		{
			sec=0;
			min++;
	  }
		if(min==60)
		{
			min=0;
			hour++;
		}
		if(hour==100)
		{
			hour=0;
		}
		micro_sec++;
}

void OUTPUT(u32 num, char *str)   //将数字转为字符串
{
	u32 i = num / 10;
  u32 j = num % 10;
  *str = (char)(i + 48);
  *(str + 1) = (char)(j + 48);
}
void OUTPUT_ms(char *str)    //将毫秒数字转为字符串
{
	u32 i = micro_sec / 100;
	u32 j = (micro_sec % 100) / 10;
	u32 k = micro_sec % 10;
	*str = (char)(i + 48);
	*(str + 1) = (char)(j + 48);
	*(str + 2) = (char)(k + 48);
}
void ZERO_ms(char *str)    //将毫秒数字归零
{
	*str = (char)(48);
	*(str + 1) = (char)(48);
	*(str + 2) = (char)(48);
}
void TIME_RESET(void)    //时间复位
{
		micro_sec = 0;
	  sec = 0;
	  min = 0;
	  hour = 0;
}
void TIME_STOP(void)     //时间暂停
{
	  TIM_ITConfig(TIM3,TIM_IT_Update,(STATE == FALSE)?DISABLE:ENABLE);
		STATE = (STATE == FALSE);
}
int OUT_STATE(void)
{
	return STATE;
}
void Display_MODE_2()   //非刷新画面显示
{
	LCD_Init();
	LCD_Fill(0,0,LCD_W,LCD_H,GRAYBLUE);
  LCD_ShowString(3,10,"STOPWATCH",RED,GRAYBLUE,16,0);
	LCD_ShowPicture(3,35,120,120,gImage_0);
	LCD_ShowString(55,110,"Designed By",BLACK,GRAYBLUE,12,0);
	LCD_ShowString(55,126,"Y.W.J",BLACK,GRAYBLUE,12,0);
}
void Display_MODE_2_FRESH()    //刷新画面显示（时间的字符串）
{
	OUTPUT(sec, str_output + 6);
	OUTPUT(min, str_output + 3);
	OUTPUT(hour, str_output);
	OUTPUT_ms(str_output + 9);
	LCD_ShowString(18,30,str_output,WHITE,GRAYBLUE,16,0);
}
void record_md2(void)      //记录
{
	if(flag_rec == 4)
	{
		flag_rec = 0;
	}
	OUTPUT(sec, *(rec + flag_rec) + 11);    
	OUTPUT(min, *(rec + flag_rec)	+ 8);
	OUTPUT(hour, *(rec + flag_rec) + 5);
	OUTPUT_ms(*(rec + flag_rec) + 14);
	LCD_ShowString(18,50 + 12*flag_rec,*(rec + flag_rec),WHITE,GRAYBLUE,12,0);
	flag_rec++;
}
void record_reset(void)    //记录重置
{
	int f;
	for(f = 0;f <= 3;f++)
	{
	  OUTPUT(0, *(rec + f) + 11);
	  OUTPUT(0, *(rec + f)	+ 8);
	  OUTPUT(0, *(rec + f) + 5);
	  ZERO_ms(*(rec + f) + 14);
	}
	flag_rec = 0;
}
