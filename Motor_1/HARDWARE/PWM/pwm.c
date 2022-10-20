#include "pwm.h"
#include "stepmotor.h"
#include "usart.h"
#include "adc.h"

//TIM1 TIM8  PWM���ֳ�ʼ�� 
//PWM�����ʼ��
//arr���Զ���װֵ
//psc��ʱ��Ԥ��Ƶ��

void TIM1_PWM_Init(u16 arr,u16 psc) //  �����PWM�źų�ʼ������
{
	GPIO_InitTypeDef 					 GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef 	 TIM_TimeBaseInitStructure;
	TIM_OCInitTypeDef  				 TIM_OCInitStructure;                   // Ҫ�õ���һЩ�ṹ�������

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);              //���� IO�ڵ�ʱ��
	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;                      // IO���õ���ز�������
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;                    
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_8 | GPIO_Pin_9|          
																 GPIO_Pin_10 | GPIO_Pin_11;         //������������4�� IO �� PWM���
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;                  // ʵ�ʱ���������Ҳ�������õ�  һ����ʱ�� ������·
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;                // ռ�ձȲ�ͬ�� PWM�ź�  ����·�ź�ռ��һ����ʱ������Դ
  GPIO_Init(GPIOA,&GPIO_InitStructure);															// ��·�źŵ�Ƶ����һ�µ�
  
  GPIO_PinAFConfig(GPIOA,GPIO_PinSource8, GPIO_AF_TIM1);            // IO����
  GPIO_PinAFConfig(GPIOA,GPIO_PinSource9, GPIO_AF_TIM1);
  GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_TIM1);
  GPIO_PinAFConfig(GPIOA,GPIO_PinSource11,GPIO_AF_TIM1);
  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);              //������ʱ����ʱ��
	
	//Timer clock = sysclock /(TIM_Prescaler+1) = 168M
	//Period = (TIM counter clock / TIM output clock) - 1 = 20K
	
  TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;      //��ʱ��������������
  TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;  
  TIM_TimeBaseInitStructure.TIM_Prescaler = psc;  
  TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
  TIM_TimeBaseInitStructure.TIM_Period = arr;    
  TIM_TimeBaseInit(TIM1,&TIM_TimeBaseInitStructure);
  
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;              //��ʱ������� ����
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
  TIM_OCInitStructure.TIM_Pulse = 0;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
  TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
  
  TIM_OC1Init(TIM1,&TIM_OCInitStructure);                        // ���ͨ��1 ��ʼ��

  TIM_OCInitStructure.TIM_Pulse = 0;
  TIM_OC2Init(TIM1,&TIM_OCInitStructure);                        // ���ͨ��2 ��ʼ��
  
  TIM_OCInitStructure.TIM_Pulse = 0;
  TIM_OC3Init(TIM1,&TIM_OCInitStructure);                        // ���ͨ��3 ��ʼ��
  
  TIM_OCInitStructure.TIM_Pulse = 0;
  TIM_OC4Init(TIM1,&TIM_OCInitStructure);                        // ���ͨ��4 ��ʼ��
  
  TIM_Cmd(TIM1,ENABLE);
  TIM_CtrlPWMOutputs(TIM1,ENABLE);
}

void Set_Motorpwm(void)
{
	  extern int16_t PWM[];
	  TIM1->CCR1 = 0;  
	  TIM1->CCR2 = 0;
	  TIM1->CCR3 = 0;  
	  TIM1->CCR4 = 0;
	
	  TIM8->CCR1 = 0;  
	  TIM8->CCR2 = 0;
	  TIM8->CCR3 = 0;  
	  TIM8->CCR4 = 0;
    
	  extern int16_t temp[4];
	  
	  if(temp[0]>0){
		TIM1->CCR1 = PWM[0] ;  
		}
		if(temp[0]<0){
		TIM1->CCR2 = PWM[0] ; 
		}
		
		if(temp[1]>0){
		TIM1->CCR3 = PWM[1] ;  
		}
		if(temp[1]<0){
		TIM1->CCR4 = PWM[1] ; 
		}
		
		if(temp[2]>0){
		TIM8->CCR1 = PWM[2] ;  
		}
		if(temp[2]<0){
		TIM8->CCR2 = PWM[2] ; 
		}
		
		if(temp[3]>0){
		TIM8->CCR3 = PWM[3] ;  
		}
		if(temp[3]<0){
		TIM8->CCR4 = PWM[3] ; 
		}
		
}

//void Moter_Init(void)
//{
////	TIM2_ENC_Init();
////	TIM3_ENC_Init();
////	TIM4_ENC_Init();
////	TIM5_ENC_Init();
//	
//	TIM1_PWM_Init(1000-1,84-1);
//	TIM8_PWM_Init(1000-1,84-1);
//	
//	TIM7_Int_Init(500-1,8400-1);
//}

//��ʱ��7��ʱ�ж�
void TIM7_Int_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7,ENABLE);  ///ʹ��TIM7ʱ��
	
  TIM_TimeBaseInitStructure.TIM_Period = arr; 	//�Զ���װ��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //��ʱ����Ƶ
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM7,&TIM_TimeBaseInitStructure);//��ʼ��TIM7
	
	TIM_ITConfig(TIM7,TIM_IT_Update,ENABLE); //����ʱ��7�����ж�
	TIM_Cmd(TIM7,ENABLE); //ʹ�ܶ�ʱ��7
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM7_IRQn; //��ʱ��7�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x03; 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x01; 
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}


/* 2.8�汾�ĵ�·������������ӳ������
 *���    ����������     ��������ʱ��   ��������������    ����:EN            ��ʱ��ͨ��
 *  A       D12,D13       TIM4          Encoder_Date[2]   PE9  11           TIM1->CH1  CH2
 *  B       A6 ,A7        TIM3          Encoder_Date[1]   PE13 14           TIM1->CH3  CH4
 *  C       A0 ,A1        TIM5          Encoder_Date[3]   PC6  7            TIM8->CH1  CH2
 *  D       A15,B3        TIM2          Encoder_Date[0]   PC8  9            TIM8->CH3  CH4
 */

void TIM7_IRQHandler(void)
{
	extern u16 adc[4];
	if(TIM_GetITStatus(TIM7,TIM_IT_Update)==SET)            // �ж�������ж�
	{
		
		adc[0]=Get_Adc(ADC_Channel_8);
		adc[1]=Get_Adc(ADC_Channel_10);
		adc[2]=Get_Adc(ADC_Channel_11);
		adc[3]=Get_Adc(ADC_Channel_12);
		
	}
	TIM_ClearITPendingBit(TIM7,TIM_IT_Update);              // ����жϱ�־λ
}
