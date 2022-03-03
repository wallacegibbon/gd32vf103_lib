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
	PMU_CTL &= ~PMU_CTL_LVDEN; // disable LVD
	PMU_CTL &= ~PMU_CTL_LVDT; // clear LVDT bits
	PMU_CTL |= lvdt_n; // set LVDT bits according to lvdt_n
	PMU_CTL |= PMU_CTL_LVDEN; // enable LVD
}

void pmu_lvd_disable() {
	PMU_CTL &= ~PMU_CTL_LVDEN;
}

static inline void call_wfe() {
	clear_csr(mstatus, MSTATUS_MIE);
	set_csr(0x810, 1);
	__WFI();
	clear_csr(0x810, 1);
	set_csr(mstatus, MSTATUS_MIE);
}

static inline void call_wfi_or_wfe_by_cmd(enum wfi_wfe_cmd sleepmodecmd,
		int is_deepsleep) {

	// set/clear CSR_SLEEPVALUE bit to select/unselect deepsleep or standby
	if (is_deepsleep)
		set_csr(0x811, 1);
	else
		clear_csr(0x811, 1);

	if (sleepmodecmd == WFI_CMD)
		__WFI();
	else
		call_wfe();

	if (is_deepsleep)
		clear_csr(0x811, 1);
}

void pmu_to_sleepmode(enum wfi_wfe_cmd sleepmodecmd) {
	call_wfi_or_wfe_by_cmd(sleepmodecmd, 0);
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

	call_wfi_or_wfe_by_cmd(deepsleepmodecmd, 1);
}

void pmu_to_standbymode(enum wfi_wfe_cmd standbymodecmd) {
	// set stbmod bit
	PMU_CTL |= PMU_CTL_STBMOD;
	// reset wakeup flag
	PMU_CTL |= PMU_CTL_WURST;

	call_wfi_or_wfe_by_cmd(standbymodecmd, 1);
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

