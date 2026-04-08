#include "Encode.h"
/***************************
编码器引脚连接说明：

上一层驱动板控制前轮 
左前轮  TIM2    PA5     ------> TIM2_CH1
                PB3 (JTDO/TRACESWO)     ------> TIM2_CH2

右前轮  TIM3    PC6     ------> TIM3_CH1
                PC7     ------> TIM3_CH2
    
下一层驱动板控制后轮
左后轮  TIM4    PD12     ------> TIM4_CH1
                PD13     ------> TIM4_CH2


右后轮  TIM5    PA0     ------> TIM5_CH1
                PA1     ------> TIM5_CH2

****************************/

int Left_Front_Wheel_Encoder=0;//读取左前轮编码器的值
int Right_Front_Wheel_Encoder=0;//读取右前轮编码器的值
int Left_Rear_Wheel_Encoder=0;//读取左后轮编码器的值
int Right_Rear_Wheel_Encoder=0;//读取右后轮编码器的值



/**************************************************************************
函数功能：单位时间读取编码器计数
入口参数：定时器
返回  值：速度值
**************************************************************************/
int Read_Encoder(uint8_t TIMX)
{
   int Encoder_TIM;    
   switch(TIMX)
	 {
	   case 2:  Encoder_TIM= (short)TIM2 -> CNT;  TIM2 -> CNT=0;break;
		 case 3:  Encoder_TIM= (short)TIM3 -> CNT;  TIM3 -> CNT=0;break;	
		 case 4:  Encoder_TIM= (short)TIM4 -> CNT;  TIM4 -> CNT=0;break;	
		 case 5:  Encoder_TIM= (short)TIM5 -> CNT;  TIM5 -> CNT=0;break;	
		 default: Encoder_TIM=0;
	 }
		return Encoder_TIM;
}

