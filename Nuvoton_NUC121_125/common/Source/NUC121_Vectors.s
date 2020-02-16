/*****************************************************************************
 * THIS FILE IS PROVIDED AS IS WITH NO WARRANTY OF ANY KIND, INCLUDING THE   *
 * WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. *
 *****************************************************************************/

  .syntax unified
  .code 16

  .section .init, "ax"
  .align 0

#ifndef STARTUP_FROM_RESET
  .thumb_func
  .weak Reset_Wait
Reset_Wait:
  b .
#endif

  .thumb_func
  .weak NMI_Handler
NMI_Handler:
  b .

  .thumb_func
  .weak HardFault_Handler
HardFault_Handler:
  b .

  .thumb_func
  .weak SVC_Handler
SVC_Handler:
  b .

  .thumb_func
  .weak PendSV_Handler
PendSV_Handler:
  b .

  .thumb_func
  .weak SysTick_Handler
SysTick_Handler:
  b .

  .thumb_func
  .weak BOD_IRQHandler
BOD_IRQHandler:
  b .

  .thumb_func
  .weak WDT_IRQHandler
WDT_IRQHandler:
  b .

  .thumb_func
  .weak EINT024_IRQHandler
EINT024_IRQHandler:
  b .

  .thumb_func
  .weak EINT135_IRQHandler
EINT135_IRQHandler:
  b .

  .thumb_func
  .weak GPAB_IRQHandler
GPAB_IRQHandler:
  b .

  .thumb_func
  .weak GPCDEF_IRQHandler
GPCDEF_IRQHandler:
  b .

  .thumb_func
  .weak PWM0_IRQHandler
PWM0_IRQHandler:
  b .

  .thumb_func
  .weak PWM1_IRQHandler
PWM1_IRQHandler:
  b .

  .thumb_func
  .weak TMR0_IRQHandler
TMR0_IRQHandler:
  b .

  .thumb_func
  .weak TMR1_IRQHandler
TMR1_IRQHandler:
  b .

  .thumb_func
  .weak TMR2_IRQHandler
TMR2_IRQHandler:
  b .

  .thumb_func
  .weak TMR3_IRQHandler
TMR3_IRQHandler:
  b .

  .thumb_func
  .weak UART0_IRQHandler
UART0_IRQHandler:
  b .

  .thumb_func
  .weak SPI0_IRQHandler
SPI0_IRQHandler:
  b .

  .thumb_func
  .weak I2C0_IRQHandler
I2C0_IRQHandler:
  b .

  .thumb_func
  .weak I2C1_IRQHandler
I2C1_IRQHandler:
  b .

  .thumb_func
  .weak BPWM0_IRQHandler
BPWM0_IRQHandler:
  b .

  .thumb_func
  .weak BPWM1_IRQHandler
BPWM1_IRQHandler:
  b .

  .thumb_func
  .weak USCI_IRQHandler
USCI_IRQHandler:
  b .

  .thumb_func
  .weak USBD_IRQHandler
USBD_IRQHandler:
  b .

  .thumb_func
  .weak PWM_BRAKE_IRQHandler
PWM_BRAKE_IRQHandler:
  b .

  .thumb_func
  .weak PDMA_IRQHandler
PDMA_IRQHandler:
  b .

  .thumb_func
  .weak PWRWU_IRQHandler
PWRWU_IRQHandler:
  b .

  .thumb_func
  .weak ADC_IRQHandler
ADC_IRQHandler:
  b .

  .thumb_func
  .weak CLKDIRC_IRQHandler
CLKDIRC_IRQHandler:
  b .

  .thumb_func
  .weak Default_Handler
Default_Handler:
  b .

  .section .vectors, "ax"
  .align 0
  .global _vectors
  .extern __stack_end__
#ifdef STARTUP_FROM_RESET
  .extern Reset_Handler
#endif

_vectors:
  .word __stack_end__
#ifdef STARTUP_FROM_RESET
  .word Reset_Handler
#else
  .word Reset_Wait
#endif
  .word NMI_Handler
  .word HardFault_Handler
  .word 0 /* Reserved */
  .word 0 /* Reserved */
  .word 0 /* Reserved */
  .word 0 /* Reserved */
  .word 0 /* Reserved */
  .word 0 /* Reserved */
  .word 0 /* Reserved */
  .word SVC_Handler
  .word 0 /* Reserved */
  .word 0 /* Reserved */
  .word PendSV_Handler
  .word SysTick_Handler
  // External interrupts start here
  .word BOD_IRQHandler
  .word WDT_IRQHandler
  .word EINT024_IRQHandler
  .word EINT135_IRQHandler
  .word GPAB_IRQHandler
  .word GPCDEF_IRQHandler
  .word PWM0_IRQHandler
  .word PWM1_IRQHandler
  .word TMR0_IRQHandler
  .word TMR1_IRQHandler
  .word TMR2_IRQHandler
  .word TMR3_IRQHandler
  .word UART0_IRQHandler
  .word Default_Handler
  .word SPI0_IRQHandler
  .word Default_Handler
  .word Default_Handler
  .word Default_Handler
  .word I2C0_IRQHandler
  .word I2C1_IRQHandler
  .word BPWM0_IRQHandler
  .word BPWM1_IRQHandler
  .word USCI_IRQHandler
  .word USBD_IRQHandler
  .word Default_Handler
  .word PWM_BRAKE_IRQHandler
  .word PDMA_IRQHandler
  .word Default_Handler
  .word PWRWU_IRQHandler
  .word ADC_IRQHandler
  .word CLKDIRC_IRQHandler
  .word Default_Handler
_vectors_end:

#ifdef VECTORS_IN_RAM
  .section .vectors_ram, "ax"
  .align 0
  .global _vectors_ram

_vectors_ram:
  .space _vectors_end - _vectors, 0
#endif
