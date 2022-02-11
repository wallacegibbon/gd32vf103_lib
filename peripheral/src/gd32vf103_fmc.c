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
 * 	one or more parameters can be selected which are shown as below:
 * 		OB_WP_x(x = 0..31): write protect specify sector
 * 		OB_WP_ALL: write protect all sector
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
	if (state != FMC_TOERR) {
		// reset the OBPG bit
		FMC_CTL &= ~FMC_CTL_OBPG;
	}
	return state;
}

/*
 * configure security protection
 * ob_spc: specify security protection
 * 	only one parameter can be selected which is shown as below:
 * 		FMC_NSPC: no security protection
 * 		FMC_USPC: under security protection
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

/*!
    \brief      program the FMC user option byte
    \param[in]  ob_fwdgt: option byte watchdog value
      \arg        OB_FWDGT_SW: software free watchdog
      \arg        OB_FWDGT_HW: hardware free watchdog
    \param[in]  ob_deepsleep: option byte deepsleep reset value
      \arg        OB_DEEPSLEEP_NRST: no reset when entering deepsleep mode
      \arg        OB_DEEPSLEEP_RST: generate a reset instead of entering deepsleep mode
    \param[in]  ob_stdby:option byte standby reset value
      \arg        OB_STDBY_NRST: no reset when entering standby mode
      \arg        OB_STDBY_RST: generate a reset instead of entering standby mode
    \param[in]  ob_boot: specifies the option byte boot bank value
      \arg        OB_BOOT_B0: boot from bank0
    \param[out] none
    \retval     state of FMC, refer to enum fmc_state
 */
enum fmc_state ob_user_write(uint8_t ob_fwdgt, uint8_t ob_deepsleep,
		uint8_t ob_stdby, uint8_t ob_boot) {
	enum fmc_state fmc_state = FMC_READY;
	uint8_t temp;

	/* wait for the FMC ready */
	fmc_state = fmc_ready_wait(FMC_TIMEOUT_COUNT);

	if (FMC_READY == fmc_state) {
		/* set the OBPG bit */
		FMC_CTL |= FMC_CTL_OBPG;

		temp =
		    ((uint8_t)
		     ((uint8_t) ((uint8_t) (ob_boot | ob_fwdgt) | ob_deepsleep)
		      | ob_stdby) | OB_USER_MASK);
		OB_USER = (uint16_t) temp;

		/* wait for the FMC ready */
		fmc_state = fmc_ready_wait(FMC_TIMEOUT_COUNT);

		if (FMC_TOERR != fmc_state) {
			/* reset the OBPG bit */
			FMC_CTL &= ~FMC_CTL_OBPG;
		}
	}
	/* return the FMC state */
	return fmc_state;
}

/*!
    \brief      program the FMC data option byte
    \param[in]  address: the option bytes address to be programmed
    \param[in]  data: the byte to be programmed
    \param[out] none
    \retval     state of FMC, refer to enum fmc_state
 */
enum fmc_state ob_data_program(uint32_t address, uint8_t data) {
	enum fmc_state fmc_state = fmc_ready_wait(FMC_TIMEOUT_COUNT);

	if (FMC_READY == fmc_state) {
		/* set the OBPG bit */
		FMC_CTL |= FMC_CTL_OBPG;
		REG16(address) = data;

		/* wait for the FMC ready */
		fmc_state = fmc_ready_wait(FMC_TIMEOUT_COUNT);

		if (FMC_TOERR != fmc_state) {
			/* reset the OBPG bit */
			FMC_CTL &= ~FMC_CTL_OBPG;
		}
	}
	/* return the FMC state */
	return fmc_state;
}

/*!
    \brief      get OB_USER in register FMC_OBSTAT
    \param[in]  none
    \param[out] none
    \retval     the FMC user option byte values
 */
uint8_t ob_user_get() {
	/* return the FMC user option byte value */
	return (uint8_t) (FMC_OBSTAT >> 2U);
}

/*!
    \brief      get OB_DATA in register FMC_OBSTAT
    \param[in]  none
    \param[out] none
    \retval     ob_data
 */
uint16_t ob_data_get() {
	return (uint16_t) (FMC_OBSTAT >> 10U);
}

/*!
    \brief      get the FMC option byte write protection
    \param[in]  none
    \param[out] none
    \retval     the FMC write protection option byte value
 */
uint32_t ob_write_protection_get() {
	/* return the FMC write protection option byte value */
	return FMC_WP;
}

/*!
    \brief      get FMC option byte security protection state
    \param[in]  none
    \param[out] none
    \retval     enum flag_status: SET or RESET
 */
enum flag_status ob_spc_get() {
	enum flag_status spc_state = RESET;

	if (RESET != (FMC_OBSTAT & FMC_OBSTAT_SPC)) {
		spc_state = SET;
	} else {
		spc_state = RESET;
	}
	return spc_state;
}

/*!
    \brief      enable FMC interrupt
    \param[in]  interrupt: the FMC interrupt source
    only one parameter can be selected which is shown as below:
      \arg        FMC_INT_END: enable FMC end of program interrupt
      \arg        FMC_INT_ERR: enable FMC error interrupt
    \param[out] none
    \retval     none
 */
