#ifndef FUNCTION_H
#define FUNCTION_H

#include "include.h"

typedef struct
{
	u8 check_flag;
	u16 err_flag;
	s16 cnt_2ms;
	s16 cnt_6ms;
	s16 cnt_10ms;
	s16 cnt_20ms;
	s16 cnt_50ms;
	s16 cnt_1000ms;
}loop_t;

void 	Loop_check(void);
void Set_Check(void);
#endif
