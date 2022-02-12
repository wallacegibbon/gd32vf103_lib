#include "gd32vf103_fmc.h"

/*
 * set the FMC wait state counter
 * wscnt: wait state counter value
 * 	WS_WSCNT_0: FMC 0 wait state
 * 	WS_WSCNT_1: FMC 1 wait state
 * 	WS_WSCNT_2: FMC 2 wait state
 */
void fmc_wscnt_set(uint32_t wscnt) {
	uint32_t reg = FMC_WS;
	// set the wait state counter value
	reg &= ~FMC_WS_WSCNT;
	FMC_WS = reg | wscnt;
}

void fmc_unlock() {
	if ((FMC_CTL & FMC_CTL_LK) != RESET) {
		// write the FMC unlock key
		FMC_KEY = UNLOCK_KEY0;
		FMC_KEY = UNLOCK_KEY1;
	}
}

void fmc_lock() {
	// set the LK bit
	FMC_CTL |= FMC_CTL_LK;
}

enum fmc_state fmc_page_erase(uint32_t page_address) {
	enum fmc_state state = fmc_ready_wait(FMC_TIMEOUT_COUNT);
	if (state != FMC_READY)
		return state;

	FMC_CTL |= FMC_CTL_PER;
	FMC_ADDR = page_address;
	FMC_CTL |= FMC_CTL_START;
	// wait for the FMC ready
	state = fmc_ready_wait(FMC_TIMEOUT_COUNT);
	// reset the PER bit
	FMC_CTL &= ~FMC_CTL_PER;
	return state;
}

// FMC erase whole chip
enum fmc_state fmc_mass_erase() {
	enum fmc_state state = fmc_ready_wait(FMC_TIMEOUT_COUNT);
	if (state != FMC_READY)
		return state;

	// start whole chip erase
	FMC_CTL |= FMC_CTL_MER;
	FMC_CTL |= FMC_CTL_START;
	// wait for the FMC ready
	state = fmc_ready_wait(FMC_TIMEOUT_COUNT);
	// reset the MER bit
	FMC_CTL &= ~FMC_CTL_MER;
	return state;
}

/*
 * FMC program a word at the corresponding address
 * address: address to program
 * data: word to program
 */
enum fmc_state fmc_word_program(uint32_t address, uint32_t data) {
	enum fmc_state state = fmc_ready_wait(FMC_TIMEOUT_COUNT);
	if (state != FMC_READY)
		return state;

	// set the PG bit to start program
	FMC_CTL |= FMC_CTL_PG;
	REG32(address) = data;
	// wait for the FMC ready
	state = fmc_ready_wait(FMC_TIMEOUT_COUNT);
	// reset the PG bit
	FMC_CTL &= ~FMC_CTL_PG;
	return state;
}

/*
 * FMC program a half word at the corresponding address
 * address: address to program
 * data: halfword to program
 */
enum fmc_state fmc_halfword_program(uint32_t address, uint16_t data) {
	enum fmc_state state = fmc_ready_wait(FMC_TIMEOUT_COUNT);
	if (state != FMC_READY)
		return state;

	// set the PG bit to start program
	FMC_CTL |= FMC_CTL_PG;
	REG16(address) = data;
	// wait for the FMC ready
	state = fmc_ready_wait(FMC_TIMEOUT_COUNT);
	// reset the PG bit
	FMC_CTL &= ~FMC_CTL_PG;
	return state;
}

void ob_unlock() {
	if ((FMC_CTL & FMC_CTL_OBWEN) == RESET) {
		// write the FMC key
		FMC_OBKEY = UNLOCK_KEY0;
		FMC_OBKEY = UNLOCK_KEY1;
	}

	// wait until OBWEN bit is set by hardware
	while ((FMC_CTL & FMC_CTL_OBWEN) == RESET);
}

void ob_lock() {
	// reset the OBWEN bit
	FMC_CTL &= ~FMC_CTL_OBWEN;
}

/*
 * erase the FMC option byte
 * unlock the FMC_CTL and option byte before calling this function
 */
enum fmc_state ob_erase() {
	uint16_t temp_spc = FMC_NSPC;

