/******************************************************************************
  * @file    WNK8010.c
  * @author  https://github.com/ypkdani00
  * @version V1.0
  * @date    18-04-2023
  ******************************************************************************
  The MIT License (MIT)
  Copyright © 2023 <copyright holders>
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the “Software”), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is furnished
  to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
  THE SOFTWARE.

 */
  
  /* Includes ------------------------------------------------------------------*/

#include "wnk8010.h"

#include "timeServer.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define WNK8010_1_0m 1000
#define WNK8010_2_5m 2500
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Exported functions ---------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
/* I2C handler declaration */
#ifdef USE_WNK8010
int WNK8010_pres,WNK8010_temp;
I2C_HandleTypeDef I2cHandle1;
extern bool debug_flags;
/* I2C TIMING Register define when I2C clock source is SYSCLK */
/* I2C TIMING is calculated in case of the I2C Clock source is the SYSCLK = 32 MHz */
#define I2C_TIMING    0x10A13E56 /* 100 kHz with analog Filter ON, Rise Time 400ns, Fall Time 100ns */
//#define I2C_TIMING    0x00B1112E /* 400 kHz with analog Filter ON, Rise Time 250ns, Fall Time 100ns */
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Exported functions ---------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
void  BSP_WNK8010_Init( void )
{
  /*##-1- Configure the I2C peripheral ######################################*/
  I2cHandle1.Instance              = I2Cx;
  I2cHandle1.Init.Timing           = I2C_TIMING;
  I2cHandle1.Init.AddressingMode   = I2C_ADDRESSINGMODE_7BIT;
  I2cHandle1.Init.DualAddressMode  = I2C_DUALADDRESS_DISABLE;
  I2cHandle1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  I2cHandle1.Init.GeneralCallMode  = I2C_GENERALCALL_DISABLE;
  I2cHandle1.Init.NoStretchMode    = I2C_NOSTRETCH_DISABLE;
  I2cHandle1.Init.OwnAddress1      = 0xF0;
  I2cHandle1.Init.OwnAddress2      = 0xFE;
  
  if (debug_flags == 1) {
  			PPRINTF("\r\n");
  			PPRINTF("Init WNK8010, BEGIN\r\n");
  }

  if(HAL_I2C_Init(&I2cHandle1) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
    if (debug_flags == 1) {
    	PPRINTF("Init WNK8010, ERROR\r\n");
    }
  }

  if (debug_flags == 1) {
	  PPRINTF("Init WNK8010, DONE\r\n");
  }

  /* Enable the Analog I2C Filter */
  HAL_I2CEx_ConfigAnalogFilter(&I2cHandle1,I2C_ANALOGFILTER_ENABLE);
  /* Infinite loop */
}

uint8_t WNK8010_Read(uint8_t rx_pres_data[], uint8_t rx_temp_data[])
{
	uint8_t temp_addr_val = 0x0A;

	if (debug_flags == 1) {
		PPRINTF("Read WNK8010, RAW DATA\r\n");
	}

	//HAL_I2C_Mem_Read(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size, uint32_t Timeout);
	if (HAL_I2C_Mem_Read(&I2cHandle1, WNK8010_ADDRESS_PRES+1, 0x06, 0x01, rx_pres_data, 0x03, 1000)
			!= HAL_OK) {
		return HAL_I2C_STATE_ERROR;
	}

	//HAL_I2C_Mem_Write(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size, uint32_t Timeout)
	if (HAL_I2C_Mem_Write(&I2cHandle1, WNK8010_ADDRESS_TEMP, 0x30, 1, &temp_addr_val, 0x01, 1000)
			!= HAL_OK) {
		return HAL_I2C_STATE_ERROR;
	}

	HAL_Delay(100);
	if (HAL_I2C_Mem_Read(&I2cHandle1, WNK8010_ADDRESS_TEMP+1, 0x06, 0x01, rx_temp_data, 0x03, 1000)
			!= HAL_OK) {
		return HAL_I2C_STATE_ERROR;
	}

	if (debug_flags == 1) {
		PPRINTF("Read WNK8010, RAW DATA DONE\r\n");
	}

	return HAL_I2C_STATE_RESET;
}

void __attribute__((optimize("O0"))) tran_WNK8010data(void) {
	uint8_t rx_pres_data[3] = { 0 };
	uint8_t rx_temp_data[3] = { 0 };
	uint32_t raw_pres = 0, raw_temp = 0;
	float fadc;


	IWDG_Refresh();
	if (WNK8010_Read(rx_pres_data, rx_temp_data) == HAL_I2C_STATE_RESET) {
		//pressure
		raw_pres = (rx_pres_data[0] << 16) + (rx_pres_data[1] << 8)
				+ rx_pres_data[2];

		if (raw_pres > 0x800000) {
			raw_pres = raw_pres - 16777216;
		}
		fadc = (float)raw_pres/8388608.0;
#ifdef WNK8010_PRES_SENDRAW
#warning "WNK8010, RAW PRESSURE DATA SEND"
		WNK8010_pres = raw_pres;
#else
		WNK8010_pres = ((fadc-0.1525)/0.5865) * WNK8010_1_0m; //pressure in mmH2O
#endif

		//temperature
		raw_temp = (rx_temp_data[0] << 16) + (rx_temp_data[1] << 8)
				+ rx_temp_data[2];

		if (raw_temp > 0x800000) {
			raw_temp = raw_temp - 16777216;
		}
		fadc = (float)raw_temp/8388608.0;
		WNK8010_temp = 10*(-50.0 + (fadc-0.1)/0.8 * 180.0); //temperature * 10

		if (debug_flags == 1) {
			PPRINTF("\r\n");
			PPRINTF("WNK8010 data:\r\n");
			PPRINTF("Pressure = %d, Pressure RAW = %d\r\n", (int)(WNK8010_pres), (int)raw_pres);
			PPRINTF("Temperature = %d, , Temperature RAW = %d\r\n", (int)(WNK8010_temp), (int)raw_temp);
		}
	}
	IWDG_Refresh();
}

#endif
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

