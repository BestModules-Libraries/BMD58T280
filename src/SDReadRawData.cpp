#include <Arduino.h>
#include <SPI.h>
#include "SDReadRawData.h"

static SPISettings settings;
static uint8_t chip_select_asserted = 0;
#if !defined(ARDUINO_AVR_UNO)
#ifndef SDCARD_SPI
  #define SDCARD_SPI SPI
#endif

/**********************************************************
Description: Send SD command
Input:       cmd : command 
             arg : argument
Output:
Return:
Others:
**********************************************************/
uint8_t SDReadRawData::cardCommand(uint8_t cmd, uint32_t arg)
{
  chipSelectHigh();

  // select card
  chipSelectLow();

  // wait up to 300 ms if busy
  waitNotBusy(300);

  // send command
  SDCARD_SPI.transfer(cmd | 0x40);

  // send argument
  for (int8_t s = 24; s >= 0; s -= 8) {
    SDCARD_SPI.transfer(arg >> s);
  }

  // send CRC
  uint8_t crc = 0XFF;
  if (cmd == CMD0) {
    crc = 0X95;  // correct crc for CMD0 with arg 0
  }
  if (cmd == CMD8) {
    crc = 0X87;  // correct crc for CMD8 with arg 0X1AA
  }
  SDCARD_SPI.transfer(crc);

  // wait for response
  for (uint8_t i = 0; ((status_ = SDCARD_SPI.transfer(0xFF)) & 0X80) && i != 0XFF; i++)
    ;
  return status_;
}

/**********************************************************
Description: Set CS high level
Input:
Output:
Return:
Others:
**********************************************************/
void SDReadRawData::chipSelectHigh(void)
{
  digitalWrite(chipSelectPin_, HIGH);
  if (chip_select_asserted)
  {
    chip_select_asserted = 0;
    SDCARD_SPI.endTransaction();
  }
}

/**********************************************************
Description: Set CS low level
Input:
Output:
Return:
Others:
**********************************************************/
void SDReadRawData::chipSelectLow(void)
{
  if (!chip_select_asserted)
  {
    chip_select_asserted = 1;
    SDCARD_SPI.beginTransaction(settings);
  }
  digitalWrite(chipSelectPin_, LOW);
}

/**********************************************************
Description: SD initialization
Input:       chipSelectPin : SPI's CS
Output:
Return:
Others:
**********************************************************/
uint8_t SDReadRawData::init(uint8_t chipSelectPin)
{
  errorCode_ = type_ = 0;
  chipSelectPin_ = chipSelectPin;

  unsigned int t0 = millis();
  uint32_t arg;

  // set pin modes
  pinMode(chipSelectPin_, OUTPUT);
  digitalWrite(chipSelectPin_, HIGH);

  pinMode(SPI_MISO_PIN, INPUT);
  pinMode(SPI_MOSI_PIN, OUTPUT);
  pinMode(SPI_SCK_PIN, OUTPUT);

  SDCARD_SPI.begin();
  settings = SPISettings(250000, MSBFIRST, SPI_MODE0);

  SDCARD_SPI.beginTransaction(settings);

  for (uint8_t i = 0; i < 10; i++)
  {
    SDCARD_SPI.transfer(0XFF);
  }
  SDCARD_SPI.endTransaction();

  chipSelectLow();


  while ((status_ = cardCommand(CMD0, 0)) != R1_IDLE_STATE)
  {
    unsigned int d = millis() - t0;
    if (d > SD_INIT_TIMEOUT)
    {
      error(SD_CARD_ERROR_CMD0);
      goto fail;
    }
  }
  // check SD version
  if ((cardCommand(CMD8, 0x1AA) & R1_ILLEGAL_COMMAND))
  {
    type_ = SD_CARD_TYPE_SD1;
  }
  else
  {
    // only need last byte of r7 response
    for (uint8_t i = 0; i < 4; i++)
    {
      status_ = SDCARD_SPI.transfer(0xFF);
    }
    if (status_ != 0XAA)
    {
      error(SD_CARD_ERROR_CMD8);
      goto fail;
    }
    type_ = SD_CARD_TYPE_SD2;
  }

  // initialize card and send host supports SDHC if SD2
  arg = type_ == SD_CARD_TYPE_SD2 ? 0X40000000 : 0;

  while ((status_ = cardAcmd(ACMD41, arg)) != R1_READY_STATE)
  {
    // check for timeout
    unsigned int d = millis() - t0;
    if (d > SD_INIT_TIMEOUT)
    {
      error(SD_CARD_ERROR_ACMD41);
      goto fail;
    }
  }
  // if SD2 read OCR register to check for SDHC card
  if (type_ == SD_CARD_TYPE_SD2)
  {
    if (cardCommand(CMD58, 0))
    {
      error(SD_CARD_ERROR_CMD58);
      goto fail;
    }
    if ((SDCARD_SPI.transfer(0xFF) & 0XC0) == 0XC0)
    {
      type_ = SD_CARD_TYPE_SDHC;
    }
    // discard rest of ocr - contains allowed voltage range
    for (uint8_t i = 0; i < 3; i++)
    {
      SDCARD_SPI.transfer(0xFF);
    }
  }
  chipSelectHigh();

  settings = SPISettings(F_CPU/2, MSBFIRST, SPI_MODE3);

  return true;

fail:
  chipSelectHigh();
  return false;
}

