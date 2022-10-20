#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "stepmotor.h"
#include "pwm.h"
#include "timer.h"
#include "adc.h"
#include "key.h"

int16_t PWM[4],Encoder_Date[4],Target_Speed[4];
int16_t temp[4];
int32_t PUL[7];
float angle_z;
u8 dir[12]={0};
int16_t pos[2]={0};
u8 dev=3,dev1=3;
u8 sir[10]={0};
u8 i,i1=0;
u8 state=10;
u16 adc[4]={1000,1000,1000,1000};
u8 count;
u8 len;	
u16 times=0; 
u8 Rec[6]={0x34,0x36,0x35,0x32,0x31,0x33};  //蓝：三角、圆、矩形   红：三角、圆、矩形
u8 cor=0;
u8 key;           //保存键值

void set_pos0(int16_t PUL1,int16_t PUL2,int16_t PUL3);
void set_pos1(int16_t PUL1,int16_t PUL2,int16_t PUL3,int16_t PUL4,int16_t PUL5,int16_t PUL6,int16_t PUL7,int16_t PUL8);
void set_pos2(int16_t PUL1,int16_t PUL2,int16_t PUL3,int16_t PUL4,int16_t PUL5,int16_t PUL6,int16_t PUL7,int16_t PUL8);

void set_state0(void);
void set_state1(void);
void set_state2(void);
void set_state3(void);
void set_state4(void);
void set_state5(void);

void forward(int32_t PUL1);
void back(int32_t PUL1);
void left(int32_t PUL1);
void right(int32_t PUL1);
void stop(void);
void Hook_M1(void);
void Hook_M2(void);
	
void state_w(int32_t *PUL1);
void state_w1(int32_t *PUL1);
void state_w2(int j);

