/*****************************************************************
File:        Low level interface
Author:      BESTMODULES
Description: TFT_LowLevelInf header file
History:     V1.0.2   -- 2022-11-20
******************************************************************/

/* Define to prevent recursive inclusion -------------------------------------------------------------------*/
#ifndef __TFT_LOWLEVELINF_H
#define __TFT_LOWLEVELINF_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------------------------------------*/
class TFT_LowLevelInf{
  public:
    virtual void init(__attribute__ ((unused))uint32_t freq, __attribute__ ((unused))uint8_t spiMode)=0;
    virtual void startWrite(void){};
    virtual void endWrite(void){};
    virtual void writeCmd8(uint8_t comm8)=0;
    virtual void writeData8(uint8_t data)=0;
    virtual void writeData16(uint16_t data)=0;
    virtual uint8_t readData8()=0;
};

#ifdef __cplusplus
}
#endif

#endif
