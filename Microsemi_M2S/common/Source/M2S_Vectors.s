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
  .weak MemManage_Handler
MemManage_Handler:
  b .

  .thumb_func
  .weak BusFault_Handler
BusFault_Handler:
  b .

  .thumb_func
  .weak UsageFault_Handler
UsageFault_Handler:
  b .

  .thumb_func
  .weak SVC_Handler
SVC_Handler:
  b .

  .thumb_func
  .weak DebugMon_Handler
DebugMon_Handler:
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
  .weak WdogWakeup_IRQHandler
WdogWakeup_IRQHandler:
  b .

  .thumb_func
  .weak RTC_Wakeup_IRQHandler
RTC_Wakeup_IRQHandler:
  b .

  .thumb_func
  .weak SPI0_IRQHandler
SPI0_IRQHandler:
  b .

  .thumb_func
  .weak SPI1_IRQHandler
SPI1_IRQHandler:
  b .

  .thumb_func
  .weak I2C0_IRQHandler
I2C0_IRQHandler:
  b .

  .thumb_func
  .weak I2C0_SMBAlert_IRQHandler
I2C0_SMBAlert_IRQHandler:
  b .

  .thumb_func
  .weak I2C0_SMBus_IRQHandler
I2C0_SMBus_IRQHandler:
  b .

  .thumb_func
  .weak I2C1_IRQHandler
I2C1_IRQHandler:
  b .

  .thumb_func
  .weak I2C1_SMBAlert_IRQHandler
I2C1_SMBAlert_IRQHandler:
  b .

  .thumb_func
  .weak I2C1_SMBus_IRQHandler
I2C1_SMBus_IRQHandler:
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
  .weak EthernetMAC_IRQHandler
EthernetMAC_IRQHandler:
  b .

  .thumb_func
  .weak DMA_IRQHandler
DMA_IRQHandler:
  b .

  .thumb_func
  .weak Timer1_IRQHandler
Timer1_IRQHandler:
  b .

  .thumb_func
  .weak Timer2_IRQHandler
Timer2_IRQHandler:
  b .

  .thumb_func
  .weak CAN_IRQHandler
CAN_IRQHandler:
  b .

  .thumb_func
  .weak ENVM0_IRQHandler
ENVM0_IRQHandler:
  b .

  .thumb_func
  .weak ENVM1_IRQHandler
ENVM1_IRQHandler:
  b .

  .thumb_func
  .weak ComBlk_IRQHandler
ComBlk_IRQHandler:
  b .

  .thumb_func
  .weak USB_IRQHandler
USB_IRQHandler:
  b .

  .thumb_func
  .weak USB_DMA_IRQHandler
USB_DMA_IRQHandler:
  b .

  .thumb_func
  .weak PLL_Lock_IRQHandler
PLL_Lock_IRQHandler:
  b .

  .thumb_func
  .weak PLL_LockLost_IRQHandler
PLL_LockLost_IRQHandler:
  b .

  .thumb_func
  .weak CommSwitchError_IRQHandler
CommSwitchError_IRQHandler:
  b .

  .thumb_func
  .weak CacheError_IRQHandler
CacheError_IRQHandler:
  b .

  .thumb_func
  .weak DDR_IRQHandler
DDR_IRQHandler:
  b .

  .thumb_func
  .weak HPDMA_Complete_IRQHandler
HPDMA_Complete_IRQHandler:
  b .

  .thumb_func
  .weak HPDMA_Error_IRQHandler
HPDMA_Error_IRQHandler:
  b .

  .thumb_func
  .weak ECC_Error_IRQHandler
ECC_Error_IRQHandler:
  b .

  .thumb_func
  .weak MDDR_IOCalib_IRQHandler
MDDR_IOCalib_IRQHandler:
  b .

  .thumb_func
  .weak FAB_PLL_Lock_IRQHandler
FAB_PLL_Lock_IRQHandler:
  b .

  .thumb_func
  .weak FAB_PLL_LockLost_IRQHandler
FAB_PLL_LockLost_IRQHandler:
  b .

  .thumb_func
  .weak FIC64_IRQHandler
FIC64_IRQHandler:
  b .

  .thumb_func
  .weak FabricIrq0_IRQHandler
FabricIrq0_IRQHandler:
  b .

  .thumb_func
  .weak FabricIrq1_IRQHandler
FabricIrq1_IRQHandler:
  b .

  .thumb_func
  .weak FabricIrq2_IRQHandler
FabricIrq2_IRQHandler:
  b .

  .thumb_func
  .weak FabricIrq3_IRQHandler
