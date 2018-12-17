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

  .weak TMRSE0_Handler
  .thumb_set TMRSE0_Handler,Dummy_Handler

  .weak TMRSE1_Handler
  .thumb_set TMRSE1_Handler,Dummy_Handler

  .weak WDT_Handler
  .thumb_set WDT_Handler,Dummy_Handler

  .weak ACMP_Handler
  .thumb_set ACMP_Handler,Dummy_Handler

  .weak GPIO0_Handler
  .thumb_set GPIO0_Handler,Dummy_Handler

  .weak GPIO1_Handler
  .thumb_set GPIO1_Handler,Dummy_Handler

  .weak GPIO2_Handler
  .thumb_set GPIO2_Handler,Dummy_Handler

  .weak GPIO3_Handler
  .thumb_set GPIO3_Handler,Dummy_Handler

  .weak GPIO4_Handler
  .thumb_set GPIO4_Handler,Dummy_Handler

  .weak GPIO5_Handler
  .thumb_set GPIO5_Handler,Dummy_Handler

  .weak GPIO6_Handler
  .thumb_set GPIO6_Handler,Dummy_Handler

  .weak GPIO7_Handler
  .thumb_set GPIO7_Handler,Dummy_Handler

  .weak GPIO8_Handler
  .thumb_set GPIO8_Handler,Dummy_Handler

  .weak GPIO9_Handler
  .thumb_set GPIO9_Handler,Dummy_Handler

#else

  .thumb_func
  .weak TMRSE0_Handler
TMRSE0_Handler:
  b .

  .thumb_func
  .weak TMRSE1_Handler
TMRSE1_Handler:
  b .

  .thumb_func
  .weak WDT_Handler
WDT_Handler:
  b .

  .thumb_func
  .weak ACMP_Handler
ACMP_Handler:
  b .

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
  .weak GPIO4_Handler
GPIO4_Handler:
  b .

  .thumb_func
  .weak GPIO5_Handler
GPIO5_Handler:
  b .

  .thumb_func
  .weak GPIO6_Handler
GPIO6_Handler:
  b .

  .thumb_func
  .weak GPIO7_Handler
GPIO7_Handler:
  b .

  .thumb_func
  .weak GPIO8_Handler
GPIO8_Handler:
  b .

  .thumb_func
  .weak GPIO9_Handler
GPIO9_Handler:
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
  .word TMRSE0_Handler
  .word TMRSE1_Handler
  .word WDT_Handler
  .word ACMP_Handler
  .word GPIO0_Handler
  .word GPIO1_Handler
  .word GPIO2_Handler
  .word GPIO3_Handler
  .word GPIO4_Handler
  .word GPIO5_Handler
  .word GPIO6_Handler
  .word GPIO7_Handler
  .word GPIO8_Handler
  .word GPIO9_Handler
_vectors_end:
