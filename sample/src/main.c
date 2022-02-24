#include <gd32vf103.h>
#include <stdio.h>

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

int putchar(int ch) {
	usart_data_transmit(USART0, (uint8_t) ch);
	while (usart_flag_get(USART0, USART_FLAG_TBE) == RESET);

	return ch;
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

	int n = printf("this is from the serial port, %s\r\n", my_variable);
	printf("the size of previous printf is %d\r\n", n);
	printf("done.\r\n");
	printf("test %d-%d-%d.\r\n", 3, 30, 300);

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

