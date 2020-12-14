#include "filter.h"
#include "i2c_soft.h"
#include "ANO_Drv_MPU6050.h"
#include "ANO_Param.h"

u8 mpu6050_buffer[14];

void MPU6050_Read(void)
{
	IIC_Read_nByte(MPU6050_ADDR,MPU6050_RA_ACCEL_XOUT_H,14,mpu6050_buffer);
}


/**************************实现函数********************************************
*函数原型:		u8 IICwriteBit(u8 dev, u8 reg, u8 bitNum, u8 data)
*功　　能:	  读 修改 写 指定设备 指定寄存器一个字节 中的1个位
输入	dev  目标设备地址
reg	   寄存器地址
bitNum  要修改目标字节的bitNum位
data  为0 时，目标位将被清0 否则将被置位
返回   成功 为1 
失败为0
*******************************************************************************/ 
u8 mpu6050_ok = 0;
void IICwriteBit(u8 dev, u8 reg, u8 bitNum, u8 data){
	u8 b;
	IIC_Read_nByte(dev, reg, 1, &b);
	b = (data != 0) ? (b | (1 << bitNum)) : (b & ~(1 << bitNum));
	IIC_Write_1Byte(dev, reg, b);
}
/**************************实现函数********************************************
*函数原型:		u8 IICwriteBits(u8 dev,u8 reg,u8 bitStart,u8 length,u8 data)
*功　　能:	    读 修改 写 指定设备 指定寄存器一个字节 中的多个位
输入	dev  目标设备地址
reg	   寄存器地址
bitStart  目标字节的起始位
length   位长度
data    存放改变目标字节位的值
返回   成功 为1 
失败为0
*******************************************************************************/ 
void IICwriteBits(u8 dev,u8 reg,u8 bitStart,u8 length,u8 data)
{
	
	u8 b,mask;
	IIC_Read_nByte(dev, reg, 1, &b);
	mask = (0xFF << (bitStart + 1)) | 0xFF >> ((8 - bitStart) + length - 1);
	data <<= (8 - length);
	data >>= (7 - bitStart);
	b &= mask;
	b |= data;
	IIC_Write_1Byte(dev, reg, b);
}

/**************************实现函数********************************************
*函数原型:		
*功　　能:	    设置 采样率
*******************************************************************************/
void MPU6050_set_SMPLRT_DIV(uint16_t hz)
{
	IIC_Write_1Byte(MPU6050_ADDR, MPU6050_RA_SMPLRT_DIV,1000/hz - 1);
}


/**************************实现函数********************************************
*函数原型:		void MPU6050_setClockSource(uint8_t source)
*功　　能:	    设置  MPU6050 的时钟源
* CLK_SEL | Clock Source
* --------+--------------------------------------
* 0       | Internal oscillator
* 1       | PLL with X Gyro reference
* 2       | PLL with Y Gyro reference
* 3       | PLL with Z Gyro reference
* 4       | PLL with external 32.768kHz reference
* 5       | PLL with external 19.2MHz reference
* 6       | Reserved
* 7       | Stops the clock and keeps the timing generator in reset
*******************************************************************************/
void MPU6050_setClockSource(uint8_t source)
{
	IICwriteBits(MPU6050_ADDR, MPU6050_RA_PWR_MGMT_1, MPU6050_PWR1_CLKSEL_BIT, MPU6050_PWR1_CLKSEL_LENGTH, source);
	
}
/** Set full-scale gyroscope range.
* @param range New full-scale gyroscope range value
* @see getFullScaleRange()
* @see MPU6050_GYRO_FS_250
* @see MPU6050_RA_GYRO_CONFIG
* @see MPU6050_GCONFIG_FS_SEL_BIT
* @see MPU6050_GCONFIG_FS_SEL_LENGTH
*/
void MPU6050_setFullScaleGyroRange(uint8_t range) {
	IICwriteBits(MPU6050_ADDR, MPU6050_RA_GYRO_CONFIG, MPU6050_GCONFIG_FS_SEL_BIT, MPU6050_GCONFIG_FS_SEL_LENGTH, range);
	IICwriteBits(MPU6050_ADDR, MPU6050_RA_GYRO_CONFIG,7, 3, 0x00);   //不自检
}

