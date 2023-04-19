#include "sys.h"
#include "lcd.h"
#include "lcd_init.h"
#include "Remilia.h"    //蕾米莉亚————威严满满

#define TRUE 1
#define FALSE 0

u16 STATE_0 = TRUE;       //判断暂停状态

char str_output_0[13]="00:00:00:000";
int sec_0 = 0;
int min_0 = 0;
int hour_0 = 0;
int micro_sec_0 = 0;

void TIME_COUNT_FRESH(void)    //TIM4中断事件
{
	  if(micro_sec_0 < 0)
	  {
		  micro_sec_0 = 999;
		  sec_0--;
	  }
		if(sec_0 < 0)
		{
			sec_0 = 59;
			min_0--;
	  }
		if(min_0 < 0)
		{
			min_0 = 59;
			hour_0--;
		}
		if(micro_sec_0 == 0 && sec_0 == 0 && min_0 == 0 && hour_0 == 0)    //倒计时到零时，TIM4停止工作
	  {
		  TIM_ITConfig(TIM4,TIM_IT_Update,DISABLE);
		  STATE_0 = TRUE;
			return;
	  }
		micro_sec_0--;
}
void TIME_RESET_0(void)      //复位或提前结束倒计时
{
		micro_sec_0 = 0;
	  sec_0 = 0;
	  min_0 = 0;
	  hour_0 = 0;
}
void TIME_STOP_0(void)       //倒计时暂停
{
	  TIM_ITConfig(TIM4,TIM_IT_Update,(STATE_0 == FALSE)?DISABLE:ENABLE);     
		STATE_0 = (STATE_0 == FALSE);
}
/*int OUTPUT_STOP(void)       
{
	return STATE_0;
}*/
int FRS_STOP(void)       //时间不为0时，倒计时才会启动
{
	return (micro_sec_0 == 0 && sec_0 == 0 && min_0 == 0 && hour_0 == 0)?FALSE:TRUE;
}
void OUTPUT_0(u32 num, char *str)       //数字转为字符串
{
	u32 i = num / 10;
  u32 j = num % 10;
  *str = (char)(i + 48);
  *(str + 1) = (char)(j + 48);
}
void OUTPUT_ms_C(char *str)     //毫秒数字转为字符串
{
	u32 i = micro_sec_0 / 100;
	u32 j = (micro_sec_0 % 100) / 10;
	u32 k = micro_sec_0 % 10;
	*str = (char)(i + 48);
	*(str + 1) = (char)(j + 48);
	*(str + 2) = (char)(k + 48);
}
void Display_MODE_3()
{
	LCD_Init();
	LCD_Fill(0,0,LCD_W,LCD_H,GRAYBLUE);
	LCD_ShowString(3,10,"COUNTING",RED,GRAYBLUE,16,0);
	LCD_ShowPicture(10,55,100,100,gImage_Remilia);    //显示大小姐的照片
}
void Display_MODE_3_FRESH()
{
	OUTPUT_0(sec_0, str_output_0 + 6);
	OUTPUT_0(min_0, str_output_0	+ 3);
	OUTPUT_0(hour_0, str_output_0);
	OUTPUT_ms_C(str_output_0 + 9);
	LCD_ShowString(18,30,str_output_0,WHITE,GRAYBLUE,16,0);
}
int ADJ_JUDGE(void)     //输出STATE_0数值，让主函数判断定时器状态 
{
	return STATE_0;
}
void COUNT_ADJ(int adj_flag)        //调整数据，增加
{
	switch(adj_flag)
	{
		case 1:
	  {
		  sec_0++;
	    if(sec_0 == 60) {sec_0 = 0;}
			break;
    }
	  case 2:
	  {
		  min_0++;
	    if(min_0 == 60) {min_0 = 0;}
			break;
	  }
		case 3:
	  {
	    hour_0++;
	    if(hour_0 == 100) {hour_0 = 0;}
			break;
    }
  }
}
void COUNT_ADJ_2(int adj_flag)     //减少
{
	switch(adj_flag)
	{
		case 1:
	  {
		  sec_0--;
	    if(sec_0 < 0) {sec_0 = 59;}
			break;
    }
	  case 2:
	  {
		  min_0--;
	    if(min_0 < 0) {min_0 = 59;}
			break;
	  }
		case 3:
	  {
	    hour_0--;
	    if(hour_0 < 0) {hour_0 = 99;}
			break;
    }
  }
}
