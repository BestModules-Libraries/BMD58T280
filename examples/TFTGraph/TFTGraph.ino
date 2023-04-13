/*****************************************************************
File:         TFTColorPicker.ino
Description:  1.  This example f reads the value of the touch screen's
                  x-axis, and graphs the values on the screen.
              2. This example is based on the arduino example TFTGraph. 
                 For details, please refer to the following website
                 http://www.arduino.cc/en/Tutorial/TFTGraph
******************************************************************/
// include the necessary libraries
#include <BMD58T280.h>
#include <SPI.h>            // SPI library

// Choose communication interface
#if !defined(ARDUINO_AVR_UNO) // For BMduino
BMD58T280 TFTscreen;          // Create LCD objects. Please comment out this line of code if you don't use EBI mode
//BMD58T280 TFTscreen(&SPI);    // Create LCD objects. Please comment out this line of code if you don't use SPI mode
#else // For UNO
BMD58T280 TFTscreen(&SPI);    // Create LCD objects
#endif

BM_XPT2046 touchScreen; // Create touchScreen objects.

// position of the line on screen
int xPos = 0;

void setup() {
  // initialize the serial port
  Serial.begin(9600);

  // initialize the display
  TFTscreen.begin();
  touchScreen.begin();
  TFTscreen.setRotation(1);
  // clear the screen with a pretty color
  TFTscreen.background(250, 16, 200);
}

void loop() {
  // read the sensor and map it to the screen height
  
  if (touchScreen.touched()) 
  {  
    // screen is touched .Read the touch screen's x-axis
    BM_XPT2046::TS_Point p = touchScreen.getPoint();
     int drawHeight = map(p.x, 400, 3600, 0, TFTscreen.height());

    // print out the height to the serial monitor
    Serial.println(drawHeight);
  
    // draw a line in a nice color
    TFTscreen.stroke(250, 180, 10);
    TFTscreen.line(xPos, TFTscreen.height() - drawHeight, xPos, TFTscreen.height());
  
    // if the graph has reached the screen edge
    // erase the screen and start again
    if (xPos >= 360) {
      xPos = 0;
      TFTscreen.background(250, 16, 200);
    } else {
      // increment the horizontal position:
      xPos++;
      delay(16);
    }
  }
}
