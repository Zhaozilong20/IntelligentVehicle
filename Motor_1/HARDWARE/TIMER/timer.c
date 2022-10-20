#include "timer.h"
#include "stepmotor.h"
//////////////////////////////////////////////////////////////////////////////////	 

////////////////////////////////////////////////////////////////////////////////// 	 

//通用定时器6中断初始化
//arr：自动重装值。
//psc：时钟预分频数
//定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=定时器工作频率,单位:Mhz
//这里使用的是定时器6!
//定时器6定时中断
void TIM6_Int_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6,ENABLE);  ///使能TIM6时钟
	
  TIM_TimeBaseInitStructure.TIM_Period = arr; 	//自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //定时器分频
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM6,&TIM_TimeBaseInitStructure);//初始化TIM6
	
	TIM_ITConfig(TIM6,TIM_IT_Update,ENABLE); //允许定时器6更新中断
	TIM_Cmd(TIM6,ENABLE); //使能定时器6
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM6_DAC_IRQn; //定时器6中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x03; 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x01; 
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void TIM3_Int_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  ///使能TIM3时钟
	
  TIM_TimeBaseInitStructure.TIM_Period = arr; 	//自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //定时器分频
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);//初始化TIM3
	
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE); //允许定时器3更新中断
	TIM_Cmd(TIM3,ENABLE); //使能定时器3
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn; //定时器3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x03; 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x01; 
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void TIM3_IRQHandler(void)
{
	extern int32_t PUL[7];
	extern u8 dev,dev1,dev2,i,i1,i2;
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)==SET)            // 判断是溢出中断
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
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);              // 清除中断标志位
}

void TIM6_DAC_IRQHandler (void)
{
	extern int32_t PUL[7];
	extern u8 dev,dev1,dev2,i,i1,i2;
	
	if(TIM_GetITStatus(TIM6,TIM_IT_Update)==SET)            // 判断是溢出中断
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
		 
		 //LED1=!LED1;//DS1翻转
	}
	TIM_ClearITPendingBit(TIM6,TIM_IT_Update);              // 清除中断标志位
}