int main(void)
{    
	  dir[0]=1;dir[1]=1;dir[2]=1;dir[3]=1;dir[4]=1;dir[5]=1;dir[6]=1;dir[7]=1;dir[8]=1;dir[9]=1;dir[10]=1;dir[11]=1;
	
  	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	  delay_init(168);  //初始化延时函数
  	uart_init(9600);//初始化串口波特率为115200
  	Adc_Init();         //初始化ADC
    Step_Init();	
	  KEY_Init();       //初始化与按键连接的硬件接口

  	TIM1_PWM_Init(1000-1,84-1);
  	TIM6_Int_Init(168-1,42-1);  //  84M/42=2M  2M/5000=400Hz
	  TIM3_Int_Init(84-1,42-1);   //  84M/42=2M  2M/5000=400Hz
  	TIM7_Int_Init(100-1,8400-1);
	
	  EN_WHEEL=0;
    EN_HOOK=0;
	  EN_OPENMV=0; 
	  delay_ms(200);
	while(1) //实现比较值从0-300递增，到300后从300-0递减，循环
	{ 
      //  printf("adc0   %d   adc1   %d    \r\n ", adc[2],adc[3]);
 		  //  set_state0();
		  switch(state)
		  {
				case 1 :                                     //起始，最右端
					while(1){
						if(sir[0]<3){
					    if( (adc[0]>=4000) && (adc[1]>=4000)){
			              sir[0]++;
									  back(400);
									  state_w1(PUL);
							}
							else{
							   if(dir[10]){
								    PUL[5]=11650;
									  PUL[6]= -12600;
									  dir[10]=0;
								 }
								 back(4000);
							}
						}
						else{
							 stop();
							 delay_ms(250);
							 delay_ms(250);
							 state=2;
						   break;
						}
					 }
				continue;	
					 
				case 2 :                        //   右到左，第一层
					while(1){
						 if(sir[1]<6){
					    if(adc[0]>=4000&&adc[1]>=4000){
								sir[1]++;
								if(Rec[cor]==0x32||Rec[cor]==0x34||Rec[cor]==0x36){
									stop();
								  delay_ms(250);
								  delay_ms(250);
			            Hook_M1();
									forward(400);
									state_w1(PUL);
								}else{
									cor++;
									forward(400);
									state_w1(PUL);
								}
							}
							else{
								 forward(8000);
							}
						}
						else{
							 stop();
						   delay_ms(250);
							 delay_ms(250);
							 cor--;
						   state=3;
						   break;
						}
				  }
				continue;	
					 
				case 3 :                    //左到右，第二层
					while(1){
					    if(sir[2]<6){
					    if( (adc[0]>=4000) && (adc[1]>=4000)){
								sir[2]++;
								if(Rec[cor]==0x31||Rec[cor]==0x35||Rec[cor]==0x33){
									stop();
								  delay_ms(250);
								  delay_ms(250);
			            Hook_M2();
									if(sir[2]!=6)
									{
										   back(400);
										   state_w1(PUL);
										 }
								}else{
									cor--;
									if(sir[2]!=6)
										 {
										   back(400);
										   state_w1(PUL);
										 }
								}
							}
							else{
								 back(8000);
							}
						}
						else{
							 stop();
						   delay_ms(250);
							 delay_ms(250);
							 forward(800);
							 state_w(PUL);
							 PUL[6]=1200*5;
						   state=4;
						   break;
						}
					 }
				continue;	
					 
				case 4 :                         //向后
						      i1=0;dev1=7;
						      left(200);
						      state_w1(PUL);
						      i1=0;dev1=5;
						      left(200);
						      state_w1(PUL);
						      i1=0;dev1=3;
						      left(400);
						      state_w1(PUL);
						      i1=0;dev1=1;
								  left(10000);
						      state_w1(PUL);
						      i1=0;dev1=3;
						      left(400);
						      state_w1(PUL);
						      i1=0;dev1=5;
						      left(200);
						      state_w1(PUL);
						      i1=0;dev1=7;
						      left(200);
						      state_w(PUL);
						      state=5;
						      break;
				continue;	
					 
			  case 5 :                         //向右
						     i1=0;dev1=7;
						     forward(200);
						     state_w1(PUL);
						     i1=0;dev1=5;
						     forward(200);
						     state_w1(PUL);
						     i1=0;dev1=3;
						     forward(400);
						     state_w1(PUL);
						     i1=0;dev1=1;
						     forward(8000);
						     state_w1(PUL);
						     i1=0;dev1=3;
						     forward(400);
						     state_w1(PUL);
						     i1=0;dev1=5;
						     forward(200);
						     state_w1(PUL);
						     i1=0;dev1=7;
						     forward(200);
						     state_w(PUL);
						     state=6;
						     break;
				continue;
					 
				case 6 :         //向后
						      i1=0;dev1=7;
						      left(200);
						      state_w1(PUL);
						      i1=0;dev1=5;
						      left(200);
						      state_w1(PUL);
						      i1=0;dev1=3;
						      left(400);
						      state_w1(PUL);
						      i1=0;dev1=1;
								  left(8000);
						      state_w1(PUL);
						      i1=0;dev1=3;
				          while(1){
						        left(4000);
									  if( (adc[0]>=4000) && (adc[1]>=4000)){
							          stop();
								        delay_ms(250);
								        delay_ms(250);
								        back(1400);
								        state_w1(PUL);
								        state=7;
						            break;
							      }
									}					
				continue;
				
				case 7 :      //向左
					while(1){
					    if( (adc[0]>=4000) && (adc[1]>=4000)){
							    stop();
								  delay_ms(250);
								  delay_ms(250);
								  EN_HOOK=1;
								  if(dir[8]){
									TIM1->CCR1 = 540;  
	                TIM1->CCR2 = 0;
									TIM1->CCR3 = 0;  
	                TIM1->CCR4 = 0;
									state_w2(25);      //推出时间
									
								  TIM1->CCR1 = 0;  
	                TIM1->CCR2 = 0;
									delay_ms(250);
									TIM1->CCR1 = 0;  
	                TIM1->CCR2 = 500;
									delay_ms(250);
									delay_ms(250);
									TIM1->CCR1 = 0;  
	                TIM1->CCR2 = 0;
									dir[8]=0;
									}
								  back(800);
								  state_w1(PUL);                             //put 2
								  state=8;
						      break;
							}
							else
								  back(8000);
					 }
				continue;
					 
				case 8 :
					while(1){
					    if( (adc[0]>=4000) && (adc[1]>=4000)){
							    stop();
								  delay_ms(250);
								  delay_ms(250);
								  EN_HOOK=1;
								  if(dir[9]){
									TIM1->CCR1 = 0;  
	                TIM1->CCR2 = 0;
									TIM1->CCR3 = 540;  
	                TIM1->CCR4 = 0;
									state_w2(25);   //推出时间
									
								  TIM1->CCR3 = 0;  
	                TIM1->CCR4 = 0;
									dir[9]=0;
								  }
									right(1000);
									state_w1(PUL);
									state=9;
									break;
								  
							}
							else
								  back(8000);
						 }
					 
				continue;
						 
			case 9 :
					while(1){
						   stop();
							 delay_ms(250);
						 }
					 
				continue;
						 
			case 10 :
					while(1){
						 if((USART_RX_STA&0x8000)&&(dir[7]==1)){
								for(int j=0;j<6;j++){
								    Rec[j]=USART_RX_BUF[j];
								}
								dir[7]==0;
		         }
						 key=KEY_Scan(0);		//得到键值
						 if(key){
						   state = 1;
						   break;
						 }
				  }
					 
				continue;
						 
			 case 11 :
					while(1){
//						  forward(3000);
//						  state_w1(PUL);
//						  back(3000);
//						  state_w1(PUL);
//						  forward(3000);
//						  state_w1(PUL);
//						  back(3000);
//						  state_w1(PUL);
//						  left(3000);
//						  state_w1(PUL);
//									state=9;
//									break;
					    printf("adc0   %d   adc1   %d    \r\n ", adc[0],adc[1]);
						 }
				continue;
		}
			
		  
	}
	
}


