/*****************************************************************************
 * THIS FILE IS PROVIDED AS IS WITH NO WARRANTY OF ANY KIND, INCLUDING THE   *
 * WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. *
 *****************************************************************************/

  .syntax unified

  .global Reset_Handler
  .extern _vectors

  .section .init, "ax"
  .thumb_func

#ifndef STACK_INIT_VAL
#define STACK_INIT_VAL __RAM_segment_end__
#endif

Reset_Handler:
#ifndef NO_STACK_INIT
  ldr r0, =STACK_INIT_VAL
  ldr r1, =0x7
  bics r0, r1
  mov sp, r0
#endif

#ifndef NO_SYSTEM_INIT
  ldr r0, =SystemInit
  blx r0
  .pushsection .init_array, "aw", %init_array
  .word SystemCoreClockUpdate
  .popsection
#endif

#ifdef MEMORY_INIT
  ldr r0, =MemoryInit
  blx r0
#endif

  b _start
