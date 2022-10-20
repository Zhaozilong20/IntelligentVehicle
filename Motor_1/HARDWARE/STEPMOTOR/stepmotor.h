#ifndef __STEPMOTOR_H
#define __STEPMOTOR_H
#include "sys.h"

//////////////////////////////////////////////////////////////////////////////////	 
							  
////////////////////////////////////////////////////////////////////////////////// 	


//LED�˿ڶ���
#define LED0 PAout(6)	// D2
#define LED1 PAout(7)	// D3	 

#define step_motor0 PDout(8)   //  �������������
#define step_motor1 PDout(10)  //
#define step_motor2 PDout(12)  //
#define step_motor3 PDout(14)  // 
#define step_motor4 PEout(7)   //
#define step_motor5 PEout(9)   //
#define step_motor6 PEout(11)  // 

#define step_dir0 PDout(9)     //  �����������
#define step_dir1 PDout(11)    //
#define step_dir2 PDout(13)    //
#define step_dir3 PDout(15)    //
#define step_dir4 PEout(8)     //
#define step_dir5 PEout(10)    //
#define step_dir6 PEout(12)    //
#define step_dir7 PCout(4)    //
#define step_dir8 PCout(5)    //

#define EN_WHEEL     PEout(13)  // 
#define EN_HOOK      PEout(14)  // 
#define EN_OPENMV    PCout(6)  // 

void Step_Init(void);//��ʼ��		 				    
#endif
