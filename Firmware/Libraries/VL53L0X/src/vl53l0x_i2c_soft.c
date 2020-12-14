/******************** (C) COPYRIGHT 2017 ANO Tech ********************************
 * 作者    ：匿名科创
 * 官网    ：www.anotc.com
 * 淘宝    ：anotc.taobao.com
 * 技术Q群 ：190169595
 * 描述    ：软件模拟i2c通信
**********************************************************************************/
#include "vl53l0x_i2c_soft.h"

volatile u8 _VL53L0X_I2C_FastMode;

void _VL53L0X_I2c_Soft_delay()
{
    if ( _VL53L0X_I2C_FastMode )
    {
        __nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
		__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
		__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
		__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
		__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
    }
	else
	{
		u8 i = 200;
        while ( i-- )
			__nop();
	}
}

void _VL53L0X_I2c_Soft_Init()
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	GPIO_StructInit(&GPIO_InitStructure);
	
	RCC_AHB1PeriphClockCmd ( _VL53L0X__RCC_I2C , ENABLE );
	
	GPIO_InitStructure.GPIO_Pin =  _VL53L0X_I2C_Pin_SCL | _VL53L0X_I2C_Pin_SDA;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init ( _VL53L0X__GPIO_I2C, &GPIO_InitStructure );
	
	_VL53L0X_I2C_FastMode = 0;
	
	//VL_XSH引脚配置
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	VL53L0X_Xshut=1;
}

int _VL53L0X_I2c_Soft_Start()
{
    _VL53L0X_SDA_H;
    _VL53L0X_SCL_H;
    _VL53L0X_I2c_Soft_delay();
    if ( !_VL53L0X_SDA_read ) 
		return 0;	//SDA线为低电平则总线忙,退出
    _VL53L0X_SDA_L;
    _VL53L0X_I2c_Soft_delay();
    if ( _VL53L0X_SDA_read ) 
		return 0;	//SDA线为高电平则总线出错,退出
    _VL53L0X_SDA_L;
    _VL53L0X_I2c_Soft_delay();
    return 1;

}

void _VL53L0X_I2c_Soft_Stop()
{
    _VL53L0X_SCL_L;
    _VL53L0X_I2c_Soft_delay();
    _VL53L0X_SDA_L;
    _VL53L0X_I2c_Soft_delay();
    _VL53L0X_SCL_H;
    _VL53L0X_I2c_Soft_delay();
    _VL53L0X_SDA_H;
    _VL53L0X_I2c_Soft_delay();
}

void _VL53L0X_I2c_Soft_Ask()
{
    _VL53L0X_SCL_L;
    _VL53L0X_I2c_Soft_delay();
    _VL53L0X_SDA_L;
    _VL53L0X_I2c_Soft_delay();
    _VL53L0X_SCL_H;
    _VL53L0X_I2c_Soft_delay();
    _VL53L0X_SCL_L;
    _VL53L0X_I2c_Soft_delay();
}

void _VL53L0X_I2c_Soft_NoAsk()
{
    _VL53L0X_SCL_L;
    _VL53L0X_I2c_Soft_delay();
    _VL53L0X_SDA_H;
    _VL53L0X_I2c_Soft_delay();
    _VL53L0X_SCL_H;
    _VL53L0X_I2c_Soft_delay();
    _VL53L0X_SCL_L;
    _VL53L0X_I2c_Soft_delay();
}

int _VL53L0X_I2c_Soft_WaitAsk ( void ) 	 //返回为:=1无ASK,=0有ASK
{
    u8 ErrTime = 0;
    _VL53L0X_SCL_L;
    _VL53L0X_I2c_Soft_delay();
    _VL53L0X_SDA_H;
    _VL53L0X_I2c_Soft_delay();
    _VL53L0X_SCL_H;
    _VL53L0X_I2c_Soft_delay();
    while ( _VL53L0X_SDA_read )
    {
        ErrTime++;
        if ( ErrTime > 50 )
        {
            _VL53L0X_I2c_Soft_Stop();
            return 1;
        }
    }
    _VL53L0X_SCL_L;
    _VL53L0X_I2c_Soft_delay();
    return 0;
}

void _VL53L0X_I2c_Soft_SendByte ( u8 SendByte ) //数据从高位到低位//
{
//    u8 i = 8;
//    while ( i-- )
//    {
//        SCL_L;
//        I2c_Soft_delay();
//        if ( SendByte & 0x80 )
//            SDA_H;
//        else
//            SDA_L;
//        SendByte <<= 1;
//        I2c_Soft_delay();
//        SCL_H;
//        I2c_Soft_delay();
//    }
//    SCL_L;
	u8 i = 8;
    while ( i-- )
    {
        _VL53L0X_SCL_L;
        _VL53L0X_I2c_Soft_delay();
        if ( SendByte & 0x80 )
            _VL53L0X_SDA_H;
        else
            _VL53L0X_SDA_L;
        SendByte <<= 1;
        //I2c_Soft_delay();
        _VL53L0X_SCL_H;
        _VL53L0X_I2c_Soft_delay();
    }
    _VL53L0X_SCL_L;
}

