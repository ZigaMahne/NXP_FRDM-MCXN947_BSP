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
#include CMSIS_device_header

#include "main.h"

#include "cmsis_vio.h"
#include "fsl_mailbox.h"

#define CORE0_MAILBOX_ID    kMAILBOX_CM33_Core0

uint32_t core1_loop_counter = 0xFFFFFFFFU;

/*-----------------------------------------------------------------------------
 * Application main function
 *----------------------------------------------------------------------------*/
int app_main (void) {
  uint32_t n;

  /* Initialize Inter-CPU Mailbox */
  MAILBOX_Init(MAILBOX);

  /* Send the first mailbox message to the primary core */
  MAILBOX_SetValue(MAILBOX, CORE0_MAILBOX_ID, 0);

  /* Initialize loop counter */
  core1_loop_counter = 0;

  while (1) {
    /* Switch LED1 on */
    vioSetSignal(vioLED1, vioLEDon);
    /* Wait a bit */
    for(n=SystemCoreClock/20; n != 0; n--) { __NOP(); }

    /* Switch LED1 off */
    vioSetSignal(vioLED1, vioLEDoff);
    /* Wait a bit */
    for(n=SystemCoreClock/20; n != 0; n--) { __NOP(); }

    /* Increment loop counter */
    core1_loop_counter += 1;

    /* Send the loop counter value to the primary core */
    MAILBOX_SetValue(MAILBOX, CORE0_MAILBOX_ID, core1_loop_counter);
  }
}
