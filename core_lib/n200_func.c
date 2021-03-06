#include <gd32vf103.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "riscv_encoding.h"
#include "n200_func.h"

// Configure PMP to make all the address space accesable and executable
void pmp_open_all_space() {
	// Config entry0 addr to all 1s to make the range cover all space
	asm volatile ("li x6, 0xffffffff":::"x6");
	asm volatile ("csrw pmpaddr0, x6":::);
	// Config entry0 cfg to make it NAPOT address mode, and R/W/X okay
	asm volatile ("li x6, 0x7f":::"x6");
	asm volatile ("csrw pmpcfg0, x6":::);
}

void switch_m2u_mode() {
	clear_csr(mstatus, MSTATUS_MPP);
	asm volatile ("la x6, 0x1f":::"x6");
	asm volatile ("csrw mepc, x6":::);
	asm volatile ("mret":::);
	asm volatile ("1:":::);
}

uint32_t mtime_lo(void) {
	return *(volatile uint32_t *) (TIMER_CTRL_ADDR + TIMER_MTIME);
}

uint32_t mtime_hi(void) {
	return *(volatile uint32_t *) (TIMER_CTRL_ADDR + TIMER_MTIME + 4);
}

uint64_t get_timer_value() {
	while (1) {
		uint32_t hi = mtime_hi();
		uint32_t lo = mtime_lo();
		if (hi == mtime_hi())
			return ((uint64_t) hi << 32) | lo;
	}
}

uint32_t get_timer_freq() {
	return TIMER_FREQ;
}

uint64_t get_instret_value() {
	while (1) {
		uint32_t hi = read_csr(minstreth);
		uint32_t lo = read_csr(minstret);
		if (hi == read_csr(minstreth))
			return ((uint64_t) hi << 32) | lo;
	}
}

uint64_t get_cycle_value() {
	while (1) {
		uint32_t hi = read_csr(mcycleh);
		uint32_t lo = read_csr(mcycle);
		if (hi == read_csr(mcycleh))
			return ((uint64_t) hi << 32) | lo;
	}
}

uint32_t __attribute__((noinline)) measure_cpu_freq(size_t n) {
	uint32_t start_mtime, delta_mtime;
	uint32_t mtime_freq = get_timer_freq();

	// Don't start measuring until we see an mtime tick
	uint32_t tmp = mtime_lo();
	do {
		start_mtime = mtime_lo();
	} while (start_mtime == tmp);

	uint32_t start_mcycle = read_csr(mcycle);

	do {
		delta_mtime = mtime_lo() - start_mtime;
	} while (delta_mtime < n);

	uint32_t delta_mcycle = read_csr(mcycle) - start_mcycle;

	return (delta_mcycle / delta_mtime) * mtime_freq +
		((delta_mcycle % delta_mtime) * mtime_freq) / delta_mtime;
}

uint32_t get_cpu_freq() {
	uint32_t cpu_freq;

	// warm up
	measure_cpu_freq(1);
	// measure for real
	cpu_freq = measure_cpu_freq(100);

	return cpu_freq;
}

// Note that there are no assertions or bounds checking on these
// parameter values.


#define REG32(addr)	(* (volatile uint32_t *) (uintptr_t) (addr))
#define REG16(addr)	(* (volatile uint16_t *) (uintptr_t) (addr))
#define REG8(addr)	(* (volatile uint8_t *) (uintptr_t) (addr))

#define ECLIC_INT_IP_REG(offset) \
	REG8(ECLIC_ADDR_BASE + ECLIC_INT_IP_OFFSET + (offset) * 4)

#define ECLIC_INT_IE_REG(offset) \
	REG8(ECLIC_ADDR_BASE + ECLIC_INT_IE_OFFSET + (offset) * 4)

#define ECLIC_INT_ATTR_REG(offset) \
	REG8(ECLIC_ADDR_BASE + ECLIC_INT_ATTR_OFFSET + (offset) * 4)

