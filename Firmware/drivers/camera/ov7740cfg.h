#ifndef _OV7740CFG_H
#define _OV7740CFG_H
#include "camera.h"

#define OV7740_PID	0x7742

#define YUV 0
#define RAW_10 1
#define RAW_8 2
#define Sensor_RAW 3

//设置输出格式
void OV7740_Output_Format(u8 Format)
{
	switch(Format)
	{
		case YUV:
			SCCB_WR_Reg(0x12, 0x00);
			SCCB_WR_Reg(0x36, 0x3f);
		break;
		
		case RAW_10:
			SCCB_WR_Reg(0x12, 0x01);
			SCCB_WR_Reg(0x36, 0x2f);
			SCCB_WR_Reg(0x83, 0x01);
		break;	
		
		case RAW_8:
			SCCB_WR_Reg(0x12, 0x01);
			SCCB_WR_Reg(0x36, 0x2f);
			SCCB_WR_Reg(0x83, 0x05);
		break;	
		
		case Sensor_RAW:
			SCCB_WR_Reg(0x12, 0x11);
			SCCB_WR_Reg(0x36, 0x3f);
		break;
	}
}

#define QVGA_320x240 0
#define VGA_640x480 1
#define	QQVGA_160x120 2
#define	CIF_352x288 3
#define USER_SIZE 4

//设置分辨率
void OV7740_Output_Size(u8 Size)
{
	switch(Size)
	{
		case QVGA_320x240:
			SCCB_WR_Reg(0x31, 0x50);//0x50*4 = 320
			SCCB_WR_Reg(0x32, 0x78);//0x78*2 = 240
			SCCB_WR_Reg(0x82, 0x3f);
		break;
		
		case VGA_640x480:
			SCCB_WR_Reg(0x31, 0xa0);
			SCCB_WR_Reg(0x32, 0xf0);
			SCCB_WR_Reg(0x82, 0x32);
		break;
	
		case QQVGA_160x120:
			SCCB_WR_Reg(0x31, 0x28);
			SCCB_WR_Reg(0x32, 0x3c);
			SCCB_WR_Reg(0x82, 0x3f);
		break;

		case CIF_352x288:
			SCCB_WR_Reg(0x31, 0x58);
			SCCB_WR_Reg(0x32, 0x90);
			SCCB_WR_Reg(0x82, 0x3f);
		break;
		
		case USER_SIZE:
			SCCB_WR_Reg(0x31, WID/4);
			SCCB_WR_Reg(0x32, HIG/2);
			SCCB_WR_Reg(0x82, 0x3f);
		break;
	}
}

//Frame Rate Adjustment for 24Mhz input clock
void OV7740_60_fps(void)
{
	SCCB_WR_Reg(0x11, 0x00);//不分频
	SCCB_WR_Reg(0x55, 0x40);
	SCCB_WR_Reg(0x2b, 0xf0);
	SCCB_WR_Reg(0x2c, 0x01);
}

#define YUYV 0
#define YVYU 1
#define UYVY 2
#define VYUY 3

void OV7740_YUV_Sequence(u8 Sequence)
{
	u8 temp;
	
	switch(Sequence)
	{
		case YUYV:
			temp = SCCB_RD_Reg(0x0C); //set 0x0c.bit4='0'
			temp &= 0xef;
			SCCB_WR_Reg(0x0C, temp);
			temp = SCCB_RD_Reg(0xD9); //set 0xd9.bit1='0'
			temp &= 0xfd;
			SCCB_WR_Reg(0xD9, temp);
		break;
		
		case YVYU:
			temp = SCCB_RD_Reg(0x0C); //set 0x0c.bit4='0'
			temp &= 0xef;
			SCCB_WR_Reg(0x0C, temp);
			temp = SCCB_RD_Reg(0xD9); //set 0xd9.bit1='1'
			temp |= 0x02;
			SCCB_WR_Reg(0xD9, temp);
		break;
		
		case UYVY:
			temp = SCCB_RD_Reg(0x0C); //set 0x0c.bit4='1'
			temp |= 0x10;
			SCCB_WR_Reg(0x0C, temp);
			temp = SCCB_RD_Reg(0xD9); //set 0xd9.bit1='0'
			temp &= 0xfd;
			SCCB_WR_Reg(0xD9, temp);
		break;
		
		case VYUY:
			temp = SCCB_RD_Reg(0x0C); //set 0x0c.bit4='1'
			temp |= 0x10;
			SCCB_WR_Reg(0x0C, temp);
			temp = SCCB_RD_Reg(0xD9); //set 0xd9.bit1='1'
			temp |= 0x02;
			SCCB_WR_Reg(0xD9, temp);
		break;
	}
}

