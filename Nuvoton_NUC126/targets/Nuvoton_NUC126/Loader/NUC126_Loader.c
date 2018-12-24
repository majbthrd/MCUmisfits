/*****************************************************************************
 * Copyright (c) 2018 Peter Lawrence                                         *
 *                                                                           *
 * THIS FILE IS PROVIDED AS IS WITH NO WARRANTY OF ANY KIND, INCLUDING THE   *
 * WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. *
 *****************************************************************************/

#include <libmem.h>
#include <libmem_loader.h>
#include <string.h>
#include <NUC126.h>

extern unsigned char __RAM_segment_used_end__[];
extern unsigned char __RAM_segment_end__[];

#define PAGE_SIZE  2048
#define LDROM_SIZE 4096
#define SPROM_SIZE 2048

static libmem_geometry_t aprom_geometry[2];
static libmem_geometry_t ldrom_geometry[2] =
{
  [0] = { .size = PAGE_SIZE, .count = LDROM_SIZE / PAGE_SIZE, },
};
static libmem_geometry_t sprom_geometry[2] =
{
  [0] = { .size = PAGE_SIZE, .count = SPROM_SIZE / PAGE_SIZE, },
};

enum
{
  ISP_CMD_READ_DEVICE_ID = 0x0C,
  ISP_CMD_PAGE_ERASE     = 0x22,
  ISP_CMD_UINT32_WRITE   = 0x21,
  ISP_CMD_UINT64_READ    = 0x40,
};

struct config_struct
{
  union
  {
    struct
    {
      uint32_t DFEN:1;
      uint32_t LOCK:1;
      uint32_t reserved2:1;
      uint32_t CWDTEN:2;
      uint32_t MERASE:1;
      uint32_t CBS:2;
      uint32_t reserved8:2;
      uint32_t CIOINI:1;
      uint32_t reserved11:1;
      uint32_t ICELOCK:1;
      uint32_t reserved13:7;
      uint32_t CBORST:1;
      uint32_t CBOV:2;
      uint32_t CBODEN:1;
      uint32_t reserved24:2;
      uint32_t CFOSC:1;
      uint32_t CFGXT1:1;
      uint32_t reserved28:2;
      uint32_t CWDTPDEN:1;
      uint32_t CWDTEN2:1;
    } config0bits;
    uint32_t config0;
  };
  union
  {
    struct
    {
      uint32_t DFBA:20;
      uint32_t reserved20:12;
    } config1bits;
    uint32_t config1;
  };
};

static void fmc_enable(int flash_write)
{
  uint32_t val;

  /* unlock sequence to allow access to FMC->ISPCTL */
  SYS->REGLCTL = 0x59;
  SYS->REGLCTL = 0x16;
  SYS->REGLCTL = 0x88;

  CLK->AHBCLK |= CLK_AHBCLK_ISPCKEN_Msk;
  val = FMC_ISPCTL_ISPEN_Msk;
  if (flash_write)
    val |= FMC_ISPCTL_APUEN_Msk | FMC_ISPCTL_LDUEN_Msk | FMC_ISPCTL_SPUEN_Msk;
  FMC->ISPCTL = val;
}

static void fmc_disable(void)
{
  FMC->ISPCTL = 0;
}

static uint32_t fmc_id(void)
{
  uint32_t id;

  fmc_enable(0);

  /* clear any existing fail flag */
  FMC->ISPCTL |= FMC_ISPCTL_ISPFF_Msk;

  /* perform ID command */
  FMC->ISPCMD = ISP_CMD_READ_DEVICE_ID;
  FMC->ISPADDR = 0;
  FMC->ISPTRG = FMC_ISPTRG_ISPGO_Msk;
  asm volatile("isb");
  while (FMC->ISPTRG & FMC_ISPTRG_ISPGO_Msk);

  id = (FMC->ISPCTL & FMC_ISPCTL_ISPFF_Msk) ? 0 : FMC->ISPDAT;

  fmc_disable();

  return id;
}

static struct config_struct fmc_config(void)
{
  struct config_struct result;
  fmc_enable(0);

  /* clear any existing fail flag */
  FMC->ISPCTL |= FMC_ISPCTL_ISPFF_Msk;

  /* perform ID command */
  FMC->ISPCMD = ISP_CMD_UINT64_READ;
  FMC->ISPADDR = 0x300000;
  FMC->ISPTRG = FMC_ISPTRG_ISPGO_Msk;
  asm volatile("isb");
  while (FMC->ISPTRG & FMC_ISPTRG_ISPGO_Msk);

  if (0 == (FMC->ISPCTL & FMC_ISPCTL_ISPFF_Msk))
  {
    result.config0 = FMC->MPDAT0;
    result.config1 = FMC->MPDAT1;
  }

  fmc_disable();

  return result;
}

