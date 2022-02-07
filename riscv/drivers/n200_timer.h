#ifndef __N200_TIMER_H__
#define __N200_TIMER_H__

#define TIMER_MSIP		0xFFC
#define TIMER_MSIP_size		0x4
#define TIMER_MTIMECMP		0x8
#define TIMER_MTIMECMP_size	0x8
#define TIMER_MTIME		0x0
#define TIMER_MTIME_size	0x8

#define TIMER_CTRL_ADDR		0xd1000000
#define TIMER_REG(offset)	_REG32(TIMER_CTRL_ADDR, offset)
#define TIMER_FREQ		((uint32_t) sys_core_clk / 4) //units HZ

#endif