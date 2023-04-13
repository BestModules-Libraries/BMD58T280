/*****************************************************************
File:        BM_GraphicsLib.h
Author:      BESTMODULES
Description: BM_GraphicsLib header file
History:
V1.0.1   -- initial version£»2022-07-20£»Arduino IDE : v1.8.16
******************************************************************/
#ifndef _BM_GRAPHICSLIB
#define _BM_GRAPHICSLIB
#include "SD.h"
using namespace SDLib;
#if ARDUINO >= 100
 #include "Arduino.h"
 #include "Print.h"
#else
 #include "WProgram.h"
#endif

#define swap(a, b) { int16_t t = a; a = b; b = t; }

class BM_GraphicsLib : public Print {
 public:
  BM_GraphicsLib(int16_t w, int16_t h);

#if ARDUINO >= 100
  virtual size_t write(uint8_t);
#else
  virtual void   write(uint8_t);
#endif
  int16_t  height(void) { return _height;  }
  int16_t  width(void) { return _width; }

  virtual void setRotation(__attribute__ ((unused))uint8_t r) {}
  virtual uint8_t getRotation(void) { return rotation; }

  virtual void fill(uint8_t red, uint8_t green, uint8_t blue) { fill(newColor(red, green, blue)); }
  virtual void fill(uint16_t c) { useFill = true; fillColor = c; }

  virtual void stroke(uint8_t red, uint8_t green, uint8_t blue) { stroke(newColor(red, green, blue)); }
  virtual void stroke(uint16_t c){ useStroke = true; strokeColor = c; setTextColor(c); }

  virtual void noFill() { useFill = false; }
  virtual void noStroke() {useStroke = false;}

  virtual void background(uint8_t red, uint8_t green, uint8_t blue) {  background(newColor(red, green, blue));}
  virtual void background(uint16_t c) { fillScreen(c); }

  virtual void text(const char * text, int16_t x, int16_t y);
  virtual void textWrap(const char * text, int16_t x, int16_t y);
  virtual void textSize(uint8_t size) { setTextSize(size); }
  virtual void setTextSize(uint8_t s){ textsize = (s > 0) ? s : 1; }

  virtual void circle(int16_t x, int16_t y, int16_t r);
  virtual void point(int16_t x, int16_t y);
  virtual void line(int16_t x1, int16_t y1, int16_t x2, int16_t y2);
  virtual void rect(int16_t x, int16_t y, int16_t width, int16_t height);
  virtual void rect(int16_t x, int16_t y, int16_t width, int16_t height, int16_t radius);
  virtual void triangle(int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t x3, int16_t y3);
  virtual void drawImage(int16_t x, int16_t y, const uint8_t *_image, int16_t w, int16_t h, uint16_t color);
 protected:
  virtual void drawPixel(__attribute__ ((unused))int16_t x, __attribute__ ((unused))int16_t y, __attribute__ ((unused))uint16_t color){};
  virtual void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);
  virtual void drawVLine(int16_t x, int16_t y, int16_t h, uint16_t color);
  virtual void drawHLine(int16_t x, int16_t y, int16_t w, uint16_t color);
  virtual void drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
  virtual void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
  virtual void fillScreen(uint16_t color);
  virtual void invertDisplay(__attribute__ ((unused))boolean i){};
  virtual void drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
  virtual void drawCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, uint16_t color);
  virtual void fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
  virtual void fillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, int16_t delta, uint16_t color);

  virtual void drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);
  virtual void fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);
  virtual void drawRoundRect(int16_t x0, int16_t y0, int16_t w, int16_t h, int16_t radius, uint16_t color);
  virtual void fillRoundRect(int16_t x0, int16_t y0, int16_t w, int16_t h, int16_t radius, uint16_t color);

  
  virtual void drawChar(int16_t x, int16_t y, unsigned char c, uint16_t color, uint16_t bg, uint8_t size);
  virtual void setCursor(int16_t x, int16_t y){ cursor_x = x; cursor_y = y;}
  virtual void setTextColor(uint16_t c){ textcolor = c; textbgcolor = c;  }
  virtual void setTextColor(uint16_t c, uint16_t bg){ textcolor = c; textbgcolor = bg; }
  virtual void setTextWrap(boolean w) { wrap = w; }
  virtual uint16_t newColor(uint8_t r, uint8_t g, uint8_t b) { return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3); }
  int16_t WIDTH, HEIGHT;
  int16_t _width, _height, cursor_x, cursor_y;
  uint16_t textcolor, textbgcolor;
  uint8_t textsize,  rotation;
  boolean wrap;

  uint16_t strokeColor;
  bool useStroke;
  uint16_t fillColor;
  bool useFill;
};
#endif
