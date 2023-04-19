#ifndef __TIMER_4_FUNC_03_H
#define __TIMER_4_FUNC_03_H
#include "sys.h"

void TIM4_Int_Init(u32 arr,u16 psc);
void TIME_RESET_0(void);
void TIME_STOP_0(void);
int FRS_STOP(void);
void Display_MODE_3(void);
void Display_MODE_3_FRESH(void);
int ADJ_JUDGE(void);
void COUNT_ADJ(int adj_flag);
void COUNT_ADJ_2(int adj_flag);

#endif

