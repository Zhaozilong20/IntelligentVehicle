#include "timer.h"
#include "stepmotor.h"
//////////////////////////////////////////////////////////////////////////////////	 

////////////////////////////////////////////////////////////////////////////////// 	 

//ͨ�ö�ʱ��6�жϳ�ʼ��
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//��ʱ�����ʱ����㷽��:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=��ʱ������Ƶ��,��λ:Mhz
//����ʹ�õ��Ƕ�ʱ��6!
//��ʱ��6��ʱ�ж�
void TIM6_Int_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6,ENABLE);  ///ʹ��TIM6ʱ��
	
  TIM_TimeBaseInitStructure.TIM_Period = arr; 	//�Զ���װ��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //��ʱ����Ƶ
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM6,&TIM_TimeBaseInitStructure);//��ʼ��TIM6
	
	TIM_ITConfig(TIM6,TIM_IT_Update,ENABLE); //����ʱ��6�����ж�
	TIM_Cmd(TIM6,ENABLE); //ʹ�ܶ�ʱ��6
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM6_DAC_IRQn; //��ʱ��6�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x03; 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x01; 
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void TIM3_Int_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  ///ʹ��TIM3ʱ��
	
  TIM_TimeBaseInitStructure.TIM_Period = arr; 	//�Զ���װ��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //��ʱ����Ƶ
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);//��ʼ��TIM3
	
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE); //����ʱ��3�����ж�
	TIM_Cmd(TIM3,ENABLE); //ʹ�ܶ�ʱ��3
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn; //��ʱ��3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x03; 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x01; 
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void TIM3_IRQHandler(void)
{
	extern int32_t PUL[7];
	extern u8 dev,dev1,dev2,i,i1,i2;
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)==SET)            // �ж�������ж�
	{
	   if(i1==dev1){
		 if(PUL[0]<0){
		    step_dir0=1;
			  step_motor0=!step_motor0;
			  PUL[0]++;
		 }
		 else if(PUL[0]>0){
		    step_dir0=0;
			  step_motor0=!step_motor0;
			  PUL[0]--;
		 }
		 else if(PUL[0]==0){
		    step_motor0=0;
		 }
		 
		 if(PUL[1]<0){
		    step_dir1=0;
			  step_motor1=!step_motor1;
			  PUL[1]++;
		 }
		 else if(PUL[1]>0){
		    step_dir1=1;
			  step_motor1=!step_motor1;
			  PUL[1]--;
		 }
		 else if(PUL[1]==0){
		    step_motor1=0;
		 }
		 
		 if(PUL[2]<0){
		    step_dir2=0;
			  step_motor2=!step_motor2;
			  PUL[2]++;
		 }
		 else if(PUL[2]>0){
		    step_dir2=1;
			  step_motor2=!step_motor2;
			  PUL[2]--;
		 }
		 else if(PUL[2]==0){
		    step_motor2=0;
		 }
		 
		 if(PUL[3]<0){
		    step_dir3=1;
			  step_motor3=!step_motor3;
			  PUL[3]++;
		 }
		 else if(PUL[3]>0){
		    step_dir3=0;
			  step_motor3=!step_motor3;
			  PUL[3]--;
		 }
		 else if(PUL[3]==0){
		    step_motor3=0;
		 }
		   
		   i1=0;
	   }
		 else{
			 i1++;
		 }
		
	}
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);              // ����жϱ�־λ
}

void TIM6_DAC_IRQHandler (void)
{
	extern int32_t PUL[7];
	extern u8 dev,dev1,dev2,i,i1,i2;
	
	if(TIM_GetITStatus(TIM6,TIM_IT_Update)==SET)            // �ж�������ж�
	{ 
		 if(PUL[6]<0){
		    step_dir6=0;
			  step_dir8=0;
			  step_motor6=!step_motor6;
			  PUL[6]++;
		 }
		 else if(PUL[6]>0){
		    step_dir6=1;
			  step_dir8=1;
			  step_motor6=!step_motor6;
			  PUL[6]--;
		 }
		 else if(PUL[6]==0){
		    step_motor6=0;
		 }
		   
		 
		 if(i==dev){
		 if(PUL[4]<0){
		    step_dir4=0;
			  step_motor4=!step_motor4;
			  PUL[4]++;
		 }
		 else if(PUL[4]>0){
		    step_dir4=1;
			  step_motor4=!step_motor4;
			  PUL[4]--;
		 }
		 else if(PUL[4]==0){
		    step_motor4=0;
		 }
		 
		 if(PUL[5]<0){
		    step_dir5=1;
			  step_dir7=0;
			  step_motor5=!step_motor5;
			  PUL[5]++;
		 }
		 else if(PUL[5]>0){
		    step_dir5=0;
			  step_dir7=1;
			  step_motor5=!step_motor5;
			  PUL[5]--;
		 }
		 else if(PUL[5]==0){
		    step_motor5=0;
		 }
		  i=0;
	   }
		 else{
			 i++;
		 }
		 
		 //LED1=!LED1;//DS1��ת
	}
	TIM_ClearITPendingBit(TIM6,TIM_IT_Update);              // ����жϱ�־λ
}


