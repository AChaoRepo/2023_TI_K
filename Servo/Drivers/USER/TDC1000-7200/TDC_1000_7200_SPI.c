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

//#include "all_header.h"
#include "main.h"
#include "spi.h"
#include "TDC_1000_7200_SPI.h"
#include "TDC_1000_7200.h"

//void TDC_SPISetup(void)
//{

//	TDC7200_CSn_PxDIR |= TDC7200_CSn_PIN;
//	TDC1000_CSn_PxDIR |= TDC1000_CSn_PIN;

//	
//	UCB1_PxSEL0 |= UCB1_SIMO_PIN + UCB1_SOMI_PIN + UCB1_UCLK_PIN;
//	UCB1_PxSEL1 &= ~(UCB1_SIMO_PIN + UCB1_SOMI_PIN + UCB1_UCLK_PIN);
//	
//	UCB1CTLW0 |= UCSWRST;
//	
//	// 3-pin, 8-bit SPI master
//	// Clock polarity high, MSB
//	// Clock Source: SMCLK
//	UCB1CTLW0 |= UCMST + UCSYNC + UCCKPL + UCMSB + UCSSEL_2;
//	
//	UCB1BRW = 0;

//}

void TDC1000_SPIByteWriteReg(uint8_t addr, uint8_t value)
{
	uint8_t temp_buffer[2]={0};
	addr |= TDC1000_WRITE_BIT;			// for write make sure bit6 is 1 
	temp_buffer[0]=addr;
	temp_buffer[1]=value;
	
	//使能SPI引脚
	HAL_GPIO_WritePin(SPI1CS_GPIO_Port, SPI1CS_Pin, GPIO_PIN_RESET);
	//TDC1000_CS_EN;
	//HAL_SPI_Transmit(&hspi1,&addr,1,1000);
	HAL_SPI_Transmit(&hspi1,temp_buffer,2,1000);
	//UCB1IFG		&= ~UCRXIFG;
	//UCB1IE		|= UCRXIE;
	//UCB1TXBUF	= addr;
	//LPM0;
	
	//UCB1IFG		&= ~UCRXIFG;
	//UCB1TXBUF	= value;
	//LPM0;
	//UCB1IE		&= ~UCRXIE;
	
	//TDC1000_CS_DIS;
  HAL_GPIO_WritePin(SPI1CS_GPIO_Port, SPI1CS_Pin, GPIO_PIN_SET);
}

uint8_t TDC1000_SPIByteReadReg(uint8_t addr)
{
	uint8_t Tx_temp_buffer[2]={0};
	uint8_t Rx_temp_buffer[2]={0};
	uint8_t value;
	addr &= ~TDC1000_WRITE_BIT;
	Tx_temp_buffer[0]=addr;
	Tx_temp_buffer[1]=0;
	
	//TDC1000_CS_EN;
  //使能SPI引脚
	//HAL_GPIO_WritePin(SPI1CS_GPIO_Port, SPI1CS_Pin, GPIO_PIN_RESET);
	//HAL_SPI_TransmitReceive(&hspi1,&addr,&value,1,1000);
	//addr=0;
	//HAL_SPI_TransmitReceive(&hspi1,&addr,&value,1,1000);
	HAL_SPI_TransmitReceive(&hspi1,Tx_temp_buffer,Rx_temp_buffer,2,1000);
	//TDC1000_CS_DIS;
  HAL_GPIO_WritePin(SPI1CS_GPIO_Port, SPI1CS_Pin, GPIO_PIN_SET);
	value=Rx_temp_buffer[1];
	return value;
	//HAL_SPI_TransmitReceive(SPI_HandleTypeDef *hspi, const uint8_t *pTxData, uint8_t *pRxData,
  //                                        uint16_t Size, uint32_t Timeout)
//	value		= UCB1RXBUF;
//	UCB1IE		|= UCRXIE;
//	UCB1TXBUF	= addr;
//	LPM0;
//	value		= UCB1RXBUF;

//	UCB1TXBUF	= 0;
//	LPM0;
//	value		= UCB1RXBUF;

//	UCB1IE		&= ~UCRXIE;
//	TDC1000_CS_DIS;
//	
	
}

