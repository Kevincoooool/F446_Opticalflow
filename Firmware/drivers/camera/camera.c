#include "include.h"
#include "camera.h" 
#include "ov7670cfg.h"	
#include "ov7740cfg.h"

//初始化OV7670
//返回0:成功
//返回其他值:错误代码
u16 reg=0;
u8 Camera_Init(void)
{
	
	
	//设置IO   	   
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; //推挽输出
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; //推挽输出
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化

 	OV_PWDN=0;
	delay_ms(10);
	OV_RST=0;	//复位
	delay_ms(10);
	OV_RST=1;	//结束复位 
	
	SCCB_Init();        			//初始化SCCB 的IO口
 	SCCB_WR_Reg(0X12, 0x80);	//软复位OV7670
	delay_ms(50);
	
	reg=SCCB_RD_Reg(0X0a);	//读取厂家ID 高八位
	reg<<=8;
	reg|=SCCB_RD_Reg(0X0b);	//读取厂家ID 低八位
	
	if(reg==OV7670_PID)
	{
		OV7670_Init();
		return 0;
	}   
	else if(reg==OV7740_PID)
	{
		OV7740_Init();
		return 0;
	}
	
  return 0x02;
} 