	enum fmc_state fmc_state = fmc_ready_wait(FMC_TIMEOUT_COUNT);

	// check the option byte security protection value
	if (ob_spc_get() != RESET)
		temp_spc = FMC_USPC;

	if (fmc_state == FMC_READY) {
		// start erase the option byte
		FMC_CTL |= FMC_CTL_OBER;
		FMC_CTL |= FMC_CTL_START;

		// wait for the FMC ready
		fmc_state = fmc_ready_wait(FMC_TIMEOUT_COUNT);

		if (FMC_READY == fmc_state) {
			// reset the OBER bit
			FMC_CTL &= ~FMC_CTL_OBER;
			// set the OBPG bit
			FMC_CTL |= FMC_CTL_OBPG;
			// no security protection
			OB_SPC = (uint16_t) temp_spc;
			// wait for the FMC ready
			fmc_state = fmc_ready_wait(FMC_TIMEOUT_COUNT);
			if (FMC_TOERR != fmc_state)
				// reset the OBPG bit
				FMC_CTL &= ~FMC_CTL_OBPG;
		} else {
			if (FMC_TOERR != fmc_state)
				// reset the OBPG bit
				FMC_CTL &= ~FMC_CTL_OBPG;
		}
	}
	return fmc_state;
}

/*
 * enable write protection
 * ob_wp:
 * 	specify sector to be write protected, set the bit to 1 if you want to
 * 	protect the corresponding pages. meanwhile, sector macro could used
 * 	to set specific sector write protected.
 *
 * 	OB_WP_x(x = 0..31): write protect specify sector
 * 	OB_WP_ALL: write protect all sector
 */
enum fmc_state ob_write_protection_enable(uint32_t ob_wp) {
	uint16_t temp_wp0, temp_wp1, temp_wp2, temp_wp3;

	enum fmc_state state = fmc_ready_wait(FMC_TIMEOUT_COUNT);
	if (state != FMC_READY)
		return state;

	ob_wp = ~ob_wp;
	temp_wp0 = (uint16_t) (ob_wp & OB_WP0_WP0);
	temp_wp1 = (uint16_t) ((ob_wp & OB_WP1_WP1) >> 8U);
	temp_wp2 = (uint16_t) ((ob_wp & OB_WP2_WP2) >> 16U);
	temp_wp3 = (uint16_t) ((ob_wp & OB_WP3_WP3) >> 24U);

	// set the OBPG bit
	FMC_CTL |= FMC_CTL_OBPG;

	if (temp_wp0 != 0xFFU) {
		OB_WP0 = temp_wp0;

		// wait for the FMC ready
		state = fmc_ready_wait(FMC_TIMEOUT_COUNT);
	}
	if ((state == FMC_READY) && (temp_wp1 != 0xFFU)) {
		OB_WP1 = temp_wp1;

		// wait for the FMC ready
		state = fmc_ready_wait(FMC_TIMEOUT_COUNT);
	}
	if ((state == FMC_READY) && (temp_wp2 != 0xFFU)) {
		OB_WP2 = temp_wp2;

		// wait for the FMC ready
		state = fmc_ready_wait(FMC_TIMEOUT_COUNT);
	}
	if ((state == FMC_READY) && (temp_wp3 != 0xFFU)) {
		OB_WP3 = temp_wp3;

		// wait for the FMC ready
		state = fmc_ready_wait(FMC_TIMEOUT_COUNT);
	}

	if (state != FMC_TOERR)
		// reset the OBPG bit
		FMC_CTL &= ~FMC_CTL_OBPG;

	return state;
}

/*
 * configure security protection
 * ob_spc: specify security protection
 * 	FMC_NSPC: no security protection
 * 	FMC_USPC: under security protection
 */
enum fmc_state ob_security_protection_config(uint8_t ob_spc) {
	enum fmc_state state = fmc_ready_wait(FMC_TIMEOUT_COUNT);
	if (state != FMC_READY)
		return state;

	FMC_CTL |= FMC_CTL_OBER;
	FMC_CTL |= FMC_CTL_START;

	// wait for the FMC ready
	state = fmc_ready_wait(FMC_TIMEOUT_COUNT);

