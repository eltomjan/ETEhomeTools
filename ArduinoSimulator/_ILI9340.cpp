#include <string>
#include "ArduinoWindow.h"
#include "_ILI9340.h"
#include <avr/pgmspace.h>
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
#if defined(__arm__) && defined(CORE_TEENSY)
#define USE_SPI_LIBRARY
#define SET_BIT(port, bitMask) digitalWrite(*(port), HIGH);
#define CLEAR_BIT(port, bitMask) digitalWrite(*(port), LOW);
#endif

// Constructor when using software SPI. All output pins are configurable.
_ILI9340::_ILI9340(uint8_t cs, uint8_t dc, uint8_t mosi,
								   uint8_t sclk, uint8_t rst, uint8_t miso) : Adafruit_GFX(ILI9340_TFTWIDTH, ILI9340_TFTHEIGHT) {
	m_proportional = true;
}


// Constructor when using hardware SPI. Faster, but must use SPI pins
// specific to each board type (e.g. 11,13 for Uno, 51,52 for Mega, etc.)
_ILI9340::_ILI9340(uint8_t cs, uint8_t dc, uint8_t rst) : Adafruit_GFX(ILI9340_TFTWIDTH, ILI9340_TFTHEIGHT) {

	m_proportional = true;
}

void _ILI9340::spiwrite(uint8_t c) {
}


void _ILI9340::writecommand(uint8_t c) {
}


void _ILI9340::writedata(uint8_t c) {
}

// Rather than a bazillion writecommand() and writedata() calls, screen
// initialization commands and arguments are organized in these tables
// stored in PROGMEM. The table may look bulky, but that's mostly the
// formatting -- storage-wise this is hundreds of bytes more compact
// than the equivalent code. Companion function follows.
#define DELAY 0x80

// Companion code to the above tables. Reads and issues
// a series of LCD commands stored in PROGMEM byte array.
void _ILI9340::commandList(uint8_t *addr) {
}


void _ILI9340::begin(void) {
}


void _ILI9340::setAddrWindow(uint16_t x0, uint16_t y0, uint16_t x1,
 uint16_t y1) {
	 int X0 = x0, Y0 = y0,
		 X1 = x1, Y1 = y1;

  writecommand(ILI9340_CASET); // Column addr set
  writedata(X0 >> 8);
  writedata(X0 & 0xFF); // XSTART
  writedata(X1 >> 8);
  writedata(X1 & 0xFF); // XEND

  writecommand(ILI9340_PASET); // Row addr set
  writedata(Y0>>8);
  writedata(Y0); // YSTART
  writedata(Y1>>8);
  writedata(Y1); // YEND

  writecommand(ILI9340_RAMWR); // write to RAM
}


void _ILI9340::pushColor(uint16_t color) {
  spiwrite(color >> 8);
  spiwrite(color);
}

