#ifndef __SYSTEM_PSOC4_H__
#define __SYSTEM_PSOC4_H__

#ifdef __cplusplus
 extern "C" {
#endif 

#include <stdint.h>
	 
extern uint32_t SystemCoreClock;

extern void SystemInit(void);

extern void SystemCoreClockUpdate(void);

#ifdef __cplusplus
}
#endif

#endif
