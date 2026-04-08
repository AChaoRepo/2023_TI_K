/* --COPYRIGHT--,BSD
 * Copyright (c) 2014, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * --/COPYRIGHT--*/
 
 /*************************************
 STM32H743与TDC1000引脚连接
 SPI:  
    19--->  CSB  ----> PA4   TDC1000的SPi通信的片选引脚（低电平有效）
  	18--->	SCLK ----> PA5   TDC1000的SPi通信的串行时钟引脚
  	20--->	SDI  ----> PA7   TDC1000的SPi通信的串行数据输入
    21--->  SDO  ----> PA6   TDC1000的SPi通信的串行数据输出
    
    15--->  EN   ---->PC4    TDC1000的使能引脚(高电平有效，低电平时TDC处于睡眠状态)   
		17--->  RST  ---->PC5    TDC1000的复位引脚（高电平有效）
    
STM32H743 与TDC7200引脚连接
SPI:
    11--->  CSB -----> PD0   TDC7200的SPi通信的片选引脚（低电平有效） 
		12--->  SCLK ----> PC10  TDC7200的SPi通信的串行时钟引脚
		10--->  DIN  ----> PC12  TDC7200的SPi通信的串行数据输入
		9 --->  DOU  ----> PC11  TDC7200的SPi通信的串行数据输出
    8 --->  INT  ----> PD1   TDC7200测量完成的中断信号，低电平有效
    1 --->  EN   ----> PD3   TDC7200使能信号		


 **************************************/


/*****************************************
TDC1000的初始化配置  时钟频率：12.5MHz  探头谐振频率：200KHz
uint8_t TDC1000_reg_local_copy[10] = {0xA9, 0x47, 0x12, 0x02, 0x1F, 0x88, 0x19, 0x01, 0x23, 0x01};
CONFIG_0  	0xA9  10101001 	需要64分频101       01001 一个突发中TX脉冲的数量为9 
CONFIG_1  	0x47  01000111	均值计算的测量周期1个，预期接收事件的数量为7个  
CONFIG_2  	0x12  00010010	 共模电压内部基准控制，AFE测量类型：飞行时间测量
                   	         TX突发阻尼禁止，启用自动通道交换
                   		       禁止外部通道选择          CH_SEL=0 处于工作状态的TX/RX通道为通道0
					  		             飞行模式测量：模式1
CONFIG_3  	0x02  00000010  	 Vcom 的回波鉴定DAC阀值电平：1h -50mv
CONFIG_4  	0x1F  00011111  	 接收回波模式：0 单回波模式  触发边沿极性上升沿触发
					 		 TX 180度脉冲以为为默认
TOF_1寄存器    	0X88  10001000  	 PGA_GAIN为4h  12dB  PGA处于运行状态
					  		 LNA 绕过并断电，LNA：0位电容式            1为电阻式	 
TOF_0寄存器      0X19	00011001     0x19
ERROR_FLAGS   0X01    00000001     接收到的回波振幅超过比较器
TIMEOUT寄存器     0X23      00100011      
CLOK_RATE寄存器 0X01        00000001   


TDC7200 的初始化
//uint8_t TDC7200_reg_local_copy[10] = {0x02, 0x44, 0x07, 0x07, 0xFF, 0xFF, 0xFF, 0xFF, 0x0, 0x0 };
//CONFIG1   0X02   选择测量模式2        TRIGG_EDGE STOP_EDGE START_EDGE均选择上升沿输出 
//CONFIG2   0X44   CALIBRATION2_PERIODS 10个时钟周期
				   100 5个停止位
//INT_STATUS 0x07  设置中断
//INT_MASK   0X07  开启中断
//COARSE_CNTR_OVF_H 0XFF 设置粗计数器高位的溢出触发值 
//COARSE_CNTR_OVF_H 0XFF  设置粗计数器低位的溢出触发值 
//CLOCK_Counter_Overflow_Value_H 
//CLOCK_Counter_Overflow_Value_L 
//CLOCK_CNTR_STOP_MASK_H   0x00
//CLOCK_CNTR_STOP_MASK_L   0x00
 */
 
 
 
 
//#include "all_header.h"
#include "main.h"
#include "TDC_1000_7200.h"
#include "TDC_1000_7200_SPI.h"
#include "delay.h"

//用于表示测量是否完成
uint8_t TDC_INT_FLAG=0;

