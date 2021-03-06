/**************************************************************************//**
 * @file     system_NUC505Series.c
 * @version  V1.00
 * $Revision: 10 $
 * $Date: 13/11/10 9:32a $
 * @brief    NUC505 series system clock init code and assert handler
 *
 * @note
 * Copyright (C) 2013 Nuvoton Technology Corp. All rights reserved.
*****************************************************************************/

#include "NUC505Series.h"

/*----------------------------------------------------------------------------
  Clock Variable definitions
 *----------------------------------------------------------------------------*/
uint32_t SystemCoreClock = __HSI;               /*!< System Clock Frequency (Core Clock)*/
uint32_t CyclesPerUs      = (__HSI / 1000000);  /*!< Cycles per micro second            */
uint32_t gau32ClkSrcTbl[] = {__HXT, 0}; 				/*!< System clock source table */

#if defined (__ICCARM__)
void SystemCoreClockUpdate (void) @ "SystemCoreClockUpdate";    // Ensure this function can be initialized by copy.
#endif

/**
  * @brief                      This function get PLL frequency. The frequency unit is Hz.
  * @return     PLL frequency.
  */
static uint32_t CLK_GetPLLClockFreq(void)
{
    uint32_t u32Freq =0, u32PLLSrc;
    uint32_t u32PllReg, u32M,u32N,u32P;
	
		u32PLLSrc = __HXT;
    u32PllReg = CLK->PLLCTL;
		if((u32PllReg & CLK_PLLCTL_PD_Msk))
			return 0;
		u32M = ((u32PllReg&CLK_PLLCTL_INDIV_Msk)>>CLK_PLLCTL_INDIV_Pos)+1;
		u32N = ((u32PllReg&CLK_PLLCTL_FBDIV_Msk)>>CLK_PLLCTL_FBDIV_Pos)+1;
		u32P = ((u32PllReg&CLK_PLLCTL_OUTDIV_Msk)>>CLK_PLLCTL_OUTDIV_Pos)+1;
		u32Freq = (u32PLLSrc/u32M)*u32N/u32P;	

    return u32Freq;
}

/*----------------------------------------------------------------------------
  Clock functions
 *----------------------------------------------------------------------------*/
void SystemCoreClockUpdate (void)            /* Get Core Clock Frequency      */
{

    uint32_t u32Freq, u32ClkSrc;
    uint32_t u32HclkDiv;

    u32ClkSrc = CLK->CLKDIV0 & CLK_CLKDIV0_HCLKSEL_Msk;

    if(u32ClkSrc != CLK_CLKDIV0_HCLKSEL_Msk) {
        /* Use the clock sources directly */
        u32Freq = gau32ClkSrcTbl[u32ClkSrc];
    } else {
        /* Use PLL clock */
        u32Freq = CLK_GetPLLClockFreq();
    }

    u32HclkDiv = (CLK->CLKDIV0 & CLK_CLKDIV0_HCLKDIV_Msk) + 1;

    /* Update System Core Clock */
    SystemCoreClock = u32Freq/u32HclkDiv;

    CyclesPerUs = (SystemCoreClock + 500000) / 1000000;

}

/**
 * Initialize the system
 *
 * @param  none
 * @return none
 *
 * @brief  Setup the microcontroller system.
 */
void SystemInit (void)
{

    /* FPU settings ------------------------------------------------------------*/
#if (__FPU_PRESENT == 1) && (__FPU_USED == 1)
    SCB->CPACR |= ((3UL << 10*2) |                 /* set CP10 Full Access */
                   (3UL << 11*2)  );               /* set CP11 Full Access */
#endif

}
/*** (C) COPYRIGHT 2013 Nuvoton Technology Corp. ***/
