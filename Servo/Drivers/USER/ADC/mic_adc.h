#ifndef __MIC_ADC_H_
#define __MIC_ADC_H_

#include "stm32h7xx_hal.h"

//#define start_value   0 //启动DMA采集的阈值
#define MAX_FFT_N		1024 //采集的数据点数 //在FFTInc.h中也存在此宏定义 ,main.c中也存在此宏定义

extern uint8_t   start_fft_flag; 
extern uint16_t  adc2_value;
extern uint16_t adc_value_buffer[MAX_FFT_N];
#endif




