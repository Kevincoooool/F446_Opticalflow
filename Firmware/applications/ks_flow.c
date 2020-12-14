
#include "ks_flow.h"

//去掉UV值，只取灰度值
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

//图片缩小两倍
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
			p_o[x+y*wid] = arg; //取平均值
		}
	}
}

//图片对比度最大化
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
	
	if(zoom == 1) return;//对比度已经最大
	
	for(y=0;y<hig;y++)
	{
		for(x=0;x<wid;x++)
		{
			p[y*wid+x] = (p[y*wid+x] - min) * zoom;
		}
	}
	
}

///////////////////////////////////////////////////////
//函数功能：检测某点是否是特征点
///////////////////////////////////////////////////////
static inline uint32_t compute_diff(uint8_t *image, uint16_t offX, uint16_t offY, uint16_t row_size)
{
	uint16_t off = (offY + 2) * row_size + (offX + 2); // we calc only the 4x4 pattern
	uint32_t acc;
	
	//4*4像素向下求差，再累加
	acc = __USAD8 (*((uint32_t*) &image[off + 0 + 0 * row_size]), *((uint32_t*) &image[off + 0 + 1 * row_size]));
	acc = __USADA8(*((uint32_t*) &image[off + 0 + 1 * row_size]), *((uint32_t*) &image[off + 0 + 2 * row_size]), acc);
	acc = __USADA8(*((uint32_t*) &image[off + 0 + 2 * row_size]), *((uint32_t*) &image[off + 0 + 3 * row_size]), acc);

	//4*4像素向右求差，再累加
	uint32_t col1 = (image[off + 0 + 0 * row_size] << 24) | image[off + 0 + 1 * row_size] << 16 | image[off + 0 + 2 * row_size] << 8 | image[off + 0 + 3 * row_size];
	uint32_t col2 = (image[off + 1 + 0 * row_size] << 24) | image[off + 1 + 1 * row_size] << 16 | image[off + 1 + 2 * row_size] << 8 | image[off + 1 + 3 * row_size];
	uint32_t col3 = (image[off + 2 + 0 * row_size] << 24) | image[off + 2 + 1 * row_size] << 16 | image[off + 2 + 2 * row_size] << 8 | image[off + 2 + 3 * row_size];
	uint32_t col4 = (image[off + 3 + 0 * row_size] << 24) | image[off + 3 + 1 * row_size] << 16 | image[off + 3 + 2 * row_size] << 8 | image[off + 3 + 3 * row_size];

	//全部累加
	acc = __USADA8(col1, col2, acc);
	acc = __USADA8(col2, col3, acc);
	acc = __USADA8(col3, col4, acc);

	return acc;
}

