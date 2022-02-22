#include "gd32vf103.h"

char my_variable[] = "hello, world!\r\n";

char buf[100];

void init() {
	rcu_periph_clock_enable(RCU_GPIOA);
	rcu_periph_clock_enable(RCU_GPIOC);
	rcu_periph_clock_enable(RCU_USART0);

	eclic_global_interrupt_enable();
	eclic_priority_group_set(ECLIC_PRIGROUP_LEVEL3_PRIO1);
	eclic_irq_enable(USART0_IRQn, 1, 0);

	// IO for USART0
	gpio_init(GPIOA, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_9);
	gpio_init(GPIOA, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_10);

	// trun off all LEDs
	gpio_bit_set(GPIOA, GPIO_PIN_1 | GPIO_PIN_2);
	gpio_bit_set(GPIOC, GPIO_PIN_13);

	// IO for LED
	gpio_init(GPIOA, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ,
			GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_13);
	gpio_init(GPIOC, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_13);

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

int _put_char(int ch) {
	usart_data_transmit(USART0, (uint8_t) ch);
	while (usart_flag_get(USART0, USART_FLAG_TBE) == RESET);

	return ch;
}

int puts(const char *str) {
	while (*str != '\0')
		_put_char(*str++);
}

int sleep(int t) {
	volatile int r;
	while (t--)
		for (int i = 0; i < 10000000; i++)
			r = i;
}

int main(int argc, const char **argv) {
	init();

	//gpio_bit_reset(GPIOA, GPIO_PIN_1);
	//gpio_bit_reset(GPIOA, GPIO_PIN_2);
	//gpio_bit_reset(GPIOC, GPIO_PIN_13);

	puts("this is from the serial port\r\n");
	puts(my_variable);

	while (1) {
		gpio_bit_set(GPIOC, GPIO_PIN_13);
		sleep(1);
		gpio_bit_reset(GPIOC, GPIO_PIN_13);
		sleep(1);
	}

	return 0;
}

void USART0_IRQn_handler() {
	static int flag = ~0;
	int key = usart_data_receive(USART0);
	int mask = 0;

	if (key == 'g')
		mask = GPIO_PIN_1;
	else if (key == 'b')
		mask = GPIO_PIN_2;

	if (flag & mask) {
		gpio_bit_reset(GPIOA, mask);
		flag &= ~mask;
	} else {
		gpio_bit_set(GPIOA, mask);
		flag |= mask;
	}
}

