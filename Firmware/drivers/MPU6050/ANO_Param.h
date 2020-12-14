#ifndef __ANO_PARAM_H
#define __ANO_PARAM_H

#include "include.h"
#include "ANO_Data.h"
/////////////////////////////////////////////////
#define FLASH_CHECK_FLAG 		0xAA

struct _save_param_st_pk
{	
	u8 sensor_type;
	u8 work_mode;
	u16 diff_line;
	
	_xyz_f_st_pk acc_offset;
	_xyz_f_st_pk gyr_offset;
	
	u8 flash_check_flag;
};

/////////////////////////////////////////////////
extern struct _save_param_st_pk ANO_Param;
extern u8 Save_Over_Time;
void ANO_Param_Read(void);
void ANO_Param_Save(void);
void Param_Save_Over_Time(void);
#endif
