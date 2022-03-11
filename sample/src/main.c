#include <gd32vf103.h>
#include <stdio.h>
#include <string.h>
#include <longan_lcd.h>
#include "c_lib_test.h"
#include "init.h"

void update_loop_display() {
	static unsigned short start_color = 0;
	for (int i = 0; i < 36; i++) {
		start_color += 50;
		longan_lcd_draw_circle(80, 40, i, start_color);
	}
}

void longan_all_screen_white() {
	memset(longan_lcd_handle.buffer, 0xff, longan_lcd_handle.buffer_size);
	longan_lcd_draw_buffer(0, 0, 160, 80);
}

void wfi_test_loop() {
	while (1) {
		printf("entering sleep mode...\r\n");
		pmu_to_sleepmode(WFI_CMD);
		//pmu_to_deepsleepmode(PMU_LDO_NORMAL, WFI_CMD);
		//pmu_to_standbymode(WFI_CMD);
	}
}

void circle_display_loop() {
	while (1) {
		gpio_bit_set(GPIOC, GPIO_PIN_13);
		update_loop_display();
		gpio_bit_reset(GPIOC, GPIO_PIN_13);
		update_loop_display();

	}
}

int main(int argc, const char **argv) {
	basic_init();

	longan_lcd_clear(BLACK);
	longan_lcd_draw_rectangle(10, 10, 150, 70, CYAN);

	c_lib_test();

	circle_display_loop();

	//wfi_test_loop();

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

