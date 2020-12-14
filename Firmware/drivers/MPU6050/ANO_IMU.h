#ifndef __ANO_imu_H
#define __ANO_imu_H

#include "include.h"
#include "ANO_Data.h"
#include "filter.h"

typedef struct
{
	float w;//q0;
	float x;//q1;
	float y;//q2;
	float z;//q3;

	_xyz_f_st x_vec;
	_xyz_f_st y_vec;
	_xyz_f_st z_vec;

	_xyz_f_st a_acc;
	_xyz_f_st w_acc;
	
	float rol;
	float pit;
	float yaw;
} _imu_st ;
extern _imu_st imu_data;
extern _imu_st imu_data1;

void MY_IMU_update(void);
void IMU_update(float dT,_xyz_f_st *gyr, _xyz_f_st *acc,_imu_st *imu);
#endif

