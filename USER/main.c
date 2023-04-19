#include "sys.h"
#include "delay.h"
#include "usart.h"     
#include "led.h"
#include "rtc_FUNC_01.h"       //RTC相关设置与主模式
#include "timer_3_FUNC_02.h"   //TIM3相关设置与秒表模式
#include "timer_4_FUNC_03.h"   //TIM4相关设置与倒计时模式
#include "alarm.h"             //闹钟设置/模式
#include "key.h"
#include "lcd.h"
#include "lcd_init.h"
#include "Cirno.h"    //Baka----Baka! (Idiot, idiot!)
#include "rtc.h"

#define TRUE 1
#define FALSE 0

#define MODE_1 1          //主模式
#define MODE_2 2          //秒表模式
#define MODE_3 3          //倒计时模式
#define MODE_ALARM 4      //闹铃模式

u16 timer_first_start_state = FALSE;     //判断是否第一次进入秒表模式标识
u16 Count_first_start_state = FALSE;     //判断是否第一次进入倒计时模式标识
u16 Alarm_first_start_state = FALSE;

int MODE = MODE_1;       //记录模式，开机时，默认模式为主界面模式
int FRESH_STATE = 0;     //判断由某一模式进入另一模式时，是否刷新屏幕

u16 key = -1;            //记录按下哪个按键

void Display(void);      //控制显示模式
void MODE_CHG(void);     //改变模式变量

void flag_CHG(void);     //主界面模式中，切换12H/24H模式
int flag_FRESH = FALSE;    //切换12H/24H模式时，判断是否需要刷新屏幕

int flag_H12_24 = FALSE;   //记录12H/24H模式，默认为24H模式

int flag_md1 = 0;          //主模式中，存储对应要修改的变量为时、分、秒或日期的年月日/星期，同时对应屏幕光标
int flag_md3 = 0;          //倒计时模式中，存储对应要设定的变量为时、分、秒，同时对应屏幕光标
int flag_md_a = 0;         //闹钟模式中，存储对应要设定变量为时、分、月、日，同时对应屏幕光标

void ADJUST01(void);       //调整光标和存储位置的变量
void ADJUST02(void);       //同上，但是是模式3
void ADJUST_A(void);       //同上，但是时闹钟模式（下称模式4）

const int length = 24;     //光标长度，用于模式1/3
const int length_s = 16;   //同上，但是长度不同

int USART_Re = FALSE;      //记录是否打开接受串口信息模式

