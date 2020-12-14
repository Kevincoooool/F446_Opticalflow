
#include "include.h"
#ifdef  USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line)
{ 
  while (1)
  {
		//系统出现错误会进入这个死循环
  }
}
#endif

u8 Init_Finish = 0;
int main(void)
{
	Init_Finish = All_Init();
	
	while(1)
		
	{
		//LCD_ShowString(10,35,"1.14 TFT SPI 135*240",RED);
		//QDTFT_Test_Demo();
		Set_Check();
		Image_Duty();
		//LCD_Clear(WHITE);
		LCD_ShowNum(180,0,frame,2,RED);
		LCD_ShowNum1(180,20,pixel_flow.fix_x_i,5,RED);
		LCD_ShowNum1(180,40,pixel_flow.fix_y_i,5,RED);
	}
}
