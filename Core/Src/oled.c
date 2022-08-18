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

#include "oled.h"


/** @brief	求绝对值
  */
#define ABS(X)  ((X) > 0 ? (X) : -(X)) 

/*oled graphical buffer*/
uint8_t OLED_BUFFER[OLED_WIDTH*OLED_HIGHT/8];


/**
  * @brief  oled initial.
	*	@param	None
  * @retval None
  */
void oled_init(void)
{	
	HAL_GPIO_WritePin(OLED_RES_GPIO_Port, OLED_RES_Pin, GPIO_PIN_SET);
	HAL_Delay(200);
	HAL_GPIO_WritePin(OLED_RES_GPIO_Port, OLED_RES_Pin, GPIO_PIN_RESET);
	HAL_Delay(200);
	HAL_GPIO_WritePin(OLED_RES_GPIO_Port, OLED_RES_Pin, GPIO_PIN_SET);
	
	HAL_GPIO_WritePin(OLED_CS_GPIO_Port, OLED_CS_Pin, GPIO_PIN_RESET);
	
	HAL_GPIO_WritePin(OLED_DC_GPIO_Port, OLED_DC_Pin, GPIO_PIN_RESET);
	
	oled_write_byte(0xAE); //关闭显示
	oled_write_byte(0xD5); //设置时钟分频因子,震荡频率
	oled_write_byte(0x80); //[3:0],分频因子;[7:4],震荡频率
	oled_write_byte(0xA8); //设置驱动路数
	oled_write_byte(0X3F); //默认0X3F(1/64) 
	oled_write_byte(0xD3); //设置显示偏移
	oled_write_byte(0X00); //默认为0

	oled_write_byte(0x40); //设置显示开始行 [5:0],行数.
													
	oled_write_byte(0x8D); //电荷泵设置
	oled_write_byte(0x14); //bit2，开启/关闭
	oled_write_byte(0x20); //设置内存地址模式
	oled_write_byte(0x00); //[1:0],00，列地址模式;01，行地址模式;10,页地址模式;默认10;
	oled_write_byte(0xA1); //段重定义设置,bit0:0,0->0;1,0->127;
	oled_write_byte(0xC0); //设置COM扫描方向;bit3:0,普通模式;1,重定义模式 COM[N-1]->COM0;N:驱动路数
	oled_write_byte(0xDA); //设置COM硬件引脚配置
	oled_write_byte(0x12); //[5:4]配置
		 
	oled_write_byte(0x81); //对比度设置
	oled_write_byte(0xEF); //1~255;默认0X7F (亮度设置,越大越亮)
	oled_write_byte(0xD9); //设置预充电周期
	oled_write_byte(0xf1); //[3:0],PHASE 1;[7:4],PHASE 2;
	oled_write_byte(0xDB); //设置VCOMH 电压倍率
	oled_write_byte(0x30); //[6:4] 000,0.65*vcc;001,0.77*vcc;011,0.83*vcc;

	oled_write_byte(0xA4); //全局显示开启;bit0:1,开启;0,关闭;(白屏/黑屏)
	oled_write_byte(0xA6); //设置显示方式;bit0:1,反相显示;0,正常显示	    						   
	oled_write_byte(0xAF); //开启显示	 	

	HAL_GPIO_WritePin(OLED_DC_GPIO_Port, OLED_DC_Pin, GPIO_PIN_SET);

	oled_clear();

	oled_refresh();
}


/**
  * @brief  oled spi send byte data.
	*	@param	Byte	:send data
  * @retval None
  */
void oled_write_byte(uint8_t Byte)
{
	HAL_SPI_Transmit(&oled_spi_hander, &Byte, 1, 50);
}


/**
  * @brief  将图形缓存刷新至屏幕
	*	@param	None
  * @retval None
  */
void oled_refresh(void)
{
	HAL_SPI_Transmit_DMA(&oled_spi_hander, OLED_BUFFER, sizeof(OLED_BUFFER));
}


/**
  * @brief  clear
	*	@param	None
  * @retval None
  */
void oled_clear(void)
{
	uint16_t count = sizeof(OLED_BUFFER);
	uint8_t *p_temp = OLED_BUFFER;
	
	while(count--)
	{
		*p_temp++ = 0;
	}
}


/**
  * @brief  点亮一个像素点
  * @param  Xpos
  *	@param	Ypos
  * @retval None
  */
void oled_draw_pixel(uint16_t Xpos, uint16_t Ypos, uint8_t State)
{
	if (Xpos > (OLED_WIDTH - 1) || (Ypos > (OLED_HIGHT - 1)))
		return ;

	uint8_t *p_temp = OLED_BUFFER + (Ypos	/	8)*128 + Xpos;

	if(State)
		*p_temp |= (0x01 << (Ypos	%	8));
	else
		*p_temp &= ~(0x01 << (Ypos	%	8));
}


