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

#include <stdio.h>
#include <stdint.h>
#include <arm_cmse.h>

#include "RTE_Components.h"
#include CMSIS_device_header

/* Start address of the non-secure application */
#ifndef NS_APP_START_ADDRESS
#define NS_APP_START_ADDRESS  (0x00020000)
#endif

/* Start address of the core1 application */
#ifndef CORE1_BOOT_ADDRESS
#define CORE1_BOOT_ADDRESS    (0x000C0000)
#endif

void core1_enable (uint32_t boot_address);

/* Define a pointer to a function that will be executed in non-secure state */
typedef void (*NS_Func_t) (void) __attribute__((cmse_nonsecure_call));

/*
  Boots secondary core.

  This function sets the secondary core boot address, enables clock to the core
  and releases secondary core from reset.
*/
void core1_enable (uint32_t boot_address) {
  uint32_t cpuctrl;

  /* Set CPU1 boot source */
  SYSCON->CPBOOT = (boot_address & SYSCON_CPBOOT_CPBOOT_MASK);

  /* Read out CPU Control register */
  cpuctrl = SYSCON->CPUCTRL;
  /* Set Write Protect mask, enable CPU1 clock but keep it under reset */
  cpuctrl |= (0xC0C4 << 16) | SYSCON_CPUCTRL_CPU1RSTEN_MASK | SYSCON_CPUCTRL_CPU1CLKEN_MASK;
  SYSCON->CPUCTRL = cpuctrl;

  /* Release CPU1 from reset */
  SYSCON->CPUCTRL = cpuctrl & ~SYSCON_CPUCTRL_CPU1RSTEN_MASK;
}

/*
  Switch to the non-secure state.

  Fist entry in the non-secure vector table is the Main Stack Pointer:
  *((uint32_t *)(NS_APP_START_ADDRESS)) == MSP_NS

  Second entry in the non-secure vector table is the Reset Handler:
  *((uint32_t *)(NS_APP_START_ADDRESS + 4U)) == Reset_Handler
*/
void nonsecure_app_start (uint32_t ns_app_start_addr) {
  uint32_t  Stack_NS;
  NS_Func_t ResetHandler_NS;

  /* Read the address of the main stack in non-secure state */
  Stack_NS = *((volatile uint32_t *)(NS_APP_START_ADDRESS));

  /* Set Main Stack Pointer for the non-secure side (MSP_NS) */
  __TZ_set_MSP_NS(Stack_NS);

  /* Set address of the non-secure Reset Handler */
  ResetHandler_NS = (NS_Func_t)(*((uint32_t *)(NS_APP_START_ADDRESS + 4U)));

  /* Call non-secure Reset Handler and start executing non-secure application */
  ResetHandler_NS();
}

/*
  Application main function.
*/
int app_main (void) {

  printf("Hello World!\n");

  /* Boot core1 */
  core1_enable(CORE1_BOOT_ADDRESS);

  /* Switch to the non-secure application */
  nonsecure_app_start(NS_APP_START_ADDRESS);

  /* Non-secure application does not return */
  for (;;){;}
}
