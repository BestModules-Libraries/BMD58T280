/*****************************************************************
File:        BM_XPT2046.h
Author:      BESTMODULES
Description: BM_XPT2046 header file
History:
V1.0.1   -- initial version£»2022-07-20£»Arduino IDE : v1.8.16
******************************************************************/
#ifndef _BM_XPT2046_h_
#define _BM_XPT2046_h_

#include "Arduino.h"
#include <SPI.h>


#if ARDUINO < 10600
#error "Arduino 1.6.0 or later (SPI library) is required"
#endif

class BM_XPT2046 {
public:
  class TS_Point {
  public:
    TS_Point(void) : x(0), y(0), z(0) {}
    TS_Point(int16_t x, int16_t y, int16_t z) : x(x), y(y), z(z) {}
    bool operator==(TS_Point p) { return ((p.x == x) && (p.y == y) && (p.z == z)); }
    bool operator!=(TS_Point p) { return ((p.x != x) || (p.y != y) || (p.z != z)); }
    int16_t x, y, z;
  };
  constexpr BM_XPT2046(uint8_t cspin=18, uint8_t tirq=19)
    : _csPin(cspin), _tirqPin(tirq) { }
  bool begin(SPIClass &wspi = SPI);

  TS_Point getPoint();
  bool touched();
  void setRotation(uint8_t n) { _rotation = n ; }

private:
  bool _tirqTouched();
  void _readData(uint16_t *x, uint16_t *y, uint8_t *z);
  bool _bufferEmpty();
  uint8_t _bufferSize() { return 1; }
  volatile bool _isrWake=true;
  void _update();
  uint8_t _csPin, _tirqPin, _rotation=0;
  int16_t _xraw=0, _yraw=0, _zraw=0;
  uint32_t _msraw=0x80000000;
  SPIClass *_pspi = nullptr;
};
#endif
