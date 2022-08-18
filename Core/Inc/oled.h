/**
  ******************************************************************************
  * @file    oled.c
  * @author  Celeron
  * @brief   oled source code
  *
  * @attention
  *
  ******************************************************************************
  */

#ifndef __OLED_H
#define __OLED_H


#include "stm32f0xx_hal.h"
#include "main.h"


#define OLED_WIDTH	(128u)
#define OLED_HIGHT	(64u)




extern SPI_HandleTypeDef hspi1;
#define oled_spi_hander	hspi1



void oled_init(void);
void oled_write_byte(uint8_t Byte);
void oled_refresh(void);
void oled_clear(void);
void oled_draw_pixel(uint16_t Xpos, uint16_t Ypos, uint8_t State);
void oled_draw_hline(uint16_t Xpos, uint16_t Ypos, uint16_t Length);
void oled_draw_vline(uint16_t Xpos, uint16_t Ypos, uint16_t Length);
void oled_draw_line(uint16_t X1, uint16_t Y1, uint16_t X2, uint16_t Y2);
void oled_draw_circle(uint16_t Xpos, uint16_t Ypos, uint16_t Radius);
void oled_draw_ellipse(int Xpos, int Ypos, int XRadius, int YRadius);
void oled_draw_rectangle(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height);
void oled_draw_fill_circle(uint16_t Xpos, uint16_t Ypos, uint16_t Radius);
void oled_draw_fill_ellipse(int Xpos, int Ypos, int XRadius, int YRadius);
void oled_draw_fill_rectangle(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height);
uint8_t *oled_get_buffer(void);



#endif
