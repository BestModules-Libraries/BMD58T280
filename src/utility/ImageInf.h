/*****************************************************************
File:        Image interface
Author:      BESTMODULES
Description: ImageInf header file
History:
V1.0.1   -- initial version£»2022-07-20£»Arduino IDE : v1.8.16
******************************************************************/
#ifndef __IMAGEINF_
#define __IMAGEINF_

#include "Arduino.h"

class ImageInf{
  public:
  virtual ~ImageInf() {}
  virtual int width()=0;
  virtual int height()=0;
  virtual const char* getName()=0;
  virtual bool loadImage(const char * fileName, HardwareSerial *pSerial) = 0;
  static uint32_t read32(File f)
  {
    uint32_t result;
    ((uint8_t *)&result)[0] = f.read(); // LSB
    ((uint8_t *)&result)[1] = f.read();
    ((uint8_t *)&result)[2] = f.read();
    ((uint8_t *)&result)[3] = f.read(); // MSB
    return result;
  }
  static uint16_t read16(File f)
  {
    uint16_t result;
    ((uint8_t *)&result)[0] = f.read(); // LSB
    ((uint8_t *)&result)[1] = f.read(); // MSB
    return result;
  }
};
#endif