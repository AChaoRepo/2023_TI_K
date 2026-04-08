 
 #include "stm32h7xx_hal.h"
 #include "adc.h"
 #include "main.h"
 #include "mic_adc.h"
 uint8_t   start_fft_flag=0;  //启动fft计算标志， start_fft_flag:1 启动  0:停止
 
 uint16_t adc_value_buffer[MAX_FFT_N]={0};
 
 
 // ADC1 选用通道3  : PA6  用于幅度判断
 // ADC2 选用通道10 ：PC0 用于数据采集
 //
 //
 //
 //
 //HAL_ADC_Start_IT 启动ADC的转换
 
 
 //adc单次转换完成的中断函数
 void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
 {
   
//		if(hadc->Instance==ADC2)
//		{
//       adc2_value = HAL_ADC_GetValue(&hadc2);
//			 HAL_ADC_Stop_IT(&hadc2);
//			// printf("%d \n",adc2_value);
//		 }	
      
    //DMA传输完成的中断回调函数
		if(hadc->Instance==ADC1)
		{
       HAL_ADC_Stop_DMA(&hadc1);
			 start_fft_flag=1;
			 //printf("HAL_ADC_ConvCpltCallback\n");
		 }
		 
 }
 
 //HAL_ADC_ConvCpltCallback(hadc);
 /********************
 HAL_ADC_Start_DMA(ADC_HandleTypeDef* hadc, uint32_t* pData, uint32_t Length); ADC启动DMA传输
 HAL_ADCEx_Calibration_Start(&hadc1);
 
  // Set the DMA transfer complete callback 
     //   hadc->DMA_Handle->XferCpltCallback = ADC_DMAConvCplt;


 ADC1 DMA传输完成中断函数
 void DMA1_Stream1_IRQHandler(void)
{*/
  /* USER CODE BEGIN DMA1_Stream1_IRQn 0 */

  /* USER CODE END DMA1_Stream1_IRQn 0 */
 // HAL_DMA_IRQHandler(&hdma_adc1);
  /* USER CODE BEGIN DMA1_Stream1_IRQn 1 */
 /*if(__HAL_DMA_GET_FLAG(&hdma_adc1,DMA_FLAG_TCIF1_5))//等待DMA1_Steam0传输完成
	 {
	 //此处编写中断程序
	 //清除中断
		 a=0;
	   __HAL_DMA_CLEAR_FLAG(&hdma_adc1,DMA_FLAG_TCIF1_5);//清除DMA1_Steam0传输完成标志 
		 HAL_ADC_Stop_DMA(&hadc2);
	 }
*/
	
	
  /* USER CODE END DMA1_Stream1_IRQn 1 */
//}***
//************/

 