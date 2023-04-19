#include "sys.h"
#include "delay.h"
#include "usart.h"     
#include "led.h"
#include "rtc_FUNC_01.h"       //RTC�����������ģʽ
#include "timer_3_FUNC_02.h"   //TIM3������������ģʽ
#include "timer_4_FUNC_03.h"   //TIM4��������뵹��ʱģʽ
#include "alarm.h"             //��������/ģʽ
#include "key.h"
#include "lcd.h"
#include "lcd_init.h"
#include "Cirno.h"    //Baka----Baka! (Idiot, idiot!)
#include "rtc.h"

#define TRUE 1
#define FALSE 0

#define MODE_1 1          //��ģʽ
#define MODE_2 2          //���ģʽ
#define MODE_3 3          //����ʱģʽ
#define MODE_ALARM 4      //����ģʽ

u16 timer_first_start_state = FALSE;     //�ж��Ƿ��һ�ν������ģʽ��ʶ
u16 Count_first_start_state = FALSE;     //�ж��Ƿ��һ�ν��뵹��ʱģʽ��ʶ
u16 Alarm_first_start_state = FALSE;

int MODE = MODE_1;       //��¼ģʽ������ʱ��Ĭ��ģʽΪ������ģʽ
int FRESH_STATE = 0;     //�ж���ĳһģʽ������һģʽʱ���Ƿ�ˢ����Ļ

u16 key = -1;            //��¼�����ĸ�����

void Display(void);      //������ʾģʽ
void MODE_CHG(void);     //�ı�ģʽ����

void flag_CHG(void);     //������ģʽ�У��л�12H/24Hģʽ
int flag_FRESH = FALSE;    //�л�12H/24Hģʽʱ���ж��Ƿ���Ҫˢ����Ļ

int flag_H12_24 = FALSE;   //��¼12H/24Hģʽ��Ĭ��Ϊ24Hģʽ

int flag_md1 = 0;          //��ģʽ�У��洢��ӦҪ�޸ĵı���Ϊʱ���֡�������ڵ�������/���ڣ�ͬʱ��Ӧ��Ļ���
int flag_md3 = 0;          //����ʱģʽ�У��洢��ӦҪ�趨�ı���Ϊʱ���֡��룬ͬʱ��Ӧ��Ļ���
int flag_md_a = 0;         //����ģʽ�У��洢��ӦҪ�趨����Ϊʱ���֡��¡��գ�ͬʱ��Ӧ��Ļ���

void ADJUST01(void);       //�������ʹ洢λ�õı���
void ADJUST02(void);       //ͬ�ϣ�������ģʽ3
void ADJUST_A(void);       //ͬ�ϣ�����ʱ����ģʽ���³�ģʽ4��

const int length = 24;     //��곤�ȣ�����ģʽ1/3
const int length_s = 16;   //ͬ�ϣ����ǳ��Ȳ�ͬ

int USART_Re = FALSE;      //��¼�Ƿ�򿪽��ܴ�����Ϣģʽ

