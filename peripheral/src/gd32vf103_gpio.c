#include "gd32vf103_gpio.h"

#define AFIO_EXTI_SOURCE_MASK		((uint8_t) 0x03U)
#define AFIO_EXTI_SOURCE_FIELDS		((uint8_t) 0x04U)
#define LSB_16BIT_MASK			((uint16_t) 0xFFFFU)
// AFIO_PCF register position mask
#define PCF_POSITION_MASK		((uint32_t) 0x000F0000U)
// AFIO_PCF register SWJCFG mask
#define PCF_SWJCFG_MASK			((uint32_t) 0xF0FFFFFFU)
// AFIO_PCF register location1 mask
#define PCF_LOCATION1_MASK		((uint32_t) 0x00200000U)
// AFIO_PCF register location2 mask
#define PCF_LOCATION2_MASK		((uint32_t) 0x00100000U)
// select AFIO_PCF1 register
#define AFIO_PCF1_FIELDS		((uint32_t) 0x80000000U)
// GPIO event output port offset
#define GPIO_OUTPUT_PORT_OFFSET		((uint32_t) 4U)

void gpio_deinit(uint32_t gpio_periph) {
	switch (gpio_periph) {
	case GPIOA:
		rcu_periph_reset_enable(RCU_GPIOARST);
		rcu_periph_reset_disable(RCU_GPIOARST);
		break;
	case GPIOB:
		rcu_periph_reset_enable(RCU_GPIOBRST);
		rcu_periph_reset_disable(RCU_GPIOBRST);
		break;
	case GPIOC:
		rcu_periph_reset_enable(RCU_GPIOCRST);
		rcu_periph_reset_disable(RCU_GPIOCRST);
		break;
	case GPIOD:
		rcu_periph_reset_enable(RCU_GPIODRST);
		rcu_periph_reset_disable(RCU_GPIODRST);
		break;
	case GPIOE:
		rcu_periph_reset_enable(RCU_GPIOERST);
		rcu_periph_reset_disable(RCU_GPIOERST);
		break;
	default:
		break;
	}
}

void gpio_afio_deinit(void) {
	rcu_periph_reset_enable(RCU_AFRST);
	rcu_periph_reset_disable(RCU_AFRST);
}

/*
 * GPIO parameter initialization
 * gpio_periph: GPIOx(x = A,B,C,D,E)
 * mode: gpio pin mode
 * 	only one parameter can be selected which is shown as below:
 * 		GPIO_MODE_AIN: analog input mode
 * 		GPIO_MODE_IN_FLOATING: floating input mode
 * 		GPIO_MODE_IPD: pull-down input mode
 * 		GPIO_MODE_IPU: pull-up input mode
 * 		GPIO_MODE_OUT_OD: GPIO output with open-drain
 * 		GPIO_MODE_OUT_PP: GPIO output with push-pull
 * 		GPIO_MODE_AF_OD: AFIO output with open-drain
 * 		GPIO_MODE_AF_PP: AFIO output with push-pull
 * speed: gpio output max speed value
 * 	only one parameter can be selected which is shown as below:
 * 		GPIO_OSPEED_10MHZ: output max speed 10MHz
 * 		GPIO_OSPEED_2MHZ: output max speed 2MHz
 * 		GPIO_OSPEED_50MHZ: output max speed 50MHz
 * pin: GPIO pin
 * 	one or more parameters can be selected which are shown as below:
 * 		GPIO_PIN_x(x=0..15), GPIO_PIN_ALL
 */
void gpio_init(uint32_t gpio_periph, uint32_t mode, uint32_t speed,
		uint32_t pin) {
	uint32_t reg = 0;

	// GPIO mode configuration
	uint32_t temp_mode = (mode & 0x0FU);

	// GPIO speed configuration
	if (mode & 0x10)
		// output mode max speed:10MHz,2MHz,50MHz
		temp_mode |= speed;

	// configure the eight low port pins with GPIO_CTL0
	for (int i = 0; i < 8; i++) {
		if (!(pin & (1 << i)))
			continue;

		reg = GPIO_CTL0(gpio_periph);

		reg &= ~GPIO_MODE_MASK(i);
		reg |= GPIO_MODE_SET(i, temp_mode);

		// set IPD or IPU
		if (mode == GPIO_MODE_IPD)
			GPIO_BC(gpio_periph) = ((1 << i) & pin);
		else if (mode == GPIO_MODE_IPU)
			GPIO_BOP(gpio_periph) = ((1 << i) & pin);

		// set GPIO_CTL0 register
		GPIO_CTL0(gpio_periph) = reg;
	}

	// configure the eight high port pins with GPIO_CTL1
	for (int i = 8; i < 16; i++) {
		if (!(pin & (1 << i)))
			continue;

		reg = GPIO_CTL1(gpio_periph);

		reg &= ~GPIO_MODE_MASK(i - 8);
		reg |= GPIO_MODE_SET(i - 8, temp_mode);

		// set IPD or IPU
		if (mode == GPIO_MODE_IPD)
			GPIO_BC(gpio_periph) = ((1 << i) & pin);
		else if (mode == GPIO_MODE_IPU)
			GPIO_BOP(gpio_periph) = ((1 << i) & pin);

		// set GPIO_CTL1 register
		GPIO_CTL1(gpio_periph) = reg;
	}
}

void gpio_bit_set(uint32_t gpio_periph, uint32_t pin) {
	GPIO_BOP(gpio_periph) = (uint32_t) pin;
}

void gpio_bit_reset(uint32_t gpio_periph, uint32_t pin) {
	GPIO_BC(gpio_periph) = pin;
}

/*
 * write data to the specified GPIO pin
 * gpio_periph: GPIOx(x = A,B,C,D,E)
 * pin: GPIO pin: GPIO_PIN_x(x=0..15) or GPIO_PIN_ALL
 * bit_value: SET or RESET
*/
void gpio_bit_write(uint32_t gpio_periph, uint32_t pin, bit_status bit_value) {
	if (bit_value != RESET)
		GPIO_BOP(gpio_periph) = pin;
	else
		GPIO_BC(gpio_periph) = pin;
}

void gpio_port_write(uint32_t gpio_periph, uint16_t data) {
	GPIO_OCTL(gpio_periph) = data;
}

enum flag_status gpio_input_bit_get(uint32_t gpio_periph, uint32_t pin) {
	if ((GPIO_ISTAT(gpio_periph) & pin) != RESET)
		return SET;
	else
		return RESET;
}

uint16_t gpio_input_port_get(uint32_t gpio_periph) {
	return GPIO_ISTAT(gpio_periph);
}

enum flag_status gpio_output_bit_get(uint32_t gpio_periph, uint32_t pin) {
	if ((GPIO_OCTL(gpio_periph) & pin) != RESET)
		return SET;
	else
		return RESET;
}

uint16_t gpio_output_port_get(uint32_t gpio_periph) {
	return GPIO_OCTL(gpio_periph);
}

