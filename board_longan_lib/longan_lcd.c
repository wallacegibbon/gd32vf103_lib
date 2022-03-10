#include <gd32vf103.h>
#include "longan_lcd.h"
#include "util.h"

unsigned char scr_buffer[160 * 80 * 2];

struct scr1_handle longan_lcd_handle = {
	.width = 160,
	.height = 80,
	.buffer_size = 160 * 80 * 2,
	.buffer = scr_buffer,
	.spi_dev = SPI0,
	.rst_port = GPIOB,
	.rst_pin = GPIO_PIN_1,
	.cs_port = GPIOB,
	.cs_pin = GPIO_PIN_2,
	.dc_port = GPIOB,
	.dc_pin = GPIO_PIN_0
};

void spi0_init() {
	struct spi_param spi;
	spi.trans_mode = SPI_TRANSMODE_FULLDUPLEX;
	spi.device_mode = SPI_MASTER;
	spi.frame_size = SPI_FRAMESIZE_8BIT;
	spi.clock_polarity_phase = SPI_CK_PL_HIGH_PH_2EDGE;
	spi.nss = SPI_NSS_SOFT;
	spi.prescale = SPI_PSC_8;
	spi.endian = SPI_ENDIAN_MSB;

	spi_init(SPI0, &spi);
	spi_crc_polynomial_set(SPI0, 7);
	spi_enable(SPI0);
}