#define Default   0
#define X_Mirror  1
#define Y_Mirror  2
#define XY_Mirror 3

void Mirror_And_Flip_Image(u8 mirror)
{
	u8 temp;
	switch(mirror)
	{
		case Default:
			temp = SCCB_RD_Reg(0x0C);
			temp |= 0xC0;//set 0x0c.bit6,7='1'
			SCCB_WR_Reg(0x0C, temp);
		break;
		
		case X_Mirror:
			temp = SCCB_RD_Reg(0x0C);
			temp |= 0x40;//set 0x0c.bit6='1'
			temp &= 0x7F;//set 0x0c.bit7='0'
			SCCB_WR_Reg(0x0C, temp);
		break;
		
		case Y_Mirror:
			temp = SCCB_RD_Reg(0x0C);
			temp |= 0x80;//set 0x0c.bit7='1'
			temp &= 0xBF;//set 0x0c.bit6='0'
			SCCB_WR_Reg(0x0C, temp);
		break;
		
		case XY_Mirror:
			temp = SCCB_RD_Reg(0x0C);
			temp &= 0x3F;//set 0x0c.bit6,7='0'
			SCCB_WR_Reg(0x0C, temp);
		break;
	}
}

#define Auto 0
#define Sunny 1
#define Cloudy 2
#define Office 3
#define Home 4
#define Night 5

void OV7740_Light_Mode(u8 mode)
{
	switch(mode)
	{
		case Auto:
			SCCB_WR_Reg(0x13, 0xff); //AWB on
			SCCB_WR_Reg(0x15, 0x00);
			SCCB_WR_Reg(0x2d, 0x00);
			SCCB_WR_Reg(0x2e, 0x00);
		break;
		
		case Sunny:
			SCCB_WR_Reg(0x13, 0xff); //AWB off
			SCCB_WR_Reg(0x01, 0x5a);
			SCCB_WR_Reg(0x02, 0x5c);
			SCCB_WR_Reg(0x03, 0x42);
			SCCB_WR_Reg(0x15, 0x00);
			SCCB_WR_Reg(0x2d, 0x00);
			SCCB_WR_Reg(0x2e, 0x00);
		break;
		
		case Cloudy:
			SCCB_WR_Reg(0x13, 0xff); //AWB off
			SCCB_WR_Reg(0x01, 0x58);
			SCCB_WR_Reg(0x02, 0x60);
			SCCB_WR_Reg(0x03, 0x40);
			SCCB_WR_Reg(0x15, 0x00);
			SCCB_WR_Reg(0x2d, 0x00);
			SCCB_WR_Reg(0x2e, 0x00);
		break;
		
		case Office:
			SCCB_WR_Reg(0x13, 0xff); //AWB off
			SCCB_WR_Reg(0x01, 0x84);
			SCCB_WR_Reg(0x02, 0x4c);
			SCCB_WR_Reg(0x03, 0x40);
			SCCB_WR_Reg(0x15, 0x00);
			SCCB_WR_Reg(0x2d, 0x00);
			SCCB_WR_Reg(0x2e, 0x00);
		break;
		
		case Home:
			SCCB_WR_Reg(0x13, 0xff); //AWB off
			SCCB_WR_Reg(0x01, 0x96);
			SCCB_WR_Reg(0x02, 0x40);
			SCCB_WR_Reg(0x03, 0x4a);
			SCCB_WR_Reg(0x15, 0x00);
			SCCB_WR_Reg(0x2d, 0x00);
			SCCB_WR_Reg(0x2e, 0x00);
		break;
		
		case Night:
			SCCB_WR_Reg(0x13, 0xff); //AWB on
			SCCB_WR_Reg(0x15, 0x00);
			SCCB_WR_Reg(0x2d, 0x00);
			SCCB_WR_Reg(0x2e, 0x00);
			SCCB_WR_Reg(0x15, 0xf4);
		break;
	}
}

