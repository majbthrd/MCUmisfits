/*****************************************************************************
 * Copyright (c) 2018 Peter Lawrence                                         *
 *                                                                           *
 * THIS FILE IS PROVIDED AS IS WITH NO WARRANTY OF ANY KIND, INCLUDING THE   *
 * WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. *
 *****************************************************************************/

#include <libmem.h>
#include <libmem_loader.h>
#include <string.h>
#include <limits.h>
#include "m2sxxx.h"
#include "mss_nvm.h"

extern uint8_t __SRAM_segment_end__;
extern uint8_t __SRAM_segment_used_end__;

static int
libmem_write_impl(libmem_driver_handle_t *h, uint8_t *dest, const uint8_t *src, size_t size)
{
  nvm_status_t status;
        
  /* adjust remapped eNVM (FLASH) back to its native address */
  dest += 0x60000000 - (uint32_t)h->start;

  status = NVM_unlock((uint32_t)dest, size);
  
  if ( (NVM_SUCCESS != status) && (NVM_WRITE_THRESHOLD_WARNING != status) )
    return LIBMEM_STATUS_LOCKED;

  status = NVM_write((uint32_t)dest, src, size, NVM_DO_NOT_LOCK_PAGE);

  if ( (NVM_SUCCESS == status) || (NVM_WRITE_THRESHOLD_WARNING == status) )
    return LIBMEM_STATUS_SUCCESS;
  else
    return LIBMEM_STATUS_ERROR;
}

static int
libmem_erase_impl(libmem_driver_handle_t *h, uint8_t *start, size_t size, uint8_t **erase_start, size_t *erase_size)
{
  /* this follows the "If this operation is not required" description of libmem_driver_erase_fn_t in libmem.h */

  if (erase_start)
    *erase_start = start;
  if (erase_size)
    *erase_size = size;

  return LIBMEM_STATUS_SUCCESS;
}

static const libmem_driver_functions_t driver_functions =
{
  libmem_write_impl,
  0,
  libmem_erase_impl,
  0,
  0,
  0,
};

static const libmem_ext_driver_functions_t ext_driver_functions =
{
  0,
  0,
  0
};


static libmem_geometry_t geometry[] =
{
  { .count = 0, .size = 128 },
  { .count = 0, .size = 0 },
};

int
main(uint32_t flags, uint32_t param)
{
  size_t size = 0;
  unsigned int count = 0;
  uint32_t device_version;
  const char *error_string = 0;
  int res = LIBMEM_STATUS_SUCCESS;
  libmem_driver_handle_t flash_native_handle, flash_remap_handle;

  device_version = (SYSREG->DEVICE_VERSION & 0xFFFFu);

  switch (device_version)
  {
  case 0xF805u: /* 005 */
    size = 131072;
    count = 1024;
    break;
  case 0xF802u: /* 050 */
  case 0xF803u: /* 010 */
  case 0xF804u: /* 025 */
  case 0xF808u: /* 060 */
    size = 262144;
    count = 2048;
    break;
  case 0xF806u: /* 150 */
  case 0xF807u: /* 090 */
    size = 524288;
    count = 4096;
    break;
  }

  geometry[0].count = count;

  if (size)
  {
    libmem_register_driver(&flash_remap_handle,  (uint8_t *)0x00000000, size, geometry, 0, &driver_functions, &ext_driver_functions);  
    libmem_register_driver(&flash_native_handle, (uint8_t *)0x60000000, size, geometry, 0, &driver_functions, &ext_driver_functions);  

    /* Run the loader */
    libmem_rpc_loader_start(&__SRAM_segment_used_end__, &__SRAM_segment_end__ -1);
  }
  else
  {
    res = LIBMEM_STATUS_ERROR;
    error_string = "unknown target device";
  }

  libmem_rpc_loader_exit(res, error_string);
  return 0;
}
