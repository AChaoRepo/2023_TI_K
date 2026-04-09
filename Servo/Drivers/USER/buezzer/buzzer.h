#ifndef __BUZZER_H
#define __BUZZER_H

#include "gpio.h"

#define BUZZER1TOGGLE HAL_GPIO_TogglePin(BUZZER_GPIO_Port, BUZZER_Pin)
#define BUZZER1(a) \
    do { \
        if (a) HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, GPIO_PIN_SET); \
        else HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, GPIO_PIN_RESET); \
    } while (0)

extern unsigned char Buzzer_play_Enable;
extern unsigned char enable;
extern unsigned char tmrflag;

void hmi_printf(char *fmt, ...);
void Play_Note(unsigned char _Note, unsigned char _Beat, unsigned char play_count);

#define UART4_MACRO 1U
extern uint8_t UART4_Rx_Buffer;

#endif
