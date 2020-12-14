
#include "drv_vl53l0x.h"
#include "vl53l0x_def.h"
#include "vl53l0x_platform.h"

u8 VL53L0X_LINKOK = 0;

u16 VL53L0X_Dist,Dist_Out;

void Drv_Vl53_Init(void)
{
	mode = 2;//0��Ĭ��;1:�߾���;2:������;3:����
	
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
	
	VL53L0X_Dist = VL53L0X_FastRead()/10;//��λCM
	
	//���ݴ���200���Ǵ���ġ�
	//������ֻ�������ڡ�����ȹ���ǿ�Ȳ��ߵĻ�����ʹ�á�
	if(VL53L0X_Dist>200)	return;
	else	Dist_Out = VL53L0X_Dist;
}