void set_pos0(int16_t PUL1,int16_t PUL2,int16_t PUL3)
{
	    EN_WHEEL=1;
      EN_HOOK=0;
	    i=0;dev=2;
			PUL[6]=PUL1*5;			 //up down
			  state_w1(PUL+6);
			PUL[4]=PUL2;			 //left right
			  state_w1(PUL+4);
			PUL[6]=PUL3*5;			
			  state_w1(PUL+6);

}

void set_pos1(int16_t PUL1,int16_t PUL2,int16_t PUL3,int16_t PUL4,int16_t PUL5,int16_t PUL6,int16_t PUL7,int16_t PUL8)
{ 
	    EN_WHEEL=1;
      EN_HOOK=0;
	    i=0;dev=4;
	    if(PUL1<0){pos[0]= -1;}
			else if(PUL1>0){pos[0]=1;}
			else if(PUL1==0){pos[0]=0;}
			
			if(PUL2<0){pos[1]= -1;}
			else if(PUL2>0){pos[1]=1;}
			else if(PUL2==0){pos[1]=0;}
	    PUL[5]=100*pos[0];			 //forward back
      PUL[4]=100*pos[1];	
	      state_w1(PUL+5);
			i=0;dev=3;
	    PUL[5]=400*pos[0];			 //forward back
      PUL[4]=400*pos[1];			
			  state_w1(PUL+5);
	    i=0;dev=2;
	    PUL[5]=400*pos[0];			 //forward back
      PUL[4]=400*pos[1];			
			  state_w1(PUL+5);
	    i=0;dev=1;
	    PUL[5]=PUL1-900*pos[0];			 //forward back
      PUL[4]=PUL2-900*pos[1];	
	      state_w1(PUL+5);       //第一段左右 前后
			
			i=0;dev=1;
	    PUL[5]=4000*pos[0];			
			  state_w1(PUL+5);
			i=0;dev=2;
	    PUL[5]=800*pos[0];			
			  state_w1(PUL+5);
			i=0;dev=3;
	    PUL[5]=PUL3-4800*pos[0];			
			  state_w(PUL+5);
			
	      delay_ms(250);
	      delay_ms(250);          //第二段前后
				
      i=0;dev=2;
			PUL[6]=PUL4*5;			
			  state_w1(PUL+6);
			PUL[4]=PUL5;			
			  state_w1(PUL+4);        //第三段上下 左右
			
				i=0;dev=1; 
				PUL[5]=PUL6;			
					delay_ms(200);
					delay_ms(200);
					delay_ms(200);	
					delay_ms(200);
				PUL[4]=PUL7;			
				PUL[6]=PUL8*5;			
			
}

void set_pos2(int16_t PUL1,int16_t PUL2,int16_t PUL3,int16_t PUL4,int16_t PUL5,int16_t PUL6,int16_t PUL7,int16_t PUL8)
{ 
	    EN_WHEEL=1;
      EN_HOOK=0;
	    if(PUL1<0){pos[0]= -1;}
			else if(PUL1>0){pos[0]=1;}
			else if(PUL1==0){pos[0]=0;}
			
			if(PUL2<0){pos[1]= -1;}
			else if(PUL2>0){pos[1]=1;}
			else if(PUL2==0){pos[1]=0;}
			
			i=0;dev=4;
			PUL[4]=100*pos[1];
			 state_w1(PUL+4);
			i=0;dev=3;
			PUL[4]=400*pos[1];
			 state_w1(PUL+4);
			i=0;dev=2;
			PUL[4]=400*pos[1];	
			 state_w1(PUL+4);
			i=0;dev=1;
			PUL[4]=PUL2-1800*pos[1];
			 state_w1(PUL+4);
			i=0;dev=2;
			PUL[4]=400*pos[1];
			 state_w1(PUL+4);
			i=0;dev=3;
			PUL[4]=400*pos[1];
			 state_w1(PUL+4);
			i=0;dev=4;
			PUL[4]=100*pos[1];	
			 state_w1(PUL+4);
			
			i=0;dev=4;
			PUL[5]=-100;
			 state_w1(PUL+5);
			i=0;dev=3;
			PUL[5]=-400;
			 state_w1(PUL+5);
			i=0;dev=2;
			PUL[5]=-400;	
			 state_w1(PUL+5);
			i=0;dev=1;
			PUL[5]=PUL3+2900;
			 state_w1(PUL+5);
			i=0;dev=2;
			PUL[5]=-800;
			 state_w1(PUL+5);
			i=0;dev=3;
			PUL[5]=-800;
			 state_w1(PUL+5);
			i=0;dev=4;
			PUL[5]=-400;	
			 state_w1(PUL+5);
			
	      delay_ms(250);
	      delay_ms(250);          //第二段前后
				
      i=0;dev=2;
			PUL[6]=PUL4*5;			
			  state_w1(PUL+6);
			PUL[4]=PUL5;			
			  state_w1(PUL+4);        //第三段上下 左右
				
			if(count<=2){
				i=0;dev=1; 
				PUL[5]=PUL6;			
					delay_ms(200);
					delay_ms(200);
					delay_ms(200);	
					delay_ms(200);
				PUL[4]=PUL7;			
				PUL[6]=PUL8*5;			
			}
}

