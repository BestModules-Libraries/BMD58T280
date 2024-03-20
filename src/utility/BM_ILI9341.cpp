#include "BM_ILI9341.h"

#define ILI9341_NOP 0x00     // No-op register
#define ILI9341_SWRESET 0x01 // Software reset register
#define ILI9341_RDDID 0x04   // Read display identification information
#define ILI9341_RDDST 0x09   // Read Display Status

#define ILI9341_SLPIN 0x10  // Enter Sleep Mode
#define ILI9341_SLPOUT 0x11 // Sleep Out
#define ILI9341_PTLON 0x12  // Partial Mode ON
#define ILI9341_NORON 0x13  // Normal Display Mode ON

#define ILI9341_RDMODE 0x0A     // Read Display Power Mode
#define ILI9341_RDMADCTL 0x0B   // Read Display MADCTL
#define ILI9341_RDPIXFMT 0x0C   // Read Display Pixel Format
#define ILI9341_RDIMGFMT 0x0D   // Read Display Image Format
#define ILI9341_RDSELFDIAG 0x0F // Read Display Self-Diagnostic Result

#define ILI9341_INVOFF 0x20   // Display Inversion OFF
#define ILI9341_INVON 0x21    // Display Inversion ON
#define ILI9341_GAMMASET 0x26 // Gamma Set
#define ILI9341_DISPOFF 0x28  // Display OFF
#define ILI9341_DISPON 0x29   // Display ON

#define ILI9341_CASET 0x2A // Column Address Set
#define ILI9341_PASET 0x2B // Page Address Set
#define ILI9341_RAMWR 0x2C // Memory Write
#define ILI9341_RAMRD 0x2E // Memory Read

#define ILI9341_PTLAR 0x30    // Partial Area
#define ILI9341_VSCRDEF 0x33  // Vertical Scrolling Definition
#define ILI9341_MADCTL 0x36   // Memory Access Control
#define ILI9341_VSCRSADD 0x37 // Vertical Scrolling Start Address
#define ILI9341_PIXFMT 0x3A   // COLMOD: Pixel Format Set

#define ILI9341_FRMCTR1 0xB1 // Frame Rate Control (In Normal Mode/Full Colors)
#define ILI9341_FRMCTR2 0xB2 // Frame Rate Control (In Idle Mode/8 colors)
#define ILI9341_FRMCTR3 0xB3 // Frame Rate control (In Partial Mode/Full Colors)
#define ILI9341_INVCTR 0xB4  // Display Inversion Control
#define ILI9341_DFUNCTR 0xB6 // Display Function Control

#define ILI9341_PWCTR1 0xC0 // Power Control 1
#define ILI9341_PWCTR2 0xC1 // Power Control 2
#define ILI9341_PWCTR3 0xC2 // Power Control 3
#define ILI9341_PWCTR4 0xC3 // Power Control 4
#define ILI9341_PWCTR5 0xC4 // Power Control 5
#define ILI9341_VMCTR1 0xC5 // VCOM Control 1
#define ILI9341_VMCTR2 0xC7 // VCOM Control 2

#define ILI9341_RDID1 0xDA // Read ID 1
#define ILI9341_RDID2 0xDB // Read ID 2
#define ILI9341_RDID3 0xDC // Read ID 3
#define ILI9341_RDID4 0xDD // Read ID 4

#define ILI9341_GMCTRP1 0xE0 // Positive Gamma Correction
#define ILI9341_GMCTRN1 0xE1 // Negative Gamma Correction
#define ILI9341_PWCTR6     0xFC

#define MADCTL_MY 0x80  // Bottom to top
#define MADCTL_MX 0x40  // Right to left
#define MADCTL_MV 0x20  // Reverse Mode
#define MADCTL_ML 0x10  // LCD refresh Bottom to top
#define MADCTL_RGB 0x00 // Red-Green-Blue pixel order
#define MADCTL_BGR 0x08 // Blue-Green-Red pixel order
#define MADCTL_MH 0x04  // LCD refresh right to left


