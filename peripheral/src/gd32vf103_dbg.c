#include "gd32vf103_dbg.h"

/* read DBG_ID code register */
uint32_t dbg_id_get(void) {
	return DBG_ID;
}

/*
 * enable low power behavior when the mcu is in debug mode
 * dbg_low_power:
 * 	one or more parameters can be selected which are shown as below:
 * 		DBG_LOW_POWER_SLEEP:
 * 			keep debugger connection during sleep mode
 * 		DBG_LOW_POWER_DEEPSLEEP:
 * 			keep debugger connection during deepsleep mode
 * 		DBG_LOW_POWER_STANDBY:
 * 			keep debugger connection during standby mode
 */
void dbg_low_power_enable(uint32_t dbg_low_power) {
	DBG_CTL |= dbg_low_power;
}

/*
 * disable low power behavior when the mcu is in debug mode
 * dbg_low_power:
 * 	one or more parameters can be selected which are shown as below:
 * 		DBG_LOW_POWER_SLEEP:
 * 			donot keep debugger connection during sleep mode
 * 		DBG_LOW_POWER_DEEPSLEEP:
 * 			donot keep debugger connection during deepsleep mode
 * 		DBG_LOW_POWER_STANDBY:
 * 			donot keep debugger connection during standby mode
 *
 */
void dbg_low_power_disable(uint32_t dbg_low_power) {
	DBG_CTL &= ~dbg_low_power;
}

/*
 * enable peripheral behavior when the mcu is in debug mode
 * dbg_periph: refer to enum dbg_periph
 * 	one or more parameters can be selected which are shown as below:
 * 		DBG_FWDGT_HOLD :
 * 			debug FWDGT kept when core is halted
 * 		DBG_WWDGT_HOLD :
 * 			debug WWDGT kept when core is halted
 * 		DBG_CANx_HOLD (x=0,1):
 * 			hold CANx counter when core is halted
 * 		DBG_I2Cx_HOLD (x=0,1):
 * 			hold I2Cx smbus when core is halted
 * 		DBG_TIMERx_HOLD (x=0,1,2,3,4,5,6):
 * 			hold TIMERx counter when core is halted
 */
void dbg_periph_enable(enum dbg_periph dbg_periph) {
	DBG_CTL |= (uint32_t) dbg_periph;
}

/*
 * disable peripheral behavior when the mcu is in debug mode
 * dbg_periph: refer to enum dbg_periph
 * 	one or more parameters can be selected which are shown as below:
 * 		DBG_FWDGT_HOLD :
 * 			debug FWDGT kept when core is halted
 * 		DBG_WWDGT_HOLD :
 * 			debug WWDGT kept when core is halted
 * 		DBG_CANx_HOLD (x=0,1):
 * 			hold CAN0 counter when core is halted
 * 		DBG_I2Cx_HOLD (x=0,1):
 * 			hold I2Cx smbus when core is halted
 * 		DBG_TIMERx_HOLD (x=0,1,2,3,4,5,6):
 * 			hold TIMERx counter when core is halted
 */
void dbg_periph_disable(enum dbg_periph dbg_periph) {
	DBG_CTL &= ~(uint32_t) dbg_periph;
}
