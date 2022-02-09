#ifndef GD32VF103_DBG_H
#define GD32VF103_DBG_H

#include "gd32vf103.h"

#define DBG			DBG_BASE

#define DBG_ID			REG32(DBG + 0x00U)
#define DBG_CTL			REG32(DBG + 0x04U)

#define DBG_ID_ID_CODE		BITS(0,31)

// keep debugger connection during sleep mode
#define DBG_CTL_SLP_HOLD	BIT(0)

// keep debugger connection during deepsleep mode
#define DBG_CTL_DSLP_HOLD	BIT(1)

// keep debugger connection during standby mode
#define DBG_CTL_STB_HOLD	BIT(2)

// debug FWDGT kept when core is halted
#define DBG_CTL_FWDGT_HOLD	BIT(8)
#define DBG_CTL_WWDGT_HOLD	BIT(9) debug WWDGT kept when core is halted
#define DBG_CTL_TIMER0_HOLD	BIT(10) hold TIMER0 counter when core is halted
#define DBG_CTL_TIMER1_HOLD	BIT(11) hold TIMER1 counter when core is halted
#define DBG_CTL_TIMER2_HOLD	BIT(12) hold TIMER2 counter when core is halted
#define DBG_CTL_TIMER3_HOLD	BIT(13) hold TIMER3 counter when core is halted
#define DBG_CTL_CAN0_HOLD	BIT(14) debug CAN0 kept when core is halted
#define DBG_CTL_I2C0_HOLD	BIT(15) hold I2C0 smbus when core is halted
#define DBG_CTL_I2C1_HOLD	BIT(16) hold I2C1 smbus when core is halted
#define DBG_CTL_TIMER4_HOLD	BIT(18) hold TIMER4 counter when core is halted
#define DBG_CTL_TIMER5_HOLD	BIT(19) hold TIMER5 counter when core is halted
#define DBG_CTL_TIMER6_HOLD	BIT(20) hold TIMER6 counter when core is halted
#define DBG_CTL_CAN1_HOLD	BIT(21) debug CAN1 kept when core is halted

 constants definitions
 debug hold when core is halted
enum dbg_periph {
	DBG_FWDGT_HOLD	= BIT(8), // debug FWDGT kept when core is halted
	DBG_WWDGT_HOLD	= BIT(9), // debug WWDGT kept when core is halted
	DBG_TIMER0_HOLD	= BIT(10) // hold TIMER0 counter when core is halted
	DBG_TIMER1_HOLD	= BIT(11) // hold TIMER1 counter when core is halted
	DBG_TIMER2_HOLD	= BIT(12) // hold TIMER2 counter when core is halted
	DBG_TIMER3_HOLD	= BIT(13) // hold TIMER3 counter when core is halted
	DBG_CAN0_HOLD	= BIT(14) // debug CAN0 kept when core is halted
	DBG_I2C0_HOLD	= BIT(15) // hold I2C0 smbus when core is halted
	DBG_I2C1_HOLD	= BIT(16) // hold I2C1 smbus when core is halted
	DBG_TIMER4_HOLD	= BIT(17) // hold TIMER4 counter when core is halted
	DBG_TIMER5_HOLD	= BIT(18) // hold TIMER5 counter when core is halted
	DBG_TIMER6_HOLD	= BIT(19) // hold TIMER6 counter when core is halted
	DBG_CAN1_HOLD	= BIT(21) // debug CAN1 kept when core is halted
};


// DBG low power mode configurations

// keep debugger connection during sleep mode
#define DBG_LOW_POWER_SLEEP	DBG_CTL_SLP_HOLD

// keep debugger connection during deepsleep mode
#define DBG_LOW_POWER_DEEPSLEEP	DBG_CTL_DSLP_HOLD

// keep debugger connection during standby mode
#define DBG_LOW_POWER_STANDBY	DBG_CTL_STB_HOLD

// read DBG_ID code register
uint32_t dbg_id_get(void);

// low power behavior configuration

// enable low power behavior when the MCU is in debug mode
void dbg_low_power_enable(uint32_t dbg_low_power);

// disable low power behavior when the MCU is in debug mode
void dbg_low_power_disable(uint32_t dbg_low_power);

// peripheral behavior configuration

// enable peripheral behavior when the MCU is in debug mode
void dbg_periph_enable(enum dbg_periph dbg_periph);

// disable peripheral behavior when the MCU is in debug mode
void dbg_periph_disable(enum dbg_periph dbg_periph);

#endif
