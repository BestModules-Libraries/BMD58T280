/*****************************************************************
File:        BM_GraphicsLib.cpp
Author:      BESTMODULES
Description: BmpImage.cpp is used for graphics drawing.
History:     V1.0.2   -- 2022-11-20
******************************************************************/
#include "BM_GraphicsLib.h"
#include "font.c"

#ifdef __AVR__
 #include <avr/pgmspace.h>
#else
 #define pgm_read_byte(addr) (*(const unsigned char *)(addr))
#endif

/**********************************************************
Description: Constructor
Input:       w : Display width in pixels
             h : Display height in pixels
Output:
Return:
Others:
**********************************************************/
BM_GraphicsLib::BM_GraphicsLib(int16_t w, int16_t h) :
  WIDTH(w), HEIGHT(h)
{
  _width    = WIDTH;
  _height   = HEIGHT;
  rotation  = 0;
  cursor_y  = cursor_x    = 0;
  textsize  = 1;
  textcolor = textbgcolor = 0xFFFF;
  wrap = true;
}

/**********************************************************
Description: Draw a circle
Input:       x0 : The location of the center of the circle on the x axis
             y0 : The location of the center of the circle on the y axis
             r : The radius of the circle
             color : pattern color
Output:
Return:
Others:
**********************************************************/
void BM_GraphicsLib::drawCircle(int16_t x0, int16_t y0, int16_t r,
            uint16_t color) {
  int16_t f = 1 - r;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * r;
  int16_t x = 0;
  int16_t y = r;

  drawPixel(x0, y0+r, color);
  drawPixel(x0, y0-r, color);
  drawPixel(x0+r, y0, color);
  drawPixel(x0-r, y0, color);

  while (x<y) {
    if (f >= 0) {
      y--;
      ddF_y += 2;
      f += ddF_y;
    }
    x++;
    ddF_x += 2;
    f += ddF_x;

    drawPixel(x0 + x, y0 + y, color);
    drawPixel(x0 - x, y0 + y, color);
    drawPixel(x0 + x, y0 - y, color);
    drawPixel(x0 - x, y0 - y, color);
    drawPixel(x0 + y, y0 + x, color);
    drawPixel(x0 - y, y0 + x, color);
    drawPixel(x0 + y, y0 - x, color);
    drawPixel(x0 - y, y0 - x, color);
  }
}

/**********************************************************
Description: Draw a quarter circle
Input:       x0 : The location of the center of the circle on the x axis
             y0 : The location of the center of the circle on the y axis
             r : The radius of the circle
             cornername : corner name
             color : pattern color
Output:
Return:
Others:
**********************************************************/
void BM_GraphicsLib::drawCircleHelper( int16_t x0, int16_t y0,
               int16_t r, uint8_t cornername, uint16_t color) {
  int16_t f     = 1 - r;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * r;
  int16_t x     = 0;
  int16_t y     = r;

  while (x<y) {
    if (f >= 0) {
      y--;
      ddF_y += 2;
      f     += ddF_y;
    }
    x++;
    ddF_x += 2;
    f     += ddF_x;
    if (cornername & 0x4) {
      drawPixel(x0 + x, y0 + y, color);
      drawPixel(x0 + y, y0 + x, color);
    }
    if (cornername & 0x2) {
      drawPixel(x0 + x, y0 - y, color);
      drawPixel(x0 + y, y0 - x, color);
    }
    if (cornername & 0x8) {
      drawPixel(x0 - y, y0 + x, color);
      drawPixel(x0 - x, y0 + y, color);
    }
    if (cornername & 0x1) {
      drawPixel(x0 - y, y0 - x, color);
      drawPixel(x0 - x, y0 - y, color);
    }
  }
}

/**********************************************************
Description: Fill a circle
Input:       x0 : The location of the center of the circle on the x axis
             y0 : The location of the center of the circle on the y axis
             r : The radius of the circle
             color : pattern color
Output:
Return:
Others:
**********************************************************/
void BM_GraphicsLib::fillCircle(int16_t x0, int16_t y0, int16_t r,
            uint16_t color)
{
  drawVLine(x0, y0-r, 2*r+1, color);
  fillCircleHelper(x0, y0, r, 3, 0, color);
}