FabricIrq3_IRQHandler:
  b .

  .thumb_func
  .weak FabricIrq4_IRQHandler
FabricIrq4_IRQHandler:
  b .

  .thumb_func
  .weak FabricIrq5_IRQHandler
FabricIrq5_IRQHandler:
  b .

  .thumb_func
  .weak FabricIrq6_IRQHandler
FabricIrq6_IRQHandler:
  b .

  .thumb_func
  .weak FabricIrq7_IRQHandler
FabricIrq7_IRQHandler:
  b .

  .thumb_func
  .weak FabricIrq8_IRQHandler
FabricIrq8_IRQHandler:
  b .

  .thumb_func
  .weak FabricIrq9_IRQHandler
FabricIrq9_IRQHandler:
  b .

  .thumb_func
  .weak FabricIrq10_IRQHandler
FabricIrq10_IRQHandler:
  b .

  .thumb_func
  .weak FabricIrq11_IRQHandler
FabricIrq11_IRQHandler:
  b .

  .thumb_func
  .weak FabricIrq12_IRQHandler
FabricIrq12_IRQHandler:
  b .

  .thumb_func
  .weak FabricIrq13_IRQHandler
FabricIrq13_IRQHandler:
  b .

  .thumb_func
  .weak FabricIrq14_IRQHandler
FabricIrq14_IRQHandler:
  b .

  .thumb_func
  .weak FabricIrq15_IRQHandler
FabricIrq15_IRQHandler:
  b .

  .thumb_func
  .weak GPIO0_IRQHandler
GPIO0_IRQHandler:
  b .

  .thumb_func
  .weak GPIO1_IRQHandler
GPIO1_IRQHandler:
  b .

  .thumb_func
  .weak GPIO2_IRQHandler
GPIO2_IRQHandler:
  b .

  .thumb_func
  .weak GPIO3_IRQHandler
GPIO3_IRQHandler:
  b .

  .thumb_func
  .weak GPIO4_IRQHandler
GPIO4_IRQHandler:
  b .

  .thumb_func
  .weak GPIO5_IRQHandler
GPIO5_IRQHandler:
  b .

  .thumb_func
  .weak GPIO6_IRQHandler
GPIO6_IRQHandler:
  b .

  .thumb_func
  .weak GPIO7_IRQHandler
GPIO7_IRQHandler:
  b .

  .thumb_func
  .weak GPIO8_IRQHandler
GPIO8_IRQHandler:
  b .

  .thumb_func
  .weak GPIO9_IRQHandler
GPIO9_IRQHandler:
  b .

  .thumb_func
  .weak GPIO10_IRQHandler
GPIO10_IRQHandler:
  b .

  .thumb_func
  .weak GPIO11_IRQHandler
GPIO11_IRQHandler:
  b .

  .thumb_func
  .weak GPIO12_IRQHandler
GPIO12_IRQHandler:
  b .

  .thumb_func
  .weak GPIO13_IRQHandler
GPIO13_IRQHandler:
  b .

  .thumb_func
  .weak GPIO14_IRQHandler
GPIO14_IRQHandler:
  b .

  .thumb_func
  .weak GPIO15_IRQHandler
GPIO15_IRQHandler:
  b .

  .thumb_func
  .weak GPIO16_IRQHandler
GPIO16_IRQHandler:
  b .

  .thumb_func
  .weak GPIO17_IRQHandler
GPIO17_IRQHandler:
  b .

  .thumb_func
  .weak GPIO18_IRQHandler
GPIO18_IRQHandler:
  b .

  .thumb_func
  .weak GPIO19_IRQHandler
GPIO19_IRQHandler:
  b .

  .thumb_func
  .weak GPIO20_IRQHandler
GPIO20_IRQHandler:
  b .

  .thumb_func
  .weak GPIO21_IRQHandler
GPIO21_IRQHandler:
  b .

  .thumb_func
  .weak GPIO22_IRQHandler
GPIO22_IRQHandler:
  b .

  .thumb_func
  .weak GPIO23_IRQHandler
GPIO23_IRQHandler:
  b .

  .thumb_func
  .weak GPIO24_IRQHandler
GPIO24_IRQHandler:
  b .

  .thumb_func
  .weak GPIO25_IRQHandler
GPIO25_IRQHandler:
  b .

  .thumb_func
  .weak GPIO26_IRQHandler
GPIO26_IRQHandler:
  b .

  .thumb_func
  .weak GPIO27_IRQHandler
GPIO27_IRQHandler:
  b .

  .thumb_func
  .weak GPIO28_IRQHandler
