#ifndef _BM_ILI9341_H_
#define _BM_ILI9341_H_

#include "BM_GraphicsLib.h"
#include <SPI.h>

#if defined(ARDUINO_BM53A367A)
#include "TFT_EBI.h"
#endif
#include "TFT_SPI.h"

class BM_ILI9341 : public BM_GraphicsLib {
public:
  /* 16-bit color(R, G, B) constant for BM_ILI9341 */
  enum{
    BLACK       = 0x0000,       //   0,   0,   0
    NAVY        = 0x000F,       //   0,   0, 123
    DARKGREEN   = 0x03E0,       //   0, 125,   0
    DARKCYAN    = 0x03EF,       //   0, 125, 123
    MAROON      = 0x7800,       // 123,   0,   0
    PURPLE      = 0x780F,       // 123,   0, 123
    OLIVE       = 0x7BE0,       // 123, 125,   0
    LIGHTGREY   = 0xC618,       // 198, 195, 198
    DARKGREY    = 0x7BEF,       // 123, 125, 123
    BLUE        = 0x001F,       //   0,   0, 255
    GREEN       = 0x07E0,       //   0, 255,   0
    CYAN        = 0x07FF,       //   0, 255, 255
    RED         = 0xF800,       // 255,   0,   0
    MAGENTA     = 0xF81F,       // 255,   0, 255
    YELLOW      = 0xFFE0,       // 255, 255,   0
    WHITE       = 0xFFFF,       // 255, 255, 255
    ORANGE      = 0xFD20,       // 255, 165,   0
    GREENYELLOW = 0xAFE5,       // 173, 255,  41
    PINK        = 0xFC18        // 255, 130, 198
  } Color;

  void setScrollMargins(uint16_t top, uint16_t bottom);
  void scrollTo(uint16_t y);
  void setRotation(uint8_t r);
  void setAddrWindow(uint16_t x, uint16_t y, uint16_t w, uint16_t h);
  TFT_LowLevelInf* lowLevel;
  void invertDisplay(bool invert);
  void drawPixels(int16_t x,
                  int16_t y,
                  int16_t w,
                  int16_t h,
                  const uint8_t* pColor);
  void writeColor(uint16_t color, uint32_t len);
protected:
  BM_ILI9341(uint16_t w, uint16_t h, SPIClass *spiClass);
  ~BM_ILI9341(){};
  void initTFT(uint32_t freq = 0, uint8_t spiMode = SPI_MODE0);


  void startWrite(void) { lowLevel->startWrite(); }
  void endWrite(void) { lowLevel->endWrite(); }
  void writeCmd(uint8_t commandByte,
                uint8_t *dataBytes = NULL,
                uint8_t numDataBytes = 0);
  void drawPixel(int16_t x, int16_t y, uint16_t color);
  uint8_t readReg(uint8_t reg, uint8_t index = 0);
  uint8_t readData(uint8_t commandByte, uint8_t index = 0);
};

#endif
