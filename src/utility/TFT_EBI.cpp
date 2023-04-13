/*****************************************************************
File:        TFT_EBI.cpp
Author:      BESTMODULES
Description: TFT-LCD implementation using EBI
History:
V1.0.1   -- initial version£»2022-07-20£»Arduino IDE : v1.8.16
******************************************************************/

/* Includes ------------------------------------------------------------------------------------------------*/
#if !defined(ARDUINO_AVR_UNO)
#include "TFT_EBI.h"
  #if LIBCFG_EBI
    #define LCD_EBI_RST_UNUSE  1
    #if __CORTEX_M == 0
      #include "ht32f5xxxx_ebi.c"
    #endif
    #if __CORTEX_M == 3
      #include "ht32f1xxxx_ebi.c"
    #endif
  #endif
#endif