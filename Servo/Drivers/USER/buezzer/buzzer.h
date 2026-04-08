#ifndef __BUZZER_H
#define __BUZZER_H

#include "gpio.h"

/* 使用寄存器控制IO输出状态反转*/
#define	BUZZER1TOGGLE		HAL_GPIO_TogglePin(BUZZER_GPIO_Port, BUZZER_Pin);

#define	BUZZER1(a) if(a) HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, GPIO_PIN_SET);\
										else HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, GPIO_PIN_RESET);


extern unsigned char Buzzer_play_Enable;//蜂鸣器状态
extern unsigned char enable;   //蜂鸣器发声使能标志
extern unsigned char tmrflag;  //定时器中断完成标志
void hmi_printf(char* fmt,...);
void Play_Note(unsigned char _Note, unsigned char _Beat,unsigned char play_count);
//void PlayTwoTiger(void);


#define UART4_MACRO   1 
extern uint8_t UART4_Rx_Buffer;


#endif /*__LED_H*/
