#include "gd32vf103_pmu.h"
#include "riscv_encoding.h"

void pmu_deinit() {
	rcu_periph_reset_enable(RCU_PMURST);
	rcu_periph_reset_disable(RCU_PMURST);
}

/*
 * select low voltage detector threshold
 * lvdt_n:
 * 	PMU_LVDT_0: voltage threshold is 2.2V
 * 	PMU_LVDT_1: voltage threshold is 2.3V
 * 	PMU_LVDT_2: voltage threshold is 2.4V
 * 	PMU_LVDT_3: voltage threshold is 2.5V
 * 	PMU_LVDT_4: voltage threshold is 2.6V
 * 	PMU_LVDT_5: voltage threshold is 2.7V
 * 	PMU_LVDT_6: voltage threshold is 2.8V
 * 	PMU_LVDT_7: voltage threshold is 2.9V
 */
void pmu_lvd_select(uint32_t lvdt_n) {
	// disable LVD
	PMU_CTL &= ~PMU_CTL_LVDEN;
	// clear LVDT bits
	PMU_CTL &= ~PMU_CTL_LVDT;
	// set LVDT bits according to lvdt_n
	PMU_CTL |= lvdt_n;
	// enable LVD
	PMU_CTL |= PMU_CTL_LVDEN;
}

void pmu_lvd_disable() {
	PMU_CTL &= ~PMU_CTL_LVDEN;
}

/*
 * PMU work at sleep mode
 * sleepmodecmd:
 * 	WFI_CMD: use WFI command
 * 	WFE_CMD: use WFE command
 */
void pmu_to_sleepmode(uint8_t sleepmodecmd) {
	// clear sleepdeep bit of RISC-V system control register
	clear_csr(0x811, 1);

	// select WFI or WFE command to enter sleep mode
	if (WFI_CMD == sleepmodecmd) {
		__WFI();
	} else {
		clear_csr(mstatus, MSTATUS_MIE);
		set_csr(0x810, 1);
		__WFI();
		clear_csr(0x810, 1);
		set_csr(mstatus, MSTATUS_MIE);
	}
}

/*
 * PMU work at deepsleep mode
 * ldo:
 * 	PMU_LDO_NORMAL:
 * 		LDO work at normal power mode when pmu enter
 * 		deepsleep mode
 * 	PMU_LDO_LOWPOWER:
 * 		LDO work at low power mode when pmu enter deepsleep mode
 * deepsleepmodecmd:
 * 	WFI_CMD: use WFI command
 * 	WFE_CMD: use WFE command
 */
void pmu_to_deepsleepmode(uint32_t ldo, uint8_t deepsleepmodecmd) {
	// clear stbmod and ldolp bits
	PMU_CTL &= ~((uint32_t) (PMU_CTL_STBMOD | PMU_CTL_LDOLP));
	// set ldolp bit according to pmu_ldo
	PMU_CTL |= ldo;
	// set CSR_SLEEPVALUE bit of RISC-V system control register
	set_csr(0x811, 1);
	// select WFI or WFE command to enter deepsleep mode
	if (WFI_CMD == deepsleepmodecmd) {
		__WFI();
	} else {
		clear_csr(mstatus, MSTATUS_MIE);
		set_csr(0x810, 1);
		__WFI();
		clear_csr(0x810, 1);
		set_csr(mstatus, MSTATUS_MIE);
	}
	// reset sleepdeep bit of RISC-V system control register
	clear_csr(0x811, 1);
}

/*
 * pmu work at standby mode
 * standbymodecmd:
 * 	WFI_CMD: use WFI command
 * 	WFE_CMD: use WFE command
 */
void pmu_to_standbymode(uint8_t standbymodecmd) {
	// set CSR_SLEEPVALUE bit of RISC-V system control register
	set_csr(0x811, 1);
	// set stbmod bit
	PMU_CTL |= PMU_CTL_STBMOD;
	// reset wakeup flag
	PMU_CTL |= PMU_CTL_WURST;
	// select WFI or WFE command to enter standby mode
	if (WFI_CMD == standbymodecmd) {
		__WFI();
	} else {
		clear_csr(mstatus, MSTATUS_MIE);
		set_csr(0x810, 1);
		__WFI();
		clear_csr(0x810, 1);
		set_csr(mstatus, MSTATUS_MIE);
	}
	clear_csr(0x811, 1);
}

void pmu_wakeup_pin_enable() {
	PMU_CS |= PMU_CS_WUPEN;
}

void pmu_wakeup_pin_disable() {
	PMU_CS &= ~PMU_CS_WUPEN;
}

/* enable write access to the registers in backup domain */
void pmu_backup_write_enable() {
	PMU_CTL |= PMU_CTL_BKPWEN;
}

/* disable write access to the registers in backup domain */
void pmu_backup_write_disable() {
	PMU_CTL &= ~PMU_CTL_BKPWEN;
}

/*
 * get flag state
 * flag:
 * 	PMU_FLAG_WAKEUP: wakeup flag
 * 	PMU_FLAG_STANDBY: standby flag
 * 	PMU_FLAG_LVD: lvd flag
 */
enum flag_status pmu_flag_get(uint32_t flag) {
	if (flag & PMU_CS)
		return SET;
	else
		return RESET;
}

/*
 * flag_reset:
 * 	PMU_FLAG_RESET_WAKEUP: reset wakeup flag
 * 	PMU_FLAG_RESET_STANDBY: reset standby flag
 */
void pmu_flag_clear(uint32_t flag_reset) {
	switch (flag_reset) {
	case PMU_FLAG_RESET_WAKEUP:
		PMU_CTL |= PMU_CTL_WURST;
		break;
	case PMU_FLAG_RESET_STANDBY:
		PMU_CTL |= PMU_CTL_STBRST;
		break;
	default:
		break;
	}
}
