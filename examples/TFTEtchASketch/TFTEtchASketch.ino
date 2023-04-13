/*****************************************************************
File:         TFTColorPicker.ino
Description:  1. This example draws a yellow circle (x, y, z) based 
                 on the touch screen.
              2. This example is based on the arduino example TFTEtchASketch. 
                 For details, please refer to the following website
                 http://www.arduino.cc/en/Tutorial/TFTEtchASketch
******************************************************************/
// include the necessary libraries
#include <BMD58T280.h>  // Arduino LCD library
#include <SPI.h>            // SPI library

// Choose communication interface
#if !defined(ARDUINO_AVR_UNO) // For BMduino
BMD58T280 TFTscreen;          // Create LCD objects. Please comment out this line of code if you don't use EBI mode
//BMD58T280 TFTscreen(&SPI);    // Create LCD objects. Please comment out this line of code if you don't use SPI mode
#else // For UNO
BMD58T280 TFTscreen(&SPI);    // Create LCD objects
#endif

BM_XPT2046 touchScreen; // Create touchScreen objects.

void setup() {

  // initialize the screen
  TFTscreen.begin();
  touchScreen.begin();
  Serial.begin(9600);
  // make the background black
  TFTscreen.background(0, 0, 0);
  // Set fill with yellow
  TFTscreen.fill(BM_ILI9341::YELLOW);
  Serial.println("Now you can draw on the screen");
}

void loop() {
    if (touchScreen.touched()) 
    {
      // screen is touched .Read the touch screen's (x, y, z)
      BM_XPT2046::TS_Point p = touchScreen.getPoint();
      p.x = map(p.x, 400, 3600, 0, TFTscreen.width());
      p.y = map(p.y, 400, 3600, 0, TFTscreen.height());
      p.z = map(p.z, 400, 2800, 0, 20);
      // draw a circle
      TFTscreen.circle(p.x, p.y, p.z);
    }
}
