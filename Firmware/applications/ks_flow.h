#ifndef _ks_flow_
#define _ks_flow_

#include "include.h"

#define SUB_IMAGE 8	//��ͼ��С
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

//����������ݽṹ��
struct _pixel_flow_
{
	float x_i;			//x�����ԭʼֵ
	float y_i;			//y�����ԭʼֵ
	float fix_x_i;			//x�����ԭʼֵ
	float fix_y_i;			//y�����ԭʼֵ
	float x;				//x���ٶ�ԭʼֵ
	float y;				//y���ٶ�ԭʼֵ
	float fix_x;		//x���ٶ��ں�ֵ
	float fix_y;		//y���ٶ��ں�ֵ
	float gyr_x;		//x���ٶ��ں�ֵ
	float gyr_y;		//y���ٶ��ں�ֵ
	float fix_gyr_x;		//x���ٶ��ں�ֵ
	float fix_gyr_y;		//y���ٶ��ں�ֵ
	float out_x;				//x���ٶ�ԭʼֵ
	float out_y;				//y���ٶ�ԭʼֵ
	float out_fix_x;				//x���ٶ�ԭʼֵ
	float out_fix_y;				//y���ٶ�ԭʼֵ
	
	float angle_x;	//�����ǽǶ�x
	float angle_y;	//�����ǽǶ�y
	float angle_z;	//�����ǽǶ�z
	
	uint16_t high;  //������߶�(10mm~2000mm)
	float match_num;   //ƥ���������Խ������Խ�ɿ�
	float fix_match_num;   //ƥ���������Խ������Խ�ɿ�
};
extern struct _pixel_flow_ pixel_flow;

//�������Ʋ����ṹ��
struct _flow_param_
{	
	u16 Diff_Threshold;					//�����ż�����Ҫ���ݻ������ȵ��ڣ�Խ�����ֵԽС��С��������ʾ������������ɡ�
	u16 Match_Threshold;        //ƥ����ż���ԽСԽƥ��
	
	u16  Diff_Max; 							//�������ֵ
	u16  Diff_Min;
	u16  Match_Max;
	u16  Match_Min;							//���ƶ���Сֵ
	
	int8_t  dirsx[100];	//x���ƶ���С
	int8_t  dirsy[100];	//y���ƶ���С
};

extern struct _flow_param_ flow_param;
	
void Flow_Param_Init(void);
void Copy_Image(u8 *p);
void compute_flow(uint8_t *image1, uint8_t *image2);
void Image_Duty(void);
void Update_Image(u8 *p);
void Pixel_Flow_Fix(void);

#endif