//色彩饱和度
void OV7740_Color_Saturation(u8 saturation)
{
	u8 temp;
  switch(saturation)
	{
		case 0://+4
			temp = SCCB_RD_Reg(0x81);
			temp |= 0x20;
			SCCB_WR_Reg(0x81, temp);
			temp = SCCB_RD_Reg(0xDA);
			temp |= 0x02;
			SCCB_WR_Reg(0xDA, temp);
			SCCB_WR_Reg(0xDD, 0x80);
			SCCB_WR_Reg(0xDE, 0x80);
		break;	
		
		case 1://+3
			temp = SCCB_RD_Reg(0x81);
			temp |= 0x20;
			SCCB_WR_Reg(0x81, temp);
			temp = SCCB_RD_Reg(0xDA);
			temp |= 0x02;
			SCCB_WR_Reg(0xDA, temp);
			SCCB_WR_Reg(0xDD, 0x70);
			SCCB_WR_Reg(0xDE, 0x70);
		break;	
		
		case 2://+2
			temp = SCCB_RD_Reg(0x81);
			temp |= 0x20;
			SCCB_WR_Reg(0x81, temp);
			temp = SCCB_RD_Reg(0xDA);
			temp |= 0x02;
			SCCB_WR_Reg(0xDA, temp);
			SCCB_WR_Reg(0xDD, 0x60);
			SCCB_WR_Reg(0xDE, 0x60);
		break;	
		
		case 3://+1
			temp = SCCB_RD_Reg(0x81);
			temp |= 0x20;
			SCCB_WR_Reg(0x81, temp);
			temp = SCCB_RD_Reg(0xDA);
			temp |= 0x02;
			SCCB_WR_Reg(0xDA, temp);
			SCCB_WR_Reg(0xDD, 0x50);
			SCCB_WR_Reg(0xDE, 0x50);
		break;	
		
		case 4://0
			temp = SCCB_RD_Reg(0x81);
			temp |= 0x20;
			SCCB_WR_Reg(0x81, temp);
			temp = SCCB_RD_Reg(0xDA);
			temp |= 0x02;
			SCCB_WR_Reg(0xDA, temp);
			SCCB_WR_Reg(0xDD, 0x40);
			SCCB_WR_Reg(0xDE, 0x40);
		break;	
		
		case 5://-1
			temp = SCCB_RD_Reg(0x81);
			temp |= 0x20;
			SCCB_WR_Reg(0x81, temp);
			temp = SCCB_RD_Reg(0xDA);
			temp |= 0x02;
			SCCB_WR_Reg(0xDA, temp);
			SCCB_WR_Reg(0xDD, 0x30);
			SCCB_WR_Reg(0xDE, 0x30);
		break;	
		
		case 6://-2
			temp = SCCB_RD_Reg(0x81);
			temp |= 0x20;
			SCCB_WR_Reg(0x81, temp);
			temp = SCCB_RD_Reg(0xDA);
			temp |= 0x02;
			SCCB_WR_Reg(0xDA, temp);
			SCCB_WR_Reg(0xDD, 0x20);
			SCCB_WR_Reg(0xDE, 0x20);
		break;	
		
		case 7://-3
			temp = SCCB_RD_Reg(0x81);
			temp |= 0x20;
			SCCB_WR_Reg(0x81, temp);
			temp = SCCB_RD_Reg(0xDA);
			temp |= 0x02;
			SCCB_WR_Reg(0xDA, temp);
			SCCB_WR_Reg(0xDD, 0x10);
			SCCB_WR_Reg(0xDE, 0x10);
		break;	
		
		case 8://-4
			temp = SCCB_RD_Reg(0x81);
			temp |= 0x20;
			SCCB_WR_Reg(0x81, temp);
			temp = SCCB_RD_Reg(0xDA);
			temp |= 0x02;
			SCCB_WR_Reg(0xDA, temp);
			SCCB_WR_Reg(0xDD, 0x00);
			SCCB_WR_Reg(0xDE, 0x00);
		break;	
	}
}