/**********************************************************
Description: Constructor
Input:       w : Display width in pixels
             h : Display height in pixels
             SPIClass : SPI's objecgt
Output:
Return:
Others:
**********************************************************/
BM_ILI9341::BM_ILI9341(uint16_t w, uint16_t h, SPIClass *spiClass)
    : BM_GraphicsLib(w, h)
{
  if(spiClass != NULL)
    lowLevel = new TFT_SPI(spiClass);
  else
  {
    #if !defined(ARDUINO_AVR_UNO)
    lowLevel = new TFT_EBI();
    #endif
  }
}

/**********************************************************
Description: ILI9341 initialization
Input:       freq : SPI's sclk freq
             spiMode : SPI transfer mode
Output:
Return:
Others:
**********************************************************/
void BM_ILI9341::initTFT(uint32_t freq, uint8_t spiMode)
{
  lowLevel->init(freq, spiMode);
  writeCmd(ILI9341_SWRESET);
  delay(150);

  startWrite();
  lowLevel->writeCmd8(0xEF);             lowLevel->writeData8(0x03);  lowLevel->writeData8(0x80);  lowLevel->writeData8(0x02);
  lowLevel->writeCmd8(0xCF);             lowLevel->writeData8(0x00);  lowLevel->writeData8(0xC1);  lowLevel->writeData8(0x30);
  lowLevel->writeCmd8(0xED);             lowLevel->writeData8(0x64);  lowLevel->writeData8(0x03);  lowLevel->writeData8(0x12);  lowLevel->writeData8(0x81);
  lowLevel->writeCmd8(0xE8);             lowLevel->writeData8(0x85);  lowLevel->writeData8(0x00);  lowLevel->writeData8(0x78);
  lowLevel->writeCmd8(0xCB);             lowLevel->writeData8(0x39);  lowLevel->writeData8(0x2C);  lowLevel->writeData8(0x00);  lowLevel->writeData8(0x34);
                                        lowLevel->writeData8(0x02);
  lowLevel->writeCmd8(0xF7);             lowLevel->writeData8(0x20);
  lowLevel->writeCmd8(0xEA);             lowLevel->writeData8(0x00);  lowLevel->writeData8(0x00);
  lowLevel->writeCmd8(ILI9341_PWCTR1);   lowLevel->writeData8(0x23);
  lowLevel->writeCmd8(ILI9341_PWCTR2);   lowLevel->writeData8(0x10);
  lowLevel->writeCmd8(ILI9341_VMCTR1);   lowLevel->writeData8(0x3E);  lowLevel->writeData8(0x28);
  lowLevel->writeCmd8(ILI9341_VMCTR2);   lowLevel->writeData8(0x86);
  lowLevel->writeCmd8(ILI9341_MADCTL);   lowLevel->writeData8(0x48);
  lowLevel->writeCmd8(ILI9341_VSCRSADD); lowLevel->writeData8(0x00);
  lowLevel->writeCmd8(ILI9341_PIXFMT);   lowLevel->writeData8(0x55);
  lowLevel->writeCmd8(ILI9341_FRMCTR1);  lowLevel->writeData8(0x00);  lowLevel->writeData8(0x18);
  lowLevel->writeCmd8(ILI9341_DFUNCTR);  lowLevel->writeData8(0x08);  lowLevel->writeData8(0x82);  lowLevel->writeData8(0x27);
  lowLevel->writeCmd8(0xF2);             lowLevel->writeData8(0x00);
  lowLevel->writeCmd8(ILI9341_GAMMASET); lowLevel->writeData8(0x01);
  lowLevel->writeCmd8(ILI9341_GMCTRP1);  lowLevel->writeData8(0x0F);  lowLevel->writeData8(0x31);  lowLevel->writeData8(0x2B);
                                         lowLevel->writeData8(0x0C);  lowLevel->writeData8(0x0E);  lowLevel->writeData8(0x08);
                                         lowLevel->writeData8(0x4E);  lowLevel->writeData8(0xF1);  lowLevel->writeData8(0x37);
                                         lowLevel->writeData8(0x07);  lowLevel->writeData8(0x10);  lowLevel->writeData8(0x03);
                                         lowLevel->writeData8(0x0E);  lowLevel->writeData8(0x09);  lowLevel->writeData8(0x00);
  lowLevel->writeCmd8(ILI9341_GMCTRN1);  lowLevel->writeData8(0x00);  lowLevel->writeData8(0x0E);  lowLevel->writeData8(0x14);
                                         lowLevel->writeData8(0x03);  lowLevel->writeData8(0x11);  lowLevel->writeData8(0x07);
                                         lowLevel->writeData8(0x31);  lowLevel->writeData8(0xC1);  lowLevel->writeData8(0x48);
                                         lowLevel->writeData8(0x08);  lowLevel->writeData8(0x0F);  lowLevel->writeData8(0x0C);
                                         lowLevel->writeData8(0x31);  lowLevel->writeData8(0x36);  lowLevel->writeData8(0x0F);
  lowLevel->writeCmd8(ILI9341_SLPOUT);   delay(150);
  lowLevel->writeCmd8(ILI9341_DISPON);   delay(150);
  endWrite();
}