void longan_lcd_init() {
	struct scr1_handle *scr = &longan_lcd_handle;

	rcu_periph_clock_enable(RCU_GPIOA);
	rcu_periph_clock_enable(RCU_GPIOB);
	rcu_periph_clock_enable(RCU_AF);
	rcu_periph_clock_enable(RCU_SPI0);

	scr1_cs_set(scr);

	/* SPI0 GPIO config: NSS/PA4, SCK/PA5, MOSI/PA7 */
	gpio_init(GPIOA, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ,
			GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7);
	gpio_init(GPIOB, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_2);
	spi0_init();

	gpio_init(GPIOB, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ,
			GPIO_PIN_0 | GPIO_PIN_1);

	gpio_bit_reset(GPIOB, GPIO_PIN_0 | GPIO_PIN_1);

	scr1_rst_clr(scr);

	delay_ms(200);
	scr1_rst_set(scr);
	delay_ms(20);
	
	// turn off sleep mode
	scr1_write_reg(scr, 0x11);
	delay_ms(100);

	// display inversion mode
	scr1_write_reg(scr, 0x21);

	// Set the frame frequency of the full colors normal mode
	scr1_write_reg(scr, 0xB1);

	// Frame rate=fosc/((RTNA x 2 + 40) x (LINE + FPA + BPA +2))
	// fosc = 850kHz
	scr1_write_data8(scr, 0x05); // RTNA
	scr1_write_data8(scr, 0x3A); // FPA
	scr1_write_data8(scr, 0x3A); // BPA

	// Set the frame frequency of the Idle mode
	scr1_write_reg(scr, 0xB2);

	// Frame rate=fosc/((RTNB x 2 + 40) x (LINE + FPB + BPB +2))
	// fosc = 850kHz
	scr1_write_data8(scr, 0x05); // RTNB
	scr1_write_data8(scr, 0x3A); // FPB
	scr1_write_data8(scr, 0x3A); // BPB

	// Set the frame frequency of the Partial mode/ full colors
	scr1_write_reg(scr, 0xB3);

	scr1_write_data8(scr, 0x05);
	scr1_write_data8(scr, 0x3A);
	scr1_write_data8(scr, 0x3A);
	scr1_write_data8(scr, 0x05);
	scr1_write_data8(scr, 0x3A);
	scr1_write_data8(scr, 0x3A);

	scr1_write_reg(scr, 0xB4);
	scr1_write_data8(scr, 0x03);

	scr1_write_reg(scr, 0xC0);
	scr1_write_data8(scr, 0x62);
	scr1_write_data8(scr, 0x02);
	scr1_write_data8(scr, 0x04);

	scr1_write_reg(scr, 0xC1);
	scr1_write_data8(scr, 0xC0);

	scr1_write_reg(scr, 0xC2);
	scr1_write_data8(scr, 0x0D);
	scr1_write_data8(scr, 0x00);

	scr1_write_reg(scr, 0xC3);
	scr1_write_data8(scr, 0x8D);
	scr1_write_data8(scr, 0x6A);

	scr1_write_reg(scr, 0xC4);
	scr1_write_data8(scr, 0x8D);
	scr1_write_data8(scr, 0xEE);

	scr1_write_reg(scr, 0xC5);
	scr1_write_data8(scr, 0x0E); // VCOM

	scr1_write_reg(scr, 0xE0);
	scr1_write_data8(scr, 0x10);
	scr1_write_data8(scr, 0x0E);
	scr1_write_data8(scr, 0x02);
	scr1_write_data8(scr, 0x03);
	scr1_write_data8(scr, 0x0E);
	scr1_write_data8(scr, 0x07);
	scr1_write_data8(scr, 0x02);
	scr1_write_data8(scr, 0x07);
	scr1_write_data8(scr, 0x0A);
	scr1_write_data8(scr, 0x12);
	scr1_write_data8(scr, 0x27);
	scr1_write_data8(scr, 0x37);
	scr1_write_data8(scr, 0x00);
	scr1_write_data8(scr, 0x0D);
	scr1_write_data8(scr, 0x0E);
	scr1_write_data8(scr, 0x10);

	scr1_write_reg(scr, 0xE1);
	scr1_write_data8(scr, 0x10);
	scr1_write_data8(scr, 0x0E);
	scr1_write_data8(scr, 0x03);
	scr1_write_data8(scr, 0x03);
	scr1_write_data8(scr, 0x0F);
	scr1_write_data8(scr, 0x06);
	scr1_write_data8(scr, 0x02);
	scr1_write_data8(scr, 0x08);
	scr1_write_data8(scr, 0x0A);
	scr1_write_data8(scr, 0x13);
	scr1_write_data8(scr, 0x26);
	scr1_write_data8(scr, 0x36);
	scr1_write_data8(scr, 0x00);
	scr1_write_data8(scr, 0x0D);
	scr1_write_data8(scr, 0x0E);
	scr1_write_data8(scr, 0x10);

	// define the format of RGB picture data
	scr1_write_reg(scr, 0x3A);

	scr1_write_data8(scr, 0x05); // 16-bit/pixel

	scr1_write_reg(scr, 0x36);
	scr1_write_data8(scr, 0x78);

	scr1_write_reg(scr, 0x29); // Display On
}

void longan_lcd_clear(int color) {
	scr1_clear(&longan_lcd_handle, color);
}

void longan_lcd_fill(int x1, int y1, int x2, int y2, int color) {
	scr1_fill(&longan_lcd_handle, x1, y1, x2, y2, color);
}

void longan_lcd_draw_point_big(int x, int y, int color) {
	scr1_draw_point_big(&longan_lcd_handle, x, y, color);
}

void longan_lcd_draw_point(int x, int y, int color) {
	scr1_draw_point(&longan_lcd_handle, x, y, color);
}

void longan_lcd_draw_line(int x1, uint16_t y1, int x2, int y2, int color) {
	scr1_draw_line(&longan_lcd_handle, x1, y1, x2, y2, color);
}

void longan_lcd_draw_rectangle(int x1, int y1, int x2, int y2, int color) {
	scr1_draw_rectangle(&longan_lcd_handle, x1, y1, x2, y2, color);
}

void longan_lcd_draw_circle(int x, int y, int r, int color) {
	scr1_draw_circle(&longan_lcd_handle, x, y, r, color);
}

void longan_lcd_draw_buffer(int x1, int y1, int x2, int y2) {
	scr1_draw_buffer(&longan_lcd_handle, x1, y1, x2, y2);
}