	if (FMC_READY == state) {
		// reset the OBER bit
		FMC_CTL &= ~FMC_CTL_OBER;

		// start the option byte program
		FMC_CTL |= FMC_CTL_OBPG;

		OB_SPC = (uint16_t) ob_spc;

		// wait for the FMC ready
		state = fmc_ready_wait(FMC_TIMEOUT_COUNT);

		if (FMC_TOERR != state)
			// reset the OBPG bit
			FMC_CTL &= ~FMC_CTL_OBPG;
	} else {
		if (FMC_TOERR != state)
			// reset the OBER bit
			FMC_CTL &= ~FMC_CTL_OBER;
	}
}

/*
 * program the FMC user option byte
 * ob_fwdgt: option byte watchdog value
 * 	OB_FWDGT_SW: software free watchdog
 * 	OB_FWDGT_HW: hardware free watchdog
 *
 * ob_deepsleep: option byte deepsleep reset value
 * 	OB_DEEPSLEEP_NRST: no reset when entering deepsleep mode
 * 	OB_DEEPSLEEP_RST: generate a reset instead of entering deepsleep mode
 *
 * ob_stdby:option byte standby reset value
 * 	OB_STDBY_NRST: no reset when entering standby mode
 * 	OB_STDBY_RST: generate a reset instead of entering standby mode
 *
 * ob_boot: specifies the option byte boot bank value
 * 	OB_BOOT_B0: boot from bank0
 */
enum fmc_state ob_user_write(uint8_t ob_fwdgt, uint8_t ob_deepsleep,
		uint8_t ob_stdby, uint8_t ob_boot) {
	// wait for the FMC ready
	enum fmc_state state = fmc_ready_wait(FMC_TIMEOUT_COUNT);

	if (state != FMC_READY)
		return state;

	// set the OBPG bit
	FMC_CTL |= FMC_CTL_OBPG;

	uint8_t temp =
		((uint8_t) ((uint8_t)
			((uint8_t) (ob_boot | ob_fwdgt) | ob_deepsleep)
			| ob_stdby) |
		 OB_USER_MASK);

	OB_USER = (uint16_t) temp;

	// wait for the FMC ready
	state = fmc_ready_wait(FMC_TIMEOUT_COUNT);

	if (state != FMC_TOERR)
		// reset the OBPG bit
		FMC_CTL &= ~FMC_CTL_OBPG;

	return state;
}

/*!
    \brief      program the FMC data option byte
    \param[in]  address: the option bytes address to be programmed
    \param[in]  data: the byte to be programmed
    \param[out] none
    \retval     state of FMC, refer to enum fmc_state
 */
enum fmc_state ob_data_program(uint32_t address, uint8_t data) {
	enum fmc_state state = fmc_ready_wait(FMC_TIMEOUT_COUNT);

	if (state != FMC_READY)
		return state;

	// set the OBPG bit
	FMC_CTL |= FMC_CTL_OBPG;
	REG16(address) = data;

	// wait for the FMC ready
	state = fmc_ready_wait(FMC_TIMEOUT_COUNT);

	if (state != FMC_TOERR)
		// reset the OBPG bit
		FMC_CTL &= ~FMC_CTL_OBPG;

	return state;
}

/* get OB_USER in register FMC_OBSTAT */
uint8_t ob_user_get() {
	/* return the FMC user option byte value */
	return (uint8_t) (FMC_OBSTAT >> 2);
}

/* get OB_DATA in register FMC_OBSTAT */
uint16_t ob_data_get() {
	return (uint16_t) (FMC_OBSTAT >> 10);
}

/* get the FMC option byte write protection */
uint32_t ob_write_protection_get() {
	// return the FMC write protection option byte value
	return FMC_WP;
}

/* get FMC option byte security protection state */
enum flag_status ob_spc_get() {
	if (FMC_OBSTAT & FMC_OBSTAT_SPC != RESET)
		return SET;
	else
		return RESET;
}

/*
 * enable FMC interrupt
 * interrupt: the FMC interrupt source
 * 	FMC_INT_END: enable FMC end of program interrupt
 * 	FMC_INT_ERR: enable FMC error interrupt
 */
