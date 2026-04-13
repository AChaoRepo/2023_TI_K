/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

#include "delay.h"
#include "mic_adc.h"
#include "fft_user.h"
#include "PGA117.h"
#include "buzzer.h"

#include <stdlib.h>

#define PGA117_SELF_TEST_ENABLE 0U

typedef enum run_state_e {
    STATE_IDLE = 0,
    STATE_LERN_1,          /* Learn cup #1 */
    STATE_LERN_2,          /* Learn cup #2 */
    STATE_LERN_3,          /* Learn cup #3 */
    STATE_LERN_4,          /* Learn cup #4 */
    STATE_LERN_5,          /* Learn cup #5 */
    STATE_PLAY,            /* One-key auto play */
    STATE_LESSON1_TEST,    /* Basic recognition test */
    STATE_LESSON2_TEST,    /* Recognition + pitch output */
    STATE_LESSON3_TEST,    /* 5-cup play mode */
    STATE_LESSON4_TEST = 0x10, /* 8-cup mode, unknown cup => 0 */
} run_state_t;

volatile uint8_t runing_state = STATE_IDLE;

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

/* Private user code ---------------------------------------------------------*/
#if !PGA117_SELF_TEST_ENABLE
static int match_position_index(int freq, const int *position, int length)
{
    int i;
    int best_index = 0;
    int best_diff = 0x7FFFFFFF;

    for (i = 0; i < length; i++) {
        int delta;
        int tolerance;

        if (position[i] <= 0) {
            continue;
        }

        delta = abs(position[i] - freq);
        tolerance = (position[i] >= 130) ? 4 : 2;

        if ((delta <= tolerance) && (delta < best_diff)) {
            best_diff = delta;
            best_index = i + 1;
        }
    }

    return best_index;
}
#endif

#if PGA117_SELF_TEST_ENABLE
static void pga117_self_test_step(void)
{
    static const uint32_t gains[] = {1U, 2U, 5U, 10U, 20U, 50U};
    static uint32_t index = 0U;

    pga117_config(1U, gains[index]);
    HAL_GPIO_TogglePin(PRONUNCIATION_LED_GPIO_Port, PRONUNCIATION_LED_Pin);
    HAL_Delay(1000U);

    index++;
    if (index >= (sizeof(gains) / sizeof(gains[0]))) {
        index = 0U;
    }
}
#endif

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
#if !PGA117_SELF_TEST_ENABLE
    int result = 0;
    int i;
    int freq = 0;
    int freq_update = 0;
    uint32_t last_freq_tick = 0;
    int position[5] = {0};
#endif

    /* Enable I-Cache---------------------------------------------------------*/
    SCB_EnableICache();

    /* Enable D-Cache---------------------------------------------------------*/
    // SCB_EnableDCache();

    /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
    HAL_Init();

    /* Configure the system clock */
    SystemClock_Config();

    /* Initialize all configured peripherals */
    MX_GPIO_Init();
    MX_DMA_Init();
    MX_UART4_Init();
    MX_TIM1_Init();
    MX_USART1_UART_Init();
    MX_UART5_Init();
    MX_SPI3_Init();
    MX_ADC1_Init();

    delay_init(400);
    HAL_GPIO_WritePin(SPI1CS_GPIO_Port, SPI1CS_Pin, GPIO_PIN_SET);
    delay_ms(100);
    pa117_init();
    delay_ms(300);

    HAL_ADC_Start_DMA(&hadc1, (uint32_t *)adc_value_buffer, MAX_FFT_N);
    hmi_printf("t68.txt=\"READY\"\xff\xff\xff");
    HAL_UART_Receive_IT(&huart4, &UART4_Rx_Buffer, UART4_MACRO);

    while (1) {
#if PGA117_SELF_TEST_ENABLE
        pga117_self_test_step();
        continue;
#else
        if (runing_state == STATE_PLAY) {
            /* Handle one-key play immediately, independent from FFT cycle. */
            Play_Note(101, 4, 1);
            runing_state = STATE_IDLE;
        }

        if (start_fft_flag == 1U) {
            start_fft_flag = 0U;
            freq_update = 0;
            freq = 0;

            for (i = 0; i < MAX_FFT_N; i++) {
                if ((uint16_t)adc_value_buffer[i] > 20000U) {
                    freq = cfft_f32_mag();
                    if ((freq > 0) && ((HAL_GetTick() - last_freq_tick) > 300U)) {
                        freq_update = 1;
                        last_freq_tick = HAL_GetTick();
                    }
                    break;
                }
            }

            HAL_GPIO_WritePin(PRONUNCIATION_LED_GPIO_Port,
                              PRONUNCIATION_LED_Pin,
                              freq_update ? GPIO_PIN_SET : GPIO_PIN_RESET);

            switch (runing_state) {
            case STATE_LERN_1:
            case STATE_LERN_2:
            case STATE_LERN_3:
            case STATE_LERN_4:
            case STATE_LERN_5:
                if (freq_update) {
                    position[runing_state - 1] = freq;
                    hmi_printf("t68.txt=\"%d\"\xff\xff\xff", freq);
                }
                break;

            case STATE_LESSON1_TEST:
                if (freq_update) {
                    result = match_position_index(freq, position, 5);
                    if (result > 0) {
                        hmi_printf("t68.txt=\"Cup:%d\"\xff\xff\xff", result);
                    }
                }
                break;

            case STATE_LESSON2_TEST:
            case STATE_LESSON3_TEST:
                if (freq_update) {
                    result = match_position_index(freq, position, 5);
                    if (result > 0) {
                        hmi_printf("t68.txt=\"Cup:%d\"\xff\xff\xff", result);
                        Play_Note((unsigned char)result, 1, 1);
                    }
                }
                break;

            case STATE_LESSON4_TEST:
                if (freq_update) {
                    result = match_position_index(freq, position, 5);
                    hmi_printf("t68.txt=\"%d\"\xff\xff\xff", result);
                }
                break;

            default:
                break;
            }

            HAL_ADC_Start_DMA(&hadc1, (uint32_t *)adc_value_buffer, MAX_FFT_N);
        }
#endif
    }
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    while (!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {
    }

    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM = 5;
    RCC_OscInitStruct.PLL.PLLN = 160;
    RCC_OscInitStruct.PLL.PLLP = 2;
    RCC_OscInitStruct.PLL.PLLQ = 4;
    RCC_OscInitStruct.PLL.PLLR = 2;
    RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_2;
    RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
    RCC_OscInitStruct.PLL.PLLFRACN = 0;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
        Error_Handler();
    }

    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK |
                                  RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2 |
                                  RCC_CLOCKTYPE_D3PCLK1 | RCC_CLOCKTYPE_D1PCLK1;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
    RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
    RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK) {
        Error_Handler();
    }
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
    __disable_irq();
    while (1) {
    }
}

#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
    (void)file;
    (void)line;
}
#endif /* USE_FULL_ASSERT */
