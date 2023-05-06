/*****************************************************************
File:         TFTLogoVideo.ino
Description:  1. Read the raw data in SD and display these data on TFT-LCD
              2. Please download win32-disk-imager, and use this tool to write Video.img to SD card
                 (please note that this behavior will destroy the data in the original SD card).
              win32-disk-imager URL: https://win32diskimager.download/download-win32-disk-imager/
                 There is a .bmp file included with this sketch.
                 - open the sketch folder 
                 - open the win32-disk-imager, and write Video.img to SD card.
                 - put the SD into the SD slot of the TFT module.
              3. This example does not support UNO and only EBI mode is supported.
******************************************************************/
// include the necessary libraries
#include <BMD58T280.h>        // Arduino LCD library
#include <SD.h>               // Arduino SD library
#include <SDReadRawData.h>    // BMduino SD library

SDReadRawData sdcard;      // Create SD objects.
BMD58T280 TFTscreen;       // Create LCD objects.

// pin definition for the SD
#define sd_cs  10

#define IMAGE_HEIGHT       160
#define IMAGE_WIDTH        192
#if defined(ARDUINO_AVR_UNO)
  #ERROR "This example don't support UNO. Please use the BMduino development board instead."
#endif

void setup() 
{
  // initialize the serial port: it will be used to
  // print some diagnostic info
  Serial.begin(9600);
  // try to access the SD card. If that fails (e.g.
  // no card present), the setup process will stop.
    if (!sdcard.init(sd_cs)) {
    Serial.println(F("failed!"));
    return;
  }
  Serial.println(F("succeeded!"));
  // initialize the LCD and show a message
  TFTscreen.begin();
}

void loop()
{
  // Display first Video(background blue)
  // clear the GLCD screen before starting
  TFTscreen.background(0, 216, 255);  //background blue
  // Display 50 images on LCD.
  for(int i =0;i<50;i++)
  {
    // Display image, start coordinates (40,69), image size 160*192 pixels.
    displayImage(40,69,IMAGE_HEIGHT,IMAGE_WIDTH,IMAGE_HEIGHT*IMAGE_WIDTH*2*i);
  }
  // Display second Video(background gray)
  // clear the GLCD screen before starting
  TFTscreen.background(135, 144, 146);  //background gray
  // Display 25 images on LCD.
  for(int i =50;i<75;i++)
  {
    // Display image, start coordinates (40,69), image size 160*192 pixels.
    displayImage(40,69,IMAGE_HEIGHT,IMAGE_WIDTH,IMAGE_HEIGHT*IMAGE_WIDTH*2*i);
  }
}

#define SD_BLOCK_BYTE_SIZE             512
#define SD_BLOCK_PIXEL_SIZE            (SD_BLOCK_BYTE_SIZE / 2)
#define SD_BLOCK_BYTE_SIZE_WITH_CRC    (SD_BLOCK_BYTE_SIZE + 2)
#define HALF_ADDRESS_INDEX             516

// Image buffer
u8 dataBuffer[1032];
// Image buffer start address
u8 *pStartDatabuffer = &(dataBuffer[0]);
// Image buffer half address
u8 *pHalfDatabuffer = &(dataBuffer[HALF_ADDRESS_INDEX]);

/**********************************************************
Description: In order to speed up the reading of SD card data, a
             ping-pong buffering process of reading SD card is designed.
             So this function process is a little bit complicated.
Input:       x : The horizontal position where the imamge 
             y : The vertical position where the imamge
             w : The width of the imamge
             h : The height of the imamge
             SDOffset : the location where the SD is stored.
Output:
Return:
Others:
**********************************************************/
void displayImage(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint32_t SDOffset)
{
  // Get the block index of SD.
  SDOffset = SDOffset/SD_BLOCK_BYTE_SIZE;
  // Set the starting position, width and length of the image.
  TFTscreen.setAddrWindow(x, y, w, h);
  
  /* 
    In order to speed up, read the 514 bytes in the SD card first.
    Read the data of a SD Block, 512 (Data) + 2 (CRC) = 514 bytes.
    Store the data at the start of the buffer.
  */
  sdcard.readBlock(SDOffset, SD_BLOCK_BYTE_SIZE_WITH_CRC, pStartDatabuffer);
   /* Waiting for reading 514 bytes finish.
      (The data at the start of the buffer) 
   */
  sdcard.waitDmaFinish();
  for(int i=1;i<(w*h/SD_BLOCK_PIXEL_SIZE);i++)
  {
    /* 
      Read the data of a SD Block, 512 (Data) + 2 (CRC) = 514 bytes.
      Store the data at the half of the buffer.
    */
    sdcard.readBlock(SDOffset + i, SD_BLOCK_BYTE_SIZE_WITH_CRC, pHalfDatabuffer);

    /* 
       Display 512 bytes data on the LCD , and skip 2 bytes CRC.
       (The data at the start of the buffer)
    */
    for(int j=0;j<SD_BLOCK_BYTE_SIZE;j+=2)
    {
      //Convert raw data to color
      u16 _color = (u16)((dataBuffer[j+1]<<8) | dataBuffer[j]);
      //In order to speed up the writing of data through the low level EBI
      TFTscreen.lowLevel->writeData16(_color);
    }
    /* 
      Waiting for reading 514 bytes finish.
      (The data at the half of the buffer)
    */
    sdcard.waitDmaFinish();
    i++;
    // Confirm whether the image data is completely read from SD.
    if(i < (w*h/SD_BLOCK_PIXEL_SIZE))
    {
      /* 
        Read the data of a SD Block, 512 (Data) + 2 (CRC) = 514 bytes.
        Store the data at the start of the buffer.
        (The data at the start of the buffer)
      */
      sdcard.readBlock(SDOffset + i, SD_BLOCK_BYTE_SIZE_WITH_CRC, pStartDatabuffer);
    }
    /* 
      Display 512 bytes data on the LCD , and skip 2 bytes CRC
     (The data at the half of the buffer)
    */
    for(int j=0;j<SD_BLOCK_BYTE_SIZE;j+=2)
    {
      u16 _color = (u16)((dataBuffer[j+HALF_ADDRESS_INDEX+1]<<8) | dataBuffer[j+HALF_ADDRESS_INDEX]);
      TFTscreen.lowLevel->writeData16(_color);
    }
    // Waiting for reading 514 bytes finish.
   sdcard.waitDmaFinish();
  }
}
