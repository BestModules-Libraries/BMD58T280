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