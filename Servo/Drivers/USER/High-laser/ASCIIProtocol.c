/****************************************************************************************
* Copyright (C), 2020-2030,MyAntenna Tech. Co.,Ltd.
* 文件名： ASCIIProtocol.h
* 描   述：L1模组的ASCII协议指令的实现
* 作   者：pamala           
* 版   本：V1.0
* 日   期: 2020.2.14
*****************************************************************************************/
#include <stdio.h>
#include <string.h>
#include "ASCIIProtocol.h"
//#include "hal_drivers.h"
#include "main.h"
#include "usart.h"
#include "delay.h"

#define L1MOD_USART huart4

/*****激光测距的数据收发采用UART4*************
波特率：38400
TX：PA12
RX：PA11
*********************************************/

uint8_t UART4_Buffer_State=Ready;//缓存区是否正在存储数据，Busy：正在存储数据  Ready：未存储数据，可以使用
uint8_t Data_State=Busy;//数据是否被读取，Busy：数据已经被读取,Ready:数据未被读取，可以读取数据
uint8_t UART4_Rx_Buffer[128]={0};
uint8_t Rx_Buffer[128]={0};



usart_buf_t g_L1Mod_node;
/*****************************************************
* 函数名：Clear_L1Mod_Usart
* 参  数：void
* 返回值：void
* 描  述：清除串口缓冲区
* 作  者：pamala， 2020.2.16
*****************************************************/
void Clear_L1Mod_Usart()
{
	  g_L1Mod_node.length = 0;
	  g_L1Mod_node.valid  = false;
	  g_L1Mod_node.type   = 0;
	  memset(g_L1Mod_node.buf, 0, 256);
	  
}


/**************************
串口4的发送采用中断方式完成，接收采用DMA空闲中断的方式完成
*/
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
	uint8_t i;
	if(huart->Instance==UART4)
	{
    UART4_Buffer_State=Busy;
		for(i=0;i<Size;i++)
    {g_L1Mod_node.buf[i]=UART4_Rx_Buffer[i];}
		//{Rx_Buffer[i]=UART4_Rx_Buffer[i];}
		//Rx_Buffer[Size]='\0';			
		memset(UART4_Rx_Buffer,0,128);//接收数组清零   
	//__HAL_UART_ENABLE_IT(&huart4, UART_IT_IDLE);			
	//HAL_UARTEx_ReceiveToIdle_DMA(&huart4,UART4_Rx_Buffer, 128);
		
		UART4_Buffer_State=Ready;
		Data_State=Ready;
		HAL_UARTEx_ReceiveToIdle_IT(&huart4,UART4_Rx_Buffer,128);	
	}
    
}


/***********************************************************
* 函数名：Get_Param_Cmd
* 参数：
*    x[in]: 命令类型
* 返回值：成功返回ture，失败返回false
* 描述：根据不同的命令类型，读取L1模组的参数值
* 作者：pamala, 2020.2.15
************************************************************/
bool Get_Param_Cmd(unsigned char x)
{
    unsigned char cmd[16] = {0};
		
		sprintf((char*)cmd, "iGET:%d", x);
    
		Clear_L1Mod_Usart();
    HAL_UART_Transmit(&L1MOD_USART, cmd, strlen((char*)cmd),1000);		
		if(strstr((char*)g_L1Mod_node.buf, "OK")!= NULL)
		{
			  return true;
		}
		else
		{
			  return false;
		}
}

/***********************************************************
* 函数名：Set_Param_Cmd
* 参数：
*    x[in]: 命令类型
*    y[in]: 设置参数值
* 返回值：成功返回true，失败返回false
* 描述：根据不同的命令类型，设置L1模组的参数值
* 作者：pamala, 2020.2.15
************************************************************/
bool Set_Param_Cmd(unsigned char x, int y)
{
    unsigned char cmd[32] = {0};
		sprintf((char*)cmd, "iSET:%d,%d", x, y);
		
		Clear_L1Mod_Usart();
		HAL_UART_Transmit(&L1MOD_USART, cmd, strlen((char*)cmd),1000);
		//Usart_Write_Bytes(L1MOD_USART, cmd, strlen((char*)cmd));
		delay_ms(200);
		if(strstr((char*)g_L1Mod_node.buf, "OK")!= NULL)
		{
			  return true;
		}
		else
		{
			  return false;
		}
}

