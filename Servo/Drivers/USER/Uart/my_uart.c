/***********************
完成与PC端串口连接及数据收发
端口采用USART1:
PB14     ------> USART1_TX
PB15     ------> USART1_RX

***********************/

#include "my_uart.h"
#include "stdio.h"
#include <string.h>




//uint8_t UART4_BufferFlag=Buffer_Ready;
//uint8_t UART4_Rx_Buffer[128]={0};
//uint8_t Rx_Buffer[128]={0};

/***********************************



*************************************/



/********
函数功能：重定向C库函数printf到USART1
输入参数：无
返回值：ch
说明：无
//****************/
int fputc(int ch,FILE *f)
{
  HAL_UART_Transmit(&huart1,(uint8_t*)&ch,1,0xffff);
	return ch;
}


/********
函数功能：重定向C库函数getchar,scanf到USART1
输入参数：无
返回值：无
说明：无
****************/
int fgetc(FILE *f)
{
	uint8_t ch=0;
  while(HAL_UART_Receive(&huart1,&ch,1,0xffff)!=HAL_OK);
	return ch;
}

/****
在的DMA模式下，使能DMA接收，并接收完后进入空闲中断函数
HAL_StatusTypeDef HAL_UARTEx_ReceiveToIdle_DMA(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Si
空闲中断的回调函数
__weak void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)



*****/





