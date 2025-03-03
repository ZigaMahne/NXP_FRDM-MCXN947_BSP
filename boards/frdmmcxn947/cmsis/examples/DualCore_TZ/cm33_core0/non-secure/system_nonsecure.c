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

#include "system_MCXN947_cm33_core0.h"
#include "fsl_clock.h"

/*
  Ideally this or similar implementation goes into system_MCXN947_cm33_core0.c.
  At the moment we have it here to workaround the SystemCoreClock issue.
*/
#define CORE0_SYSCLK_FREQ 150000000

#if defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE != 3U)
/* ----------------------------------------------------------------------------
   -- SystemInit()
   ---------------------------------------------------------------------------- */
 void SystemInit (void) {
  SystemCoreClock = CORE0_SYSCLK_FREQ; /*CLOCK_GetCoreSysClkFreq();*/
 }

#if 0 // This would need weak implementation in system_MCXN947_cm33_core0.c
/* ----------------------------------------------------------------------------
   -- SystemCoreClockUpdate()
   ---------------------------------------------------------------------------- */
 void SystemCoreClockUpdate (void) {
  SystemCoreClock = = CORE0_SYSCLK_FREQ; /*CLOCK_GetCoreSysClkFreq();*/
 }
#endif
#endif
