/*****************************************************************
File:        BmpImage.h
Author:      BESTMODULES
Description: BmpImage header file
History:
V1.0.1   -- initial version£»2022-07-20£»Arduino IDE : v1.8.16
******************************************************************/
#ifndef __BMPIMAGE_
#define __BMPIMAGE_

#include "SD.h"
#include "Arduino.h"
#include "ImageInf.h"

#define BMPIMAGE  "BmpImage"

using namespace SDLib;

class BmpImage: public ImageInf{
  public:
    File _bmpFile;
    bool     _flip=0;
    uint32_t _bmpImageOffset=0;        // Start of image data in file
    uint32_t _rowSize=0;
    int  _bmpWidth, _bmpHeight=0;
    BmpImage(){}
    bool loadImage(const char * fileName, HardwareSerial *pSerial);
    operator bool() { return _valid; }
    const char* getName(){ return name; }
    int width() { return _bmpWidth; }
    int height() { return _bmpHeight; }
  private:
    const char* name = BMPIMAGE;
    uint8_t  _bmpDepth;
    bool     _valid;
};
#endif