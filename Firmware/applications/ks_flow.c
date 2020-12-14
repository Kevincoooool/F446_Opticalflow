
#include "ks_flow.h"

//ȥ��UVֵ��ֻȡ�Ҷ�ֵ
void Copy_Image(u8 *p)
{
	u16 x,y,cnt=0;
	u16 wid_temp = WID*2;
	
	for(y=0;y<HIG;y++)
	{
		for(x=0;x<wid_temp;x+=2)
		{
			p[cnt++]=p[y*wid_temp+x];
		}
	}
}

//ͼƬ��С����
void image_zoom(uint8_t *p_i, uint8_t *p_o)
{
	u8 arg;
	u16 x,y;
	u16 wid  = WID/2;
	u16 hig = HIG/2;
	u32 acc;
	
	for(y=0;y<hig;y++)
	{
		for(x=0;x<wid;x++)
		{
			acc = *((uint16_t*) &p_i[ 2*x + 2*y*WID]) << 16 | 
						*((uint16_t*) &p_i[2*x + (2*y+1)*WID]);
			
			arg =(u8)(__USAD8( acc, 0 )>>2);
			p_o[x+y*wid] = arg; //ȡƽ��ֵ
		}
	}
}

//ͼƬ�Աȶ����
void Photo_Duty(u8 *p)
{
	u16 x,y;
	u8 max=0x00,min=0xFF;
	u16 wid = WID/2;
	u16 hig = HIG/2;
	
	for(y=0;y<hig;y++)
	{
		for(x=0;x<wid;x++)
		{
			if( max < p[y*wid+x] )	max = p[y*wid+x];
			if( min > p[y*wid+x] )	min = p[y*wid+x];
		}
	}
	
	float zoom = 255.0f / ( max - min );
	
	if(zoom == 1) return;//�Աȶ��Ѿ����
	
	for(y=0;y<hig;y++)
	{
		for(x=0;x<wid;x++)
		{
			p[y*wid+x] = (p[y*wid+x] - min) * zoom;
		}
	}
	
}

///////////////////////////////////////////////////////
//�������ܣ����ĳ���Ƿ���������
///////////////////////////////////////////////////////
static inline uint32_t compute_diff(uint8_t *image, uint16_t offX, uint16_t offY, uint16_t row_size)
{
	uint16_t off = (offY + 2) * row_size + (offX + 2); // we calc only the 4x4 pattern
	uint32_t acc;
	
	//4*4�������������ۼ�
	acc = __USAD8 (*((uint32_t*) &image[off + 0 + 0 * row_size]), *((uint32_t*) &image[off + 0 + 1 * row_size]));
	acc = __USADA8(*((uint32_t*) &image[off + 0 + 1 * row_size]), *((uint32_t*) &image[off + 0 + 2 * row_size]), acc);
	acc = __USADA8(*((uint32_t*) &image[off + 0 + 2 * row_size]), *((uint32_t*) &image[off + 0 + 3 * row_size]), acc);

	//4*4�������������ۼ�
	uint32_t col1 = (image[off + 0 + 0 * row_size] << 24) | image[off + 0 + 1 * row_size] << 16 | image[off + 0 + 2 * row_size] << 8 | image[off + 0 + 3 * row_size];
	uint32_t col2 = (image[off + 1 + 0 * row_size] << 24) | image[off + 1 + 1 * row_size] << 16 | image[off + 1 + 2 * row_size] << 8 | image[off + 1 + 3 * row_size];
	uint32_t col3 = (image[off + 2 + 0 * row_size] << 24) | image[off + 2 + 1 * row_size] << 16 | image[off + 2 + 2 * row_size] << 8 | image[off + 2 + 3 * row_size];
	uint32_t col4 = (image[off + 3 + 0 * row_size] << 24) | image[off + 3 + 1 * row_size] << 16 | image[off + 3 + 2 * row_size] << 8 | image[off + 3 + 3 * row_size];

	//ȫ���ۼ�
	acc = __USADA8(col1, col2, acc);
	acc = __USADA8(col2, col3, acc);
	acc = __USADA8(col3, col4, acc);

	return acc;
}