/**********************************************************
Description: Fill a quarter circle
Input:       x0 : The location of the center of the circle on the x axis
             y0 : The location of the center of the circle on the y axis
             r : The radius of the circle
             cornername : corner name
             color : pattern color
Output:
Return:
Others:
**********************************************************/
void BM_GraphicsLib::fillCircleHelper(int16_t x0, int16_t y0, int16_t r,
            uint8_t cornername, int16_t delta,             uint16_t color)
{
  int16_t f     = 1 - r;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * r;
  int16_t x     = 0;
  int16_t y     = r;

  while (x<y) {
    if (f >= 0) {
      y--;
      ddF_y += 2;
      f     += ddF_y;
    }
    x++;
    ddF_x += 2;
    f     += ddF_x;

    if (cornername & 0x1) {
      drawVLine(x0+x, y0-y, 2*y+1+delta, color);
      drawVLine(x0+y, y0-x, 2*x+1+delta, color);
    }
    if (cornername & 0x2) {
      drawVLine(x0-x, y0-y, 2*y+1+delta, color);
      drawVLine(x0-y, y0-x, 2*x+1+delta, color);
    }
  }
}

/**********************************************************
Description: Draw a line
Input: x0 : The horizontal position where the line starts
       y0 : The vertical position where the line starts
       x1 : The horizontal position where the line ends
       y1 : The vertical position where the line ends
      color : pattern color
Output:
Return:
Others:
**********************************************************/
void BM_GraphicsLib::drawLine(int16_t x0, int16_t y0,
          int16_t x1, int16_t y1,
          uint16_t color)
{
  int16_t steep = abs(y1 - y0) > abs(x1 - x0);
  if (steep) {
    swap(x0, y0);
    swap(x1, y1);
  }

  if (x0 > x1) {
    swap(x0, x1);
    swap(y0, y1);
  }

  int16_t dx, dy;
  dx = x1 - x0;
  dy = abs(y1 - y0);

  int16_t err = dx / 2;
  int16_t ystep;

  if (y0 < y1) {
    ystep = 1;
  } else {
    ystep = -1;
  }

  for (; x0<=x1; x0++) {
    if (steep) {
      drawPixel(y0, x0, color);
    } else {
      drawPixel(x0, y0, color);
    }
    err -= dy;
    if (err < 0) {
      y0 += ystep;
      err += dx;
    }
  }
}

/**********************************************************
Description: Draw a rectangle
Input:       x : The horizontal position where the line starts
             y : The vertical position where the line starts
             w : The width of the rectangle
             h : The height of the rectangle
             color : pattern color
Output:
Return:
Others:
**********************************************************/
void BM_GraphicsLib::drawRect(int16_t x, int16_t y,
          int16_t w, int16_t h,
          uint16_t color)
{
  drawHLine(x, y, w, color);
  drawHLine(x, y+h-1, w, color);
  drawVLine(x, y, h, color);
  drawVLine(x+w-1, y, h, color);
}

/**********************************************************
Description: Draw a vertical line
Input:       x : The horizontal position where the line starts
             y : The vertical position where the line starts
             h : The line length
Output:
Return:
Others:
**********************************************************/
void BM_GraphicsLib::drawVLine(int16_t x, int16_t y,
         int16_t h, uint16_t color)
{
  // Update in subclasses if desired!
  drawLine(x, y, x, y+h-1, color);
}

/**********************************************************
Description: Draw a horizontal line
Input:       x : The horizontal position where the line starts
             y : The vertical position where the line starts
             h : The line length
Output:
Return:
Others:
**********************************************************/
void BM_GraphicsLib::drawHLine(int16_t x, int16_t y,
         int16_t w, uint16_t color)
{
  // Update in subclasses if desired!
  drawLine(x, y, x+w-1, y, color);
}

/**********************************************************
Description: Fill a rectangle
Input:       x : The horizontal position where the line starts
             y : The vertical position where the line starts
             w : The width of the rectangle
             h : The height of the rectangle
             color : pattern color
Output:
Return:
Others:
**********************************************************/
void BM_GraphicsLib::fillRect(int16_t x, int16_t y, int16_t w, int16_t h,
          uint16_t color)
{
  for (int16_t i=x; i<x+w; i++) {
    drawVLine(i, y, h, color);
  }
}

/**********************************************************
Description: Fill the screen
Input:       color : pattern color
Output:
Return:
Others:
**********************************************************/
void BM_GraphicsLib::fillScreen(uint16_t color) {
  fillRect(0, 0, _width, _height, color);
}

