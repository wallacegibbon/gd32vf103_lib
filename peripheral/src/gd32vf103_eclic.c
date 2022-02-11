#include "gd32vf103_eclic.h"
#include "riscv_encoding.h"

#define REG_DBGMCU2		((uint32_t) 0xE0042008)
#define REG_DBGMCU2EN		((uint32_t) 0xE004200C)

void eclic_global_interrupt_enable() {
	// set machine interrupt enable bit
	set_csr(mstatus, MSTATUS_MIE);
}

void eclic_global_interrupt_disable() {
	// clear machine interrupt enable bit
	clear_csr(mstatus, MSTATUS_MIE);
}

/*
 * set the priority group
 * prigroup: specify the priority group
 * 	ECLIC_PRIGROUP_LEVEL0_PRIO4
 * 	ECLIC_PRIGROUP_LEVEL1_PRIO3
 * 	ECLIC_PRIGROUP_LEVEL2_PRIO2
 * 	ECLIC_PRIGROUP_LEVEL3_PRIO1
 * 	ECLIC_PRIGROUP_LEVEL4_PRIO0
 */
void eclic_priority_group_set(uint32_t prigroup) {
	eclic_set_nlbits(prigroup);
}

/*
 * enable the interrupt request
 * source: interrupt request, detailed in `enum IRQn`
 *
 * level: the level needed to set (maximum is 15, refer to the priority group)
 *
 * priority: the priority needed to set
 * 		(maximum is 15, refer to the priority group)
*/
void eclic_irq_enable(uint32_t source, uint8_t level, uint8_t priority) {
	eclic_enable_interrupt(source);
	eclic_set_irq_lvl_abs(source, level);
	eclic_set_irq_priority(source, priority);
}

void eclic_irq_disable(uint32_t source) {
	eclic_disable_interrupt(source);
}

void eclic_system_reset() {
	REG32(REG_DBGMCU2EN) = 0x4b5a6978;
	REG32(REG_DBGMCU2) = 0x1;
}

void eclic_send_event() {
	set_csr(0x812, 1);
}
