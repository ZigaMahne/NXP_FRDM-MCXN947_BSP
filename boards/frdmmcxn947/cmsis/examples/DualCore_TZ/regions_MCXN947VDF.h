#ifndef REGIONS_MCXN947VDF_H
#define REGIONS_MCXN947VDF_H

//-------- <<< Use Configuration Wizard in Context Menu >>> --------------------
//------ With VS Code: Open Preview for Configuration Wizard -------------------

// <i> Device Family Pack (DFP):   NXP::MCXN947_DFP@19.0.0
// <i> Board Support Pack (BSP):   NXP::FRDM-MCXN947_BSP@19.1.0-dev19

// <h> Core 0 Memory Configuration
// ===============================
// <h> Secure State
// ================
//   <o> ROM Base Address <0x0-0xFFFFFFFF:8>
//   <i> Defines base address of memory region. Default: 0x00000000
//   <i> Contains Startup and Vector Table
#define CORE0_S_ROM_BASE    0x10000000
//   <o> ROM Region Size [bytes] <0x0-0xFFFFFFFF:8>
//   <i> Defines size of memory region. Default: 0x00000000
#define CORE0_S_ROM_SIZE    0x0001FE00

//   <o> Veneers Base Address <0x0-0xFFFFFFFF:8>
//   <i> Defines base address of memory region. Default: 0x1001FE00
#define CORE0_S_VENEER_BASE (CORE0_S_ROM_BASE + CORE0_S_ROM_SIZE)
//   <o> Veneers Region Size [bytes] <0x0-0xFFFFFFFF:8>
//   <i> Defines size of memory region. Default: 0x00000000
#define CORE0_S_VENEER_SIZE 0x00000200

//   <o> RAM Base Address <0x0-0xFFFFFFFF:8>
//   <i> Defines base address of memory region. Default: 0x00000000
//   <i> Contains uninitialized RAM, Stack, and Heap
#define CORE0_S_RAM_BASE    0x30000000
//   <o> RAM Region Size [bytes] <0x0-0xFFFFFFFF:8>
//   <i> Defines size of memory region. Default: 0x00000000
#define CORE0_S_RAM_SIZE    0x00008000

//   <o> Stack Size (in Bytes) <0x0-0xFFFFFFFF:8>
#define CORE0_S_STACK_SIZE  0x00000400

//   <o> Heap Size (in Bytes) <0x0-0xFFFFFFFF:8>
#define CORE0_S_HEAP_SIZE   0x00000400
// </h>

// <h> Non-secure State
// ====================
//   <o> ROM Base Address <0x0-0xFFFFFFFF:8>
//   <i> Defines base address of memory region. Default: 0x00000000
//   <i> Contains Startup and Vector Table
#define CORE0_NS_ROM_BASE   0x00020000
//   <o> ROM Region Size [bytes] <0x0-0xFFFFFFFF:8>
//   <i> Defines size of memory region. Default: 0x00000000
#define CORE0_NS_ROM_SIZE   0x000A0000

//   <o> RAM Base Address <0x0-0xFFFFFFFF:8>
//   <i> Defines base address of memory region. Default: 0x00000000
//   <i> Contains uninitialized RAM, Stack, and Heap
#define CORE0_NS_RAM_BASE   0x20008000
//   <o> RAM Region Size [bytes] <0x0-0xFFFFFFFF:8>
//   <i> Defines size of memory region. Default: 0x00000000
#define CORE0_NS_RAM_SIZE   0x00046000

//   <o> Stack Size (in Bytes) <0x0-0xFFFFFFFF:8>
#define CORE0_NS_STACK_SIZE 0x00000400

//   <o> Heap Size (in Bytes) <0x0-0xFFFFFFFF:8>
#define CORE0_NS_HEAP_SIZE  0x00010000
// </h>
// </h>

// <h> Core 1 Memory Configuration
// ===============================
//   <o> ROM Base Address <0x0-0xFFFFFFFF:8>
//   <i> Defines base address of memory region. Default: 0x00000000
//   <i> Contains Startup and Vector Table
#define CORE1_ROM_BASE      0x000C0000
//   <o> ROM Region Size [bytes] <0x0-0xFFFFFFFF:8>
//   <i> Defines size of memory region. Default: 0x00200000
#define CORE1_ROM_SIZE      0x00040000

//   <o> RAM Base Address <0x0-0xFFFFFFFF:8>
//   <i> Defines base address of memory region. Default: 0x20000000
//   <i> Contains uninitialized RAM, Stack, and Heap
#define CORE1_RAM_BASE      0x2004E000
//   <o> RAM Region Size [bytes] <0x0-0xFFFFFFFF:8>
//   <i> Defines size of memory region. Default: 0x00068000
#define CORE1_RAM_SIZE      0x0001A000

//   <o> Stack Size (in Bytes) <0x0-0xFFFFFFFF:8>
#define CORE1_STACK_SIZE    0x00000400

//   <o> Heap Size (in Bytes) <0x0-0xFFFFFFFF:8>
#define CORE1_HEAP_SIZE     0x00000400
// </h>

//-------- End of configuration section --------

// Map defines to match linker script expectations
#ifdef CPU_MCXN947VDF_cm33_core0
#if defined(__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
#define __ROM0_BASE     CORE0_S_ROM_BASE
#define __ROM0_SIZE     CORE0_S_ROM_SIZE
#define __RAM0_BASE     CORE0_S_RAM_BASE
#define __RAM0_SIZE     CORE0_S_RAM_SIZE
#define __STACK_SIZE    CORE0_S_STACK_SIZE
#define __HEAP_SIZE     CORE0_S_HEAP_SIZE
#define __VENEER_BASE   CORE0_S_VENEER_BASE
#define __VENEER_SIZE   CORE0_S_VENEER_SIZE
#else
#define __ROM0_BASE     CORE0_NS_ROM_BASE
#define __ROM0_SIZE     CORE0_NS_ROM_SIZE
#define __RAM0_BASE     CORE0_NS_RAM_BASE
#define __RAM0_SIZE     CORE0_NS_RAM_SIZE
#define __STACK_SIZE    CORE0_NS_STACK_SIZE
#define __HEAP_SIZE     CORE0_NS_HEAP_SIZE
#endif
#endif

#ifdef CPU_MCXN947VDF_cm33_core1
#define __ROM0_BASE     CORE1_ROM_BASE
#define __ROM0_SIZE     CORE1_ROM_SIZE
#define __RAM0_BASE     CORE1_RAM_BASE
#define __RAM0_SIZE     CORE1_RAM_SIZE
#define __STACK_SIZE    CORE1_STACK_SIZE
#define __HEAP_SIZE     CORE1_HEAP_SIZE
#endif

#endif /* REGIONS_MCXN947VDF_H */
