#include "timer9.h"
#include "stepmotor.h"
//////////////////////////////////////////////////////////////////////////////////	 

////////////////////////////////////////////////////////////////////////////////// 	 

//ͨ�ö�ʱ��9�жϳ�ʼ��
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//��ʱ�����ʱ����㷽��:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=��ʱ������Ƶ��,��λ:Mhz
//����ʹ�õ��Ƕ�ʱ��9!
//��ʱ��9��ʱ�ж�
void TIM9_Int_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM9,ENABLE);  ///ʹ��TIM9ʱ��
	
  TIM_TimeBaseInitStructure.TIM_Period = arr; 	//�Զ���װ��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //��ʱ����Ƶ
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM9,&TIM_TimeBaseInitStructure);//��ʼ��TIM9
	
	TIM_ITConfig(TIM9,TIM_IT_Update,ENABLE); //����ʱ��9�����ж�
	TIM_Cmd(TIM9,ENABLE); //ʹ�ܶ�ʱ��9
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM6_DAC_IRQn; //��ʱ��6�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x03; 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x01; 
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void TIM6_DAC_IRQHandler (void)
{
	extern int16_t PUL[];
	extern u16 adc[4];
	if(TIM_GetITStatus(TIM6,TIM_IT_Update)==SET)            // �ж�������ж�
	{
		 if(PUL[0]<0){
		    step_dir1=1;
			  step_motor1=!step_motor1;
			  PUL[0]++;
		 }
		 else if(PUL[0]>0){
		    step_dir1=0;
			  step_motor1=!step_motor1;
			  PUL[0]--;
		 }
		 else if(PUL[0]=0){
		    step_motor1=0;
		 }
		 
		 if(PUL[1]<0){
		    step_dir2=1;
			  step_motor2=!step_motor2;
			  PUL[1]++;
		 }
		 else if(PUL[1]>0){
		    step_dir2=0;
			  step_motor2=!step_motor2;
			  PUL[1]--;
		 }
		 else if(PUL[1]=0){
		    step_motor2=0;
		 }
		 
		 if(PUL[2]<0){
		    step_dir3=1;
			  step_motor3=!step_motor3;
			  PUL[2]++;
		 }
		 else if(PUL[2]>0){
		    step_dir3=0;
			  step_motor3=!step_motor3;
			  PUL[2]--;
		 }
		 else if(PUL[2]=0){
		    step_motor3=0;
		 }
		 
		 //LED1=!LED1;//DS1��ת
	}
	TIM_ClearITPendingBit(TIM6,TIM_IT_Update);              // ����жϱ�־λ
}


