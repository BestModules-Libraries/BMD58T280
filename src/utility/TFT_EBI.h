#ifndef __TFT_EBI_H
#define __TFT_EBI_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------------------------------------*/
#include "ht32.h"
#include "TFT_LowLevelInf.h"


/* Settings ------------------------------------------------------------------------------------------------*/
#define LCD_EBI                     (HT_EBI)

#define LCD_EBI_CS_GPIO_ID          (GPIO_PC)
#define LCD_EBI_CS_AFIO_PIN         (AFIO_PIN_2)
#define LCD_EBI_CS_AFIO_MODE        (AFIO_FUN_EBI)

#define LCD_EBI_OE_GPIO_ID          (GPIO_PC)
#define LCD_EBI_OE_AFIO_PIN         (AFIO_PIN_1)
#define LCD_EBI_OE_AFIO_MODE        (AFIO_FUN_EBI)

#define LCD_EBI_WE_GPIO_ID          (GPIO_PC)
#define LCD_EBI_WE_AFIO_PIN         (AFIO_PIN_3)
#define LCD_EBI_WE_AFIO_MODE        (AFIO_FUN_EBI)

#define LCD_EBI_RS_GPIO_ID          (GPIO_PD)
#define LCD_EBI_RS_AFIO_PIN         (AFIO_PIN_4)
#define LCD_EBI_RS_AFIO_MODE        (AFIO_FUN_EBI)
#define LCD_EBI_RS_ADDR_PIN         (2)

#define LCD_EBI_AD0_GPIO_ID         (GPIO_PA)
#define LCD_EBI_AD0_AFIO_PIN        (AFIO_PIN_14)
#define LCD_EBI_AD0_AFIO_MODE       (AFIO_FUN_EBI)

#define LCD_EBI_AD1_GPIO_ID         (GPIO_PA)
#define LCD_EBI_AD1_AFIO_PIN        (AFIO_PIN_15)
#define LCD_EBI_AD1_AFIO_MODE       (AFIO_FUN_EBI)

#define LCD_EBI_AD2_GPIO_ID         (GPIO_PB)
#define LCD_EBI_AD2_AFIO_PIN        (AFIO_PIN_0)
#define LCD_EBI_AD2_AFIO_MODE       (AFIO_FUN_EBI)

#define LCD_EBI_AD3_GPIO_ID         (GPIO_PB)
#define LCD_EBI_AD3_AFIO_PIN        (AFIO_PIN_1)
#define LCD_EBI_AD3_AFIO_MODE       (AFIO_FUN_EBI)

#define LCD_EBI_AD4_GPIO_ID         (GPIO_PB)
#define LCD_EBI_AD4_AFIO_PIN        (AFIO_PIN_2)
#define LCD_EBI_AD4_AFIO_MODE       (AFIO_FUN_EBI)

#define LCD_EBI_AD5_GPIO_ID         (GPIO_PB)
#define LCD_EBI_AD5_AFIO_PIN        (AFIO_PIN_3)
#define LCD_EBI_AD5_AFIO_MODE       (AFIO_FUN_EBI)

#define LCD_EBI_AD6_GPIO_ID         (GPIO_PB)
#define LCD_EBI_AD6_AFIO_PIN        (AFIO_PIN_4)
#define LCD_EBI_AD6_AFIO_MODE       (AFIO_FUN_EBI)

#define LCD_EBI_AD7_GPIO_ID         (GPIO_PB)
#define LCD_EBI_AD7_AFIO_PIN        (AFIO_PIN_5)
#define LCD_EBI_AD7_AFIO_MODE       (AFIO_FUN_EBI)

#define LCD_Bank_ADDR             0x60000000
#define EBI_LCD                  ((EBI_LCD_TypeDef *) LCD_Bank_ADDR)