#define ECLIC_INT_CTRL_REG(offset) \
	REG8(ECLIC_ADDR_BASE + ECLIC_INT_CTRL_OFFSET + (offset) * 4)

#define ECLIC_CFG_REG	REG8(ECLIC_ADDR_BASE + ECLIC_CFG_OFFSET)
#define ECLIC_MTH_REG	REG8(ECLIC_ADDR_BASE + ECLIC_MTH_OFFSET)

void eclic_init(uint32_t num_irq) {
	//clear cfg register and minthresh register
	ECLIC_CFG_REG = 0;
	ECLIC_MTH_REG = 0;

	//clear all IP/IE/ATTR/CTRL bits for all interrupt sources
	volatile uint32_t *p = (volatile uint32_t *) &ECLIC_INT_IP_REG(0);
	volatile uint32_t *upper = (volatile uint32_t *) (p + num_irq * 4);

	while (p < upper)
		*p++ = 0;
}

//
void eclic_enable_interrupt(uint32_t source) {
	ECLIC_INT_IE_REG(source) = 1;
}

void eclic_disable_interrupt(uint32_t source) {
	ECLIC_INT_IE_REG(source) = 0;
}

void eclic_set_pending(uint32_t source) {
	ECLIC_INT_IP_REG(source) = 1;
}

void eclic_clear_pending(uint32_t source) {
	ECLIC_INT_IP_REG(source) = 0;
}

void eclic_set_intctrl(uint32_t source, uint8_t intctrl) {
	ECLIC_INT_CTRL_REG(source) = intctrl;
}

uint8_t eclic_get_intctrl(uint32_t source) {
	return ECLIC_INT_CTRL_REG(source);
}

void eclic_set_intattr(uint32_t source, uint8_t intattr) {
	ECLIC_INT_ATTR_REG(source) = intattr;
}

uint8_t eclic_get_intattr(uint32_t source) {
	return ECLIC_INT_ATTR_REG(source);
}

//
void eclic_set_cliccfg(uint8_t cliccfg) {
	ECLIC_CFG_REG = cliccfg;
}

uint8_t eclic_get_cliccfg() {
	return ECLIC_CFG_REG;
}

void eclic_set_mth(uint8_t mth) {
	ECLIC_MTH_REG = mth;
}

uint8_t eclic_get_mth() {
	return ECLIC_MTH_REG;
}

void eclic_set_nlbits(uint8_t nlbits) {
	//shift nlbits to correct position
	uint8_t nlbits_shifted = nlbits << ECLIC_CFG_NLBITS_LSB;

	uint8_t old_cliccfg = eclic_get_cliccfg();
	uint8_t new_cliccfg =
		(old_cliccfg & ~ECLIC_CFG_NLBITS_MASK) |
		(ECLIC_CFG_NLBITS_MASK & nlbits_shifted);

	eclic_set_cliccfg(new_cliccfg);
}

//get nlbits
uint8_t eclic_get_nlbits(void) {
	uint8_t nlbits = eclic_get_cliccfg();
	nlbits = (nlbits & ECLIC_CFG_NLBITS_MASK) >> ECLIC_CFG_NLBITS_LSB;
	return nlbits;
}

//sets an interrupt level based encoding of nlbits and ECLICINTCTLBITS
void eclic_set_irq_lvl(uint32_t source, uint8_t lvl) {
	uint8_t nlbits = eclic_get_nlbits();
	if (nlbits > ECLICINTCTLBITS)
		nlbits = ECLICINTCTLBITS;

	// mask off unused bits
	lvl >>= (8 - nlbits);
	lvl <<= (8 - nlbits);

	uint8_t current_intctrl = eclic_get_intctrl(source);
	// mask off unused bits
	current_intctrl <<= nlbits;
	current_intctrl >>= nlbits;

	eclic_set_intctrl(source, (current_intctrl | lvl));
}