void set_state0(void)
{
     if(dir[0]){ 
								 set_pos0(-1200,980,4000);
							   set_pos2(0,-4370,-11940,-3260,-980,11940,4370,460);  //POSE1 -4970  -4370  -6970			 
		   dir[0]=0;
		 }
}

void set_state1(void)
{
     if(dir[1]){ 
								 set_pos0(-1200,980,4000);
							   set_pos2(0,0,-11940,-3260,-980,11940,0,460);      // POSE2
		   dir[1]=0;
		 }
}

void set_state2(void)
{
     if(dir[2]){ 
		             set_pos0(-1200,980,4000);
							   set_pos2(0,4300,-11940,-3260,-980,11940,-4300,460);  // POSE3		 
		   dir[2]=0;
		 }
}

void set_state3(void)
{
     if(dir[3]){ 
		             set_pos0(-1200,1000,4500);
							   set_pos2(0,-4400,-11240,-1200,-1000,11240,4400,-2100);  //POSE4		 
		   dir[3]=0;
		 }
}

void set_state4(void)
{
     if(dir[4]){ 
		             set_pos0(-1200,1000,4500);
							   set_pos2(0,0,-11540,-1200,-1000,11540,0,-2100);  //POSE5		 		 
		   dir[4]=0;
		 }
}

void set_state5(void)
{
     if(dir[5]){ 
		             set_pos0(-1200,1000,4500);
							   set_pos2(0,4350,-11240,-1200,-1000,11240,-4350,-2100);  //POSE6		 	 
		   dir[5]=0;
		 }
}

void forward(int32_t PUL1)
{    
	   EN_WHEEL=0;
     EN_HOOK=0;
     PUL[0]= PUL1*5; 
	   PUL[1]= PUL1*5; 
	   PUL[2]= PUL1*5; 
	   PUL[3]= PUL1*5; 
}

void back(int32_t PUL1)
{
	   EN_WHEEL=0;
     EN_HOOK=0;
     PUL[0]= -PUL1*5; 
	   PUL[1]= -PUL1*5; 
	   PUL[2]= -PUL1*5; 
	   PUL[3]= -PUL1*5; 
}

void left(int32_t PUL1)
{
	   EN_WHEEL=0;
     EN_HOOK=0;
     PUL[0]= PUL1*5; 
	   PUL[1]= -PUL1*5; 
	   PUL[2]= PUL1*5; 
	   PUL[3]= -PUL1*5; 
}

void right(int32_t PUL1)
{
	   EN_WHEEL=0;
     EN_HOOK=1;
     PUL[0]= -PUL1*5; 
	   PUL[1]= PUL1*5; 
	   PUL[2]= -PUL1*5; 
	   PUL[3]= PUL1*5; 
}

void stop(void)
{
	   EN_WHEEL=0;
     EN_HOOK=0;
     PUL[0]= 0; 
	   PUL[1]= 0; 
	   PUL[2]= 0; 
	   PUL[3]= 0; 
}

void Hook_M1(void)
{
		   if(Rec[cor]==0x32)
			 {
			    set_state2();
			 }
			 else if(Rec[cor]==0x36)
			 {
			    set_state1();
			 }
			 else if(Rec[cor]==0x34)
			 {
			    set_state0();
			 }
			 cor++;
}

void Hook_M2(void)
{
	     if(Rec[cor]==0x31)
			 {
				  count++;
			    set_state5();
			 }
			 else if(Rec[cor]==0x35)
			 {
				  count++;
			    set_state4();
				  
			 }
			 else if(Rec[cor]==0x33)
			 {
				  count++;
			    set_state3();
			 }
			 cor--;
}

void state_w(int32_t *PUL1)
{
    while(*PUL1 != 0){
			}
			delay_ms(250);
			delay_ms(250);
}

void state_w1(int32_t *PUL1)
{
    while(*PUL1 != 0){
			}
}

void state_w2(int j)
{
	  while(j--){
		   delay_ms(250);
		}
}

