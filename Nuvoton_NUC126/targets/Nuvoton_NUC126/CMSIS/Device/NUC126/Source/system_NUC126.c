/**************************************************************************//**
 * @file     system_NUC126.c
 * @version  V2.00
 * $Revision: 6 $
 * $Date: 16/10/25 4:25p $
 * @brief    NUC126 Series System Setting Source File
 *
 * @note
 * Copyright (C) 2016 Nuvoton Technology Corp. All rights reserved.
 *
 ******************************************************************************/
#include <stdio.h>
#include <stdint.h>
#include "NUC126.h"

#define CLK_CLKSEL0_HCLKSEL_PLL (0x02UL<<CLK_CLKSEL0_HCLKSEL_Pos) /*!< Setting HCLK clock source as PLL */
#define CLK_PLLCTL_PLLSRC_HIRC  0x00080000UL    /*!< For PLL clock source is HIRC. 3.2MHz < FIN < 150MHz */

/*----------------------------------------------------------------------------
  Clock Variable definitions
 *----------------------------------------------------------------------------*/
uint32_t SystemCoreClock  = __HSI;              /*!< System Clock Frequency (Core Clock) */
uint32_t CyclesPerUs      = (__HSI / 1000000);  /*!< Cycles per micro second             */
uint32_t PllClock         = __HSI;              /*!< PLL Output Clock Frequency          */
static const uint32_t gau32ClkSrcTbl[] = { (uint32_t)__HXT, (uint32_t)__LXT, (uint32_t)__HSI, (uint32_t)__LIRC, (uint32_t)__HIRC48, (uint32_t)NULL, (uint32_t)NULL, (uint32_t)__HIRC};


/**
  * @brief      Get PLL clock frequency
  * @param      None
  * @return     PLL frequency
  * @details    This function get PLL frequency. The frequency unit is Hz.
  */
static uint32_t GetPLLClockFreq(void)
{
    uint32_t u32PllFreq = 0, u32PllReg;
    uint32_t u32FIN, u32NF, u32NR, u32NO;
    uint8_t au8NoTbl[4] = {1, 2, 2, 4};

    u32PllReg = CLK->PLLCTL;

    if(u32PllReg & (CLK_PLLCTL_PD_Msk | CLK_PLLCTL_OE_Msk))
        return 0;           /* PLL is in power down mode or fix low */

    if(u32PllReg & CLK_PLLCTL_PLLSRC_HIRC)
        u32FIN = __HIRC;    /* PLL source clock from HIRC */
    else
        u32FIN = __HXT;     /* PLL source clock from HXT */

    if(u32PllReg & CLK_PLLCTL_BP_Msk)
        return u32FIN;      /* PLL is in bypass mode */

    /* PLL is output enabled in normal work mode */
    u32NO = au8NoTbl[((u32PllReg & CLK_PLLCTL_OUTDIV_Msk) >> CLK_PLLCTL_OUTDIV_Pos)];
    u32NF = ((u32PllReg & CLK_PLLCTL_FBDIV_Msk) >> CLK_PLLCTL_FBDIV_Pos) + 2;
    u32NR = ((u32PllReg & CLK_PLLCTL_INDIV_Msk) >> CLK_PLLCTL_INDIV_Pos) + 2;

    /* u32FIN is shifted 2 bits to avoid overflow */
    u32PllFreq = (((u32FIN >> 2) * u32NF) / (u32NR * u32NO) << 2);

    return u32PllFreq;
}

/**
 * @brief    Update the Variable SystemCoreClock
 *
 * @param    None
 *
 * @return   None
 *
 * @details  This function is used to update the variable SystemCoreClock
 *           and must be called whenever the core clock is changed.
 */
void SystemCoreClockUpdate(void)
{
    uint32_t u32Freq, u32ClkSrc;
    uint32_t u32HclkDiv;

    u32ClkSrc = CLK->CLKSEL0 & CLK_CLKSEL0_HCLKSEL_Msk;

    /* Update PLL Clock */
    PllClock = GetPLLClockFreq();

    if(u32ClkSrc != CLK_CLKSEL0_HCLKSEL_PLL)
    {
        /* Use the clock sources directly */
        u32Freq = gau32ClkSrcTbl[u32ClkSrc];
    }
    else
    {
        /* Use PLL clock */
        u32Freq = PllClock;
    }

    u32HclkDiv = (CLK->CLKDIV0 & CLK_CLKDIV0_HCLKDIV_Msk) + 1;

    /* Update System Core Clock */
    SystemCoreClock = u32Freq / u32HclkDiv;

    CyclesPerUs = (SystemCoreClock + 500000) / 1000000;
}


/**
 * @brief    System Initialization
 *
 * @param    None
 *
 * @return   None
 *
 * @details  The necessary initialization of system. Global variables are forbidden here.
 */
void SystemInit(void)
{
#ifdef INIT_SYSCLK_AT_BOOTING
    int32_t i32TimeoutCnt;
    uint32_t u32HclkSelect;
    int8_t i8IsPllEn;

    PllClock = 0;
    i8IsPllEn = 0;
    u32HclkSelect = CLK->CLKSEL0 & CLK_CLKSEL0_HCLKSEL_Msk;
    if(u32HclkSelect == CLK_CLKSEL0_HCLKSEL_HXT)
    {
        /* Set to 72MHz system clock frequency when clock source is from external 12MHz */
        CLK->PLLCTL = CLK_PLLCTL_72MHz_HXT;

        /* Waiting for PLL ready */
        i32TimeoutCnt = (__HXT / 1000); /* Timeout is about 1ms */
        while((CLK->STATUS & CLK_STATUS_PLLSTB_Msk) == 0)
        {
            if(i32TimeoutCnt-- <= 0)
                break;
        }

        i8IsPllEn = 1;
    }
    else if(u32HclkSelect == CLK_CLKSEL0_HCLKSEL_HIRC)
    {
        /* Set to 71.8848MHz system clock frequency when clock source is from internal 22.1184MHz RC clock */
        CLK->PLLCTL = CLK_PLLCTL_72MHz_HIRC;

        /* Waiting for PLL ready */
        i32TimeoutCnt = (__HIRC / 1000); /* Timeout is about 1ms */
        while((CLK->STATUS & CLK_STATUS_PLLSTB_Msk) == 0)
        {
            if(i32TimeoutCnt-- <= 0)
                break;
        }

        i8IsPllEn = 1;
    }

    if(i8IsPllEn)
    {
        /* Set PLL as HCLK clock source (HCLK_S is locked setting)*/
        SYS_UnlockReg();
        CLK->CLKSEL0 = CLK_CLKSEL0_HCLKSEL_PLL;
        SYS_LockReg();
    }
#endif
}
