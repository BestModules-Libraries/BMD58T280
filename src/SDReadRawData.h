/*****************************************************************
File:        SDReadRawData.h
Author:      BESTMODULES
Description: SDReadRawData header file
History:     V1.0.2   -- 2022-11-20
******************************************************************/
#ifndef _SDREADROWDATA_H
#define _SDREADROWDATA_H

#include <SD.h>
#include <Arduino.h>

class SDReadRawData {
  public:
    SDReadRawData(void) : errorCode_(0), type_(0) {}
    uint8_t init(uint8_t chipSelectPin);
    uint8_t readBlock(uint32_t block, uint16_t count, uint8_t* dst);
    void waitDmaFinish();
  private:
    uint8_t chipSelectPin_;
    uint8_t errorCode_;
    uint8_t status_;
    uint8_t type_;

    void spiDmaRead(uint8_t *rx_buffer, uint16_t len);
    uint8_t cardAcmd(uint8_t cmd, uint32_t arg)
    {
      cardCommand(CMD55, 0);
      return cardCommand(cmd, arg);
    }
    uint8_t cardCommand(uint8_t cmd, uint32_t arg);
    void error(uint8_t code) {
      errorCode_ = code;
    }
    void chipSelectHigh(void);
    void chipSelectLow(void);
    uint8_t waitNotBusy(unsigned int timeoutMillis);
    uint8_t writeData(uint8_t token, const uint8_t* src);
    uint8_t waitStartBlock(void);
};
#endif
