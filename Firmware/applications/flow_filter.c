
#include "flow_filter.h"



void Flow_Window_Filter(float *data0, float *data1, float *data2)
{
	static struct _Filter filter0,filter1,filter2;
	static uint8_t 	Filter_Count = 0;
	
	//丢数据
	filter0.sum -= filter0.old[Filter_Count];
	filter1.sum -= filter1.old[Filter_Count];
	filter2.sum -= filter2.old[Filter_Count];
	
	//更新数据
	filter0.old[Filter_Count] =  *data0;
	filter1.old[Filter_Count] =  *data1;
	filter2.old[Filter_Count] =  *data2;
	
	filter0.sum += filter0.old[Filter_Count];
	filter1.sum += filter1.old[Filter_Count];
	filter2.sum += filter2.old[Filter_Count];
	
	//输出数据
	*data0 = filter0.sum / Filter_Num;
	*data1 = filter1.sum / Filter_Num;
	*data2 = filter2.sum / Filter_Num;
	
	Filter_Count++;
	if(Filter_Count == Filter_Num)	Filter_Count=0;
}

void Gyro_Window_Filter(float *data0, float *data1)
{
	static struct _Filter filter0,filter1;
	static uint8_t 	Filter_Count = 0;
	
	//丢数据
	filter0.sum -= filter0.old[Filter_Count];
	filter1.sum -= filter1.old[Filter_Count];
	
	//更新数据
	filter0.old[Filter_Count] =  *data0;
	filter1.old[Filter_Count] =  *data1;
	
	filter0.sum += filter0.old[Filter_Count];
	filter1.sum += filter1.old[Filter_Count];
	
	//输出数据
	*data0 = filter0.sum / Filter_Num;
	*data1 = filter1.sum / Filter_Num;
	
	Filter_Count++;
	if(Filter_Count == Filter_Num)	Filter_Count=0;
}
