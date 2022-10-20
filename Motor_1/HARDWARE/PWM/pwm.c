#include "pwm.h"
#include "stepmotor.h"
#include "usart.h"
#include "adc.h"

//TIM1 TIM8  PWM部分初始化 
//PWM输出初始化
//arr：自动重装值
//psc：时钟预分频数

void TIM1_PWM_Init(u16 arr,u16 psc) //  电机的PWM信号初始化函数
{
	GPIO_InitTypeDef 					 GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef 	 TIM_TimeBaseInitStructure;
	TIM_OCInitTypeDef  				 TIM_OCInitStructure;                   // 要用到的一些结构体的声明

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);              //开启 IO口的时钟
	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;                      // IO复用的相关参数配置
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;                    
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_8 | GPIO_Pin_9|          
																 GPIO_Pin_10 | GPIO_Pin_11;         //这里是配置了4个 IO 的 PWM输出
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;                  // 实际比赛中我们也是这样用的  一个定时器 生成四路
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;                // 占空比不同的 PWM信号  这四路信号占用一个定时器的资源
  GPIO_Init(GPIOA,&GPIO_InitStructure);															// 四路信号的频率是一致的
  
  GPIO_PinAFConfig(GPIOA,GPIO_PinSource8, GPIO_AF_TIM1);            // IO复用
  GPIO_PinAFConfig(GPIOA,GPIO_PinSource9, GPIO_AF_TIM1);
  GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_TIM1);
  GPIO_PinAFConfig(GPIOA,GPIO_PinSource11,GPIO_AF_TIM1);
  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);              //开启定时器的时钟
	
	//Timer clock = sysclock /(TIM_Prescaler+1) = 168M
	//Period = (TIM counter clock / TIM output clock) - 1 = 20K
	
  TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;      //定时器基本参数设置
  TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;  
  TIM_TimeBaseInitStructure.TIM_Prescaler = psc;  
  TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
  TIM_TimeBaseInitStructure.TIM_Period = arr;    
  TIM_TimeBaseInit(TIM1,&TIM_TimeBaseInitStructure);
  
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;              //定时器的输出 配置
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
  TIM_OCInitStructure.TIM_Pulse = 0;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
  TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
  
  TIM_OC1Init(TIM1,&TIM_OCInitStructure);                        // 输出通道1 初始化

  TIM_OCInitStructure.TIM_Pulse = 0;
  TIM_OC2Init(TIM1,&TIM_OCInitStructure);                        // 输出通道2 初始化
  
  TIM_OCInitStructure.TIM_Pulse = 0;
  TIM_OC3Init(TIM1,&TIM_OCInitStructure);                        // 输出通道3 初始化
  
  TIM_OCInitStructure.TIM_Pulse = 0;
  TIM_OC4Init(TIM1,&TIM_OCInitStructure);                        // 输出通道4 初始化
  
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

//定时器7定时中断
void TIM7_Int_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7,ENABLE);  ///使能TIM7时钟
	
  TIM_TimeBaseInitStructure.TIM_Period = arr; 	//自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //定时器分频
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM7,&TIM_TimeBaseInitStructure);//初始化TIM7
	
	TIM_ITConfig(TIM7,TIM_IT_Update,ENABLE); //允许定时器7更新中断
	TIM_Cmd(TIM7,ENABLE); //使能定时器7
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM7_IRQn; //定时器7中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x03; 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x01; 
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}


/* 2.8版本的电路板电机驱动部分映射如下
 *电机    编码器引脚     编码器定时器   编码器数组数据    引脚:EN            定时器通道
 *  A       D12,D13       TIM4          Encoder_Date[2]   PE9  11           TIM1->CH1  CH2
 *  B       A6 ,A7        TIM3          Encoder_Date[1]   PE13 14           TIM1->CH3  CH4
 *  C       A0 ,A1        TIM5          Encoder_Date[3]   PC6  7            TIM8->CH1  CH2
 *  D       A15,B3        TIM2          Encoder_Date[0]   PC8  9            TIM8->CH3  CH4
 */

void TIM7_IRQHandler(void)
{
	extern u16 adc[4];
	if(TIM_GetITStatus(TIM7,TIM_IT_Update)==SET)            // 判断是溢出中断
	{
		
		adc[0]=Get_Adc(ADC_Channel_8);
		adc[1]=Get_Adc(ADC_Channel_10);
		adc[2]=Get_Adc(ADC_Channel_11);
		adc[3]=Get_Adc(ADC_Channel_12);
		
	}
	TIM_ClearITPendingBit(TIM7,TIM_IT_Update);              // 清除中断标志位
}
