MCUmisfits: Microsemi SmartFusion2
==================================

The [Actel, then Microsemi, now Microchip SmartFusion2](https://www.microsemi.com/product-directory/soc-fpgas/1692-smartfusion2) is a non-volatile FPGA with an embedded 166MHz ARM Cortex-M3.

All devices in the family have 32kBytes of SRAM; flash sizes are 128kB, 256kB, or 512kB depending on the model.  Part numbers begin with "M2S".

## Notes

I wrote this code several years ago, and re-reading my notes, the caveats for usage make it apparent why I punted on publishing the code at the time.  However, it still might be of use to you as it was for me; read the details below for specifics.

Rather than put both FPGA and ARM Cortex-M3 JTAG interfaces in the same boundary chain, Microsemi created custom logic to multiplex the JTAG pins between the two.  There is a JTAGSEL pin that, when driven to logic '0', selects the ARM.  In the JTAGSEL = '1' mode (sometimes the only available mode on certain boards), the FPGA is selected but the ARM can also be chosen by shifting in a particular JTAG command sequence.

Confusingly, there are at least four variants of FlashPro5 devices sold by Actel/Microsemi/Microchip.  Each has the same USB VID:PID, but all have different internal signal mappings that render them incompatible.  So, one variant might use one of the pins as an output enable to a driver buffer, but another might use that same pin in a different manner that might cause electrical contention or erroneous behavior.  One of those FlashPro5 variants is embedded in the inexpensive [SmartFusion2 Maker Board](https://www.digikey.com/en/product-highlight/m/microsemi-soc/smartfusion2-maker-board).

A SEGGER J-Link can only be used with a JTAGSEL = '0' configuration.  Even though the J-Link can be directed to issue the special bit sequence that works with JTAGSEL = '1', SEGGER's implementation insists on performing a JTAG test logic reset whenever connecting to the ARM target, and that action immediately reverses all that hard work.

This package was tested successfully with the following configurations:

|JTAGSEL|target         |debugger       |IDE                      |
|-------|---------------|---------------|-------------------------|
|0      |Emcraft SF2 kit|SEGGER J-Link  |Rowley Crossworks for ARM|
|0      |Emcraft SF2 kit|SEGGER J-Link  |SEGGER Embedded Studio   |
|0      |Emcraft SF2 kit|Amontec JTAGkey|Rowley Crossworks for ARM|
|1      |[SmartFusion2 Maker Board](https://www.digikey.com/en/product-highlight/m/microsemi-soc/smartfusion2-maker-board)|integrated Embedded FlashPro5|Rowley Crossworks for ARM|
|1      |Emcraft SF2 kit|Amontec JTAGkey|Rowley Crossworks for ARM|

To use the [SmartFusion2 Maker Board](https://www.digikey.com/en/product-highlight/m/microsemi-soc/smartfusion2-maker-board), you must use Rowley Crossworks for ARM; SEGGER Embedded Studio is incompatible with this target.  When first using the board, add a new target interface of type 'Generic FT2232 Device' and employ this configuration:

```
<GenericFT2232TargetInterface outputDirection="0x007B" fastMemoryAccessesEnabled="No" favorite="Yes" trstMask="0x0010" usbPid="0x2008" srstMask="0x0000" outputValue="0x0078" name="Microsemi Embedded FlashPro5" usbVid="0x1514" jtagDivider="5"/>
```

If using the [SmartFusion2 Maker Board](https://www.digikey.com/en/product-highlight/m/microsemi-soc/smartfusion2-maker-board) in Linux, you will also need a /etc/udev/rules.d file with the following content to allow Crossworks to access the JTAG interface:

```
# Microsemi Embedded FlashPro5
SUBSYSTEMS=="usb", ATTRS{idVendor}=="1514", ATTRS{idProduct}=="2008", MODE="0666"
```

In the more unusual scenario where the user has both Rowley Crossworks for ARM and SEGGER Embedded Studio installed on the same PC, at the time of writing there is a bug in SEGGER's implementation.  If the J-Link is not unplugged and then plugged back in before loading the IDE, SEGGER Embedded Studio will falsely claim that the user must purchase a higher cost J-Link with flash breakpoints.

