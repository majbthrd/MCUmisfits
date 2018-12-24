MCUmisfits: Nuvoton NUC126
==========================

The [Nuvoton NUC126](http://www.nuvoton.com/hq/products/microcontrollers/arm-cortex-m0-mcus/nuc126-usb-series/) is Cortex-M0 based USB microcontroller capable of crystal-less USB operation.

All devices in the family have 20kBytes of SRAM and either 128kB or 256kB of flash.  Additional flash includes 4kB of LDROM (memory region that can be mapped in at boot as a bootloader) and 2kB of SPROM that can be used for more secure storage.

This device family could be viewed as an alternative to the Atmel/Microchip SAMD21.

## Notes

This package was tested with both the SEGGER J-Link  and a CMSIS-DAP adapter.

At the time of writing this, SEGGER J-Link software is at V6.40 (2018-10-26) and it contains a buggy NUC126 flash driver.

By default, Rowley Crossworks preferentially uses SEGGER's flash drivers over the ones contained in a CPU support package.  This is because someone at SEGGER "felt that a lot of times the flash loaders that come with a tool chain did not work reliably or not nearly as fast as our flash loaders".  Alas, SEGGER provides a flash driver for the NUC126 that just doesn't work.

To defeat this default behavior (to allow the *working* flash loader provided with this package) in Rowley Crossworks, right-click on SEGGER J-Link in Targets window, and select Properties.  In Properties, change the "Use Built-In Loader" to "No".

This is a nuisance, particularly since changing this setting overrides behavior for all targets rather than just one particular CPU.  I've submitted a bug report to SEGGER, and will revise this if and when they do something.
