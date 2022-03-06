#ifndef __GD32VF103_FMC_H
#define __GD32VF103_FMC_H

#include "gd32vf103.h"

// FMC register base address
#define FMC				FMC_BASE
// option bytes base address
#define OB				OB_BASE

// FMC wait state register
#define FMC_WS				REG32((FMC) + 0x00U)
// FMC unlock key register
#define FMC_KEY				REG32((FMC) + 0x04U)
// FMC option bytes unlock key register
#define FMC_OBKEY			REG32((FMC) + 0x08U)
// FMC status register
#define FMC_STAT			REG32((FMC) + 0x0CU)
// FMC control register
#define FMC_CTL				REG32((FMC) + 0x10U)
// FMC address register
#define FMC_ADDR			REG32((FMC) + 0x14U)
// FMC option bytes status register
#define FMC_OBSTAT			REG32((FMC) + 0x1CU)
// FMC erase/program protection register
#define FMC_WP				REG32((FMC) + 0x20U)
// FMC product ID register
#define FMC_PID				REG32((FMC) + 0x100U)

// option byte security protection value
#define OB_SPC				REG16((OB) + 0x00U)
// option byte user valu
#define OB_USER				REG16((OB) + 0x02U)
// option byte write protection 0
#define OB_WP0				REG16((OB) + 0x08U)
// option byte write protection 1
#define OB_WP1				REG16((OB) + 0x0AU)
// option byte write protection 2
#define OB_WP2				REG16((OB) + 0x0CU)
// option byte write protection 3
#define OB_WP3				REG16((OB) + 0x0EU)

// wait state counter
#define FMC_WS_WSCNT			BITS(0,2)

// FMC_CTL unlock key bits
#define FMC_KEY_KEY			BITS(0,31)

// option bytes unlock key bits
#define FMC_OBKEY_OBKEY			BITS(0,31)

// flash busy flag bit
#define FMC_STAT_BUSY			BIT(0)
// flash program error flag bit
#define FMC_STAT_PGERR			BIT(2)
// erase/program protection error flag bit
#define FMC_STAT_WPERR			BIT(4)
// end of operation flag bit
#define FMC_STAT_ENDF			BIT(5)

// main flash program command bit
#define FMC_CTL_PG			BIT(0)
// main flash page erase command bit
#define FMC_CTL_PER			BIT(1)
// main flash mass erase command bit
#define FMC_CTL_MER			BIT(2)
// option bytes program command bit
#define FMC_CTL_OBPG			BIT(4)
// option bytes erase command bit
#define FMC_CTL_OBER			BIT(5)
// send erase command to FMC bit
#define FMC_CTL_START			BIT(6)
// FMC_CTL lock bit
#define FMC_CTL_LK			BIT(7)
// option bytes erase/program enable bit
#define FMC_CTL_OBWEN			BIT(9)
// error interrupt enable bit
#define FMC_CTL_ERRIE			BIT(10)
// end of operation interrupt enable bit
#define FMC_CTL_ENDIE			BIT(12)

// Flash erase/program command address bits
#define FMC_ADDR0_ADDR			BITS(0,31)

// option bytes read error bit.
#define FMC_OBSTAT_OBERR		BIT(0)
// option bytes security protection code
#define FMC_OBSTAT_SPC			BIT(1)
// store USER of option bytes block after system reset
#define FMC_OBSTAT_USER			BITS(2,9)
// store DATA of option bytes block after system reset.
#define FMC_OBSTAT_DATA			BITS(10,25)

// store WP of option bytes block after system reset
#define FMC_WP_WP			BITS(0,31)

// FMC wait state enable bit
#define FMC_WSEN_WSEN			BIT(0)

// product ID bits
#define FMC_PID_PID			BITS(0,31)

// define the FMC bit position and its register index offset
#define FMC_REGIDX_BIT(regidx, bitpos) \
	(((regidx) << 6) | (bitpos))

#define FMC_REG_VAL(offset) \
	(REG32(FMC + ((uint32_t) (offset) >> 6)))

#define FMC_BIT_POS(val) \
	((val) & 0x1FU)

#define FMC_REGIDX_BITS(regidx, bitpos0, bitpos1) \
	(((regidx) << 12) | ((bitpos0) << 6) | (bitpos1))

#define FMC_REG_VALS(offset) \
	(REG32(FMC + ((uint32_t) (offset) >> 12)))

#define FMC_BIT_POS0(val) \
	(((uint32_t) (val) >> 6) & 0x1FU)

#define FMC_BIT_POS1(val) \
	((val) & 0x1FU)

#define FMC_REG_OFFSET_GET(flag) \
	((uint32_t) (flag) >> 12)