static int libmem_erase_impl(libmem_driver_handle_t *h, uint8_t *start, size_t size, uint8_t **erased_start, size_t *erased_size)
{
  uint32_t erase_addr = ((uint32_t)start & ~(PAGE_SIZE - 1)); /* align downwards to a page boundary */
  uint32_t ending_addr = (uint32_t)start + size;
  uint32_t outcome = LIBMEM_STATUS_SUCCESS;

  fmc_enable(1);

  /* sequentially erase pages until we've at least reached the desired end address */
  while (erase_addr < ending_addr)
  {
    /* clear any existing fail flag */
    FMC->ISPCTL |= FMC_ISPCTL_ISPFF_Msk;

    /* perform page erase */
    FMC->ISPADDR = erase_addr;
    FMC->ISPDAT = 0x0055AA03;
    FMC->ISPCMD = ISP_CMD_PAGE_ERASE;
    FMC->ISPTRG = FMC_ISPTRG_ISPGO_Msk;
    asm volatile("isb");
    while (FMC->ISPTRG & FMC_ISPTRG_ISPGO_Msk);

    /* bail if operation failed */
    if (FMC->ISPCTL & FMC_ISPCTL_ISPFF_Msk)
    {
      outcome = LIBMEM_STATUS_ERROR;
      goto bail;
    }

    erase_addr += PAGE_SIZE;
  }

bail:
  fmc_disable();
  return outcome;
}

static int libmem_write_impl(libmem_driver_handle_t *h, uint8_t *dest, const uint8_t *src, size_t size)
{
  uint32_t *write_pnt = (uint32_t *)((uint32_t)dest & ~3); /* align downwards to a word boundary */
  uint32_t write_val, mask, shifted, byte_offset;
  uint32_t outcome = LIBMEM_STATUS_SUCCESS;

  byte_offset = (uint32_t)dest & 3; /* use alignment remainder to point to our starting point */

  fmc_enable(1);

  /* clear any existing fail flag */
  FMC->ISPCTL |= FMC_ISPCTL_ISPFF_Msk;

  FMC->ISPCMD = ISP_CMD_UINT32_WRITE;

  while (size)
  {
    write_val = 0xFFFFFFFF;

    do
    {
      mask = 0xFF << (byte_offset << 3);
      shifted = (uint32_t)*src++ << (byte_offset << 3);
      write_val ^= mask ^ shifted;
    } while (--size && (++byte_offset < 4));

    FMC->ISPADDR = (uint32_t)write_pnt;
    FMC->ISPDAT = write_val;
    FMC->ISPTRG = FMC_ISPTRG_ISPGO_Msk;
    asm volatile("isb");
    while (FMC->ISPTRG & FMC_ISPTRG_ISPGO_Msk);

    /* bail if operation failed */
    if (FMC->ISPCTL & FMC_ISPCTL_ISPFF_Msk)
    {
      outcome = LIBMEM_STATUS_ERROR;
      goto bail;
    }

    write_pnt++;
    byte_offset = 0;
  }

bail:
  fmc_disable();
  return outcome;
}

static const libmem_driver_functions_t driver_functions =
{       
  .write  = libmem_write_impl,
  .fill   = NULL,
  .erase  = libmem_erase_impl,
  .lock   = NULL,
  .unlock = NULL,
  .flush  = NULL,
};

int main(unsigned long param0)
{
  libmem_driver_handle_t aprom, ldrom, sprom;
  int res;
  uint32_t id;
  uint32_t flash_size;
  struct config_struct config;

  id = fmc_id();
  config = fmc_config();

  flash_size = 512 << ((id >> 4) & 0x0F);

  aprom_geometry[0].size = PAGE_SIZE;
  aprom_geometry[0].count = flash_size / PAGE_SIZE;

#if 0
  volatile int wait = 1; while (wait);
  static uint8_t pattern[256];
  libmem_erase_impl(NULL, 0x200000, sizeof(pattern), NULL, NULL);
  for (res = 0; res < sizeof(pattern); res++)
    pattern[res] = res;
  libmem_write_impl(NULL, 0x200001, pattern, sizeof(pattern));
#endif

  if (1 != config.config0bits.CBS)
    libmem_register_driver(&aprom, (uint8_t *)0x00000000, flash_size, aprom_geometry, 0, &driver_functions, 0);

  if (3 != config.config0bits.CBS)
    libmem_register_driver(&ldrom, (uint8_t *)0x00100000, LDROM_SIZE, ldrom_geometry, 0, &driver_functions, 0);

  libmem_register_driver(&sprom, (uint8_t *)0x00200000, SPROM_SIZE, sprom_geometry, 0, &driver_functions, 0);

  res = libmem_rpc_loader_start(__RAM_segment_used_end__, __RAM_segment_end__ - 1); 
  libmem_rpc_loader_exit(res, 0);

  return 0;
}
