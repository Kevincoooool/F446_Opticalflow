#include "include.h"
#include "camera.h" 
#include "stdio.h"

u8 ov_frame,frame; //֡��

//DCMI DMA����
//DMA_Memory0BaseAddr:�洢����ַ    ��Ҫ�洢����ͷ���ݵ��ڴ��ַ(Ҳ�����������ַ)
//DMA_BufferSize:�洢������    0~65535
//DMA_MemoryDataSize:�洢��λ��  
//DMA_MemoryDataSize:�洢��λ��    @defgroup DMA_memory_data_size :DMA_MemoryDataSize_Byte/DMA_MemoryDataSize_HalfWord/DMA_MemoryDataSize_Word
//DMA_MemoryInc:�洢��������ʽ  @defgroup DMA_memory_incremented_mode  /** @defgroup DMA_memory_incremented_mode : DMA_MemoryInc_Enable/DMA_MemoryInc_Disable
void DCMI_DMA_Init(u32 DMA_Memory0BaseAddr,u16 DMA_BufferSize,u32 DMA_MemoryDataSize,u32 DMA_MemoryInc)
{ 
	NVIC_InitTypeDef NVIC_InitStructure;
	DMA_InitTypeDef  DMA_InitStructure;
	
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);//DMA2ʱ��ʹ�� 
	DMA_DeInit(DMA2_Stream1);
	while (DMA_GetCmdStatus(DMA2_Stream1) != DISABLE){}//�ȴ�DMA2_Stream1������ 
	
  /* ���� DMA Stream */
  DMA_InitStructure.DMA_Channel = DMA_Channel_1;  //ͨ��1 DCMIͨ�� 
  DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&DCMI->DR;//�����ַΪ:DCMI->DR
  DMA_InitStructure.DMA_Memory0BaseAddr = DMA_Memory0BaseAddr;//DMA �洢��0��ַ
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;//���赽�洢��ģʽ
  DMA_InitStructure.DMA_BufferSize = DMA_BufferSize;//���ݴ����� 
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//���������ģʽ
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc;//�洢������ģʽ
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;//�������ݳ���:32λ
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize;//�洢�����ݳ��� 
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;// ʹ��ѭ��ģʽ 
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;//�����ȼ�
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Enable; //FIFOģʽ        
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;//ʹ��ȫFIFO 
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;//����ͻ�����δ���
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;//�洢��ͻ�����δ���
		
	DMA_Init(DMA2_Stream1, &DMA_InitStructure);//��ʼ��DMA Stream
		
//	/*DMA double buffer mode*/
//	//DMA_DoubleBufferModeConfig(DMA2_Stream1,DMA_Memory0BaseAddr,DMA_Memory_0);//DMA_Memory_0���ȱ�����
//	DMA_DoubleBufferModeConfig(DMA2_Stream1,DMA_Memory1BaseAddr,DMA_Memory_1);
//	
//	DMA_DoubleBufferModeCmd(DMA2_Stream1,ENABLE);
	
	NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	//DMA_ITConfig(DMA2_Stream1,DMA_IT_TC,ENABLE);//������������ж�
	//DMA_ITConfig(DMA2_Stream1,DMA_IT_HT,ENABLE);//�������봫��
} 

////DMA��������ж�
//void DMA2_Stream1_IRQHandler(void)
//{
//	if(DMA_GetITStatus(DMA2_Stream1,DMA_IT_HTIF1) != RESET)
//	{
//		DMA_ClearITPendingBit(DMA2_Stream1,DMA_IT_HTIF1);
//		Dma_Buf1_Ok = 1;
//	}
//	if(DMA_GetITStatus(DMA2_Stream1,DMA_IT_TCIF1) != RESET)
//	{
//		DMA_ClearITPendingBit(DMA2_Stream1,DMA_IT_TCIF1);
//		
//		Dma_Buf2_Ok = 1;
//	}
//}

