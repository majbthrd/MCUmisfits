#include <stdint.h>
#include <NUC121.h>
#include <debugio.h>

void main(void)
{
  uint32_t count = 0x12345678;

  for (;;)
  {
    count++;
    debug_printf("Hello, world! %d\n", count);
  }
}
