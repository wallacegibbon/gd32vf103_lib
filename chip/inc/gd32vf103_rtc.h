#ifndef __GD32VF103_RTC_H
#define __GD32VF103_RTC_H

#include "gd32vf103.h"

#define RTC			RTC_BASE

#define RTC_INTEN		REG32(RTC + 0x00U) // interrupt enable register
#define RTC_CTL			REG32(RTC + 0x04U) // control register
#define RTC_PSCH		REG32(RTC + 0x08U) // prescaler high register
#define RTC_PSCL		REG32(RTC + 0x0CU) // prescaler low register
#define RTC_DIVH		REG32(RTC + 0x10U) // divider high register
#define RTC_DIVL		REG32(RTC + 0x14U) // divider low register
#define RTC_CNTH		REG32(RTC + 0x18U) // counter high register
#define RTC_CNTL		REG32(RTC + 0x1CU) // counter low register
#define RTC_ALRMH		REG32(RTC + 0x20U) // alarm high register
#define RTC_ALRML		REG32(RTC + 0x24U) // alarm low register

#define RTC_INTEN_SCIE		BIT(0) // second interrupt enable
#define RTC_INTEN_ALRMIE	BIT(1) // alarm interrupt enable
#define RTC_INTEN_OVIE		BIT(2) // overflow interrupt enable

#define RTC_CTL_SCIF		BIT(0) // second interrupt flag
#define RTC_CTL_ALRMIF		BIT(1) // alarm interrupt flag
#define RTC_CTL_OVIF		BIT(2) // overflow interrupt flag
#define RTC_CTL_RSYNF		BIT(3) // registers synchronized flag
#define RTC_CTL_CMF		BIT(4) // configuration mode flag
#define RTC_CTL_LWOFF		BIT(5) // last write operation finished flag

#define RTC_PSCH_PSC		BITS(0, 3) // prescaler high value
#define RTC_PSCL_PSC		BITS(0, 15) // prescaler low value

#define RTC_DIVH_DIV		BITS(0, 3) // divider high value
#define RTC_DIVL_DIV		BITS(0, 15) // divider low value

#define RTC_CNTH_CNT		BITS(0, 15) // counter high value
#define RTC_CNTL_CNT		BITS(0, 15) // counter low value

#define RTC_ALRMH_ALRM		BITS(0, 15) // alarm high value
#define RTC_ALRML_ALRM		BITS(0, 15) // alarm low value

// RTC interrupt enable or disable definitions

#define RTC_INT_SECOND		RTC_INTEN_SCIE // second interrupt enable
#define RTC_INT_ALARM		RTC_INTEN_ALRMIE // alarm interrupt enable
#define RTC_INT_OVERFLOW	RTC_INTEN_OVIE // overflow interrupt enable

// RTC interrupt flag definitions
#define RTC_INT_FLAG_SECOND	RTC_CTL_SCIF // second interrupt flag
#define RTC_INT_FLAG_ALARM	RTC_CTL_ALRMIF // alarm interrupt flag
#define RTC_INT_FLAG_OVERFLOW	RTC_CTL_OVIF // overflow interrupt flag

// RTC flag definitions
#define RTC_FLAG_SECOND		RTC_CTL_SCIF // second interrupt flag
#define RTC_FLAG_ALARM		RTC_CTL_ALRMIF // alarm interrupt flag
#define RTC_FLAG_OVERFLOW	RTC_CTL_OVIF // overflow interrupt flag
#define RTC_FLAG_RSYN		RTC_CTL_RSYNF // registers synchronized flag
#define RTC_FLAG_LWOF		RTC_CTL_LWOFF // last write operation finished flag

// initialization functions
void rtc_configuration_mode_enter(void);
void rtc_configuration_mode_exit(void);
void rtc_counter_set(uint32_t cnt);
void rtc_prescaler_set(uint32_t psc);

// operation functions
void rtc_lwoff_wait(void);
void rtc_register_sync_wait(void);
void rtc_alarm_config(uint32_t alarm);
uint32_t rtc_counter_get(void);
uint32_t rtc_divider_get(void);

// flag & interrupt functions
enum flag_status rtc_flag_get(uint32_t flag);
void rtc_flag_clear(uint32_t flag);

enum flag_status rtc_interrupt_flag_get(uint32_t flag);
void rtc_interrupt_flag_clear(uint32_t flag);

void rtc_interrupt_enable(uint32_t interrupt);
void rtc_interrupt_disable(uint32_t interrupt);

#endif