int main(void)             //������
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
	LCD_ShowPicture(0,0,128,128,gImage_Cirno);       //��������������ǰ����ʾ��9������Ƭ
	LCD_ShowString(0,130,"Baka----Baka!",WHITE,GRAYBLUE,12,0);      //����:������������
	delay_ms(3500);      //��ʱ3.5s��Ȼ����û��ʲô����
	
	while(TRUE) 
	{	
		key = KEY_Scan(0, MODE);   //��Ⲣ��¼����״̬
		if(key != 0)         //����а�������
		{
			switch(key)
			{
				case KEY0_PRES:   //����K0
				{
          if(MODE == MODE_1) {flag_CHG();}  
					if(MODE == MODE_2) {TIME_RESET();}
					if(MODE == MODE_3 && ADJ_JUDGE() == TRUE) {TIME_RESET_0();}
					if(MODE == MODE_ALARM) {ALARM_RESET();}     //��������ʱ��
				  break;
				}
				case KEY1_PRES:   //����K1
				{
					if(MODE == MODE_1)
					{
						USART_Re = (USART_Re==FALSE);
						LCD_DrawRectangle(86,1,96,11,(USART_Re == TRUE)?YELLOW:GRAYBLUE);    //�Ƿ���ʾ���ڹ��
					}
					if(timer_first_start_state == FALSE && MODE == MODE_2)  //��һ���������
					{
						timer_first_start_state = TRUE;
						TIM3_Int_Init(10-1, 8400-1);   //������ʱ��3
					}
					if(Count_first_start_state == FALSE && MODE == MODE_3 && FRS_STOP() == TRUE)   //��һ����������ʱ
					{
						Count_first_start_state = TRUE;
						TIM4_Int_Init(10-1,8400-1);    //������ʱ��4
					}
					if(MODE == MODE_ALARM)
					{
						ALARM_SET_FIN();     //�趨�Ƿ���������
					}
					else
						{
					    if(MODE == MODE_2)  {TIME_STOP();}       //��ͣ
					    if(MODE == MODE_3 && FRS_STOP() == TRUE) {TIME_STOP_0();}   //ͬ�ϣ�����ģʽ3
					  }
				  break;
				}
				case KEY2_PRES:     //����K2
				{
					if(MODE == MODE_1)
					{
						if(flag_md1 < 4)
						  REAL_TIME_CHG(flag_md1);    //������ǰʱ��
						else
							REAL_DATE_CHG(flag_md1);    //������ǰ����
					}
					if(MODE == MODE_2)
						record_md2();     //��¼ʱ��
					if(MODE == MODE_3)
					  COUNT_ADJ(flag_md3);
					if(MODE == MODE_ALARM)
						PRE_SET_TIME(flag_md_a);    //�趨����
					break;
				}
				case KEY3_PRES:    //����K3
				{
					if(MODE == MODE_1 && flag_H12_24 == FALSE)   
						ADJUST01();     //�������
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
				case KEY5_PRES:    //����K5
				{
					if(MODE == MODE_1)
					{
						if(flag_md1 < 4)
						  REAL_TIME_CHG_2(flag_md1);    //���������Ǽ���
						else
							REAL_DATE_CHG_2(flag_md1);
					}
					if(MODE == MODE_3)
						COUNT_ADJ_2(flag_md3);
					if(MODE == MODE_ALARM)
						PRE_SET_TIME_2(flag_md_a);
					break;
				}
				case KEY4_PRES:    //����K4
				{
					MODE_CHG();
					break;
				}
				case WKUP_PRES:    //����WK_UP
				{
					MODE_CHG();
				  break;
				}
			}    
		}
		else if(USART_Re == TRUE) {USART_REC_TIME();}
		Display();   //��ʾ����
	}
}
/********************************************************************************************************/
void Display()
{
	if(MODE == MODE_1)
	{
		if(FRESH_STATE != MODE_1)
		{
			DISPLAY_MODE();    //�����µĲ���
			FRESH_STATE = MODE_1;
		}
		if(flag_H12_24 == TRUE)
		{
			flag_CHG_DISPLAY();  //ģʽ1�¸ı�12H/24H��ʾ������AM/PM��־
		  flag_FRESH = TRUE;
		}
		if(flag_H12_24 == FALSE)
		{
			if(flag_FRESH == TRUE) {DISPLAY_MODE();}   //�Ļ�24Hģʽ
			flag_FRESH = FALSE;
		}
		if(flag_H12_24 == FALSE) {DISPLAY_MODE__FRESH();}   //���µĲ���
		if(flag_H12_24 == TRUE) {DISPLAY_MODE__FRESH_12();}   //���µĲ��֣�����12Hģʽ
	}
	if(MODE == MODE_2)
	{
		if(FRESH_STATE != MODE_2) 
    {
		  Display_MODE_2();    //�����µĲ���
			FRESH_STATE = MODE_2;
		}
		Display_MODE_2_FRESH();   //���µĲ���
	}
	if(MODE == MODE_3)
	{
		if(FRESH_STATE != MODE_3) 
    {
		  Display_MODE_3();    //ͬ��
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
		MODE = MODE_1;  //��־�л���ģʽ1
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
			 LCD_DrawRectangle(10 + (4 - flag_md1)*length,16,10 + (5 - flag_md1)*length,18,GRAYBLUE);   //���֮ǰ�Ĺ��
		}
		if(flag_md1 != 0 && flag_md1 != 4)
		{
			 LCD_DrawRectangle(10 + (3 - flag_md1)*length,16,10 + (4 - flag_md1)*length,18,GREEN);      //��ʱ��Ķ�Ӧ���ֻ����
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
					LCD_DrawRectangle(20 + (flag_md1 - 5)*length_s,31,20 + (flag_md1 - 4)*length_s,33,GRAYBLUE);     //���֮ǰ�Ĺ��
				}
				if(flag_md1 != 0)
				{
					LCD_DrawRectangle(20 + (flag_md1 - 4)*length_s,31,20 + (flag_md1 - 3)*length_s,33,GREEN);       //�����ڵĶ�Ӧ���ֻ����
				}
	}
}
void ADJUST02()     //���������ƣ��˴�����׸��
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
