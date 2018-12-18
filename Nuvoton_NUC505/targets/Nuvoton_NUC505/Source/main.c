#include <stdint.h>
#include <NUC505Series.h>
#include <debugio.h>

void main(void)
{
  int count = 0x12345678;

  for (;;)
  {
    count++;
    debug_printf("Hello, world! %d\n", count);
  }
}