// status register offset
#define FMC_STAT_REG_OFFSET		0x0CU
// control register offset
#define FMC_CTL_REG_OFFSET		0x10U
// option byte status register offset
#define FMC_OBSTAT_REG_OFFSET		0x1CU

enum fmc_state {
	// the operation has been completed
	FMC_READY,

	// the operation is in progress
	FMC_BUSY,

	// program error
	FMC_PGERR,

	// erase/program protection error
	FMC_WPERR,

	// timeout error
	FMC_TOERR,

};

// FMC interrupt enable
enum fmc_int {
	// enable FMC end of program interrupt
	FMC_INT_END = FMC_REGIDX_BIT(FMC_CTL_REG_OFFSET, 12U),

	// enable FMC error interrupt
	FMC_INT_ERR = FMC_REGIDX_BIT(FMC_CTL_REG_OFFSET, 10U),

};

// FMC flags
enum fmc_flag {
	// FMC busy flag
	FMC_FLAG_BUSY = FMC_REGIDX_BIT(FMC_STAT_REG_OFFSET, 0U),

	// FMC operation error flag bit
	FMC_FLAG_PGERR = FMC_REGIDX_BIT(FMC_STAT_REG_OFFSET, 2U),

	// FMC erase/program protection error flag bit
	FMC_FLAG_WPERR = FMC_REGIDX_BIT(FMC_STAT_REG_OFFSET, 4U),

	// FMC end of operation flag bit
	FMC_FLAG_END = FMC_REGIDX_BIT(FMC_STAT_REG_OFFSET, 5U),

	// FMC option bytes read error flag
	FMC_FLAG_OBERR = FMC_REGIDX_BIT(FMC_OBSTAT_REG_OFFSET, 0U),

};

// FMC interrupt flags
enum fmc_interrupt_flag {
	// FMC operation error interrupt flag bit
	FMC_INT_FLAG_PGERR = FMC_REGIDX_BITS(FMC_STAT_REG_OFFSET, 2U, 10U),

	// FMC erase/program protection error interrupt flag bit
	FMC_INT_FLAG_WPERR = FMC_REGIDX_BITS(FMC_STAT_REG_OFFSET, 4U, 10U),

	// FMC end of operation interrupt flag bit
	FMC_INT_FLAG_END = FMC_REGIDX_BITS(FMC_STAT_REG_OFFSET, 5U, 12U),

};

#define UNLOCK_KEY0			((uint32_t) 0x45670123U)
#define UNLOCK_KEY1			((uint32_t) 0xCDEF89ABU)

// FMC wait state counter
#define WS_WSCNT(regval)		(BITS(0,2) & ((uint32_t) (regval)))
// FMC 0 wait
#define WS_WSCNT_0			WS_WSCNT(0)
// FMC 1 wait
#define WS_WSCNT_1			WS_WSCNT(1)
// FMC 2 wait
#define WS_WSCNT_2			WS_WSCNT(2)

// option bytes software/hardware free watch dog timer
// software free watchdog
#define OB_FWDGT_SW			((uint8_t) 0x01U)
// hardware free watchdog
#define OB_FWDGT_HW			((uint8_t) 0x00U)

// option bytes reset or not entering deep sleep mode
// no reset when entering deepsleep mode
#define OB_DEEPSLEEP_NRST		((uint8_t) 0x02U)
// generate a reset instead of entering deepsleep mode
#define OB_DEEPSLEEP_RST		((uint8_t) 0x00U)

// option bytes reset or not entering standby mode
// no reset when entering deepsleep mode
#define OB_STDBY_NRST			((uint8_t) 0x04U)
// generate a reset instead of entering standby mode
#define OB_STDBY_RST			((uint8_t) 0x00U)

// option bytes boot bank value
// boot from bank0
#define OB_BOOT_B0			((uint8_t) 0x08U)

// MASK value
#define OB_USER_MASK			((uint8_t) 0xF0U)

// read protect configure
// no security protection
#define FMC_NSPC			((uint8_t) 0xA5U)
// under security protection
#define FMC_USPC			((uint8_t) 0xBBU)

// option byte security protection value
#define OB_SPC_SPC			((uint32_t)0x000000FFU)
// option byte security protection complement value
#define OB_SPC_SPC_N			((uint32_t)0x0000FF00U)

// user option value
#define OB_USER_USER			((uint32_t)0x00FF0000U)
// user option complement value
#define OB_USER_USER_N			((uint32_t)0xFF000000U)

// FMC write protection option value
#define OB_WP0_WP0			((uint32_t)0x000000FFU)

// FMC write protection option complement value
#define OB_WP1_WP1			((uint32_t)0x0000FF00U)

// FMC write protection option value
#define OB_WP2_WP2			((uint32_t)0x00FF0000U)

