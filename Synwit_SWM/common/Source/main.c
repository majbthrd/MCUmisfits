#include <stdint.h>
#include <SWM.h>
#include <debugio.h>

void main(void)
{
  uint32_t count = 0x12345678;

  for (;;)
  {
    count++;
    debug_printf("Hello, world! %u\n", count);^M
  }
}
