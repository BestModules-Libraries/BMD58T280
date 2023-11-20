/*****************************************************************
File:        BMD58T280.h
Author:      BESTMODULES
Description: BMD58T280 header file
History:     V1.0.2   -- 2022-11-20
******************************************************************/
#ifndef __BMD58T280_
#define __BMD58T280_

#include "SD.h"
#include "Arduino.h"
#include "Print.h"
#include "utility/BM_GraphicsLib.h"
#include "utility/BM_ILI9341.h"
#include "utility/BmpImage.h"
#include "BM_XPT2046.h"
#include <SPI.h>

#define TFTWIDTH 240  // TFT width
#define TFTHEIGHT 320 // TFT height
#define BUFFPIXEL 20

using namespace SDLib;
class BMD58T280 : public BM_ILI9341{
  public:
    BMD58T280();
    BMD58T280(SPIClass *spiClass);
    void begin(uint32_t freq = 0);
    void image(ImageInf& img, uint16_t x, uint16_t y);
  protected:
    void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
    void drawHLine(int16_t x, int16_t y, int16_t w, uint16_t color);
    void drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color);
    inline void writeFillRectPreclipped(int16_t x, int16_t y, int16_t w,
                                        int16_t h, uint16_t color);
};
#endif
