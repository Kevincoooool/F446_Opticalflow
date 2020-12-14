
#include "include.h"

loop_t loop;

void Duty_2ms()
{
//	MPU6050_Data_Prepare(); //6050传感器数据准备
}

void Duty_6ms()
{
//	MY_IMU_update(); //姿态解算
}

void Duty_10ms()
{
//	ANO_DT_Data_Exchange();	//通过USB_HID发送数据到上位机
}

void Duty_20ms()
{
//	Usart_Send_Flow(); //串口1发送光流数据
	
}

void Duty_50ms()
{
//	Drv_Vl53_RunTask(); //读取激光测距数据
}

u8 seting=0;

void Duty_1000ms()
{
	//计算图像处理频率
	frame=ov_frame;
	ov_frame=0;
	
	//参数超时保存
	Param_Save_Over_Time();
	
	//检测5s内按键次数
	key_time++;
	if(key_time==Delay_Time)//5秒内按键次数
	{
		seting = 1;
	}
	else if(key_time>=Delay_Time) key_time = Delay_Time + 1;
}

void Loop_check()
{
	loop.cnt_2ms++;
	loop.cnt_6ms++;
	loop.cnt_10ms++;
	loop.cnt_20ms++;
	loop.cnt_50ms++;
	loop.cnt_1000ms++;

	if( loop.cnt_2ms >= 2 )
	{
		loop.cnt_2ms = 0;
		Duty_2ms();						//周期2ms的任务
	}
	if( loop.cnt_6ms >= 6 )
	{
		loop.cnt_6ms = 0;
		Duty_6ms();						//周期6ms的任务
	}
	if( loop.cnt_10ms >= 10 )
	{
		loop.cnt_10ms = 0;
		Duty_10ms();					//周期10ms的任务
	} 
	if( loop.cnt_20ms >= 20 )
	{
		loop.cnt_20ms = 0;
		Duty_20ms();					//周期20ms的任务
	}
	if( loop.cnt_50ms >= 50 )
	{
		loop.cnt_50ms = 0;
		Duty_50ms();					//周期50ms的任务
	}
	if( loop.cnt_1000ms >= 1000)
	{
		loop.cnt_1000ms = 0;
		Duty_1000ms();				//周期1s的任务
	}
}

void Set_Check()
{
	if(seting)
	{
		switch(key_cnt)
		{
			case 3://校准传感器
				sensor.CALIBRATE=1;
			break;
			
			default: break;
		}
		seting  = 0;
		key_cnt = 0;
	}
}
/******************* (C) COPYRIGHT 2016 ANO TECH *****END OF FILE************/
