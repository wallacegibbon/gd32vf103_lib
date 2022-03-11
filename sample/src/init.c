#include <gd32vf103.h>
#include <string.h>
#include <longan_lcd.h>
#include <custom/fd.h>

static void uart_init() {
	rcu_periph_clock_enable(RCU_GPIOA);
	rcu_periph_clock_enable(RCU_USART0);

	eclic_global_interrupt_enable();
	eclic_priority_group_set(ECLIC_PRIGROUP_LEVEL3_PRIO1);
	eclic_irq_enable(USART0_IRQn, 1, 0);

	// IO for USART0
	gpio_init(GPIOA, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_9);
	gpio_init(GPIOA, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_10);

	usart_deinit(USART0);
	usart_baudrate_set(USART0, 128000);
	usart_word_length_set(USART0, USART_WL_8BIT);
	usart_stop_bit_set(USART0, USART_STB_1BIT);
	usart_parity_config(USART0, USART_PM_NONE);
	usart_hardware_flow_rts_config(USART0, USART_RTS_DISABLE);
	usart_hardware_flow_cts_config(USART0, USART_CTS_DISABLE);
	usart_receive_config(USART0, USART_RECEIVE_ENABLE);
	usart_transmit_config(USART0, USART_TRANSMIT_ENABLE);
	usart_enable(USART0);

	// enable the interrupt after initializing the usart
	usart_interrupt_enable(USART0, USART_INT_RBNE);
}

static void led_init() {
	rcu_periph_clock_enable(RCU_GPIOA);
	rcu_periph_clock_enable(RCU_GPIOC);

	// trun off all LEDs
	gpio_bit_set(GPIOA, GPIO_PIN_1 | GPIO_PIN_2);
	gpio_bit_set(GPIOC, GPIO_PIN_13);

	// IO for LED
	gpio_init(GPIOA, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ,
			GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_13);
	gpio_init(GPIOC, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_13);

}

/* this is needed by printf */
int putchar(int ch) {
	usart_data_transmit(USART0, (uint8_t) ch);
	while (usart_flag_get(USART0, USART_FLAG_TBE) == RESET);
	return 1;
}

static void fd_init() {
	bind_file_descriptor(1, putchar);
}

void basic_init() {
	uart_init();
	led_init();
	fd_init();
	longan_lcd_init();
}