int main(void)             //主函数
{ 
	RTC_Set_WakeUp(RTC_WakeUpClock_CK_SPRE_16bits,0);	    
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	delay_init(168);     
	uart_init(115200);	
	LED_Init();					
	KEY_Init();
	LCD_Init();
	My_RTC_Init();
	LCD_Fill(0,0,LCD_W,LCD_H,GRAYBLUE);
	LCD_ShowPicture(0,0,128,128,gImage_Cirno);       //开机进入主界面前，显示（9）的照片
	LCD_ShowString(0,130,"Baka----Baka!",WHITE,GRAYBLUE,12,0);      //翻译:笨蛋，笨蛋！
	delay_ms(3500);      //延时3.5s，然而并没有什么卵用
	
	while(TRUE) 
	{	
		key = KEY_Scan(0, MODE);   //检测并记录按键状态
		if(key != 0)         //如果有按键输入
		{
			switch(key)
			{
				case KEY0_PRES:   //按下K0
				{
          if(MODE == MODE_1) {flag_CHG();}  
					if(MODE == MODE_2) {TIME_RESET();}
					if(MODE == MODE_3 && ADJ_JUDGE() == TRUE) {TIME_RESET_0();}
					if(MODE == MODE_ALARM) {ALARM_RESET();}     //重置闹钟时间
				  break;
				}
				case KEY1_PRES:   //按下K1
				{
					if(MODE == MODE_1)
					{
						USART_Re = (USART_Re==FALSE);
						LCD_DrawRectangle(86,1,96,11,(USART_Re == TRUE)?YELLOW:GRAYBLUE);    //是否显示串口光标
					}
					if(timer_first_start_state == FALSE && MODE == MODE_2)  //第一次启动秒表
					{
						timer_first_start_state = TRUE;
						TIM3_Int_Init(10-1, 8400-1);   //启动定时器3
					}
					if(Count_first_start_state == FALSE && MODE == MODE_3 && FRS_STOP() == TRUE)   //第一次启动倒计时
					{
						Count_first_start_state = TRUE;
						TIM4_Int_Init(10-1,8400-1);    //启动定时器4
					}
					if(MODE == MODE_ALARM)
					{
						ALARM_SET_FIN();     //设定是否启动闹钟
					}
					else
						{
					    if(MODE == MODE_2)  {TIME_STOP();}       //暂停
					    if(MODE == MODE_3 && FRS_STOP() == TRUE) {TIME_STOP_0();}   //同上，但是模式3
					  }
				  break;
				}
				case KEY2_PRES:     //按下K2
				{
					if(MODE == MODE_1)
					{
						if(flag_md1 < 4)
						  REAL_TIME_CHG(flag_md1);    //调整当前时间
						else
							REAL_DATE_CHG(flag_md1);    //调整当前日期
					}
					if(MODE == MODE_2)
						record_md2();     //记录时间
					if(MODE == MODE_3)
					  COUNT_ADJ(flag_md3);
					if(MODE == MODE_ALARM)
						PRE_SET_TIME(flag_md_a);    //设定闹钟
					break;
				}
				case KEY3_PRES:    //按下K3
				{
					if(MODE == MODE_1 && flag_H12_24 == FALSE)   
						ADJUST01();     //调整光标
					if(MODE == MODE_2 && OUT_STATE() == TRUE)
					{
						record_reset();
						Display_MODE_2();
					}
					if(MODE == MODE_3 && ADJ_JUDGE() == TRUE)
						ADJUST02();
					if(MODE == MODE_ALARM)
						ADJUST_A();
					break;
				}
				case KEY5_PRES:    //按下K5
				{
					if(MODE == MODE_1)
					{
						if(flag_md1 < 4)
						  REAL_TIME_CHG_2(flag_md1);    //调整，但是减少
						else
							REAL_DATE_CHG_2(flag_md1);
					}
					if(MODE == MODE_3)
						COUNT_ADJ_2(flag_md3);
					if(MODE == MODE_ALARM)
						PRE_SET_TIME_2(flag_md_a);
					break;
				}
				case KEY4_PRES:    //按下K4
				{
					MODE_CHG();
					break;
				}
				case WKUP_PRES:    //按下WK_UP
				{
					MODE_CHG();
				  break;
				}
			}    
		}
		else if(USART_Re == TRUE) {USART_REC_TIME();}
		Display();   //显示画面
	}
}
/********************************************************************************************************/
void Display()
{
	if(MODE == MODE_1)
	{
		if(FRESH_STATE != MODE_1)
		{
			DISPLAY_MODE();    //不更新的部分
			FRESH_STATE = MODE_1;
		}
		if(flag_H12_24 == TRUE)
		{
			flag_CHG_DISPLAY();  //模式1下改变12H/24H显示，包括AM/PM标志
		  flag_FRESH = TRUE;
		}
		if(flag_H12_24 == FALSE)
		{
			if(flag_FRESH == TRUE) {DISPLAY_MODE();}   //改回24H模式
			flag_FRESH = FALSE;
		}
		if(flag_H12_24 == FALSE) {DISPLAY_MODE__FRESH();}   //更新的部分
		if(flag_H12_24 == TRUE) {DISPLAY_MODE__FRESH_12();}   //更新的部分，但是12H模式
	}
	if(MODE == MODE_2)
	{
		if(FRESH_STATE != MODE_2) 
    {
		  Display_MODE_2();    //不更新的部分
			FRESH_STATE = MODE_2;
		}
		Display_MODE_2_FRESH();   //更新的部分
	}
	if(MODE == MODE_3)
	{
		if(FRESH_STATE != MODE_3) 
    {
		  Display_MODE_3();    //同上
			FRESH_STATE = MODE_3;
		}
		Display_MODE_3_FRESH();
	}
	if(MODE == MODE_ALARM)
	{
		if(FRESH_STATE != MODE_ALARM)
		{
			ALARM_PRE();
			FRESH_STATE = MODE_ALARM;
		}
		if(Alarm_first_start_state == FALSE)
		{
			ALARM_PRE_2();
			Alarm_first_start_state = TRUE;
		}
		if(FRESH_STATE == MODE_ALARM)
			SPE_ADJ_RTC();
		ALARM_DISPLAY();
		ALARM_CON_ST();
	}
}
void MODE_CHG()
{
	if(MODE >= 4)
	{
		MODE = MODE_1;  //标志切换回模式1
		return;
	}
	MODE++;
}
void flag_CHG()     
{
	if(flag_H12_24 == TRUE) {flag_H12_24 = FALSE;}
	else {flag_H12_24 = TRUE;}
}
void ADJUST01()
{
	flag_md1++;
	if(flag_md1 <= 4)
	{
		if(flag_md1 == 4)
	  {
			 LCD_DrawRectangle(10,16,10 + length,18,GRAYBLUE);     
		}
		else if(flag_md1 >= 2 && flag_md1 < 4)
		{
			 LCD_DrawRectangle(10 + (4 - flag_md1)*length,16,10 + (5 - flag_md1)*length,18,GRAYBLUE);   //清除之前的光标
		}
		if(flag_md1 != 0 && flag_md1 != 4)
		{
			 LCD_DrawRectangle(10 + (3 - flag_md1)*length,16,10 + (4 - flag_md1)*length,18,GREEN);      //在时间的对应部分画光标
		}
	}
	if(flag_md1 <= 8 && flag_md1 >= 4)
	{
			 if(flag_md1 == 8)
			 {
					LCD_DrawRectangle(20 + 3*length_s,31,20 + 4*length_s,33,GRAYBLUE);
					flag_md1 = 0;
					return;
			 }
				else if(flag_md1 >= 5 && flag_md1 < 8)
				{
					LCD_DrawRectangle(20 + (flag_md1 - 5)*length_s,31,20 + (flag_md1 - 4)*length_s,33,GRAYBLUE);     //清除之前的光标
				}
				if(flag_md1 != 0)
				{
					LCD_DrawRectangle(20 + (flag_md1 - 4)*length_s,31,20 + (flag_md1 - 3)*length_s,33,GREEN);       //在日期的对应部分画光标
				}
	}
}
void ADJUST02()     //与上面类似，此处不再赘述
{
	flag_md3++;
	if(flag_md3 <= 4)
	{
		if(flag_md3 == 4)
		{
			LCD_DrawRectangle(16,50,16 + length,52,GRAYBLUE);
			flag_md3 = 0;
			return;
		}
		else if(flag_md3 >= 2)
		{
			LCD_DrawRectangle(16 + (4 - flag_md3)*length,50,16 + (5 - flag_md3)*length,52,GRAYBLUE);   
		}
		if(flag_md3 != 0)
		{
			LCD_DrawRectangle(16 + (3 - flag_md3)*length,50,16 + (4 - flag_md3)*length,52,GREEN);
		}
	}
}
void ADJUST_A()     //
{
	flag_md_a++;
	switch(flag_md_a)
	{
		case 1:
	  {
			LCD_DrawLine(76,44,84,44,GREEN);
			break;
	  }
		case 2:
		{
			LCD_DrawLine(76,44,84,44,GRAYBLUE);
			LCD_DrawLine(89,44,97,44,GREEN);
			break;
		}
		case 3:
		{
			LCD_DrawLine(89,44,97,44,GRAYBLUE);
			LCD_DrawLine(76,59,84,59,GREEN);
			break;
		}
		case 4:
		{
			LCD_DrawLine(76,59,84,59,GRAYBLUE);
			LCD_DrawLine(89,59,97,59,GREEN);
			break;
		}
	}
	if(flag_md_a == 5)
	{
		LCD_DrawLine(89,59,97,59,GRAYBLUE);
		flag_md_a = 0;
		return;
	}
}
