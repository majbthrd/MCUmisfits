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
Dummy_Handler:
  b .

#if defined(__OPTIMIZATION_SMALL)

  .weak GPIO0_Handler
  .thumb_set GPIO0_Handler,Dummy_Handler

  .weak GPIO1_Handler
  .thumb_set GPIO1_Handler,Dummy_Handler

  .weak GPIO2_Handler
  .thumb_set GPIO2_Handler,Dummy_Handler

  .weak GPIO3_Handler
  .thumb_set GPIO3_Handler,Dummy_Handler

  .weak WDT_Handler
  .thumb_set WDT_Handler,Dummy_Handler

  .weak SCB_Handler
  .thumb_set SCB_Handler,Dummy_Handler

  .weak SPC_Handler
  .thumb_set SPC_Handler,Dummy_Handler

  .weak CSD_Handler
  .thumb_set CSD_Handler,Dummy_Handler

  .weak TCPWM0_Handler
  .thumb_set TCPWM0_Handler,Dummy_Handler

#else

  .thumb_func
  .weak GPIO0_Handler
GPIO0_Handler:
  b .

  .thumb_func
  .weak GPIO1_Handler
GPIO1_Handler:
  b .

  .thumb_func
  .weak GPIO2_Handler
GPIO2_Handler:
  b .

  .thumb_func
  .weak GPIO3_Handler
GPIO3_Handler:
  b .

  .thumb_func
  .weak WDT_Handler
WDT_Handler:
  b .

  .thumb_func
  .weak SCB_Handler
SCB_Handler:
  b .

  .thumb_func
  .weak SPC_Handler
SPC_Handler:
  b .

  .thumb_func
  .weak CSD_Handler
CSD_Handler:
  b .

  .thumb_func
  .weak TCPWM0_Handler
TCPWM0_Handler:
  b .

#endif

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
  .word GPIO0_Handler
  .word GPIO1_Handler
  .word GPIO2_Handler
  .word GPIO3_Handler
  .word WDT_Handler
  .word SCB_Handler
  .word SPC_Handler
  .word CSD_Handler
  .word TCPWM0_Handler
_vectors_end:
