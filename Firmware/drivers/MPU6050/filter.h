#ifndef __FILTER_H
#define __FILTER_H

#include "include.h"
#include "mymath.h"
#include "math.h"
#include "ANO_Data.h"


typedef struct
{
	float integration_1;
	float integration_2;
	float out_tmp;
	float out;

}_com_fil_t;

typedef struct
{
	float lpf_1;

	float out;
}_lf_t;

typedef struct
{
	float lpf_1;
	float lpf_2;
	float in_old;
	float out;
}_jldf_t;

typedef struct
{
	u8 cnt;

	s32 lst_pow_sum;
	
	s32 now_out;
	s32 lst_out;
	s32 now_velocity_xdt;
} _steepest_st;

#define LPF_1_(hz,t,in,out) ((out) += ( 1 / ( 1 + 1 / ( (hz) *6.28f *(t) ) ) ) *( (in) - (out) ))

void steepest_descend(s32 arr[],u8 len,_steepest_st *steepest,u8 step_num,s32 in);

void limit_filter(float T,float hz,_lf_t *data,float in);

void jyoun_limit_deadzone_filter(float T,float hz1,float hz2,_jldf_t *data,float in);//白噪声滤波

void jyoun_filter(float dT,float hz,float ref_value,float exp,float fb,float *out);
//float Moving_Average(u8 item,u8 width_num,float in);
void Moving_Average(float moavarray[],//滤波数组 数组长度：len+1
										u16 len ,//滤波数据长度
										u16 *fil_cnt,//滤波元素号数标记（静态，用作存储）
										float in,//输入
										float *out //输出
										);



void step_filter(float step,float in,float *out);

void fir_arrange_filter(float *arr,u16 len,u8 *fil_cnt,float in,float *arr_out);  //len<=255 len >= 3

void LPF_1(float hz,//截止频率
					float time,//周期
					float in,//输入
					float *out//输出
					);

					
void LPF_1_db(float hz,float time,double in,double *out); //低通滤波，2hz代表0.5秒上升至目标值0.7倍，大约1秒上升到90%

void LPF_I(float raw_a,float raw_b,float time,float in,float *out,float *intera);

void Complementary_filter(float T,//周期
													float kp,//截止频率（hz）
													float ki_1, //积分速度1 *使用ki_2前必须有ki_1
													float ki_2, //积分速度2
													float i_lim_1,//积分1限幅
													float i_lim_2,
													float in1_delta, //输入1，增量值
													float in2 ,			//输入2，绝对位置值
													_com_fil_t *   //互补融合数据结构体
													);


float my_deadzone_3(float T,float hz,float x,float ,float zoom,float range_x,float *zoom_adj); //range_x   0 ----- 1  *****


/*============ 坐标转换 ===============
适用坐标系
					x
					|
			y---z
			
对应世界坐标中，x为地磁方向，z为重力方向。

======================================*/
void vec_3d_transition(_xyz_f_st *ref, _xyz_f_st *in, _xyz_f_st *out);

void mag_horizon_trans(_xyz_f_st *ref, _xyz_f_st *in, _xyz_f_st *out);

/*============== AHRS =================
		自创AHRS姿态解算，非四元数
======================================*/
#define G_KP 1.0f
#define M_KP 1.0f

//mx，my，mz模长需在300 ---- 100 以内
void Jyoun_AHRS(float T,
								u8 acc_fix_en,
								u8 mag_fix_en,
								float ax,float ay,float az,
								float gx,float gy,float gz,
								float mx,float my,float mz,
								float *yaw,float *pit,float *rol,
								_xyz_f_st *acc_ng_offset
								);

extern _xyz_f_st acc_3d_ng,//3d矫正后的机体坐标下运动加速度（去除重力加速度）；
								acc_ng_offset,
								acc_3d_hg,//3d校正后世界坐标运动加速度（不含航向），
								gyro_3d_h;//3d矫正后，世界坐标下的陀螺仪数据（不含航向）

extern _xyz_f_st vec_g;
extern _xyz_f_st vec_m;
extern _xyz_f_st vec_m_h;
extern u8 acc_ng_cali;

#endif