// FMC write protection option complement value
#define OB_WP3_WP3			((uint32_t)0xFF000000U)

// option bytes write protection
// erase/program protection of sector 0 - 31
#define OB_WP_0				((uint32_t)0x00000001U)
#define OB_WP_1				((uint32_t)0x00000002U)
#define OB_WP_2				((uint32_t)0x00000004U)
#define OB_WP_3				((uint32_t)0x00000008U)
#define OB_WP_4				((uint32_t)0x00000010U)
#define OB_WP_5				((uint32_t)0x00000020U)
#define OB_WP_6				((uint32_t)0x00000040U)
#define OB_WP_7				((uint32_t)0x00000080U)
#define OB_WP_8				((uint32_t)0x00000100U)
#define OB_WP_9				((uint32_t)0x00000200U)
#define OB_WP_10			((uint32_t)0x00000400U)
#define OB_WP_11			((uint32_t)0x00000800U)
#define OB_WP_12			((uint32_t)0x00001000U)
#define OB_WP_13			((uint32_t)0x00002000U)
#define OB_WP_14			((uint32_t)0x00004000U)
#define OB_WP_15			((uint32_t)0x00008000U)
#define OB_WP_16			((uint32_t)0x00010000U)
#define OB_WP_17			((uint32_t)0x00020000U)
#define OB_WP_18			((uint32_t)0x00040000U)
#define OB_WP_19			((uint32_t)0x00080000U)
#define OB_WP_20			((uint32_t)0x00100000U)
#define OB_WP_21			((uint32_t)0x00200000U)
#define OB_WP_22			((uint32_t)0x00400000U)
#define OB_WP_23			((uint32_t)0x00800000U)
#define OB_WP_24			((uint32_t)0x01000000U)
#define OB_WP_25			((uint32_t)0x02000000U)
#define OB_WP_26			((uint32_t)0x04000000U)
#define OB_WP_27			((uint32_t)0x08000000U)
#define OB_WP_28			((uint32_t)0x10000000U)
#define OB_WP_29			((uint32_t)0x20000000U)
#define OB_WP_30			((uint32_t)0x40000000U)
#define OB_WP_31			((uint32_t)0x80000000U)
#define OB_WP_ALL			((uint32_t)0xFFFFFFFFU)

// FMC timeout count value
#define FMC_TIMEOUT_COUNT		((uint32_t)0x000F0000U)

// FMC BANK address
// FMC size
#define FMC_SIZE			(*(uint16_t *) 0x1FFFF7E0U)
// SRAM siz
#define SRAM_SIZE			(*(uint16_t *) 0x1FFFF7E2U)

// FMC main memory programming functions

// set the FMC wait state counter
void fmc_wscnt_set(uint32_t wscnt);

void fmc_unlock();
void fmc_lock();

enum fmc_state fmc_page_erase(uint32_t page_address);

// FMC erase whole chip
enum fmc_state fmc_mass_erase();

// FMC program a word at the corresponding address
enum fmc_state fmc_word_program(uint32_t address, uint32_t data);

// FMC program a half word at the corresponding address
enum fmc_state fmc_halfword_program(uint32_t address, uint16_t data);


// FMC option bytes programming functions
void ob_unlock();
void ob_lock();

// erase the FMC option byte
enum fmc_state ob_erase();
// enable write protection
enum fmc_state ob_write_protection_enable(uint32_t ob_wp);
// configure security protection
enum fmc_state ob_security_protection_config(uint8_t ob_spc);
// program the FMC user option byte
enum fmc_state ob_user_write(uint8_t ob_fwdgt, uint8_t ob_deepsleep,
		uint8_t ob_stdby, uint8_t ob_boot);

// program the FMC data option byte
enum fmc_state ob_data_program(uint32_t address, uint8_t data);
// get OB_USER in register FMC_OBSTAT
uint8_t ob_user_get();
// get OB_DATA in register FMC_OBSTAT
uint16_t ob_data_get();
// get the FMC option byte write protection
uint32_t ob_write_protection_get();
// get FMC option byte security protection state
enum flag_status ob_spc_get();

// FMC interrupts and flags management functions
void fmc_interrupt_enable(uint32_t interrupt);
void fmc_interrupt_disable(uint32_t interrupt);

enum flag_status fmc_flag_get(uint32_t flag);
void fmc_flag_clear(uint32_t flag);

enum flag_status fmc_interrupt_flag_get(enum fmc_interrupt_flag flag);
void fmc_interrupt_flag_clear(enum fmc_interrupt_flag flag);

enum fmc_state fmc_state_get();

enum fmc_state fmc_ready_wait(uint32_t timeout);

#endif
