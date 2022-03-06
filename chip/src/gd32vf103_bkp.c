#include "gd32vf103_bkp.h"

/* BKP register bits offset */
#define BKP_TAMPER_BITS_OFFSET	((uint32_t) 8U)

/*
 * reset BKP registers
*/
void bkp_deinit(void) {
	rcu_bkp_reset_enable();
	rcu_bkp_reset_disable();
}

/*
 * write BKP data register
 * register_number: refer to enum bkp_data_reg
 * 	BKP_DATA_x(x = 0..41): bkp data register number x
 *
 * data: the data to be write in BKP data register
 *
 */
void bkp_data_write(enum bkp_data_reg register_number, uint16_t data) {
	if ((register_number >= BKP_DATA_10) &&
			(register_number <= BKP_DATA_41)) {
		BKP_DATA10_41(register_number - 1U) = data;
	} else if ((register_number >= BKP_DATA_0) &&
			(register_number <= BKP_DATA_9)) {
		BKP_DATA0_9(register_number - 1U) = data;
	} else {
		/* illegal parameters */
	}
}

/*
 * read BKP data register
 * register_number: refer to enum bkp_data_reg
 * 	BKP_DATA_x(x = 0..41): bkp data register number x
 *
 */
uint16_t bkp_data_read(enum bkp_data_reg register_number) {
	uint16_t data = 0U;

	/* get the data from the BKP data register */
	if ((register_number >= BKP_DATA_10) &&
			(register_number <= BKP_DATA_41)) {
		data = BKP_DATA10_41(register_number - 1U);
	} else if ((register_number >= BKP_DATA_0) &&
			(register_number <= BKP_DATA_9)) {
		data = BKP_DATA0_9(register_number - 1U);
	} else {
		/* illegal parameters */
	}
	return data;
}

/*
 * enable RTC clock calibration output
 */
void bkp_rtc_calibration_output_enable(void) {
	BKP_OCTL |= (uint16_t) BKP_OCTL_COEN;
}

/*
 * disable RTC clock calibration output
 */
void bkp_rtc_calibration_output_disable(void) {
	BKP_OCTL &= (uint16_t) ~ BKP_OCTL_COEN;
}

/*
 * enable RTC alarm or second signal output
 */
void bkp_rtc_signal_output_enable(void) {
	BKP_OCTL |= (uint16_t) BKP_OCTL_ASOEN;
}

/*
 * disable RTC alarm or second signal output
 */
void bkp_rtc_signal_output_disable(void) {
	BKP_OCTL &= (uint16_t) ~ BKP_OCTL_ASOEN;
}

/*
 * select RTC output
 * outputsel: RTC output selection
 *	RTC_OUTPUT_ALARM_PULSE:
 * 		RTC alarm pulse is selected as the RTC output
 *
 * 	RTC_OUTPUT_SECOND_PULSE:
 * 		RTC second pulse is selected as the RTC output
 *
 */
void bkp_rtc_output_select(uint16_t outputsel) {
	uint16_t ctl = 0U;

	/* configure BKP_OCTL_ROSEL with outputsel */
	ctl = BKP_OCTL;
	ctl &= (uint16_t) ~ BKP_OCTL_ROSEL;
	ctl |= outputsel;
	BKP_OCTL = ctl;
}

/*
 * set RTC clock calibration value
 * value: RTC clock calibration value 0x00 - 0x7F
 */
void bkp_rtc_calibration_value_set(uint8_t value) {
	uint16_t ctl;

	/* configure BKP_OCTL_RCCV with value */
	ctl = BKP_OCTL;
	ctl &= (uint16_t) ~ BKP_OCTL_RCCV;
	ctl |= (uint16_t) OCTL_RCCV(value);
	BKP_OCTL = ctl;
}

/*
 * enable tamper detection
 */
void bkp_tamper_detection_enable(void) {
	BKP_TPCTL |= (uint16_t) BKP_TPCTL_TPEN;
}

/*
 * disable tamper detection
 */
void bkp_tamper_detection_disable(void) {
	BKP_TPCTL &= (uint16_t) ~ BKP_TPCTL_TPEN;
}

/*
 * set tamper pin active level
 * level: tamper active level
 * 	TAMPER_PIN_ACTIVE_HIGH: the tamper pin is active high
 * 	TAMPER_PIN_ACTIVE_LOW: the tamper pin is active low
 */
void bkp_tamper_active_level_set(uint16_t level) {
	uint16_t ctl = 0U;

	/* configure BKP_TPCTL_TPAL with level */
	ctl = BKP_TPCTL;
	ctl &= (uint16_t) ~ BKP_TPCTL_TPAL;
	ctl |= level;
	BKP_TPCTL = ctl;
}

/*
 * enable tamper interrupt
 */
void bkp_interrupt_enable(void) {
	BKP_TPCS |= (uint16_t) BKP_TPCS_TPIE;
}

/*
 * disable tamper interrupt
 */
void bkp_interrupt_disable(void) {
	BKP_TPCS &= (uint16_t) ~ BKP_TPCS_TPIE;
}

/*
 * get tamper flag state
 */
enum flag_status bkp_flag_get(void) {
	if (RESET != (BKP_TPCS & BKP_FLAG_TAMPER))
		return SET;
	else
		return RESET;
}

/*
 * clear tamper flag state
 */
void bkp_flag_clear(void) {
	BKP_TPCS |= (uint16_t) (BKP_FLAG_TAMPER >> BKP_TAMPER_BITS_OFFSET);
}

/*
 * get tamper interrupt flag state
 */
enum flag_status bkp_interrupt_flag_get(void) {
	if (RESET != (BKP_TPCS & BKP_INT_FLAG_TAMPER))
		return SET;
	else
		return RESET;
}

/*
 * clear tamper interrupt flag state
 */
void bkp_interrupt_flag_clear(void) {
	BKP_TPCS |= (uint16_t) (BKP_INT_FLAG_TAMPER >> BKP_TAMPER_BITS_OFFSET);
}
