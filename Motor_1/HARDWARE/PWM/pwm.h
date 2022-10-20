#ifndef _PWM_H
#define _PWM_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
							  
////////////////////////////////////////////////////////////////////////////////// 	

#define PWM_MAX   999
#define PWM_MIN   0

//速度代号
extern int16_t Target_Speed[4];

extern int16_t temp[4];

/*************电机速度PI的时间基准*******************/
void TIM7_Int_Init(u16 arr,u16 psc);


void TIM1_PWM_Init(u16 arr,u16 psc); //  电机的PWM信号初始化函数

#endif
