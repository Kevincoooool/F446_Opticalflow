#ifndef __LED_H
#define __LED_H

#include "include.h"

//LED端口定义
#define LED_RCC  RCC_AHB1Periph_GPIOB
#define LED_GPIO GPIOB
#define LED_PIN  GPIO_Pin_14
#define LED0 PBout(14)	  


void LED_Init(void);//初始化	
void LED_Flash(void);
#endif
