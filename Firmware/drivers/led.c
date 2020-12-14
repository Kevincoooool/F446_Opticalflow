#include "led.h" 
#include "time.h"

void LED_Init(void)
{    	 
  GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(LED_RCC, ENABLE);//使能GPIO时钟

  GPIO_InitStructure.GPIO_Pin = LED_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;                  //普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;                 //推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;             //100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;                   //上拉
  GPIO_Init(LED_GPIO, &GPIO_InitStructure);                         //初始化GPIO
	
	GPIO_SetBits(LED_GPIO, LED_PIN);   
}

void LED_Flash(void)
{
	for(u8 i=0;i<3;i++)
	{
		LED0 = 0;
		delay_ms(50);
		LED0 = 1;
		delay_ms(50);
	}
}




