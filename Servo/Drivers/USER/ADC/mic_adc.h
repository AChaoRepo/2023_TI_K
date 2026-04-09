#ifndef __MIC_ADC_H_
#define __MIC_ADC_H_

#include "stm32h7xx_hal.h"

#define MAX_FFT_N 1024U

extern volatile uint8_t start_fft_flag;
extern volatile uint16_t adc_value_buffer[MAX_FFT_N];

#endif
