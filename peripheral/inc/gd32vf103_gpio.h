#ifndef GD32VF103_GPIO_H
#define GD32VF103_GPIO_H

#include "gd32vf103.h"

#define GPIOA				(GPIO_BASE + 0x00000000U)
#define GPIOB				(GPIO_BASE + 0x00000400U)
#define GPIOC				(GPIO_BASE + 0x00000800U)
#define GPIOD				(GPIO_BASE + 0x00000C00U)
#define GPIOE				(GPIO_BASE + 0x00001000U)

#define AFIO				AFIO_BASE


#define GPIO_CTL0(gpiox)		REG32((gpiox) + 0x00U)
#define GPIO_CTL1(gpiox)		REG32((gpiox) + 0x04U)
#define GPIO_ISTAT(gpiox)		REG32((gpiox) + 0x08U)
#define GPIO_OCTL(gpiox)		REG32((gpiox) + 0x0CU)
// GPIO port bit operation register
#define GPIO_BOP(gpiox)			REG32((gpiox) + 0x10U)
// GPIO bit clear register
#define GPIO_BC(gpiox)			REG32((gpiox) + 0x14U)
// GPIO port configuration lock register
#define GPIO_LOCK(gpiox)		REG32((gpiox) + 0x18U)

// AFIO event control register
#define AFIO_EC				REG32(AFIO + 0x00U)
// AFIO port configuration register 0
#define AFIO_PCF0			REG32(AFIO + 0x04U)
#define AFIO_EXTISS0			REG32(AFIO + 0x08U)
#define AFIO_EXTISS1			REG32(AFIO + 0x0CU)
#define AFIO_EXTISS2			REG32(AFIO + 0x10U)
#define AFIO_EXTISS3			REG32(AFIO + 0x14U)
// AFIO port configuration register 1
#define AFIO_PCF1			REG32(AFIO + 0x1CU)

#define GPIO_CTL0_MD0			BITS(0, 1) // port 0 mode bits
#define GPIO_CTL0_CTL0			BITS(2, 3) // pin 0 configuration bits
#define GPIO_CTL0_MD1			BITS(4, 5) // port 1 mode bits
#define GPIO_CTL0_CTL1			BITS(6, 7) // pin 1 configuration bits
#define GPIO_CTL0_MD2			BITS(8, 9) 
#define GPIO_CTL0_CTL2			BITS(10, 11)
#define GPIO_CTL0_MD3			BITS(12, 13)
#define GPIO_CTL0_CTL3			BITS(14, 15)
#define GPIO_CTL0_MD4			BITS(16, 17)
#define GPIO_CTL0_CTL4			BITS(18, 19)
#define GPIO_CTL0_MD5			BITS(20, 21)
#define GPIO_CTL0_CTL5			BITS(22, 23)
#define GPIO_CTL0_MD6			BITS(24, 25)
#define GPIO_CTL0_CTL6			BITS(26, 27)
#define GPIO_CTL0_MD7			BITS(28, 29)
#define GPIO_CTL0_CTL7			BITS(30, 31)

#define GPIO_CTL1_MD8			BITS(0, 1)
#define GPIO_CTL1_CTL8			BITS(2, 3)
#define GPIO_CTL1_MD9			BITS(4, 5)
#define GPIO_CTL1_CTL9			BITS(6, 7)
#define GPIO_CTL1_MD10			BITS(8, 9)
#define GPIO_CTL1_CTL10			BITS(10, 11)
#define GPIO_CTL1_MD11			BITS(12, 13)
#define GPIO_CTL1_CTL11			BITS(14, 15)
#define GPIO_CTL1_MD12			BITS(16, 17)
#define GPIO_CTL1_CTL12			BITS(18, 19)
#define GPIO_CTL1_MD13			BITS(20, 21)
#define GPIO_CTL1_CTL13			BITS(22, 23)
#define GPIO_CTL1_MD14			BITS(24, 25)
#define GPIO_CTL1_CTL14			BITS(26, 27)
#define GPIO_CTL1_MD15			BITS(28, 29)
#define GPIO_CTL1_CTL15			BITS(30, 31)