///////////////////////////////////////////////////////
//�������ܣ��Ƚ�����8X8����ͼƬ�����ƶ�
///////////////////////////////////////////////////////
static inline uint32_t compute_sad_8x8(uint8_t *image1, uint8_t *image2, uint16_t off1X, uint16_t off1Y, uint16_t off2X, uint16_t off2Y, uint16_t row_size)
{
	uint16_t off1 = off1Y * row_size + off1X; // image1
	uint16_t off2 = off2Y * row_size + off2X; // image2

	//����8*8���ض�Ӧ��������ۼ�
	uint32_t acc;
	
	//��һ����������ۼ�
	acc = __USAD8 (*((uint32_t*) &image1[off1 + 0 + 0 * row_size]), *((uint32_t*) &image2[off2 + 0 + 0 * row_size]));
	acc = __USADA8(*((uint32_t*) &image1[off1 + 4 + 0 * row_size]), *((uint32_t*) &image2[off2 + 4 + 0 * row_size]), acc);
	
	//�ڶ�����������ۼӣ���������
	acc = __USADA8(*((uint32_t*) &image1[off1 + 0 + 1 * row_size]), *((uint32_t*) &image2[off2 + 0 + 1 * row_size]), acc);
	acc = __USADA8(*((uint32_t*) &image1[off1 + 4 + 1 * row_size]), *((uint32_t*) &image2[off2 + 4 + 1 * row_size]), acc);

	acc = __USADA8(*((uint32_t*) &image1[off1 + 0 + 2 * row_size]), *((uint32_t*) &image2[off2 + 0 + 2 * row_size]), acc);
	acc = __USADA8(*((uint32_t*) &image1[off1 + 4 + 2 * row_size]), *((uint32_t*) &image2[off2 + 4 + 2 * row_size]), acc);

	acc = __USADA8(*((uint32_t*) &image1[off1 + 0 + 3 * row_size]), *((uint32_t*) &image2[off2 + 0 + 3 * row_size]), acc);
	acc = __USADA8(*((uint32_t*) &image1[off1 + 4 + 3 * row_size]), *((uint32_t*) &image2[off2 + 4 + 3 * row_size]), acc);

	acc = __USADA8(*((uint32_t*) &image1[off1 + 0 + 4 * row_size]), *((uint32_t*) &image2[off2 + 0 + 4 * row_size]), acc);
	acc = __USADA8(*((uint32_t*) &image1[off1 + 4 + 4 * row_size]), *((uint32_t*) &image2[off2 + 4 + 4 * row_size]), acc);

	acc = __USADA8(*((uint32_t*) &image1[off1 + 0 + 5 * row_size]), *((uint32_t*) &image2[off2 + 0 + 5 * row_size]), acc);
	acc = __USADA8(*((uint32_t*) &image1[off1 + 4 + 5 * row_size]), *((uint32_t*) &image2[off2 + 4 + 5 * row_size]), acc);

	acc = __USADA8(*((uint32_t*) &image1[off1 + 0 + 6 * row_size]), *((uint32_t*) &image2[off2 + 0 + 6 * row_size]), acc);
	acc = __USADA8(*((uint32_t*) &image1[off1 + 4 + 6 * row_size]), *((uint32_t*) &image2[off2 + 4 + 6 * row_size]), acc);

	acc = __USADA8(*((uint32_t*) &image1[off1 + 0 + 7 * row_size]), *((uint32_t*) &image2[off2 + 0 + 7 * row_size]), acc);
	acc = __USADA8(*((uint32_t*) &image1[off1 + 4 + 7 * row_size]), *((uint32_t*) &image2[off2 + 4 + 7 * row_size]), acc);

	return acc;
}

struct _flow_param_ flow_param;//�������Ʋ����ṹ��
struct _pixel_flow_ pixel_flow;//����������ݽṹ��

//����������ʼ��
void Flow_Param_Init(void)
{
	flow_param.Diff_Threshold = 400;
	flow_param.Match_Threshold = 800;
}

