# Board: NXP [FRDM-MCXN947](https://www.nxp.com/design/design-center/development-boards-and-designs/FRDM-MCXN947)

## Default Board Layer supporting USB Device

Device: **MCXN947VDF**

System Core Clock: **150 MHz**

This setup is configured using **MCUXpresso Config Tools**, an interactive tool provided by NXP for device configuration.
Refer to ["Configure NXP MCX Devices with MCUXpresso Config Tools"](https://open-cmsis-pack.github.io/cmsis-toolbox/MCUXpressoConfig) for additional information.

### System Configuration

| System resource       | Setting
|:----------------------|:--------------------------------------
| Heap                  | 64 kB (configured in the MCXN947_cm33_core0_*.scf file)
| Stack (MSP)           |  1 kB (configured in the MCXN947_cm33_core0_*.scf file)

### STDIO mapping

**STDIO** is routed to Virtual COM port on the MCU-Link (using **LP_FLEXCOMM4** peripheral)

### CMSIS-Driver mapping

| CMSIS-Driver          | Peripheral            | Board connector/component                       | Connection
|:----------------------|:----------------------|:------------------------------------------------|:------------------------------
| Driver_I2C1           | LP_FLEXCOMM2          | Arduino I2C pins D20..D21                       | ARDUINO_UNO_I2C
| Driver_SPI1           | LP_FLEXCOMM1          | Arduino SPI pins D10..D13                       | ARDUINO_UNO_SPI
| Driver_USART2         | LP_FLEXCOMM2          | Arduino UART pins D0..D1                        | ARDUINO_UNO_UART
| CMSIS-Driver VIO      | GPIO                  | RGB LED (Red, Blue) and USER buttons (SW2, SW3) | CMSIS_VIO

Reference to [Arduino UNO connector description](https://open-cmsis-pack.github.io/cmsis-toolbox/ReferenceApplications/#arduino-shield).

### CMSIS-Driver Virtual I/O mapping

| CMSIS-Driver VIO      | Board component
|:----------------------|:--------------------------------------
| vioBUTTON0            | USER button (SW2)
| vioLED0               | LED red     (RGB)
| vioLED1               | LED blue    (RGB)
