#ifndef __GD32VF103_WWDGT_H
#define __GD32VF103_WWDGT_H

#include "gd32vf103.h"

#define WWDGT			WWDGT_BASE

#define WWDGT_CTL		REG32(WWDGT + 0x00000000U)
#define WWDGT_CFG		REG32(WWDGT + 0x00000004U)
#define WWDGT_STAT		REG32(WWDGT + 0x00000008U)

// WWDGT counter value
#define WWDGT_CTL_CNT		BITS(0, 6)
// WWDGT counter enable
#define WWDGT_CTL_WDGTEN	BIT(7)

// WWDGT counter window value
#define WWDGT_CFG_WIN		BITS(0, 6)
// WWDGT prescaler divider value
#define WWDGT_CFG_PSC		BITS(7, 8)
// early wakeup interrupt enable
#define WWDGT_CFG_EWIE		BIT(9)

// early wakeup interrupt flag
#define WWDGT_STAT_EWIF		BIT(0)

// write value to WWDGT_CFG_PSC bit field
#define CFG_PSC(regval) \
	(BITS(7, 8) & ((uint32_t) (regval) << 7))

// the time base of WWDGT = (PCLK1/4096)/1
#define WWDGT_CFG_PSC_DIV1		CFG_PSC(0)
// the time base of WWDGT = (PCLK1/4096)/2
#define WWDGT_CFG_PSC_DIV2		CFG_PSC(1)
// the time base of WWDGT = (PCLK1/4096)/4
#define WWDGT_CFG_PSC_DIV4		CFG_PSC(2)
// the time base of WWDGT = (PCLK1/4096)/8
#define WWDGT_CFG_PSC_DIV8		CFG_PSC(3)

/* reset the window watchdog timer configuration */
void wwdgt_deinit(void);
/* start the window watchdog timer counter */
void wwdgt_enable(void);

/* configure the window watchdog timer counter value */
void wwdgt_counter_update(uint16_t counter_value);
/* configure counter value, window value, and prescaler divider value */
void wwdgt_config(uint16_t counter, uint16_t window, uint32_t prescaler);

/* enable early wakeup interrupt of WWDGT */
void wwdgt_interrupt_enable(void);
/* check early wakeup interrupt state of WWDGT */
enum flag_status wwdgt_flag_get(void);
/* clear early wakeup interrupt state of WWDGT */
void wwdgt_flag_clear(void);

#endif
