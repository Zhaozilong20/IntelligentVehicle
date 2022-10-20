#include "timer9.h"
#include "stepmotor.h"
//////////////////////////////////////////////////////////////////////////////////	 

////////////////////////////////////////////////////////////////////////////////// 	 

//通用定时器9中断初始化
//arr：自动重装值。
//psc：时钟预分频数
//定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=定时器工作频率,单位:Mhz
//这里使用的是定时器9!
//定时器9定时中断
void TIM9_Int_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM9,ENABLE);  ///使能TIM9时钟
	
  TIM_TimeBaseInitStructure.TIM_Period = arr; 	//自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //定时器分频
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM9,&TIM_TimeBaseInitStructure);//初始化TIM9
	
	TIM_ITConfig(TIM9,TIM_IT_Update,ENABLE); //允许定时器9更新中断
	TIM_Cmd(TIM9,ENABLE); //使能定时器9
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM6_DAC_IRQn; //定时器6中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x03; 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x01; 
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void TIM6_DAC_IRQHandler (void)
{
	extern int16_t PUL[];
	extern u16 adc[4];
	if(TIM_GetITStatus(TIM6,TIM_IT_Update)==SET)            // 判断是溢出中断
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
		 
		 //LED1=!LED1;//DS1翻转
	}
	TIM_ClearITPendingBit(TIM6,TIM_IT_Update);              // 清除中断标志位
}


