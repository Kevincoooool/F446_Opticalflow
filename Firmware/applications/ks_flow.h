#ifndef _ks_flow_
#define _ks_flow_

#include "include.h"

#define SUB_IMAGE 8	//子图大小
#define image_size_2x  (HIG/2)*(WID/2)

typedef struct
{
	float x,y;
	float speed_ei;
	float speed;
	float speed_fix;
} _flow_fusion_st;

extern _flow_fusion_st flowx_fus;
extern _flow_fusion_st flowy_fus;
extern u8 old_image;
extern u8 image1_2x[image_size_2x],image2_2x[image_size_2x];

//光流输出数据结构体
struct _pixel_flow_
{
	float x_i;			//x轴积分原始值
	float y_i;			//y轴积分原始值
	float fix_x_i;			//x轴积分原始值
	float fix_y_i;			//y轴积分原始值
	float x;				//x轴速度原始值
	float y;				//y轴速度原始值
	float fix_x;		//x轴速度融合值
	float fix_y;		//y轴速度融合值
	float gyr_x;		//x轴速度融合值
	float gyr_y;		//y轴速度融合值
	float fix_gyr_x;		//x轴速度融合值
	float fix_gyr_y;		//y轴速度融合值
	float out_x;				//x轴速度原始值
	float out_y;				//y轴速度原始值
	float out_fix_x;				//x轴速度原始值
	float out_fix_y;				//y轴速度原始值
	
	float angle_x;	//陀螺仪角度x
	float angle_y;	//陀螺仪角度y
	float angle_z;	//陀螺仪角度z
	
	uint16_t high;  //激光测距高度(10mm~2000mm)
	float match_num;   //匹配点数量，越大数据越可靠
	float fix_match_num;   //匹配点数量，越大数据越可靠
};
extern struct _pixel_flow_ pixel_flow;

//光流控制参数结构体
struct _flow_param_
{	
	u16 Diff_Threshold;					//轮廓门槛，需要根据环境亮度调节，越暗这个值越小，小到可以显示物体的轮廓即可。
	u16 Match_Threshold;        //匹配度门槛，越小越匹配
	
	u16  Diff_Max; 							//轮廓最大值
	u16  Diff_Min;
	u16  Match_Max;
	u16  Match_Min;							//相似度最小值
	
	int8_t  dirsx[100];	//x轴移动大小
	int8_t  dirsy[100];	//y轴移动大小
};

extern struct _flow_param_ flow_param;
	
void Flow_Param_Init(void);
void Copy_Image(u8 *p);
void compute_flow(uint8_t *image1, uint8_t *image2);
void Image_Duty(void);
void Update_Image(u8 *p);
void Pixel_Flow_Fix(void);

#endif
