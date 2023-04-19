#ifndef __RTC_FUNC_01_H
#define __RTC_FUNC_01_H
#include "sys.h"

void DISPLAY_MODE(void);
void DISPLAY_MODE__FRESH(void);
void flag_CHG_DISPLAY(void);
void DISPLAY_MODE__FRESH_12(void);
void USART_REC_TIME(void);
void REAL_TIME_CHG(int rtc_flag_adj);
void REAL_DATE_CHG(int rtc_flag_adj);
void REAL_TIME_CHG_2(int rtc_flag_adj);
void REAL_DATE_CHG_2(int rtc_flag_adj);
#endif