void fmc_interrupt_enable(uint32_t interrupt) {
	FMC_REG_VAL(interrupt) |= BIT(FMC_BIT_POS(interrupt));
}

/*
 * disable FMC interrupt
 * interrupt: the FMC interrupt source
 * 	FMC_INT_END: enable FMC end of program interrupt
 * 	FMC_INT_ERR: enable FMC error interrupt
 */
void fmc_interrupt_disable(uint32_t interrupt) {
	FMC_REG_VAL(interrupt) &= ~BIT(FMC_BIT_POS(interrupt));
}

/*
 * check flag is set or not
 * flag: check FMC flag
 * 	FMC_FLAG_BUSY: FMC busy flag bit
 * 	FMC_FLAG_PGERR: FMC operation error flag bit
 * 	FMC_FLAG_WPERR: FMC erase/program protection error flag bit
 * 	FMC_FLAG_END: FMC end of operation flag bit
 * 	FMC_FLAG_OBERR: FMC option byte read error flag bit
 */
enum flag_status fmc_flag_get(uint32_t flag) {
	if (FMC_REG_VAL(flag) & BIT(FMC_BIT_POS(flag)) != RESET)
		return SET;
	else
		return RESET;
}

/*
 * clear the FMC flag
 * flag: clear FMC flag
 * 	FMC_FLAG_PGERR: FMC operation error flag bit
 * 	FMC_FLAG_WPERR: FMC erase/program protection error flag bit
 * 	FMC_FLAG_END: FMC end of operation flag bit
 */
void fmc_flag_clear(uint32_t flag) {
	FMC_REG_VAL(flag) = (!FMC_REG_VAL(flag)) | BIT(FMC_BIT_POS(flag));
}

/*
 * get FMC interrupt flag state
 * flag: FMC interrupt flags, refer to enum fmc_interrupt_flag
 * 	FMC_INT_FLAG_PGERR:
 * 		FMC operation error interrupt flag bit
 * 	FMC_INT_FLAG_WPERR:
 * 		FMC erase/program protection error interrupt flag bit
 * 	FMC_INT_FLAG_END:
 * 		FMC end of operation interrupt flag bit
 */
enum flag_status fmc_interrupt_flag_get(enum fmc_interrupt_flag flag) {
	enum flag_status ret1 = RESET;
	enum flag_status ret2 = RESET;

	if (FMC_REG_OFFSET_GET(flag) == FMC_STAT_REG_OFFSET) {
		// get the staus of interrupt flag
		ret1 = (enum flag_status)
			(FMC_REG_VALS(flag) & BIT(FMC_BIT_POS0(flag)));

		// get the staus of interrupt enale bit
		ret2 = (enum flag_status) (FMC_CTL & BIT(FMC_BIT_POS1(flag)));
	}

	if (ret1 && ret2)
		return SET;
	else
		return RESET;
}

/*
 * clear FMC interrupt flag state
 * flag: FMC interrupt flags, refer to enum can_interrupt_flag
 * 	FMC_INT_FLAG_PGERR:
 * 		FMC operation error interrupt flag bit
 * 	FMC_INT_FLAG_WPERR:
 * 		FMC erase/program protection error interrupt flag bit
 * 	FMC_INT_FLAG_END:
 * 		FMC end of operation interrupt flag bit
 */
void fmc_interrupt_flag_clear(enum fmc_interrupt_flag flag) {
	FMC_REG_VALS(flag) |= BIT(FMC_BIT_POS0(flag));
}

enum fmc_state fmc_state_get() {
	if (FMC_STAT & FMC_STAT_BUSY)
		return FMC_BUSY;

	enum fmc_state state = FMC_READY;

	if (FMC_STAT & FMC_STAT_WPERR)
		state = FMC_WPERR;
	else if (FMC_STAT & FMC_STAT_PGERR)
		state = FMC_PGERR;

	return state;
}

enum fmc_state fmc_ready_wait(uint32_t timeout) {
	enum fmc_state state = FMC_BUSY;

	// wait for FMC ready
	do {
		// get FMC state
		state = fmc_state_get();
		timeout--;
	} while (state == FMC_BUSY && timeout);

	if (state == FMC_BUSY)
		state = FMC_TOERR;

	return state;
}
