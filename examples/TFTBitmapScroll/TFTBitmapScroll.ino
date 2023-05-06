/*****************************************************************
File:         TFTBitmapScroll.ino
Description:  1. This example reads an image file from a micro-SD card and draws it on the screen,
                 and the image moves left and right.
              2. In this sketch, the Logo.bmp is read from a micro-SD card. 
                 There is a .bmp file included with this sketch.
                 - open the sketch folder 
                 - copy the "Logo.bmp" file to a micro-SD
                 - put the SD into the SD slot of the TFT module.
              3. This example is based on the arduino example TFTBitmap. 
                 For details, please refer to the following website
                 http://www.arduino.cc/en/Tutorial/TFTBitmapLogo
******************************************************************/
// include the necessary libraries
#include <SD.h>
#include <BMD58T280.h>      // Arduino LCD library
#include <SPI.h>            // SPI library

// Choose communication interface
#if !defined(ARDUINO_AVR_UNO) // For BMduino
BMD58T280 TFTscreen;          // Create LCD objects. Please comment out this line of code if you don't use EBI mode
//BMD58T280 TFTscreen(&SPI);    // Create LCD objects. Please comment out this line of code if you don't use SPI mode
#else // For UNO
BMD58T280 TFTscreen(&SPI);    // Create LCD objects
#endif

// pin definition for the SD
#define sd_cs  10

// this variable represents the image to be drawn on screen
BmpImage image1;

void setup() {
  // initialize the LCD and show a message
  TFTscreen.begin();
  TFTscreen.background(255, 255, 255);
  TFTscreen.textSize (1);
  TFTscreen.stroke(0, 0, 255);
  TFTscreen.println();
  TFTscreen.println(F("BMduino TFT Bitmap Example And Scroll"));
  TFTscreen.stroke(0, 0, 0);
  TFTscreen.println(F("Open serial monitor"));
  TFTscreen.println(F("to run the sketch"));

  delay(3000);
  // initialize the serial port: it will be used to
  // print some diagnostic info
  Serial.begin(9600);

  while (!Serial) {
    // wait for serial port to connect. Needed for native USB port only
  }

  // clear the GLCD screen before starting
  TFTscreen.background(0, 0xd8, 0xFF);

  // try to access the SD card. If that fails (e.g.
  // no card present), the setup process will stop.
  Serial.print(F("Initializing SD card..."));
  if (!SD.begin(sd_cs)) {
    Serial.println(F("failed!"));
    return;
  }
  Serial.println(F("OK!"));

  // now that the SD card can be access, try to load the
  // image file.
  image1.loadImage("Logo.bmp", &Serial);
  if (!image1) {
    Serial.println(F("error while loading Logo.bmp"));
  }
  Serial.println(F("drawing image"));
  // Draw the image to the screen, the image's position is the center of the x-axis
  TFTscreen.image(image1, (TFTWIDTH - image1.width())/2, 0);
}

void loop(){
    // The image moves left to right.
    for(int i= TFTscreen.height() ;i>image1.height();i--)
    {
      TFTscreen.scrollTo(i);
      delay(4);
    }
    // The image moves right to left.
    for(int i=image1.height();i<TFTscreen.height();i++)
    {
      TFTscreen.scrollTo(i);
      delay(4);
    }
}
