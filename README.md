<div align=center>
<img src="https://github.com/BestModules-Libraries/img/blob/main/BMD58T280_V1.0.png" width="320" height="240"> 
</div> 


BMD58T280 
===========================================================

The Best Modules BMD58T280 is a shield board for 2.8” TFT-LCD display function, which uses the SPI and EBI communication methods. This document provides the description of the BMD58T280 Arduino Lib functions and how to install the Arduino Lib. The example demonstrates the function of TFT display.

This library can be installed via the Arduino Library manager. Search for **BMD58T280**. 

Repository Contents
-------------------

* **/examples** - Example sketches for the library (.ino). Run these from the Arduino IDE. 
* **/src** - Source files for the library (.cpp, .h).
* **keywords.txt** - Keywords from this library that will be highlighted in the Arduino IDE. 
* **library.properties** - General library properties for the Arduino package manager. 

Documentation 
-------------------

* **[BMD58T280 Arduino Library Description]( https://www.bestmodulescorp.com/bmd58t280.html#tab-product2 )** - Arduino Library Description.

Version History  
-------------------

* **V1.0.1**  
&emsp;&emsp;- Initial public release.
* **V1.0.2**  
&emsp;&emsp;- Change the function setRotation to adjust the LCD coordinates to match the absolute position
* **V1.0.3**  
&emsp;&emsp;- Add function "void text(const char * text, int16_t x, int16_t y, uint16_t textcolor, uint16_t textbgcolor)" and "void textWrap(const char * text, int16_t x, int16_t y, uint16_t textcolor, uint16_t textbgcolor)"：You can directly set the font color and background color(These two colors are not related to the settings of the "stroke" and "background" functions).  
&emsp;&emsp;- Change example "TFTDisplayText": Use newly added functions for demonstration to avoid flickering issues.
* **V1.0.4**  
&emsp;&emsp;- Modify the content of the "TFD_SPI. h" document to solve the problem of LCD backlight not fully turned on.

License Information
-------------------

This product is _**open source**_! 

Please review the LICENSE.md file for license information. 

If you have any questions or concerns on licensing, please contact technical support on our BEST MODULES CORP. Email:service@bestmodulescorp.com

Distributed as-is; no warranty is given.

BEST MODULES CORP.
