/*---------------------------------------------------------------------------
 * Copyright (c) 2024 Arm Limited (or its affiliates).
 * All rights reserved.
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *---------------------------------------------------------------------------*/

#ifndef FRDM_MCXN947_H_
#define FRDM_MCXN947_H_

//TODO: #include "Driver_ETH_MAC.h"
//TODO: #include "Driver_I2C.h"
//TODO: #include "Driver_SPI.h"
//TODO: #include "Driver_USART.h"

// FRDM-MCXN947 Arduino connector pin definitions
//TODO: #define ARDUINO_UNO_D0      GPIO_PIN(0,0)  // USARTx: RX
//TODO: #define ARDUINO_UNO_D1      GPIO_PIN(0,0)  // USARTx: TX
//TODO: #define ARDUINO_UNO_D2      GPIO_PIN(0,0)
//TODO: #define ARDUINO_UNO_D3      GPIO_PIN(0,0)
//TODO: #define ARDUINO_UNO_D4      GPIO_PIN(0,0)
//TODO: #define ARDUINO_UNO_D5      GPIO_PIN(0,0)
//TODO: #define ARDUINO_UNO_D6      GPIO_PIN(0,0)
//TODO: #define ARDUINO_UNO_D7      GPIO_PIN(0,0)
//TODO: #define ARDUINO_UNO_D8      GPIO_PIN(0,0)
//TODO: #define ARDUINO_UNO_D9      GPIO_PIN(0,0)
//TODO: #define ARDUINO_UNO_D10     GPIO_PIN(0,0)
//TODO: #define ARDUINO_UNO_D11     GPIO_PIN(0,0)  // SPIx: MOSI
//TODO: #define ARDUINO_UNO_D12     GPIO_PIN(0,0)  // SPIx: MISO
//TODO: #define ARDUINO_UNO_D13     GPIO_PIN(0,0)  // SPIx: SCK
//TODO: #define ARDUINO_UNO_D14     GPIO_PIN(0,0)  // I2Cx: SDA
//TODO: #define ARDUINO_UNO_D15     GPIO_PIN(0,0)  // I2Cx: SCL

// CMSIS Driver instances on Arduino connector
//TODO: #define ARDUINO_UNO_I2C     0
//TODO: #define ARDUINO_UNO_SPI     0
//TODO: #define ARDUINO_UNO_UART    0

// FRDM-MCXN947 mikroBUS connector pin definitions
//TODO: #define MIKROBUS_AN         GPIO_PIN(0,0)
//TODO: #define MIKROBUS_INT        GPIO_PIN(0,0)
//TODO: #define MIKROBUS_PWM        GPIO_PIN(0,0)

// CMSIS Driver instances on mikroBUS connector
//TODO: #define MIKROBUS_I2C        0
//TODO: #define MIKROBUS_SPI        3
//TODO: #define MIKROBUS_UART       0

// CMSIS Driver instances of Board peripherals
//TODO: #define CMSIS_DRIVER_ETH    0

// CMSIS Drivers
//TODO: extern ARM_DRIVER_ETH_MAC   ARM_Driver_ETH_MAC_(CMSIS_DRIVER_ETH);  /* Ethernet      */
//TODO: extern ARM_DRIVER_I2C       ARM_Driver_I2C_(ARDUINO_UNO_I2C);       /* Arduino I2C   */
//TODO: extern ARM_DRIVER_SPI       ARM_Driver_SPI_(ARDUINO_UNO_SPI);       /* Arduino SPI   */
//TODO: extern ARM_DRIVER_USART     ARM_Driver_USART_(ARDUINO_UNO_UART);    /* Arduino UART  */
//TODO: extern ARM_DRIVER_I2C       ARM_Driver_I2C_(MIKROBUS_I2C);          /* mikroBUS I2C  */
//TODO: extern ARM_DRIVER_SPI       ARM_Driver_SPI_(MIKROBUS_SPI);          /* mikroBUS SPI  */
//TODO: extern ARM_DRIVER_USART     ARM_Driver_USART_(MIKROBUS_UART);       /* mikroBUS UART */

#ifdef CMSIS_shield_header
#include CMSIS_shield_header
#endif

#endif /* FRDM_MCXN947_H_ */
