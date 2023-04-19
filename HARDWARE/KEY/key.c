#include "key.h"
#include "delay.h" 

//������ʼ������
void KEY_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOE, ENABLE);//ʹ��GPIOA,GPIOEʱ��
 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7; //KEY0 KEY1 KEY2 KEY3�ȶ�Ӧ����
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//��ͨ����ģʽ
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(GPIOE, &GPIO_InitStructure);//��ʼ��GPIOE2,3,4,5,6,7
	
	 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;//WK_UP��Ӧ����PA0
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN ;//����
  GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA0
 
} 
//����������
//���ذ���ֵ
//mode:0,��֧��������;1,֧��������;
//0��û���κΰ�������
//1��KEY0����
//2��KEY1����
//3��KEY2���� 
//4��WKUP���� WK_UP
//ע��˺�������Ӧ���ȼ�,KEY0>KEY1>KEY2>WK_UP!!
u8 KEY_Scan(u8 mode, u16 md)    
{	 
	static u8 key_up = 1;//�������ɿ���־
	if(mode)key_up = 1;  //֧������		  
	if(key_up&&(KEY0 == 0||KEY1 == 0||KEY2 == 0||KEY3 == 0||KEY4 == 0||KEY5 == 0||WK_UP==1))
	{
		delay_ms(10);//ȥ���� 
		key_up = 0;
		if(KEY0 == 0)return 1;
		else if(KEY1 == 0)return 2;
		else if(KEY2 == 0){if(md == 1 || md == 3 || md == 4){key_up = 1;delay_ms(150);}return 3;} //K2��ģʽ1��3��4����¿ɳ���
		else if(WK_UP == 1)return 4;
		else if(KEY3 == 0)return 5;
		else if(KEY4 == 0)return 6;
		else if(KEY5 == 0){if(md == 1 || md == 3 || md == 4){key_up = 1;delay_ms(150);}return 7;} //K5ͬK2
	}else if(KEY0 == 1 && KEY1 == 1 && KEY2 == 1 && KEY3 == 1 && KEY4 == 1 && KEY5 == 1 && WK_UP == 0)key_up = 1; 	    
 	return 0;// �ް�������
}