//亮度
void OV7740_Brightness(u8 bright)
{
	u8 temp;
  switch(bright)
	{
		case 0://+4
			temp = SCCB_RD_Reg(0x81);
			temp |= 0x20;
			SCCB_WR_Reg(0x81, temp);
			temp = SCCB_RD_Reg(0xDA);
			temp |= 0x04;
			SCCB_WR_Reg(0xDA, temp);
			SCCB_WR_Reg(0xE4, 0x0E);
			SCCB_WR_Reg(0xE3, 0x40);
		break;	
		
		case 1://+3
			temp = SCCB_RD_Reg(0x81);
			temp |= 0x20;
			SCCB_WR_Reg(0x81, temp);
			temp = SCCB_RD_Reg(0xDA);
			temp |= 0x04;
			SCCB_WR_Reg(0xDA, temp);
			SCCB_WR_Reg(0xE4, 0x0E);
			SCCB_WR_Reg(0xE3, 0x30);
		break;	
		
		case 2://+2
			temp = SCCB_RD_Reg(0x81);
			temp |= 0x20;
			SCCB_WR_Reg(0x81, temp);
			temp = SCCB_RD_Reg(0xDA);
			temp |= 0x04;
			SCCB_WR_Reg(0xDA, temp);
			SCCB_WR_Reg(0xE4, 0x0E);
			SCCB_WR_Reg(0xE3, 0x20);
		break;	
		
		case 3://+1
			temp = SCCB_RD_Reg(0x81);
			temp |= 0x20;
			SCCB_WR_Reg(0x81, temp);
			temp = SCCB_RD_Reg(0xDA);
			temp |= 0x04;
			SCCB_WR_Reg(0xDA, temp);
			SCCB_WR_Reg(0xE4, 0x0E);
			SCCB_WR_Reg(0xE3, 0x10);
		break;	
		
		case 4://0
			temp = SCCB_RD_Reg(0x81);
			temp |= 0x20;
			SCCB_WR_Reg(0x81, temp);
			temp = SCCB_RD_Reg(0xDA);
			temp |= 0x04;
			SCCB_WR_Reg(0xDA, temp);
			SCCB_WR_Reg(0xE4, 0x0E);
			SCCB_WR_Reg(0xE3, 0x00);
		break;	
		
		case 5://-1
			temp = SCCB_RD_Reg(0x81);
			temp |= 0x20;
			SCCB_WR_Reg(0x81, temp);
			temp = SCCB_RD_Reg(0xDA);
			temp |= 0x04;
			SCCB_WR_Reg(0xDA, temp);
			SCCB_WR_Reg(0xE4, 0x06);
			SCCB_WR_Reg(0xE3, 0x10);
		break;	
		
		case 6://-2
			temp = SCCB_RD_Reg(0x81);
			temp |= 0x20;
			SCCB_WR_Reg(0x81, temp);
			temp = SCCB_RD_Reg(0xDA);
			temp |= 0x04;
			SCCB_WR_Reg(0xDA, temp);
			SCCB_WR_Reg(0xE4, 0x06);
			SCCB_WR_Reg(0xE3, 0x20);
		break;	
		
		case 7://-3
			temp = SCCB_RD_Reg(0x81);
			temp |= 0x20;
			SCCB_WR_Reg(0x81, temp);
			temp = SCCB_RD_Reg(0xDA);
			temp |= 0x04;
			SCCB_WR_Reg(0xDA, temp);
			SCCB_WR_Reg(0xE4, 0x06);
			SCCB_WR_Reg(0xE3, 0x30);
		break;	
		
		case 8://-4
			temp = SCCB_RD_Reg(0x81);
			temp |= 0x20;
			SCCB_WR_Reg(0x81, temp);
			temp = SCCB_RD_Reg(0xDA);
			temp |= 0x04;
			SCCB_WR_Reg(0xDA, temp);
			SCCB_WR_Reg(0xE4, 0x06);
			SCCB_WR_Reg(0xE3, 0x40);
		break;	
	}
}
//对比度设置
//0:+4
//1:+3
//2,+2
//3,+1
//4,0
//5,-1
//6:-2
//7:-3
//8,-4
void OV7740_Contrast(u8 contrast)
{
	u8 temp;
  switch(contrast)
	{
		case 0://Contrast +4
			temp = SCCB_RD_Reg(0x81);
			temp |= 0x20;
			SCCB_WR_Reg(0x81, temp);
			temp = SCCB_RD_Reg(0xDA);
			temp |= 0x04;
			SCCB_WR_Reg(0xDA, temp);
			SCCB_WR_Reg(0xE1, 0x20);
			SCCB_WR_Reg(0xE2, 0x30);
			SCCB_WR_Reg(0xE3, 0x00);
			temp = SCCB_RD_Reg(0xE4);
			temp &= 0xfb;
			SCCB_WR_Reg(0xE4, temp); 	 
			break;	
		case 1://Contrast +3
			temp = SCCB_RD_Reg(0x81);
			temp |= 0x20;
			SCCB_WR_Reg(0x81, temp);
			temp = SCCB_RD_Reg(0xDA);
			temp |= 0x04;
			SCCB_WR_Reg(0xDA, temp);
			SCCB_WR_Reg(0xE1, 0x20);
			SCCB_WR_Reg(0xE2, 0x2c);
			SCCB_WR_Reg(0xE3, 0x00);
			temp = SCCB_RD_Reg(0xE4);
			temp &= 0xfb;
			SCCB_WR_Reg(0xE4, temp); 	 
			break;	
		case 2://Contrast +2
			temp = SCCB_RD_Reg(0x81);
			temp |= 0x20;
			SCCB_WR_Reg(0x81, temp);
			temp = SCCB_RD_Reg(0xDA);
			temp |= 0x04;
			SCCB_WR_Reg(0xDA, temp);
			SCCB_WR_Reg(0xE1, 0x20);
			SCCB_WR_Reg(0xE2, 0x28);
			SCCB_WR_Reg(0xE3, 0x00); 	
			temp = SCCB_RD_Reg(0xE4);
			temp &= 0xfb;
			SCCB_WR_Reg(0xE4, temp);		
			break;	
		case 3://Contrast +1
			temp = SCCB_RD_Reg(0x81);
			temp |= 0x20;
			SCCB_WR_Reg(0x81, temp);
			temp = SCCB_RD_Reg(0xDA);
			temp |= 0x04;
			SCCB_WR_Reg(0xDA, temp);
			SCCB_WR_Reg(0xE1, 0x20);
			SCCB_WR_Reg(0xE2, 0x24);
			SCCB_WR_Reg(0xE3, 0x00); 	
			temp = SCCB_RD_Reg(0xE4);
			temp &= 0xfb;
			SCCB_WR_Reg(0xE4, temp);	 	 
			break;	
		case 4://Contrast 0
			temp = SCCB_RD_Reg(0x81);
			temp |= 0x20;
			SCCB_WR_Reg(0x81, temp);
			temp = SCCB_RD_Reg(0xDA);
			temp |= 0x04;
			SCCB_WR_Reg(0xDA, temp);
			SCCB_WR_Reg(0xE1, 0x20);
			SCCB_WR_Reg(0xE2, 0x20);
			SCCB_WR_Reg(0xE3, 0x00); 	
			temp = SCCB_RD_Reg(0xE4);
			temp &= 0xfb;
			SCCB_WR_Reg(0xE4, temp);	 	 
			break;
		case 5://Contrast -1
			temp = SCCB_RD_Reg(0x81);
			temp |= 0x20;
			SCCB_WR_Reg(0x81, temp);
			temp = SCCB_RD_Reg(0xDA);
			temp |= 0x04;
			SCCB_WR_Reg(0xDA, temp);
			SCCB_WR_Reg(0xE1, 0x20);
			SCCB_WR_Reg(0xE2, 0x1C);
			SCCB_WR_Reg(0xE3, 0x20); 	
			temp = SCCB_RD_Reg(0xE4);
			temp |= 0x04;
			SCCB_WR_Reg(0xE4, temp);	 	 
			break;
		case 6://Contrast -2
			temp = SCCB_RD_Reg(0x81);
			temp |= 0x20;
			SCCB_WR_Reg(0x81, temp);
			temp = SCCB_RD_Reg(0xDA);
			temp |= 0x04;
			SCCB_WR_Reg(0xDA, temp);
			SCCB_WR_Reg(0xE1, 0x20);
			SCCB_WR_Reg(0xE2, 0x18);
			SCCB_WR_Reg(0xE3, 0x48); 	
			temp = SCCB_RD_Reg(0xE4);
			temp |= 0x04;
			SCCB_WR_Reg(0xE4, temp);	 	 
			break;
		case 7://Contrast -3
			temp = SCCB_RD_Reg(0x81);
			temp |= 0x20;
			SCCB_WR_Reg(0x81, temp);
			temp = SCCB_RD_Reg(0xDA);
			temp |= 0x04;
			SCCB_WR_Reg(0xDA, temp);
			SCCB_WR_Reg(0xE1, 0x20);
			SCCB_WR_Reg(0xE2, 0x14);
			SCCB_WR_Reg(0xE3, 0x80); 	
			temp = SCCB_RD_Reg(0xE4);
			temp |= 0x04;
			SCCB_WR_Reg(0xE4, temp);	 	 
			break;
		case 8://Contrast -4
			temp = SCCB_RD_Reg(0x81);
			temp |= 0x20;
			SCCB_WR_Reg(0x81, temp);
			temp = SCCB_RD_Reg(0xDA);
			temp |= 0x04;
			SCCB_WR_Reg(0xDA, temp);
			SCCB_WR_Reg(0xE1, 0x20);
			SCCB_WR_Reg(0xE2, 0x10);
			SCCB_WR_Reg(0xE3, 0xD0); 	
			temp = SCCB_RD_Reg(0xE4);
			temp |= 0x04;
			SCCB_WR_Reg(0xE4, temp);	 	 
			break;
	}
}
//特效设置
//0,普通模式    
//1,复古
//2,偏蓝
//3,偏绿
//4,偏红
//5,B&W(啥效果，不清楚)
//6,负片
//7,B&W negative
void OV7740_Special_Effects(u8 eft)
{
	u8 temp;
	switch(eft)
	{
		case 0://正常
			temp = SCCB_RD_Reg(0x81);
			temp |= 0x20;
			SCCB_WR_Reg(0x81, temp);
			temp = SCCB_RD_Reg(0xDA);
			temp &= 0xe7;
			SCCB_WR_Reg(0xDA, temp);
			SCCB_WR_Reg(0xDF, 0x80);
			SCCB_WR_Reg(0xE0, 0x80);
			break;	
		case 1://复古
			temp = SCCB_RD_Reg(0x81);
			temp |= 0x20;
			SCCB_WR_Reg(0x81, temp);
			temp = SCCB_RD_Reg(0xDA);
			temp &= 0xe18;
			SCCB_WR_Reg(0xDA, temp);
			SCCB_WR_Reg(0xDF, 0x40);
			SCCB_WR_Reg(0xE0, 0xA0);
			break;	
		case 2://偏蓝
			temp = SCCB_RD_Reg(0x81);
			temp |= 0x20;
			SCCB_WR_Reg(0x81, temp);
			temp = SCCB_RD_Reg(0xDA);
			temp &= 0xe18;
			SCCB_WR_Reg(0xDA, temp);
			SCCB_WR_Reg(0xDF, 0xA0);
			SCCB_WR_Reg(0xE0, 0x40);
			break;	
		case 3://偏绿
			temp = SCCB_RD_Reg(0x81);
			temp |= 0x20;
			SCCB_WR_Reg(0x81, temp);
			temp = SCCB_RD_Reg(0xDA);
			temp &= 0xe18;
			SCCB_WR_Reg(0xDA, temp);
			SCCB_WR_Reg(0xDF, 0x60);
			SCCB_WR_Reg(0xE0, 0x60);
			break;	
		case 4://偏棕
			temp = SCCB_RD_Reg(0x81);
			temp |= 0x20;
			SCCB_WR_Reg(0x81, temp);
			temp = SCCB_RD_Reg(0xDA);
			temp &= 0xe18;
			SCCB_WR_Reg(0xDA, temp);
			SCCB_WR_Reg(0xDF, 0x80);
			SCCB_WR_Reg(0xE0, 0xC0);
			break;	
		case 5://B&W
			temp = SCCB_RD_Reg(0x81);
			temp |= 0x20;
			SCCB_WR_Reg(0x81, temp);
			temp = SCCB_RD_Reg(0xDA);
			temp &= 0xe18;
			SCCB_WR_Reg(0xDA, temp);
			SCCB_WR_Reg(0xDF, 0x80);
			SCCB_WR_Reg(0xE0, 0x80);
			break;	
		case 6://负片
			temp = SCCB_RD_Reg(0x81);
			temp |= 0x20;
			SCCB_WR_Reg(0x81, temp);
			temp = SCCB_RD_Reg(0xDA);
			temp &= 0xe7;
			temp |= 0x40;
			SCCB_WR_Reg(0xDA, temp);
			SCCB_WR_Reg(0xDF, 0x80);
			SCCB_WR_Reg(0xE0, 0x80);
			break;	 
		case 7://B&W negative
			temp = SCCB_RD_Reg(0x81);
			temp |= 0x20;
			SCCB_WR_Reg(0x81, temp);
			temp = SCCB_RD_Reg(0xDA);
			temp &= 0x18;
			temp |= 0x40;
			SCCB_WR_Reg(0xDA, temp);
			SCCB_WR_Reg(0xDF, 0x80);
			SCCB_WR_Reg(0xE0, 0x80);
			break;	
	}
}	

