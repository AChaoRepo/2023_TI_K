//#include "PWM.h"

//TIM_HandleTypeDef htim1;

///* TIM1 init function */
///***PWM使用定时器1
//PWM输出使用定时器1 的CH1 CH2  CH3  CH4 通道

//上一层驱动板控制前轮 
//	TIM1通道1
//  PE9      ------> TIM1_CH1  ----->  左前轮
//  PB0     ------>AIN1     1   前转     0  后转
//  PB1     ------>AIN2     0            1  

//  TIM1通道2
//  PE11     ------> TIM1_CH2  -----> 右前轮
//  PE10     ------>BIN1    1   前转    0   后转
//	PE12     ------>BIN2    0           1  
//	 
//下一层驱动板控制后轮	 
//	 
//  TIM1通道3
//  PE13     ------> TIM1_CH3  -----> 左后轮
//	PE15     ------>AIN1     1   前转     0  后转
//  PB11     ------>AIN2     0            1  
//	
//	TIM1通道4
//  PE14     ------> TIM1_CH4  -----> 右后轮
//	PB10     ------>BIN1    1   前转    0   后转
//	PB12     ------>BIN2    0           1  





//********************************/

////PWM引脚初始化函数
//void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);


//void MX_TIM1_Init(void)
//{

//  /* USER CODE BEGIN TIM1_Init 0 */

//  /* USER CODE END TIM1_Init 0 */

//  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
//  TIM_MasterConfigTypeDef sMasterConfig = {0};
//  TIM_OC_InitTypeDef sConfigOC = {0};
//  TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = {0};

//  /* USER CODE BEGIN TIM1_Init 1 */

//  /* USER CODE END TIM1_Init 1 */
//  htim1.Instance = TIM1;
//  htim1.Init.Prescaler = 200-1;
//  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
//  htim1.Init.Period = 1000;
//  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
//  htim1.Init.RepetitionCounter = 0;
//  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
//  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
//  {
//    Error_Handler();
//  }
//  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
//  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
//  {
//    Error_Handler();
//  }
//  if (HAL_TIM_PWM_Init(&htim1) != HAL_OK)
//  {
//    Error_Handler();
//  }
//  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
//  sMasterConfig.MasterOutputTrigger2 = TIM_TRGO2_RESET;
//  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
//  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
//  {
//    Error_Handler();
//  }
//  sConfigOC.OCMode = TIM_OCMODE_PWM1;
//  sConfigOC.Pulse = 500;
//  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
//  sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
//  sConfigOC.OCFastMode = TIM_OCFAST_ENABLE;
//  sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
//  sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
//  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
//  {
//    Error_Handler();
//  }
//  sConfigOC.Pulse = 500;
//  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
//  {
//    Error_Handler();
//  }
//  sConfigOC.Pulse = 500;
//  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
//  {
//    Error_Handler();
//  }
//  sConfigOC.Pulse = 500;
//  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_4) != HAL_OK)
//  {
//    Error_Handler();
//  }
//  sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
//  sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
//  sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
//  sBreakDeadTimeConfig.DeadTime = 0;
//  sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
//  sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
//  sBreakDeadTimeConfig.BreakFilter = 0;
//  sBreakDeadTimeConfig.Break2State = TIM_BREAK2_DISABLE;
//  sBreakDeadTimeConfig.Break2Polarity = TIM_BREAK2POLARITY_HIGH;
//  sBreakDeadTimeConfig.Break2Filter = 0;
//  sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
//  if (HAL_TIMEx_ConfigBreakDeadTime(&htim1, &sBreakDeadTimeConfig) != HAL_OK)
//  {
//    Error_Handler();
//  }
//  /* USER CODE BEGIN TIM1_Init 2 */

//  /* USER CODE END TIM1_Init 2 */
//  HAL_TIM_MspPostInit(&htim1);

//	//使能PWM输出，通道1、2、3、4输出PWM
//	HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_1);
//  HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_2); 
//	HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_3);
//  HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_4);
//}

////PWM输出引脚配置
//void HAL_TIM_MspPostInit(TIM_HandleTypeDef* timHandle)
//{

//  GPIO_InitTypeDef GPIO_InitStruct = {0};
//  if(timHandle->Instance==TIM1)
//  {
//  /* USER CODE BEGIN TIM1_MspPostInit 0 */

//  /* USER CODE END TIM1_MspPostInit 0 */

//    __HAL_RCC_GPIOE_CLK_ENABLE();
//    /**TIM1 GPIO Configuration
//    PE9     ------> TIM1_CH1
//    PE11     ------> TIM1_CH2
//    PE13     ------> TIM1_CH3
//    PE14     ------> TIM1_CH4
//    */
//    GPIO_InitStruct.Pin = GPIO_PIN_9|GPIO_PIN_11|GPIO_PIN_13|GPIO_PIN_14;
//    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
//    GPIO_InitStruct.Pull = GPIO_NOPULL;
//    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
//    GPIO_InitStruct.Alternate = GPIO_AF1_TIM1;
//    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

//  /* USER CODE BEGIN TIM1_MspPostInit 1 */

//  /* USER CODE END TIM1_MspPostInit 1 */
//  }
//}




////compare：占空比的值,范围：0-1000
////设置TIM1通道1的占空比
//void TIM_SetTIM1Compare1(uint32_t	compare)
//{
//	 TIM1->CCR1=compare;
// }

////设置TIM1通道2的占空比 
//void TIM_SetTIM1Compare2(uint32_t	compare)
//{
//	 TIM1->CCR2=compare;
// }

////设置TIM1通道3的占空比
//void TIM_SetTIM1Compare3(uint32_t	compare)
//{
//	 TIM1->CCR3=compare;
// }

////设置TIM1通道4的占空比 
//void TIM_SetTIM1Compare4(uint32_t	compare)
//{
//	 TIM1->CCR4=compare;
// }
