MCUmisfits
==========

## What is this?

[Rowley Crossworks for ARM](http://www.rowley.co.uk/arm/) and [SEGGER Embedded Studio](https://www.segger.com/products/development-tools/embedded-studio/) are lacking support for assorted microcontrollers.

This is a collection of unofficial CPU support packages for some such microcontrollers.

## Which microcontrollers does this project try to add support for?

[Nuvoton NUC121/125](./Nuvoton_NUC121_125/README.md)

[Nuvoton NUC126](./Nuvoton_NUC126/README.md)

[Nuvoton NUC505](./Nuvoton_NUC505/README.md)

[Cypress PSoC4](./Cypress_PSoC4/README.md)

[Synwit SWM](./Synwit_SWM/README.md)

[Microsemi SmartFusion2](./Microsemi_M2S/README.md)

## Installing a pre-built package (most likely option)

Most users will likely just want to download the Rowley .hzq / SEGGER .emPackage files from the Releases tab near the top of this page.

Installing a package manually in [Rowley Crossworks for ARM](http://www.rowley.co.uk/arm/) is straightforward: Choose Tools -> Packages -> Manually Install Packages.. from the pull-down menus, and pick your .hzq file of choice.

Installing a package manually in [SEGGER Embedded Studio](https://www.segger.com/products/development-tools/embedded-studio/) is  also straightforward: Choose Tools -> Manually Install Packages.. from the pull-down menus, and pick your .emPackage file of choice.

## Building from source code (not necessary)

The source code is also provided to show how custom CPU support packages can be done.  Note that whilst [Rowley Crossworks for ARM](http://www.rowley.co.uk/arm/) can generate packages for both tools, it appears that [SEGGER Embedded Studio](https://www.segger.com/products/development-tools/embedded-studio/) can only generate packages for itself.  When generating Rowley .hzp packages, be sure to compile the Loader in the "Release" configuration before building the package.

