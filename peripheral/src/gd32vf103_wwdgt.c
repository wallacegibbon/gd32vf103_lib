#include "gd32vf103_wwdgt.h"

// write value to WWDGT_CTL_CNT bit field
#define CTL_CNT(regval)		(BITS(0, 6) & ((uint32_t) (regval) << 0))

// write value to WWDGT_CFG_WIN bit field
#define CFG_WIN(regval)		(BITS(0, 6) & ((uint32_t) (regval) << 0))

// reset the window watchdog timer configuration
void wwdgt_deinit() {
	rcu_periph_reset_enable(RCU_WWDGTRST);
	rcu_periph_reset_disable(RCU_WWDGTRST);
}

/* start the window watchdog timer counter */
void wwdgt_enable() {
	WWDGT_CTL |= WWDGT_CTL_WDGTEN;
}

/*
 * configure the window watchdog timer counter value
 * counter_value: 0x00 - 0x7F
 */
void wwdgt_counter_update(uint16_t counter_value) {
	uint32_t reg = 0U;

	reg = (WWDGT_CTL & (~WWDGT_CTL_CNT));
	reg |= CTL_CNT(counter_value);

	WWDGT_CTL = reg;
}

/*
 * configure counter value, window value, and prescaler divider value
 * counter: 0x00 - 0x7F
 * window: 0x00 - 0x7F
 * prescaler: wwdgt prescaler value
 * 	only one parameter can be selected which is shown as below:
 * 		WWDGT_CFG_PSC_DIV1:
 * 			the time base of window watchdog counter =
 * 				(PCLK1/4096)/1
 *
 * 		WWDGT_CFG_PSC_DIV2:
 * 			the time base of window watchdog counter =
 * 				(PCLK1/4096)/2
 *
 * 		WWDGT_CFG_PSC_DIV4:
 * 			the time base of window watchdog counter =
 * 				(PCLK1/4096)/4
 *
 * 		WWDGT_CFG_PSC_DIV8:
 * 			the time base of window watchdog counter =
 * 				(PCLK1/4096)/8
 */
void wwdgt_config(uint16_t counter, uint16_t window, uint32_t prescaler) {
	// clear WIN and PSC bits, clear CNT bit
	uint32_t reg_cfg = WWDGT_CFG & ~(WWDGT_CFG_WIN | WWDGT_CFG_PSC);
	uint32_t reg_ctl = WWDGT_CTL & ~WWDGT_CTL_CNT;

	// configure WIN and PSC bits, configure CNT bit
	reg_cfg |= CFG_WIN(window);
	reg_cfg |= prescaler;
	reg_ctl |= CTL_CNT(counter);

	WWDGT_CTL = reg_ctl;
	WWDGT_CFG = reg_cfg;
}

/* enable early wakeup interrupt of WWDGT */
void wwdgt_interrupt_enable() {
	WWDGT_CFG |= WWDGT_CFG_EWIE;
}

/* check early wakeup interrupt state of WWDGT */
enum flag_status wwdgt_flag_get() {
	if (WWDGT_STAT & WWDGT_STAT_EWIF)
		return SET;
	else
		return RESET;
}

/* early wakeup interrupt state of WWDGT */
void wwdgt_flag_clear() {
	WWDGT_STAT &= ~WWDGT_STAT_EWIF;
}
