#include "stm32h7xx_hal.h"
#include "adc.h"
#include "mic_adc.h"

volatile uint8_t start_fft_flag = 0U;
volatile uint16_t adc_value_buffer[MAX_FFT_N] = {0};

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
    if (hadc->Instance == ADC1) {
        HAL_ADC_Stop_DMA(&hadc1);
        start_fft_flag = 1U;
    }
}
