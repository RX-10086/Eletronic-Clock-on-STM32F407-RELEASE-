#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h" 
 

/*下面的方式是通过直接操作库函数方式读取IO*/
#define KEY0 		GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4) //PE4
#define KEY1 		GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_3)	//PE3 
#define KEY2 		GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_2) //PE2
#define WK_UP 	GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)	//PA0
#define KEY3    GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_5) //PE5
#define KEY4    GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_6) //PE6
#define KEY5    GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_7) //PE7
//板子上面没有KEY2，所以KEY2是外接在PE2上面的
//KEY3 KEY4 等同理
/*下面方式是通过位带操作方式读取IO*/
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

void KEY_Init(void);	//IO初始化
u8 KEY_Scan(u8, u16);  		//按键扫描函数	

#endif