#define GPIO_ISTAT_ISTAT0		BIT(0) // pin 0 input status
#define GPIO_ISTAT_ISTAT1		BIT(1) // pin 1 input status
#define GPIO_ISTAT_ISTAT2		BIT(2)
#define GPIO_ISTAT_ISTAT3		BIT(3)
#define GPIO_ISTAT_ISTAT4		BIT(4)
#define GPIO_ISTAT_ISTAT5		BIT(5)
#define GPIO_ISTAT_ISTAT6		BIT(6)
#define GPIO_ISTAT_ISTAT7		BIT(7)
#define GPIO_ISTAT_ISTAT8		BIT(8)
#define GPIO_ISTAT_ISTAT9		BIT(9)
#define GPIO_ISTAT_ISTAT10		BIT(10)
#define GPIO_ISTAT_ISTAT11		BIT(11)
#define GPIO_ISTAT_ISTAT12		BIT(12)
#define GPIO_ISTAT_ISTAT13		BIT(13)
#define GPIO_ISTAT_ISTAT14		BIT(14)
#define GPIO_ISTAT_ISTAT15		BIT(15)

#define GPIO_OCTL_OCTL0			BIT(0) // pin 0 output bit
#define GPIO_OCTL_OCTL1			BIT(1) // pin 1 output bit
#define GPIO_OCTL_OCTL2			BIT(2)
#define GPIO_OCTL_OCTL3			BIT(3)
#define GPIO_OCTL_OCTL4			BIT(4)
#define GPIO_OCTL_OCTL5			BIT(5)
#define GPIO_OCTL_OCTL6			BIT(6)
#define GPIO_OCTL_OCTL7			BIT(7)
#define GPIO_OCTL_OCTL8			BIT(8)
#define GPIO_OCTL_OCTL9			BIT(9)
#define GPIO_OCTL_OCTL10		BIT(10)
#define GPIO_OCTL_OCTL11		BIT(11)
#define GPIO_OCTL_OCTL12		BIT(12)
#define GPIO_OCTL_OCTL13		BIT(13)
#define GPIO_OCTL_OCTL14		BIT(14)
#define GPIO_OCTL_OCTL15		BIT(15)

#define GPIO_BOP_BOP0			BIT(0) // pin 0 set bit
#define GPIO_BOP_BOP1			BIT(1) // pin 1 set bit
#define GPIO_BOP_BOP2			BIT(2)
#define GPIO_BOP_BOP3			BIT(3)
#define GPIO_BOP_BOP4			BIT(4)
#define GPIO_BOP_BOP5			BIT(5)
#define GPIO_BOP_BOP6			BIT(6)
#define GPIO_BOP_BOP7			BIT(7)
#define GPIO_BOP_BOP8			BIT(8)
#define GPIO_BOP_BOP9			BIT(9)
#define GPIO_BOP_BOP10			BIT(10)
#define GPIO_BOP_BOP11			BIT(11)
#define GPIO_BOP_BOP12			BIT(12)
#define GPIO_BOP_BOP13			BIT(13)
#define GPIO_BOP_BOP14			BIT(14)
#define GPIO_BOP_BOP15			BIT(15)

#define GPIO_BOP_CR0			BIT(16) // pin 0 clear bit
#define GPIO_BOP_CR1			BIT(17) // pin 1 clear bit
#define GPIO_BOP_CR2			BIT(18)
#define GPIO_BOP_CR3			BIT(19)
#define GPIO_BOP_CR4			BIT(20)
#define GPIO_BOP_CR5			BIT(21)
#define GPIO_BOP_CR6			BIT(22)
#define GPIO_BOP_CR7			BIT(23)
#define GPIO_BOP_CR8			BIT(24)
#define GPIO_BOP_CR9			BIT(25)
#define GPIO_BOP_CR10			BIT(26)
#define GPIO_BOP_CR11			BIT(27)
#define GPIO_BOP_CR12			BIT(28)
#define GPIO_BOP_CR13			BIT(29)
#define GPIO_BOP_CR14			BIT(30)
#define GPIO_BOP_CR15			BIT(31)

#define GPIO_BC_CR0			BIT(0) // pin 0 clear bit
#define GPIO_BC_CR1			BIT(1) // pin 1 clear bit
#define GPIO_BC_CR2			BIT(2)
#define GPIO_BC_CR3			BIT(3)
#define GPIO_BC_CR4			BIT(4)
#define GPIO_BC_CR5			BIT(5)
#define GPIO_BC_CR6			BIT(6)
#define GPIO_BC_CR7			BIT(7)
#define GPIO_BC_CR8			BIT(8)
#define GPIO_BC_CR9			BIT(9)
#define GPIO_BC_CR10			BIT(10)
#define GPIO_BC_CR11			BIT(11)
#define GPIO_BC_CR12			BIT(12)
#define GPIO_BC_CR13			BIT(13)
#define GPIO_BC_CR14			BIT(14)
#define GPIO_BC_CR15			BIT(15)

