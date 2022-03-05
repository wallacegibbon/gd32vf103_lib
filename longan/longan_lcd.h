#ifndef __LONGAN_LCD_H
#define __LONGAN_LCD_H

#include "scr1.h"

extern struct scr1_handle longan_lcd_handle;

void longan_lcd_init();

void longan_lcd_fill(int x1, int y1, int x2, int y2, int color);

void longan_lcd_draw_point_big(int x, int y, int color);

void longan_lcd_draw_point(int x, int y, int color);

void longan_lcd_draw_line(int x1, uint16_t y1, int x2, int y2, int color);

void longan_lcd_draw_rectangle(int x1, int y1, int x2, int y2, int color);

void longan_lcd_draw_circle(int x, int y, int r, int color);

void longan_lcd_draw_buffer(int x1, int y1, int x2, int y2);

void longan_lcd_clear(int color);

#endif