/**************************实现函数********************************************
*函数原型:		void MPU6050_setFullScaleAccelRange(uint8_t range)
*功　　能:	    设置  MPU6050 加速度计的最大量程
*******************************************************************************/
void MPU6050_setFullScaleAccelRange(uint8_t range) {
	IICwriteBits(MPU6050_ADDR, MPU6050_RA_ACCEL_CONFIG, MPU6050_ACONFIG_AFS_SEL_BIT, MPU6050_ACONFIG_AFS_SEL_LENGTH, range);
	IICwriteBits(MPU6050_ADDR, MPU6050_RA_ACCEL_CONFIG,7, 3, 0x00);   //不自检

}
/**************************实现函数********************************************
*函数原型:		void MPU6050_setSleepEnabled(uint8_t enabled)
*功　　能:	    设置  MPU6050 是否进入睡眠模式
enabled =1   睡觉
enabled =0   工作
*******************************************************************************/
void MPU6050_setSleepEnabled(uint8_t enabled) {
	IICwriteBit(MPU6050_ADDR, MPU6050_RA_PWR_MGMT_1, MPU6050_PWR1_SLEEP_BIT, enabled);
}

/**************************实现函数********************************************
*函数原型:		void MPU6050_setI2CMasterModeEnabled(uint8_t enabled)
*功　　能:	    设置 MPU6050 是否为AUX I2C线的主机
*******************************************************************************/
void MPU6050_setI2CMasterModeEnabled(uint8_t enabled) {
	IICwriteBit(MPU6050_ADDR, MPU6050_RA_USER_CTRL, MPU6050_USERCTRL_I2C_MST_EN_BIT, enabled);
}

/**************************实现函数********************************************
*函数原型:		void MPU6050_setI2CBypassEnabled(uint8_t enabled)
*功　　能:	    设置 MPU6050 是否为AUX I2C线的主机
*******************************************************************************/
void MPU6050_setI2CBypassEnabled(uint8_t enabled) {
	IICwriteBit(MPU6050_ADDR, MPU6050_RA_INT_PIN_CFG, MPU6050_INTCFG_I2C_BYPASS_EN_BIT, enabled);
}
/**************************实现函数********************************************
*函数原型:		
*功　　能:	    设置低通滤波截止频率
*******************************************************************************/
void MPU6050_setDLPF(uint8_t mode)
{
	IICwriteBits(MPU6050_ADDR, MPU6050_RA_CONFIG, MPU6050_CFG_DLPF_CFG_BIT, MPU6050_CFG_DLPF_CFG_LENGTH, mode);
}

/**************************实现函数********************************************
*函数原型:		void MPU6050_initialize(void)
*功　　能:	    初始化 	MPU6050 以进入可用状态。
*******************************************************************************/
u8 MPU_ID[5];
u8 MPU_Err;
void MPU6050_Init(u16 lpf)
{	
	u16 default_filter = 1;
	u8 temp;
	
	IIC_Read_1Byte(MPU6050_ADDR,MPU6050_RA_WHO_AM_I,&temp);
	if(temp!= 0x68)//检测MPU6050是否连接正常
	{
		MPU_Err = 1;
	}
	else
	{
		MPU_Err = 0;
	}
  switch(lpf)
	{
	case 5:
			default_filter = MPU6050_DLPF_BW_5;
			break;
	case 10:
			default_filter = MPU6050_DLPF_BW_10;
			break;
	case 20:
			default_filter = MPU6050_DLPF_BW_20;
			break;
	case 42:
			default_filter = MPU6050_DLPF_BW_42;
			break;
	case 98:
			default_filter = MPU6050_DLPF_BW_98;
			break;
	case 188:
			default_filter = MPU6050_DLPF_BW_188;
			break;
	case 256:
			default_filter = MPU6050_DLPF_BW_256;
			break;
	default:
			default_filter = MPU6050_DLPF_BW_20;
			break;
	}
	
	delay_ms(10);
	//设备复位
	IIC_Write_1Byte(MPU6050_ADDR,MPU6050_RA_PWR_MGMT_1, 0x80);
	delay_ms(10);
	IIC_Write_1Byte(MPU6050_ADDR,MPU6050_RA_PWR_MGMT_1, 0x00);	
	delay_ms(10);
	MPU6050_setSleepEnabled(0); //进入工作状态
	delay_ms(10);
	MPU6050_setClockSource(MPU6050_CLOCK_PLL_ZGYRO); //设置时钟  0x6b   0x03
	delay_ms(10);
	MPU6050_set_SMPLRT_DIV(1000);  //<1000hz！！！
	delay_ms(10);
	MPU6050_setFullScaleGyroRange(MPU6050_GYRO_FS_2000);//陀螺仪最大量程 +-2000度每秒
	delay_ms(10);
	MPU6050_setFullScaleAccelRange(MPU6050_ACCEL_FS_8);	//加速度度最大量程 +-8G
	delay_ms(10);
	MPU6050_setDLPF(default_filter);  //21hz
	delay_ms(10);
	MPU6050_setI2CMasterModeEnabled(0);	 //不让MPU6050 控制AUXI2C
	delay_ms(10);
	MPU6050_setI2CBypassEnabled(1);	 //主控制器的I2C与	MPU6050的AUXI2C	直通。控制器可以直接访问SPL06
	delay_ms(10);
	IIC_Read_nByte(MPU6050_ADDR, 0x75, 1, &MPU_ID[1]);
	delay_ms(10);
}