/**********************************************************
Description:
Input:
Output:
Return:
Others:
**********************************************************/
void BM_ILI9341::setRotation(uint8_t m) {
  rotation = m % 4; // can't be higher than 3
  uint16_t w = this->_width;
  uint16_t h = this->_height;
  switch (rotation)
  {
     case 0:
      m = (MADCTL_MX | MADCTL_BGR);
      BM_GraphicsLib::_width = 240;
      BM_GraphicsLib::_height = 320;
      break;
    case 1:
      m = (MADCTL_MV | MADCTL_BGR);
      BM_GraphicsLib::_width = 320;
      BM_GraphicsLib::_height = 240;
      break;
    case 2:
      m = (MADCTL_MY | MADCTL_BGR);
      BM_GraphicsLib::_width = 240;
      BM_GraphicsLib::_height = 320;
      break;
    case 3:
      m = (MADCTL_MX | MADCTL_MY | MADCTL_MV | MADCTL_BGR);
      BM_GraphicsLib::_width = 320;
      BM_GraphicsLib::_height = 240;
      break;
  }
  writeCmd(ILI9341_MADCTL, &m, 1);
}

/**********************************************************
Description: invert Display
Input: true / false
Output:
Return:
Others:
**********************************************************/
void BM_ILI9341::invertDisplay(bool invert) {
  writeCmd(invert ? ILI9341_INVON : ILI9341_INVOFF);
}

/**********************************************************
Description: Scroll
Input:       y : scroll y pixels
Output:
Return:
Others:
**********************************************************/
void BM_ILI9341::scrollTo(uint16_t y) {
  uint8_t data[2];
  data[0] = y >> 8;
  data[1] = y & 0xff;
  writeCmd(ILI9341_VSCRSADD, (uint8_t *)data, 2);
}

/**********************************************************
Description: Scroll margins
Input:       top : The height of the top scroll margin
             botton : The height of the botton scroll margin
Output:
Return:
Others:
**********************************************************/
void BM_ILI9341::setScrollMargins(uint16_t top, uint16_t bottom) {
  if (top + bottom <= (uint16_t)_height)
  {
    uint16_t middle = _height - (top + bottom);
    uint8_t data[6];
    data[0] = top >> 8;
    data[1] = top & 0xff;
    data[2] = middle >> 8;
    data[3] = middle & 0xff;
    data[4] = bottom >> 8;
    data[5] = bottom & 0xff;
    writeCmd(ILI9341_VSCRDEF, (uint8_t *)data, 6);
  }
}

/**********************************************************
Description: Read register
Input:       commandByte : Command
             index       : index
Output:
Return:      register data
Others:
**********************************************************/
uint8_t BM_ILI9341::readReg(uint8_t commandByte, uint8_t index) {
  uint8_t data = 0x10 + index;
  writeCmd(0xD9, &data, 1); // Set Index Register
  return readData(commandByte);
}

