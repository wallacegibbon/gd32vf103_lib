#ifndef __SCR1_H
#define __SCR1_H

struct scr1_handle {
	int width, height;
	int buffer_size;
	unsigned char *buffer;
	int spi_dev;
	int rst_port;
	int rst_pin;
	int cs_port;
	int cs_pin;
	int dc_port;
	int dc_pin;
};

static inline void scr1_rst_clr(struct scr1_handle *scr) {
	gpio_bit_reset(scr->rst_port, scr->rst_pin);
}

static inline void scr1_rst_set(struct scr1_handle *scr) {
	gpio_bit_set(scr->rst_port, scr->rst_pin);
}

static inline void scr1_cs_clr(struct scr1_handle *scr) {
	gpio_bit_reset(scr->cs_port, scr->cs_pin);
}

static inline void scr1_cs_set(struct scr1_handle *scr) {
	gpio_bit_set(scr->cs_port, scr->cs_pin);
}

static inline void scr1_dc_clr(struct scr1_handle *scr) {
	gpio_bit_reset(scr->dc_port, scr->dc_pin);
}

static inline void scr1_dc_set(struct scr1_handle *scr) {
	gpio_bit_set(scr->dc_port, scr->dc_pin);
}

void scr1_write_data8(struct scr1_handle *scr, int data);
void scr1_write_data(struct scr1_handle *scr, int data);
void scr1_write_reg(struct scr1_handle *scr, int data);

void scr1_addr_set(struct scr1_handle *scr, int x1,
		int y1, int x2, int y2);

void scr1_clear(struct scr1_handle *scr, int color);

void scr1_fill(struct scr1_handle *scr, int xsta, int ysta,
		int xend, int yend, int color);

void scr1_draw_point_big(struct scr1_handle *scr, int x, int y,
		int color);

void scr1_draw_point(struct scr1_handle *scr, int x, int y,
		int color);

void scr1_draw_line(struct scr1_handle *scr, int x1, int y1,
		int x2, int y2, int color);

void scr1_draw_rectangle(struct scr1_handle *scr, int x1, int y1,
		int x2, int y2, int color);

void scr1_draw_circle(struct scr1_handle *scr, int x0, int y0,
		int r, int color);

void scr1_draw_buffer(struct scr1_handle *scr, int x1, int y1, int x2, int y2);

// Brush color
#define WHITE		0xFFFF
#define BLACK		0x0000	  
#define BLUE		0x001F  
#define BRED		0XF81F
#define GRED		0XFFE0
#define GBLUE		0X07FF
#define RED		0xF800
#define MAGENTA		0xF81F
#define GREEN		0x07E0
#define CYAN		0x7FFF
#define YELLOW		0xFFE0
#define BROWN		0XBC40
#define BRRED		0XFC07 //maroon
#define GRAY		0X8430

//GUI color
#define DARKBLUE	0X01CF
#define LIGHTBLUE	0X7D7C
#define GRAYBLUE	0X5458

//The above three colors are the colors of PANEL
#define LIGHTGREEN	0X841F
#define LGRAY		0XC618
#define LGRAYBLUE	0XA651
#define LBBLUE		0X2B12

#endif

