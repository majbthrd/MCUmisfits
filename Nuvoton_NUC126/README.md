MCUmisfits: Nuvoton NUC126
==========================

The [Nuvoton NUC126](http://www.nuvoton.com/hq/products/microcontrollers/arm-cortex-m0-mcus/nuc126-usb-series/) is Cortex-M0 based USB microcontroller capable of crystal-less USB operation.

All devices in the family have 20kBytes of SRAM and either 128kB or 256kB of flash.  Additional flash includes 4kB of LDROM (memory region that can be mapped in at boot as a bootloader) and 2kB of SPROM that can be used for more secure storage.

This device family could be viewed as an alternative to the Atmel/Microchip SAMD21.

## Notes

This package was tested with both the SEGGER J-Link  and a CMSIS-DAP adapter.

If using SEGGER, the J-Link software must be updated to V6.40b (2019-1-22) or higher.  Earlier versions contained a buggy NUC126 flash driver.
