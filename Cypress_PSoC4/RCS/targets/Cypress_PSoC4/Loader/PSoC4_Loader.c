/*****************************************************************************
 * Copyright (c) 2015 Peter Lawrence                                         *
 *                                                                           *
 * THIS FILE IS PROVIDED AS IS WITH NO WARRANTY OF ANY KIND, INCLUDING THE   *
 * WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. *
 *****************************************************************************/

#include <libmem.h>
#include <libmem_loader.h>
#include <string.h> // for memcpy

#define OPTION_CHECKSUM_WORKAROUND
#define OPTION_CLOCK_CONFIGURE

extern unsigned char __RAM_segment_start__[];
extern unsigned char __RAM_segment_used_end__[];

#if (CY_PSOC4_4200 || CY_PSOC4_4100)
  #define SPCIF_GEOMETRY *((volatile uint32_t *)0x400E0000)
  #define CPUSS_SYSREQ   *((volatile uint32_t *)0x40000004)
  #define CPUSS_SYSARG   *((volatile uint32_t *)0x40000008)
  #define CLK_IMO_CONFIG *((volatile uint32_t *)0x400B0108)
  #define CPUSS_CONFIG   *((volatile uint32_t *)0x40000000)
  #define RAM_SIZE        (4 * 1024)
  #define ROW_SIZE_LOG2   (7)
#elif (CY_PSOC4_4000)
  #define SPCIF_GEOMETRY *((volatile uint32_t *)0x40110000) /* 20140410 TRM says 0x4012_0000, but this is apparently wrong */
  #define CPUSS_SYSREQ   *((volatile uint32_t *)0x40100004)
  #define CPUSS_SYSARG   *((volatile uint32_t *)0x40100008)
  #define CLK_IMO_CONFIG *((volatile uint32_t *)0x40030030) /* 20140410 TRM says 0x4006_0030, but this is apparently wrong */
  #define CPUSS_CONFIG   *((volatile uint32_t *)0x40100000)
  #define RAM_SIZE        (2 * 1024)
  #define ROW_SIZE_LOG2   (6)
#else
  #error
#endif

#define SROM_KEY 0xD3B6

#define SROM_CMD_GET_SILICON_ID   0x00
#define SROM_CMD_LOAD_LATCH       0x04
#define SROM_CMD_WRITE_ROW        0x05
#define SROM_CMD_PROGRAM_ROW      0x06
#define SROM_CMD_CLK_CONFIG       0x15
#define SROM_CMD_CLK_RESTORE      0x17

#define SROM_SYSREQ_BIT     0x80000000
#define SROM_PRIVILEGED_BIT 0x10000000

#define CPUSS_NO_RST_OVR    (1UL << 27) /* bit in CPUSS_SYSREQ */

#define FLASH_START_ADDRESS (0x00000000)
#define ROW_SIZE            (1 << ROW_SIZE_LOG2)

static unsigned psoc4_wait(void);

static unsigned FLASH_SIZE;
static libmem_geometry_t geometry[2];
static uint32_t imo_backup;

/* word-aligned buffer for passing data to Cypress (NMI-based) API */
static uint32_t write_buffer[2 + (ROW_SIZE / sizeof(uint32_t))];

static unsigned init_write_buffer(uint8_t *addr)
{
  unsigned row;

  row = (unsigned)addr >> ROW_SIZE_LOG2;

  /*
  fill in a parameter array of erased state for a 
  SROM_CMD_LOAD_LATCH command at the prescribed address
  */

  /* the erased state of the PSoC4 flash is all-zeros, rather than the more common all-ones */
  memset(&write_buffer[2], 0x00, ROW_SIZE);

  /* prepend the parameters */
  if (row > 0x1FFF)
    write_buffer[0] = SROM_KEY + (SROM_CMD_LOAD_LATCH << 8) | (0x01 << 24); /* flash macro 1 (used on larger memory parts) */
  else
    write_buffer[0] = SROM_KEY + (SROM_CMD_LOAD_LATCH << 8); /* flash macro 0 */
  write_buffer[1] = ROW_SIZE - 1; /* bytes, expressed as N-1, of entire row */

  return row;
}

