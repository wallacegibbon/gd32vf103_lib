#include "gd32vf103_rtc.h"

#define RTC_HIGH_BITS_MASK	((uint32_t) 0x000F0000U)
#define RTC_LOW_BITS_MASK	((uint32_t) 0x0000FFFFU)

#define RTC_HIGH_BITS_OFFSET	((uint32_t) 16U)

void rtc_configuration_mode_enter() {
	RTC_CTL |= RTC_CTL_CMF;
}

void rtc_configuration_mode_exit() {
	RTC_CTL &= ~RTC_CTL_CMF;
}

void rtc_counter_set(uint32_t cnt) {
	rtc_configuration_mode_enter();
	RTC_CNTH = (cnt >> RTC_HIGH_BITS_OFFSET);
	RTC_CNTL = (cnt & RTC_LOW_BITS_MASK);
	rtc_configuration_mode_exit();
}

void rtc_prescaler_set(uint32_t psc) {
	rtc_configuration_mode_enter();
	RTC_PSCH = ((psc & RTC_HIGH_BITS_MASK) >> RTC_HIGH_BITS_OFFSET);
	RTC_PSCL = (psc & RTC_LOW_BITS_MASK);
	rtc_configuration_mode_exit();
}

void rtc_lwoff_wait() {
	while (RTC_CTL & RTC_CTL_LWOFF == RESET);
}

/* wait RTC registers synchronized flag set */
void rtc_register_sync_wait() {
	RTC_CTL &= ~RTC_CTL_RSYNF;
	while (RTC_CTL & RTC_CTL_RSYNF == RESET);
}

void rtc_alarm_config(uint32_t alarm) {
	rtc_configuration_mode_enter();
	RTC_ALRMH = (alarm >> RTC_HIGH_BITS_OFFSET);
	RTC_ALRML = (alarm & RTC_LOW_BITS_MASK);
	rtc_configuration_mode_exit();
}

uint32_t rtc_counter_get() {
	uint32_t temp = RTC_CNTL;
	return temp | (RTC_CNTH << RTC_HIGH_BITS_OFFSET);
}

uint32_t rtc_divider_get() {
	uint32_t temp = (RTC_DIVH & RTC_DIVH_DIV) << RTC_HIGH_BITS_OFFSET;
	return temp | RTC_DIVL;
}

/*
 * flag: specify which flag status to get
 * 	RTC_FLAG_SECOND: second interrupt flag
 * 	RTC_FLAG_ALARM: alarm interrupt flag
 * 	RTC_FLAG_OVERFLOW: overflow interrupt flag
 * 	RTC_FLAG_RSYN: registers synchronized flag
 * 	RTC_FLAG_LWOF: last write operation finished flag
 */
enum flag_status rtc_flag_get(uint32_t flag) {
	if (RTC_CTL & flag != RESET)
		return SET;
	else
		return RESET;
}

void rtc_flag_clear(uint32_t flag) {
	RTC_CTL &= ~flag;
}

/*
 * flag: specify which flag status to get
 * 	RTC_INT_FLAG_SECOND: second interrupt flag
 * 	RTC_INT_FLAG_ALARM: alarm interrupt flag
 * 	RTC_INT_FLAG_OVERFLOW: overflow interrupt flag
 */
enum flag_status rtc_interrupt_flag_get(uint32_t flag) {
	if (RTC_CTL & flag != RESET)
		return SET;
	else
		return RESET;
}

void rtc_interrupt_flag_clear(uint32_t flag) {
	RTC_CTL &= ~flag;
}

/*
 * interrupt: specify which interrupt to enbale
 * 	RTC_INT_SECOND: second interrupt
 * 	RTC_INT_ALARM: alarm interrupt
 * 	RTC_INT_OVERFLOW: overflow interrupt
*/
void rtc_interrupt_enable(uint32_t interrupt) {
	RTC_INTEN |= interrupt;
}

void rtc_interrupt_disable(uint32_t interrupt) {
	RTC_INTEN &= ~interrupt;
}
