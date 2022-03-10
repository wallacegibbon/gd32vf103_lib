#ifndef __N200_FUNC_H
#define __N200_FUNC_H

#include "riscv_encoding.h"
#include "n200_timer.h"
#include "n200_eclic.h"
#include <stddef.h>

void eclic_init(uint32_t num_irq);

void eclic_enable_interrupt(uint32_t source);
void eclic_disable_interrupt(uint32_t source);

void eclic_set_pending(uint32_t source);
void eclic_clear_pending(uint32_t source);

void eclic_set_intctrl(uint32_t source, uint8_t intctrl);
uint8_t eclic_get_intctrl(uint32_t source);

void eclic_set_intattr(uint32_t source, uint8_t intattr);
uint8_t eclic_get_intattr(uint32_t source);

void eclic_set_cliccfg(uint8_t cliccfg);
uint8_t eclic_get_cliccfg();

void eclic_set_mth(uint8_t mth);
uint8_t eclic_get_mth();

void eclic_set_nlbits(uint8_t nlbits);
uint8_t eclic_get_nlbits();

void eclic_set_irq_lvl(uint32_t source, uint8_t lvl);
uint8_t eclic_get_irq_lvl(uint32_t source);

void eclic_set_irq_lvl_abs(uint32_t source, uint8_t lvl_abs);
uint8_t eclic_get_irq_lvl_abs(uint32_t source);

uint8_t eclic_set_irq_priority(uint32_t source, uint8_t priority);
uint8_t eclic_get_irq_priority(uint32_t source);

void eclic_mode_enable();

void eclic_set_vmode(uint32_t source);
void eclic_set_nonvmode(uint32_t source);

void eclic_set_level_trig(uint32_t source);

void eclic_set_posedge_trig(uint32_t source);

void eclic_set_negedge_trig(uint32_t source);

// some util functions for eclic
static inline void eclic_irq_enable(uint32_t source, uint8_t level,
		uint8_t priority) {
	eclic_enable_interrupt(source);
	eclic_set_irq_lvl_abs(source, level);
	eclic_set_irq_priority(source, priority);
}

static inline void eclic_irq_disable(uint32_t source) {
	eclic_disable_interrupt(source);
}

enum eclic_priority_group_val {
	// 0 bits for level, 4 bits for priority
	ECLIC_PRIGROUP_LEVEL0_PRIO4 = 0,
	// 1 bits for level, 3 bits for priority
	ECLIC_PRIGROUP_LEVEL1_PRIO3 = 1,
	ECLIC_PRIGROUP_LEVEL2_PRIO2 = 2,
	ECLIC_PRIGROUP_LEVEL3_PRIO1 = 3,
	ECLIC_PRIGROUP_LEVEL4_PRIO0 = 4,
};

static inline void eclic_priority_group_set(
		enum eclic_priority_group_val prigroup) {
	eclic_set_nlbits(prigroup);
}

#define REG_DBGMCU2		((uint32_t) 0xE0042008)
#define REG_DBGMCU2EN		((uint32_t) 0xE004200C)

static inline void eclic_system_reset() {
	REG32(REG_DBGMCU2EN) = 0x4b5a6978;
	REG32(REG_DBGMCU2) = 1;
}

static inline void eclic_global_interrupt_enable() {
	set_csr(mstatus, MSTATUS_MIE);
}

static inline void eclic_global_interrupt_disable() {
	clear_csr(mstatus, MSTATUS_MIE);
}

__attribute__((always_inline)) static inline void __WFI() {
	asm volatile ("wfi");
}

__attribute__((always_inline)) static inline void __WFE() {
	set_csr(0x810, 1);
	asm volatile ("wfi");
	clear_csr(0x810, 1);
}

__attribute__((always_inline)) static inline void __SEV() {
	set_csr(0x812, 1);
}

void pmp_open_all_space();

void switch_m2u_mode();

uint64_t get_timer_value();

uint32_t get_mtime_freq();

uint32_t mtime_lo(void);
uint32_t mtime_hi(void);

uint64_t get_mtime_value();

uint64_t get_instret_value();

uint64_t get_cycle_value();

uint32_t get_cpu_freq();

uint32_t __attribute__((noinline)) measure_cpu_freq(size_t n);

#endif

