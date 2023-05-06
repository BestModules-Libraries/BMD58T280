/*****************************************************************
File:         TFTColorPicker.ino
Description:  1.  The BMD58T280 example reads the touch screen's (x,y,z)
                  and uses these values to change the screen's RGB.
              2. This example is based on the arduino example TFTColorPicker. 
                 For details, please refer to the following website
                 http://www.arduino.cc/en/Tutorial/TFTColorPicker
******************************************************************/
// include the necessary libraries
#include <BMD58T280.h>  // LCD library
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
  // begin serial communication
  Serial.begin(9600);

  // initialize
  TFTscreen.begin();
  touchScreen.begin();

  // set the background to white
  TFTscreen.background(255, 255, 255);
}

void loop() {
  if (touchScreen.touched()) 
  {
    // screen is touched .Read the touch screen's (x,y,z)
    BM_XPT2046::TS_Point p = touchScreen.getPoint();
    p.x = map(p.x, 400, 3600, 0, 255);   if(p.x<0) p.x = 0;
    p.y = map(p.y, 400, 3600, 0, 255);   if(p.y<0) p.y = 0;
    p.z = map(p.z, 400, 2800, 0, 255);

    // draw the background based on the mapped values
    TFTscreen.background(p.x , p.y, p.z);

    // send the values to the serial monitor
    Serial.print("background(");
    Serial.print(p.x);
    Serial.print(" , ");
    Serial.print(p.y);
    Serial.print(" , ");
    Serial.print(p.z);
    Serial.println(")");
  }
  // wait for a moment
  delay(20);

}
