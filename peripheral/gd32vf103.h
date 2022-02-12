#ifndef __GD32VF103_H__
#define __GD32VF103_H__

#ifdef LONGON_NANO
#define HXTAL_VALUE		((uint32_t) 8000000)
#define LXTAL_VALUE		((uint32_t) 32768)
#else
#error "HXTAL_VALUE and LXTAL_VALUE is not given, or board type is unknown."
#endif

// define startup timeout value of high speed crystal oscillator (HXTAL)
#ifndef HXTAL_STARTUP_TIMEOUT
#define HXTAL_STARTUP_TIMEOUT	((uint16_t) 0xFFFF)
#endif

// define startup timeout value of internal 8MHz RC oscillator (IRC8M)
#ifndef IRC8M_STARTUP_TIMEOUT
#define IRC8M_STARTUP_TIMEOUT	((uint16_t) 0x0500)
#endif

// define value of internal 8MHz RC oscillator (IRC8M) in Hz
#ifndef IRC8M_VALUE
#define IRC8M_VALUE		((uint32_t) 8000000)
#endif

// define value of internal 40KHz RC oscillator (IRC40K) in Hz
#ifndef IRC40K_VALUE
#define IRC40K_VALUE		((uint32_t) 40000)
#endif

enum IRQn {
	// RISC-V reserved
	CLIC_INT_RESERVED	= 0,

	// Software interrupt
	CLIC_INT_SFT		= 3,

	// CPU Timer interrupt
	CLIC_INT_TMR		= 7,

	// Bus Error interrupt
	CLIC_INT_BWEI		= 17,

	// Performance Monitor
	CLIC_INT_PMOVI		= 18,

	// interruput numbers

	// window watchDog timer interrupt
	WWDGT_IRQn		= 19,

	// LVD through EXTI line detect interrupt
	LVD_IRQn		= 20,

	// tamper through EXTI line detect
	TAMPER_IRQn		= 21,

	// RTC alarm interrupt
	RTC_IRQn		= 22,

	FMC_IRQn		= 23,
	RCU_CTC_IRQn		= 24,
	EXTI0_IRQn		= 25,
	EXTI1_IRQn		= 26,
	EXTI2_IRQn		= 27,
	EXTI3_IRQn		= 28,
	EXTI4_IRQn		= 29,
	DMA0_Channel0_IRQn	= 30,
	DMA0_Channel1_IRQn	= 31,
	DMA0_Channel2_IRQn	= 32,
	DMA0_Channel3_IRQn	= 33,
	DMA0_Channel4_IRQn	= 34,
	DMA0_Channel5_IRQn	= 35,
	DMA0_Channel6_IRQn	= 36,
	ADC0_1_IRQn		= 37,
	CAN0_TX_IRQn		= 38,
	CAN0_RX0_IRQn		= 39,
	CAN0_RX1_IRQn		= 40,
	CAN0_EWMC_IRQn		= 41,
	EXTI5_9_IRQn		= 42,

	// TIMER0 break interrupts
	TIMER0_BRK_IRQn		= 43,

	// TIMER0 update interrupts
	TIMER0_UP_IRQn		= 44,

	// TIMER0 trigger and commutation interrupts
	TIMER0_TRG_CMT_IRQn	= 45,

	// TIMER0 channel capture compare interrupts
	TIMER0_Channel_IRQn	= 46,

	TIMER1_IRQn		= 47,
	TIMER2_IRQn		= 48,
	TIMER3_IRQn		= 49,
	I2C0_EV_IRQn		= 50,
	I2C0_ER_IRQn		= 51,
	I2C1_EV_IRQn		= 52,
	I2C1_ER_IRQn		= 53,
	SPI0_IRQn		= 54,
	SPI1_IRQn		= 55,
	USART0_IRQn		= 56,
	USART1_IRQn		= 57,
	USART2_IRQn		= 58,
	EXTI10_15_IRQn		= 59,
	RTC_ALARM_IRQn		= 60,
	USBFS_WKUP_IRQn		= 61,

	EXMC_IRQn		= 67,