static int libmem_erase_impl(libmem_driver_handle_t *h, uint8_t *start, size_t size, uint8_t **erased_start, size_t *erased_size)
{
  unsigned row, index;
  size_t count_size;

  if (!LIBMEM_ADDRESS_IN_RANGE(start, (uint8_t *)FLASH_START_ADDRESS, (uint8_t *)(FLASH_START_ADDRESS + FLASH_SIZE - 1)))
    return LIBMEM_STATUS_INVALID_RANGE;

  /*
  adjust start (and size) to reflect that we must start on a row boundary
  */

  index = (unsigned)start & (ROW_SIZE - 1);

  if (index)
  {
    start -= index;
    size += index;
  }

  if (erased_start)
    *erased_start = start;

  /*
  for each row in turn, use the SROM_CMD_WRITE_ROW command as an erase
  */

  for (count_size = 0; count_size < size; count_size += ROW_SIZE)
  {
    /* init write_buffer[] and get row number */
    row = init_write_buffer(start);

    for (index = 0; index < ROW_SIZE; index++)
      if (start[index])
        goto do_erase;
    
    goto skip;

do_erase:
    /* issue the load latch command */
    CPUSS_SYSARG = (uint32_t)write_buffer;
    CPUSS_SYSREQ = SROM_SYSREQ_BIT | SROM_CMD_LOAD_LATCH;

    if (psoc4_wait())
      return LIBMEM_STATUS_ERROR;

    /* set parameters for the write (which is being used as an erase) */
    write_buffer[0] = SROM_KEY + (SROM_CMD_WRITE_ROW << 8) + (row << 16);

    /* issue the write row command */
    CPUSS_SYSARG = (uint32_t)write_buffer;
    CPUSS_SYSREQ = SROM_SYSREQ_BIT | SROM_CMD_WRITE_ROW;

    if (psoc4_wait())
      return LIBMEM_STATUS_ERROR;

skip:
    start += ROW_SIZE;
  }

  if (erased_size)
    *erased_size = count_size;

  return LIBMEM_STATUS_SUCCESS;
}

static int libmem_write_impl(libmem_driver_handle_t *h, uint8_t *dest, const uint8_t *src, size_t size)
{
  unsigned row, index;
  size_t chunk_size;
#ifdef OPTION_CHECKSUM_WORKAROUND
  uint32_t checksum, backup;
  unsigned non_zero_entry, repeat;
#endif

  if (!LIBMEM_ADDRESS_IN_RANGE(dest, (uint8_t *)FLASH_START_ADDRESS, (uint8_t *)(FLASH_START_ADDRESS + FLASH_SIZE - 1)))
    return LIBMEM_STATUS_INVALID_RANGE;

  if (!LIBMEM_ADDRESS_IN_RANGE(dest + size, (uint8_t *)FLASH_START_ADDRESS, (uint8_t *)(FLASH_START_ADDRESS + FLASH_SIZE)))
    return LIBMEM_STATUS_INVALID_RANGE;

  while (size)
  {
    index = (unsigned)dest & (ROW_SIZE - 1);
    chunk_size = ROW_SIZE - index;
    if (size < chunk_size)
      chunk_size = size;

    /* init write_buffer[] and get row number */
    row = init_write_buffer(dest);

    /* overwrite erased state with intended data */
    memcpy((uint8_t *)&write_buffer[2] + index, src, chunk_size);

#ifdef OPTION_CHECKSUM_WORKAROUND
    /*
    part 1 of a workaround inferred from Cypress's patchy information
    */
    non_zero_entry = -1;

    checksum = 0;
    for (index = 0; index < (ROW_SIZE / sizeof(uint32_t)); index++)
    {
      backup = write_buffer[2 + index];
      checksum += backup;
      if (backup)
        non_zero_entry = index;
    }

    repeat = ( (0 == checksum) && (-1 != non_zero_entry) );
    if (repeat)
    {
      /* workaround by setting this word to its erased state, and then use a second program iteration to fix it */
      backup = write_buffer[2 + non_zero_entry];
      write_buffer[2 + non_zero_entry] = 0;
    }
redo: 
#endif

    /* issue the load latch command */
    CPUSS_SYSARG = (uint32_t)write_buffer;
    CPUSS_SYSREQ = SROM_SYSREQ_BIT | SROM_CMD_LOAD_LATCH;

    if (psoc4_wait())
      return LIBMEM_STATUS_ERROR;

    /* set parameters for the program */
    write_buffer[0] = SROM_KEY + (SROM_CMD_PROGRAM_ROW << 8) + (row << 16);

    /* issue the program row command (which is setting some bits from their erased state of 0 to 1) */
    CPUSS_SYSARG = (uint32_t)write_buffer;
    CPUSS_SYSREQ = SROM_SYSREQ_BIT | SROM_CMD_PROGRAM_ROW;

    if (psoc4_wait())
      return LIBMEM_STATUS_ERROR;

#ifdef OPTION_CHECKSUM_WORKAROUND
    /*
    part 2 of a workaround inferred from Cypress's patchy information
    */
    if (repeat)
    {
      write_buffer[2 + non_zero_entry] = backup;
      repeat = 0;
      goto redo;
    }
#endif

skip:
    size -= chunk_size;
    src += chunk_size;
    dest += chunk_size;
  }

  return LIBMEM_STATUS_SUCCESS;
}

