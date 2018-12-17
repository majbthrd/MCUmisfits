#include <stdint.h>
#include <SWM.h>

#define __HSI (18000000UL)

uint32_t SystemCoreClock;

void SystemCoreClockUpdate(void)
{
  uint32_t clk_rate = __HSI;
  uint32_t clk_div;

  if (SYS->DBL_IRC.DBL_IRC)
    clk_rate <<= 1;

  if (SYS->CLK_CFG.CORE_CLK_DIV & 1)
    clk_div = 1;
  else if (0 == SYS->CLK_CFG.CORE_CLK_DIV)
    clk_div = 2;
  else
    clk_div = SYS->CLK_CFG.CORE_CLK_DIV;

  SystemCoreClock = clk_rate / clk_div;
}

void SystemInit(void)
{
  SYS->CLK_CFG.CORE_CLK_DIV = 1;
  SYS->DBL_IRC.DBL_IRC = 0;

  SystemCoreClockUpdate();
}
