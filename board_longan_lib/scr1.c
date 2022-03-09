#include <stdint.h>
#include <gd32vf103.h>
#include "scr1.h"

static void scr1_write_bus(struct scr1_handle *scr, unsigned char data) {
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
	// column address settings
	scr1_write_reg(scr, 0x2a);
	scr1_write_data(scr, x1 + 1);
	scr1_write_data(scr, x2 + 1);

	// row address setting
	scr1_write_reg(scr, 0x2b);
	scr1_write_data(scr, y1 + 26);
	scr1_write_data(scr, y2 + 26);

	// memory write
	scr1_write_reg(scr, 0x2c);
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

#define max(a, b) (((a) > (b)) ? (a) : (b))

struct draw_line_state {
	int delta_x, delta_y;
	int distance;
	int incx, incy;
	int x, y;
	int xerr, yerr;
	int color;
};

void draw_line_state_init(struct draw_line_state *s, int x1, int y1,
		int x2, int y2, int color) {
	s->delta_x = x2 - x1;
	s->delta_y = y2 - y1;
	if (s->delta_x < 0) {
		s->incx = -1;
		s->delta_x = -s->delta_x;
	} else {
		s->incx = 1;
	}
	if (s->delta_y < 0) {
		s->incy = -1;
		s->delta_y = -s->delta_y;
	} else {
		s->incy = 1;
	}
	s->distance = max(s->delta_x, s->delta_y);
	s->x = x1;
	s->y = y1;
	s->color = color;
	s->xerr = 0;
	s->yerr = 0;
}

void scr1_draw_line_point(struct scr1_handle *scr, struct draw_line_state *s) {
	scr1_draw_point(scr, s->x, s->y, s->color);
	s->xerr += s->delta_x;
	if (s->xerr >= s->distance) {
		s->xerr -= s->distance;
		s->x += s->incx;
	}
	s->yerr += s->delta_y;
	if (s->yerr >= s->distance) {
		s->yerr -= s->distance;
		s->y += s->incy;
	}
}

void scr1_draw_line(struct scr1_handle *scr, int x1, int y1, int x2, int y2,
		int color) {

	struct draw_line_state s;

	draw_line_state_init(&s, x1, x2, y1, y2, color);

	for (int i = 0; i <= s.distance; i++)
		scr1_draw_line_point(scr, &s);
}

void scr1_draw_rectangle(struct scr1_handle *scr, int x1, int y1,
		int x2, int y2, int color) {

	scr1_draw_line(scr, x1, y1, x2, y1, color);
	scr1_draw_line(scr, x1, y1, x1, y2, color);
	scr1_draw_line(scr, x1, y2, x2, y2, color);
	scr1_draw_line(scr, x2, y1, x2, y2, color);
}

static void scr1_draw_point_x(struct scr1_handle *scr, int x0, int y0,
		int a, int b, int color) {
	scr1_draw_point(scr, x0 - a, y0 + b, color);
	scr1_draw_point(scr, x0 + a, y0 - b, color);

	scr1_draw_point(scr, x0 - a, y0 - b, color);
	scr1_draw_point(scr, x0 + a, y0 + b, color);
}

void scr1_draw_circle(struct scr1_handle *scr, int x0, int y0, int r,
		int color) {
	int a = 0, b = r;
	while (a <= b) {
		scr1_draw_point_x(scr, x0, y0, a, b, color);
		scr1_draw_point_x(scr, x0, y0, b, a, color);
		a++;
		if ((a * a + b * b) > (r * r))
			b--;
	}
}

void scr1_draw_buffer(struct scr1_handle *scr, int x1, int y1, int x2, int y2) {
	scr1_addr_set(scr, x1, y1, x2, y2);
	for (int i = 0; i < scr->buffer_size; i++)
		scr1_write_data8(scr, scr->buffer[i]);
}

