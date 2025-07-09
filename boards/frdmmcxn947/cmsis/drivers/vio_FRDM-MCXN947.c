/******************************************************************************
 * @file     vio_FRDM-MCXN947.c
 * @brief    Virtual I/O implementation for board FRDM-MCXN947
 * @version  V1.0.0
 * @date     9. July 2025
 ******************************************************************************/
/*
 * Copyright (c) 2025 Arm Limited (or its affiliates).
 * All rights reserved.
 *
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
 */

/*! \page vio_frdm_mcxn947 Physical I/O Mapping

The table below lists the physical I/O mapping of this CMSIS-Driver VIO implementation.

| Virtual I/O   | Variable       | Board component      | Pin
|:--------------|:---------------|:---------------------|:------
| vioBUTTON0    | vioSignalIn.0  | WAKEUP button (SW2)  | P0_23
| vioBUTTON1    | vioSignalIn.1  | ISP button    (SW3)  | P0_06
| vioLED0       | vioSignalOut.0 | LED red       (RGB)  | P0_10
| vioLED1       | vioSignalOut.1 | LED blue      (RGB)  | P1_02
*/

#include <string.h>
#include "cmsis_vio.h"

#include "RTE_Components.h"                     // Component selection
#include CMSIS_device_header

#if !defined CMSIS_VOUT || !defined CMSIS_VIN
#include "fsl_gpio.h"
#endif

// VIO input, output definitions
#ifndef VIO_VALUE_NUM
#define VIO_VALUE_NUM           5U              // Number of values
#endif

// VIO input, output variables
static uint32_t vioSignalIn             __USED; // Memory for incoming signal
static uint32_t vioSignalOut            __USED; // Memory for outgoing signal
static int32_t  vioValue[VIO_VALUE_NUM] __USED; // Memory for value used in vioGetValue/vioSetValue

#if !defined CMSIS_VOUT
#define LED_0_GPIO                      GPIO0   // BOARD_INITLEDSPINS_LED_RED_GPIO
#define LED_0_PIN                       10U     // BOARD_INITLEDSPINS_LED_RED_GPIO_PIN
#define LED_1_GPIO                      GPIO1   // BOARD_INITLEDSPINS_LED_BLUE_GPIO
#define LED_1_PIN                       2U      // BOARD_INITLEDSPINS_LED_BLUE_GPIO_PIN

#define LED_INIT_ON                     (0U)
#define LED_INIT_OFF                    (1U)

#define LED_INIT(GPIO, PIN, STATE)      do {                               \
                                          GPIO_PinWrite(GPIO, PIN, STATE); \
                                          GPIO->PDDR |= (1U << PIN);       \
                                        } while(0)
#define LED_ON(GPIO, PIN)               GPIO_PortClear(GPIO, 1U << PIN)
#define LED_OFF(GPIO, PIN)              GPIO_PortSet(GPIO, 1U << PIN)

#endif

#if !defined CMSIS_VIN
#define BUTTON_0_GPIO                   GPIO0   // BOARD_INITBUTTONSPINS_SW2_GPIO
#define BUTTON_0_PIN                    23U     // BOARD_INITBUTTONSPINS_SW2_GPIO_PIN
#define BUTTON_1_GPIO                   GPIO0   // BOARD_INITBUTTONSPINS_SW3_GPIO
#define BUTTON_1_PIN                    6U      // BOARD_INITBUTTONSPINS_SW3_GPIO_PIN
#endif

// Initialize test input, output.
void vioInit (void) {

  vioSignalIn  = 0U;
  vioSignalOut = 0U;

  memset(vioValue, 0, sizeof(vioValue));

#if !defined CMSIS_VOUT
  // Turn off all LEDs
  LED_INIT(LED_0_GPIO, LED_0_PIN, LED_INIT_OFF);
  LED_INIT(LED_1_GPIO, LED_1_PIN, LED_INIT_OFF);
#endif
}

// Set signal output.
void vioSetSignal (uint32_t mask, uint32_t signal) {

  vioSignalOut &= ~mask;
  vioSignalOut |=  mask & signal;

#if !defined CMSIS_VOUT
  // Output signals to LEDs
  if ((mask & vioLED0) != 0U) {
    if ((signal & vioLED0) != 0U) {
      LED_ON(LED_0_GPIO, LED_0_PIN);
    } else {
      LED_OFF(LED_0_GPIO, LED_0_PIN);
    }
  }

  if ((mask & vioLED1) != 0U) {
    if ((signal & vioLED1) != 0U) {
      LED_ON(LED_1_GPIO, LED_1_PIN);
    } else {
      LED_OFF(LED_1_GPIO, LED_1_PIN);
    }
  }
#endif
}

// Get signal input.
uint32_t vioGetSignal (uint32_t mask) {
  uint32_t signal;

#if !defined CMSIS_VIN
  // Get input signals from buttons
  if ((mask & vioBUTTON0) != 0U) {
    if (!GPIO_PinRead (BUTTON_0_GPIO, BUTTON_0_PIN)) {
      vioSignalIn |=  vioBUTTON0;
    } else {
      vioSignalIn &= ~vioBUTTON0;
    }
  }
  if ((mask & vioBUTTON1) != 0U) {
    if (!GPIO_PinRead (BUTTON_1_GPIO, BUTTON_1_PIN)) {
      vioSignalIn |=  vioBUTTON1;
    } else {
      vioSignalIn &= ~vioBUTTON1;
    }
  }
#endif

  signal = vioSignalIn & mask;

  return signal;
}

// Set value output.
//   Note: vioAOUT not supported.
void vioSetValue (uint32_t id, int32_t value) {
  uint32_t index = id;
#if !defined CMSIS_VOUT
// Add user variables here:

#endif

  if (index >= VIO_VALUE_NUM) {
    return;                             /* return in case of out-of-range index */
  }

  vioValue[index] = value;

#if !defined CMSIS_VOUT
// Add user code here:

#endif
}

// Get value input.
//   Note: vioAIN not supported.
int32_t vioGetValue (uint32_t id) {
  uint32_t index = id;
  int32_t  value;
#if !defined CMSIS_VIN
// Add user variables here:

#endif

  if (index >= VIO_VALUE_NUM) {
    return 0U;                          /* return 0 in case of out-of-range index */
  }

#if !defined CMSIS_VIN
// Add user code here:

#endif

  value = vioValue[index];

  return value;
}