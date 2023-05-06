/*****************************************************************
File:         TFTEtchASketchAddICON.ino
Description:  1. This example draws a yellow circle (x, y, z) based 
                 on the touch screen.
              2. Add three icons, respectively "Clear", "Yellow", "Blue".
              3. This example is based on the arduino example TFTEtchASketch. 
                 For details, please refer to the following website
                 http://www.arduino.cc/en/Tutorial/TFTEtchASketch
******************************************************************/
#include <BMD58T280.h>  // Arduino LCD library
#include "icon.h"       // A "clear" icon's raw data
#include <SPI.h>            // SPI library

// Choose communication interface
#if !defined(ARDUINO_AVR_UNO) // For BMduino
BMD58T280 TFTscreen;          // Create LCD objects. Please comment out this line of code if you don't use EBI mode
//BMD58T280 TFTscreen(&SPI);    // Create LCD objects. Please comment out this line of code if you don't use SPI mode
#else // For UNO
BMD58T280 TFTscreen(&SPI);    // Create LCD objects
#endif

BM_XPT2046 touchScreen; // Create touchScreen objects.

#define X_SATART         12   //Icon x-axis starting position
#define Y_SATART         195  //Icon y-axis starting position

#define ICON_W           35  //Icon width
#define ICON_H           36  //Icon high
#define ICON_OFFSET      10  //Distance per ICON


uint16_t color = (uint16_t)BM_ILI9341::YELLOW;
void setup() {

  // initialize the screen
  TFTscreen.begin();
  touchScreen.begin();
  Serial.begin(9600);
  // make the background black
  TFTscreen.background(0, 0, 0);
  // Set fill with yellow
  TFTscreen.fill(BM_ILI9341::YELLOW);
  // Draw a yellow icon.
  TFTscreen.rect(X_SATART,Y_SATART + ICON_W + ICON_OFFSET ,ICON_W,ICON_H);
  // Set fill with BLUE
  TFTscreen.fill(BM_ILI9341::BLUE);
  // Draw a yellow icon.
  TFTscreen.rect(X_SATART,Y_SATART + ICON_W + ICON_OFFSET + ICON_W + ICON_OFFSET,ICON_W,ICON_H);
  // Draw a "clear" icon.
  TFTscreen.drawPixels(X_SATART,Y_SATART,ICON_W,ICON_H, clearIcon);
  Serial.println("Now you can draw on the screen");
}

void loop() {
    uint16_t isTouchIcon;
    if (touchScreen.touched()) 
    {
      // screen is touched .Read the touch screen's (x, y, z)
      BM_XPT2046::TS_Point p = touchScreen.getPoint();
      p.x = map(p.x, 400, 3600, 0, TFTWIDTH);
      p.y = map(p.y, 400, 3600, 0, TFTHEIGHT);
      p.z = map(p.z, 400, 2800, 0, 20);
      isTouchIcon = 0;
      if(p.x>X_SATART & p.x<(ICON_H + X_SATART))
      {
        if((p.y>Y_SATART &&
            p.y <(Y_SATART +ICON_W) ))
        {
          //User touches the "clear" icon. The screen layout needs to be reset 
          isTouchIcon = 1;
          // make the background black
          TFTscreen.background(0, 0, 0);
          // Set fill with yellow
          TFTscreen.fill(BM_ILI9341::YELLOW);
          // Draw a yellow icon.
          TFTscreen.rect(X_SATART,Y_SATART + ICON_W + ICON_OFFSET ,ICON_W,ICON_H);
          // Set fill with BLUE
          TFTscreen.fill(BM_ILI9341::BLUE);
          // Draw a yellow icon.
          TFTscreen.rect(X_SATART,Y_SATART + ICON_W + ICON_OFFSET + ICON_W + ICON_OFFSET,ICON_W,ICON_H);
          // Draw a "clear" icon.
          TFTscreen.drawPixels(X_SATART,Y_SATART,ICON_W,ICON_H, clearIcon);
          // Set the color of the brush.
          TFTscreen.fill(color);
        } 
        else if((p.y>(Y_SATART + ICON_W + ICON_OFFSET) &&
                 p.y<(Y_SATART + ICON_W + ICON_OFFSET + ICON_W) ))
        {
          //User touches the "yellow" icon. Repaint the yellow icon.
          isTouchIcon = 1;
          // Set fill with yellow
          TFTscreen.fill(BM_ILI9341::YELLOW);
          // Draw a yellow icon.
          TFTscreen.rect(X_SATART,Y_SATART + ICON_W + ICON_OFFSET ,ICON_W,ICON_H);
          // Set the color of the brush.
          color = (uint16_t)BM_ILI9341::YELLOW;
          TFTscreen.fill(color);
        }
        else if((p.y>(Y_SATART + ICON_W + ICON_OFFSET + ICON_W + ICON_OFFSET) && 
                 p.y<(Y_SATART + ICON_W + ICON_OFFSET + ICON_W + ICON_OFFSET +ICON_W) ))
        {
          //User touches the "blue" icon. Repaint the blue icon.
          isTouchIcon = 1;
          // Set fill with blue
          TFTscreen.fill(BM_ILI9341::BLUE);
          // Draw a blue icon.
          TFTscreen.rect(X_SATART,Y_SATART + ICON_W + ICON_OFFSET + ICON_W + ICON_OFFSET,ICON_W,ICON_H);
          // Set the color of the brush.
          color = (uint16_t)BM_ILI9341::BLUE;
          TFTscreen.fill(color);
        }
      }

      if(isTouchIcon == 0)
      {
        // No icon is touched, start drawing circles
        TFTscreen.circle(p.x, p.y, p.z);
      }
    }
}