void mpu_type(float x,float y,float z)//6050批次问题检测
{
	float length;
	length = my_sqrt(my_pow(x)+my_pow(y)+my_pow(z));
	
	if(length > 1500 && length < 2600)
	{
		ANO_Param.sensor_type = MPU_6050_1;
	}
	else
	{
		ANO_Param.sensor_type = MPU_6050_0;
	}
}
_sensor_st sensor;

s32 sum_temp[7]={0,0,0,0,0,0,0};
u16 sum_cnt = 0;

float acc_3d_cali[6];

u8 acc_3d_step;
u8 acc_3d_calibrate_f;
#define OFFSET_AV_NUM 50

void MPU6050_Data_Offset()
{
	if(sensor.CALIBRATE==1)
	{
    sum_cnt++;
		sum_temp[A_X] += sensor.Acc_I16.x;
		sum_temp[A_Y] += sensor.Acc_I16.y;
		sum_temp[A_Z] += sensor.Acc_I16.z;

		sum_temp[G_X] += sensor.Gyro_I16.x;
		sum_temp[G_Y] += sensor.Gyro_I16.y;
		sum_temp[G_Z] += sensor.Gyro_I16.z;
		
    if( sum_cnt >= OFFSET_AV_NUM )
		{
			ANO_Param.acc_offset.x = sum_temp[A_X]/OFFSET_AV_NUM;
			ANO_Param.acc_offset.y = sum_temp[A_Y]/OFFSET_AV_NUM;
			ANO_Param.acc_offset.z = sum_temp[A_Z]/OFFSET_AV_NUM;
			mpu_type(ANO_Param.acc_offset.x,ANO_Param.acc_offset.y,ANO_Param.acc_offset.z);//6050批次检测（量程不同）
			
			ANO_Param.gyr_offset.x = (float)sum_temp[G_X]/OFFSET_AV_NUM;
			ANO_Param.gyr_offset.y = (float)sum_temp[G_Y]/OFFSET_AV_NUM;
			ANO_Param.gyr_offset.z = (float)sum_temp[G_Z]/OFFSET_AV_NUM;
			
			sum_cnt =0;
			sum_cnt =0;
			
			sensor.CALIBRATE = 0;
			ANO_Param_Save();
			
			sum_temp[A_X] = sum_temp[A_Y] = sum_temp[A_Z] = sum_temp[TEM] = 0;
			sum_temp[G_X] = sum_temp[G_Y] = sum_temp[G_Z] = sum_temp[TEM] = 0;
		}	
	}
}

/*
传感器默认方向
     +x
     |
 +y--|--
     |

0:默认  
1：传感器顺时针90 度
2：传感器顺时针180度
3：传感器顺时针270度
*/

void sensor_dir(u8 dir,float itx,float ity,float itz,float *it_x,float *it_y,float *it_z)
{
	switch(dir)
	{
		case 1: //传感器顺时针90度
				*it_x = ity;
				*it_y = -itx;
				*it_z = itz;
		break;
		case 2://传感器顺时针180度
				*it_x = -itx;
				*it_y = -ity;
				*it_z = itz;
		break;
		case 3://传感器顺时针270度
				*it_x = -ity;
				*it_y = itx;
				*it_z = itz;
		break;
		default://传感器默认方向
			*it_x = itx;
			*it_y = ity;
			*it_z = itz;			
		break;
	}
	
}

#define MPU_WINDOW_NUM 5
#define MPU_STEEPEST_NUM 5

#define MPU_WINDOW_NUM_ACC 20
#define MPU_STEEPEST_NUM_ACC 20

_steepest_st steepest_ax;
_steepest_st steepest_ay;
_steepest_st steepest_az;
_steepest_st steepest_gx;
_steepest_st steepest_gy;
_steepest_st steepest_gz;

s32 steepest_ax_arr[MPU_WINDOW_NUM_ACC ];
s32 steepest_ay_arr[MPU_WINDOW_NUM_ACC ];
s32 steepest_az_arr[MPU_WINDOW_NUM_ACC ];
s32 steepest_gx_arr[MPU_WINDOW_NUM ];
s32 steepest_gy_arr[MPU_WINDOW_NUM ];
s32 steepest_gz_arr[MPU_WINDOW_NUM ];

float sensor_val[6];

