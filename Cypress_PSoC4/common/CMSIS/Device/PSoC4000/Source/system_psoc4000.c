/*****************************************************************************
 * THIS FILE IS PROVIDED AS IS WITH NO WARRANTY OF ANY KIND, INCLUDING THE   *
 * WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. *
 *****************************************************************************/

#include <stdint.h>
#include <psoc4.h>

#if (CY_PSOC4_4200 || CY_PSOC4_4100)
  #define CLK_IMO_TRIM2   *((volatile uint32_t *)0x400BFF2C)
  #define CLK_SELECT      *((volatile uint32_t *)0x400B0100)
#elif (CY_PSOC4_4000)
  #define CLK_IMO_SELECT  *((volatile uint32_t *)0x40030F08)
  #define CLK_SELECT      *((volatile uint32_t *)0x40030028)
#endif

uint32_t SystemCoreClock;

void SystemCoreClockUpdate(void)
{
#if (CY_PSOC4_4200 || CY_PSOC4_4100)
  static const uint8_t imo_lookup[64] =
  {
    [3]  =  3, [4]  =  4, [5]  =  5, [6]  =  6, 
    [7]  =  7, [8]  =  8, [9]  =  9, [10] = 10, 
    [11] = 11, [12] = 12, [14] = 13, [15] = 14,
    [16] = 15, [17] = 16, [18] = 17, [19] = 18,
    [20] = 19, [21] = 20, [22] = 21, [23] = 22,
    [24] = 23, [25] = 24, [27] = 25, [28] = 26,
    [29] = 27, [30] = 28, [31] = 29, [32] = 30,
    [33] = 31, [34] = 32, [35] = 33, [37] = 34,
    [38] = 35, [39] = 36, [40] = 37, [41] = 38,
    [42] = 39, [43] = 40, [46] = 41, [47] = 42,
    [48] = 43, [49] = 44, [50] = 45, [51] = 46,
    [52] = 47, [53] = 48,
  };
  SystemCoreClock = imo_lookup[(CLK_IMO_TRIM2 >> 0) & (sizeof(imo_lookup) - 1)] * 1000000UL;
  SystemCoreClock >>= (CLK_SELECT >> 19) & 0x7; /* SYSCLK pre-scaler */
#elif (CY_PSOC4_4000)
  static const uint8_t imo_lookup[8] =
  {
    [0] = 24, [1] = 28, [2] = 32, [3] = 36, 
    [4] = 40, [5] = 44, [6] = 48,
  };
  SystemCoreClock = imo_lookup[(CLK_IMO_SELECT >> 0) & (sizeof(imo_lookup) - 1)] * 1000000UL;
  SystemCoreClock >>= (CLK_SELECT >> 2) & 0x3; /* clk_hf predivider */
  SystemCoreClock >>= (CLK_SELECT >> 6) & 0x3; /* clk_sys prescaler */
#endif
}

void SystemInit(void)
{
  SystemCoreClockUpdate();
}
