#include "BM_XPT2046.h"

#define Z_THRESHOLD          400
#define Z_THRESHOLD_INT      75
#define MSEC_THRESHOLD       3
#define SPI_SETTING          SPISettings(2000000, MSBFIRST, SPI_MODE0)

/**********************************************************
Description: brgin
Input:       wspi : SPI's object
Output:
Return:
Others:
**********************************************************/
bool BM_XPT2046::begin(SPIClass &wspi)
{
  _pspi = &wspi;
  _pspi->begin();
  pinMode(_csPin, OUTPUT);
  digitalWrite(_csPin, HIGH);
  if (255 != _tirqPin) {
    pinMode( _tirqPin, INPUT );
  }
  return true;
}

/**********************************************************
Description: Get point
Input:
Output:
Return: point
Others:
**********************************************************/
BM_XPT2046::TS_Point BM_XPT2046::getPoint()
{
  _update();
  return BM_XPT2046::TS_Point(_xraw, _yraw, _zraw);
}

/**********************************************************
Description: Get the status of INT (Trigger Pin)
Input:
Output:
Return: true / false
Others:
**********************************************************/
bool BM_XPT2046::_tirqTouched()
{
  if(digitalRead(_tirqPin) == LOW)
  {
    _isrWake = true;
    return (_isrWake);
  }
  return false;
}

/**********************************************************
Description: Whether to touch the screen
Input:
Output:
Return:      true / false
Others:
**********************************************************/
bool BM_XPT2046::touched()
{
  _zraw =0;
  _update();
  return (_zraw >= Z_THRESHOLD);
}

/**********************************************************
Description: Read data
Input:
Output:      uint16_t *x : The point's X value
             uint16_t *y : The point's Y value
             uint8_t  *z  :The point's Z value
Return:
Others:
**********************************************************/
void BM_XPT2046::_readData(uint16_t *x, uint16_t *y, uint8_t *z)
{
  _update();
  *x = _xraw;
  *y = _yraw;
  *z = _zraw;
}

/**********************************************************
Description: Is buffer empty
Input:
Output:
Return:      true / false
Others:
**********************************************************/
bool BM_XPT2046::_bufferEmpty()
{
  return ((millis() - _msraw) < MSEC_THRESHOLD);
}

/**********************************************************
Description: best two avg
Input:       uint16_t *x : The point's X value
             uint16_t *y : The point's Y value
             uint16_t *z  :The point's Z value
Output:
Return: avg
Others:
**********************************************************/
static int16_t besttwoavg( int16_t x , int16_t y , int16_t z ) {
  int16_t da, db, dc;
  int16_t reta = 0;
  if ( x > y ) da = x - y; else da = y - x;
  if ( x > z ) db = x - z; else db = z - x;
  if ( z > y ) dc = z - y; else dc = y - z;

  if ( da <= db && da <= dc ) reta = (x + y) >> 1;
  else if ( db <= da && db <= dc ) reta = (x + z) >> 1;
  else reta = (y + z) >> 1;

  return (reta);
}

/**********************************************************
Description: Update touch value
Input:
Output:
Return:
Others:
**********************************************************/
void BM_XPT2046::_update()
{
  int16_t data[6];
  int z;
  if (!_tirqTouched()) return;
  uint32_t now = millis();
  if (now - _msraw < MSEC_THRESHOLD) return;
  if (_pspi == NULL)
  {
    return;
  }
  else
  {
    _pspi->beginTransaction(SPI_SETTING);
    digitalWrite(_csPin, LOW);
    _pspi->transfer(0xB1 /* Z1 */);
    int16_t z1 = _pspi->transfer16(0xC1 /* Z2 */) >> 3;
    z = z1 + 4095;
    int16_t z2 = _pspi->transfer16(0x91 /* X */) >> 3;
    z -= z2;
    if (z >= Z_THRESHOLD) {
      _pspi->transfer16(0x91 /* X */);  // dummy X measure, 1st is always noisy
      data[0] = _pspi->transfer16(0xD1 /* Y */) >> 3;
      data[1] = _pspi->transfer16(0x91 /* X */) >> 3; // make 3 x-y measurements
      data[2] = _pspi->transfer16(0xD1 /* Y */) >> 3;
      data[3] = _pspi->transfer16(0x91 /* X */) >> 3;
    }
    else data[0] = data[1] = data[2] = data[3] = 0;  // Compiler warns these values may be used unset on early exit.
    data[4] = _pspi->transfer16(0xD0 /* Y */) >> 3;  // Last Y touch power down
    data[5] = _pspi->transfer16(0) >> 3;
    digitalWrite(_csPin, HIGH);
    _pspi->endTransaction();
  }

  //Serial.printf("z=%d  ::  z1=%d,  z2=%d  ", z, z1, z2);
  if (z < 0) z = 0;
  if (z < Z_THRESHOLD) { //  if ( !touched ) {
    // Serial.println();
    _zraw = 0;
    if (z < Z_THRESHOLD_INT) { //  if ( !touched ) {
      if (255 != _tirqPin) _isrWake = false;
    }
    return;
  }
  _zraw = z;

  // Average pair with least distance between each measured x then y
  int16_t x = besttwoavg( data[0], data[2], data[4] );
  int16_t y = besttwoavg( data[1], data[3], data[5] );

  if (z >= Z_THRESHOLD) {
    _msraw = now;  // good read completed, set wait
    switch (_rotation) {
      case 0:
      _xraw = y;
      _yraw = x;
      break;
      case 1:
      _xraw = x;
      _yraw = 4095 - y;
      break;
      case 2: // 2
      _xraw = 4095 - y;
      _yraw = 4095 - x;
      break;
      default:
      _xraw = 4095 - x;
      _yraw = y;
      break;
    }
  }
}