/**********************************************************
Description: Write color
Input:       color : Color
             len   : Length of pixels
Output:
Return:
Others:
**********************************************************/
void BM_ILI9341::writeColor(uint16_t color, uint32_t len) {

  if (!len)
    return;
  while (len--)
  {
    lowLevel->writeData16(color);
  }
}
/**********************************************************
Description: Draw pixels
Input:       x : The horizontal position
             y : The vertical position
             w : Display width in pixels
             h : Display height in pixels
             pColor : color point
Output:
Return:
Others:
**********************************************************/
void BM_ILI9341::drawPixels(int16_t x,
                           int16_t y,
                            int16_t w,
                           int16_t h,
                           const PROGMEM uint8_t* pColor )
{
  
  lowLevel->startWrite();
  setAddrWindow(x, y, w, h);
  for(int i=0;i<w;i++)
  {
    for(int j=0;j<(h*2);j+=2)
    {
      u16 _color = (u16)((pgm_read_byte(&pColor[j+i*h*2])<<8) | pgm_read_byte(&pColor[j+i*h*2+1]));
      lowLevel->writeData16(_color);
    }
  }
  lowLevel->endWrite();
}

/**********************************************************
Description: Draw pixel
Input:       x : The horizontal position
             y : The vertical position
             color : Color
Output:
Return:
Others:
**********************************************************/
void BM_ILI9341::drawPixel(int16_t x,
                           int16_t y,
                           uint16_t color)
{
  if ((x >= 0) && (x < _width) && (y >= 0) && (y < _height))
  {
    lowLevel->startWrite();
    setAddrWindow(x, y, 1, 1);
    lowLevel->writeData16(color);
    lowLevel->endWrite();
  }
}

/**********************************************************
Description: Write command
Input:       commandByte : Command
             dataBytes : data buffer
             numDataBytes  : data buffer length
Output:
Return:
Others:
**********************************************************/
void BM_ILI9341::writeCmd(uint8_t commandByte, uint8_t *dataBytes,
                                  uint8_t numDataBytes)
{

  startWrite();
  lowLevel->writeCmd8(commandByte);
  for (int i = 0; i < numDataBytes; i++)
  {
    lowLevel->writeData8(*dataBytes);
    dataBytes ++;
  }
  endWrite();
}

/**********************************************************
Description: Read data
Input:       commandByte : Command
             index : index
Output:
Return:      data
Others:
**********************************************************/
uint8_t BM_ILI9341::readData(uint8_t commandByte, uint8_t index)
{
  uint8_t result;
  startWrite();
    lowLevel->writeCmd8(commandByte);
  do {
    result = lowLevel->readData8();
  } while (index--);
  endWrite();
  return result;
}

/**********************************************************
Description: Set address window
Input:       x : The horizontal position
             y : The vertical position
             width : The width of the window
             height : The height of the window
Output:
Return:
Others:
**********************************************************/
void BM_ILI9341::setAddrWindow(uint16_t x1, uint16_t y1, uint16_t w,
                                     uint16_t h) {
  static uint16_t old_x1 = 0xffff, old_x2 = 0xffff;
  static uint16_t old_y1 = 0xffff, old_y2 = 0xffff;

  uint16_t x2 = (x1 + w - 1), y2 = (y1 + h - 1);
  if (x1 != old_x1 || x2 != old_x2) {
    lowLevel->writeCmd8(ILI9341_CASET); // Column address set
    lowLevel->writeData16(x1);
    lowLevel->writeData16(x2);
    old_x1 = x1;
    old_x2 = x2;
  }
  if (y1 != old_y1 || y2 != old_y2) {
    lowLevel->writeCmd8(ILI9341_PASET); // Row address set
    lowLevel->writeData16(y1);
    lowLevel->writeData16(y2);
    old_y1 = y1;
    old_y2 = y2;
  }
  lowLevel->writeCmd8(ILI9341_RAMWR); // Write to RAM
}
