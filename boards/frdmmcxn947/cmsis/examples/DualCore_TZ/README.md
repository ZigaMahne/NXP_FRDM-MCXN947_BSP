# Dual Core Project with TrustZone

The **Dual Core TZ** project is an example that can be used to verify the
basic tool setup and shows how to utilizes TrustZone in dual core configuration.

It is compliant to the Cortex Microcontroller Software Interface Standard (CMSIS)
and uses the CMSIS-RTOS2 API interface for RTOS functionality. The CMSIS-RTOS2 API
is available with various real-time operating systems, for example RTX5 or FreeRTOS.

## Operation

**Core 0 secure - Cortex-M33:**
- Core 0 starts execution in secure mode.
- System initialization.
- Outputs "Hello World" to UART that is connected to MCU-Link (baudrate 115200bps).
- Release Core 1 from reset and provide it with a start address.
- Switch to the Core 0 non-secure mode,  involves setting up the non-secure vector table and stack pointer.

**Core 0 non-secure - Cortex-M33:**
- The vioLED0 blinks in 1 sec interval.
- The vioBUTTON0 changes the blink frequency.

**Core 1 - Cortex-M33:**
- Does not start automatically.
- Wait for Core 0 to perform system initialization.
- The vioLED1 blinks in 2 sec interval.
 
### CMSIS-Driver Virtual I/O mapping

| CMSIS-Driver VIO      | Board component
|:----------------------|:--------------------------------------
| vioBUTTON0            | WAKEUP button (SW2)
| vioLED0               | LED red       (RGB)
| vioLED1               | LED blue      (RGB)