class TFT_EBI: public TFT_LowLevelInf{
  typedef struct
  {
    vu8 EBI_LCD_REG;
    #if (LCD_EBI_RS_ADDR_PIN)
    vu8 _RESERVED[(0x1ul << LCD_EBI_RS_ADDR_PIN) - 1];
    #endif
    vu8 EBI_LCD_RAM;
  } EBI_LCD_TypeDef;
  public:
    TFT_EBI(){};
    void init(__attribute__ ((unused))uint32_t freq, __attribute__ ((unused))uint8_t spiMode)
    {
        /* !!! NOTICE !!!
       Notice that the local variable (structure) did not have an initial value.
       Please confirm that there are no missing members in the parameter settings below this function.
      */
      EBI_InitTypeDef  EBI_InitStructure;
      /*  Enable EBI clock                                                                        */
      HT_CKCU->AHBCCR = HT_CKCU->AHBCCR | 0x1000;
      /* Configure EBI pins & LCD RESET pin                                                                     */
      AFIO_GPxConfig(LCD_EBI_CS_GPIO_ID, LCD_EBI_CS_AFIO_PIN, LCD_EBI_CS_AFIO_MODE);
      AFIO_GPxConfig(LCD_EBI_OE_GPIO_ID, LCD_EBI_OE_AFIO_PIN, LCD_EBI_OE_AFIO_MODE);
      AFIO_GPxConfig(LCD_EBI_WE_GPIO_ID, LCD_EBI_WE_AFIO_PIN, LCD_EBI_WE_AFIO_MODE);

      AFIO_GPxConfig(LCD_EBI_RS_GPIO_ID, LCD_EBI_RS_AFIO_PIN, LCD_EBI_RS_AFIO_MODE);

      AFIO_GPxConfig(LCD_EBI_AD0_GPIO_ID, LCD_EBI_AD0_AFIO_PIN, LCD_EBI_AD0_AFIO_MODE);
      AFIO_GPxConfig(LCD_EBI_AD1_GPIO_ID, LCD_EBI_AD1_AFIO_PIN, LCD_EBI_AD1_AFIO_MODE);
      AFIO_GPxConfig(LCD_EBI_AD2_GPIO_ID, LCD_EBI_AD2_AFIO_PIN, LCD_EBI_AD2_AFIO_MODE);
      AFIO_GPxConfig(LCD_EBI_AD3_GPIO_ID, LCD_EBI_AD3_AFIO_PIN, LCD_EBI_AD3_AFIO_MODE);
      AFIO_GPxConfig(LCD_EBI_AD4_GPIO_ID, LCD_EBI_AD4_AFIO_PIN, LCD_EBI_AD4_AFIO_MODE);
      AFIO_GPxConfig(LCD_EBI_AD5_GPIO_ID, LCD_EBI_AD5_AFIO_PIN, LCD_EBI_AD5_AFIO_MODE);
      AFIO_GPxConfig(LCD_EBI_AD6_GPIO_ID, LCD_EBI_AD6_AFIO_PIN, LCD_EBI_AD6_AFIO_MODE);
      AFIO_GPxConfig(LCD_EBI_AD7_GPIO_ID, LCD_EBI_AD7_AFIO_PIN, LCD_EBI_AD7_AFIO_MODE);

      /*  EBI Configuration                                                                                     */
      EBI_InitStructure.EBI_Bank = EBI_BANK_0;

      EBI_InitStructure.EBI_Mode = EBI_MODE_D8A8;

      #if (LIBCFG_EBI_BYTELAND_ASYNCREADY == 1)
      EBI_InitStructure.EBI_ByteLane = EBI_BYTELANE_DISABLE;
      EBI_InitStructure.EBI_AsynchronousReady = EBI_ASYNCHRONOUSREADY_DISABLE;
      EBI_InitStructure.EBI_ARDYTimeOut = EBI_ARDYTIMEOUT_DISABLE;
      EBI_InitStructure.EBI_ByteLanePolarity = EBI_BYTELANEPOLARITY_LOW;
      EBI_InitStructure.EBI_ReadySignalPolarity = EBI_READYSIGNALPOLARITY_LOW;
      #endif
      EBI_InitStructure.EBI_IdleCycle = EBI_IDLECYCLE_DISABLE;
      EBI_InitStructure.EBI_ChipSelectPolarity = EBI_CHIPSELECTPOLARITY_LOW;
      EBI_InitStructure.EBI_AddressLatchPolarity = EBI_ADDRESSLATCHPOLARITY_LOW;
      EBI_InitStructure.EBI_WriteEnablePolarity = EBI_WRITEENABLEPOLARITY_LOW;
      EBI_InitStructure.EBI_ReadEnablePolarity = EBI_READENABLEPOLARITY_LOW;
      EBI_InitStructure.EBI_IdleCycleTime = 0;
      EBI_InitStructure.EBI_AddressSetupTime = 0;
      EBI_InitStructure.EBI_AddressHoldTime = 0;
      EBI_InitStructure.EBI_WriteSetupTime = 1;
      EBI_InitStructure.EBI_WriteStrobeTime = 1;
      EBI_InitStructure.EBI_WriteHoldTime = 1;
      EBI_InitStructure.EBI_ReadSetupTime = 2;
      EBI_InitStructure.EBI_ReadStrobeTime = 5;
      EBI_InitStructure.EBI_ReadHoldTime = 1;
      EBI_Init(&EBI_InitStructure);

      EBI_Cmd(EBI_BANK_0, ENABLE);
    }
    void writeCmd8(uint8_t comm8)
    {
      EBI_LCD->EBI_LCD_REG = comm8;
    }
    uint8_t readData8()
    {
      return (*(vu8*)&EBI_LCD->EBI_LCD_RAM);
    }
    void writeData16(uint16_t data)
    {
      EBI_LCD->EBI_LCD_RAM = (data>>8)&0xFF;
      EBI_LCD->EBI_LCD_RAM = data&0xFF;
      return;
    }
    void writeData8(uint8_t data)
    {
      EBI_LCD->EBI_LCD_RAM = data;
      return;
    }
};
#ifdef __cplusplus
}
#endif

#endif
