#include "include.h"
#include "usart.h"	
#include "DataExchange.h"

u8 USART_RX_BUF[USART_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
u16 USART_RX_STA=0;       //����״̬���	

void USART2_init(u32 bound){
   //GPIO�˿�����
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //ʹ��GPIOAʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);//ʹ��USART2ʱ��
 
	//����1��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART2); //GPIOA9����ΪUSART2
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_USART2); //GPIOA10����ΪUSART2
	
	//USART2�˿�����
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3; //GPIOA9��GPIOA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOA,&GPIO_InitStructure); //��ʼ��PA9��PA10

   //USART2 ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
  USART_Init(USART2, &USART_InitStructure); //��ʼ������1
	
  USART_Cmd(USART2, ENABLE);  //ʹ�ܴ���1 
	
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//��������ж�

	//USART2 NVIC ����
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;//����1�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =2;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ�����
}

u8 TxBuffer[256];
u8 TxCounter=0;
u8 count=0; 

void USART2_IRQHandler(void)                	//����1�жϷ������
{
	u8 Res;
	
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
	{
		Res =USART_ReceiveData(USART2);//(USART2->DR);	//��ȡ���յ�������
		
		ANO_DT_Data_Receive_Prepare(Res); 		 
  } 
	
	//���ͣ�������λ���ж�
	if( USART_GetITStatus(USART2,USART_IT_TXE ) )
	{
				
		USART2->DR = TxBuffer[TxCounter++]; //дDR����жϱ�־          
		if(TxCounter == count)
		{
			USART2->CR1 &= ~USART_CR1_TXEIE;		//�ر�TXE�������жϣ��ж�
		}
	}
} 

void USART2_Send(unsigned char *DataToSend ,u8 data_num)
{
  u8 i;
	for(i=0;i<data_num;i++)
	{
		TxBuffer[count++] = *(DataToSend+i);
	}

	if(!(USART2->CR1 & USART_CR1_TXEIE))
	{
		USART_ITConfig(USART2, USART_IT_TXE, ENABLE); //�򿪷����ж�
	}

}


void USART2_Show_Photo(u8 *p)
{
	u16 x,y,i;
	
//	USART_SendData(USART2,0xFF);
//	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET);
	USART_SendData(USART2,0x00);
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET);
	USART_SendData(USART2,0xff);
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET);
	USART_SendData(USART2,0x01);
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET);
	USART_SendData(USART2,0x01);
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET);
//	for(y=0;y<HIG;y++)
//	{
//		for(x=0;x<WID;x++)
//		{
//			
//			//USART_SendData(USART2,p[y*2*WID+x*2]);
//			USART_SendData(USART2,p[y*2*WID+x*2]);
//			while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET);	
//		}
//	}
	for(i=0; i<(OV7740_SIZE); i++)
    {
        USART_SendData(USART2,((u8 *)(p))[i]);
		while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET);
    }
}

