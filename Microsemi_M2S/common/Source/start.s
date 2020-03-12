//
// start.s : Defines entry point for an M2S assembly code only
//           application.
//
//           Please note, as this is an assembly code only project, the C/C++
//           runtime library has not been initialised. So do not attempt to call
//           any C/C++ library functions because they probably won't work.
//

.syntax unified
.global _start
.text

.thumb_func:
_start:
  ldr r0, =#0x12345678
loop:
  adds r0, r0, #1
  b loop
