/*****************************************************************
File:         TFTColorPicker.ino
Description:  1. This example demonstrates how to draw text on the
                 TFT. The BMD58T280 reads the touch screen's (x,y),
                 and writes the value to the LCD screen, updating 
                 every quarter second.
              2. This example is based on the arduino example TFTDisplayText. 
                 For details, please refer to the following website
                 http://www.arduino.cc/en/Tutorial/TFTDisplayText
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

// char array to print to the screen
char AxisPrintout[20];

void setup() {

  // Put this line at the beginning of every sketch that uses the GLCD:
  TFTscreen.begin();
  touchScreen.begin();
  Serial.begin(9600);
  // clear the screen with a black background
  TFTscreen.background(0, 0, 0);

  // write the static text to the screen
  // set the font color to white
  TFTscreen.stroke(255, 255, 255);
  // set the font size
  TFTscreen.setTextSize(2);
  // write the text to the top left corner of the screen
  TFTscreen.text("Please touch the\nscreen.", 0, 0);
  // ste the font size very large for the loop
  TFTscreen.setTextSize(5);
  Serial.println("Please touch the nscreen");
}

void loop() {
  if (touchScreen.touched()) 
  {
    // screen is touched .Read the touch screen's x and y
    BM_XPT2046::TS_Point p = touchScreen.getPoint();
    p.x = map(p.x, 400, 3600, 0, TFTscreen.width());   if(p.x<0) p.x = 0;
    p.y = map(p.y, 400, 3600, 0, TFTscreen.height());   if(p.y<0) p.y = 0;
    String strXaxis = String(p.x);
    String strYaxis = String(p.y);
    String strResult = strXaxis + "," + strYaxis;
    
    // convert the reading to a char array
    strResult.toCharArray(AxisPrintout, 20);
  
    // set the font color
    TFTscreen.stroke(255, 255, 255);
    // print the sensor value
    TFTscreen.text(AxisPrintout, 0, 40);

    // wait for a moment
    delay(250);

    // erase the text you just wrote
    TFTscreen.stroke(0, 0, 0);
    TFTscreen.text(AxisPrintout, 0, 40);
  }
}
