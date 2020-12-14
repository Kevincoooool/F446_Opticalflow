#include "include.h"
#include "camera.h" 
#include "ov7670cfg.h"	
#include "ov7740cfg.h"

//��ʼ��OV7670
//����0:�ɹ�
//��������ֵ:�������
u16 reg=0;
u8 Camera_Init(void)
{
	
	
	//����IO   	   
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; //�������
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; //�������
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��

 	OV_PWDN=0;
	delay_ms(10);
	OV_RST=0;	//��λ
	delay_ms(10);
	OV_RST=1;	//������λ 
	
	SCCB_Init();        			//��ʼ��SCCB ��IO��
 	SCCB_WR_Reg(0X12, 0x80);	//��λOV7670
	delay_ms(50);
	
	reg=SCCB_RD_Reg(0X0a);	//��ȡ����ID �߰�λ
	reg<<=8;
	reg|=SCCB_RD_Reg(0X0b);	//��ȡ����ID �Ͱ�λ
	
	if(reg==OV7670_PID)
	{
		OV7670_Init();
		return 0;
	}   
	else if(reg==OV7740_PID)
	{
		OV7740_Init();
		return 0;
	}
	
  return 0x02;
} 