void TDC7200_SPIByteWriteReg(uint8_t addr, uint8_t value)
{
	
	uint8_t temp_buffer[2]={0};
	
	addr &= ~(TDC7200_AUTOINC_ON_BIT);
	addr |= TDC7200_WRITE_BIT;
	temp_buffer[0]=addr;
	temp_buffer[1]=value;
	//TDC7200_CS_EN;
  HAL_GPIO_WritePin(SPI3CS_GPIO_Port, SPI3CS_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi3,temp_buffer,2,1000);
	//HAL_SPI_Transmit(&hspi3,&value,1,1000);
	HAL_GPIO_WritePin(SPI3CS_GPIO_Port, SPI3CS_Pin, GPIO_PIN_SET);
//	
//	UCB1IE	&= ~ (UCTXIE + UCRXIE);


//	UCB1TXBUF	= addr;
//	while(!(UCB1IFG & UCRXIFG));
//	UCB1TXBUF	= value;
//	while(!(UCB1IFG & UCRXIFG));

//	TDC7200_CS_DIS;
}


uint8_t TDC7200_SPIByteReadReg(uint8_t addr)
{
	uint8_t value;
	
	addr &= ~(TDC7200_AUTOINC_ON_BIT + TDC7200_WRITE_BIT);
	uint8_t Tx_temp_buffer[2]={0};
	uint8_t Rx_temp_buffer[2]={0};
	Tx_temp_buffer[0]=addr;
	Tx_temp_buffer[1]=0;
	HAL_GPIO_WritePin(SPI3CS_GPIO_Port, SPI3CS_Pin, GPIO_PIN_RESET);
	//HAL_SPI_TransmitReceive(&hspi3,&addr,&value,1,1000);
	//addr=0;
	HAL_SPI_TransmitReceive(&hspi3,Tx_temp_buffer,Rx_temp_buffer,2,1000);
	
	HAL_GPIO_WritePin(SPI3CS_GPIO_Port, SPI3CS_Pin, GPIO_PIN_SET);
	value=Rx_temp_buffer[1];	
	return value;
//	TDC7200_CS_EN;
//	
//	UCB1IFG		&= ~UCRXIFG;
//	UCB1IE		|= UCRXIE;
//	UCB1TXBUF	= addr;
//	LPM0;
//	UCB1IFG		&= ~UCRXIFG;

//	UCB1TXBUF	= 0;
//	LPM0;
//	value		= UCB1RXBUF;
//	
//	UCB1IE		&= ~UCRXIE;
//	TDC7200_CS_DIS;



}

void TDC7200_SPIAutoIncReadReg(uint8_t addr, uint8_t *buffer, uint8_t size)
{
	uint8_t i;
  uint8_t Tx_temp_buffer[64]={0};
  uint8_t Rx_temp_buffer[64]={0};
	addr &= ~TDC7200_WRITE_BIT;
	addr |= TDC7200_AUTOINC_ON_BIT;
	uint8_t value=0;
	Tx_temp_buffer[0]=addr;
	//TDC7200_CS_EN;
//	HAL_GPIO_WritePin(SPI3CS_GPIO_Port, SPI3CS_Pin, GPIO_PIN_RESET);
//	HAL_SPI_TransmitReceive(&hspi3,&addr,&value,1,1000);
//	addr=0;
//	HAL_SPI_TransmitReceive(&hspi3,&addr,buffer,size,10000);
	
	HAL_SPI_TransmitReceive(&hspi3,Tx_temp_buffer,Rx_temp_buffer,(size+1),10000);
	HAL_GPIO_WritePin(SPI3CS_GPIO_Port, SPI3CS_Pin, GPIO_PIN_SET);
	for(i=0;i<size;i++)
	{buffer[i]=Rx_temp_buffer[i+1];}
//	UCB1IFG		&= ~UCRXIFG;
//	UCB1IE		&= ~UCRXIE;
//	UCB1IE		&= ~UCTXIE;

//	UCB1TXBUF	= addr;

//	while(!(UCB1IFG & UCRXIFG));

//	do
//	{
//		UCB1TXBUF	= 0;
//	while(!(UCB1IFG & UCTXIFG));
//		*buffer++	= UCB1RXBUF;
//	}
//	while(--size);
//	
//	UCB1IE		&= ~UCRXIE;
//	TDC7200_CS_DIS;
}

