#ifndef __KEY_H
#define __KEY_H	 
#include "include.h"

/*����ķ�ʽ��ͨ��ֱ�Ӳ����⺯����ʽ��ȡIO*/
#define KEY0 		GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5)

#define Delay_Time 5
extern u8 key_time,key_cnt;

void KEY_Init(void);	//IO��ʼ��

#endif
