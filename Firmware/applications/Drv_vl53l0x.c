
#include "drv_vl53l0x.h"
#include "vl53l0x_def.h"
#include "vl53l0x_platform.h"

u8 VL53L0X_LINKOK = 0;

u16 VL53L0X_Dist,Dist_Out;

void Drv_Vl53_Init(void)
{
	mode = 2;//0：默认;1:高精度;2:长距离;3:高速
	
	if(VL53L0X_Init() == VL53L0X_ERROR_NONE)
	{
		VL53L0X_LINKOK = 1;
		_VL53L0X_I2C_FastMode = 1;
	}
	else
	{
		VL53L0X_LINKOK = 0;
	}
}
void Drv_Vl53_RunTask(void)
{
	if(!VL53L0X_LINKOK)	return;
	
	VL53L0X_Dist = VL53L0X_FastRead()/10;//单位CM
	
	//数据大于200都是错误的。
	//激光测距只能在室内、阴天等光线强度不高的环境下使用。
	if(VL53L0X_Dist>200)	return;
	else	Dist_Out = VL53L0X_Dist;
}


