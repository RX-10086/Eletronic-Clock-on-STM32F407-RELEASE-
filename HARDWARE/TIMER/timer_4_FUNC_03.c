#include "FUNC_03.h"
#include "timer_4_FUNC_03.h"

//����ʹ�õ��Ƕ�ʱ��2��
void TIM4_Int_Init(u32 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE); 

	TIM_TimeBaseInitStructure.TIM_Period = arr; 
	TIM_TimeBaseInitStructure.TIM_Prescaler = psc; 
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStructure);
	
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);
	TIM_Cmd(TIM4,ENABLE); 
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn; //��ʱ��4�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02; //��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x04; //�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}
//��ʱ��4�жϷ�����
void TIM4_IRQHandler()
{
	if(TIM_GetITStatus(TIM4,TIM_IT_Update)==SET) //����ж�
	{
     TIME_COUNT_FRESH();
	   TIM_ClearITPendingBit(TIM4,TIM_IT_Update);  //����жϱ�־λ
  }
}

