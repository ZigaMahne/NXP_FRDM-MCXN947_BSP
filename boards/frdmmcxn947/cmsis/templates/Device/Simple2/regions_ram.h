#ifndef REGIONS_RAM_H
#define REGIONS_RAM_H

//-------- <<< Use Configuration Wizard in Context Menu >>> --------------------
//------ With VS Code: Open Preview for Configuration Wizard -------------------

// <i> Device Family Pack (DFP):   NXP::MCXN947_DFP@19.0.0

// <h> Core 0 Memory Configuration
// ===============================
//   <o> ROM Base Address <0x0-0xFFFFFFFF:8>
//   <i> Defines base address of memory region. Default: 0x00000000
//   <i> Contains Startup and Vector Table
#define __ROM0_BASE         0x04000000
//   <o> ROM Region Size [bytes] <0x0-0xFFFFFFFF:8>
//   <i> Defines size of memory region. Default: 0x00000000
#define __ROM0_SIZE         0x00018000

//   <o> RAM Base Address <0x0-0xFFFFFFFF:8>
//   <i> Defines base address of memory region. Default: 0x00000000
//   <i> Contains uninitialized RAM, Stack, and Heap
#define __RAM0_BASE         0x20000000
//   <o> RAM Region Size [bytes] <0x0-0xFFFFFFFF:8>
//   <i> Defines size of memory region. Default: 0x00000000
#define __RAM0_SIZE         0x0004E000

//   <o> Stack Size (in Bytes) <0x0-0xFFFFFFFF:8>
#define __STACK_SIZE        0x00000400

//   <o> Heap Size (in Bytes) <0x0-0xFFFFFFFF:8>
#define __HEAP_SIZE         0x00000C00
// </h>
// </h>

//-------- End of configuration section --------

#endif /* REGIONS_RAM_H */