/**********************************************************
Description: Draw a rounded rectangle
Input:       x : The horizontal position where the line starts
             y : The vertical position where the line starts
             w : The width of the rectangle
             h : The height of the rectangle
             color : pattern color
Output:
Return:
Others:
**********************************************************/
void BM_GraphicsLib::drawRoundRect(int16_t x, int16_t y, int16_t w,
           int16_t h, int16_t r, uint16_t color)
{
  // smarter version
  drawHLine(x+r  , y    , w-2*r, color); // Top
  drawHLine(x+r  , y+h-1, w-2*r, color); // Bottom
  drawVLine(x    , y+r  , h-2*r, color); // Left
  drawVLine(x+w-1, y+r  , h-2*r, color); // Right
  // draw four corners
  drawCircleHelper(x+r    , y+r    , r, 1, color);
  drawCircleHelper(x+w-r-1, y+r    , r, 2, color);
  drawCircleHelper(x+w-r-1, y+h-r-1, r, 4, color);
  drawCircleHelper(x+r    , y+h-r-1, r, 8, color);
}

/**********************************************************
Description: Fill a rounded rectangle
Input:       x : The horizontal position where the line starts
             y : The vertical position where the line starts
             w : The width of the rectangle
             h : The height of the rectangle
             color : pattern color
Output:
Return:
Others:
**********************************************************/
void BM_GraphicsLib::fillRoundRect(int16_t x, int16_t y, int16_t w,
         int16_t h, int16_t r, uint16_t color)
{
  // smarter version
  fillRect(x+r, y, w-2*r, h, color);

  // draw four corners
  fillCircleHelper(x+w-r-1, y+r, r, 1, h-2*r-1, color);
  fillCircleHelper(x+r    , y+r, r, 2, h-2*r-1, color);
}

/**********************************************************
Description: Draw a triangle
Input:       x0 : The horizontal position of the first angle
             y0 : The vertical position of the first angle
             x1 : The horizontal position of the second angle
             y1 : The vertical position of the second angle
             x2 : The horizontal position of the third angle
             y2 : The vertical position of the third angle
             color : pattern color
Output:
Return:
Others:
**********************************************************/
void BM_GraphicsLib::drawTriangle(int16_t x0, int16_t y0,
        int16_t x1, int16_t y1,
        int16_t x2, int16_t y2, uint16_t color) {
  drawLine(x0, y0, x1, y1, color);
  drawLine(x1, y1, x2, y2, color);
  drawLine(x2, y2, x0, y0, color);
}

/**********************************************************
Description: Fill a triangle
Input:       x0 : The horizontal position of the first angle
             y0 : The vertical position of the first angle
             x1 : The horizontal position of the second angle
             y1 : The vertical position of the second angle
             x2 : The horizontal position of the third angle
             y2 : The vertical position of the third angle
             color : pattern color
Output:
Return:
Others:
**********************************************************/
void BM_GraphicsLib::fillTriangle ( int16_t x0, int16_t y0,
          int16_t x1, int16_t y1,
          int16_t x2, int16_t y2, uint16_t color) {
  int16_t a, b, y, last;

  // Sort coordinates by Y order (y2 >= y1 >= y0)
  if (y0 > y1) {
    swap(y0, y1); swap(x0, x1);
  }
  if (y1 > y2) {
    swap(y2, y1); swap(x2, x1);
  }
  if (y0 > y1) {
    swap(y0, y1); swap(x0, x1);
  }

  if(y0 == y2) { // Handle awkward all-on-same-line case as its own thing
    a = b = x0;
    if(x1 < a)      a = x1;
    else if(x1 > b) b = x1;
    if(x2 < a)      a = x2;
    else if(x2 > b) b = x2;
    drawHLine(a, y0, b-a+1, color);
    return;
  }

  int16_t
    dx01 = x1 - x0,
    dy01 = y1 - y0,
    dx02 = x2 - x0,
    dy02 = y2 - y0,
    dx12 = x2 - x1,
    dy12 = y2 - y1,
    sa   = 0,
    sb   = 0;

  if(y1 == y2) last = y1;   // Include y1 scanline
  else         last = y1-1; // Skip it

  for(y=y0; y<=last; y++) {
    a   = x0 + sa / dy01;
    b   = x0 + sb / dy02;
    sa += dx01;
    sb += dx02;
    /* longhand:
    a = x0 + (x1 - x0) * (y - y0) / (y1 - y0);
    b = x0 + (x2 - x0) * (y - y0) / (y2 - y0);
    */
    if(a > b) swap(a,b);
    drawHLine(a, y, b-a+1, color);
  }

  // For lower part of triangle, find scanline crossings for segments
  // 0-2 and 1-2.  This loop is skipped if y1=y2.
  sa = dx12 * (y - y1);
  sb = dx02 * (y - y0);
  for(; y<=y2; y++) {
    a   = x1 + sa / dy12;
    b   = x0 + sb / dy02;
    sa += dx12;
    sb += dx02;
    /* longhand:
    a = x1 + (x2 - x1) * (y - y1) / (y2 - y1);
    b = x0 + (x2 - x0) * (y - y0) / (y2 - y0);
    */
    if(a > b) swap(a,b);
    drawHLine(a, y, b-a+1, color);
  }
}

