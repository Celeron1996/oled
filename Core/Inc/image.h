/**
  ******************************************************************************
  * @file    image.c
  * @author  ZhuSL
  * @brief   image source code
  *
  * @attention
  *
  ******************************************************************************
  */

#ifndef __image_H
#define __image_H


#include "stm32f0xx_hal.h"
#include "main.h"


#define IMAGE_WIDTH	(128u)
#define IMAGE_HIGHT	(64u)


void image_set_buffer(uint8_t *p_buffer);
void image_clear(void);
void image_draw_pixel(uint16_t Xpos, uint16_t Ypos, uint8_t State);
void image_draw_hline(uint16_t Xpos, uint16_t Ypos, uint16_t Length);
void image_draw_vline(uint16_t Xpos, uint16_t Ypos, uint16_t Length);
void image_draw_line(uint16_t X1, uint16_t Y1, uint16_t X2, uint16_t Y2);
void image_draw_circle(uint16_t Xpos, uint16_t Ypos, uint16_t Radius);
void image_draw_ellipse(int Xpos, int Ypos, int XRadius, int YRadius);
void image_draw_rectangle(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height);
void image_draw_fill_circle(uint16_t Xpos, uint16_t Ypos, uint16_t Radius);
void image_draw_fill_ellipse(int Xpos, int Ypos, int XRadius, int YRadius);
void image_draw_fill_rectangle(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height);
uint8_t *image_get_buffer(void);



#endif
