/****************************************************************************************
* Copyright (C), 2020-2030,MyAntenna Tech. Co.,Ltd.
* 文件名： HEXProtocol.h
* 描   述：L1模组的十六进制协议命令定义
* 作   者：pamala           
* 版   本：V1.0
* 日   期: 2020.2.14
*****************************************************************************************/
#ifndef __X_HEXPROTOCOL_H_X__
#define __X_HEXPROTOCOL_H_X__
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
void HEX_Single_Meas_Cmd(void);             //单次测量
void HEX_Conti_Meas_Cmd(void);              //连续测量
void HEX_FastConti_Meas_Cmd(void);          //快速连续测量
void HEX_Stop_Meas_Cmd(void);               //停止测量
#endif