GPIO28_IRQHandler:
  b .

  .thumb_func
  .weak GPIO29_IRQHandler
GPIO29_IRQHandler:
  b .

  .thumb_func
  .weak GPIO30_IRQHandler
GPIO30_IRQHandler:
  b .

  .thumb_func
  .weak GPIO31_IRQHandler
GPIO31_IRQHandler:
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
  .word MemManage_Handler
  .word BusFault_Handler
  .word UsageFault_Handler
  .word 0 /* Reserved */
  .word 0 /* Reserved */
  .word 0 /* Reserved */
  .word 0 /* Reserved */
  .word SVC_Handler
  .word DebugMon_Handler
  .word 0 /* Reserved */
  .word PendSV_Handler
  .word SysTick_Handler
  .word WdogWakeup_IRQHandler
  .word RTC_Wakeup_IRQHandler
  .word SPI0_IRQHandler
  .word SPI1_IRQHandler
  .word I2C0_IRQHandler
  .word I2C0_SMBAlert_IRQHandler
  .word I2C0_SMBus_IRQHandler
  .word I2C1_IRQHandler
  .word I2C1_SMBAlert_IRQHandler
  .word I2C1_SMBus_IRQHandler
  .word UART0_IRQHandler
  .word UART1_IRQHandler
  .word EthernetMAC_IRQHandler
  .word DMA_IRQHandler
  .word Timer1_IRQHandler
  .word Timer2_IRQHandler
  .word CAN_IRQHandler
  .word ENVM0_IRQHandler
  .word ENVM1_IRQHandler
  .word ComBlk_IRQHandler
  .word USB_IRQHandler
  .word USB_DMA_IRQHandler
  .word PLL_Lock_IRQHandler
  .word PLL_LockLost_IRQHandler
  .word CommSwitchError_IRQHandler
  .word CacheError_IRQHandler
  .word DDR_IRQHandler
  .word HPDMA_Complete_IRQHandler
  .word HPDMA_Error_IRQHandler
  .word ECC_Error_IRQHandler
  .word MDDR_IOCalib_IRQHandler
  .word FAB_PLL_Lock_IRQHandler
  .word FAB_PLL_LockLost_IRQHandler
  .word FIC64_IRQHandler
  .word FabricIrq0_IRQHandler
  .word FabricIrq1_IRQHandler
  .word FabricIrq2_IRQHandler
  .word FabricIrq3_IRQHandler
  .word FabricIrq4_IRQHandler
  .word FabricIrq5_IRQHandler
  .word FabricIrq6_IRQHandler
  .word FabricIrq7_IRQHandler
  .word FabricIrq8_IRQHandler
  .word FabricIrq9_IRQHandler
  .word FabricIrq10_IRQHandler
  .word FabricIrq11_IRQHandler
  .word FabricIrq12_IRQHandler
  .word FabricIrq13_IRQHandler
  .word FabricIrq14_IRQHandler
  .word FabricIrq15_IRQHandler    
  .word GPIO0_IRQHandler
  .word GPIO1_IRQHandler
  .word GPIO2_IRQHandler
  .word GPIO3_IRQHandler
  .word GPIO4_IRQHandler
  .word GPIO5_IRQHandler
  .word GPIO6_IRQHandler
  .word GPIO7_IRQHandler
  .word GPIO8_IRQHandler
  .word GPIO9_IRQHandler
  .word GPIO10_IRQHandler
  .word GPIO11_IRQHandler
  .word GPIO12_IRQHandler
  .word GPIO13_IRQHandler
  .word GPIO14_IRQHandler
  .word GPIO15_IRQHandler
  .word GPIO16_IRQHandler
  .word GPIO17_IRQHandler
  .word GPIO18_IRQHandler
  .word GPIO19_IRQHandler
  .word GPIO20_IRQHandler
  .word GPIO21_IRQHandler
  .word GPIO22_IRQHandler
  .word GPIO23_IRQHandler
  .word GPIO24_IRQHandler
  .word GPIO25_IRQHandler
  .word GPIO26_IRQHandler
  .word GPIO27_IRQHandler
  .word GPIO28_IRQHandler
  .word GPIO29_IRQHandler
  .word GPIO30_IRQHandler
  .word GPIO31_IRQHandler
  .word 0 /* Reserved */
  .word 0 /* Reserved */
_vectors_end:

#ifdef VECTORS_IN_RAM
  .section .vectors_ram, "ax"
  .align 0
  .global _vectors_ram

_vectors_ram:
  .space _vectors_end - _vectors, 0
#endif
