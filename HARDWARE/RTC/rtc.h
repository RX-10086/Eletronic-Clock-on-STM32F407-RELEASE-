#ifndef __RTC_H
#define __RTC_H	 
#include "sys.h" 

u8 My_RTC_Init(void);						
ErrorStatus RTC_Set_Time(u8 hour,u8 min,u8 sec,u8 ampm);			
ErrorStatus RTC_Set_Date(u8 year,u8 month,u8 date,u8 week); 		
void RTC_Set_AlarmA(u8 week,u8 hour,u8 min,u8 sec);		
void RTC_Set_WakeUp(u32 wksel,u16 cnt);					


#endif


















