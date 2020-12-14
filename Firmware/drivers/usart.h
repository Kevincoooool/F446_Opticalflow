#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "stm32f4xx_conf.h"
#include "include.h"

#define USART_REC_LEN  			200  	//定义最大接收字节数 200
	  	
extern u8  USART_RX_BUF[USART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u16 USART_RX_STA;         		//接收状态标记	

void USART2_init(u32 bound);
void Uart2_Put_Buf(unsigned char *DataToSend , u8 data_num);
void USART2_Send(unsigned char *DataToSend ,u8 data_num);
void USART2_Show_Photo(u8 *p);
#endif


