/*****************************************************************
File:        BmpImage.cpp
Author:      BESTMODULES
Description: BmpImage.cpp is used to parse BMP image files
History:
V1.0.1   -- initial version£»2022-07-20£»Arduino IDE : v1.8.16
******************************************************************/
#include "BmpImage.h"

/**********************************************************
Description: Get BMP image file information in SD
Input:       fileName : faile name
             pSerial : Serial obj for debug
Output:
Return:      true: Get file information / false: Failed
Others:
**********************************************************/
bool BmpImage::loadImage(const char * fileName, HardwareSerial *pSerial)
{
  File     bmpFile;
  int      bmpWidth, bmpHeight;   // W+H in pixels
  uint8_t  bmpDepth;              // Bit depth (currently must be 24)
  uint32_t bmpImageoffset;        // Start of image data in file
  uint32_t rowSize;               // Not always = bmpWidth; may have padding
  bool     flip    = true;        // BMP is stored bottom-to-top

  // Open requested file on SD card
  if ((bmpFile = SD.open(fileName)) == 0) {
    if(pSerial != NULL)
    {
      pSerial->print(F("loadImage: file not found: "));
      pSerial->println(fileName);
    }
    return false;
  }

  // Parse BMP header
  if(ImageInf::read16(bmpFile) != 0x4D42) { // BMP signature
    if(pSerial != NULL)
      pSerial->println(F("loadImage: file doesn't look like a BMP"));
    return false;
  }

  if(pSerial != NULL)
  {
    pSerial->print(F("File size: ")); pSerial->println(ImageInf::read32(bmpFile));
  }
  else
  {
    ImageInf::read32(bmpFile);
  }
  (void)ImageInf::read32(bmpFile); // Read & ignore creator bytes
  bmpImageoffset = ImageInf::read32(bmpFile); // Start of image data
  if(pSerial != NULL)
  {
    pSerial->print(F("Image Offset: ")); pSerial->println(bmpImageoffset, DEC);
  }
  // Read DIB header
  if(pSerial != NULL)
  {
    pSerial->print(F("Header size: ")); pSerial->println(ImageInf::read32(bmpFile));
  }
  else
    ImageInf::read32(bmpFile);

  bmpWidth  = ImageInf::read32(bmpFile);
  bmpHeight = ImageInf::read32(bmpFile);
  if(ImageInf::read16(bmpFile) != 1) { // # planes -- must be '1'
    if(pSerial != NULL)
      pSerial->println(F("loadImage: invalid n. of planes"));
    return false;
  }

  bmpDepth = ImageInf::read16(bmpFile); // bits per pixel
  if(pSerial != NULL)
  {
    pSerial->print(F("Bit Depth: "));
    pSerial->println(bmpDepth);
  }
  if((bmpDepth != 24) || (ImageInf::read32(bmpFile) != 0)) { // 0 = uncompressed {
    if(pSerial != NULL)
      pSerial->println(F("loadImage: invalid pixel format"));
    return false;
  }
  if(pSerial != NULL)
  {
    pSerial->print(F("Image size: "));
    pSerial->print(bmpWidth);
    pSerial->print('x');
    pSerial->println(bmpHeight);
  }
  rowSize = (bmpWidth * 3 + 3) & ~3;

  if(bmpHeight < 0) {
    bmpHeight = -bmpHeight;
    flip      = false;
  }
  _bmpFile = bmpFile;
  _bmpWidth = bmpWidth;
  _bmpHeight = bmpHeight;
  _bmpDepth = bmpDepth;
  _bmpImageOffset = bmpImageoffset;
  _rowSize = rowSize;
  _flip = flip;
  _valid = true;
  return true;
}