///////////////////////////////////////////////////////
//�������ܣ��������
///////////////////////////////////////////////////////
void compute_flow(uint8_t *image_new, uint8_t *image_old)
{
	const int8_t winmin = -4;					//�Ƚϴ����ƶ��ķ�Χ
	const int8_t winmax = +4;					//�Ƚϴ����ƶ��ķ�Χ
	
	u16 wid = WID/2;
	u16 hig = HIG/2;
	u16 pixStepX;											//�������x
	u16 pixStepY;											//�������y
	u16 pixStartX = SUB_IMAGE/2 + 4;	//������x����
	u16 pixStartY = SUB_IMAGE/2 + 4;	//������y����
	u16 pixEndX = wid-pixStartX;			//����յ�x����
	u16 pixEndY = hig-pixStartY;			//����յ�Y����
	
	u16 diff_count=0;									//��¼�ҵ����ص�ĸ���
	u16 match_count=0;								//��¼ƥ�������ĸ���
	u16 temp_match;										//ƥ��ָ����ʱ����
	
	//��Ҫ��ʼ���Ĳ��������Բ���ʱʹ��
	flow_param.Diff_Max = 0x0000;
	flow_param.Diff_Min = 0xFFFF;
	flow_param.Match_Max = 0x0000;
	flow_param.Match_Min = 0xFFFF;
	
	for( u16 y=pixStartY; y<pixEndY; y+=pixStepY )
	{
		pixStepY = 2;//��ʼ��Y��������
		for( u16 x=pixStartX; x<pixEndX; x+=pixStepX )
		{
			//��������
			u16 diff = compute_diff(image_new,x,y,wid);
			
			//////////////////////////////////////////////////
			//�������㼫ֵ�������������Ҫ�õ���flow_param.Diff_Threshold
			if(flow_param.Diff_Max < diff)	flow_param.Diff_Max = diff;
			if(flow_param.Diff_Min > diff)	flow_param.Diff_Min = diff;
			/////////////////////////////////////////////////
			
			//�������ֵ̫С�ͺ��Ե�������������
			if(diff < flow_param.Diff_Threshold ) 
			{
				pixStepX = 2;//��2���ؿ�ʼ���
				continue;
			}
			else 
			{
				diff_count ++;//������+1
				pixStepX = 8;//��8���ؿ�ʼ���
				pixStepY = 8;//��8�п�ʼ���
			}
			
			//��ʼ�����ƶ�ֵ
			uint16_t match_min = 0xFFFF;
			int8_t sumx = 0;
			int8_t sumy = 0;
			
			//��������ѭ���ƶ�4�����ضԱ�ͼƬ���ƶ�
			for(int8_t yy=winmin; yy<=winmax; yy++)
			{
				for(int8_t xx=winmin; xx<=winmax; xx++)
				{
					//����ƥ���
					temp_match = compute_sad_8x8(image_new, image_old, x, y, x + xx, y + yy, wid);
					
					//Ѱ����Сֵ��ԽСԽ����
					if (temp_match < match_min)
					{
						sumx = xx;
						sumy = yy;
						match_min = temp_match;
					}
				}
			}
			
			////////////////////////////////////////////
			//�����ƶȼ�ֵ�������������Ҫ�õ����ƶȼ�ֵ��flow_param.Match_Threshold
			if(flow_param.Match_Max<match_min)	flow_param.Match_Max = match_min;
			if(flow_param.Match_Min>match_min)	flow_param.Match_Min = match_min;
			///////////////////////////////////////////
			
			//���ƶ��㹻С��Ϊƥ��
			if(match_min < flow_param.Match_Threshold)
			{
				//��¼��meancount���������ƶ��Ĵ�С�ͷ���
				flow_param.dirsx[match_count]   = sumx;
				flow_param.dirsy[match_count]   = sumy;
				match_count ++;
				
				//LCD��ʾ������ʹ��
//				#ifdef LCD_SHOW
					//LCD_Draw_Circle(160+x,180+y,5);	//LCD��ʾƥ���
				//Draw_Circle(40+x,40+y,5,0);
//				#endif
			}
		}
	}

	//ͼƬ�����жϣ�
	
	/*
	������Щ�������Ҫ������ͼƬ��
	
		1�����̫С��Ҳ����˵����ͼƬ������ƶ�̫С������1�����ء�
		2�����µ�ͼƬ���Ҳ��������㡣
	
	������Щ�������Ҫ���¾�ͼƬ��
	
		1�����ֵ������
		2�����µ�ͼƬ���ҵ��������ˣ������ھ�ͼƬ���Ҳ���ƥ�䡣
	*/
	
	
	//���µ�ͼƬ���Ҳ���������
	if(diff_count == 0)
	{
		old_image = old_image;		//������ͼƬ
		pixel_flow.x = 0;					//���Ϊ��
		pixel_flow.y = 0;					//���Ϊ��
		pixel_flow.match_num = 0;	//���Ϊ��
		return;										//�������
	}
	
	//���µ�ͼƬ���ҵ��������ˣ������ھ�ͼƬ���Ҳ���ƥ�䡣
	if(diff_count>0 && match_count == 0)
	{
		old_image = !old_image;		//���¾�ͼƬ
		pixel_flow.x = 0;					//���Ϊ��
		pixel_flow.y = 0;					//���Ϊ��
		pixel_flow.match_num = 0;	//���Ϊ��
		return;										//�������
	}
	
	//�����е�����˵������ͼƬ���Ѿ��ҵ������㲢���ھ�ͼƬ��Ҳ�ҵ���ƥ��㡣
	//��������жϣ�
	
	pixel_flow.match_num = match_count;	//ƥ���������
	
	float flowx = 0.0f;
	float flowy = 0.0f;
	
	//�������յĹ���
	for (uint8_t i = 0; i < match_count; i++)
	{
		flowx += flow_param.dirsx[i];
		flowy += flow_param.dirsy[i];
	}
	
	flowx = flowx / match_count;
	flowy = flowy / match_count;
	
	//������̫С,λ�Ʋ���1����
	if( flowx*flowx<1 && flowy*flowy<1 )
	{
		pixel_flow.x = 0;				//���Ϊ��
		pixel_flow.y = 0;				//���Ϊ��
		old_image = old_image;	//������ͼƬ
	}
	else
	{
		//����ԭʼ�������(�Ŵ�10��)
		pixel_flow.x = +flowx*10;
		pixel_flow.y = -flowy*10;
		
		//���¾�ͼƬ
		old_image = !old_image;
	}
}