/**********************************************************
Description: Read data through SPI with PDMA
Input:       rx_buffer : rx buffer
             len : length
Output:
Return:
Others:
**********************************************************/
void SDReadRawData::spiDmaRead(uint8_t *rx_buffer, uint16_t len)
{
  uint32_t size = len;
  u8 _tx_buffer = 0;
  while((SPI0_PORT->SR & SPI_FLAG_TXE) != SPI_FLAG_TXE);
  while((SPI0_PORT->SR & SPI_FLAG_RXBNE) == SPI_FLAG_RXBNE)
  {
    SPI0_PORT->DR;
  }

  PDMACH_InitTypeDef PDMACH_InitStructure_TX;
  PDMACH_InitTypeDef PDMACH_InitStructure_RX;
  u32 uSPITxDmaChNumber;
  u32 uSPIRxDmaChNumber;

  uSPIRxDmaChNumber = SPI0_RX_PDMA_CH;
  uSPITxDmaChNumber = SPI0_TX_PDMA_CH;

  /* SPI0 Master Tx PDMA channel configuration                                                              */
  PDMACH_InitStructure_TX.PDMACH_SrcAddr = (u32) &_tx_buffer;
  PDMACH_InitStructure_TX.PDMACH_DstAddr = (u32) &SPI0_PORT->DR;
  PDMACH_InitStructure_TX.PDMACH_BlkLen = 1;
  PDMACH_InitStructure_TX.PDMACH_BlkCnt = size;
  PDMACH_InitStructure_TX.PDMACH_DataSize = WIDTH_8BIT;
  PDMACH_InitStructure_TX.PDMACH_Priority = M_PRIO;
  PDMACH_InitStructure_TX.PDMACH_AdrMod = SRC_ADR_FIX | DST_ADR_FIX;

  /* SPI0 Master Rx PDMA channel configuration                                                              */
  PDMACH_InitStructure_RX.PDMACH_SrcAddr = (u32) &SPI0_PORT->DR;
  PDMACH_InitStructure_RX.PDMACH_DstAddr = (u32) rx_buffer;
  PDMACH_InitStructure_RX.PDMACH_BlkLen = 1;
  PDMACH_InitStructure_RX.PDMACH_BlkCnt = size;
  PDMACH_InitStructure_RX.PDMACH_DataSize = WIDTH_8BIT;
  PDMACH_InitStructure_RX.PDMACH_Priority = VH_PRIO;
  PDMACH_InitStructure_RX.PDMACH_AdrMod = SRC_ADR_FIX | DST_ADR_LIN_INC;
  /* PDMA configuration, SPI Tx                                                                           */
  PDMA_Config(uSPITxDmaChNumber, &PDMACH_InitStructure_TX);

  /* PDMA configuration, SPI Rx                                                                           */
  PDMA_Config(uSPIRxDmaChNumber, &PDMACH_InitStructure_RX);

  PDMA_EnaCmd(uSPITxDmaChNumber, ENABLE);
  PDMA_EnaCmd(uSPIRxDmaChNumber, ENABLE);

    SPI_PDMACmd(SPI0_PORT, SPI_PDMAREQ_TX | SPI_PDMAREQ_RX, ENABLE);
  /* Start Transmission data                                                                              */
  return;
}