/**
  * @brief  Draws an horizontal line.
  * @param  Xpos: X position
  * @param  Ypos: Y position
  * @param  Length: Line length
  */
void oled_draw_hline(uint16_t Xpos, uint16_t Ypos, uint16_t Length)
{
  uint32_t index = 0;

	for(index = 0; index < Length; index++)
	{
		oled_draw_pixel((Xpos + index), Ypos, 1);
	}
}


/**
  * @brief  Draws a vertical line.
  * @param  Xpos: X position
  * @param  Ypos: Y position
  * @param  Length: Line length
  */
void oled_draw_vline(uint16_t Xpos, uint16_t Ypos, uint16_t Length)
{
  uint32_t index = 0;

	for(index = 0; index < Length; index++)
	{
		oled_draw_pixel(Xpos, Ypos + index, 1);
	}
}


/**
  * @brief  Draws an uni-line (between two points).
  * @param  X1: Point 1 X position
  * @param  Y1: Point 1 Y position
  * @param  X2: Point 2 X position
  * @param  Y2: Point 2 Y position
  */
void oled_draw_line(uint16_t X1, uint16_t Y1, uint16_t X2, uint16_t Y2)
{
  int16_t deltax = 0, deltay = 0, x = 0, y = 0, xinc1 = 0, xinc2 = 0, 
  yinc1 = 0, yinc2 = 0, den = 0, num = 0, numadd = 0, numpixels = 0, 
  curpixel = 0;
  
  deltax = ABS(X2 - X1);        /* The difference between the x's */
  deltay = ABS(Y2 - Y1);        /* The difference between the y's */
  x = X1;                       /* Start x off at the first pixel */
  y = Y1;                       /* Start y off at the first pixel */
  
  if (X2 >= X1)                 /* The x-values are increasing */
  {
    xinc1 = 1;
    xinc2 = 1;
  }
  else                          /* The x-values are decreasing */
  {
    xinc1 = -1;
    xinc2 = -1;
  }
  
  if (Y2 >= Y1)                 /* The y-values are increasing */
  {
    yinc1 = 1;
    yinc2 = 1;
  }
  else                          /* The y-values are decreasing */
  {
    yinc1 = -1;
    yinc2 = -1;
  }
  
  if (deltax >= deltay)         /* There is at least one x-value for every y-value */
  {
    xinc1 = 0;                  /* Don't change the x when numerator >= denominator */
    yinc2 = 0;                  /* Don't change the y for every iteration */
    den = deltax;
    num = deltax / 2;
    numadd = deltay;
    numpixels = deltax;         /* There are more x-values than y-values */
  }
  else                          /* There is at least one y-value for every x-value */
  {
    xinc2 = 0;                  /* Don't change the x for every iteration */
    yinc1 = 0;                  /* Don't change the y when numerator >= denominator */
    den = deltay;
    num = deltay / 2;
    numadd = deltax;
    numpixels = deltay;         /* There are more y-values than x-values */
  }
  
  for (curpixel = 0; curpixel <= numpixels; curpixel++)
  {
    oled_draw_pixel(x, y, 1);  /* Draw the current pixel */
    num += numadd;                            /* Increase the numerator by the top of the fraction */
    if (num >= den)                           /* Check if numerator >= denominator */
    {
      num -= den;                             /* Calculate the new numerator value */
      x += xinc1;                             /* Change the x as appropriate */
      y += yinc1;                             /* Change the y as appropriate */
    }
    x += xinc2;                               /* Change the x as appropriate */
    y += yinc2;                               /* Change the y as appropriate */
  }
}


/**
  * @brief  Draws a circle.
  * @param  Xpos: X position
  * @param  Ypos: Y position
  * @param  Radius: Circle radius
  */
void oled_draw_circle(uint16_t Xpos, uint16_t Ypos, uint16_t Radius)
{
  int32_t  D;       /* Decision Variable */ 
  uint32_t  CurX;   /* Current X Value */
  uint32_t  CurY;   /* Current Y Value */ 
  
  D = 3 - (Radius << 1);
  CurX = 0;
  CurY = Radius;
  
  while (CurX <= CurY)
  {
    oled_draw_pixel((Xpos + CurX), (Ypos - CurY), 1);

    oled_draw_pixel((Xpos - CurX), (Ypos - CurY), 1);

    oled_draw_pixel((Xpos + CurY), (Ypos - CurX), 1);

    oled_draw_pixel((Xpos - CurY), (Ypos - CurX), 1);

    oled_draw_pixel((Xpos + CurX), (Ypos + CurY), 1);

    oled_draw_pixel((Xpos - CurX), (Ypos + CurY), 1);

    oled_draw_pixel((Xpos + CurY), (Ypos + CurX), 1);

    oled_draw_pixel((Xpos - CurY), (Ypos + CurX), 1);   

    if (D < 0)
    { 
      D += (CurX << 2) + 6;
    }
    else
    {
      D += ((CurX - CurY) << 2) + 10;
      CurY--;
    }
    CurX++;
  } 
}