void _ILI9340::drawPixel(int16_t x, int16_t y, uint16_t color) {

  if((x < 0) ||(x >= _width) || (y < 0) || (y >= _height)) return;

  setAddrWindow(x,y,x+1,y+1);

  spiwrite(color >> 8);
  spiwrite(color);
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


void _ILI9340::drawFastVLine(int16_t x, int16_t y, int16_t h,
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


void _ILI9340::drawFastHLine(int16_t x, int16_t y, int16_t w,
  uint16_t color) {

  // Rudimentary clipping
  if((x >= _width) || (y >= _height)) return;
  if((x+w-1) >= _width) w = _width-x;
  setAddrWindow(x, y, x+w-1, y);

  for(int i=x;i<x+w-1;i++) drawPixel(i,y,color);
  return;

  uint8_t hi = color >> 8, lo = color;
  while (w--) {
    spiwrite(hi);
    spiwrite(lo);
  }
}

void _ILI9340::fillScreen(uint16_t color) {
  fillRect(0, 0, _width-1, _height-1, color);
}

// fill a rectangle
void _ILI9340::fillRect(int16_t x, int16_t y, int16_t w, int16_t h,
  uint16_t color) {
  // rudimentary clipping (drawChar w/big text requires this)
  if((x >= _width) || (y >= _height)) return;
  if((x + w) >= _width) w = _width - x - 1;
  if((y + h) >= _height) h = _height - y - 1;

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

void _ILI9340::drawBitmap(int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h,
			      uint16_t color, uint16_t bgcolor) {

  // fillRect copy, except sending color/bgcolor B/W bitmapdata
  // bitmap read as bit stream - no byte align @ row level !
  // rudimentary clipping (drawChar w/big text requires this)
  if((x >= _width) || (y >= _height)) return;
  int16_t h2 = h, w2 = w; // start with requested size
  if((x + w - 1) >= _width)  w2 = _width  - x;
  if((y + h - 1) >= _height) h2 = _height - y;

  setAddrWindow(x, y, x+w2-1, y+h2-1); // smaller window

  uint8_t hi = color >> 8, lo = color;
  uint8_t hiBg = bgcolor >> 8, loBg = bgcolor; // prepare also background color

  //SET_BIT(dcport, dcpinmask);
  //digitalWrite(_dc, HIGH);
  //CLEAR_BIT(csport, cspinmask);
  //digitalWrite(_cs, LOW);

  int16_t i, j;

  unsigned char mask = 128;
  char oneB = *(bitmap++);
  for(j=0; j<h; j++) {
	  for(i=0; i<w; i++ ) {
		  if((j<h2) && (i<w2)) { // draw only when inside bitmap && screen
			if(oneB & mask) { // foreground
				  drawPixel(x+i,y+j,color);
				  spiwrite(hi);
				  spiwrite(lo);
			} else { // background
				  drawPixel(x+i,y+j,bgcolor);
				  spiwrite(hiBg);
				  spiwrite(loBg);
			}
		  }
		  mask >>= 1;
		if(!mask) { // bits sent - take next byte
			  mask = 128;
			  oneB = *(bitmap++);
		  }
	  }
  }
  //digitalWrite(_cs, HIGH);
//  SET_BIT(csport, cspinmask);
}

// Pass 8-bit (each) R,G,B, get back 16-bit packed color
uint16_t _ILI9340::Color565(uint8_t r, uint8_t g, uint8_t b) {
  return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}


void _ILI9340::setRotation(uint8_t m) {

  writecommand(ILI9340_MADCTL);
  rotation = m % 4; // can't be higher than 3
  switch (rotation) {
   case 0:
     writedata(ILI9340_MADCTL_MX | ILI9340_MADCTL_BGR);
     _width = ILI9340_TFTWIDTH;
     _height = ILI9340_TFTHEIGHT;
     break;
   case 1:
     writedata(ILI9340_MADCTL_MV | ILI9340_MADCTL_BGR);
     _width = ILI9340_TFTHEIGHT;
     _height = ILI9340_TFTWIDTH;
     break;
  case 2:
    writedata(ILI9340_MADCTL_MY | ILI9340_MADCTL_BGR);
     _width = ILI9340_TFTWIDTH;
     _height = ILI9340_TFTHEIGHT;
    break;
   case 3:
     writedata(ILI9340_MADCTL_MV | ILI9340_MADCTL_MY | ILI9340_MADCTL_MX | ILI9340_MADCTL_BGR);
     _width = ILI9340_TFTHEIGHT;
     _height = ILI9340_TFTWIDTH;
     break;
  }
}


void _ILI9340::invertDisplay(boolean i) {
  writecommand(i ? ILI9340_INVON : ILI9340_INVOFF);
}


////////// stuff not actively being used, but kept for posterity


uint8_t _ILI9340::spiread(void) {
  uint8_t r = 0;

  return r;
}

 uint8_t _ILI9340::readdata(void) {
   uint8_t r = spiread();
   
   return r;
}
 

 uint8_t _ILI9340::readcommand8(uint8_t c) {
   uint8_t r = spiread();
   return r;
}

#if ARDUINO >= 100
size_t _ILI9340::write(uint8_t c) {
#else
void _ILI9340::write(uint8_t c) {
#endif
	int charWidth;
  if (c == '\n') {
	const unsigned char* begin = 0;//m_font;
	unsigned char height = 8;
    cursor_y += height;
    cursor_x = 0;
  } else if (c == '\r') {
    // skip em
  } else {
	charWidth = cursor_x;
    drawChar(cursor_x, cursor_y, c, textcolor, textbgcolor, textsize);
	if(!m_proportional) {
		cursor_x += textsize*6;
		if(!textsize) cursor_x += 6;
	} else charWidth = cursor_x - charWidth;
    if (wrap && (cursor_x > (_width - textsize*6))) {
      cursor_y += textsize*9-2;
      cursor_x = 0;
    }
  }
/*	if(!m_proportional)
		return 1;
	else return charWidth;*/
  return 1;
}

// draw a character
#include "glcdfont.c"
void _ILI9340::drawChar(int16_t x, int16_t y, unsigned char c,
			    uint16_t color, uint16_t bg, uint8_t size) {

  if((x >= _width)            || // Clip right
     (y >= _height)           || // Clip bottom
     ((x + 8 * size - 1) < 0) || // Clip left
     ((y + 12 * size - 1) < 0))  // Clip top
    if(size) return;

	/*if(!size)*/ {
		m_proportional = false;
		size = 0;
	  for (int8_t i=0; i<6; i++ ) {
		uint8_t line;
		if (i == 5) 
		  line = 0x0;
		else 
		  line = font[(c*5)+i];//pgm_read_byte(font+(c*5)+i);

		size++;
		for (int8_t j = 0; j<8; j++) {
		  if (line & 0x1) {
			if (size == 1) // default size
			  drawPixel(x+i, y+j, color);
			else {  // big size
			int size = 3;
			  fillRect(x+(i*size), y+(j*size), size, size, color);
			} 
		  } else if (bg != color) {
			if (size == 1) // default size
			  drawPixel(x+i, y+j, bg);
			else {  // big size
			  fillRect(x+i*size, y+j*size, size, size, bg);
			} 	
		  }
		  line >>= 1;
		}
		size--;
	  }
	  return;
	}
	return;
}