void MPU6050_Data_Prepare(void)
{	
	MPU6050_Read();
	
	MPU6050_Data_Offset(); //校准函数

	/*读取buffer原始数据*/
	sensor.Acc_I16.x = ((((int16_t)mpu6050_buffer[0]) << 8) | mpu6050_buffer[1]) ;
	sensor.Acc_I16.y = ((((int16_t)mpu6050_buffer[2]) << 8) | mpu6050_buffer[3]) ;
	sensor.Acc_I16.z = ((((int16_t)mpu6050_buffer[4]) << 8) | mpu6050_buffer[5]) ;
 
	sensor.Gyro_I16.x = ((((int16_t)mpu6050_buffer[ 8]) << 8) | mpu6050_buffer[ 9]) ;
	sensor.Gyro_I16.y = ((((int16_t)mpu6050_buffer[10]) << 8) | mpu6050_buffer[11]) ;
	sensor.Gyro_I16.z = ((((int16_t)mpu6050_buffer[12]) << 8) | mpu6050_buffer[13]) ;

	sensor.Tempreature = ((((int16_t)mpu6050_buffer[6]) << 8) | mpu6050_buffer[7]); //tempreature
	sensor.Ftempreature = sensor.Tempreature/340.0f + 36.5f;
	
	
	/*得出校准后的数据*/
	if(ANO_Param.sensor_type == MPU_6050_0)
	{
		sensor_val[A_X] = sensor.Acc_I16.x - ANO_Param.acc_offset.x ;
		sensor_val[A_Y] = sensor.Acc_I16.y - ANO_Param.acc_offset.y ;
		sensor_val[A_Z] = sensor.Acc_I16.z - ANO_Param.acc_offset.z + 4096;
	}
	else if(ANO_Param.sensor_type == MPU_6050_1)
	{
		sensor_val[A_X] = 2 *(sensor.Acc_I16.x - ANO_Param.acc_offset.x) ;
		sensor_val[A_Y] = 2 *(sensor.Acc_I16.y - ANO_Param.acc_offset.y) ;
		sensor_val[A_Z] = 2 *(sensor.Acc_I16.z - ANO_Param.acc_offset.z + 2048);
	}
	else
	{
		sensor.CALIBRATE = 1;
	}
 
	sensor_val[G_X] = sensor.Gyro_I16.x - ANO_Param.gyr_offset.x ;
	sensor_val[G_Y] = sensor.Gyro_I16.y - ANO_Param.gyr_offset.y ;
	sensor_val[G_Z] = sensor.Gyro_I16.z - ANO_Param.gyr_offset.z ;	
	
	/*梯度下降拟合数据*/	
	steepest_descend(steepest_ax_arr ,MPU_WINDOW_NUM_ACC ,&steepest_ax ,MPU_STEEPEST_NUM_ACC,(s32) sensor_val[A_X]);
	steepest_descend(steepest_ay_arr ,MPU_WINDOW_NUM_ACC ,&steepest_ay ,MPU_STEEPEST_NUM_ACC,(s32) sensor_val[A_Y]);
	steepest_descend(steepest_az_arr ,MPU_WINDOW_NUM_ACC ,&steepest_az ,MPU_STEEPEST_NUM_ACC,(s32) sensor_val[A_Z]);
	steepest_descend(steepest_gx_arr ,MPU_WINDOW_NUM ,&steepest_gx ,MPU_STEEPEST_NUM,(s32) sensor_val[G_X]);
	steepest_descend(steepest_gy_arr ,MPU_WINDOW_NUM ,&steepest_gy ,MPU_STEEPEST_NUM,(s32) sensor_val[G_Y]);
	steepest_descend(steepest_gz_arr ,MPU_WINDOW_NUM ,&steepest_gz ,MPU_STEEPEST_NUM,(s32) sensor_val[G_Z]);

	/*传感器方向调整*/
	sensor_dir( 1,
				(float)steepest_ax.now_out,
				(float)steepest_ay.now_out,
				(float)steepest_az.now_out,
				&sensor.Acc.x ,
				&sensor.Acc.y ,
				&sensor.Acc.z );
						
	sensor_dir( 1,
				(float)steepest_gx.now_out,
				(float)steepest_gy.now_out,
				(float)steepest_gz.now_out,
				&sensor.Gyro.x,
				&sensor.Gyro.y,
				&sensor.Gyro.z);
	
//======================================================================
	/*陀螺仪转换到度每秒*/
	sensor.Gyro_deg.x = sensor.Gyro.x *0.0610361f ;//  /65535 * 4000; +-2000度
	sensor.Gyro_deg.y = sensor.Gyro.y *0.0610361f ;
	sensor.Gyro_deg.z = sensor.Gyro.z *0.0610361f ;
	
	/*加速度计转换到毫米每平方秒*/
	sensor.Acc_mmss.x = sensor.Acc.x *2.392615f ;//   /65535 * 16*9800; +-8G
	sensor.Acc_mmss.y = sensor.Acc.y *2.392615f ;
	sensor.Acc_mmss.z = sensor.Acc.z *2.392615f ;
}

