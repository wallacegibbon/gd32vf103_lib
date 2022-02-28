#include <gd32vf103.h>
#include <stdio.h>
#include <string.h>

char my_variable[] = "hello, world!";

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

int putchar(int ch) {
	usart_data_transmit(USART0, (uint8_t) ch);
	while (usart_flag_get(USART0, USART_FLAG_TBE) == RESET);
	return 1;
}

int sleep(int t) {
	volatile int r;
	while (t--)
		for (int i = 0; i < 10000000; i++)
			r = i;
}

void c_lib_test() {
	char buf1[] = "hello, this is from buffer 1.";
	char buf2[] = "hello, quick brown fox jumps over the lazy dog.";

	printf("buf1(%p): %s\r\nbuf2(%p): %s\r\n", buf1, buf1, buf2, buf2);

	printf("strcmp(buf1, buf2) = %d\r\n", strcmp(buf1, buf2));
	printf("strcmp(buf2, buf1) = %d\r\n", strcmp(buf2, buf1));

	printf("memcmp(buf1, buf2, 3) = %d\r\n", memcmp(buf1, buf2, 3));
	printf("memcmp(buf2, buf1, 3) = %d\r\n", memcmp(buf2, buf1, 3));

	printf("memcmp(buf1, buf2, 10) = %d\r\n", memcmp(buf1, buf2, 10));
	printf("memcmp(buf2, buf1, 10) = %d\r\n", memcmp(buf2, buf1, 10));

	printf("strlen(buf1) = %d\r\n", strlen(buf1));

	printf("memcpy(buf1, buf2, 10): %p\r\n", memcpy(buf1, buf2, 10));
	printf("buf1(%p): %s\r\nbuf2(%p): %s\r\n", buf1, buf1, buf2, buf2);

	printf("memset(buf1, 'a', 10): %p\r\n", memset(buf1, 'a', 10));
	printf("buf1(%p): %s\r\nbuf2(%p): %s\r\n", buf1, buf1, buf2, buf2);

	printf("\r\n");
}

void printf_float_test() {
	printf("floating point number print:\r\n"
			"\t pi = |%010.3f|,  e = |%10.3f|\r\n"
			"\t-pi = |%010.3f|, -e = |%10.3f|\r\n"
			"\t-pi = |%-010.3f|, -e = |%-10.3f|\r\n"
			"\t pi = |%.4f|, e = |%10f|\r\n"
			"\t-pi = |%10f|, -e = |%10f|\r\n"
			"\t pi = |%3f|, e = |%f|\r\n",
			3.1415926, 2.718281,
			-3.1415926, -2.718281,
			-3.1415926, -2.718281,
			3.1415926, 2.718281,
			-3.1415926, -2.718281,
			3.1415926, 2.718281);
}

void printf_width_test() {
	printf("too big width test:\r\n|%0100f|\r\n", -3.1415926);
	printf("too big width test:\r\n|%100f|\r\n", -3.1415926);
	printf("too big width test:\r\n|%-0100f|\r\n", -3.1415926);

	printf("star test: |%*.*f|\r\n", 10, 3, 3.1415926);
	printf("star test: |%0*.*f|\r\n", 10, 3, 3.1415926);
	printf("star test: |%-0*.*f|\r\n", 10, 3, 3.1415926);

	printf("string pad test: |%10.3s|\r\n", "hello");
	printf("string pad test: |%010.3s|\r\n", "hello");
	printf("string pad test: |%-10.3s|\r\n", "hello");
	printf("string pad test: |%-10s|\r\n", "hello");
	printf("string pad test: |%-.3s|\r\n", "hello");
	printf("string pad test: |%-.8s|\r\n", "hello");
	printf("string pad test: |%-10.8s|\r\n", "hello");
	printf("string pad test: |%10.8s|\r\n", "hello");
	printf("string pad test: |%4s|\r\n", "hello");
}

int main(int argc, const char **argv) {
	init();

	int n = printf("this is from the serial port, %s\r\n", my_variable);
	printf("the size of previous printf is %04d(%04x)%c\r\n", n, n, '~');

	// pointer printing test
	printf("the address of a string literal: %p\r\n", "hi");

	printf_float_test();
	printf_width_test();

	c_lib_test();

	while (1) {
		gpio_bit_set(GPIOC, GPIO_PIN_13);
		sleep(1);
		gpio_bit_reset(GPIOC, GPIO_PIN_13);
		sleep(1);
	}

	return 0;
}

struct led_controller {
	int port, pin, state;
};

struct led_controller leds[2] =
	{{GPIOA, GPIO_PIN_1, 0}, {GPIOA, GPIO_PIN_2, 0}};

void led_controller_toggle(struct led_controller *led) {
	led->state = !led->state;
	if (led->state)
		gpio_bit_reset(led->port, led->pin);
	else
		gpio_bit_set(led->port, led->pin);
}

void USART0_IRQn_handler() {
	int key = usart_data_receive(USART0);
	if (key == 'g')
		led_controller_toggle(&leds[0]);
	else if (key == 'b')
		led_controller_toggle(&leds[1]);
}

