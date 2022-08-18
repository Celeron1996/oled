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

#include "image.h"
#include "stdint.h"


/** @brief	????
  */
#define ABS(X)  ((X) > 0 ? (X) : -(X)) 

/*image graphical buffer*/
static uint8_t *image_buffer;






void image_set_buffer(uint8_t *p_buffer)
{
	image_buffer = p_buffer;
}



/**
  * @brief  clear
	*	@param	None
  * @retval None
  */
void image_clear(void)
{
	uint16_t count = IMAGE_WIDTH*IMAGE_HIGHT/32;
	uint32_t *p_temp = (uint32_t *)image_buffer;
	
	while(count--)
	{
		*p_temp++ = 0;
	}
}


/**
  * @brief  ???????
  * @param  Xpos
  *	@param	Ypos
  * @retval None
  */
void image_draw_pixel(uint16_t Xpos, uint16_t Ypos, uint8_t State)
{
	if (Xpos > (IMAGE_WIDTH - 1) || (Ypos > (IMAGE_HIGHT - 1)))
		return ;
	
	uint8_t *p_temp = image_buffer + (Ypos	/	8)*128 + Xpos;

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
void image_draw_hline(uint16_t Xpos, uint16_t Ypos, uint16_t Length)
{
  uint32_t index = 0;

	for(index = 0; index < Length; index++)
	{
		image_draw_pixel((Xpos + index), Ypos, 1);
	}
}


/**
  * @brief  Draws a vertical line.
  * @param  Xpos: X position
  * @param  Ypos: Y position
  * @param  Length: Line length
  */
void image_draw_vline(uint16_t Xpos, uint16_t Ypos, uint16_t Length)
{
  uint32_t index = 0;

	for(index = 0; index < Length; index++)
	{
		image_draw_pixel(Xpos, Ypos + index, 1);
	}
}


/**
  * @brief  Draws an uni-line (between two points).
  * @param  X1: Point 1 X position
  * @param  Y1: Point 1 Y position
  * @param  X2: Point 2 X position
  * @param  Y2: Point 2 Y position
  */
void image_draw_line(uint16_t X1, uint16_t Y1, uint16_t X2, uint16_t Y2)
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
    image_draw_pixel(x, y, 1);  /* Draw the current pixel */
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
void image_draw_circle(uint16_t Xpos, uint16_t Ypos, uint16_t Radius)
{
  int32_t  D;       /* Decision Variable */ 
  uint32_t  CurX;   /* Current X Value */
  uint32_t  CurY;   /* Current Y Value */ 
  
  D = 3 - (Radius << 1);
  CurX = 0;
  CurY = Radius;
  
  while (CurX <= CurY)
  {
    image_draw_pixel((Xpos + CurX), (Ypos - CurY), 1);

    image_draw_pixel((Xpos - CurX), (Ypos - CurY), 1);

    image_draw_pixel((Xpos + CurY), (Ypos - CurX), 1);

    image_draw_pixel((Xpos - CurY), (Ypos - CurX), 1);

    image_draw_pixel((Xpos + CurX), (Ypos + CurY), 1);

    image_draw_pixel((Xpos - CurX), (Ypos + CurY), 1);

    image_draw_pixel((Xpos + CurY), (Ypos + CurX), 1);

    image_draw_pixel((Xpos - CurY), (Ypos + CurX), 1);   

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
void image_draw_ellipse(int Xpos, int Ypos, int XRadius, int YRadius)
{
  int x = 0, y = -YRadius, err = 2-2*XRadius, e2;
  float K = 0, rad1 = 0, rad2 = 0;
  
  rad1 = XRadius;
  rad2 = YRadius;
  
  K = (float)(rad2/rad1);
  
  do {      
    image_draw_pixel((Xpos-(uint16_t)(x/K)), (Ypos+y), 1);
    image_draw_pixel((Xpos+(uint16_t)(x/K)), (Ypos+y), 1);
    image_draw_pixel((Xpos+(uint16_t)(x/K)), (Ypos-y), 1);
    image_draw_pixel((Xpos-(uint16_t)(x/K)), (Ypos-y), 1);      
    
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
void image_draw_rectangle(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height)
{
  /* Draw horizontal lines */
  image_draw_hline(Xpos, Ypos, Width);
  image_draw_hline(Xpos, (Ypos+ Height), Width);
  
  /* Draw vertical lines */
  image_draw_vline(Xpos, Ypos, Height);
  image_draw_vline((Xpos + Width), Ypos, Height);
}


/**
  * @brief  Draws a full circle.
  * @param  Xpos: X position
  * @param  Ypos: Y position
  * @param  Radius: Circle radius
  */
void image_draw_fill_circle(uint16_t Xpos, uint16_t Ypos, uint16_t Radius)
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
      image_draw_hline(Xpos - CurY, Ypos + CurX, 2*CurY);
      image_draw_hline(Xpos - CurY, Ypos - CurX, 2*CurY);
    }

    if(CurX > 0) 
    {
      image_draw_hline(Xpos - CurX, Ypos - CurY, 2*CurX);
      image_draw_hline(Xpos - CurX, Ypos + CurY, 2*CurX);
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

  image_draw_circle(Xpos, Ypos, Radius);
}


/**
  * @brief  Draws a full ellipse.
  * @param  Xpos: X position
  * @param  Ypos: Y position
  * @param  XRadius: Ellipse X radius
  * @param  YRadius: Ellipse Y radius  
  */
void image_draw_fill_ellipse(int Xpos, int Ypos, int XRadius, int YRadius)
{
  int x = 0, y = -YRadius, err = 2-2*XRadius, e2;
  float K = 0, rad1 = 0, rad2 = 0;
  
  rad1 = XRadius;
  rad2 = YRadius;
  
  K = (float)(rad2/rad1);    
  
  do 
  { 
    image_draw_hline((Xpos-(uint16_t)(x/K)), (Ypos+y), (2*(uint16_t)(x/K) + 1));
    image_draw_hline((Xpos-(uint16_t)(x/K)), (Ypos-y), (2*(uint16_t)(x/K) + 1));
    
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
void image_draw_fill_rectangle(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height)
{
  do
  {
    image_draw_hline(Xpos, Ypos++, Width);    
  }
  while(Height--);
}
