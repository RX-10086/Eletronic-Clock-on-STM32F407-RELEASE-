#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h" 
 

/*����ķ�ʽ��ͨ��ֱ�Ӳ����⺯����ʽ��ȡIO*/
#define KEY0 		GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4) //PE4
#define KEY1 		GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_3)	//PE3 
#define KEY2 		GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_2) //PE2
#define WK_UP 	GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)	//PA0
#define KEY3    GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_5) //PE5
#define KEY4    GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_6) //PE6
#define KEY5    GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_7) //PE7
//��������û��KEY2������KEY2�������PE2�����
//KEY3 KEY4 ��ͬ��
/*���淽ʽ��ͨ��λ��������ʽ��ȡIO*/
/*
#define KEY0 		PEin(4)   	//PE4
#define KEY1 		PEin(3)		//PE3 
#define KEY2 		PEin(2)		//PE2
#define WK_UP 	PAin(0)		//PA0
*/

#define KEY0_PRES 	1
#define KEY1_PRES	  2
#define KEY2_PRES	  3
#define WKUP_PRES   4
#define KEY3_PRES   5
#define KEY4_PRES   6
#define KEY5_PRES   7

void KEY_Init(void);	//IO��ʼ��
u8 KEY_Scan(u8, u16);  		//����ɨ�躯��	

#endif
