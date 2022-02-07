#include "gd32vf103_exti.h"

#define EXTI_REG_RESET_VALUE            ((uint32_t)0x00000000U)

/*!
 \brief      deinitialize the EXTI
 \param[in]  none
 \param[out] none
 \retval     none
 */
void exti_deinit(void) {
	/* reset the value of all the EXTI registers */
	EXTI_INTEN = EXTI_REG_RESET_VALUE;
	EXTI_EVEN = EXTI_REG_RESET_VALUE;
	EXTI_RTEN = EXTI_REG_RESET_VALUE;
	EXTI_FTEN = EXTI_REG_RESET_VALUE;
	EXTI_SWIEV = EXTI_REG_RESET_VALUE;
}

/*!
    \brief      initialize the EXTI
    \param[in]  linex: EXTI line number, refer to enum exti_line
                only one parameter can be selected which is shown as below:
      \arg        EXTI_x (x=0..18): EXTI line x
    \param[in]  mode: interrupt or event mode, refer to enum exti_mode
                only one parameter can be selected which is shown as below:
      \arg        EXTI_INTERRUPT: interrupt mode
      \arg        EXTI_EVENT: event mode
    \param[in]  trig_type: trigger type, refer to enum exti_trig_type
                only one parameter can be selected which is shown as below:
      \arg        EXTI_TRIG_RISING: rising edge trigger
      \arg        EXTI_TRIG_FALLING: falling edge trigger
      \arg        EXTI_TRIG_BOTH: rising edge and falling edge trigger
      \arg        EXTI_TRIG_NONE: without rising edge or falling edge trigger
    \param[out] none
    \retval     none
 */
void exti_init(enum exti_line linex, enum exti_mode mode,
	       enum exti_trig_type trig_type) {
	/* reset the EXTI line x */
	EXTI_INTEN &= ~(uint32_t) linex;
	EXTI_EVEN &= ~(uint32_t) linex;
	EXTI_RTEN &= ~(uint32_t) linex;
	EXTI_FTEN &= ~(uint32_t) linex;

	/* set the EXTI mode and enable the interrupts or events from EXTI line x */
	switch (mode) {
	case EXTI_INTERRUPT:
		EXTI_INTEN |= (uint32_t) linex;
		break;
	case EXTI_EVENT:
		EXTI_EVEN |= (uint32_t) linex;
		break;
	default:
		break;
	}

	/* set the EXTI trigger type */
	switch (trig_type) {
	case EXTI_TRIG_RISING:
		EXTI_RTEN |= (uint32_t) linex;
		EXTI_FTEN &= ~(uint32_t) linex;
		break;
	case EXTI_TRIG_FALLING:
		EXTI_RTEN &= ~(uint32_t) linex;
		EXTI_FTEN |= (uint32_t) linex;
		break;
	case EXTI_TRIG_BOTH:
		EXTI_RTEN |= (uint32_t) linex;
		EXTI_FTEN |= (uint32_t) linex;
		break;
	case EXTI_TRIG_NONE:
	default:
		break;
	}
}

/*!
    \brief      enable the interrupts from EXTI line x
    \param[in]  linex: EXTI line number, refer to enum exti_line
                only one parameter can be selected which is shown as below:
      \arg        EXTI_x (x=0..18): EXTI line x
    \param[out] none
    \retval     none
*/
void exti_interrupt_enable(enum exti_line linex) {
	EXTI_INTEN |= (uint32_t) linex;
}

/*!
    \brief      enable the events from EXTI line x
    \param[in]  linex: EXTI line number, refer to enum exti_line
                only one parameter can be selected which is shown as below:
      \arg        EXTI_x (x=0..18): EXTI line x
    \param[out] none
    \retval     none
*/
void exti_event_enable(enum exti_line linex) {
	EXTI_EVEN |= (uint32_t) linex;
}

/*!
    \brief      disable the interrupt from EXTI line x
    \param[in]  linex: EXTI line number, refer to enum exti_line
                only one parameter can be selected which is shown as below:
      \arg        EXTI_x (x=0..18): EXTI line x
    \param[out] none
    \retval     none
*/
void exti_interrupt_disable(enum exti_line linex) {
	EXTI_INTEN &= ~(uint32_t) linex;
}

/*!
    \brief      disable the events from EXTI line x
    \param[in]  linex: EXTI line number, refer to enum exti_line
                only one parameter can be selected which is shown as below:
      \arg        EXTI_x (x=0..18): EXTI line x
    \param[out] none
    \retval     none
*/
void exti_event_disable(enum exti_line linex) {
	EXTI_EVEN &= ~(uint32_t) linex;
}

/*!
    \brief      get EXTI lines flag
    \param[in]  linex: EXTI line number, refer to enum exti_line
                only one parameter can be selected which is shown as below:
      \arg        EXTI_x (x=0..18): EXTI line x
    \param[out] none
    \retval     enum flag_status: status of flag (RESET or SET)
*/
enum flag_status exti_flag_get(enum exti_line linex) {
	if (RESET != (EXTI_PD & (uint32_t) linex)) {
		return SET;
	} else {
		return RESET;
	}
}

/*!
    \brief      clear EXTI lines pending flag
    \param[in]  linex: EXTI line number, refer to enum exti_line
                only one parameter can be selected which is shown as below:
      \arg        EXTI_x (x=0..18): EXTI line x
    \param[out] none
    \retval     none
*/
void exti_flag_clear(enum exti_line linex) {
	EXTI_PD = (uint32_t) linex;
}

/*!
    \brief      get EXTI lines flag when the interrupt flag is set
    \param[in]  linex: EXTI line number, refer to enum exti_line
                only one parameter can be selected which is shown as below:
      \arg        EXTI_x (x=0..18): EXTI line x
    \param[out] none
    \retval     enum flag_status: status of flag (RESET or SET)
*/
enum flag_status exti_interrupt_flag_get(enum exti_line linex) {
	uint32_t flag_left, flag_right;

	flag_left = EXTI_PD & (uint32_t) linex;
	flag_right = EXTI_INTEN & (uint32_t) linex;

	if ((RESET != flag_left) && (RESET != flag_right)) {
		return SET;
	} else {
		return RESET;
	}
}

/*!
    \brief      clear EXTI lines pending flag
    \param[in]  linex: EXTI line number, refer to enum exti_line
                only one parameter can be selected which is shown as below:
      \arg        EXTI_x (x=0..18): EXTI line x
    \param[out] none
    \retval     none
*/
void exti_interrupt_flag_clear(enum exti_line linex) {
	EXTI_PD = (uint32_t) linex;
}

/*!
    \brief      enable EXTI software interrupt event
    \param[in]  linex: EXTI line number, refer to enum exti_line
                only one parameter can be selected which is shown as below:
      \arg        EXTI_x (x=0..18): EXTI line x
    \param[out] none
    \retval     none
*/
void exti_software_interrupt_enable(enum exti_line linex) {
	EXTI_SWIEV |= (uint32_t) linex;
}

/*!
    \brief      disable EXTI software interrupt event
    \param[in]  linex: EXTI line number, refer to enum exti_line
                only one parameter can be selected which is shown as below:
      \arg        EXTI_x (x=0..18): EXTI line x
    \param[out] none
    \retval     none
*/
void exti_software_interrupt_disable(enum exti_line linex) {
	EXTI_SWIEV &= ~(uint32_t) linex;
}
