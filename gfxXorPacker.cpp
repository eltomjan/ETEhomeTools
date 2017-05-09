#include <iostream>
#include <fstream>
#include "cArrayPtr.h"
#define PROGMEM
#define pgm_read_byte(a) *(uint8_t*)(a)
#define pgm_read_word(a) *(uint16_t*)(a)
#define pgm_read_pointer(a) (uint8_t*)(*a)
#include <string>
#include "stdint.h"
#include "gfxfont.h"
#include "fonts\FreeMono12pt7b.h"
#include "fonts\FreeMono18pt7b.h"
#include "fonts\FreeMono24pt7b.h"
#include "fonts\FreeMono9pt7b.h"
#include "fonts\FreeMonoBold12pt7b.h"
#include "fonts\FreeMonoBold18pt7b.h"
#include "fonts\FreeMonoBold24pt7b.h"
#include "fonts\FreeMonoBold9pt7b.h"
#include "fonts\FreeMonoBoldOblique12pt7b.h"
#include "fonts\FreeMonoBoldOblique18pt7b.h"
#include "fonts\FreeMonoBoldOblique24pt7b.h"
#include "fonts\FreeMonoBoldOblique9pt7b.h"
#include "fonts\FreeMonoOblique12pt7b.h"
#include "fonts\FreeMonoOblique18pt7b.h"
#include "fonts\FreeMonoOblique24pt7b.h"
#include "fonts\FreeMonoOblique9pt7b.h"
#include "fonts\FreeSans12pt7b.h"
#include "fonts\FreeSans18pt7b.h"
#include "fonts\FreeSans24pt7b.h"
#include "fonts\FreeSans9pt7b.h"
#include "fonts\FreeSansBold12pt7b.h"
#include "fonts\FreeSansBold18pt7b.h"
#include "fonts\FreeSansBold24pt7b.h"
#include "fonts\FreeSansBold9pt7b.h"
#include "fonts\FreeSansBoldOblique12pt7b.h"
#include "fonts\FreeSansBoldOblique18pt7b.h"
#include "fonts\FreeSansBoldOblique24pt7b.h"
#include "fonts\FreeSansBoldOblique9pt7b.h"
#include "fonts\FreeSansOblique12pt7b.h"
#include "fonts\FreeSansOblique18pt7b.h"
#include "fonts\FreeSansOblique24pt7b.h"
#include "fonts\FreeSansOblique9pt7b.h"
#include "fonts\FreeSerif12pt7b.h"
#include "fonts\FreeSerif18pt7b.h"
#include "fonts\FreeSerif24pt7b.h"
#include "fonts\FreeSerif9pt7b.h"
#include "fonts\FreeSerifBold12pt7b.h"
#include "fonts\FreeSerifBold18pt7b.h"
#include "fonts\FreeSerifBold24pt7b.h"
#include "fonts\FreeSerifBold9pt7b.h"
#include "fonts\FreeSerifBoldItalic12pt7b.h"
#include "fonts\FreeSerifBoldItalic18pt7b.h"
#include "fonts\FreeSerifBoldItalic24pt7b.h"
#include "fonts\FreeSerifBoldItalic9pt7b.h"
#include "fonts\FreeSerifItalic12pt7b.h"
#include "fonts\FreeSerifItalic18pt7b.h"
#include "fonts\FreeSerifItalic24pt7b.h"
#include "fonts\FreeSerifItalic9pt7b.h"
#include "fonts\Org_01.h"
#include "fonts\Picopixel.h"
#include "fonts\TomThumb.h"

