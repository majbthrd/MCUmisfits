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
  .weak PWR_IRQHandler
PWR_IRQHandler:
  b .

  .thumb_func
  .weak WDT_IRQHandler
WDT_IRQHandler:
  b .

  .thumb_func
  .weak APU_IRQHandler
APU_IRQHandler:
  b .

  .thumb_func
  .weak I2S_IRQHandler
I2S_IRQHandler:
  b .

  .thumb_func
  .weak EINT0_IRQHandler
EINT0_IRQHandler:
  b .

  .thumb_func
  .weak EINT1_IRQHandler
EINT1_IRQHandler:
  b .

  .thumb_func
  .weak EINT2_IRQHandler
EINT2_IRQHandler:
  b .

  .thumb_func
  .weak EINT3_IRQHandler
EINT3_IRQHandler:
  b .

  .thumb_func
  .weak SPIM_IRQHandler
SPIM_IRQHandler:
  b .

  .thumb_func
  .weak USBD_IRQHandler
USBD_IRQHandler:
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
  .weak SDH_IRQHandler
SDH_IRQHandler:
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
  .weak PWM2_IRQHandler
PWM2_IRQHandler:
  b .

  .thumb_func
  .weak PWM3_IRQHandler
PWM3_IRQHandler:
  b .

  .thumb_func
  .weak RTC_IRQHandler
RTC_IRQHandler:
  b .

  .thumb_func
  .weak SPI0_IRQHandler
SPI0_IRQHandler:
  b .

  .thumb_func
  .weak I2C1_IRQHandler
I2C1_IRQHandler:
  b .

  .thumb_func
  .weak I2C0_IRQHandler
I2C0_IRQHandler:
  b .

  .thumb_func
  .weak UART0_IRQHandler
UART0_IRQHandler:
  b .

  .thumb_func
  .weak UART1_IRQHandler
UART1_IRQHandler:
  b .

  .thumb_func
  .weak ADC_IRQHandler
ADC_IRQHandler:
  b .

  .thumb_func
  .weak WWDT_IRQHandler
WWDT_IRQHandler:
  b .

  .thumb_func
  .weak USBH_IRQHandler
USBH_IRQHandler:
  b .

  .thumb_func
  .weak UART2_IRQHandler
UART2_IRQHandler:
  b .

  .thumb_func
  .weak LVD_IRQHandler
LVD_IRQHandler:
  b .

  .thumb_func
  .weak SPI1_IRQHandler
SPI1_IRQHandler:
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
  .word PWR_IRQHandler
  .word WDT_IRQHandler
  .word APU_IRQHandler
  .word I2S_IRQHandler
  .word EINT0_IRQHandler
  .word EINT1_IRQHandler
  .word EINT2_IRQHandler
  .word EINT3_IRQHandler
  .word SPIM_IRQHandler
  .word USBD_IRQHandler
  .word TMR0_IRQHandler
  .word TMR1_IRQHandler
  .word TMR2_IRQHandler
  .word TMR3_IRQHandler
  .word SDH_IRQHandler
  .word PWM0_IRQHandler
  .word PWM1_IRQHandler
  .word PWM2_IRQHandler
  .word PWM3_IRQHandler
  .word RTC_IRQHandler
  .word SPI0_IRQHandler
  .word I2C1_IRQHandler
  .word I2C0_IRQHandler
  .word UART0_IRQHandler
  .word UART1_IRQHandler
  .word ADC_IRQHandler
  .word WWDT_IRQHandler
  .word USBH_IRQHandler
  .word UART2_IRQHandler
  .word LVD_IRQHandler
  .word SPI1_IRQHandler
_vectors_end:

#ifdef VECTORS_IN_RAM
  .section .vectors_ram, "ax"
  .align 0
  .global _vectors_ram

_vectors_ram:
  .space _vectors_end - _vectors, 0
#endif
