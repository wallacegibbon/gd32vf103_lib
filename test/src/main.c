#include "gd32vf103.h"

char my_variable[] = "hello, world!";

char buf[100];

void init() {
	rcu_periph_clock_enable(RCU_GPIOA);
	rcu_periph_clock_enable(RCU_USART0);

	gpio_init(GPIOA, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_9);
	gpio_init(GPIOA, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_10);

	usart_deinit(USART0);
	usart_baudrate_set(USART0, 9600);
	usart_word_length_set(USART0, USART_WL_8BIT);
	usart_stop_bit_set(USART0, USART_STB_1BIT);
	usart_parity_config(USART0, USART_PM_NONE);
	usart_hardware_flow_rts_config(USART0, USART_RTS_DISABLE);
	usart_hardware_flow_cts_config(USART0, USART_CTS_DISABLE);
	usart_receive_config(USART0, USART_RECEIVE_ENABLE);
	usart_transmit_config(USART0, USART_TRANSMIT_ENABLE);
	usart_enable(USART0);
}

int _put_char(int ch) {
	usart_data_transmit(USART0, (uint8_t) ch);
	while (usart_flag_get(USART0, USART_FLAG_TBE) == RESET);

	return ch;
}

int puts(const char *str) {
	for (char c = *str; c != '\0'; str++)
		_put_char(c);
}

int main(int argc, const char **argv) {
	init();

	for (int i = 0; i < 100; i++)
		buf[i] = my_variable[i % 10];

	for (int i = 0; i < 100; i++)
		puts("hello, this is from the serial port\n");

	while (1);

	return 0;
}

