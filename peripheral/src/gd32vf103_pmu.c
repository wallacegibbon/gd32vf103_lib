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
	// disable LVD, clear LVDT
	PMU_CTL &= ~(PMU_CTL_LVDEN | PMU_CTL_LVDT);

	// set LVDT bits according to lvdt_n and enable LVD
	PMU_CTL |= PMU_CTL_LVDEN | lvdt_n;
}

void pmu_lvd_disable() {
	PMU_CTL &= ~PMU_CTL_LVDEN;
}

static inline void wfe_safe() {
	clear_csr(mstatus, MSTATUS_MIE);
	__WFE();
	set_csr(mstatus, MSTATUS_MIE);
}

static inline void call_wfi_or_wfe(enum wfi_wfe_cmd sleepmodecmd) {
	if (sleepmodecmd == WFI_CMD)
		__WFI();
	else
		wfe_safe();
}

void pmu_to_sleepmode(enum wfi_wfe_cmd sleepmodecmd) {
	// clear CSR_SLEEPVALUE bit to disable deepsleep or standby
	clear_csr(0x811, 1);
	call_wfi_or_wfe(sleepmodecmd);
}

/*
 * ldo:
 * 	PMU_LDO_NORMAL:
 * 		LDO work at normal power mode when pmu enter deepsleep mode
 * 	PMU_LDO_LOWPOWER:
 * 		LDO work at low power mode when pmu enter deepsleep mode
 */
void pmu_to_deepsleepmode(uint32_t ldo, enum wfi_wfe_cmd deepsleepmodecmd) {
	// clear stbmod and ldolp bits
	PMU_CTL &= ~(PMU_CTL_STBMOD | PMU_CTL_LDOLP);
	// set ldolp bit according to pmu_ldo
	PMU_CTL |= ldo;

	set_csr(0x811, 1);
	call_wfi_or_wfe(deepsleepmodecmd);
	clear_csr(0x811, 1);
}

void pmu_to_standbymode(enum wfi_wfe_cmd standbymodecmd) {
	// set stbmod bit
	PMU_CTL |= PMU_CTL_STBMOD;
	// reset wakeup flag
	PMU_CTL |= PMU_CTL_WURST;

	set_csr(0x811, 1);
	call_wfi_or_wfe(standbymodecmd);
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

void pmu_flag_clear(enum pmu_flag_reset flag_reset) {
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

