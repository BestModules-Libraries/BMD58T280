#ifndef __FTF_SHI_H
#define __FTF_SHI_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------------------------------------*/
#include <SPI.h>
#include "Arduino.h"
#include "TFT_LowLevelInf.h"


class TFT_SPI: public TFT_LowLevelInf{
  public:
    const uint8_t _dc = A1;
    const uint8_t _cs = A3;
    const uint8_t _bl = A0;
    SPIClass *_spiClass;
    uint8_t _spiMode;
    uint32_t _freq;
    TFT_SPI(SPIClass *spiClass)
    {
      _spiClass = spiClass;
    }
    void init(uint32_t freq, uint8_t spiMode = SPI_MODE0)
    {
      _freq = freq;
      digitalWrite(_bl, HIGH);
      _spiClass->begin();
      _spiMode = spiMode;
      digitalWrite(_dc, HIGH);
      pinMode(_dc, OUTPUT);
      digitalWrite(_cs, HIGH);
      pinMode(_cs, OUTPUT);
    }
    void writeCmd8(uint8_t comm8)
    {
      digitalWrite(_dc, LOW);
     _spiClass->transfer(comm8);
      digitalWrite(_dc, HIGH);
    }
    uint8_t readData8()
    {
      return _spiClass->transfer((uint8_t)0);
    }
    void startWrite(void)
    {
      _spiClass->beginTransaction(SPISettings(_freq, MSBFIRST, _spiMode));
      digitalWrite(_cs, LOW);
    }
    void endWrite(void)
    {
      digitalWrite(_cs, HIGH);
      _spiClass->endTransaction();
    }
    void writeData16(uint16_t data)
    {
     _spiClass->transfer((data>>8)&0xFF);
     _spiClass->transfer(data&0xFF);
      return;
    }
    void writeData8(uint8_t data)
    {
     _spiClass->transfer(data);
      return;
    }
};
#ifdef __cplusplus
}
#endif

#endif
