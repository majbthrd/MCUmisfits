MCUmisfits: Nuvoton NUC505
==========================

The [Nuvoton NUC505](http://www.nuvoton.com/hq/products/microcontrollers/arm-cortex-m4-mcus/nuc505-series/) is an aggressively priced ARM Cortex-M4F (floating-point) device that provides USB 2.0 high-speed device functionality (as well as a bevy of other peripherals).

All devices in the family have 128kBytes of single cycle SRAM that is used for both code and data.  Integrated into the device is either a 512kB or 2048kB SPI flash, and its contents are memory-mapped into the ARM's address space.  An 8kByte mask ROM provides some semblance of a bootloader.

Although code can run directly out of the SPI flash, execution is understandably SLOOOW.  For this reason, the default Section Placement setting provided for new projects by this package is "Flash Copy to RAM"; however, the usual "Flash" and "RAM" options are provided for completeness as well.

The NUC505 has a "Boot from USB" mode where the device appears as a USB Mass Storage device.  A .bin binary image copied to the device is programmed to the internal SPI memory at 0x0.  When the NUC505 is then returned to the "Boot from Internal MCP SPI flash" and restarted, it will attempt to boot from that image.  HOWEVER, the "Boot from USB" USB Mass Storage implementation appears to only work in Windows; Linux detects the emulated volume as having corruption and will refuse to mount it for write access.

When the NUC505 is set to its "SWD/ICE Mode with Internal SPI Flash" mode, the NUC505 will NOT execute user code upon hardware reset.  Instead, it runs from its internal mask ROM code and execution eventually reaches an endless loop.  This boot mode seems to be intended exclusively for using an IDE to both program memory and specifically initiate debugging execution of the user application.

The NuTiny-NUC505Y evaluation board has an integrated debugger, but it is a proprietary interface; this necessitates an external debugger (Segger J-Link and CMSIS-DAP tested).  Here are some words of caution on the evaluation board:

- do not trust the top side silkscreen polarity markings; only the additional bottom-side silkscreen markings seem to be mostly correct
- the voltage reference rail is marked "VCC33", but is 5.0 volts (be certain your debugger is compatible with this voltage)

This package should already be configured to utilize the libraries in Rowley's CMSIS-DSP package (as the Cortex-M4F is quite capable in this area).