/**********************************************************
Description: Draw image
Input:       x : The starting position of the x-axis of the image
             y : The starting position of the y-axis of the image
             _image : Image
             w : The width of the image
             h : The height of the image
             color : pattern color
Output:
Return:
Others:
**********************************************************/
void BM_GraphicsLib::drawImage(int16_t x, int16_t y,
            const uint8_t *_image, int16_t w, int16_t h,
            uint16_t color)
{
  int16_t i, j, byteWidth = (w + 7) / 8;

  for(j=0; j<h; j++) {
    for(i=0; i<w; i++ ) {
      if(pgm_read_byte(_image + j * byteWidth + i / 8) & (128 >> (i & 7))) {
        drawPixel(x+i, y+j, color);
      }
    }
  }
}

/**********************************************************
Description:  write
Input:        c : character
Output:
Return:       1 : Success
Others:
**********************************************************/
#if ARDUINO >= 100
size_t BM_GraphicsLib::write(uint8_t c) {
#else
void BM_GraphicsLib::write(uint8_t c) {
#endif
  if (c == '\n') {
    cursor_y += textsize*8;
    cursor_x = 0;
  } else if (c == '\r') {
    // skip em
  } else {
    drawChar(cursor_x, cursor_y, c, textcolor, textbgcolor, textsize);
    cursor_x += textsize*6;
    if (wrap && (cursor_x > (_width - textsize*6))) {
      cursor_y += textsize*8;
      cursor_x = 0;
    }
  }
#if ARDUINO >= 100
  return 1;
#endif
}

/**********************************************************
Description: Draw a character
Input:       x : The starting position of the x-axis of the character
             y : The starting position of the y-axis of the character
             c : character
             color : character color
             bg : character color
             size : character color
Output:
Return:
Others:
**********************************************************/
void BM_GraphicsLib::drawChar(int16_t x, int16_t y, unsigned char c,
          uint16_t color, uint16_t bg, uint8_t size)
{
  if((x >= _width)    || // Clip right
     (y >= _height)    || // Clip bottom
     ((x + 6 * size - 1) < 0)  || // Clip left
     ((y + 8 * size - 1) < 0))     // Clip top
    return;

  for (int8_t i=0; i<6; i++ ) {
    uint8_t line;
    if (i == 5)
      line = 0x0;
    else
      line = pgm_read_byte(gFont+(c*5)+i);
    for (int8_t j = 0; j<8; j++) {
      if (line & 0x1) {
        if (size == 1) // default size
          drawPixel(x+i, y+j, color);
        else {  // big size
          fillRect(x+(i*size), y+(j*size), size, size, color);
        }
      } else if (bg != color) {
        if (size == 1) // default size
          drawPixel(x+i, y+j, bg);
        else {  // big size
          fillRect(x+i*size, y+j*size, size, size, bg);
        }
      }
      line >>= 1;
    }
  }
}

/**********************************************************
Description: Write text to the screen at the given coordinates
Input:       text : char array, the text you want to write on the screen
             x : The location on the x-axis you want to start drawing text to the screen
             y : The location on the y-axis you want to start drawing text to the screen
Output:
Return:
Others:
**********************************************************/
void BM_GraphicsLib::text(const char * text, int16_t x, int16_t y)
{
  if ((useStroke) && (useFill))
  {
    setTextColor(strokeColor, fillColor);
  }
  else if(useStroke)
  {
    setTextColor(strokeColor);
  }
  else
  {
    return;
  }

  setTextWrap(false);

  setCursor(x, y);
  print(text);
}

/**********************************************************
Description: Write text to the screen at the given coordinates with word wrapping
Input:       text : char array, the text you want to write on the screen
             x : The location on the x-axis you want to start drawing text to the screen
             y : The location on the y-axis you want to start drawing text to the screen
Output:
Return:
Others:
**********************************************************/
void BM_GraphicsLib::textWrap(const char * text, int16_t x, int16_t y)
{
  if ((useStroke) && (useFill))
  {
    setTextColor(strokeColor, fillColor);
  }
  else if(useStroke)
  {
    setTextColor(strokeColor);
  }
  else
  {
    return;
  }
  setTextWrap(true);
  setCursor(x, y);
  print(text);
}

/**********************************************************
Description: Draws a point at the given coordinates
Input:       x : The horizontal position of the point
             y : The vertical position of the point
Output:
Return:
Others:
**********************************************************/
void BM_GraphicsLib::point(int16_t x, int16_t y)
{
  uint16_t _color;
  if (useStroke)
  {
    _color = strokeColor;
  }
  else if(useFill)
  {
    _color = fillColor;
  }
  else
  {
    return;
  }

  drawPixel(x, y, _color);
}

/**********************************************************
Description: Draws a line between two points
Input:       x1 : The horizontal position where the line starts
             y1 : The vertical position where the line starts
             x2 : The horizontal position where the line ends
             y2 : The vertical position where the line ends
Output:
Return:
Others:
**********************************************************/
void BM_GraphicsLib::line(int16_t x1, int16_t y1, int16_t x2, int16_t y2)
{
  uint16_t _color;
  if (useStroke)
  {
    _color = strokeColor;
  }
  else if(useFill)
  {
    _color = fillColor;
  }
  else
  {
    return;
  }

  if (x1 == x2) {
    if (y1 < y2)
      drawVLine(x1, y1, y2 - y1, _color);
    else
      drawVLine(x1, y2, y1 - y2, _color);
  }
  else if (y1 == y2) {
    if (x1 < x2)
      drawHLine(x1, y1, x2 - x1, _color);
    else
      drawHLine(x2, y1, x1 - x2, _color);
  }
  else {
    drawLine(x1, y1, x2, y2, _color);
  }
}

/**********************************************************
Description: Draws a rectangle to the TFT screen
Input:       x : The horizontal position where the line starts
             y : The vertical position where the line starts
             width : The width of the rectangle
             height : The height of the rectangle
Output:
Return:
Others:
**********************************************************/
void BM_GraphicsLib::rect(int16_t x, int16_t y, int16_t width, int16_t height)
{
  if (useFill) {
    fillRect(x, y, width, height, fillColor);
  }
  if (useStroke) {
    drawRect(x, y, width, height, strokeColor);
  }
}

/**********************************************************
Description: Draw a rounded rectangle on the TFT screen
Input:       x : The horizontal position where the line starts
             y : The vertical position where the line starts
             width : The width of the rectangle
             height : The height of the rectangle
             radius : radius
Output:
Return:
Others:
**********************************************************/
void BM_GraphicsLib::rect(int16_t x, int16_t y, int16_t width, int16_t height, int16_t radius)
{
  if (radius == 0) {
    rect(x, y, width, height);
  }
  if (useFill) {
    fillRoundRect(x, y, width, height, radius, fillColor);
  }
  if (useStroke) {
    drawRoundRect(x, y, width, height, radius, strokeColor);
  }
}

/**********************************************************
Description: Draws a circle on the screen
Input:       x : The location of the center of the circle on the x axis
             y : The location of the center of the circle on the y axis
             r : The radius of the circle
Output:
Return:
Others:
**********************************************************/
void BM_GraphicsLib::circle(int16_t x, int16_t y, int16_t r)
{
  if (r == 0)
    return;

  if (useFill) {
    fillCircle(x, y, r, fillColor);
  }
  if (useStroke) {
    drawCircle(x, y, r, strokeColor);
  }
}

/**********************************************************
Description: Draw a triangle on the screen
Input:       x1 : The horizontal position of the first angle
             y1 : The vertical position of the first angle
             x2 : The horizontal position of the second angle
             y2 : The vertical position of the second angle
             x3 : The horizontal position of the third angle
             y3 : The vertical position of the third angle
**********************************************************/
void BM_GraphicsLib::triangle(int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t x3, int16_t y3)
{
  if (useFill) {
    fillTriangle(x1, y1, x2, y2, x3, y3, fillColor);
  }
  if (useStroke) {
    drawTriangle(x1, y1, x2, y2, x3, y3, strokeColor);
  }
}