/*
 * configure GPIO pin remap
 * gpio_remap: select the pin to remap
 * 	only one parameter can be selected which are shown as below:
 * 		GPIO_SPI0_REMAP: SPI0 remapping
 * 		GPIO_I2C0_REMAP: I2C0 remapping
 * 		GPIO_USART0_REMAP: USART0 remapping
 * 		GPIO_USART1_REMAP: USART1 remapping
 * 		GPIO_USART2_PARTIAL_REMAP: USART2 partial remapping
 * 		GPIO_USART2_FULL_REMAP: USART2 full remapping
 * 		GPIO_TIMER0_PARTIAL_REMAP: TIMER0 partial remapping
 * 		GPIO_TIMER0_FULL_REMAP: TIMER0 full remapping
 * 		GPIO_TIMER1_PARTIAL_REMAP0: TIMER1 partial remapping
 * 		GPIO_TIMER1_PARTIAL_REMAP1: TIMER1 partial remapping
 * 		GPIO_TIMER1_FULL_REMAP: TIMER1 full remapping
 * 		GPIO_TIMER2_PARTIAL_REMAP: TIMER2 partial remapping
 * 		GPIO_TIMER2_FULL_REMAP: TIMER2 full remapping
 * 		GPIO_TIMER3_REMAP: TIMER3 remapping
 * 		GPIO_CAN0_PARTIAL_REMAP: CAN0 partial remapping
 * 		GPIO_CAN0_FULL_REMAP: CAN0 full remapping
 * 		GPIO_PD01_REMAP: PD01 remapping
 * 		GPIO_TIMER4CH3_IREMAP: TIMER4 channel3 internal remapping
 * 		GPIO_CAN1_REMAP: CAN1 remapping
 * 		GPIO_SWJ_NONJTRST_REMAP: JTAG-DP,but without NJTRST
 * 		GPIO_SWJ_DISABLE_REMAP: JTAG-DP disabled
 * 		GPIO_SPI2_REMAP: SPI2 remapping
 * 		GPIO_TIMER1ITI1_REMAP: TIMER1 internal trigger 1 remapping
 * 		GPIO_EXMC_NADV_REMAP: EXMC_NADV connect/disconnect
 * newvalue: ENABLE or DISABLE
 */
void gpio_pin_remap_config(uint32_t remap, enum control_status newvalue) {
	uint32_t temp_reg = 0U;

	if (AFIO_PCF1_FIELDS == (remap & AFIO_PCF1_FIELDS))
		// get AFIO_PCF1 regiter value
		temp_reg = AFIO_PCF1;
	else
		// get AFIO_PCF0 regiter value
		temp_reg = AFIO_PCF0;

	uint32_t temp_mask = (remap & PCF_POSITION_MASK) >> 0x10U;
	uint32_t remap1 = remap & LSB_16BIT_MASK;

	// judge pin remap type
	if ((PCF_LOCATION1_MASK | PCF_LOCATION2_MASK) ==
			(remap & (PCF_LOCATION1_MASK | PCF_LOCATION2_MASK))) {
		temp_reg &= PCF_SWJCFG_MASK;
		AFIO_PCF0 &= PCF_SWJCFG_MASK;
	} else if (PCF_LOCATION2_MASK == (remap & PCF_LOCATION2_MASK)) {
		uint32_t remap2 = 0x03U << temp_mask;
		temp_reg &= ~remap2;
		temp_reg |= ~PCF_SWJCFG_MASK;
	} else {
		temp_reg &= ~(remap1 << ((remap >> 0x15U) * 0x10U));
		temp_reg |= ~PCF_SWJCFG_MASK;
	}

	// set pin remap value
	if (DISABLE != newvalue)
		temp_reg |= (remap1 << ((remap >> 0x15U) * 0x10U));

	if (AFIO_PCF1_FIELDS == (remap & AFIO_PCF1_FIELDS))
		// set AFIO_PCF1 regiter value
		AFIO_PCF1 = temp_reg;
	else
		// set AFIO_PCF0 regiter value
		AFIO_PCF0 = temp_reg;
}

/*
 * select GPIO pin exti sources
 * gpio_outputport: gpio event output port
 * 	only one parameter can be selected which are shown as below:
 * 		GPIO_PORT_SOURCE_GPIOA: output port source A
 * 		GPIO_PORT_SOURCE_GPIOB: output port source B
 * 		GPIO_PORT_SOURCE_GPIOC: output port source C
 * 		GPIO_PORT_SOURCE_GPIOD: output port source D
 * 		GPIO_PORT_SOURCE_GPIOE: output port source E
 * gpio_outputpin: GPIO_PIN_SOURCE_x(x=0..15)
 */
