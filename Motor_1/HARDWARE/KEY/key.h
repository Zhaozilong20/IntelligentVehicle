#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h" 
//////////////////////////////////////////////////////////////////////////////////	 
//STM32F4����ģ��-�⺯���汾
//�Ա����̣�http://mcudev.taobao.com									  
////////////////////////////////////////////////////////////////////////////////// 	 

/*����ķ�ʽ��ͨ��ֱ�Ӳ����⺯����ʽ��ȡIO*/

#define KEY1 		GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_3)	//PE3 



/*���淽ʽ��ͨ��λ��������ʽ��ȡIO*/
/*
#define KEY1 		PEin(3)		//PE3 
*/

void KEY_Init(void);	//IO��ʼ��
u8 KEY_Scan(u8);  		//����ɨ�躯��	

#endif
