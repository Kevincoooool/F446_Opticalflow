#ifndef _DCMI_H
#define _DCMI_H
#include "include.h"  									
						  
////////////////////////////////////////////////////////////////////////////////// 	

#define buf_size (WID*HIG/2+1)

extern u32 data_len;
extern u32 Image0[1];
extern u32 Image1[buf_size];
extern u32 Image2[buf_size];

extern u8 ov_frame,frame;
extern u8 R_page1,R_page2;
extern u8 W_page1,W_page2;
extern u8 R_page1_ok,R_page2_ok;

void My_DCMI_Init(void);
void DCMI_DMA_Init(u32 DMA_Memory0BaseAddr,u16 DMA_BufferSize,u32 DMA_MemoryDataSize,u32 DMA_MemoryInc);
void DCMI_Start(void);
void DCMI_Stop(void);

void DMA_Write_Image(u32 ImageBaseAddr,u32 DMA_MemoryInc);
void DMA_Write_Ctrl(void);

#endif





















