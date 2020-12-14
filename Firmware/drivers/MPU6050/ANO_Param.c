
#include "ANO_Param.h"
#include "ANO_Drv_MPU6050.h"
#include "stmflash.h"

struct _save_param_st_pk ANO_Param;

void ANO_Param_Init()
{
	ANO_Param.flash_check_flag = FLASH_CHECK_FLAG;
	ANO_Param.work_mode=1;
	ANO_Param.diff_line=300;
	
	ANO_Param_Save();
}
void ANO_Param_Read(void)
{
	u32 data_len;
	
	data_len = sizeof(ANO_Param);
	data_len = data_len/4+((data_len%4)?1:0);
	
	STMFLASH_Read(FLASH_SAVE_ADDR,(u32*)(&ANO_Param),data_len);

	if(ANO_Param.flash_check_flag != FLASH_CHECK_FLAG)//板子从未初始化
	{
		ANO_Param_Init();
		sensor.CALIBRATE = 1;	
	}
}

void ANO_Param_Save(void)
{
	u32 data_len;
	
	data_len = sizeof(ANO_Param);
	data_len = data_len/4+((data_len%4)?1:0);
	
	STMFLASH_Write(FLASH_SAVE_ADDR,(u32*)(&ANO_Param),data_len);
}

u8 Save_Over_Time=60;

//参数改变后一分钟保存起来
void Param_Save_Over_Time(void)
{
	Save_Over_Time++;
	
	if(Save_Over_Time==59)
	{
		ANO_Param_Save();
	}
	else if(Save_Over_Time>=60)
	{
		Save_Over_Time = 60;
	}
}



