#ifndef _CAMERA_H
#define _CAMERA_H

#include "include.h"

#define OV_PWDN  	PAout(15)			//POWER DOWN控制信号 
#define OV_RST  	PBout(5)			//复位控制信号 

//实际像素
#define HIG 135	//144
#define WID 180 //192
#define OV7740_SIZE             (HIG * WID )         //图像占用空间大小


#define angle_to_rad 0.017f
#define rad_to_angle 57.29f

extern u8 Work_Mode;

u8 Camera_Init(void);		  	   		 

#endif   
