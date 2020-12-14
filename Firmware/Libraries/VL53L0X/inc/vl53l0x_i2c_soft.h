#ifndef _VL53L0X_I2C_SOFT_H
#define	_VL53L0X_I2C_SOFT_H

#include "include.h"

#define _VL53L0X_SCL_H         _VL53L0X__GPIO_I2C->BSRRL = _VL53L0X_I2C_Pin_SCL
#define _VL53L0X_SCL_L         _VL53L0X__GPIO_I2C->BSRRH = _VL53L0X_I2C_Pin_SCL
#define _VL53L0X_SDA_H         _VL53L0X__GPIO_I2C->BSRRL = _VL53L0X_I2C_Pin_SDA
#define _VL53L0X_SDA_L         _VL53L0X__GPIO_I2C->BSRRH = _VL53L0X_I2C_Pin_SDA
#define _VL53L0X_SCL_read      _VL53L0X__GPIO_I2C->IDR  & _VL53L0X_I2C_Pin_SCL
#define _VL53L0X_SDA_read      _VL53L0X__GPIO_I2C->IDR  & _VL53L0X_I2C_Pin_SDA

/***************I2C GPIO定义******************/
#define _VL53L0X__GPIO_I2C			GPIOB
#define _VL53L0X_I2C_Pin_SCL		GPIO_Pin_12
#define _VL53L0X_I2C_Pin_SDA		GPIO_Pin_13
#define _VL53L0X__RCC_I2C				RCC_AHB1Periph_GPIOB

#define VL53L0X_Xshut PBout(15)
/*********************************************/
extern volatile u8 _VL53L0X_I2C_FastMode;

void _VL53L0X_I2c_Soft_Init(void);
void _VL53L0X_I2c_Soft_SendByte(u8 SendByte);
u8 _VL53L0X_I2c_Soft_ReadByte(u8);

u8 _VL53L0X_IIC_Write_1Byte(u8 SlaveAddress,u8 REG_Address,u8 REG_data);
u8 _VL53L0X_IIC_Read_1Byte(u8 SlaveAddress,u8 REG_Address,u8 *REG_data);
u8 _VL53L0X_IIC_Write_nByte(u8 SlaveAddress, u8 REG_Address, u8 len, u8 *buf);
u8 _VL53L0X_IIC_Read_nByte(u8 SlaveAddress, u8 REG_Address, u8 len, u8 *buf);

void _VL53L0X_IIC_MultCLK(void);

#endif
