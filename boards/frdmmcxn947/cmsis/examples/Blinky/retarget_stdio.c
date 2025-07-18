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
 *
 *      Name:    retarget_stdio.c
 *      Purpose: Retarget stdio to LPUART
 *
 *---------------------------------------------------------------------------*/

#include <stdint.h>
#include <stdio.h>
#include "fsl_debug_console.h"          // NXP::Device:SDK Utilities:debug_console

/**
  Put a character to the stderr
 
  \param[in]   ch  Character to output
  \return          The character written, or -1 on write error.
*/
int stderr_putchar (int ch) {
  int32_t ret;

  ret = DbgConsole_Putchar(ch);

  if (ret != -1) {
    ret = ch;
  }
  return (ret);
}

/**
  Put a character to the stdout
 
  \param[in]   ch  Character to output
  \return          The character written, or -1 on write error.
*/
int stdout_putchar (int ch) {
  int32_t ret;

  ret = DbgConsole_Putchar(ch);

  if (ret != -1) {
    ret = ch;
  }
  return (ret);
}

/**
  Get a character from the stdio
 
  \return     The next character from the input, or -1 on read error.
*/
int stdin_getchar (void) {
  int32_t ret;

  ret = DbgConsole_Getchar();

  return (ret);
}
