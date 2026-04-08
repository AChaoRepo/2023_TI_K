 #include "stm32h7xx_hal.h"
 #include "spi.h"
 #include "delay.h"
 #include "PGA117.h"
//   PC10     ------> SPI3_SCK
//   PC11     ------> SPI3_MISO
//   PC12     ------> SPI3_MOSI
//   PD0      ------> SPI3_CS 
//MSH_CMD_EXPORT(set_ga, set_ga [ch (0-9)][ga (1, 2, 5, 10, 20, 50)]);
//  
#define GAINE_1      0x00
#define GAINE_2      0x10
#define GAINE_5      0x20
#define GAINE_10     0x30
#define GAINE_20     0x40
#define GAINE_50     0x50 
 
//MSH_CMD_EXPORT(set_ga, set_ga [ch (0-9)][ga (1, 2, 5, 10, 20, 50)]); 
 
#define PGA117_Channel  1
#define PGA117_Ga       2
 
void pa117_init(void); 
void pa117_sdn_dis(void);
void pga117_config(uint32_t ch, uint32_t ga);

 
void pa117_sdn_dis(void)
{
	uint16_t data = 0xe100;
	//uint8_t data1[2]={0};
	//data1[0]=0xe1;
//	data1[1]=0x00;
	
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_0, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi3,(uint8_t *)&data,1,10);
	//HAL_SPI_Transmit(&hspi3,data1,2,50);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_0, GPIO_PIN_SET);
	delay_ms(100);
	
//	HAL_GPIO_WritePin(SPI3CS_GPIO_Port, SPI3CS_Pin, GPIO_PIN_SET);
//	
//	rt_pin_write(SPI_NSS_PIN1, PIN_LOW);
//	HAL_SPI_Transmit(&hspi2, (uint8_t *)&data, 1, 10);
//	rt_pin_write(SPI_NSS_PIN1, PIN_HIGH);
//	rt_thread_mdelay(100);
//	
//	rt_pin_write(SPI_NSS_PIN2, PIN_LOW);
//	HAL_SPI_Transmit(&hspi2, (uint8_t *)&data, 1, 10);
//	rt_pin_write(SPI_NSS_PIN2, PIN_HIGH);
//	rt_thread_mdelay(100);
//	
//	rt_pin_write(SPI_NSS_PIN3, PIN_LOW);
//	HAL_SPI_Transmit(&hspi2, (uint8_t *)&data, 1, 10);
//	rt_pin_write(SPI_NSS_PIN3, PIN_HIGH);
//	rt_thread_mdelay(100);

}



//
void pga117_config(uint32_t ch, uint32_t ga)
{
	uint16_t data  = 0x2a00;
	//uint8_t data1[2]={0};
	
	if (ch < 10)
	{
		data |= ch;
	  //data1[1]=data1[1]|ch;
	}
	
	switch (ga)
	{
		case 1:
			data |= GAINE_1;
		  //data1[1]=data1[1]|GAINE_1;
		break;
		case 2:
			data |= GAINE_2;
		   // data1[1]=data1[1]|GAINE_2;
		break;
		case 5:
			data |= GAINE_5;
		  // data1[1]=data1[1]|GAINE_5;
		break;
		case 10:
			data |= GAINE_10;
		   //data1[1]=data1[1]|GAINE_10;
		break;
		case 20:
			data |= GAINE_20;
		   //data1[1]=data1[1]|GAINE_20;
		break;
		case 50:
			data |= GAINE_50;
		   // data1[1]=data1[1]|GAINE_50;
		break;
	}
	//data1[0]=0x2a;
	//data1[1]=0x00;
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_0, GPIO_PIN_RESET);
	//HAL_SPI_Transmit(&hspi3,data1,2,100);
	HAL_SPI_Transmit(&hspi3,(uint8_t *)&data,1,10);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_0, GPIO_PIN_SET);
	delay_ms(100);
	
//	rt_pin_write(SPI_NSS_PIN1, PIN_LOW);
//	HAL_SPI_Transmit(&hspi2, (uint8_t *)&data, 1, 10);
//	rt_pin_write(SPI_NSS_PIN1, PIN_HIGH);
//	rt_thread_mdelay(100);
//	
//	rt_pin_write(SPI_NSS_PIN2, PIN_LOW);
//	HAL_SPI_Transmit(&hspi2, (uint8_t *)&data, 1, 10);
//	rt_pin_write(SPI_NSS_PIN2, PIN_HIGH);
//	rt_thread_mdelay(100);
//	
//	rt_pin_write(SPI_NSS_PIN3, PIN_LOW);
//	HAL_SPI_Transmit(&hspi2, (uint8_t *)&data, 1, 10);
//	rt_pin_write(SPI_NSS_PIN3, PIN_HIGH);
//	rt_thread_mdelay(100);

}

uint16_t get_pga117_config(void)
{
	uint16_t data = 0x6a00;
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_0, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi3,(uint8_t *)&data,1,10);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_0, GPIO_PIN_SET);
	delay_ms(100);
	
	
//	rt_pin_write(SPI_NSS_PIN1, PIN_LOW);
//	HAL_SPI_Transmit(&hspi2, (uint8_t *)&data, 1, 10);
//	rt_pin_write(SPI_NSS_PIN1, PIN_HIGH);
//	//rt_hw_us_delay(1);
//	rt_pin_write(SPI_NSS_PIN1, PIN_LOW);
//	HAL_SPI_Receive(&hspi2, (uint8_t *)&data, 1, 10);
//	rt_pin_write(SPI_NSS_PIN1, PIN_HIGH);
	
	return data;
}

void pa117_init(void)
{
//		rt_pin_mode(SPI_NSS_PIN1, PIN_MODE_OUTPUT);
//	  rt_pin_write(SPI_NSS_PIN1, PIN_HIGH);
//	  rt_pin_mode(SPI_NSS_PIN2, PIN_MODE_OUTPUT);
//	  rt_pin_write(SPI_NSS_PIN2, PIN_HIGH);
//	  rt_pin_mode(SPI_NSS_PIN3, PIN_MODE_OUTPUT);
//	  rt_pin_write(SPI_NSS_PIN3, PIN_HIGH);
//	  MX_SPI2_Init();  
////	
//	  rt_thread_mdelay(100);
	  pa117_sdn_dis();
//		rt_thread_mdelay(100);
	  delay_ms(100);
	  pga117_config(PGA117_Channel, PGA117_Ga);
	  delay_ms(100);
//	  rt_thread_mdelay(100);
	  //rt_kprintf("pga117_config: 0x%0x\r\n", get_pga117_config());
}
//static void set_ga(int argc, char *argv[])
//{
//	uint32_t ch, ga;
//	
//	ch = atoi(argv[1]);
//	ga = atoi(argv[2]);
//	
//	pga117_config(ch, ga);
//}
/* ??? msh ????? */
//MSH_CMD_EXPORT(set_ga, set_ga [ch (0-9)][ga (1, 2, 5, 10, 20, 50)]);
