#ifndef __MOTOR_H__
#define __MOTOR_H__

#include "main.h"

extern void Car_Forward();


//全局变量，控制小车的PWM
extern uint16_t  Left_Front_Wheel_PWM; //小车左前轮PWM
extern uint16_t  Left_Rear_Wheel_PWM;  //小车左后轮PWM
extern uint16_t  Right_Front_Wheel_PWM;//小车右前轮PWM
extern uint16_t  Right_Rear_Wheel_PWM;//小车有后轮PWM



void MX_TIM1_Init(void);

#endif
/* __MOTOR_H__ */