u8 old_image=0;//������ͼƬ�õ��ǻ���1���ǻ���2
u8 image1_2x[image_size_2x],image2_2x[image_size_2x];//��С2�����ͼƬ����

u32 data_len;//DMA���ݴ�������
u32 Image0[1];
u32 Image1[buf_size];
u32 Image2[buf_size];

void Image_Duty(void)
{
	u8 *p;
	
	//ͼƬ1�ɼ���ϲ���û�д���
	if(W_page1==1 && R_page1==0)
	{
		p=(u8*)(Image1);
		Update_Image((u8*)Image1);
		showimage(p);
		//USART_DMACmd(USART2,USART_DMAReq_Tx,ENABLE);
//		USART2_Show_Photo(p);
//		MYDMA_Enable(DMA1_Stream6,OV7740_SIZE);
//	while(1){
//		if(DMA_GetFlagStatus(DMA1_Stream6,DMA_FLAG_TCIF6)!=RESET)//�ȴ�DMA2_Steam7�������
//				{ 
//					DMA_ClearFlag(DMA1_Stream6,DMA_FLAG_TCIF6);//���DMA2_Steam7������ɱ�־
//					break; 
//		        }
//			}
		ov_frame++;
		R_page1 = 1;
		LED0 = !LED0;
	}
	//ͼƬ2�ɼ���ϲ���û�д���
	else if(W_page2==1 && R_page2==0)
	{
		p=(u8*)(Image2);
		Update_Image((u8*)Image2);
		showimage(p);
//		USART_DMACmd(USART2,USART_DMAReq_Tx,ENABLE);
//		USART2_Show_Photo(p);
//		MYDMA_Enable(DMA1_Stream6,OV7740_SIZE);
//			while(1){
//		if(DMA_GetFlagStatus(DMA1_Stream6,DMA_FLAG_TCIF6)!=RESET)//�ȴ�DMA2_Steam7�������
//				{ 
//					DMA_ClearFlag(DMA1_Stream6,DMA_FLAG_TCIF6);//���DMA2_Steam7������ɱ�־
//					break; 
//		        }
//			}
		ov_frame++;
		R_page2 = 1;
		LED0 = !LED0;
	}
}

//ͼƬ���ݸ���
void Update_Image(u8 *p)
{
	//LCD��ʾ������ʹ��
	#ifdef LCD_SHOW
		//LCD_Show();
	#endif
	
	//old_image=0:��ʾimage1�Ǿ�ͼƬ
	//old_image=1:��ʾimage2�Ǿ�ͼƬ
	
	//image1�Ǿ�ͼƬ����Ҫʹ��image2_2x������С��ͼƬ
	if(old_image==0)
	{
		Copy_Image(p);											//ȥ��UVֵ
		image_zoom(p,image2_2x); 						//��С����
		Photo_Duty(image2_2x); 							//ͼƬ�Աȶ����
		compute_flow(image2_2x,image1_2x);	//�������
		Pixel_Flow_Fix();										//�����ں�
	}
	//image2�Ǿ�ͼƬ����Ҫʹ��image1_2x������С��ͼƬ
	else
	{
		Copy_Image(p);											//ȥ��UVֵ
		image_zoom(p,image1_2x); 						//��С����
		Photo_Duty(image1_2x); 							//ͼƬ�Աȶ����
		compute_flow(image1_2x,image2_2x);	//�������
		Pixel_Flow_Fix();										//�����ں�
	}
}

