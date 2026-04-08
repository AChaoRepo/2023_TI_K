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
#include "EventRecorder.h"              // Keil.ARM Compiler::Compiler:Event Recorder
#include	"buzzer.h"

typedef enum run_state_e {
	STATE_IDLE = 0,
	STATE_LERN_1,
	STATE_LERN_2,
	STATE_LERN_3,
	STATE_LERN_4,
	STATE_LERN_5,
	STATE_PLAY,
	STATE_LESSON1_TEST,
	STATE_LESSON2_TEST,
	STATE_LESSON3_TEST,
	STATE_LESSON4_TEST = 0x10,
}run_state_t;

uint8_t runing_state =0;
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
	
int main(void)
{
	int result = 0, i;
	int freq, freq_update;
	int time = 0;
	int position[5];
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* Enable I-Cache---------------------------------------------------------*/
  SCB_EnableICache();

  /* Enable D-Cache---------------------------------------------------------*/
  //SCB_EnableDCache();

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */
   EventRecorderInitialize(EventRecordAll, 1U);
   EventRecorderStart();
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_UART4_Init();
  MX_TIM1_Init();

  MX_USART1_UART_Init();
  MX_UART5_Init();
  MX_SPI3_Init();
  MX_ADC1_Init();
  /* USER CODE BEGIN 2 */
  delay_init(400);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_0, GPIO_PIN_SET);
	delay_ms(100);
	pa117_init();
//	MX_TIM6_Init();
	delay_ms(500);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  //HAL_ADCEx_Calibration_Start(&hadc1,ADC_CALIB_OFFSET,ADC_SINGLE_ENDED);
	//HAL_ADC_Start_DMA(&hadc1,adc_value_buffer,128);
	 HAL_ADC_Start_DMA(&hadc1,(uint32_t*)adc_value_buffer,MAX_FFT_N);
	 printf("HAL_ADC_Start_DMA\n");
	 hmi_printf( "t68.txt=\"%s\"\xff\xff\xff","ÄúºÃ");
   //	HAL_UARTEx_ReceiveToIdle_IT(&huart4,UART4_Rx_Buffer,UART4_MACRO);
  HAL_UART_Receive_IT(&huart4,&UART4_Rx_Buffer,UART4_MACRO);
	while (1)
  {  
		if(start_fft_flag==1)
		{
		  start_fft_flag=0;
			for(i=0;i<MAX_FFT_N;i++)
			{					 		
				 if(((uint16_t)adc_value_buffer[i])>20000)
				 {
					    //printf("%d: %d \n",i, adc_value_buffer[i]);
					   freq = cfft_f32_mag();
						 if (freq)
						 {
							 if (HAL_GetTick() - time > 300)
							 {															 
								 printf("%d\r\n", freq);
							   freq_update = 1;
								 HAL_GPIO_WritePin(PRONUNCIATION_LED_GPIO_Port, PRONUNCIATION_LED_Pin, GPIO_PIN_SET);
								 time = HAL_GetTick();
							 }
							 
						 }
			        break;
				 }
			 }
			if(freq_update==0){HAL_GPIO_WritePin(PRONUNCIATION_LED_GPIO_Port, PRONUNCIATION_LED_Pin, GPIO_PIN_RESET);}
			 	switch (runing_state)
				{
					case STATE_PLAY:
						Play_Note(101,4,1);
						runing_state = STATE_IDLE;
						break;
					case STATE_LERN_1:
					case STATE_LERN_2:
					case STATE_LERN_3:
					case STATE_LERN_4:
					case STATE_LERN_5:
						if (freq_update)
						{
							position[runing_state - 1] = freq;
							freq_update = 0;
							hmi_printf( "t68.txt=\"%d\"\xff\xff\xff",freq);
						}
						break;
					case STATE_LESSON1_TEST:
						if (freq_update)
						{
							freq_update = 0;
							result = 0;
							for (i = 0; i < 5; i++)
							{
								if (freq >= 130 && position[i] == freq)
								{
									result = i + 1;
								}
								else if (freq < 130 && (position[i] == freq || position[i] == freq - 1 || position[i] == freq + 1))
								{
									result = i + 1;
								}
							}
							//ÏÔÊ¾Êä³öÒô·û	 
							if (result)
								 hmi_printf( "t68.txt=\"±­ºÅ£º%d\"\xff\xff\xff",result);

						}
						break;
					case STATE_LESSON2_TEST:
						if (freq_update)
						{
							freq_update = 0;
							result = 0;
							for (i = 0; i < 5; i++)
							{
								if (freq >= 130 && position[i] == freq)
								{
									result = i + 1;
								}
								else if (freq < 130 && (position[i] == freq || position[i] == freq - 1 || position[i] == freq + 1))
								{
									result = i + 1;
								}
							}
							//ÏÔÊ¾Êä³öÒô·û

							if (result)
							{					
								hmi_printf( "t68.txt=\"±­ºÅ£º%d\"\xff\xff\xff",result);
								Play_Note(result, 1,1);
								//HAL_Delay(1000);
							}
						}
						break;
					case STATE_LESSON3_TEST:
						if (freq_update)
						{
							freq_update = 0;
							result = 0;
							for (i = 0; i < 5; i++)
							{
								if (freq >= 130 && position[i] == freq)
								{
									result = i + 1;
								}
								else if (freq < 130 && (position[i] == freq || position[i] == freq - 1 || position[i] == freq + 1))
								{
									result = i + 1;
								}
							}
							//ÏÔÊ¾Êä³öÒô·û

							if (result)
							{
									hmi_printf( "t68.txt=\"±­ºÅ£º%d\"\xff\xff\xff",result);
									Play_Note(result, 1,1);				
								  //HAL_Delay(1000);
							}
						}
						break;
					case STATE_LESSON4_TEST:
						if (freq_update)
						{
							freq_update = 0;
							result = 0;
							for (i = 0; i < 5; i++)
							{
								if (freq >= 130 && position[i] == freq)
								{
									result = i + 1;
								}
								else if (freq < 130 && (position[i] == freq || position[i] == freq - 1 || position[i] == freq + 1))
								{
									result = i + 1;
								}
							}
							//ÏÔÊ¾Êä³öÒô·û
							hmi_printf( "t68.txt=\"%d\"\xff\xff\xff",result);

						}
						break;
					default:
						break;
				}	

		   HAL_ADC_Start_DMA(&hadc1,(uint32_t*)adc_value_buffer,MAX_FFT_N);
		}
		

  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Supply configuration update enable
  */
  HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);

  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
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
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                              |RCC_CLOCKTYPE_D3PCLK1|RCC_CLOCKTYPE_D1PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
