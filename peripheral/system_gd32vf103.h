#ifndef __SYSTEM_GD32VF103_H__
#define __SYSTEM_GD32VF103_H__

#include <stdint.h>

// system clock frequency (core clock)
extern uint32_t sys_core_clk;

// initialize the system and update the sys_core_clk variable
extern void system_init(void);

// update `sys_core_clk` with current core clock retrieved from cpu registers
extern void sys_core_clk_update(void);

#endif