#define GPIO_LOCK_LK0			BIT(0) // pin 0 lock bit
#define GPIO_LOCK_LK1			BIT(1) // pin 1 lock bit
#define GPIO_LOCK_LK2			BIT(2)
#define GPIO_LOCK_LK3			BIT(3)
#define GPIO_LOCK_LK4			BIT(4)
#define GPIO_LOCK_LK5			BIT(5)
#define GPIO_LOCK_LK6			BIT(6)
#define GPIO_LOCK_LK7			BIT(7)
#define GPIO_LOCK_LK8			BIT(8)
#define GPIO_LOCK_LK9			BIT(9)
#define GPIO_LOCK_LK10			BIT(10)
#define GPIO_LOCK_LK11			BIT(11)
#define GPIO_LOCK_LK12			BIT(12)
#define GPIO_LOCK_LK13			BIT(13)
#define GPIO_LOCK_LK14			BIT(14)
#define GPIO_LOCK_LK15			BIT(15)
#define GPIO_LOCK_LKK			BIT(16)

#define AFIO_EC_PIN			BITS(0, 3) // event output pin selection
#define AFIO_EC_PORT			BITS(4, 6) // event output port selection
#define AFIO_EC_EOE			BIT(7) // event output enable

#define AFIO_PCF0_SPI0_REMAP		BIT(0)		// SPI0 remapping
#define AFIO_PCF0_I2C0_REMAP		BIT(1)		// I2C0 remapping
#define AFIO_PCF0_USART0_REMAP		BIT(2)		// USART0 remapping
#define AFIO_PCF0_USART1_REMAP		BIT(3)		// USART1 remapping
#define AFIO_PCF0_USART2_REMAP		BITS(4, 5)	// USART2 remapping
#define AFIO_PCF0_TIMER0_REMAP		BITS(6, 7)	// TIMER0 remapping
#define AFIO_PCF0_TIMER1_REMAP		BITS(8, 9)	// TIMER1 remapping
#define AFIO_PCF0_TIMER2_REMAP		BITS(10, 11)	// TIMER2 remapping
#define AFIO_PCF0_TIMER3_REMAP		BIT(12)		// TIMER3 remapping

#define AFIO_PCF0_CAN_REMAP		BITS(13, 14)	// CAN remapping
// port D0/port D1 mapping on OSC_IN/OSC_OUT

#define AFIO_PCF0_PD01_REMAP		BIT(15)

// TIMER3 channel3 internal remapping
#define AFIO_PCF0_TIMER4CH3_IREMAP	BIT(16)

// serial wire JTAG configuration
#define AFIO_PCF0_SWJ_CFG		BITS(24, 26)

#define AFIO_PCF0_SPI2_REMAP		BIT(28) // SPI2/I2S2 remapping


// TIMER1 internal trigger 1 remapping
#define AFIO_PCF0_TIMER1_ITI1_REMAP	BIT(29)


#define AFIO_EXTI0_SS			BITS(0, 3)// EXTI 0 sources selection
#define AFIO_EXTI1_SS			BITS(4, 7)// EXTI 1 sources selection
#define AFIO_EXTI2_SS			BITS(8, 11)// EXTI 2 sources selection
#define AFIO_EXTI3_SS			BITS(12, 15)// EXTI 3 sources selection

#define AFIO_EXTI4_SS			BITS(0, 3)// EXTI 4 sources selection
#define AFIO_EXTI5_SS			BITS(4, 7)// EXTI 5 sources selection
#define AFIO_EXTI6_SS			BITS(8, 11)// EXTI 6 sources selection
#define AFIO_EXTI7_SS			BITS(12, 15)// EXTI 7 sources selection

#define AFIO_EXTI8_SS			BITS(0, 3)// EXTI 8 sources selection
#define AFIO_EXTI9_SS			BITS(4, 7)// EXTI 9 sources selection
#define AFIO_EXTI10_SS			BITS(8, 11)// EXTI 10 sources selection
#define AFIO_EXTI11_SS			BITS(12, 15)// EXTI 11 sources selection

#define AFIO_EXTI12_SS			BITS(0, 3)// EXTI 12 sources selection
#define AFIO_EXTI13_SS			BITS(4, 7)// EXTI 13 sources selection
#define AFIO_EXTI14_SS			BITS(8, 11)// EXTI 14 sources selection
#define AFIO_EXTI15_SS			BITS(12, 15)// EXTI 15 sources selection

#define AFIO_PCF1_EXMC_NADV		BIT(10)	// EXMC_NADV connect/disconnect

typedef enum flag_status bit_status;

#define GPIO_MODE_SET(n, mode) \
	((uint32_t) ((uint32_t) (mode) << (4U * (n))))

#define GPIO_MODE_MASK(n) \
	(0xFU << (4U * (n)))