void OV7740_VGA_YUV(void)
{
	SCCB_WR_Reg(0x13, 0x00);
	SCCB_WR_Reg(0x11, 0x00);//不分频,60帧
	SCCB_WR_Reg(0x12, 0x00);
	SCCB_WR_Reg(0xd5, 0x10);
	SCCB_WR_Reg(0x0c, 0x12);//数据输出格式
	SCCB_WR_Reg(0x0d, 0x34);
	SCCB_WR_Reg(0x17, 0x25);
	SCCB_WR_Reg(0x18, 0xa0);
	SCCB_WR_Reg(0x19, 0x03);
	SCCB_WR_Reg(0x1a, 0xf0);
	SCCB_WR_Reg(0x1b, 0x89);
	
	SCCB_WR_Reg(0x22, 0x03);
	SCCB_WR_Reg(0x29, 0x17);
	SCCB_WR_Reg(0x2b, 0xf8);
	SCCB_WR_Reg(0x2c, 0x01);
	SCCB_WR_Reg(0x31, 0xa0);
	SCCB_WR_Reg(0x32, 0xf0);
	SCCB_WR_Reg(0x33, 0xc4);
	SCCB_WR_Reg(0x35, 0x05);
	SCCB_WR_Reg(0x36, 0x3f);
	SCCB_WR_Reg(0x04, 0x60);
	SCCB_WR_Reg(0x27, 0x80);
	SCCB_WR_Reg(0x3d, 0x0f);
	SCCB_WR_Reg(0x3e, 0x81);
	SCCB_WR_Reg(0x3f, 0x40);
	SCCB_WR_Reg(0x40, 0x7f);
	SCCB_WR_Reg(0x41, 0x6a);
	SCCB_WR_Reg(0x42, 0x29);
	SCCB_WR_Reg(0x44, 0xe5);
	SCCB_WR_Reg(0x45, 0x41);
	SCCB_WR_Reg(0x47, 0x42);
	SCCB_WR_Reg(0x48, 0x00);
	SCCB_WR_Reg(0x49, 0x61);
	SCCB_WR_Reg(0x4a, 0xa1);
	SCCB_WR_Reg(0x4b, 0x5e);
	SCCB_WR_Reg(0x4c, 0x18);
	SCCB_WR_Reg(0x4d, 0x50);
	SCCB_WR_Reg(0x4e, 0x13);
	SCCB_WR_Reg(0x64, 0x00);
	SCCB_WR_Reg(0x67, 0x88);
	SCCB_WR_Reg(0x68, 0x1a);
	SCCB_WR_Reg(0x14, 0x38);
	SCCB_WR_Reg(0x24, 0x3c);
	SCCB_WR_Reg(0x25, 0x30);
	SCCB_WR_Reg(0x26, 0x72);
	SCCB_WR_Reg(0x50, 0x97);
	SCCB_WR_Reg(0x51, 0x7e);
	SCCB_WR_Reg(0x52, 0x00);
	SCCB_WR_Reg(0x53, 0x00);
	SCCB_WR_Reg(0x20, 0x00);
	SCCB_WR_Reg(0x21, 0x23);
	SCCB_WR_Reg(0x38, 0x14);
	SCCB_WR_Reg(0xe9, 0x00);
	SCCB_WR_Reg(0x56, 0x55);
	SCCB_WR_Reg(0x57, 0xff);
	SCCB_WR_Reg(0x58, 0xff);
	SCCB_WR_Reg(0x59, 0xff);
	SCCB_WR_Reg(0x5f, 0x04);
	SCCB_WR_Reg(0xec, 0x00);
	
	SCCB_WR_Reg(0x13, 0xff);
	SCCB_WR_Reg(0x80, 0x7d);
	SCCB_WR_Reg(0x81, 0x3f);
	SCCB_WR_Reg(0x82, 0x32);
	SCCB_WR_Reg(0x83, 0x01);
	SCCB_WR_Reg(0x38, 0x11);
	SCCB_WR_Reg(0x84, 0x70);
	SCCB_WR_Reg(0x85, 0x00);
	SCCB_WR_Reg(0x86, 0x03);
	SCCB_WR_Reg(0x87, 0x01);
	SCCB_WR_Reg(0x88, 0x05);
	SCCB_WR_Reg(0x89, 0x30);
	SCCB_WR_Reg(0x8d, 0x30);
	SCCB_WR_Reg(0x8f, 0x85);
	SCCB_WR_Reg(0x93, 0x30);
	SCCB_WR_Reg(0x95, 0x85);
	SCCB_WR_Reg(0x99, 0x30);
	SCCB_WR_Reg(0x9b, 0x85);
	SCCB_WR_Reg(0x9c, 0x08);
	SCCB_WR_Reg(0x9d, 0x12);
	SCCB_WR_Reg(0x9e, 0x23);
	SCCB_WR_Reg(0x9f, 0x45);
	SCCB_WR_Reg(0xa0, 0x55);
	SCCB_WR_Reg(0xa1, 0x64);
	SCCB_WR_Reg(0xa2, 0x72);
	SCCB_WR_Reg(0xa3, 0x7f);
	SCCB_WR_Reg(0xa4, 0x8b);
	SCCB_WR_Reg(0xa5, 0x95);
	SCCB_WR_Reg(0xa6, 0xa7);
	SCCB_WR_Reg(0xa7, 0xb5);
	SCCB_WR_Reg(0xa8, 0xcb);
	SCCB_WR_Reg(0xa9, 0xdd);
	SCCB_WR_Reg(0xaa, 0xec);
	SCCB_WR_Reg(0xab, 0x1a);
	SCCB_WR_Reg(0xce, 0x78);
	SCCB_WR_Reg(0xcf, 0x6e);
	SCCB_WR_Reg(0xd0, 0x0a);
	SCCB_WR_Reg(0xd1, 0x0c);
	SCCB_WR_Reg(0xd2, 0x84);
	SCCB_WR_Reg(0xd3, 0x90);
	SCCB_WR_Reg(0xd4, 0x1e);
	SCCB_WR_Reg(0x5a, 0x24);
	SCCB_WR_Reg(0x5b, 0x1f);
	SCCB_WR_Reg(0x5c, 0x88);
	SCCB_WR_Reg(0x5d, 0x60);
	SCCB_WR_Reg(0xac, 0x6e);
	SCCB_WR_Reg(0xbe, 0xff);
	SCCB_WR_Reg(0xbf, 0x00);
	
	SCCB_WR_Reg(0x70, 0x00);
	SCCB_WR_Reg(0x71, 0x34);
	SCCB_WR_Reg(0x74, 0x28);
	SCCB_WR_Reg(0x75, 0x98);
	SCCB_WR_Reg(0x76, 0x00);
	SCCB_WR_Reg(0x77, 0x08);
	SCCB_WR_Reg(0x78, 0x01);
	SCCB_WR_Reg(0x79, 0xc2);
	SCCB_WR_Reg(0x7d, 0x02);
	SCCB_WR_Reg(0x7a, 0x4e);
	SCCB_WR_Reg(0x7b, 0x1f);
	SCCB_WR_Reg(0xec, 0x00);
	SCCB_WR_Reg(0x7c, 0x0c);
}

void OV7740_Init(void)
{
	//初始化YUYV序列,60帧每秒
	OV7740_VGA_YUV();
	
	//设置输出格式:YUV
	OV7740_Output_Format(YUV);
	
	//设置输出顺序:YUYV
	OV7740_YUV_Sequence(VYUY);
	
	//设置输出分辨率
	OV7740_Output_Size(USER_SIZE);
	
	//画面镜像设置
	Mirror_And_Flip_Image(Default);
	
	//对比度
	OV7740_Contrast(0);
	
	//情景模式
	OV7740_Light_Mode(Home);
	
	//色彩饱和度
	OV7740_Color_Saturation(0);
	
	//亮度
	OV7740_Brightness(0);
	//OV7740_60_fps();
}
#endif



















