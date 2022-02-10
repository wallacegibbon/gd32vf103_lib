#include "gd32vf103_exti.h"

#define EXTI_REG_RESET_VALUE            ((uint32_t) 0x00000000U)

void exti_deinit(void) {
	EXTI_INTEN = EXTI_REG_RESET_VALUE;
	EXTI_EVEN = EXTI_REG_RESET_VALUE;
	EXTI_RTEN = EXTI_REG_RESET_VALUE;
	EXTI_FTEN = EXTI_REG_RESET_VALUE;
	EXTI_SWIEV = EXTI_REG_RESET_VALUE;
}

void exti_init(enum exti_line linex, enum exti_mode mode,
		enum exti_trig_type trig_type) {

	EXTI_INTEN &= ~linex;
	EXTI_EVEN &= ~linex;
	EXTI_RTEN &= ~linex;
	EXTI_FTEN &= ~linex;

	switch (mode) {
	case EXTI_INTERRUPT:
		EXTI_INTEN |= linex;
		break;
	case EXTI_EVENT:
		EXTI_EVEN |= linex;
		break;
	default:
		break;
	}

	switch (trig_type) {
	case EXTI_TRIG_RISING:
		EXTI_RTEN |= linex;
		EXTI_FTEN &= ~linex;
		break;
	case EXTI_TRIG_FALLING:
		EXTI_RTEN &= ~linex;
		EXTI_FTEN |= linex;
		break;
	case EXTI_TRIG_BOTH:
		EXTI_RTEN |= linex;
		EXTI_FTEN |= linex;
		break;
	case EXTI_TRIG_NONE:
	default:
		break;
	}
}

void exti_interrupt_enable(enum exti_line linex) {
	EXTI_INTEN |= linex;
}

void exti_event_enable(enum exti_line linex) {
	EXTI_EVEN |= linex;
}

void exti_interrupt_disable(enum exti_line linex) {
	EXTI_INTEN &= ~linex;
}

void exti_event_disable(enum exti_line linex) {
	EXTI_EVEN &= ~linex;
}

enum flag_status exti_flag_get(enum exti_line linex) {
	if (EXTI_PD & linex != RESET)
		return SET;
	else
		return RESET;
}

void exti_flag_clear(enum exti_line linex) {
	EXTI_PD = linex;
}

enum flag_status exti_interrupt_flag_get(enum exti_line linex) {
	if ((EXTI_PD & linex != RESET) && (EXTI_INTEN & linex != RESET))
		return SET;
	else
		return RESET;
}

void exti_interrupt_flag_clear(enum exti_line linex) {
	EXTI_PD = linex;
}

void exti_software_interrupt_enable(enum exti_line linex) {
	EXTI_SWIEV |= linex;
}

void exti_software_interrupt_disable(enum exti_line linex) {
	EXTI_SWIEV &= ~linex;
}