//读1个字节，ack=1时，发送ACK，ack=0，发送NACK
u8 _VL53L0X_I2c_Soft_ReadByte ( u8 ask ) //数据从高位到低位//
{
    u8 i = 8;
    u8 ReceiveByte = 0;

    _VL53L0X_SDA_H;
    while ( i-- )
    {
        ReceiveByte <<= 1;
        _VL53L0X_SCL_L;
        _VL53L0X_I2c_Soft_delay();
        _VL53L0X_SCL_H;
        _VL53L0X_I2c_Soft_delay();
        if ( _VL53L0X_SDA_read )
        {
            ReceiveByte |= 0x01;
        }
    }
    _VL53L0X_SCL_L;

    if ( ask )
        _VL53L0X_I2c_Soft_Ask();
    else
        _VL53L0X_I2c_Soft_NoAsk();
    return ReceiveByte;
}


// IIC写一个字节数据
u8 _VL53L0X_IIC_Write_1Byte ( u8 SlaveAddress, u8 REG_Address, u8 REG_data )
{
    _VL53L0X_I2c_Soft_Start();
    _VL53L0X_I2c_Soft_SendByte ( SlaveAddress  );
    if ( _VL53L0X_I2c_Soft_WaitAsk() )
    {
        _VL53L0X_I2c_Soft_Stop();
        return 1;
    }
    _VL53L0X_I2c_Soft_SendByte ( REG_Address );
    _VL53L0X_I2c_Soft_WaitAsk();
    _VL53L0X_I2c_Soft_SendByte ( REG_data );
    _VL53L0X_I2c_Soft_WaitAsk();
    _VL53L0X_I2c_Soft_Stop();
    return 0;
}

// IIC读1字节数据
u8 _VL53L0X_IIC_Read_1Byte ( u8 SlaveAddress, u8 REG_Address, u8 *REG_data )
{
    _VL53L0X_I2c_Soft_Start();
    _VL53L0X_I2c_Soft_SendByte ( SlaveAddress  );
    if ( _VL53L0X_I2c_Soft_WaitAsk() )
    {
        _VL53L0X_I2c_Soft_Stop();
        return 1;
    }
    _VL53L0X_I2c_Soft_SendByte ( REG_Address );
    _VL53L0X_I2c_Soft_WaitAsk();
    _VL53L0X_I2c_Soft_Start();
    _VL53L0X_I2c_Soft_SendByte ( SlaveAddress << 1 | 0x01 );
    _VL53L0X_I2c_Soft_WaitAsk();
    *REG_data = _VL53L0X_I2c_Soft_ReadByte ( 0 );
    _VL53L0X_I2c_Soft_Stop();
    return 0;
}

// IIC写n字节数据
u8 _VL53L0X_IIC_Write_nByte ( u8 SlaveAddress, u8 REG_Address, u8 len, u8 *buf )
{
    _VL53L0X_I2c_Soft_Start();
    _VL53L0X_I2c_Soft_SendByte ( SlaveAddress  );
    if ( _VL53L0X_I2c_Soft_WaitAsk() )
    {
        _VL53L0X_I2c_Soft_Stop();
        return 1;
    }
    _VL53L0X_I2c_Soft_SendByte ( REG_Address );
    _VL53L0X_I2c_Soft_WaitAsk();
    while ( len-- )
    {
        _VL53L0X_I2c_Soft_SendByte ( *buf++ );
        _VL53L0X_I2c_Soft_WaitAsk();
    }
    _VL53L0X_I2c_Soft_Stop();
    return 0;
}

// IIC读n字节数据
u8 _VL53L0X_IIC_Read_nByte ( u8 SlaveAddress, u8 REG_Address, u8 len, u8 *buf )
{
    _VL53L0X_I2c_Soft_Start();
    _VL53L0X_I2c_Soft_SendByte ( SlaveAddress  );
    if ( _VL53L0X_I2c_Soft_WaitAsk() )
    {
        _VL53L0X_I2c_Soft_Stop();
        return 1;
    }
    _VL53L0X_I2c_Soft_SendByte ( REG_Address );
    _VL53L0X_I2c_Soft_WaitAsk();

    _VL53L0X_I2c_Soft_Start();
    _VL53L0X_I2c_Soft_SendByte ( SlaveAddress | 0x01 );
    _VL53L0X_I2c_Soft_WaitAsk();
    while ( len )
    {
        if ( len == 1 )
        {
            *buf = _VL53L0X_I2c_Soft_ReadByte ( 0 );
        }
        else
        {
            *buf = _VL53L0X_I2c_Soft_ReadByte ( 1 );
        }
        buf++;
        len--;
    }
    _VL53L0X_I2c_Soft_Stop();
    return 0;
}

void _VL53L0X_IIC_MultCLK(void)
{
	u8 i = 200;
	while(i)
	{
		i--;
		_VL53L0X_I2c_Soft_Stop();
	}
}

/******************* (C) COPYRIGHT 2014 ANO TECH *****END OF FILE************/