void TDC7200_SPIAutoIncWriteReg(uint8_t addr, uint8_t *buffer, uint8_t size)
{
	//uint8_t value=0;
	uint8_t i;
	uint8_t Tx_temp_buffer[64]={0};
	uint8_t Rx_temp_buffer[64]={0};
	addr |= TDC7200_WRITE_BIT + TDC7200_AUTOINC_ON_BIT;
	Tx_temp_buffer[0]=addr;
	for(i=0;i<size;i++)
	{Tx_temp_buffer[i+1]=buffer[i];}
	HAL_GPIO_WritePin(SPI3CS_GPIO_Port, SPI3CS_Pin, GPIO_PIN_RESET);
	
	//HAL_SPI_TransmitReceive(&hspi3,&addr,&value,1,1000);
	//value=0;
	//HAL_SPI_TransmitReceive(&hspi3,buffer,&value,size,10000);
	HAL_SPI_TransmitReceive(&hspi3,Tx_temp_buffer,Rx_temp_buffer,(size+1),10000);
	HAL_GPIO_WritePin(SPI3CS_GPIO_Port, SPI3CS_Pin, GPIO_PIN_SET);
	
	
//	TDC7200_CS_EN;
//	
//	UCB1IFG		&= ~UCRXIFG;
//	UCB1IE		|= UCRXIE;
//	UCB1TXBUF	= addr;
//	LPM0;
//	
//	do
//	{
//		UCB1IFG		&= ~UCRXIFG;
//		UCB1TXBUF	= *buffer++;
//		LPM0;
//	}
//	while(--size);

//	UCB1IE		&= ~UCRXIE;
//	TDC7200_CS_DIS;
}

//// Read 3 bytes long data
uint32_t TDC7200_SPILongReadReg(uint8_t addr)
{
	uint8_t   Tx_temp_buffer[4]={0};
	uint8_t   Rx_temp_buffer[4]={0};
	
	uint32_t	result = 0;
	uint8_t		size = 3;
	
	addr &= ~(TDC7200_AUTOINC_ON_BIT + TDC7200_WRITE_BIT);
	Tx_temp_buffer[0]=addr;
	
	
	HAL_GPIO_WritePin(SPI3CS_GPIO_Port, SPI3CS_Pin, GPIO_PIN_RESET);
	//HAL_SPI_TransmitReceive(&hspi3,&addr,&result,1,1000);
	//value=0;
	//addr=0;
	//HAL_SPI_TransmitReceive(&hspi3,addr,tempbuffer,size,3000);
	HAL_SPI_TransmitReceive(&hspi3,Tx_temp_buffer,Rx_temp_buffer,(size+1),3000);
	HAL_GPIO_WritePin(SPI3CS_GPIO_Port, SPI3CS_Pin, GPIO_PIN_SET);
	
	
	result =  Rx_temp_buffer[1];
	result =	(result<< 8)+Rx_temp_buffer[2];
	result =	(result<< 8)+Rx_temp_buffer[3];
	//HAL_GPIO_WritePin(SPI3CS_GPIO_Port, SPI3CS_Pin, GPIO_PIN_SET);
	return result;
//	TDC7200_CS_EN;
//	
//	UCB1IFG		&= ~UCRXIFG;
//	
//	UCB1IE		|= UCRXIE;
//	UCB1TXBUF	= addr;
//	LPM0;
//	UCB1IFG		&= ~UCRXIFG;
//	
//	do
//	{
//		UCB1TXBUF	= 0;
//		LPM0;
//		result		<<= 8;
//		result		+= UCB1RXBUF;
//	}
//	while(--size);
//	
//	UCB1IE		&= ~UCRXIE;
//	TDC7200_CS_DIS;
	

}


//#pragma vector=USCI_B1_VECTOR
//__interrupt void USCI_B1_ISR(void)
//{
//	switch (__even_in_range(UCB1IV, USCI_SPI_UCTXIFG))
//	{
//		case USCI_SPI_UCRXIFG:		//Vector 2 - RXIFG
//			LPM0_EXIT;
//			break;
//		case USCI_SPI_UCTXIFG:		//Vector 2 - TXIFG
//			LPM0_EXIT;
//			break;
//		
//		default: break;
//	}
//}
