#ifndef _FLOW_FILTER_H_
#define _FLOW_FILTER_H_

#include "include.h"

#define Filter_Num 5

struct _Filter
{
	float sum;
	float old[Filter_Num];
};

void Flow_Window_Filter(float *data0, float *data1, float *data2);
void Gyro_Window_Filter(float *data0, float *data1);

#endif