#define GPIO_MODE_AIN			((uint8_t) 0x00U) // analog input mode
#define GPIO_MODE_IN_FLOATING		((uint8_t) 0x04U) // floating input mode
#define GPIO_MODE_IPD			((uint8_t) 0x28U) // pull-down input mode
#define GPIO_MODE_IPU			((uint8_t) 0x48U) // pull-up input mode
#define GPIO_MODE_OUT_OD		((uint8_t) 0x14U) // GPIO output with open-drain
#define GPIO_MODE_OUT_PP		((uint8_t) 0x10U) // GPIO output with push-pull
#define GPIO_MODE_AF_OD			((uint8_t) 0x1CU) // AFIO output with open-drain
#define GPIO_MODE_AF_PP			((uint8_t) 0x18U) // AFIO output with push-pull

#define GPIO_OSPEED_10MHZ		((uint8_t) 0x01U)
#define GPIO_OSPEED_2MHZ		((uint8_t) 0x02U)
#define GPIO_OSPEED_50MHZ		((uint8_t) 0x03U)

#define GPIO_EVENT_PORT_GPIOA		((uint8_t) 0x00U)
#define GPIO_EVENT_PORT_GPIOB		((uint8_t) 0x01U)
#define GPIO_EVENT_PORT_GPIOC		((uint8_t) 0x02U)
#define GPIO_EVENT_PORT_GPIOD		((uint8_t) 0x03U)
#define GPIO_EVENT_PORT_GPIOE		((uint8_t) 0x04U)

#define GPIO_PORT_SOURCE_GPIOA		((uint8_t) 0x00U)
#define GPIO_PORT_SOURCE_GPIOB		((uint8_t) 0x01U)
#define GPIO_PORT_SOURCE_GPIOC		((uint8_t) 0x02U)
#define GPIO_PORT_SOURCE_GPIOD		((uint8_t) 0x03U)
#define GPIO_PORT_SOURCE_GPIOE		((uint8_t) 0x04U)

#define GPIO_EVENT_PIN_0		((uint8_t) 0x00U)
#define GPIO_EVENT_PIN_1		((uint8_t) 0x01U)
#define GPIO_EVENT_PIN_2		((uint8_t) 0x02U)
#define GPIO_EVENT_PIN_3		((uint8_t) 0x03U)
#define GPIO_EVENT_PIN_4		((uint8_t) 0x04U)
#define GPIO_EVENT_PIN_5		((uint8_t) 0x05U)
#define GPIO_EVENT_PIN_6		((uint8_t) 0x06U)
#define GPIO_EVENT_PIN_7		((uint8_t) 0x07U)
#define GPIO_EVENT_PIN_8		((uint8_t) 0x08U)
#define GPIO_EVENT_PIN_9		((uint8_t) 0x09U)
#define GPIO_EVENT_PIN_10		((uint8_t) 0x0AU)
#define GPIO_EVENT_PIN_11		((uint8_t) 0x0BU)
#define GPIO_EVENT_PIN_12		((uint8_t) 0x0CU)
#define GPIO_EVENT_PIN_13		((uint8_t) 0x0DU)
#define GPIO_EVENT_PIN_14		((uint8_t) 0x0EU)
#define GPIO_EVENT_PIN_15		((uint8_t) 0x0FU)

#define GPIO_PIN_SOURCE_0		((uint8_t) 0x00U)
#define GPIO_PIN_SOURCE_1		((uint8_t) 0x01U)
#define GPIO_PIN_SOURCE_2		((uint8_t) 0x02U)
#define GPIO_PIN_SOURCE_3		((uint8_t) 0x03U)
#define GPIO_PIN_SOURCE_4		((uint8_t) 0x04U)
#define GPIO_PIN_SOURCE_5		((uint8_t) 0x05U)
#define GPIO_PIN_SOURCE_6		((uint8_t) 0x06U)
#define GPIO_PIN_SOURCE_7		((uint8_t) 0x07U)
#define GPIO_PIN_SOURCE_8		((uint8_t) 0x08U)
#define GPIO_PIN_SOURCE_9		((uint8_t) 0x09U)
#define GPIO_PIN_SOURCE_10		((uint8_t) 0x0AU)
#define GPIO_PIN_SOURCE_11		((uint8_t) 0x0BU)
#define GPIO_PIN_SOURCE_12		((uint8_t) 0x0CU)
#define GPIO_PIN_SOURCE_13		((uint8_t) 0x0DU)
#define GPIO_PIN_SOURCE_14		((uint8_t) 0x0EU)
#define GPIO_PIN_SOURCE_15		((uint8_t) 0x0FU)

