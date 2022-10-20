#include "stepmotor.h" 
//////////////////////////////////////////////////////////////////////////////////	 
	  
////////////////////////////////////////////////////////////////////////////////// 	 

//��ʼ��PA6��PA7Ϊ�����.��ʹ���������ڵ�ʱ��		    
//LED IO��ʼ��

void Step_Init(void)
{    	 
  GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//ʹ��GPIOAʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);//ʹ��GPIOCʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);//ʹ��GPIODʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);//ʹ��GPIOEʱ��

  //GPIOA6,A7��ʼ������
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;//LED0��LED1��ӦIO��
	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIO
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_9;//  PC6 7 8
	GPIO_Init(GPIOC, &GPIO_InitStructure);//��ʼ��GPIOD
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;//  PD8 9 10 11 12 13 14 15
	GPIO_Init(GPIOD, &GPIO_InitStructure);//��ʼ��GPIOD
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15; // PE7 8 9 10 11 12 13 14 15
	GPIO_Init(GPIOE, &GPIO_InitStructure);//��ʼ��GPIOE 
	
  GPIO_SetBits(GPIOA,GPIO_Pin_6 | GPIO_Pin_7);//���øߣ�����
  GPIO_SetBits(GPIOC,GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_9);
	GPIO_SetBits(GPIOD,GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);//���øߣ�����
	GPIO_SetBits(GPIOE,GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);//���øߣ�����
	
}






