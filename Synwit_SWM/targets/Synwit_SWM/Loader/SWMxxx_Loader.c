/*****************************************************************************
 * Copyright (c) 2018 Peter Lawrence                                         *
 *                                                                           *
 * THIS FILE IS PROVIDED AS IS WITH NO WARRANTY OF ANY KIND, INCLUDING THE   *
 * WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. *
 *****************************************************************************/

#include <libmem.h>
#include <libmem_loader.h>
#include <SWM.h>

extern unsigned char __RAM_segment_used_end__[];
extern unsigned char __RAM_segment_end__[];

#define FLASHREG1	(*(uint32_t *)0x1F000000)
#define FLASHKEY	0xAAAAAAAA

#define PAGE_SIZE  256

static libmem_geometry_t geometry[2] =
{
  [0] = {
    .count = FLASH_SIZE / PAGE_SIZE,
    .size = PAGE_SIZE,
  },
};

static int libmem_erase_impl(libmem_driver_handle_t *h, uint8_t *start, size_t size, uint8_t **erased_start, size_t *erased_size)
{
  uint32_t erase_addr = ((uint32_t)start & ~(PAGE_SIZE - 1)); /* align downwards to a page boundary */
  uint32_t ending_addr = (uint32_t)start + size;

  /* sequentially erase pages until we've at least reached the desired end address */
  while (erase_addr < ending_addr)
  {
    FLASHREG1 = 4; /* erase page begin */

    *(uint32_t *)erase_addr = FLASHKEY;
    
    for (volatile int wait = 0; wait < 5000; wait++);

    FLASHREG1 = 0; /* erase page end */

    erase_addr += PAGE_SIZE;
  }

  return LIBMEM_STATUS_SUCCESS;
}

static int libmem_write_impl(libmem_driver_handle_t *h, uint8_t *dest, const uint8_t *src, size_t size)
{
  uint32_t *write_pnt = (uint32_t *)((uint32_t)dest & ~3); /* align dowards to a word boundary */
  uint32_t write_val, mask, shifted, byte_offset;

  byte_offset = (uint32_t)dest & 3; /* use alignment remainder to point to our starting point */

  FLASHREG1 = 1; /* write page begin */

  while (size)
  {
    write_val = 0xFFFFFFFF;

    do
    {
      mask = 0xFF << (byte_offset << 3);
      shifted = (uint32_t)*src++ << (byte_offset << 3);
      write_val ^= mask ^ shifted;
    } while (--size && (++byte_offset < 4));

    *write_pnt++ = write_val;
    byte_offset = 0;
  }

  FLASHREG1 = 0; /* write page end */

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

  /* set clock to ~18MHz */
  SYS->CLK_CFG.CORE_CLK_DIV = 1;
  SYS->DBL_IRC.DBL_IRC = 0;

  libmem_register_driver(&h, 0, FLASH_SIZE, geometry, 0, &driver_functions, 0);
  res = libmem_rpc_loader_start(__RAM_segment_used_end__, __RAM_segment_end__ - 1); 
  libmem_rpc_loader_exit(res, 0);

  return 0;
}