void fmc_interrupt_enable(uint32_t interrupt) {
	FMC_REG_VAL(interrupt) |= BIT(FMC_BIT_POS(interrupt));
}

/*!
    \brief      disable FMC interrupt
    \param[in]  interrupt: the FMC interrupt source
    only one parameter can be selected which is shown as below:
      \arg        FMC_INT_END: enable FMC end of program interrupt
      \arg        FMC_INT_ERR: enable FMC error interrupt
    \param[out] none
    \retval     none
 */
void fmc_interrupt_disable(uint32_t interrupt) {
	FMC_REG_VAL(interrupt) &= ~BIT(FMC_BIT_POS(interrupt));
}

/*!
    \brief      check flag is set or not
    \param[in]  flag: check FMC flag
    only one parameter can be selected which is shown as below:
      \arg        FMC_FLAG_BUSY: FMC busy flag bit
      \arg        FMC_FLAG_PGERR: FMC operation error flag bit
      \arg        FMC_FLAG_WPERR: FMC erase/program protection error flag bit
      \arg        FMC_FLAG_END: FMC end of operation flag bit
      \arg        FMC_FLAG_OBERR: FMC option byte read error flag bit
    \param[out] none
    \retval     enum flag_status: SET or RESET
 */
enum flag_status fmc_flag_get(uint32_t flag) {
	if (RESET != (FMC_REG_VAL(flag) & BIT(FMC_BIT_POS(flag)))) {
		return SET;
	} else {
		return RESET;
	}
}

/*!
 \brief      clear the FMC flag
 \param[in]  flag: clear FMC flag
 only one parameter can be selected which is shown as below:
 \arg        FMC_FLAG_PGERR: FMC operation error flag bit
 \arg        FMC_FLAG_WPERR: FMC erase/program protection error flag bit
 \arg        FMC_FLAG_END: FMC end of operation flag bit
 \param[out] none
 \retval     none
 */
void fmc_flag_clear(uint32_t flag) {
	FMC_REG_VAL(flag) = (!FMC_REG_VAL(flag)) | BIT(FMC_BIT_POS(flag));
}

/*!
    \brief      get FMC interrupt flag state
    \param[in]  flag: FMC interrupt flags, refer to enum fmc_interrupt_flag
    only one parameter can be selected which is shown as below:
      \arg        FMC_INT_FLAG_PGERR: FMC operation error interrupt flag bit
      \arg        FMC_INT_FLAG_WPERR: FMC erase/program protection error interrupt flag bit
      \arg        FMC_INT_FLAG_END: FMC end of operation interrupt flag bit
    \param[out] none
    \retval     enum flag_status: SET or RESET
 */
enum flag_status fmc_interrupt_flag_get(enum fmc_interrupt_flag flag) {
	enum flag_status ret1 = RESET;
	enum flag_status ret2 = RESET;

	if (FMC_STAT_REG_OFFSET == FMC_REG_OFFSET_GET(flag)) {
		/* get the staus of interrupt flag */
		ret1 =
		    (enum flag_status) (FMC_REG_VALS(flag) &
					BIT(FMC_BIT_POS0(flag)));
		/* get the staus of interrupt enale bit */
		ret2 = (enum flag_status) (FMC_CTL & BIT(FMC_BIT_POS1(flag)));
	}

	if (ret1 && ret2) {
		return SET;
	} else {
		return RESET;
	}
}

/*!
    \brief      clear FMC interrupt flag state
    \param[in]  flag: FMC interrupt flags, refer to enum can_interrupt_flag
    only one parameter can be selected which is shown as below:
      \arg        FMC_INT_FLAG_PGERR: FMC operation error interrupt flag bit
      \arg        FMC_INT_FLAG_WPERR: FMC erase/program protection error interrupt flag bit
      \arg        FMC_INT_FLAG_END: FMC end of operation interrupt flag bit
     \param[out] none
     \retval     none
 */
void fmc_interrupt_flag_clear(enum fmc_interrupt_flag flag) {
	FMC_REG_VALS(flag) |= BIT(FMC_BIT_POS0(flag));
}

/* get the FMC state */
enum fmc_state fmc_state_get() {
	enum fmc_state fmc_state = FMC_READY;

	if (FMC_STAT & FMC_STAT_BUSY) {
		fmc_state = FMC_BUSY;
	} else {
		if (FMC_STAT & FMC_STAT_WPERR) {
			fmc_state = FMC_WPERR;
		} else {
			if (FMC_STAT & FMC_STAT_PGERR)
				fmc_state = FMC_PGERR;
		}
	}
	return fmc_state;
}

/* check whether FMC is ready or not */
enum fmc_state fmc_ready_wait(uint32_t timeout) {
	enum fmc_state fmc_state = FMC_BUSY;

	// wait for FMC ready
	do {
		// get FMC state
		fmc_state = fmc_state_get();
		timeout--;
	} while ((fmc_state == FMC_BUSY) && timeout);

	if (fmc_state == FMC_BUSY)
		fmc_state = FMC_TOERR;

	return fmc_state;
}
