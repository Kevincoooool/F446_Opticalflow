#ifndef _TIME_H_
#define _TIME_H_

#include "stm32f4xx.h"

void delay_us(uint32_t);
void delay_ms(uint32_t);
void SysTick_Configuration(void);
void SysTick_IRQ(void);
uint32_t GetSysTime_us(void);
#endif
