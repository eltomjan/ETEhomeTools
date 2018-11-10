/***************************************************
  This is an Arduino Library for the Adafruit 2.2" SPI display.
  This library works with the Adafruit 2.2" TFT Breakout w/SD card
  ----> http://www.adafruit.com/products/1480
 
  Check out the links above for our tutorials and wiring diagrams
  These displays use SPI to communicate, 4 or 5 pins are required to
  interface (RST is optional)
  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  MIT license, all text above must be included in any redistribution
 ****************************************************/

#include "..\ArduinoWindow.h"
#include "Adafruit_ILI9340.h"
#ifdef __AVR
  #include <avr/pgmspace.h>
#elif defined(ESP8266)
  #undef USE_SPI_LIBRARY
  #include <avr/pgmspace.h>
#endif

#include <limits.h>
#include "pins_arduino.h"
//#include "wiring_private.h"
//#include <SPI.h>
using namespace System::Drawing;

#if defined(__SAM3X8E__)
#include <include/pio.h>
  #define SET_BIT(port, bitMask) (port)->PIO_SODR |= (bitMask)
  #define CLEAR_BIT(port, bitMask) (port)->PIO_CODR |= (bitMask)
  #define USE_SPI_LIBRARY
#endif
#ifdef __AVR__
  #define SET_BIT(port, bitMask) *(port) |= (bitMask)
  #define CLEAR_BIT(port, bitMask) *(port) &= ~(bitMask)
#endif
#if (defined(__arm__) && defined(CORE_TEENSY)) || defined(ESP8266)
//  #define USE_SPI_LIBRARY
  #define SET_BIT(port, bitMask) digitalWrite(*(port), HIGH);
  #define CLEAR_BIT(port, bitMask) digitalWrite(*(port), LOW);
#endif

// Constructor when using software SPI.  All output pins are configurable.
Adafruit_ILI9340::Adafruit_ILI9340(uint8_t cs, uint8_t dc, uint8_t mosi,
				   uint8_t sclk, uint8_t rst, uint8_t miso) : Adafruit_GFX(ILI9340_TFTWIDTH, ILI9340_TFTHEIGHT) {
  _cs   = cs;
  _dc   = dc;
  _mosi  = mosi;
  _miso = miso;
  _sclk = sclk;
  _rst  = rst;
  hwSPI = false;
}


// Constructor when using hardware SPI.  Faster, but must use SPI pins
// specific to each board type (e.g. 11,13 for Uno, 51,52 for Mega, etc.)
Adafruit_ILI9340::Adafruit_ILI9340(uint8_t cs, uint8_t dc, uint8_t rst) : Adafruit_GFX(ILI9340_TFTWIDTH, ILI9340_TFTHEIGHT) {
  _cs   = cs;
  _dc   = dc;
  _rst  = rst;
  hwSPI = true;
  _mosi  = _sclk = 0;
}

void Adafruit_ILI9340::spiwrite(uint8_t c) {}


void Adafruit_ILI9340::writecommand(uint8_t c) {}


void Adafruit_ILI9340::writedata(uint8_t c) {} 

// Rather than a bazillion writecommand() and writedata() calls, screen
// initialization commands and arguments are organized in these tables
// stored in PROGMEM.  The table may look bulky, but that's mostly the
// formatting -- storage-wise this is hundreds of bytes more compact
// than the equivalent code.  Companion function follows.
#define DELAY 0x80

// Companion code to the above tables.  Reads and issues
// a series of LCD commands stored in PROGMEM byte array.
void Adafruit_ILI9340::commandList(uint8_t *addr) {}


void Adafruit_ILI9340::begin(void) {}


void Adafruit_ILI9340::setAddrWindow(uint16_t x0, uint16_t y0, uint16_t x1,
 uint16_t y1) {}


void Adafruit_ILI9340::pushColor(uint16_t color) {}

