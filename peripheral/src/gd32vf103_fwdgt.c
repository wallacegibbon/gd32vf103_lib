#include "gd32vf103_fwdgt.h"

#define CTL_CMD(regval)		(BITS(0, 15) & ((uint32_t) (regval) << 0))
#define RLD_RLD(regval)		(BITS(0, 11) & ((uint32_t) (regval) << 0))

/* enable write access to FWDGT_PSC and FWDGT_RLD */
void fwdgt_write_enable() {
	FWDGT_CTL = FWDGT_WRITEACCESS_ENABLE;
}

/* disable write access to FWDGT_PSC and FWDGT_RLD */
void fwdgt_write_disable() {
	FWDGT_CTL = FWDGT_WRITEACCESS_DISABLE;
}

/* start the free watchdog timer counter */
void fwdgt_enable() {
	FWDGT_CTL = FWDGT_KEY_ENABLE;
}

void fwdgt_counter_reload() {
	FWDGT_CTL = FWDGT_KEY_RELOAD;
}

/*
 * configure counter reload value, and prescaler divider value
 * reload_value: specify reload value(0x0000 - 0x0FFF)
 * prescaler_div: FWDGT prescaler value
 * 	only one parameter can be selected which is shown as below:
 * 		FWDGT_PSC_DIV4: FWDGT prescaler set to 4
 * 		FWDGT_PSC_DIV8: FWDGT prescaler set to 8
 * 		FWDGT_PSC_DIV16: FWDGT prescaler set to 16
 * 		FWDGT_PSC_DIV32: FWDGT prescaler set to 32
 * 		FWDGT_PSC_DIV64: FWDGT prescaler set to 64
 * 		FWDGT_PSC_DIV128: FWDGT prescaler set to 128
 * 		FWDGT_PSC_DIV256: FWDGT prescaler set to 256
 */
enum err_status fwdgt_config(uint16_t reload_value, uint8_t prescaler_div) {
	uint32_t timeout = FWDGT_PSC_TIMEOUT;
	uint32_t flag_status = RESET;

	// enable write access to FWDGT_PSC,and FWDGT_RLD
	FWDGT_CTL = FWDGT_WRITEACCESS_ENABLE;

	// wait until the PUD flag to be reset
	do {
		flag_status = FWDGT_STAT & FWDGT_STAT_PUD;
	} while ((--timeout > 0) && (flag_status != RESET));

	if (flag_status != RESET)
		return ERROR;

	// configure FWDGT
	FWDGT_PSC =  prescaler_div;

	timeout = FWDGT_RLD_TIMEOUT;

	// wait until the RUD flag to be reset
	do {
		flag_status = FWDGT_STAT & FWDGT_STAT_RUD;
	} while ((--timeout > 0) && (flag_status != RESET));

	if (flag_status != RESET)
		return ERROR;

	FWDGT_RLD = RLD_RLD(reload_value);
	// reload the counter
	FWDGT_CTL = FWDGT_KEY_RELOAD;

	return SUCCESS;
}

/*
 * get flag state of FWDGT
 * flag: flag to get
 * 	only one parameter can be selected which is shown as below:
 * 		FWDGT_FLAG_PUD:
 * 			a write operation to FWDGT_PSC register is on going
 * 		FWDGT_FLAG_RUD:
 * 			a write operation to FWDGT_RLD register is on going
 */
enum flag_status fwdgt_flag_get(uint16_t flag) {
	if (flag & FWDGT_STAT)
		return SET;
	else
		return RESET;
}