/**********************************************************
Description: Wait for data transfer to complete
Input:
Output:
Return:
Others:
**********************************************************/
void SDReadRawData::waitDmaFinish()
{
  HT_PDMACH_TypeDef *SPITxDmaCh;
  HT_PDMACH_TypeDef *SPIRxDmaCh;
  u32 uSPITxDmaChNumber;
  u32 uSPIRxDmaChNumber;

  uSPIRxDmaChNumber = SPI0_RX_PDMA_CH;
  uSPITxDmaChNumber = SPI0_TX_PDMA_CH;
  SPIRxDmaCh = (HT_PDMACH_TypeDef *)(HT_PDMA_BASE + uSPIRxDmaChNumber * 6 * 4);
  SPITxDmaCh = (HT_PDMACH_TypeDef *)(HT_PDMA_BASE + uSPITxDmaChNumber * 6 * 4);
  while ((SPITxDmaCh->CTSR&0xFFFF0000) != 0);
  while ((SPIRxDmaCh->CTSR&0xFFFF0000) != 0);
  SPI_PDMACmd(SPI0_PORT, SPI_PDMAREQ_TX | SPI_PDMAREQ_RX, DISABLE);
  SDCARD_SPI.endTransaction();
  return;
}

/**********************************************************
Description: Read block data
Input:       block : Block index
             count : data length
             dst : destination 
Output:
Return:
Others:
**********************************************************/
uint8_t SDReadRawData::readBlock(uint32_t block, uint16_t count, uint8_t* dst)
{
  if (cardCommand(CMD17, block))
  {
    error(SD_CARD_ERROR_CMD17);
    goto fail;
  }
  if (!waitStartBlock())
  {
    goto fail;
  }

  spiDmaRead(dst, count);

  return true;

fail:
  chipSelectHigh();
  return false;
}

/**********************************************************
Description: Wait until SD is not busy
Input:       timeoutMillis : waiting time
Output:
Return:
Others:
**********************************************************/
uint8_t SDReadRawData::waitNotBusy(unsigned int timeoutMillis)
{
  unsigned int t0 = millis();
  unsigned int d;
  do
  {
    if (SDCARD_SPI.transfer(0xFF) == 0XFF)
    {
      return true;
    }
    d = millis() - t0;
  } while (d < timeoutMillis);
  return false;
}

/**********************************************************
Description: Waitting for SD start block
Input:
Output:
Return:
Others:
**********************************************************/
uint8_t SDReadRawData::waitStartBlock(void)
{
  unsigned int t0 = millis();
  while ((status_ = SDCARD_SPI.transfer(0xFF)) == 0XFF)
  {
    unsigned int d = millis() - t0;
    if (d > SD_READ_TIMEOUT)
    {
      error(SD_CARD_ERROR_READ_TIMEOUT);
      goto fail;
    }
  }
  if (status_ != DATA_START_BLOCK)
  {
    error(SD_CARD_ERROR_READ);
    goto fail;
  }
  return true;

fail:
  chipSelectHigh();
  return false;
}

/**********************************************************
Description: Writet data
Input:       token : src
             src : source
Output:
Return:
Others:
**********************************************************/
uint8_t SDReadRawData::writeData(uint8_t token, const uint8_t* src)
{
  SDCARD_SPI.transfer(token);
  for (uint16_t i = 0; i < 512; i++)
  {
    SDCARD_SPI.transfer(src[i]);
  }
  SDCARD_SPI.transfer(0xff);  // dummy crc
  SDCARD_SPI.transfer(0xff);  // dummy crc

  status_ = SDCARD_SPI.transfer(0xFF);
  if ((status_ & DATA_RES_MASK) != DATA_RES_ACCEPTED)
  {
    error(SD_CARD_ERROR_WRITE);
    chipSelectHigh();
    return false;
  }
  return true;
}
#endif
