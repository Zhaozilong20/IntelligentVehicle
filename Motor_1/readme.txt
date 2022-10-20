引脚定义

      RX   TX
串口1 PA10 PA9
串口2 PD6  PD5
串口3 PB11 PB10
串口4 PC10 PC11


      CH1 CH2 CH3 CH4
TIM2  PA5 PB3
TIM3  PA6 PA7
TIM4  PB6 PB7 PB8 PB9
TIM5  PA0 PA1 PA2 PA3
TIM1  PA8 PA9 
TIM8  PC6 PC7


*电机    编码器引脚     编码器定时器   编码器数组数据    引脚:EN            定时器通道
 *  A       A5,B3         TIM2          Encoder_Date[0]   PE9  11           TIM1->CH1  CH2
 *  B       A6,A7         TIM3          Encoder_Date[1]   PE13 14           TIM1->CH3  CH4
 *  C       D12,D13        TIM4          Encoder_Date[2]   PC6  7            TIM8->CH1  CH2
 *  D       A0,A1         TIM5          Encoder_Date[3]   PC8  9            TIM8->CH3  CH4
 
 
      1   2   3   4   5   6   7       8   9   10   11    12         13    14    15
ADC1                 PA5 PA6 PA7      PB0     PC0  PC1   PC2       PC13   PC4

步进电机    脉冲  方向   
     1      PB13  PB14  
	 2      PD8   PB15
	 3      PD10  PD9