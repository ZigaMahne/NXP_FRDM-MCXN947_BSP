# Dual Core Project

The **Dual Core** project is an example that can be used to verify the
basic tool setup and shows how to configure dual core application.

It is compliant to the Cortex Microcontroller Software Interface Standard (CMSIS)
and uses the CMSIS-RTOS2 API interface for RTOS functionality. The CMSIS-RTOS2 API
is available with various real-time operating systems, for example RTX5 or FreeRTOS.

## Operation

**Core 0 - Cortex-M33:**
- System initialization.
- Mailbox module initialization.
- Release Core 1 from reset and provide it with a start address.
- Outputs "Blinky Example" to UART that is connected to MCU-Link (baudrate 115200bps).
- The vioLED0 blinks in 1 sec interval.
- The vioBUTTON0 changes the blink frequency.

**Core 1 - Cortex-M33:**
- Does not start automatically.
- Wait for Core 0 to perform system initialization.
- Mailbox module initialization.
- The vioLED1 blinks in 2 sec interval.
 
### CMSIS-Driver Virtual I/O mapping

| CMSIS-Driver VIO      | Board component
|:----------------------|:--------------------------------------
| vioBUTTON0            | WAKEUP button (SW2)
| vioLED0               | LED red       (RGB)
| vioLED1               | LED blue      (RGB)