void Adafruit_ILI9340::drawPixel(int16_t x, int16_t y, uint16_t color) {

  if((x < 0) ||(x >= _width) || (y < 0) || (y >= _height)) return;

  setAddrWindow(x,y,x+1,y+1);

	unsigned char r = (color >> (8+3));
	unsigned char g = ((color >> 5) & 0x3f);
	unsigned char b = (color & 0x1f);
	r=(r*8)+(r&7);b=(b*8)+(b&7);g=(g*4)+(g&3);

	ArduinoSimulator::ArduinoWindow^ frm = ArduinoSimulator::ArduinoWindow::m_me;
	static int maxY=0;
	if(y>maxY) {
		maxY=y;
	};
	frm->setPixel(x,y,r,g,b);
}


void Adafruit_ILI9340::drawFastVLine(int16_t x, int16_t y, int16_t h,
 uint16_t color) {

  // Rudimentary clipping
  if((x >= _width) || (y >= _height)) return;

  if((y+h-1) >= _height) 
    h = _height-y;

  for(int j=y;j<y+h-1;j++) drawPixel(x,j,color);
  return;
  setAddrWindow(x, y, x, y+h-1);

  uint8_t hi = color >> 8, lo = color;

  while (h--) {
    spiwrite(hi);
    spiwrite(lo);
  }
}


void Adafruit_ILI9340::drawFastHLine(int16_t x, int16_t y, int16_t w,
  uint16_t color) {

  // Rudimentary clipping
  if((x >= _width) || (y >= _height)) return;
  if((x+w-1) >= _width)  w = _width-x;
  setAddrWindow(x, y, x+w-1, y);

  for(int i=x;i<x+w-1;i++) drawPixel(i,y,color);
}

void Adafruit_ILI9340::fillScreen(uint16_t color) {
  fillRect(0, 0,  _width, _height, color);
}

// fill a rectangle
void Adafruit_ILI9340::fillRect(int16_t x, int16_t y, int16_t w, int16_t h,
  uint16_t color) {

  // rudimentary clipping (drawChar w/big text requires this)
  if((x >= _width) || (y >= _height)) return;
  if((x + w - 1) >= _width)  w = _width  - x;
  if((y + h - 1) >= _height) h = _height - y;

  setAddrWindow(x, y, x+w-1, y+h-1);

  uint8_t hi = color >> 8, lo = color;
  unsigned char r = (color >> (8+3));
  unsigned char g = ((color >> 5) & 0x3f);
  unsigned char b = (color & 0x1f);
  r=(r*8)+(r&7);b=(b*8)+(b&7);g=(g*4)+(g&3);

  for(int i=h; i>0; i--) {
    for(int j=w; j>0; j--) {
		ArduinoSimulator::ArduinoWindow^ frm = ArduinoSimulator::ArduinoWindow::m_me;
		frm->setPixel(x+j-1,y+i-1,r,g,b);
    }
  }
}


// Pass 8-bit (each) R,G,B, get back 16-bit packed color
uint16_t Adafruit_ILI9340::Color565(uint8_t r, uint8_t g, uint8_t b) {
  return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}


void Adafruit_ILI9340::setRotation(uint8_t m) {

  rotation = m % 4; // can't be higher than 3
  switch (rotation) {
   case 0:
     _width  = ILI9340_TFTWIDTH;
     _height = ILI9340_TFTHEIGHT;
     break;
   case 1:
     _width  = ILI9340_TFTHEIGHT;
     _height = ILI9340_TFTWIDTH;
     break;
  case 2:
     _width  = ILI9340_TFTWIDTH;
     _height = ILI9340_TFTHEIGHT;
    break;
   case 3:
     _width  = ILI9340_TFTHEIGHT;
     _height = ILI9340_TFTWIDTH;
     break;
  }
}


void Adafruit_ILI9340::invertDisplay(boolean i) {}


////////// stuff not actively being used, but kept for posterity


uint8_t Adafruit_ILI9340::spiread(void) {
  uint8_t r = 0;
  return r;
}

 uint8_t Adafruit_ILI9340::readdata(void) {
   uint8_t r = spiread();
   return r;
}
 

 uint8_t Adafruit_ILI9340::readcommand8(uint8_t c) {
   uint8_t r = spiread();
   return r;
}