/**
  * @brief  Draws an ellipse on LCD.
  * @param  Xpos: X position
  * @param  Ypos: Y position
  * @param  XRadius: Ellipse X radius
  * @param  YRadius: Ellipse Y radius
  */
void oled_draw_ellipse(int Xpos, int Ypos, int XRadius, int YRadius)
{
  int x = 0, y = -YRadius, err = 2-2*XRadius, e2;
  float K = 0, rad1 = 0, rad2 = 0;
  
  rad1 = XRadius;
  rad2 = YRadius;
  
  K = (float)(rad2/rad1);
  
  do {      
    oled_draw_pixel((Xpos-(uint16_t)(x/K)), (Ypos+y), 1);
    oled_draw_pixel((Xpos+(uint16_t)(x/K)), (Ypos+y), 1);
    oled_draw_pixel((Xpos+(uint16_t)(x/K)), (Ypos-y), 1);
    oled_draw_pixel((Xpos-(uint16_t)(x/K)), (Ypos-y), 1);      
    
    e2 = err;
    if (e2 <= x) {
      err += ++x*2+1;
      if (-y == x && e2 <= y) e2 = 0;
    }
    if (e2 > y) err += ++y*2+1;     
  }
  while (y <= 0);
}


/**
  * @brief  Draws a rectangle.
  * @param  Xpos: X position
  * @param  Ypos: Y position
  * @param  Width: Rectangle width  
  * @param  Height: Rectangle height
  */
void oled_draw_rectangle(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height)
{
  /* Draw horizontal lines */
  oled_draw_hline(Xpos, Ypos, Width);
  oled_draw_hline(Xpos, (Ypos+ Height), Width);
  
  /* Draw vertical lines */
  oled_draw_vline(Xpos, Ypos, Height);
  oled_draw_vline((Xpos + Width), Ypos, Height);
}


/**
  * @brief  Draws a full circle.
  * @param  Xpos: X position
  * @param  Ypos: Y position
  * @param  Radius: Circle radius
  */
void oled_draw_fill_circle(uint16_t Xpos, uint16_t Ypos, uint16_t Radius)
{
  int32_t  D;        /* Decision Variable */ 
  uint32_t  CurX;    /* Current X Value */
  uint32_t  CurY;    /* Current Y Value */ 
  
  D = 3 - (Radius << 1);

  CurX = 0;
  CurY = Radius;

  while (CurX <= CurY)
  {
    if(CurY > 0) 
    {
      oled_draw_hline(Xpos - CurY, Ypos + CurX, 2*CurY);
      oled_draw_hline(Xpos - CurY, Ypos - CurX, 2*CurY);
    }

    if(CurX > 0) 
    {
      oled_draw_hline(Xpos - CurX, Ypos - CurY, 2*CurX);
      oled_draw_hline(Xpos - CurX, Ypos + CurY, 2*CurX);
    }
    if (D < 0)
    { 
      D += (CurX << 2) + 6;
    }
    else
    {
      D += ((CurX - CurY) << 2) + 10;
      CurY--;
    }
    CurX++;
  }

  oled_draw_circle(Xpos, Ypos, Radius);
}


/**
  * @brief  Draws a full ellipse.
  * @param  Xpos: X position
  * @param  Ypos: Y position
  * @param  XRadius: Ellipse X radius
  * @param  YRadius: Ellipse Y radius  
  */
void oled_draw_fill_ellipse(int Xpos, int Ypos, int XRadius, int YRadius)
{
  int x = 0, y = -YRadius, err = 2-2*XRadius, e2;
  float K = 0, rad1 = 0, rad2 = 0;
  
  rad1 = XRadius;
  rad2 = YRadius;
  
  K = (float)(rad2/rad1);    
  
  do 
  { 
    oled_draw_hline((Xpos-(uint16_t)(x/K)), (Ypos+y), (2*(uint16_t)(x/K) + 1));
    oled_draw_hline((Xpos-(uint16_t)(x/K)), (Ypos-y), (2*(uint16_t)(x/K) + 1));
    
    e2 = err;
    if (e2 <= x) 
    {
      err += ++x*2+1;
      if (-y == x && e2 <= y) e2 = 0;
    }
    if (e2 > y) err += ++y*2+1;
  }
  while (y <= 0);
}


/**
  * @brief  Draws a full rectangle.
  * @param  Xpos: X position
  * @param  Ypos: Y position
  * @param  Width: Rectangle width  
  * @param  Height: Rectangle height
  */
void oled_draw_fill_rectangle(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height)
{
  do
  {
    oled_draw_hline(Xpos, Ypos++, Width);    
  }
  while(Height--);
}


/**
  * @brief  get oled buffer
  * @return  return buffer point
  */
uint8_t *oled_get_buffer(void)
{
	return OLED_BUFFER;
}
