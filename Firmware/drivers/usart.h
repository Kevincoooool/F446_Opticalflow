#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "stm32f4xx_conf.h"
#include "include.h"

#define USART_REC_LEN  			200  	//�����������ֽ��� 200
	  	
extern u8  USART_RX_BUF[USART_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u16 USART_RX_STA;         		//����״̬���	

void USART2_init(u32 bound);
void Uart2_Put_Buf(unsigned char *DataToSend , u8 data_num);
void USART2_Send(unsigned char *DataToSend ,u8 data_num);
void USART2_Show_Photo(u8 *p);
#endif


