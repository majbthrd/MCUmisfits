/*****************************************************************************
 * THIS FILE IS PROVIDED AS IS WITH NO WARRANTY OF ANY KIND, INCLUDING THE   *
 * WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. *
 *****************************************************************************/

  .syntax unified

  .global Reset_Handler
  .extern _vectors

  .section .init, "ax"
  .thumb_func

  .equ VTOR_REG, 0xE000ED08

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

#ifdef VECTORS_IN_RAM
  /* Copy exception vectors into RAM */
  ldr r0, =__vectors_start__
  ldr r1, =__vectors_end__
  ldr r2, =__vectors_ram_start__
1:
  cmp r0, r1
  beq 2f
  ldr r3, [r0]
  str r3, [r2]
  adds r0, r0, #4
  adds r2, r2, #4
  b 1b
2:
#endif

#ifndef NO_VTOR_CONFIG
  /* Configure vector table offset register */
  ldr r0, =VTOR_REG
#ifdef VECTORS_IN_RAM
  ldr r1, =_vectors_ram
#else
  ldr r1, =_vectors
#endif
  str r1, [r0]
#endif

  b _start
