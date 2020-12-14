#ifndef _DATAEXCHANGE_H
#define _DATAEXCHANGE_H

#include "include.h"

void ANO_DT_Send_User(void);
void ANO_DT_Data_Exchange(void);
void ANO_DT_Send_Status(float angle_rol, float angle_pit, float angle_yaw, s32 alt, u8 fly_model, u8 armed);
void ANO_DT_Send_Senser(s16 a_x,s16 a_y,s16 a_z,s16 g_x,s16 g_y,s16 g_z,s16 m_x,s16 m_y,s16 m_z);
void ANO_DT_Data_Receive_Anl(u8 *data_buf,u8 num);
void ANO_DT_Send_PID(u8 group,s16 p1_p,s16 p1_i,s16 p1_d,s16 p2_p,s16 p2_i,s16 p2_d,s16 p3_p,s16 p3_i,s16 p3_d);
void ANO_DT_Data_Receive_Prepare(u8 data);
void Usart_Send_Flow(void);
#endif