	TIMER4_IRQn		= 69,
	SPI2_IRQn		= 70,
	UART3_IRQn		= 71,
	UART4_IRQn		= 72,
	TIMER5_IRQn		= 73,
	TIMER6_IRQn		= 74,
	DMA1_Channel0_IRQn	= 75,
	DMA1_Channel1_IRQn	= 76,
	DMA1_Channel2_IRQn	= 77,
	DMA1_Channel3_IRQn	= 78,
	DMA1_Channel4_IRQn	= 79,

	CAN1_TX_IRQn		= 82,
	CAN1_RX0_IRQn		= 83,
	CAN1_RX1_IRQn		= 84,
	CAN1_EWMC_IRQn		= 85,
	USBFS_IRQn		= 86,

	ECLIC_NUM_INTERRUPTS
};

#include "system_gd32vf103.h"
#include <stdint.h>

enum control_status {DISABLE = 0, ENABLE = !DISABLE};
enum event_status {DISABLE = 0, ENABLE = !DISABLE};
enum flag_status {RESET = 0, SET = 1, MAX = 0x7FFFFFFF};
enum err_status {ERROR = 0, SUCCESS = !ERROR};

#define REG32(addr) (* (volatile uint32_t *) (uint32_t) (addr))
#define REG16(addr) (* (volatile uint16_t *) (uint32_t) (addr))
#define REG8(addr) (* (volatile uint8_t *) (uint32_t) (addr))

#define BIT(x) ((uint32_t) ((uint32_t) 1 << (x)))

#define BITS(start, end) \
	((0xFFFFFFFFUL << (start)) & (0xFFFFFFFFUL >> (31 - (uint32_t) (end))))

#define GET_BITS(regval, start, end) \
	(((regval) & BITS((start), (end))) >> (start))

// main flash and SRAM memory map
#define FLASH_BASE	((uint32_t) 0x08000000U)
#define SRAM_BASE	((uint32_t) 0x20000000U) // SRAM0 base address
#define OB_BASE		((uint32_t) 0x1FFFF800U)
#define DBG_BASE	((uint32_t) 0xE0042000U)
#define EXMC_BASE	((uint32_t) 0xA0000000U)

// peripheral memory map
#define APB1_BUS_BASE	((uint32_t) 0x40000000U)
#define APB2_BUS_BASE	((uint32_t) 0x40010000U)
#define AHB1_BUS_BASE	((uint32_t) 0x40018000U)
#define AHB3_BUS_BASE	((uint32_t) 0x60000000U)

// advanced peripheral bus 1 memory map
#define TIMER_BASE	(APB1_BUS_BASE + 0x00000000U)
#define RTC_BASE	(APB1_BUS_BASE + 0x00002800U)
#define WWDGT_BASE	(APB1_BUS_BASE + 0x00002C00U)
#define FWDGT_BASE	(APB1_BUS_BASE + 0x00003000U)
#define SPI_BASE	(APB1_BUS_BASE + 0x00003800U)
#define USART_BASE	(APB1_BUS_BASE + 0x00004400U)
#define I2C_BASE	(APB1_BUS_BASE + 0x00005400U)
#define CAN_BASE	(APB1_BUS_BASE + 0x00006400U)
#define BKP_BASE	(APB1_BUS_BASE + 0x00006C00U)
#define PMU_BASE	(APB1_BUS_BASE + 0x00007000U)
#define DAC_BASE	(APB1_BUS_BASE + 0x00007400U)

// advanced peripheral bus 2 memory map
#define AFIO_BASE	(APB2_BUS_BASE + 0x00000000U)
#define EXTI_BASE	(APB2_BUS_BASE + 0x00000400U)
#define GPIO_BASE	(APB2_BUS_BASE + 0x00000800U)
#define ADC_BASE	(APB2_BUS_BASE + 0x00002400U)

// advanced high performance bus 1 memory map
#define DMA_BASE	(AHB1_BUS_BASE + 0x00008000U)
#define RCU_BASE	(AHB1_BUS_BASE + 0x00009000U)
#define FMC_BASE	(AHB1_BUS_BASE + 0x0000A000U)
#define CRC_BASE	(AHB1_BUS_BASE + 0x0000B000U)
#define USBFS_BASE	(AHB1_BUS_BASE + 0x0FFE8000U)

