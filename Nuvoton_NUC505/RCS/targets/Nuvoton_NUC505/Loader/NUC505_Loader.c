/*****************************************************************************
 * Copyright (c) 2018 Peter Lawrence                                         *
 *                                                                           *
 * THIS FILE IS PROVIDED AS IS WITH NO WARRANTY OF ANY KIND, INCLUDING THE   *
 * WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. *
 *****************************************************************************/

#include <libmem.h>
#include <libmem_loader.h>
#include <string.h>
#include <NUC505Series.h>

extern unsigned char __RAM_segment_used_end__[];
extern unsigned char __RAM_segment_end__[];

#define PAGE_SIZE   256
#define SECTOR_SIZE 4096

static libmem_geometry_t geometry[2];

static struct
{
  uint32_t ctl0, ctl1;
} backup;

static void spi_command(uint8_t cmd)
{
  uint32_t val;

  val = SPIM->CTL0 & (SPIM_CTL0_CMDCODE_Msk | SPIM_CTL0_SUSPITV_Msk);
  SPIM->CTL0 = val | (0UL << SPIM_CTL0_OPMODE_Pos) | (1UL << SPIM_CTL0_QDIODIR_Pos) | (0UL << SPIM_CTL0_BURSTNUM_Pos) | (7UL << SPIM_CTL0_DWIDTH_Pos);
  SPIM->TX0 = cmd;
  SPIM->CTL1 |= SPIM_CTL1_SPIMEN_Msk;
  while (SPIM->CTL1 & SPIM_CTL1_SPIMEN_Msk);
}

static uint32_t spi_read(int bytes)
{
  uint32_t val;

  val = SPIM->CTL0 & (SPIM_CTL0_CMDCODE_Msk | SPIM_CTL0_SUSPITV_Msk);
  SPIM->CTL0 = val | (0UL << SPIM_CTL0_OPMODE_Pos) | (0UL << SPIM_CTL0_QDIODIR_Pos) | (0UL << SPIM_CTL0_BURSTNUM_Pos) | (((bytes << 3) - 1) << SPIM_CTL0_DWIDTH_Pos);
  SPIM->CTL1 |= SPIM_CTL1_SPIMEN_Msk;
  while (SPIM->CTL1 & SPIM_CTL1_SPIMEN_Msk);
  return SPIM->RX0;
}

static void spi_backup(void)
{
  while (SPIM->CTL1 & SPIM_CTL1_SPIMEN_Msk);

  backup.ctl0 = SPIM->CTL0;
  backup.ctl1 = SPIM->CTL1;

  CLK->AHBCLK |= CLK_AHBCLK_SPIMCKEN_Msk;
  SYS->IPRST1 |= SYS_IPRST1_SPIMRST_Msk;
  SYS->IPRST1 &= ~SYS_IPRST1_SPIMRST_Msk;

  SPIM->CTL1 = (1UL << SPIM_CTL1_DIVIDER_Pos) | (1UL << SPIM_CTL1_IFSEL_Pos);
  SPIM->CTL1 |= SPIM_CTL1_SPIMEN_Msk;
}

static void spi_restore(void)
{
  SPIM->CTL0 = backup.ctl0;
  SPIM->CTL1 = backup.ctl1;
}

static void spi_ssel_active(void)
{
  SPIM->CTL1 &= ~SPIM_CTL1_SS_Msk;
}

static void spi_ssel_inactive(void)
{
  SPIM->CTL1 |= SPIM_CTL1_SS_Msk;
}

static void spi_write_enable(void)
{
  spi_ssel_active();
  spi_command(0x06 /* Write Enable */);
  spi_ssel_inactive();
}

static void spi_wait_until_finished(void)
{
  spi_ssel_active();
  spi_command(0x05 /* Read Status Register-1 */);
  while (spi_read(1) & 0x01 /* BUSY */);
  spi_ssel_inactive();
}

static int libmem_erase_impl(libmem_driver_handle_t *h, uint8_t *start, size_t size, uint8_t **erased_start, size_t *erased_size)
{
  uint32_t start_addr, erase_addr = ((uint32_t)start & ~(SECTOR_SIZE - 1)); /* align downwards to a sector boundary */
  uint32_t ending_addr = (uint32_t)start + size;

  start_addr = erase_addr;
  if (erased_start)
    *erased_start = (uint8_t *)start_addr;

  spi_backup();

  /* sequentially erase sectors until we've at least reached the desired end address */
  while (erase_addr < ending_addr)
  {
    spi_write_enable();

    spi_ssel_active();
    spi_command(0x20 /* Sector Erase */);
    spi_command(erase_addr >> 16);
    spi_command(erase_addr >> 8);
    spi_command(erase_addr >> 0);
    spi_ssel_inactive();

    spi_wait_until_finished();

    erase_addr += SECTOR_SIZE;
  }

  spi_restore();

  if (erased_size)
    *erased_size = erase_addr - start_addr;

  return LIBMEM_STATUS_SUCCESS;
}

static int libmem_write_impl(libmem_driver_handle_t *h, uint8_t *dest, const uint8_t *src, size_t size)
{
  uint32_t write_addr = (uint32_t)dest & ~(PAGE_SIZE - 1); /* align downwards to a page boundary */
  uint32_t byte_offset;
  static uint32_t aligned_buffer[PAGE_SIZE / sizeof(uint32_t)];
  uint8_t *const page_buffer = (uint8_t *)aligned_buffer;
  uint32_t val;

  byte_offset = (uint32_t)dest - write_addr;

  spi_backup();

  while (size)
  {
    memset(aligned_buffer, 0xFF, sizeof(aligned_buffer));

    do
    {
      page_buffer[byte_offset++] = *src++;
      byte_offset &= (PAGE_SIZE - 1);
    } while (--size && byte_offset);

    spi_write_enable();

    spi_ssel_active();

    val = SPIM->CTL0 & (SPIM_CTL0_SUSPITV_Msk);
    SPIM->CTL0 = val | (0x02 /* Page Program */ << SPIM_CTL0_CMDCODE_Pos) | (1UL << SPIM_CTL0_OPMODE_Pos);
    SPIM->SRAMADDR = (uint32_t)aligned_buffer;
    SPIM->DMATBCNT = PAGE_SIZE;
    SPIM->FADDR = write_addr;
    SPIM->CTL1 |= SPIM_CTL1_SPIMEN_Msk;
    while (SPIM->CTL1 & SPIM_CTL1_SPIMEN_Msk);
    spi_ssel_inactive();

    write_addr += PAGE_SIZE;
  }

  spi_restore();

  return LIBMEM_STATUS_SUCCESS;
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
  libmem_driver_handle_t h;
  int res;
  uint32_t id;
  uint32_t flash_size;

  spi_backup();

  spi_ssel_active();
  spi_command(0x9F /* Read JEDEC ID */);
  id = spi_read(3);
  spi_ssel_inactive();

  spi_restore();

  if ( (0 == id) || (id >= 0xFFFFFF) )
    return 0;

  flash_size = (0x1540EF == id) ? 2097152 : 524288; /* 2MB part (rather than 512kB) detected */

  geometry[0].size = SECTOR_SIZE;
  geometry[0].count = flash_size / SECTOR_SIZE;

  libmem_register_driver(&h, 0, flash_size, geometry, 0, &driver_functions, 0);
  res = libmem_rpc_loader_start(__RAM_segment_used_end__, __RAM_segment_end__ - 1); 
  libmem_rpc_loader_exit(res, 0);

  return 0;
}