//gets an interrupt level based encoding of nlbits
uint8_t eclic_get_irq_lvl(uint32_t source) {
	uint8_t nlbits = eclic_get_nlbits();
	if (nlbits > ECLICINTCTLBITS)
		nlbits = ECLICINTCTLBITS;

	uint8_t intctrl = eclic_get_intctrl(source);
	intctrl >>= (8 - nlbits);
	return intctrl << (8 - nlbits);
}

void eclic_set_irq_lvl_abs(uint32_t source, uint8_t lvl_abs) {
	uint8_t nlbits = eclic_get_nlbits();
	if (nlbits > ECLICINTCTLBITS)
		nlbits = ECLICINTCTLBITS;

	uint8_t lvl = lvl_abs << (8 - nlbits);

	uint8_t current_intctrl = eclic_get_intctrl(source);

	// mask off unused bits
	current_intctrl <<= nlbits;
	current_intctrl >>= nlbits;

	eclic_set_intctrl(source, current_intctrl | lvl);
}

uint8_t eclic_get_irq_lvl_abs(uint32_t source) {
	uint8_t nlbits = eclic_get_nlbits();
	if (nlbits > ECLICINTCTLBITS)
		nlbits = ECLICINTCTLBITS;

	uint8_t intctrl = eclic_get_intctrl(source);
	return intctrl >> (8 - nlbits);
}

//sets an interrupt priority based encoding of nlbits and ECLICINTCTLBITS
uint8_t eclic_set_irq_priority(uint32_t source, uint8_t priority) {
	uint8_t nlbits = eclic_get_nlbits();
	if (nlbits >= ECLICINTCTLBITS) {
		nlbits = ECLICINTCTLBITS;
		return 0;
	}

	//shift priority into correct bit position
	priority = priority << (8 - ECLICINTCTLBITS);

	//write to eclicintctrl
	uint8_t current_intctrl = eclic_get_intctrl(source);
	// mask off unused bits
	current_intctrl >>= (8 - nlbits);
	current_intctrl <<= (8 - nlbits);

	eclic_set_intctrl(source, (current_intctrl | priority));

	return priority;
}

//gets an interrupt priority based encoding of nlbits
uint8_t eclic_get_irq_priority(uint32_t source) {
	uint8_t nlbits = eclic_get_nlbits();
	if (nlbits > ECLICINTCTLBITS)
		nlbits = ECLICINTCTLBITS;

	uint8_t intctrl = eclic_get_intctrl(source);
	intctrl <<= nlbits;
	return intctrl >> (nlbits + (8 - ECLICINTCTLBITS));
}

void eclic_mode_enable() {
	uint32_t mtvec_value = read_csr(mtvec);
	// set bit1 and bit0
	write_csr(mtvec, mtvec_value | 3);
}

//sets vector-mode or non-vector mode
void eclic_set_vmode(uint32_t source) {
	uint8_t intattr = eclic_get_intattr(source);
	eclic_set_intattr(source, intattr | 1);
}

void eclic_set_nonvmode(uint32_t source) {
	uint8_t intattr = eclic_get_intattr(source);
	eclic_set_intattr(source, intattr & ~1);
}

//sets interrupt as level sensitive
//Bit 1, trig[0]: "edge-triggered" (0: level-triggered, 1: edge-triggered);
//Bit 2, trig[1]: "negative-edge" (0: positive-edge, 1: negative-edge).

void eclic_set_level_trig(uint32_t source) {
	uint8_t intattr = eclic_get_intattr(source);
	// clear bit1
	eclic_set_intattr(source, intattr & ~2);
}

void eclic_set_posedge_trig(uint32_t source) {
	uint8_t intattr = eclic_get_intattr(source);
	// set bit1 and clear bit2
	eclic_set_intattr(source, intattr & ~4 | 2);
}

void eclic_set_negedge_trig(uint32_t source) {
	uint8_t intattr = eclic_get_intattr(source);
	// set bit2 and bit1
	eclic_set_intattr(source, intattr | 6);
}

__attribute__((weak))
uintptr_t core_exception_handler(uintptr_t mcause, uintptr_t sp) {
	while (1);
}

