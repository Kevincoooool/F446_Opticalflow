#ifndef __KEY_H
#define __KEY_H	 
#include "include.h"

/*下面的方式是通过直接操作库函数方式读取IO*/
#define KEY0 		GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5)

#define Delay_Time 5
extern u8 key_time,key_cnt;

void KEY_Init(void);	//IO初始化

#endif