void gpio_exti_source_select(uint8_t output_port, uint8_t output_pin) {
	uint32_t source = 0x0FU << (AFIO_EXTI_SOURCE_FIELDS *
			(output_pin & AFIO_EXTI_SOURCE_MASK));

	if (GPIO_PIN_SOURCE_4 > output_pin) {
		// select EXTI0/EXTI1/EXTI2/EXTI3
		AFIO_EXTISS0 &= ~source;
		AFIO_EXTISS0 |= output_port
			<< (AFIO_EXTI_SOURCE_FIELDS *
					(output_pin & AFIO_EXTI_SOURCE_MASK));
	} else if (GPIO_PIN_SOURCE_8 > output_pin) {
		// select EXTI4/EXTI5/EXTI6/EXTI7
		AFIO_EXTISS1 &= ~source;
		AFIO_EXTISS1 |= output_port
			<< (AFIO_EXTI_SOURCE_FIELDS *
					(output_pin & AFIO_EXTI_SOURCE_MASK));
	} else if (GPIO_PIN_SOURCE_12 > output_pin) {
		// select EXTI8/EXTI9/EXTI10/EXTI11
		AFIO_EXTISS2 &= ~source;
		AFIO_EXTISS2 |= output_port
			<< (AFIO_EXTI_SOURCE_FIELDS *
					(output_pin & AFIO_EXTI_SOURCE_MASK));
	} else {
		// select EXTI12/EXTI13/EXTI14/EXTI15
		AFIO_EXTISS3 &= ~source;
		AFIO_EXTISS3 |= output_port
			<< (AFIO_EXTI_SOURCE_FIELDS *
					(output_pin & AFIO_EXTI_SOURCE_MASK));
	}
}

/*
 * configure GPIO pin event output
 * output_port: gpio event output port
 * 	only one parameter can be selected which are shown as below:
 * 		GPIO_EVENT_PORT_GPIOA: event output port A
 * 		GPIO_EVENT_PORT_GPIOB: event output port B
 * 		GPIO_EVENT_PORT_GPIOC: event output port C
 * 		GPIO_EVENT_PORT_GPIOD: event output port D
 * 		GPIO_EVENT_PORT_GPIOE: event output port E
 * output_pin:
 * 	only one parameter can be selected which are shown as below:
 * 		GPIO_EVENT_PIN_x(x=0..15)
 */
void gpio_event_output_config(uint8_t output_port, uint8_t output_pin) {
	uint32_t reg = AFIO_EC;

	// clear AFIO_EC_PORT and AFIO_EC_PIN bits
	reg &= ~(AFIO_EC_PORT | AFIO_EC_PIN);

	reg |= output_port << GPIO_OUTPUT_PORT_OFFSET;
	reg |= output_pin;

	AFIO_EC = reg;
}

void gpio_event_output_enable(void) {
	AFIO_EC |= AFIO_EC_EOE;
}

void gpio_event_output_disable(void) {
	AFIO_EC &= ~AFIO_EC_EOE;
}

/*
 * lock GPIO pin
 * gpio_periph: GPIOx(x = A,B,C,D,E)
 * pin: GPIO pin
 * 	one or more parameters can be selected which are shown as below:
 * 		GPIO_PIN_x(x=0..15), GPIO_PIN_ALL
*/
void gpio_pin_lock(uint32_t gpio_periph, uint32_t pin) {
	uint32_t lock = 0x00010000U;
	lock |= pin;

	// lock key writing sequence:
	// write 1 -> write 0 -> write 1 -> read 0 -> read 1
	GPIO_LOCK(gpio_periph) = lock;
	GPIO_LOCK(gpio_periph) = pin;
	GPIO_LOCK(gpio_periph) = lock;
	lock = GPIO_LOCK(gpio_periph);
	lock = GPIO_LOCK(gpio_periph);
}
