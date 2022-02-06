/* This file refers the RISC-V standard, some adjustments are made according to GigaDevice chips */

#ifndef SYSTEM_GD32VF103_H
#define SYSTEM_GD32VF103_H

#include <stdint.h>

/* system clock frequency (core clock) */
extern uint32_t SystemCoreClock;

/* initialize the system and update the SystemCoreClock variable */
extern void SystemInit(void);

/* update the SystemCoreClock with current core clock retrieved from cpu registers */
extern void SystemCoreClockUpdate(void);

#endif