using namespace std;
int main(int argc, char *argv[]) {
#define fontsNo 51
	const GFXfont* gfxFontObjs[fontsNo] = { &FreeMono12pt7b, &FreeMono18pt7b, &FreeMono24pt7b, &FreeMono9pt7b, &FreeMonoBold12pt7b, &FreeMonoBold18pt7b, &FreeMonoBold24pt7b, &FreeMonoBold9pt7b, &FreeMonoBoldOblique12pt7b, &FreeMonoBoldOblique18pt7b, &FreeMonoBoldOblique24pt7b, &FreeMonoBoldOblique9pt7b, &FreeMonoOblique12pt7b, &FreeMonoOblique18pt7b, &FreeMonoOblique24pt7b, &FreeMonoOblique9pt7b, &FreeSans12pt7b, &FreeSans18pt7b, &FreeSans24pt7b, &FreeSans9pt7b, &FreeSansBold12pt7b, &FreeSansBold18pt7b, &FreeSansBold24pt7b, &FreeSansBold9pt7b, &FreeSansBoldOblique12pt7b, &FreeSansBoldOblique18pt7b, &FreeSansBoldOblique24pt7b, &FreeSansBoldOblique9pt7b, &FreeSansOblique12pt7b, &FreeSansOblique18pt7b, &FreeSansOblique24pt7b, &FreeSansOblique9pt7b, &FreeSerif12pt7b, &FreeSerif18pt7b, &FreeSerif24pt7b, &FreeSerif9pt7b, &FreeSerifBold12pt7b, &FreeSerifBold18pt7b, &FreeSerifBold24pt7b, &FreeSerifBold9pt7b, &FreeSerifBoldItalic12pt7b, &FreeSerifBoldItalic18pt7b, &FreeSerifBoldItalic24pt7b, &FreeSerifBoldItalic9pt7b, &FreeSerifItalic12pt7b, &FreeSerifItalic18pt7b, &FreeSerifItalic24pt7b, &FreeSerifItalic9pt7b, &Org_01, &Picopixel, &TomThumb };
	int i=0;
	uint8_t c = 33, size = 1;
	for(int i=0;i<fontsNo;i++) {
		c=33;
	  	const GFXfont *gfxFont = gfxFontObjs[i];
        c -= pgm_read_byte(&gfxFont->first);
		int chars = gfxFont->last - gfxFont->first;
        // Character is assumed previously filtered by write() to eliminate
        // newlines, returns, non-printable characters, etc.  Calling drawChar()
        // directly with 'bad' characters of font may cause mayhem!

		uint16_t x=0,y=0;
		string bin;
		string check, check2;
		string newBytes;
		while(c <= chars) {
			GFXglyph *glyph  = &(((GFXglyph *)pgm_read_pointer(&gfxFont->glyph))[c]);
			uint8_t  *bitmap = (uint8_t *)pgm_read_pointer(&gfxFont->bitmap);

			uint16_t bo = pgm_read_word(&glyph->bitmapOffset);
			uint8_t  w  = pgm_read_byte(&glyph->width),
					h  = pgm_read_byte(&glyph->height);
			int8_t   xo = pgm_read_byte(&glyph->xOffset),
					yo = pgm_read_byte(&glyph->yOffset);
			uint8_t  xx, yy, bits = 0, bit = 0;
			int16_t  xo16 = 0, yo16 = 0;
			cArrHolder destBuf((w/8+!!(w&7))*h);

			if(size > 1) {
				xo16 = xo;
				yo16 = yo;
			}

			// Todo: Add character clipping here

			// NOTE: THERE IS NO 'BACKGROUND' COLOR OPTION ON CUSTOM FONTS.
			// THIS IS ON PURPOSE AND BY DESIGN.  The background color feature
			// has typically been used with the 'classic' font to overwrite old
			// screen contents with new data.  This ONLY works because the
			// characters are a uniform size; it's not a sensible thing to do with
			// proportionally-spaced fonts with glyphs of varying sizes (and that
			// may overlap).  To replace previously-drawn text when using a custom
			// font, use the getTextBounds() function to determine the smallest
			// rectangle encompassing a string, erase the area with fillRect(),
			// then draw new text.  This WILL infortunately 'blink' the text, but
			// is unavoidable.  Drawing 'background' pixels will NOT fix this,
			// only creates a new set of problems.  Have an idea to work around
			// this (a canvas object type for MCUs that can afford the RAM and
			// displays supporting setAddrWindow() and pushColors()), but haven't
			// implemented this yet.

			(*destBuf).set2(0);
			for(yy=0; yy<h; yy++) {
				for(xx=0; xx<w; xx++) {
					if(!(bit++ & 7)) {
						bits = pgm_read_byte(&bitmap[bo++]);
					}
					if(bits & 0x80) {
						if(size == 1) {
							bin += '*';
	//                        writePixel(x+xo+xx, y+yo+yy, color);
						} else {
	;//                        writeFillRect(x+(xo16+xx)*size, y+(yo16+yy)*size, size, size, color);
						}
						(*destBuf).setData(*(*destBuf) | (128>>(xx&7)));
					} else
						bin += ' ';
					if(7 == (xx&7)) ++destBuf;
					bits <<= 1;
				}
				if(w&7) ++destBuf;
				bin += "|\n";
			}
//			bin += "\n";
			for(int i=0;i<glyph->width;i++) {
				bin += '-';
			}
			bin += "|\n";
			for(yy=0; yy<h; yy++) {
				for(xx=0; xx<w; xx++) {
				}
			}
			unsigned int dataWidth = w/8 + !!(w&7);
#define size dataWidth*h
			(*destBuf).setPos(0l);
			cArrHolder bufPos(*destBuf, size - dataWidth); // -1 row
			if(h > 1) {
			(*bufPos).moveWindow(dataWidth); // 1 row down
			(*bufPos).setPos((*bufPos).end()); // end
			}
			cArrHolder srcBuf(*destBuf, size - dataWidth); // -1 row
			if(h > 1) {
			(*srcBuf).setPos((*srcBuf).end()); // end -1
			}
			unsigned char mask = 128;
	//		string check;
			const unsigned char *destBuf2 = (*destBuf).getPos();
			if(h > 1) {
	/*		for(int r=0;r<height;r++) {
				for(int c=0;c<dataWidth;c++) {
					mask = 128;
					while(mask) {
						check += mask&*(destBuf2+r*dataWidth+c)?'*':' ';
						mask >>= 1;
					}
				}
				check += '\n';
			}
			cout << check;
			check = "";*/
			do {
				(*bufPos).setData(**bufPos ^ **srcBuf);
				--bufPos; --srcBuf;
			} while(!(*srcBuf).isInvalid());
	/*		for(int r=0;r<height;r++) {
				for(int c=0;c<dataWidth;c++) {
					mask = 128;
					while(mask) {
						check += mask&*(destBuf2+r*dataWidth+c)?'*':' ';
						mask >>= 1;
					}
				}
				check += '\n';
			}
			cout << check;*/
			}
			(*destBuf).setPos(0l);
			destBuf2 = (*destBuf).getPos();
	//		check = "";
			uint16_t byte = 1;
			char *hexa = "0123456789ABCDEF";
			for(int r=0;r<h;r++) {
				mask = 128;
				for(int c=0;mask && c<w;c++) {
					byte <<= 1;
					if(mask&*(destBuf2+r*dataWidth+(c/8))) {
						check += '*';
						if(r) {
							if(check2[check2.length()-w-2] == '*')
								check2 += ' ';
							else
								check2 += '*';
						} else {
							check2 += '*';
						}
						byte++;
					} else {
						check += ' ';
						if(r) {
							if(check2[check2.length()-w-2] == ' ')
								check2 += ' ';
							else
								check2 += '*';
						} else {
							check2 += ' ';
						}
					}
					mask >>= 1;
					if(!mask) {
						mask = 128;
					}
					if(byte > 255) {
						newBytes += "0x";
						byte &= 255;
						newBytes += hexa[byte >> 4];
						newBytes += hexa[byte & 15];
						newBytes += ", ";
						byte = 1;
					}
				}
				check += "|\n";
				check2 += "|\n";
			}
			if((glyph->width * glyph->height) & 7) {
				newBytes += "0x";
				while(byte < 256) byte <<= 1;
				byte &= 255;
				newBytes += hexa[byte >> 4];
				newBytes += hexa[byte & 15];
				newBytes += ",/*";
			} else
				newBytes += "/*";
			for(int i=0;i<glyph->width;i++) {
				check += '-';
				check2 += '-';
			}
			check += "|\n";
			check2 += "|\n";
			uint16_t bo2 = pgm_read_word(&glyph->bitmapOffset);
			while(bo2 < bo) {
				byte = bitmap[bo2++];
				newBytes += "0x";
				newBytes += hexa[byte >> 4];
				newBytes += hexa[byte & 15];
				newBytes += ", ";
			}
			newBytes += "\n";
			newBytes += check2;
			newBytes += check;
			newBytes += "*/";
//			check2 += "\n";
			if(bin != check2) 
				throw "BUG in pack logic !";
			bin = "";
			check = "";
			check2 = "";
//			newBytes += "\n";
			c++;
		}
		string name = "output\\";
		name += i<10?'0':char((i/10)+'0');
		name += char((i%10)+'0');
		ofstream file(name.c_str());
		file << newBytes;
		file.close();
	}
}