uint8_t TDC7200_reg_local_copy[10] = {0x02, 0x44, 0x07, 0x07, 0xFF, 0xFF, 0xFF, 0xFF, 0x0, 0x0 };
uint8_t TDC1000_reg_local_copy[10] = {0xA9, 0x47, 0x12, 0x02, 0x1F, 0x88, 0x19, 0x01, 0x23, 0x01};

uint8_t upStreamBuf[BUF_LENGTH];
uint8_t downStreamBuf[BUF_LENGTH];
uint8_t RTDBuf[BUF_LENGTH];

uint8_t TDC1000_Err_Flag, TDC7200_Err_Flag;

volatile uint8_t MEASURE_MODE;
volatile uint16_t tdc_state_flag = TDC_DEFAULT_FLAG;
volatile uint16_t count_per_temp = 0;
volatile uint16_t count_meassure_temp = 0;


void Init_TDC1000_TDC7200(void)
{
	//UCB1CTLW0 &= ~UCSWRST;								// Switch on SPI module
	
	//  TDC1000reset  TDC1000复位高电平有效
	tdc1000_reset();									// Reset TDC_1000
  //tdc7200_reset();

	
	//TDC1000和TDC7200使能
	HAL_GPIO_WritePin(GPIOC, TDC1000_EN_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOD, TDC7200_EN_Pin, GPIO_PIN_SET);
	//TDC1000_ENABLE_PxOUT |= TDC1000_ENABLE_PIN;			// Enable TDC1000
	//TDC7200_ENABLE_PxOUT |= TDC7200_ENABLE_PIN;			// Enable TDC7200
	//delay_uS(TDC7200_WAKEUP_PERIOD);				    // Reset TDC7200
  delay_ms(2);
	
	//TDC1000_CS_DIS;										// TDC1000 chip disable for SPI
	//TDC7200_CS_DIS;										// TDC7200 chip disable for SPI
  
	// TDC1000和TDC7200的片选引脚失效	
	HAL_GPIO_WritePin(SPI1CS_GPIO_Port, SPI1CS_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(SPI3CS_GPIO_Port, SPI3CS_Pin, GPIO_PIN_SET);
	
	TDC1000_reg_init();									// init registers of TDC1000
	
	TDC7200_reg_init();									// init registers of TDC7200

	//UCB1CTLW0 |= UCSWRST;								// Switch oFF SPI module
	//TDC1000_CS_EN;
	HAL_GPIO_WritePin(SPI1CS_GPIO_Port, SPI1CS_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(SPI3CS_GPIO_Port, SPI3CS_Pin, GPIO_PIN_RESET);
	//TDC7200_CS_EN;
	//TDC1000_ENABLE_PxOUT &= ~TDC1000_ENABLE_PIN;		// Enable TDC1000
	
	//TDC7200_ENABLE_PxOUT &= ~TDC7200_ENABLE_PIN;		// Enable TDC7200
	
	//HAL_GPIO_WritePin(GPIOC, TDC1000_EN_Pin, GPIO_PIN_RESET);
	//HAL_GPIO_WritePin(GPIOD, TDC7200_EN_Pin, GPIO_PIN_RESET);
}

//******************************************************************************
void TDC1000_reg_init(void)
{
unsigned int i;

for (i= 0; i< 10; i++)
{
	TDC1000_SPIByteWriteReg(i, TDC1000_reg_local_copy[i]); 	// write in TDC1000 Registers
}
	MEASURE_MODE = TDC1000_reg_local_copy[2];
	MEASURE_MODE &= 0X03;

}

//******************************************************************************
void TDC7200_reg_init(void)
{
	TDC7200_SPIAutoIncWriteReg(TDC7200_CONFIG1_REG, TDC7200_reg_local_copy, TDC7200_TOTAL_NUM_CONFIG_REG);
}

uint8_t get_TDC1000_mode(void)
{
	uint8_t mch, chswp, meas_mode, bdata, mx_sel;
	uint8_t device_mode;

	bdata = TDC1000_SPIByteReadReg(TDC1000_CONFIG2_REG);
	meas_mode = bdata & 0x03;
	mch = (bdata & 0x08)>>3;
	chswp = (bdata & 0x10)>>4;
	mx_sel = (bdata & 0x40)>>6;

	if (mx_sel == TOF_MEASUREMENT)
	{
		if ((chswp == ENABLED) && (mch == DISABLED) && (meas_mode == MULTI_CYCLE))
		device_mode = AUTO_FLOW;
		else if (mch == ENABLED)
		device_mode = MANUAL_FLOW;
		else
		device_mode = REGULAR_TOF;
	} else
	device_mode = REGULAR_TMP;
	return device_mode;
}

void tdc1000_reset(void)
{
	//TDC1000_RESET_PxOUT |= TDC1000_RESET_PIN;
	// wait for 10usec  
	//delay_uS(10);
	//TDC1000_RESET_PxOUT &= ~TDC1000_RESET_PIN;
	//delay_uS(10);
	HAL_GPIO_WritePin(GPIOC, TDC1000_RESET_Pin, GPIO_PIN_RESET);
	delay_us(500);
	HAL_GPIO_WritePin(GPIOC, TDC1000_RESET_Pin, GPIO_PIN_SET);
	delay_us(500);
	
}

void tdc7200_reset(void)
{
	//TCD7200使能 高电平 
	HAL_GPIO_WritePin(GPIOD, TDC7200_EN_Pin, GPIO_PIN_RESET);
	delay_us(500);
	HAL_GPIO_WritePin(GPIOD, TDC7200_EN_Pin, GPIO_PIN_SET);
	delay_us(500);
		// Unset "Start New Measurement" bit to avoid measurement being triggered
	TDC7200_reg_local_copy[TDC7200_CONFIG1_REG] &= ~0x01;
	TDC7200_SPIAutoIncWriteReg(TDC7200_CONFIG1_REG, TDC7200_reg_local_copy, TDC7200_TOTAL_NUM_CONFIG_REG);
	
	
//	TDC7200_ENABLE_PxOUT &= ~TDC7200_ENABLE_PIN;
//	tdc_disable_clock();
//	// wait for 1sec for the cap to discharge
//	delay_ACLK(24576);
//	TDC7200_ENABLE_PxOUT |= TDC7200_ENABLE_PIN;
//	// give at least 500us
//	delay_uS(500);
//	tdc_enable_clock();
//	
//	// Unset "Start New Measurement" bit to avoid measurement being triggered
//	TDC7200_reg_local_copy[TDC7200_CONFIG1_REG] &= ~0x01;
//	TDC7200_SPIAutoIncWriteReg(TDC7200_CONFIG1_REG, TDC7200_reg_local_copy, TDC7200_TOTAL_NUM_CONFIG_REG);
}

//void tdc_enable_clock(void)
//{
//#ifdef USE_OSC
//	TDC1000_OSCENABLE_PxOUT |= TDC1000_OSCENABLE_PIN;		// Set pin high: enable afe osc
//#else
//	// Output SMCLK
//	TDC1000_XCLK_PxSEL0	|= TDC1000_XCLK_PIN;
//	TDC1000_XCLK_PxSEL1	|= TDC1000_XCLK_PIN;
//	TDC1000_XCLK_PxDIR	|= TDC1000_XCLK_PIN;
//#endif
//}

//void tdc_disable_clock(void)
//{
//#ifdef USE_OSC
//	TDC1000_OSCENABLE_PxOUT &= ~TDC1000_OSCENABLE_PIN;		// Set pin high: enable afe osc
//#else
//	// Output SMCLK
//	TDC1000_XCLK_PxOUT	&= ~TDC1000_XCLK_PIN;
//	TDC1000_XCLK_PxSEL0	&= ~TDC1000_XCLK_PIN;
//	TDC1000_XCLK_PxSEL1	&= ~TDC1000_XCLK_PIN;
//	TDC1000_XCLK_PxDIR	|= TDC1000_XCLK_PIN;
//#endif

//}

void tdc_power_cycle_on(void)
{
	// enable external osc before starting measurement
	//tdc_enable_clock();
	
	//delay_ACLK(98);												// use default delay if input is 0
  
	//TDC1000_ENABLE_PxOUT |= TDC1000_ENABLE_PIN;					// enable afe
	//TDC7200_ENABLE_PxOUT |= TDC7200_ENABLE_PIN;					// Enable device
	//delay_uS(TDC7200_WAKEUP_PERIOD);							// wait for TDC7200 wakeup delay
  
	//TDC1000和TDC7200使能
	HAL_GPIO_WritePin(GPIOC, TDC1000_EN_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOD, TDC7200_EN_Pin, GPIO_PIN_SET);
  HAL_Delay(1);
	// Unset "Start New Measurement" bit to avoid measurement being triggered
	TDC7200_reg_local_copy[TDC7200_CONFIG1_REG] &= ~0x01;
	
	// init TDC7200 with local saved data
	TDC7200_SPIAutoIncWriteReg(TDC7200_CONFIG1_REG, TDC7200_reg_local_copy, TDC7200_TOTAL_NUM_CONFIG_REG);
}

void tdc_power_cycle_off(void)
{
	// Backup register values before disable
	TDC7200_SPIAutoIncReadReg(TDC7200_CONFIG1_REG, TDC7200_reg_local_copy, TDC7200_TOTAL_NUM_CONFIG_REG);

	// Disable device
	//TDC7200_ENABLE_PxOUT &= ~TDC7200_ENABLE_PIN;    
	//TDC1000_ENABLE_PxOUT &= ~TDC1000_ENABLE_PIN;				// disable afe
	
	HAL_GPIO_WritePin(GPIOC, TDC1000_EN_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD, TDC7200_EN_Pin, GPIO_PIN_RESET);
	
	// disable osc after completing measurement
	//tdc_disable_clock();
}

void tdc_trigger_common(uint8_t *buf)
{
	uint8_t byte_data;
	
	byte_data = TDC7200_reg_local_copy[0];

	// set start measurement bit & use default mode or set by user
	TDC_INT_FLAG=0;
	byte_data |= 0x01;
	TDC7200_SPIByteWriteReg(TDC7200_CONFIG1_REG, byte_data);
		
	//start_ms_timeout(500);
	
	//wait for INTB pin to go low
//	TDC7200_INTB_PxIFG &= ~TDC7200_INTB_PIN;
//	TDC7200_INTB_PxIE |= TDC7200_INTB_PIN;
//	LPM3;
//	TDC7200_INTB_PxIE &= ~TDC7200_INTB_PIN;
	
	while(TDC_INT_FLAG==0);
	
	//stop_ms_timeout();

//	if (timeout)
//	{
//		__no_operation();
//	}
	
	// Read all the result registers
	TDC7200_SPIAutoIncReadReg(TDC7200_TIME1_REG, buf, TDC7200_TOTAL_NUM_RESULT_REG);
	buf[39] = 0x00;
}

//******************************************************************************
void tdc_trigger_measure(void)
{
	uint8_t temp1, temp2;
	
	//if (tdc_state_flag & (TDC_CONTINUOUS_TRIGGER_STATE + TDC_TOF_GRAPTH_STATE + TDC_SINGLE_SHOT_MEASURE_STATE))
	if(1)
	{
		// Perform power cycle if enabled
//		if (tdc_state_flag & TDC_POWER_CYCLE_FLAG)
//		{
//			tdc_power_cycle_on();
//		}
//		
		// Clear error flags and reset state machine
		TDC1000_SPIByteWriteReg(TDC1000_ERROR_FLAGS_REG, 0x03);      
		TDC7200_SPIByteWriteReg(TDC7200_INTRPT_STATUS_REG, 0x1F);      

		// Measure for up stream

		tdc_trigger_common(upStreamBuf);


		if (tdc_state_flag & (TDC_TOF_GRAPTH_STATE + TDC_SINGLE_SHOT_MEASURE_STATE))
		{      
			tdc_state_flag |= TDC_UP_STREAM_BUFFER_READY;
		}
		
		// TDC1000 Enable Pin toggling effect: during AUTO_FLOW state m/c is reset
		// need to make downstream measurement also before shutting down
		if (MEASURE_MODE == AUTO_FLOW)
		{
			// Give delay for transducer to settle down
			//delay_ACLK(17);
			HAL_Delay(1);
			tdc_trigger_common(downStreamBuf);

			if (tdc_state_flag & TDC_TOF_GRAPTH_STATE)
			{      
				tdc_state_flag |= TDC_DOWN_STREAM_BUFFER_READY;
			}     
		}
		
		// Temperature measurement
		if (tdc_state_flag & TDC_INTERLEAVED_TEMP_MEASURE)
		{
			if (!(--count_meassure_temp))
			{
				// Enable Temperature measurement bit
				temp1 = TDC1000_SPIByteReadReg(TDC1000_CONFIG2_REG);
				TDC1000_SPIByteWriteReg(TDC1000_CONFIG2_REG, temp1 | 0x40);
				
				// config TDC7200 for 5 stops
				// clear last 6 bits: both num stops and avgng cycles
				temp2 = TDC7200_SPIByteReadReg(TDC7200_CONFIG2_REG);
				TDC7200_SPIByteWriteReg(TDC7200_CONFIG2_REG, (temp2 & 0xC0) | 0x04);
				
				tdc_trigger_common(RTDBuf);
				RTDBuf[39] = 0xA5;

				// Restore register
				TDC1000_SPIByteWriteReg(TDC1000_CONFIG2_REG, temp1);
				TDC7200_SPIByteWriteReg(TDC7200_CONFIG2_REG, temp2);
				
				count_meassure_temp = count_per_temp;
				tdc_state_flag |= TDC_RTD_BUFFER_READY;
			}
		}
		
		if (tdc_state_flag & TDC_POWER_CYCLE_FLAG)
		{
			tdc_power_cycle_off();
		}
	}
}

//// PORT1 Interrupt Vector (P1IV) handler
//#pragma vector=PORT2_VECTOR
//__interrupt void PORT2_ISR(void)
//{
//	switch( P2IV )
//	{
//	case 0: break;
//	case 2: break;
//	case 4:	break;
//	case 6:	break;
//	case 8: break;
//	case 10: break;
//	case 12: break;
//	case 14:
//		LPM3_EXIT;
//		break;
//	case 16: break;
//	}
//}



void TDC_Chips_On(void)
{
	//UCB1CTLW0 &= ~UCSWRST;								// Switch on SPI module
	//tdc_enable_clock();									// Enable 8MHz Oscillator

	//TDC1000_ENABLE_PxOUT |= TDC1000_ENABLE_PIN;			// Enable TDC1000
	//TDC7200_ENABLE_PxOUT |= TDC7200_ENABLE_PIN;			// Enable TDC7200
  HAL_GPIO_WritePin(GPIOC, TDC1000_EN_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOD, TDC7200_EN_Pin, GPIO_PIN_SET);
	//开启数据测量完成中断
	HAL_NVIC_EnableIRQ(EXTI1_IRQn);
	//TDC7200_INTB_PxREN	|= TDC7200_INTB_PIN;			// Pull-up Resistor enable for Interrupt input

	
	HAL_GPIO_WritePin(SPI1CS_GPIO_Port, SPI1CS_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(SPI3CS_GPIO_Port, SPI3CS_Pin, GPIO_PIN_SET);
	 
	//TDC1000_CS_DIS;										// TDC1000 chip disable for SPI
	//TDC7200_CS_DIS;										// TDC7200 chip disable for SPI

	TDC1000_reg_init();									// init registers of TDC1000
	TDC7200_reg_init();									// init registers of TDC7200
  
	HAL_GPIO_WritePin(SPI1CS_GPIO_Port, SPI1CS_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(SPI3CS_GPIO_Port, SPI3CS_Pin, GPIO_PIN_SET);
	//TDC1000_CS_DIS;
	//TDC7200_CS_DIS;

}

void TDC_Chips_Off(void)
{

	//关闭初始
	HAL_GPIO_WritePin(GPIOC, TDC1000_EN_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD, TDC7200_EN_Pin, GPIO_PIN_RESET);
	//关闭数据测量完成中断
	HAL_NVIC_DisableIRQ(EXTI1_IRQn);
	//HAL_NVIC_EnableIRQ(EXTI1_IRQn);
//#ifdef GUI

//unsigned int i;

//	// Read and store Register settings of TDC7200
//	TDC7200_SPIAutoIncReadReg(TDC7200_CONFIG1_REG, TDC7200_reg_local_copy, TDC7200_TOTAL_NUM_CONFIG_REG);
//	// reset "Start New Measurement" bit to avoid measurement being triggered
//	TDC7200_reg_local_copy[TDC7200_CONFIG1_REG] &= ~0x01;

//	// Read and store Register settings of TDC1000
//	for (i=0; i< 10; i++)
//	{TDC1000_reg_local_copy[i] = TDC1000_SPIByteReadReg(i) ;}

//#endif
////#esle

//	tdc_disable_clock();								// Disable 8MHz oscillator

//	TDC1000_ENABLE_PxOUT &= ~TDC1000_ENABLE_PIN;		// Disable TDC1000
//	TDC7200_ENABLE_PxOUT &= ~TDC7200_ENABLE_PIN;		// Disable TDC7200

//	TDC7200_INTB_PxREN &= ~TDC7200_INTB_PIN;			// Pull-up Resistor disabled

//	TDC1000_CS_EN;										// Pull low the I/O pins for SPI selection
//	TDC7200_CS_EN;

//	UCB1CTLW0 |= UCSWRST;							    // Switch off SPI

////#endif

}