/***********************************************************
* 函数名：Single_Meas_Cmd
* 参  数：void
* 返回值：成功返回true，失败返回false
* 描  述：发送单次测量命令
* 作者：pamala, 2020.2.15
************************************************************/
bool Single_Meas_Cmd(void)
{   
	  unsigned char cmd[3] = {0};
		sprintf((char*)cmd, "iSM");
	  Clear_L1Mod_Usart();
	  HAL_UART_Transmit(&L1MOD_USART, cmd, strlen((char*)cmd),1000);
		//Usart_Write_Bytes(L1MOD_USART, (unsigned char*)"iSM", 3);
		delay_ms(100);
		if(strstr((char*)g_L1Mod_node.buf, "D=")!= NULL)
		{
			  return true;
		}
		else
		{
			  return false;
		}
    
}

/***********************************************************
* 函数名：Conti_Meas_Cmd
* 参  数：void
* 返回值：成功返回true，失败返回false
* 描  述：发送连续测量命令
* 作者：pamala, 2020.2.15
************************************************************/
bool Conti_Meas_Cmd(void)
{
    unsigned char cmd[4] = {0};
		sprintf((char*)cmd, "iACM");
	  Clear_L1Mod_Usart();
	  g_L1Mod_node.type = 1;
	 
	  HAL_UART_Transmit(&L1MOD_USART, cmd, strlen((char*)cmd),1000);
		//Usart_Write_Bytes(L1MOD_USART, (unsigned char*)"iACM", 4);
	  return true;
}

/***********************************************************
* 函数名：Fast_Conti_Meas_Cmd
* 参  数：void
* 返回值：成功返回true，失败返回false
* 描  述：发送快速连续测量命令
* 作者：pamala, 2020.2.15
************************************************************/
bool Fast_Conti_Meas_Cmd(void)
{
    Clear_L1Mod_Usart();
	  g_L1Mod_node.type = 1;
	  unsigned char cmd[5] = {0};
		sprintf((char*)cmd, "iFACM");
		HAL_UART_Transmit(&L1MOD_USART, cmd, strlen((char*)cmd),1000);
		 //Usart_Write_Bytes(L1MOD_USART, (unsigned char*)"iFACM", 5);
	  return true;
}

/***********************************************************
* 函数名：Stop_Meas_Cmd
* 参  数：void
* 返回值：成功返回true，失败返回false
* 描  述：发送停止测量命令
* 作者：pamala, 2020.2.15
************************************************************/
bool Stop_Meas_Cmd(void)
{
    unsigned char cmd[6] = {0};
		sprintf((char*)cmd, "iHALT");
	  Clear_L1Mod_Usart();
		HAL_UART_Transmit(&L1MOD_USART, cmd, strlen((char*)cmd),1000);
	 	//Usart_Write_Bytes(L1MOD_USART, (unsigned char*)"iHALT", 5);
		delay_ms(100);
		if(strstr((char*)g_L1Mod_node.buf, "STOP")!= NULL)
		{
			  return true;
		}
		else
		{
			  return false;
		}
}

/***********************************************************
* 函数名：Laser_Switch_Cmd
* 参  数：void
* 返回值：成功返回true，失败返回false
* 描  述：发送激光开启关闭命令
* 作者：pamala, 2020.2.15
************************************************************/
bool Laser_Switch_Cmd(unsigned char status)
{
    unsigned char cmd[5] = {0};
    sprintf((char*)cmd, "iLD:%d", status);
    
		Clear_L1Mod_Usart();
		//Usart_Write_Bytes(L1MOD_USART, cmd, strlen((char*)cmd));
		HAL_UART_Transmit(&L1MOD_USART, cmd, strlen((char*)cmd),1000);
		delay_ms(100);
		if(strstr((char*)g_L1Mod_node.buf, "LASER")!= NULL)
		{
			  return true;
		}
		else
		{
			  return false;
		}
}