#define GPIO_PIN_0			BIT(0)
#define GPIO_PIN_1			BIT(1)
#define GPIO_PIN_2			BIT(2)
#define GPIO_PIN_3			BIT(3)
#define GPIO_PIN_4			BIT(4)
#define GPIO_PIN_5			BIT(5)
#define GPIO_PIN_6			BIT(6)
#define GPIO_PIN_7			BIT(7)
#define GPIO_PIN_8			BIT(8)
#define GPIO_PIN_9			BIT(9)
#define GPIO_PIN_10			BIT(10)
#define GPIO_PIN_11			BIT(11)
#define GPIO_PIN_12			BIT(12)
#define GPIO_PIN_13			BIT(13)
#define GPIO_PIN_14			BIT(14)
#define GPIO_PIN_15			BIT(15)
#define GPIO_PIN_ALL			BITS(0, 15)

#define GPIO_SPI0_REMAP			((uint32_t) 0x00000001U)
#define GPIO_I2C0_REMAP			((uint32_t) 0x00000002U)
#define GPIO_USART0_REMAP		((uint32_t) 0x00000004U)
#define GPIO_USART1_REMAP		((uint32_t) 0x00000008U)
#define GPIO_USART2_PARTIAL_REMAP	((uint32_t) 0x00140010U)
#define GPIO_USART2_FULL_REMAP		((uint32_t) 0x00140030U)
#define GPIO_TIMER0_PARTIAL_REMAP	((uint32_t) 0x00160040U)
#define GPIO_TIMER0_FULL_REMAP		((uint32_t) 0x001600C0U)
#define GPIO_TIMER1_PARTIAL_REMAP0	((uint32_t) 0x00180100U)
#define GPIO_TIMER1_PARTIAL_REMAP1	((uint32_t) 0x00180200U)
#define GPIO_TIMER1_FULL_REMAP		((uint32_t) 0x00180300U)
#define GPIO_TIMER2_PARTIAL_REMAP	((uint32_t) 0x001A0800U)
#define GPIO_TIMER2_FULL_REMAP		((uint32_t) 0x001A0C00U)
#define GPIO_TIMER3_REMAP		((uint32_t) 0x00001000U)
#define GPIO_CAN0_PARTIAL_REMAP		((uint32_t) 0x001D4000U)
#define GPIO_CAN0_FULL_REMAP		((uint32_t) 0x001D6000U)
#define GPIO_PD01_REMAP			((uint32_t) 0x00008000U)
#define GPIO_TIMER4CH3_IREMAP		((uint32_t) 0x00200001U)
#define GPIO_CAN1_REMAP			((uint32_t) 0x00200040U)
// JTAG-DP,but without NJTRST
#define GPIO_SWJ_NONJTRST_REMAP		((uint32_t) 0x00300100U)
// JTAG-DP disabled
#define GPIO_SWJ_DISABLE_REMAP		((uint32_t) 0x00300200U)
#define GPIO_SPI2_REMAP			((uint32_t) 0x00201100U)
// TIMER1 internal trigger 1 remapping
#define GPIO_TIMER1ITI1_REMAP		((uint32_t) 0x00202000U)
// EXMC_NADV connect/disconnect
#define GPIO_EXMC_NADV_REMAP		((uint32_t) 0x80000400U)

void gpio_deinit(uint32_t gpio_periph);

// reset alternate function I/O(AFIO)
void gpio_afio_deinit(void);

// GPIO parameter initialization
void gpio_init(uint32_t gpio_periph,uint32_t mode,uint32_t speed,uint32_t pin);

void gpio_bit_set(uint32_t gpio_periph, uint32_t pin);
void gpio_bit_reset(uint32_t gpio_periph, uint32_t pin);
void gpio_bit_write(uint32_t gpio_periph, uint32_t pin, bit_status bit_value);
void gpio_port_write(uint32_t gpio_periph, uint16_t data);

enum flag_status gpio_input_bit_get(uint32_t gpio_periph, uint32_t pin);
uint16_t gpio_input_port_get(uint32_t gpio_periph);
enum flag_status gpio_output_bit_get(uint32_t gpio_periph, uint32_t pin);
uint16_t gpio_output_port_get(uint32_t gpio_periph);

void gpio_pin_remap_config(uint32_t remap, enum control_status newvalue);

void gpio_exti_source_select(uint8_t output_port, uint8_t output_pin);
void gpio_event_output_config(uint8_t output_port, uint8_t output_pin);
void gpio_event_output_enable(void);
void gpio_event_output_disable(void);

void gpio_pin_lock(uint32_t gpio_periph, uint32_t pin);

#endif
