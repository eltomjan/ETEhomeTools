#include <stdint.h>
#include "Print.h"
#include "Arduino.h"
/******************************************************************
 This is the core graphics library for all our displays, providing
 basic graphics primitives (points, lines, circles, etc.). It needs
 to be paired with a hardware-specific library for each display
 device we carry (handling the lower-level functions).
 
 Adafruit invests time and resources providing this open
 source code, please support Adafruit and open-source hardware
 by purchasing products from Adafruit!
 
 Written by Limor Fried/Ladyada for Adafruit Industries.
 Processing-like API written by Enrico Gueli for Officine Arduino.
 BSD license, check license.txt for more information.
 All text above must be included in any redistribution.
 ******************************************************************/

#ifndef _ADAFRUIT_GFX_H
#define _ADAFRUIT_GFX_H

#if ARDUINO >= 100
 #include "Arduino.h"
 #include "Print.h"
#else
 #include "WProgram.h"
#endif

/*
 * This library can work with or without the presence of an SD
 * reading library (to load images). At the moment, only the
 * Arduino SD library is supported; it is included in
 * standard Arduino libraries.
 * 
 * The presence of the SD library is detected by looking at the
 * __SD_H__ preprocessor variable, defined into 
 * Arduino SD library to avoid double inclusion. This means
 * that in order to use the image-related API of Adafruit_GFX,
 * SD.h *must* be included before Adafruit_GFX.
 * 
 * The bottom part of this include file contains the actual image
 * loading code; if it was in a separate .cpp file, there were no
 * way to check if the SD library was present or not.
 * 
 * A partial solution was to include SD.h anyway, see if that works
 * (i.e. it is found in the include search path) and act accordingly.
 * But this solution relied on the preprocessor to issue only a
 * warning when an include file is not found. Avr-gcc, used for
 * Arduino 8-bit MCUs, does that, but the standard gcc-4.4, used for
 * Arduino Due, issues a fatal error and stops compilation.
 * 
 * The best solution so far is to put the code here. It works if this
 * include is used only in one .cpp file in the build (this is the
 * case of most Arduino sketches); if used in multiple .cpp files,
 * the linker may complain about duplicate definitions.
 * 
 */

#define swap(a, b) { int16_t t = a; a = b; b = t; }

typedef uint16_t color;

class Adafruit_GFX : public Print {
 public:

  const uint8_t *m_font;
  Adafruit_GFX() : WIDTH(0), HEIGHT(0), corner_x(0), corner_y(0)
  {};
  Adafruit_GFX(int16_t w, int16_t h); // Constructor
  // This MUST be defined by the subclass:
  virtual void drawPixel(int16_t x, int16_t y, uint16_t color) = 0;
  virtual void drawChar(int16_t x, int16_t y, unsigned char c, uint16_t color,
      uint16_t bg, uint8_t size) = 0;
  virtual void fillScreen(uint16_t color) = 0;
  virtual void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) = 0;
  virtual void drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color) = 0;
  virtual void drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color) = 0;

  // These MAY be overridden by the subclass to provide device-specific
  // optimized code.  Otherwise 'generic' versions are used.
  virtual void
    drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color),
    drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color),
    invertDisplay(boolean i);

  // These exist only with Adafruit_GFX (no subclass overrides)
  void
    drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1,
      int16_t x2, int16_t y2, uint16_t color),
    fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1,
      int16_t x2, int16_t y2, uint16_t color),
    setCursor(int16_t x, int16_t y),
    setTextColor(uint16_t c),
    setTextColor(uint16_t c, uint16_t bg),
    setTextSize(uint8_t s),
    setTextWrap(boolean w),
    setRotation(uint8_t r),
	setCorner(int16_t x, int16_t y);
    
  int16_t height(void) const;
  int16_t width(void) const;

  uint8_t getRotation(void) const;

  // get current cursor position (get rotation safe maximum values, using: width() for x, height() for y)
  int16_t getCursorX(void) const;
  int16_t getCursorY(void) const;
  int16_t getCornerX(void) const { return corner_x; };
  int16_t getCornerY(void) const { return corner_y; };

 protected:
  const int16_t
    WIDTH, HEIGHT;   // This is the 'raw' display w/h - never changes
  int16_t
    _width, _height, // Display w/h as modified by current rotation
    cursor_x, cursor_y,
	corner_x, corner_y;
  uint16_t
    textcolor, textbgcolor;
  uint8_t
    textsize,
    rotation;
  boolean
    wrap; // If set, 'wrap' text at right edge of display
};

#endif // _GFX_H
