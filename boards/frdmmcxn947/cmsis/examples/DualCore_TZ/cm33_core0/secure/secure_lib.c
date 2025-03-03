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

#include <arm_cmse.h>     // CMSE definitions
#include <stdint.h>
#include "secure_lib.h"   // Header file with secure interface API

#define NON_SECURE_CALLABLE_FUNC  __attribute__((cmse_nonsecure_entry))

/* Define a pointer to a function that will be executed in non-secure state */
typedef Callback_t __attribute__((cmse_nonsecure_call)) NS_Callback_t;


/* Function that executes in secure state and can be called from non-secure state. */
NON_SECURE_CALLABLE_FUNC int32_t SwitchDelay (uint32_t delay, Callback_t callback) {
	NS_Callback_t ns_callback;
  int32_t rval;
  uint32_t delay_new;

	/* Create function pointer for non-secure function that can be called from secure state */
  ns_callback = (NS_Callback_t)cmse_nsfptr_create(callback);

  if (callback == NULL) {
    return (0);
  }

  /* Delay is either 250ms or 500ms */
  if (delay >= 500U) { delay_new = 250U; }
  else               { delay_new = 500U; }

  /* Execute user provided function in the non-secure state */
  ns_callback (delay_new);

	return (1);
}