volatile u32 flow_duty_time;
#define GYRO_MAX 20.0f

//���������˲��ں�
void Pixel_Flow_Fix(void)
{
	float dT;
	
	////////////////////////*�����ٶȻ���λ��*////////////////////////////
	
	//����ʱ��
	dT = 0.000001f *(GetSysTime_us() - flow_duty_time);
	flow_duty_time = GetSysTime_us();
	
	//����ԭʼֵ����==>>λ��
	pixel_flow.x_i += pixel_flow.x * dT;
	pixel_flow.y_i += pixel_flow.y * dT;
	
	//����λ�ƺʹ���������ں�
	pixel_flow.fix_x_i = pixel_flow.x_i + 18.0f*tan(imu_data.rol*angle_to_rad);
	pixel_flow.fix_y_i = pixel_flow.y_i - 18.0f*tan(imu_data.pit*angle_to_rad);
	
	////////////////////////*���������˲�*///////////////////////////////
	
	//���������˲�
	Flow_Window_Filter( &pixel_flow.x, &pixel_flow.y, &pixel_flow.match_num );
	
	//��ͨ�˲�
	pixel_flow.fix_x += ( pixel_flow.x - pixel_flow.fix_x ) * 0.2f;
	pixel_flow.fix_y += ( pixel_flow.y - pixel_flow.fix_y ) * 0.2f;
	pixel_flow.fix_match_num += ( pixel_flow.match_num - pixel_flow.fix_match_num ) * 0.2f;
	
	////////////////////////*���ٶ������˲�*///////////////////////////////
	
	//��ȡ���ٶ�ԭʼ����
	pixel_flow.gyr_x = -sensor_val[G_Y]*0.015f;
	pixel_flow.gyr_y = +sensor_val[G_X]*0.015f;
	
	//���������˲�
	Gyro_Window_Filter(&pixel_flow.gyr_x,&pixel_flow.gyr_y);
	
	//��ͨ�˲�
	pixel_flow.fix_gyr_x +=  ( pixel_flow.gyr_x - pixel_flow.fix_gyr_x ) * 0.2f;
	pixel_flow.fix_gyr_y +=  ( pixel_flow.gyr_y - pixel_flow.fix_gyr_y ) * 0.2f;
	
	///////////////////*������������ٶ������ں�*//////////////////////////
	
	//������ٶ�̫�󣬹������ݲ��ɿ����0.
	if( ABS(pixel_flow.fix_gyr_x) >= GYRO_MAX )
		pixel_flow.out_x = 0;
	else
		pixel_flow.out_x = (pixel_flow.fix_x - pixel_flow.fix_gyr_x) * ( GYRO_MAX - ABS(pixel_flow.fix_gyr_x) ) / GYRO_MAX;
	
	if( ABS(pixel_flow.fix_gyr_y) >= GYRO_MAX )
		pixel_flow.out_y = 0;
	else
		pixel_flow.out_y = (pixel_flow.fix_y - pixel_flow.fix_gyr_y) * ( GYRO_MAX - ABS(pixel_flow.fix_gyr_y) ) / GYRO_MAX;
	
	//�ںϺ��ٴ��˲�
	pixel_flow.out_fix_x +=  ( pixel_flow.out_x - pixel_flow.out_fix_x ) * 0.2f;
	pixel_flow.out_fix_y +=  ( pixel_flow.out_y - pixel_flow.out_fix_y ) * 0.2f;
	
	///////////////////*����������߶������ں�*//////////////////////////
	
	/*���ڻ������ȶԼ��������ݵ�Ӱ��̫��
		������û���жϲ�������Ƿ��ȶ����ɿ�����Ч��
		���Ҹ߶��ںϵ��㷨Ҳ�Ƚϼ򵥣�����һ�������Ĺ�ϵ��
		���Ը߶��ں����Լ���MUC�˴���*/
}