static int libmem_flush_impl(libmem_driver_handle_t *h)
{
#ifdef OPTION_CLOCK_CONFIGURE
  /* restore clocks back to default (after flash programming) */
  CPUSS_SYSARG = SROM_KEY + (SROM_CMD_CLK_RESTORE << 8);
  CPUSS_SYSREQ = SROM_SYSREQ_BIT | SROM_CMD_CLK_RESTORE;

  psoc4_wait();

  CLK_IMO_CONFIG = imo_backup;
#endif

  return LIBMEM_STATUS_SUCCESS;
}

static int libmem_read_imp(libmem_driver_handle_t *h, uint8_t *dest, const uint8_t *src, size_t size)
{
  uint32_t backup_copy = CPUSS_SYSREQ;

  /* override remapping of flash */
  CPUSS_SYSREQ |= CPUSS_NO_RST_OVR;

  memcpy(dest, src, size);

  /* restore register CPUSS_NO_RST_OVR */
  CPUSS_SYSREQ = backup_copy;

  return LIBMEM_STATUS_SUCCESS;
}

static uint32_t libmem_crc32_impl(libmem_driver_handle_t *h, const uint8_t *start, size_t size, uint32_t crc)
{
  uint32_t backup_copy = CPUSS_SYSREQ;

  /* override remapping of flash */
  CPUSS_SYSREQ |= CPUSS_NO_RST_OVR;

  crc = libmem_crc32_direct(start, size, crc);

  /* restore register CPUSS_NO_RST_OVR */
  CPUSS_SYSREQ = backup_copy;

  return crc;
}

static const libmem_driver_functions_t driver_functions =
{       
  .write  = libmem_write_impl,
  .fill   = 0,
  .erase  = libmem_erase_impl,
  .lock   = 0,
  .unlock = 0,
  .flush  = libmem_flush_impl,
};

static const libmem_ext_driver_functions_t ext_driver_functions =
{
  .inrange = 0,
  .read    = libmem_read_imp,
  .crc32   = libmem_crc32_impl,
};

static unsigned psoc4_register_driver(libmem_driver_handle_t *h)
{
  unsigned chunks;

  /* there is a dedicated register field that indicates flash size */
  chunks = (SPCIF_GEOMETRY & 0xFFFF);

  /* each chunk is 256 bytes; (256 / ROW_SIZE) rows are in each chunk */
  FLASH_SIZE = 256 * chunks;
  geometry[0].count = (256 / ROW_SIZE) * chunks;
  geometry[0].size = ROW_SIZE;

#ifdef OPTION_CLOCK_CONFIGURE
  imo_backup = CLK_IMO_CONFIG;

  /* suitably configure the clocks for flash programming */
  CPUSS_SYSARG = SROM_KEY + (SROM_CMD_CLK_CONFIG << 8);
  CPUSS_SYSREQ = SROM_SYSREQ_BIT | SROM_CMD_CLK_CONFIG;

  psoc4_wait();
#endif

  /* the first 192 bytes of ROM may fail verification if VECT_IN_RAM is not cleared */
  CPUSS_CONFIG &= ~0x1;

  libmem_register_driver(h, (uint8_t *)FLASH_START_ADDRESS, FLASH_SIZE, geometry, 0, &driver_functions, &ext_driver_functions);

  return RAM_SIZE;
}

int main(unsigned long param0)
{
  libmem_driver_handle_t h;
  int res;
  unsigned ramsize = psoc4_register_driver(&h);
  res = libmem_rpc_loader_start(__RAM_segment_used_end__, __RAM_segment_start__ + ramsize - 1); 
  libmem_rpc_loader_exit(res, 0);
  return 0;
}

static unsigned psoc4_wait(void)
{
  while (CPUSS_SYSREQ & (SROM_SYSREQ_BIT | SROM_PRIVILEGED_BIT)) {}

  return (0xA0000000 == (0xF0000000 & CPUSS_SYSARG)) ? 0 : 1;
}
