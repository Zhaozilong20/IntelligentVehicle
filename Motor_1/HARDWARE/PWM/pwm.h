#ifndef _PWM_H
#define _PWM_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
							  
////////////////////////////////////////////////////////////////////////////////// 	

#define PWM_MAX   999
#define PWM_MIN   0

//�ٶȴ���
extern int16_t Target_Speed[4];

extern int16_t temp[4];

/*************����ٶ�PI��ʱ���׼*******************/
void TIM7_Int_Init(u16 arr,u16 psc);


void TIM1_PWM_Init(u16 arr,u16 psc); //  �����PWM�źų�ʼ������

#endif
