#include <stdint.h>
#include <gd32vf103.h>
#include "scr1.h"

void scr1_write_bus(struct scr1_handle *scr, int data) {
	scr1_cs_clr(scr);

	spi_i2s_data_transmit(scr->spi_dev, data);
	while (spi_i2s_flag_get(scr->spi_dev, SPI_FLAG_TBE) == RESET);

	while (spi_i2s_flag_get(scr->spi_dev, SPI_FLAG_RBNE) == RESET);
	spi_i2s_data_receive(scr->spi_dev);

	scr1_cs_set(scr);
}

void scr1_write_data8(struct scr1_handle *scr, int data) {
	scr1_dc_set(scr);
	scr1_write_bus(scr, data);
}

void scr1_write_data(struct scr1_handle *scr, int data) {
	scr1_dc_set(scr);
	scr1_write_bus(scr, data >> 8);
	scr1_write_bus(scr, data);
}

void scr1_write_reg(struct scr1_handle *scr, int data) {
	scr1_dc_clr(scr);
	scr1_write_bus(scr, data);
}

void scr1_addr_set(struct scr1_handle *scr, int x1, int y1, int x2, int y2) {
	scr1_write_reg(scr, 0x2a);	// column address settings
	scr1_write_data(scr, x1 + 1);
	scr1_write_data(scr, x2 + 1);
	scr1_write_reg(scr, 0x2b);	// row address setting
	scr1_write_data(scr, y1 + 26);
	scr1_write_data(scr, y2 + 26);
	scr1_write_reg(scr, 0x2c);	// memory write
}

void scr1_clear(struct scr1_handle *scr, int color) {
	scr1_addr_set(scr, 0, 0, scr->width - 1, scr->height - 1);
	for (int i = 0; i < scr->width; i++)
		for (int j = 0; j < scr->height; j++)
			scr1_write_data(scr, color);
}

void scr1_fill(struct scr1_handle *scr, int x1, int y1, int x2, int y2,
		int color) {

	scr1_addr_set(scr, x1, y1, x2, y2);
	for (int i = y1; i <= y2; i++)
		for (int j = x1; j <= x2; j++)
			scr1_write_data(scr, color);
}

void scr1_draw_point_big(struct scr1_handle *scr, int x, int y, int color) {
	scr1_fill(scr, x - 1, y - 1, x + 1, y + 1, color);
}

void scr1_draw_point(struct scr1_handle *scr, int x, int y, int color) {
	scr1_addr_set(scr, x, y, x, y);
	scr1_write_data(scr, color);
}

void scr1_draw_line(struct scr1_handle *scr, int x1, int y1, int x2, int y2,
		int color) {

	int xerr = 0, yerr = 0;
	int delta_x = x2 - x1;
	int delta_y = y2 - y1;
	int urow = x1;
	int ucol = y1;

	int incx, incy;
	if (delta_x > 0) {
		incx = 1;
	} else if (delta_x == 0) {
		incx = 0;
	} else {
		incx = -1;
		delta_x = -delta_x;
	}

	if (delta_y > 0) {
		incy = 1;
	} else if (delta_y == 0) {
		incy = 0;
	} else {
		incy = -1;
		delta_y = -delta_x;
	}

	int distance;
	if (delta_x > delta_y)
		distance = delta_x;
	else
		distance = delta_y;

	for (int i = 0; i < distance + 1; i++) {
		scr1_draw_point(scr, urow, ucol, color);
		xerr += delta_x;
		yerr += delta_y;
		if (xerr > distance) {
			xerr -= distance;
			urow += incx;
		}
		if (yerr > distance) {
			yerr -= distance;
			ucol += incy;
		}
	}
}

void scr1_draw_rectangle(struct scr1_handle *scr, int x1, int y1,
		int x2, int y2, int color) {

	scr1_draw_line(scr, x1, y1, x2, y1, color);
	scr1_draw_line(scr, x1, y1, x1, y2, color);
	scr1_draw_line(scr, x1, y2, x2, y2, color);
	scr1_draw_line(scr, x2, y1, x2, y2, color);
}

void scr1_draw_circle(struct scr1_handle *scr, int x0, int y0,
		int r, int color) {
	int a = 0, b = r;
	while (a <= b) {
		scr1_draw_point(scr, x0 - b, y0 - a, color); // 3
		scr1_draw_point(scr, x0 + b, y0 - a, color); // 0
		scr1_draw_point(scr, x0 - a, y0 + b, color); // 1
		scr1_draw_point(scr, x0 - a, y0 - b, color); // 2
		scr1_draw_point(scr, x0 + b, y0 + a, color); // 4
		scr1_draw_point(scr, x0 + a, y0 - b, color); // 5
		scr1_draw_point(scr, x0 + a, y0 + b, color); // 6
		scr1_draw_point(scr, x0 - b, y0 + a, color); // 7
		a++;

		//Determine whether the points to be drawn are too far away
		if ((a * a + b * b) > (r * r))
			b--;
	}
}

void scr1_draw_buffer(struct scr1_handle *scr, int x1, int y1, int x2, int y2) {
	scr1_addr_set(scr, x1, y1, x2, y2);
	for (int i = 0; i < scr->buffer_size; i++)
		scr1_write_data8(scr, scr->buffer[i]);
}