//DCMI��ʼ��
void My_DCMI_Init(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	DCMI_InitTypeDef DCMI_InitStructure;
	
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOC|RCC_AHB1Periph_GPIOE, ENABLE);//ʹ��GPIOA B C E ʱ��
	RCC_AHB2PeriphClockCmd(RCC_AHB2Periph_DCMI,ENABLE);//ʹ��DCMIʱ��
 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_6;//PA4/6   ���ù������
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; //���ù������
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;// PB7  ���ù������
  GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12;//PC6/7/8/9/11 ���ù������
  GPIO_Init(GPIOC, &GPIO_InitStructure);//��ʼ��	


	GPIO_PinAFConfig(GPIOA,GPIO_PinSource4,GPIO_AF_DCMI); //PA4,AF13  DCMI_HSYNC
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource6,GPIO_AF_DCMI); //PA6,AF13  DCMI_PCLK  
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource6,GPIO_AF_DCMI); //PB7,AF13  DCMI_VSYNC 
 	GPIO_PinAFConfig(GPIOB,GPIO_PinSource7,GPIO_AF_DCMI); //PB7,AF13  DCMI_VSYNC 
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource8,GPIO_AF_DCMI); //PB7,AF13  DCMI_VSYNC 
 	GPIO_PinAFConfig(GPIOB,GPIO_PinSource9,GPIO_AF_DCMI); //PB7,AF13  DCMI_VSYNC 
 	GPIO_PinAFConfig(GPIOC,GPIO_PinSource6,GPIO_AF_DCMI); //PC6,AF13  DCMI_D0  
 	GPIO_PinAFConfig(GPIOC,GPIO_PinSource7,GPIO_AF_DCMI); //PC7,AF13  DCMI_D1 
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource8,GPIO_AF_DCMI); //PC8,AF13  DCMI_D2
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource9,GPIO_AF_DCMI); //PC9,AF13  DCMI_D3
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource10,GPIO_AF_DCMI);//PC11,AF13 DCMI_D4 
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource11,GPIO_AF_DCMI); //PB6,AF13  DCMI_D5 
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource12,GPIO_AF_DCMI); //PE5,AF13  DCMI_D6

	DCMI_DeInit();//���ԭ�������� 
 
  DCMI_InitStructure.DCMI_CaptureMode=DCMI_CaptureMode_Continuous;//����ģʽ
	DCMI_InitStructure.DCMI_CaptureRate=DCMI_CaptureRate_All_Frame;//ȫ֡����
	DCMI_InitStructure.DCMI_ExtendedDataMode= DCMI_ExtendedDataMode_8b;//8λ���ݸ�ʽ  
	DCMI_InitStructure.DCMI_HSPolarity = DCMI_HSPolarity_Low;//HSYNC �͵�ƽ��Ч
	DCMI_InitStructure.DCMI_PCKPolarity= DCMI_PCKPolarity_Rising;//PCLK ��������Ч
	DCMI_InitStructure.DCMI_SynchroMode= DCMI_SynchroMode_Hardware;//Ӳ��ͬ��HSYNC,VSYNC
	DCMI_InitStructure.DCMI_VSPolarity=DCMI_VSPolarity_High;//VSYNC �͵�ƽ��Ч
	DCMI_Init(&DCMI_InitStructure);

	DCMI_ITConfig(DCMI_IT_FRAME,ENABLE);//����֡�ж� 

	DCMI_Cmd(ENABLE);	//DCMIʹ��

  NVIC_InitStructure.NVIC_IRQChannel = DCMI_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;//��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ�����
} 

//DCMI�жϷ�����
void DCMI_IRQHandler(void)
{
	if(DCMI_GetITStatus(DCMI_IT_FRAME)==SET)
	{
		DCMI_ClearITPendingBit(DCMI_IT_FRAME);
		DMA_Write_Ctrl();
	}
} 

u8 W_page;

u8 R_page1=1,R_page2=1;//���ڶ���־
u8 W_page1=1,W_page2=1;//����д��־

void DMA_Write_Ctrl(void)
{
	if		 (W_page == 1)	W_page1 = 1,R_page1=0,W_page2 = 0;
	else if(W_page == 2)	W_page1 = 0,R_page2=0,W_page2 = 1;
	
	// ��2ҳд���     ��1ҳ�������
	if(W_page2 == 1 && R_page1 == 1)
	{
		//����д��һҳ
		W_page = 1;				
		DMA_Write_Image((u32)&Image1,DMA_MemoryInc_Enable);	
	}
	//      ��1ҳд���     ��2ҳ�������
	else if(W_page1 == 1 && R_page2 == 1)
	{
		//����д�ڶ�ҳ
		W_page = 2;
		DMA_Write_Image((u32)&Image2,DMA_MemoryInc_Enable);
	}
	else
	{
		//�����ݣ���������ٶȸ����ϣ��ᵼ�¶����ݣ�
		W_page = 0;		
		DMA_Write_Image((u32)&Image0,DMA_MemoryInc_Disable);
	}
}

//ImageBaseAddr:�洢����ַ
//DMA_MemoryInc:�洢��������ʽ 
//�ڻ�����д��ͼƬ
void DMA_Write_Image(u32 ImageBaseAddr,u32 DMA_MemoryInc)
{
	//��һ������ֹ������
	DMA_Cmd(DMA2_Stream1, DISABLE);
	while (DMA_GetCmdStatus(DMA2_Stream1) != DISABLE){}
	
	//�ڶ�������ȡ�˴����ݴ���ĳ���
	data_len=buf_size-DMA_GetCurrDataCounter(DMA2_Stream1);
		
	//�����������´�����ָ��
	DMA2_Stream1->M0AR = ImageBaseAddr;
		
	//���Ĳ����������ݴ��䳤��
	DMA2_Stream1->NDTR=buf_size;
		
	//���岽�����ô���������ģʽ
	DMA2_Stream1->CR &= 0xFFFFFBFF;
	DMA2_Stream1->CR |= DMA_MemoryInc;
		
	//�������������
	DMA_Cmd(DMA2_Stream1, ENABLE);
}

//DCMI,��������
void DCMI_Start(void)
{ 
	DMA_Cmd(DMA2_Stream1, ENABLE);//����DMA2,Stream1 
	DCMI_CaptureCmd(ENABLE);//DCMI����ʹ��  
}
//DCMI,�رմ���
void DCMI_Stop(void)
{
  DCMI_CaptureCmd(DISABLE);//DCMI����ʹ�ر�	
	while(DCMI->CR&0X01);		//�ȴ��������  	
	DMA_Cmd(DMA2_Stream1,DISABLE);//�ر�DMA2,Stream1
} 
 







