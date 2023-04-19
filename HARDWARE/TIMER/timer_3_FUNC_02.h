#ifndef __TIMER_3_FUNC_02_H
#define __TIMER_3_FUNC_02_H
#include "sys.h"

void TIM3_Int_Init(u16 arr,u16 psc);
void TIME_RESET(void);
void TIME_STOP(void);
int OUT_STATE(void);
void Display_MODE_2(void);
void Display_MODE_2_FRESH(void);
void record_md2(void);
void record_reset(void);

#endif


