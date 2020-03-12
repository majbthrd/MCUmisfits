#include <stdint.h>
#include <debugio.h>
#include "m2sxxx.h"

void main(void)
{
  uint32_t count = 0x12345678;

  for (;;)
  {
    count++;
    debug_printf("Hello, world! %u\n", count);
  }
}
