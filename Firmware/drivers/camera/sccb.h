#ifndef __SCCB_H
#define __SCCB_H
#include "include.h"

#define SCCB_ID   			0X42  			//OV7670的ID

//IO方向设置
#define SCCB_SDA_IN()  {GPIOB->MODER&=~(3<<(13*2));GPIOB->MODER|=0<<13*2;}		//PD3 输入
#define SCCB_SDA_OUT() {GPIOB->MODER&=~(3<<(13*2));GPIOB->MODER|=1<<13*2;}	//PD3 输出
//IO操作函数	 
#define SCCB_SCL    		PBout(12)	 	//SCL
#define SCCB_SDA    		PBout(13) 		//SDA	 
#define SCCB_READ_SDA   	PBin(13)  		//输入SDA  


///////////////////////////////////////////
void SCCB_Init(void);
void SCCB_Start(void);
void SCCB_Stop(void);
void SCCB_No_Ack(void);
u8 SCCB_WR_Byte(u8 dat);
u8 SCCB_RD_Byte(void);
u8 SCCB_WR_Reg(u8 reg,u8 data);
u8 SCCB_RD_Reg(u8 reg);
#endif













