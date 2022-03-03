#ifndef __GD32VF103_PMU_H
#define __GD32VF103_PMU_H

#include "gd32vf103.h"

#define PMU				PMU_BASE
#define PMU_CTL				REG32(PMU + 0)
// PMU control and status register
#define PMU_CS				REG32(PMU + 4)


// LDO low power mode
#define PMU_CTL_LDOLP			BIT(0)
// standby mode
#define PMU_CTL_STBMOD			BIT(1)
// wakeup flag reset
#define PMU_CTL_WURST			BIT(2)
// standby flag reset
#define PMU_CTL_STBRST			BIT(3)
// low voltage detector enable
#define PMU_CTL_LVDEN			BIT(4)
// low voltage detector threshold
#define PMU_CTL_LVDT			BITS(5, 7)
// backup domain write enable
#define PMU_CTL_BKPWEN			BIT(8)

// wakeup flag
#define PMU_CS_WUF			BIT(0)
// standby flag
#define PMU_CS_STBF			BIT(1)
// low voltage detector status flag
#define PMU_CS_LVDF			BIT(2)
// wakeup pin enable
#define PMU_CS_WUPEN			BIT(8)


// PMU low voltage detector threshold definitions
#define CTL_LVDT(regval) \
	(BITS(5,7) & ((uint32_t) (regval) << 5))

// voltage threshold is 2.2V
#define PMU_LVDT_0			CTL_LVDT(0)
// voltage threshold is 2.3V
#define PMU_LVDT_1			CTL_LVDT(1)
// voltage threshold is 2.4V
#define PMU_LVDT_2			CTL_LVDT(2)
// voltage threshold is 2.5V
#define PMU_LVDT_3			CTL_LVDT(3)
// voltage threshold is 2.6V
#define PMU_LVDT_4			CTL_LVDT(4)
// voltage threshold is 2.7V
#define PMU_LVDT_5			CTL_LVDT(5)
// voltage threshold is 2.8V
#define PMU_LVDT_6			CTL_LVDT(6)
// voltage threshold is 2.9V
#define PMU_LVDT_7			CTL_LVDT(7)

// wakeup flag status
#define PMU_FLAG_WAKEUP			PMU_CS_WUF
// standby flag status
#define PMU_FLAG_STANDBY		PMU_CS_STBF
// lvd flag status
#define PMU_FLAG_LVD			PMU_CS_LVDF

// PMU ldo definitions
// LDO normal work when PMU enter deepsleep mode
#define PMU_LDO_NORMAL			((uint32_t) 0)
// LDO work at low power status when PMU enter deepsleep mode
#define PMU_LDO_LOWPOWER		PMU_CTL_LDOLP

// PMU flag reset definitions
enum pmu_flag_reset {
	PMU_FLAG_RESET_WAKEUP,
	PMU_FLAG_RESET_STANDBY
};

// PMU command constants definitions
enum wfi_wfe_cmd {
	WFI_CMD,
	WFE_CMD
};

void pmu_deinit(void);

// select low voltage detector threshold
void pmu_lvd_select(uint32_t lvdt_n);

void pmu_lvd_disable(void);

void pmu_to_sleepmode(enum wfi_wfe_cmd sleepmodecmd);
void pmu_to_deepsleepmode(uint32_t ldo, enum wfi_wfe_cmd deepsleepmodecmd);
void pmu_to_standbymode(enum wfi_wfe_cmd standbymodecmd);

void pmu_wakeup_pin_enable();
void pmu_wakeup_pin_disable();

// backup related functions

void pmu_backup_write_enable(void);
void pmu_backup_write_disable(void);

// flag functions
enum flag_status pmu_flag_get(uint32_t flag);
void pmu_flag_clear(enum pmu_flag_reset flag_reset);

#endif
