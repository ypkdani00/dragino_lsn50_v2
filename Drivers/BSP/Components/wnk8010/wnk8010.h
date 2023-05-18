/******************************************************************************
  * @file    WNK8010.h
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __WNK8010_H__
#define __WNK8010_H__

#ifdef __cplusplus
 extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/
/* External variables --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */ 
/**
 * @brief  initialises the 
 *
 * @note
 * @retval None
 */
 
#include "hw.h"
#include "stm32l0xx_hal_i2c.h"

#define WNK8010_ADDRESS_PRES		0xDA
#define WNK8010_ADDRESS_TEMP		0xFE
#define WNK8010_READ_PRESSURE 	0x06
#define WNK8010_READ_TEMPERATURE 	0x09
#define WNK8010_RANGE				1470


void BSP_WNK8010_Init( void );
uint8_t WNK8010_Read(uint8_t rx_pres_data[], uint8_t rx_temp_data[]);
void tran_WNK8010data(void);
#ifdef __cplusplus
}
#endif

#endif
