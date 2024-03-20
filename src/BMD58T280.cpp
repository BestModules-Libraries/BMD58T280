/*****************************************************************
File:        BMD58T280.cpp
Author:      BESTMODULES
Description: Driver of BMD58T280
History:     V1.0.3   -- 2024-03-14
******************************************************************/
#include "BMD58T280.h"
#include <limits.h>

/**********************************************************
Description: Constructor
Input:
Output:
Return:
Others:
**********************************************************/
BMD58T280::BMD58T280()
    : BM_ILI9341(TFTWIDTH, TFTHEIGHT, NULL) {}

/**********************************************************
Description: Constructor
Input:       *spiClass : SPI's object
Output:
Return:
Others:
**********************************************************/
BMD58T280::BMD58T280(SPIClass *spiClass)
    : BM_ILI9341(TFTWIDTH, TFTHEIGHT, spiClass){}

/**********************************************************
Description: begin
Input:       freq : SPI's sclk freq
Output:
Return:
Others:
**********************************************************/
void BMD58T280::begin(uint32_t freq) {

  if (!freq)
    freq =  F_CPU/2;
  initTFT(freq);
}

/**********************************************************
Description: Write fill a rectangle((Need to clip first)
Input:       x : The horizontal position where the line starts 
             y : The vertical position where the line starts
             w : The width of the rectangle
             h : The height of the rectangle
             color : pattern color
Output:
Return:
Others:
**********************************************************/
inline void BMD58T280::writeFillRectPreclipped(int16_t x,
                                                int16_t y,
                                                int16_t w,
                                                int16_t h,
                                                uint16_t color)
{
  setAddrWindow(x, y, w, h);
  writeColor(color, (uint32_t)w * h);
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
void BMD58T280::fillRect(int16_t x, int16_t y, int16_t w, int16_t h,
                               uint16_t color) {
  if (w && h)
  {   // Nonzero width and height?
    if (w < 0) {  // If negative width...
      x += w + 1; //   Move X to left edge
      w = -w;     //   Use positive width
    }
    if (x < _width)
    { // Not off right
      if (h < 0)
     {    // If negative height...
        y += h + 1;   //   Move Y to top edge
        h = -h;       //   Use positive height
      }
      if (y < _height)
      { // Not off bottom
        int16_t x2 = x + w - 1;
        if (x2 >= 0) 
        { // Not off left
          int16_t y2 = y + h - 1;
          if (y2 >= 0) 
          { // Not off top
            // Rectangle partly or fully overlaps screen
            if (x < 0) 
            {
              x = 0;
              w = x2 + 1;
            } // Clip left
            if (y < 0) 
            {
              y = 0;
              h = y2 + 1;
            } // Clip top
            if (x2 >= _width) 
            {
              w = _width - x;
            } // Clip right
            if (y2 >= _height) 
            {
              h = _height - y;
            } // Clip bottom
            startWrite();
            writeFillRectPreclipped(x, y, w, h, color);
            endWrite();
          }
        }
      }
    }
  }
}

/**********************************************************
Description: draw horizontal line
Input:       x : The horizontal position where the line starts 
             y : The vertical position where the line starts
             w : The width of the rectangle
             color : pattern color
Output:
Return:
Others:
**********************************************************/
void BMD58T280::drawHLine(int16_t x, int16_t y, int16_t w,
                                    uint16_t color) {
  if ((y >= 0) && (y < _height) && w) // Y on screen, nonzero width
  {
    if (w < 0)                         // If negative width...
    {
      x += w + 1;                       //   Move X to left edge
      w = -w;                           //   Use positive width
    }
    if (x < _width) // Not off right
    {
      int16_t x2 = x + w - 1;
      if (x2 >= 0) // Not off left
      { 
        // Line partly or fully overlaps screen
        if (x < 0) 
        {
          x = 0;
          w = x2 + 1;
        } // Clip left
        if (x2 >= _width)
        {
          w = _width - x;
        } // Clip right
        startWrite();
        writeFillRectPreclipped(x, y, w, 1, color);
        endWrite();
      }
    }
  }
}

/**********************************************************
Description: draw vertical line
Input:       x : The horizontal position where the line starts 
             y : The vertical position where the line starts
             h : The height of the rectangle
             color : pattern color
**********************************************************/
void BMD58T280::drawFastVLine(int16_t x, int16_t y, int16_t h,
                                    uint16_t color) {
  if ((x >= 0) && (x < _width) && h)  // X on screen, nonzero height
  {
    if (h < 0)                      // If negative height...
    {
      y += h + 1;                      //   Move Y to top edge
      h = -h;                          //   Use positive height
    }
    if (y < _height)// Not off bottom
    { 
      int16_t y2 = y + h - 1;
      if (y2 >= 0)// Not off top
      { 
        // Line partly or fully overlaps screen
        if (y < 0) {
          y = 0;
          h = y2 + 1;
        } // Clip top
        if (y2 >= _height)
        {
          h = _height - y;
        } // Clip bottom
        startWrite();
        writeFillRectPreclipped(x, y, 1, h, color);
        endWrite();
      }
    }
  }
}

/**********************************************************
Description: draw image
Input:       _img : image
             y : The vertical position where the line starts
             h : The height of the rectangle
**********************************************************/
void BMD58T280::image(ImageInf& _img, uint16_t x, uint16_t y)
{
  int      w, h, row, col;
  uint8_t  r, g, b;
  uint32_t pos = 0;
  uint8_t  sdbuffer[3*BUFFPIXEL]; // pixel buffer (R+G+B per pixel)
  uint8_t  buffidx = sizeof(sdbuffer); // Current position in sdbuffer
  BmpImage* img;

  if (strcmp(_img.getName(), BMPIMAGE) == 0) 
  {
    img = (BmpImage*)&_img;
  }
  else
  {
    return;
  }
  
  w = img->_bmpWidth;
  h = img->_bmpHeight;
  if((x+w-1) >= (uint16_t)width())  w = width()  - x;
  if((y+h-1) >= (uint16_t)height()) h = height() - y;

  for (row=0; row<h; row++)
  { 

    if(img->_flip)
      pos = img->_bmpImageOffset + (img->_bmpHeight - 1 - row) * img->_rowSize;
    else
      pos = img->_bmpImageOffset + row * img->_rowSize;
    if(img->_bmpFile.position() != pos)
    { 
      img->_bmpFile.seek(pos);
      buffidx = sizeof(sdbuffer); 
    }

    for (col=0; col<w; col++)
    {
      if (buffidx >= sizeof(sdbuffer))
      {
        img->_bmpFile.read(sdbuffer, sizeof(sdbuffer));
        buffidx = 0;
      }

      b = sdbuffer[buffidx++];
      g = sdbuffer[buffidx++];
      r = sdbuffer[buffidx++];
      drawPixel(x + col, y + row, newColor(r, g, b));
      
    } 
  }
}
