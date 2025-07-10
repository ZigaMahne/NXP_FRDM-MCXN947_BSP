/*---------------------------------------------------------------------------
 * Copyright (c) 2025 Arm Limited (or its affiliates). All rights reserved.
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
#include "RTE_Components.h"
#include "cmsis_os2.h"
#include "cmsis_vio.h"
#include "fsl_mailbox.h"
#include "main.h"

/* Start address of the core1 application */
#define CORE1_BOOT_ADDRESS  0x000C0000

/* Primary core mailbox id */
#define CORE0_MAILBOX_ID    kMAILBOX_CM33_Core0

/* Number of while loops executed by the secondary core */
volatile uint32_t core1_loops = 0xFFFFFFFFU;

/* Thread attributes for the app_main thread */
static const osThreadAttr_t thread_attr_main = { .name = "app_main" };

/* Thread attributes for the LED thread */
static const osThreadAttr_t thread_attr_LED = { .name = "LED" };

/* Thread attributes for the Button thread */
static const osThreadAttr_t thread_attr_Button = { .name = "Button" };

/* Thread ID for the LED thread */
static osThreadId_t tid_LED;

/* Thread ID for the Button thread */
static osThreadId_t tid_Button;

/*
 * MAILBOX Interrupt Handler
*/
void MAILBOX_IRQHandler (void) {
  /* Get mailbox message from core1 for core0 */
  core1_loops = MAILBOX_GetValue(MAILBOX, CORE0_MAILBOX_ID);

  /* Clear interrupt bits */
  MAILBOX_ClearValueBits(MAILBOX, CORE0_MAILBOX_ID, 0xFFFFFFFF);
}

/*
 * Boot core1
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
  Thread that blinks LED.
*/
static __NO_RETURN void thread_LED (void *argument) {
  uint32_t active_flag = 0U;

  (void)argument;

  for (;;) {
    if (osThreadFlagsWait(1U, osFlagsWaitAny, 0U) == 1U) {
      active_flag ^= 1U;
    }

    if (active_flag == 1U) {
      vioSetSignal(vioLED0, vioLEDoff);         // Switch LED0 off
      osDelay(100U);                            // Delay 100 ms
      vioSetSignal(vioLED0, vioLEDon);          // Switch LED0 on
      osDelay(100U);                            // Delay 100 ms
    }
    else {
      vioSetSignal(vioLED0, vioLEDon);          // Switch LED0 on
      osDelay(500U);                            // Delay 500 ms
      vioSetSignal(vioLED0, vioLEDoff);         // Switch LED0 off
      osDelay(500U);                            // Delay 500 ms
    }
  }
}

/*
  Thread that checks Button state.
*/
static __NO_RETURN void thread_Button (void *argument) {
  uint32_t last = 0U;
  uint32_t state;

  (void)argument;

  for (;;) {
    state = (vioGetSignal(vioBUTTON0));         // Get pressed Button state
    if (state != last) {
      if (state == 1U) {
        osThreadFlagsSet(tid_LED, 1U);          // Set flag to thread_LED
      }
      last = state;
    }
    osDelay(100U);
  }
}

/*
  Application main thread.
*/
__NO_RETURN void app_main_thread (void *argument) {
  (void)argument;

  /* Initialize Inter-CPU Mailbox */
  MAILBOX_Init(MAILBOX);

  /* Enable mailbox interrupt */
  NVIC_EnableIRQ(MAILBOX_IRQn);

  /* Boot core1 */
  core1_enable(CORE1_BOOT_ADDRESS);

  printf("Blinky example\n");

  /* Create LED and Button threads */
  tid_LED = osThreadNew(thread_LED, NULL, &thread_attr_LED);
  tid_Button = osThreadNew(thread_Button, NULL, &thread_attr_Button);

  for (;;) {
    /* Delay indefinitely */
    osDelay(osWaitForever);
  }
}

/*
  Application initialization.
*/
int app_main (void) {
  osKernelInitialize();                         // Initialize CMSIS-RTOS2
  osThreadNew(app_main_thread, NULL, &thread_attr_main);
  osKernelStart();                              // Start thread execution
  return 0;
}
