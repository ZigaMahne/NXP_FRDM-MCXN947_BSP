/*---------------------------------------------------------------------------
 * Copyright (c) 2024 Arm Limited (or its affiliates). All rights reserved.
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
 *---------------------------------------------------------------------------*/

#include "RTE_Components.h"
#include  CMSIS_device_header
#include "cmsis_vio.h"
#ifdef    CMSIS_shield_header
#include  CMSIS_shield_header
#endif

#include "clock_config.h"
#include "peripherals.h"
#include "pin_mux.h"

#include "main.h"

#ifdef CMSIS_shield_header
__WEAK int32_t shield_setup (void) {
  return 0;
}
#endif

int main (void) {

  /* System initialization */
  BOARD_InitBootPeripherals();
  BOARD_InitBootClocks();
  BOARD_InitBootPins();

  NVIC_SetPriority(ETHERNET_IRQn,     8U);
  NVIC_SetPriority(LP_FLEXCOMM4_IRQn, 8U);

  /* Initialize Virtual I/O */
  vioInit();

#ifdef CMSIS_shield_header
  shield_setup();
#endif

  return (app_main());
}