///////////////////////////////////////////////////////
//函数功能：比较两副8X8像素图片的相似度
///////////////////////////////////////////////////////
static inline uint32_t compute_sad_8x8(uint8_t *image1, uint8_t *image2, uint16_t off1X, uint16_t off1Y, uint16_t off2X, uint16_t off2Y, uint16_t row_size)
{
	uint16_t off1 = off1Y * row_size + off1X; // image1
	uint16_t off2 = off2Y * row_size + off2X; // image2

	//两副8*8像素对应相减，再累加
	uint32_t acc;
	
	//第一行相减，再累加
	acc = __USAD8 (*((uint32_t*) &image1[off1 + 0 + 0 * row_size]), *((uint32_t*) &image2[off2 + 0 + 0 * row_size]));
	acc = __USADA8(*((uint32_t*) &image1[off1 + 4 + 0 * row_size]), *((uint32_t*) &image2[off2 + 4 + 0 * row_size]), acc);
	
	//第二行相减，再累加，依次类推
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

struct _flow_param_ flow_param;//光流控制参数结构体
struct _pixel_flow_ pixel_flow;//光流输出数据结构体

//光流参数初始化
void Flow_Param_Init(void)
{
	flow_param.Diff_Threshold = 400;
	flow_param.Match_Threshold = 800;
}

///////////////////////////////////////////////////////
//函数功能：计算光流
///////////////////////////////////////////////////////
void compute_flow(uint8_t *image_new, uint8_t *image_old)
{
	const int8_t winmin = -4;					//比较窗口移动的范围
	const int8_t winmax = +4;					//比较窗口移动的范围
	
	u16 wid = WID/2;
	u16 hig = HIG/2;
	u16 pixStepX;											//采样间距x
	u16 pixStepY;											//采样间距y
	u16 pixStartX = SUB_IMAGE/2 + 4;	//检测起点x坐标
	u16 pixStartY = SUB_IMAGE/2 + 4;	//检测起点y坐标
	u16 pixEndX = wid-pixStartX;			//检测终点x坐标
	u16 pixEndY = hig-pixStartY;			//检测终点Y坐标
	
	u16 diff_count=0;									//记录找到边沿点的个数
	u16 match_count=0;								//记录匹配特征的个数
	u16 temp_match;										//匹配指数临时缓存
	
	//需要初始化的参数，调试参数时使用
	flow_param.Diff_Max = 0x0000;
	flow_param.Diff_Min = 0xFFFF;
	flow_param.Match_Max = 0x0000;
	flow_param.Match_Min = 0xFFFF;
	
	for( u16 y=pixStartY; y<pixEndY; y+=pixStepY )
	{
		pixStepY = 2;//初始化Y轴采样间距
		for( u16 x=pixStartX; x<pixEndX; x+=pixStepX )
		{
			//特征点检测
			u16 diff = compute_diff(image_new,x,y,wid);
			
			//////////////////////////////////////////////////
			//求特征点极值，调这个参数需要用到：flow_param.Diff_Threshold
			if(flow_param.Diff_Max < diff)	flow_param.Diff_Max = diff;
			if(flow_param.Diff_Min > diff)	flow_param.Diff_Min = diff;
			/////////////////////////////////////////////////
			
			//如果特征值太小就忽略掉，否则继续检测
			if(diff < flow_param.Diff_Threshold ) 
			{
				pixStepX = 2;//跳2像素开始检测
				continue;
			}
			else 
			{
				diff_count ++;//特征点+1
				pixStepX = 8;//跳8像素开始检测
				pixStepY = 8;//跳8行开始检测
			}
			
			//初始化相似度值
			uint16_t match_min = 0xFFFF;
			int8_t sumx = 0;
			int8_t sumy = 0;
			
			//上下左右循环移动4个像素对比图片相似度
			for(int8_t yy=winmin; yy<=winmax; yy++)
			{
				for(int8_t xx=winmin; xx<=winmax; xx++)
				{
					//计算匹配度
					temp_match = compute_sad_8x8(image_new, image_old, x, y, x + xx, y + yy, wid);
					
					//寻找最小值，越小越相似
					if (temp_match < match_min)
					{
						sumx = xx;
						sumy = yy;
						match_min = temp_match;
					}
				}
			}
			
			////////////////////////////////////////////
			//求相似度极值，调这个参数需要用到相似度极值：flow_param.Match_Threshold
			if(flow_param.Match_Max<match_min)	flow_param.Match_Max = match_min;
			if(flow_param.Match_Min>match_min)	flow_param.Match_Min = match_min;
			///////////////////////////////////////////
			
			//相似度足够小认为匹配
			if(match_min < flow_param.Match_Threshold)
			{
				//记录第meancount个特征的移动的大小和方向
				flow_param.dirsx[match_count]   = sumx;
				flow_param.dirsy[match_count]   = sumy;
				match_count ++;
				
				//LCD显示，调试使用
//				#ifdef LCD_SHOW
					//LCD_Draw_Circle(160+x,180+y,5);	//LCD显示匹配点
				//Draw_Circle(40+x,40+y,5,0);
//				#endif
			}
		}
	}

	//图片更新判断：
	
	/*
	下面这些情况下需要保留旧图片：
	
		1、输出太小，也就是说两副图片的相对移动太小，不够1个像素。
		2、在新的图片中找不到特征点。
	
	下面这些情况下需要更新旧图片：
	
		1、输出值正常。
		2、在新的图片中找到特征点了，但是在旧图片中找不到匹配。
	*/
	
	
	//在新的图片中找不到特征点
	if(diff_count == 0)
	{
		old_image = old_image;		//保留旧图片
		pixel_flow.x = 0;					//输出为零
		pixel_flow.y = 0;					//输出为零
		pixel_flow.match_num = 0;	//输出为零
		return;										//结束检测
	}
	
	//在新的图片中找到特征点了，但是在旧图片中找不到匹配。
	if(diff_count>0 && match_count == 0)
	{
		old_image = !old_image;		//更新旧图片
		pixel_flow.x = 0;					//输出为零
		pixel_flow.y = 0;					//输出为零
		pixel_flow.match_num = 0;	//输出为零
		return;										//结束检测
	}
	
	//能运行到这里说明在新图片中已经找到特征点并且在旧图片中也找到了匹配点。
	//下面接着判断：
	
	pixel_flow.match_num = match_count;	//匹配数量输出
	
	float flowx = 0.0f;
	float flowy = 0.0f;
	
	//计算最终的光流
	for (uint8_t i = 0; i < match_count; i++)
	{
		flowx += flow_param.dirsx[i];
		flowy += flow_param.dirsy[i];
	}
	
	flowx = flowx / match_count;
	flowy = flowy / match_count;
	
	//如果输出太小,位移不到1像素
	if( flowx*flowx<1 && flowy*flowy<1 )
	{
		pixel_flow.x = 0;				//输出为零
		pixel_flow.y = 0;				//输出为零
		old_image = old_image;	//保留旧图片
	}
	else
	{
		//光流原始数据输出(放大10倍)
		pixel_flow.x = +flowx*10;
		pixel_flow.y = -flowy*10;
		
		//更新旧图片
		old_image = !old_image;
	}
}

u8 old_image=0;//控制新图片用的是缓存1还是缓存2
u8 image1_2x[image_size_2x],image2_2x[image_size_2x];//缩小2倍后的图片缓存

u32 data_len;//DMA数据传输数量
u32 Image0[1];
u32 Image1[buf_size];
u32 Image2[buf_size];

void Image_Duty(void)
{
	u8 *p;
	
	//图片1采集完毕并且没有处理
	if(W_page1==1 && R_page1==0)
	{
		p=(u8*)(Image1);
		Update_Image((u8*)Image1);
		showimage(p);
		//USART_DMACmd(USART2,USART_DMAReq_Tx,ENABLE);
//		USART2_Show_Photo(p);
//		MYDMA_Enable(DMA1_Stream6,OV7740_SIZE);
//	while(1){
//		if(DMA_GetFlagStatus(DMA1_Stream6,DMA_FLAG_TCIF6)!=RESET)//等待DMA2_Steam7传输完成
//				{ 
//					DMA_ClearFlag(DMA1_Stream6,DMA_FLAG_TCIF6);//清除DMA2_Steam7传输完成标志
//					break; 
//		        }
//			}
		ov_frame++;
		R_page1 = 1;
		LED0 = !LED0;
	}
	//图片2采集完毕并且没有处理
	else if(W_page2==1 && R_page2==0)
	{
		p=(u8*)(Image2);
		Update_Image((u8*)Image2);
		showimage(p);
//		USART_DMACmd(USART2,USART_DMAReq_Tx,ENABLE);
//		USART2_Show_Photo(p);
//		MYDMA_Enable(DMA1_Stream6,OV7740_SIZE);
//			while(1){
//		if(DMA_GetFlagStatus(DMA1_Stream6,DMA_FLAG_TCIF6)!=RESET)//等待DMA2_Steam7传输完成
//				{ 
//					DMA_ClearFlag(DMA1_Stream6,DMA_FLAG_TCIF6);//清除DMA2_Steam7传输完成标志
//					break; 
//		        }
//			}
		ov_frame++;
		R_page2 = 1;
		LED0 = !LED0;
	}
}

//图片数据更新
void Update_Image(u8 *p)
{
	//LCD显示，调试使用
	#ifdef LCD_SHOW
		//LCD_Show();
	#endif
	
	//old_image=0:表示image1是旧图片
	//old_image=1:表示image2是旧图片
	
	//image1是旧图片，需要使用image2_2x保存缩小的图片
	if(old_image==0)
	{
		Copy_Image(p);											//去掉UV值
		image_zoom(p,image2_2x); 						//缩小两倍
		Photo_Duty(image2_2x); 							//图片对比度最大化
		compute_flow(image2_2x,image1_2x);	//计算光流
		Pixel_Flow_Fix();										//数据融合
	}
	//image2是旧图片，需要使用image1_2x保存缩小的图片
	else
	{
		Copy_Image(p);											//去掉UV值
		image_zoom(p,image1_2x); 						//缩小两倍
		Photo_Duty(image1_2x); 							//图片对比度最大化
		compute_flow(image1_2x,image2_2x);	//计算光流
		Pixel_Flow_Fix();										//数据融合
	}
}

volatile u32 flow_duty_time;
#define GYRO_MAX 20.0f

//光流数据滤波融合
void Pixel_Flow_Fix(void)
{
	float dT;
	
	////////////////////////*光流速度积分位移*////////////////////////////
	
	//周期时间
	dT = 0.000001f *(GetSysTime_us() - flow_duty_time);
	flow_duty_time = GetSysTime_us();
	
	//光流原始值积分==>>位移
	pixel_flow.x_i += pixel_flow.x * dT;
	pixel_flow.y_i += pixel_flow.y * dT;
	
	//积分位移和传感器倾角融合
	pixel_flow.fix_x_i = pixel_flow.x_i + 18.0f*tan(imu_data.rol*angle_to_rad);
	pixel_flow.fix_y_i = pixel_flow.y_i - 18.0f*tan(imu_data.pit*angle_to_rad);
	
	////////////////////////*光流数据滤波*///////////////////////////////
	
	//滑动窗口滤波
	Flow_Window_Filter( &pixel_flow.x, &pixel_flow.y, &pixel_flow.match_num );
	
	//低通滤波
	pixel_flow.fix_x += ( pixel_flow.x - pixel_flow.fix_x ) * 0.2f;
	pixel_flow.fix_y += ( pixel_flow.y - pixel_flow.fix_y ) * 0.2f;
	pixel_flow.fix_match_num += ( pixel_flow.match_num - pixel_flow.fix_match_num ) * 0.2f;
	
	////////////////////////*角速度数据滤波*///////////////////////////////
	
	//读取角速度原始数据
	pixel_flow.gyr_x = -sensor_val[G_Y]*0.015f;
	pixel_flow.gyr_y = +sensor_val[G_X]*0.015f;
	
	//滑动窗口滤波
	Gyro_Window_Filter(&pixel_flow.gyr_x,&pixel_flow.gyr_y);
	
	//低通滤波
	pixel_flow.fix_gyr_x +=  ( pixel_flow.gyr_x - pixel_flow.fix_gyr_x ) * 0.2f;
	pixel_flow.fix_gyr_y +=  ( pixel_flow.gyr_y - pixel_flow.fix_gyr_y ) * 0.2f;
	
	///////////////////*光流数据与角速度数据融合*//////////////////////////
	
	//如果角速度太大，光流数据不可靠输出0.
	if( ABS(pixel_flow.fix_gyr_x) >= GYRO_MAX )
		pixel_flow.out_x = 0;
	else
		pixel_flow.out_x = (pixel_flow.fix_x - pixel_flow.fix_gyr_x) * ( GYRO_MAX - ABS(pixel_flow.fix_gyr_x) ) / GYRO_MAX;
	
	if( ABS(pixel_flow.fix_gyr_y) >= GYRO_MAX )
		pixel_flow.out_y = 0;
	else
		pixel_flow.out_y = (pixel_flow.fix_y - pixel_flow.fix_gyr_y) * ( GYRO_MAX - ABS(pixel_flow.fix_gyr_y) ) / GYRO_MAX;
	
	//融合后再次滤波
	pixel_flow.out_fix_x +=  ( pixel_flow.out_x - pixel_flow.out_fix_x ) * 0.2f;
	pixel_flow.out_fix_y +=  ( pixel_flow.out_y - pixel_flow.out_fix_y ) * 0.2f;
	
	///////////////////*光流数据与高度数据融合*//////////////////////////
	
	/*由于环境亮度对激光测距数据的影响太大，
		在这里没法判断测距数据是否稳定、可靠、有效，
		并且高度融合的算法也比较简单，就是一个比例的关系，
		所以高度融合请自己在MUC端处理。*/
}

