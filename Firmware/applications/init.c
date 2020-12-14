
#include "include.h"

u8 *SendBuff;	//发送数据缓冲区
u8 All_Init()
{
	NVIC_PriorityGroupConfig(NVIC_GROUP);
	SysTick_Configuration(); 	//滴答时钟
//	Usb_Hid_Init();						//飞控usb接口的hid初始化
	USART2_init(1000000);			//初始化串口1
	LED_Init();								//初始化LED灯
	LED_Flash();							//LED闪烁
// 	LCD_Init();								//初始化TFT屏幕
	 
// 	KEY_Init();								//初始化按键
	TIM1_PWM_Init(7-1,1-1);		//XCLK 24Mhz
//	I2c_Soft_Init();					//软件iic初始化
//	MPU6050_Init(20);					//mpu6050初始化
//	_VL53L0X_I2c_Soft_Init(); //VL53L0X软件IIC初始化
//	Drv_Vl53_Init();					//VL53L0X初始化
//	ANO_Param_Read(); 				//读取参数 
	
// 	POINT_COLOR=RED;					//设置系统字体颜色为红色
	while( Camera_Init() ); //初始化摄像头
	delay_ms(200);
	Lcd_Init();
	LCD_Clear(BLACK);
	My_DCMI_Init();
	DCMI_DMA_Init((u32)&Image1,buf_size,DMA_MemoryDataSize_Word,DMA_MemoryInc_Enable);//

	DCMI_Start();
	//MYDMA_Config(DMA1_Stream6,DMA_Channel_4,(u32)&USART2->DR,(u32)SendBuff,OV7740_SIZE);
	MYDMA_Config(DMA2_Stream3,DMA_Channel_3,(u32)&SPI1->DR,(u32)SendBuff,1);
	MYDMA_Config(DMA2_Stream2,DMA_Channel_3,(u32)&SPI1->DR,(u32)SendBuff,1);
	Flow_Param_Init();       //光流参数初始化
	
 	return (1);
}
/******************* (C) COPYRIGHT 2014 ANO TECH *****END OF FILE************/
