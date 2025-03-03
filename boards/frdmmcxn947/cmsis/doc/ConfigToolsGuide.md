# Configure NXP MCX Devices with MCUXpresso Config Tools

This document explains how to use [MCUXpresso Config Tools](https://www.nxp.com/configtools) with the CMSIS-Toolbox to manage device and board configuration.

> **Note:**
>
> - MCUXpresso Config Tools integration with CMSIS-Toolbox is available with **CMSIS packs based on MCUXpresso SDK 2.16.0** (or higher).

**Document Content:**

- [Configure MCX Devices with MCUXpresso Config Tools](#configure-mcx-devices-with-mcuxpresso-config-tools)
  - [Overview](#overview)
  - [Simple Project](#simple-project)

## Overview

The [MCUXpresso Config Tools](https://www.nxp.com/configtools) is an integrated suite of configuration tools that allow developers to quickly configure pins, clocks, peripherals and TrustZone and generate initialization C code or register values for custom board support. The CMSIS-Toolbox interacts with MCUXpresso Config Tools using the [generic interface for generators]([build-operation.md#generator-integration](https://github.com/Open-CMSIS-Pack/cmsis-toolbox/blob/main/docs/build-operation.md)) and automatically adds generated source code to the project.

The [MCUXpresso Config Tools](https://www.nxp.com/configtools) is connected to a *csolution project* via `component: Device:Config Tools:Init`. This component imports the MCUXpresso Config Tools generated files for selected `device:` or `board:` using the [generator import file](https://github.com/Open-CMSIS-Pack/cmsis-toolbox/blob/main/docs/YML-CBuild-Format.md#generator-import-file) (`*.cgen.yml`). The `*.cgen.yml` file is managed by MCUXpresso Config Tools and should not be modified directly.

## Simple Project

Below is a simple project that just adds the Config Tools generated components.

>**Note:**  
>
> The packs required for boards and devices are listed on [www.keil.arm.com/boards](https://www.keil.arm.com/boards/) and [www.keil.arm.com/devices](https://www.keil.arm.com/devices/).

**File: `Simple.csolution.yml`**

```yml
solution:
  created-for: CMSIS-Toolbox@2.6.0
  description: Configure device using MCUXpresso Config Tools

  # List of tested compilers that can be selected
  select-compiler:
    - compiler: AC6
    - compiler: GCC
    - compiler: IAR

  compiler: GCC                               # Selected compiler

  # List the pack or packs that define the device or board used in the csolution.yml file
  packs:
    - pack: ARM::CMSIS                        # CMSIS pack is required for most projects
    - pack: NXP::FRDM-MCXN947_BSP@19.0.0      # Board support pack for FRDM-MCXN947
    - pack: NXP::MCXN947_DFP@19.0.0           # Device support pack for MCXN947 devices
    
  target-types:
    - type: MyBoard                           # My evaluation kit
      board: NXP::FRDM-MCXN947                # Board name as defined by the pack

  build-types:                                # Defines toolchain options for 'debug' and 'release'
    - type: Debug
      debug: on
      optimize: none

    - type: Release
      debug: off
      optimize: balanced

  projects:                                   # List related projects
    - project: ./Simple.cproject.yml
```

**File: `Simple.cproject.yml`**

```yml
project:
  components:
    - component: Device:Config Tools:Init     # Component that connects to MCUXpresso Config Tools

    - component: Device:SDK Drivers:clock     # Following components resolve generated code dependencies
    - component: Device:SDK Drivers:common
    - component: Device:SDK Drivers:mcx_spc
    - component: Device:SDK Drivers:reset

    - component: Device:Startup               # Following components add device startup code
    - component: Device:CMSIS:MCXN947_header
    - component: Device:CMSIS:MCXN947_system

    - component: CMSIS:CORE                   # CMSIS-Core component provides Cortex core definitions

  groups:
    - group: Main
      files:
        - file: ./main.c                      # Implements main function
```

Such project cannot be directly compiled as initially the `*.cgen.yml` file is missing. It requires to run the MCUXpresso Config Tools generator.

> **NOTE:** Before you start, you may need to [`install missing packs`](build-tools.md#install-missing-packs).

To build this simple project using CMSIS-Toolbox from the command line, follow the steps below:

1. Identify the required generator and the directory where the generated files are stored:

   ```bash
   >csolution Simple.csolution.yml list generators --verbose
    MCUXpressoConfig (Global Registered Generator MCUXpresso Config tools) # generator name
      base-dir: MCUXpressoConfig/MCXN947VDF                                # directory for generated files
        cgen-file: MCUXpressoConfig/MCXN947VDF/Simple.cgen.yml             # generator import file
          context: Simple.Debug+MCXN947VDF                                 # list of context that uses this directory
          context: Simple.Release+MCXN947VDF
   ```

2. Run the generator using the above information with this command:

   ```bash
   >csolution Simple.csolution.yml run --generator MCUXpressoConfig --context Simple.Debug+MCXN947VDF
   ```

   It starts the MCUXpresso Config Tools and passes the information about the selected board, device and selected toolchain. For a project that selects a board, MCUXpresso Config Tools import the default configuration for the evaluation kit. In Config Tools review and adjust configuration options as required for your application, then just click the button `Update Code`. The generated files will be stored in the directory `./MCUXpressoConfig/MCXN947VDF`.

3. Add main function implementation and build the project using this command:

   ```bash
   >cbuild ConfigTools.csolution.yml --update-rte
   ```

>**Note:**
>
> You may run the MCUXpresso Config Tools generator any time to change the configuration setting of your device or board.

**Directory with generated files**

MCUXpresso Config Tools generates the following content in the generator output directory of the *csolution project*. In our example the generator output directory is `./MCUXpressoConfig/MCXN947VDF`.

Directory `.../MCXN947VDF` | Content
:--------------------------|:---------------
`Simple.cgen.yml`          | Generator import file that adds the Config Tools generated files to the *csolution project*.
`Simple.mex`               | MCUXpresso Config Tools project file with settings.
`board/pin_mux.c`          | Source file with pin configuration functions.
`board/pin_mux.h`          | Header file with pin configuration prototypes.
`board/clock_config.c`     | Source file with clock configuration functions.
`board/clock_config.h`     | Header file with clock configuration prototypes.
`board/peripherals.c`      | Source file with peripheral configuration functions.
`board/peripherals.h`      | Header file with peripheral configuration prototypes.
`trustzone/tzm_config.c`   | Source file with TrustZone configuration functions.
`trustzone/tzm_config.h`   | Header file with TrustZone configuration prototypes.

**Content of Generator Import File: `Simple.cgen.yml`**

The file `Simple.cgen.yml` lists the files that are generated by MCUXpresso Config Tools and imported into the *csolution project*. It is managed by MCUXpresso Config Tools and should not be modified directly - its content is shown for reference only:

```yml
generator-import:
  generated-by: 'MCUXpresso config tools'
  for-device: MCXN947

  groups:
  - group: board
    files:
    - file: board/clock_config.c
    - file: board/clock_config.h
    - file: board/peripherals.c
    - file: board/peripherals.h
    - file: board/pin_mux.c
    - file: board/pin_mux.h
    - file: trustzone/tzm_config.c
    - file: trustzone/tzm_config.h
```
