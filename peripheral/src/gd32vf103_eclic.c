#include "gd32vf103_eclic.h"
#include "riscv_encoding.h"

#define REG_DBGMCU2       ((uint32_t)0xE0042008)
#define REG_DBGMCU2EN     ((uint32_t)0xE004200C)

/*!
    \brief      enable the global interrupt
    \param[in]  none
    \param[out] none
    \retval     none
*/
void eclic_global_interrupt_enable(void) {
	/* set machine interrupt enable bit */
	set_csr(mstatus, MSTATUS_MIE);
}

/*!
    \brief      disable the global interrupt
    \param[in]  none
    \param[out] none
    \retval     none
*/
void eclic_global_interrupt_disable(void) {
	/* clear machine interrupt enable bit */
	clear_csr(mstatus, MSTATUS_MIE);
}

/*!
    \brief      set the priority group
    \param[in]  prigroup: specify the priority group
      \arg        ECLIC_PRIGROUP_LEVEL0_PRIO4
      \arg        ECLIC_PRIGROUP_LEVEL1_PRIO3
      \arg        ECLIC_PRIGROUP_LEVEL2_PRIO2
      \arg        ECLIC_PRIGROUP_LEVEL3_PRIO1
      \arg        ECLIC_PRIGROUP_LEVEL4_PRIO0
    \param[out] none
    \retval     none
*/
void eclic_priority_group_set(uint32_t prigroup) {
	eclic_set_nlbits(prigroup);
}

/*!
    \brief      enable the interrupt request
    \param[in]  source: interrupt request, detailed in `enum IRQn`
    \param[in]  level: the level needed to set (maximum is 15, refer to the priority group)
    \param[in]  priority: the priority needed to set (maximum is 15, refer to the priority group)
    \param[out] none
    \retval     none
*/
void eclic_irq_enable(uint32_t source, uint8_t level, uint8_t priority) {
	eclic_enable_interrupt(source);
	eclic_set_irq_lvl_abs(source, level);
	eclic_set_irq_priority(source, priority);
}

/*!
    \brief      disable the interrupt request
    \param[in]  source: interrupt request, detailed in `enum IRQn`
    \param[out] none
    \retval     none
*/
void eclic_irq_disable(uint32_t source) {
	eclic_disable_interrupt(source);
}

/*!
    \brief      reset system
    \param[in]  none
    \param[out] none
    \retval     none
*/
void eclic_system_reset(void) {
	REG32(REG_DBGMCU2EN) = 0x4b5a6978;
	REG32(REG_DBGMCU2) = 0x1;
}

/*!
    \brief      send event(SEV)
    \param[in]  none
    \param[out] none
    \retval     none
*/
void eclic_send_event(void) {
	set_csr(0x812, 0x1);
}
