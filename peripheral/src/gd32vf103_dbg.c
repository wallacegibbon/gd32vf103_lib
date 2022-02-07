#include "gd32vf103_dbg.h"

/*!
    \brief      read DBG_ID code register
    \param[in]  none
    \param[out] none
    \retval     DBG_ID code
*/
uint32_t dbg_id_get(void) {
	return DBG_ID;
}

/*!
    \brief      enable low power behavior when the mcu is in debug mode
    \param[in]  dbg_low_power:
                one or more parameters can be selected which are shown as below:
      \arg        DBG_LOW_POWER_SLEEP: keep debugger connection during sleep mode
      \arg        DBG_LOW_POWER_DEEPSLEEP: keep debugger connection during deepsleep mode
      \arg        DBG_LOW_POWER_STANDBY: keep debugger connection during standby mode
    \param[out] none
    \retval     none
*/
void dbg_low_power_enable(uint32_t dbg_low_power) {
	DBG_CTL |= dbg_low_power;
}

/*!
    \brief      disable low power behavior when the mcu is in debug mode
    \param[in]  dbg_low_power:
                one or more parameters can be selected which are shown as below:
      \arg        DBG_LOW_POWER_SLEEP: donot keep debugger connection during sleep mode
      \arg        DBG_LOW_POWER_DEEPSLEEP: donot keep debugger connection during deepsleep mode
      \arg        DBG_LOW_POWER_STANDBY: donot keep debugger connection during standby mode
    \param[out] none
    \retval     none
*/
void dbg_low_power_disable(uint32_t dbg_low_power) {
	DBG_CTL &= ~dbg_low_power;
}

/*!
    \brief      enable peripheral behavior when the mcu is in debug mode
    \param[in]  dbg_periph: refer to enum dbg_periph
                one or more parameters can be selected which are shown as below:
      \arg        DBG_FWDGT_HOLD : debug FWDGT kept when core is halted
      \arg        DBG_WWDGT_HOLD : debug WWDGT kept when core is halted
      \arg        DBG_CANx_HOLD (x=0,1): hold CANx counter when core is halted
      \arg        DBG_I2Cx_HOLD (x=0,1): hold I2Cx smbus when core is halted
      \arg        DBG_TIMERx_HOLD (x=0,1,2,3,4,5,6): hold TIMERx counter when core is halted
    \param[out] none
    \retval     none
*/
void dbg_periph_enable(enum dbg_periph dbg_periph) {
	DBG_CTL |= (uint32_t) dbg_periph;
}

/*!
    \brief      disable peripheral behavior when the mcu is in debug mode
    \param[in]  dbg_periph: refer to enum dbg_periph
                one or more parameters can be selected which are shown as below:
      \arg        DBG_FWDGT_HOLD : debug FWDGT kept when core is halted
      \arg        DBG_WWDGT_HOLD : debug WWDGT kept when core is halted
      \arg        DBG_CANx_HOLD (x=0,1): hold CAN0 counter when core is halted
      \arg        DBG_I2Cx_HOLD (x=0,1): hold I2Cx smbus when core is halted
      \arg        DBG_TIMERx_HOLD (x=0,1,2,3,4,5,6): hold TIMERx counter when core is halted
    \param[out] none
    \retval     none
*/
void dbg_periph_disable(enum dbg_periph dbg_periph) {
	DBG_CTL &= ~(uint32_t) dbg_periph;
}
