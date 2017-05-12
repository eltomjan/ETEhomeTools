#include "Display.h" 
#include "Font_big.h"
#include "Font_symbol.h"
#include "Font_medium.h"

#define _cs 2 //9
#define _dc 1 //8
#define _rst 0 //10

#define BIG_FONT           3

extern const uint8_t Font_big[];
extern const uint8_t Font_medium[];
extern const uint8_t Font_symbol[]; 

_ILI9340 tft = _ILI9340(_cs, _dc, _rst);

#define BG_COLOR DARK_GREEN

void showImage(int xi, int yi, uint8_t *image, int w, int h, int color, int no) {
	while(no--) {
		image += *image;
		image++;
	}
	char real = *image++ / 3;
	if(xi<0) {
		xi = tft.getCursorX();
		yi = tft.getCursorY();
	}
	tft.setCursor(tft.getCursorX() + w, tft.getCursorY());
	int x=xi+tft.getCornerX(), y=yi+tft.getCornerY();
	y += (h-real)/2 + 3;
	h = real;
	int cs = (w/8) * h; // size in octets
	tft.setAddrWindow(x, y, x+w-1, y+h-1);
	for(int j=0;j<h;j++)
		for(int i=0;i<w;i++) {
			unsigned char b = *(image + i/8 + j*(w/8));
			if(b & (128>>(i&7)))
				tft.drawPixel(x+i,y+j,color);
			else
				tft.drawPixel(x+i,y+j,BG_COLOR);
		}
}

void showChar(int x, int y, int number, int font_select, int color, int bgColor) {
	const uint8_t *orig = tft.m_font;
	tft.m_font = const_cast<uint8_t*>(Font_big);
	tft.drawChar(x, y, number + '!', color, bgColor, 1);
	tft.m_font = orig;
}

void showDigit(int x, int y, int number, boolean isSet) {
	showChar(x+9, y+12 , number, BIG_FONT, (isSet)?GREEN:RED, BG_COLOR);
}

void to_string(char *output, int i, char size)
{
	char *p = &output[size - 1];
	char *zero = output;

	if(i<0) {
		do {
			*p-- = '0' - (i%10);
		} while((i=i/10));
		*p = '-';
	} else {
		int limit = size;
		do {
			*p-- = (i%10) + '0';
			limit--;
		} while((i=i/10) && limit);
		p++;
	}
	//	while(zero < p) *(--p) = '0';
}

void showScreen(char round) {
	tft.begin(); 
	tft.m_font = Font_medium;
	tft.setRotation(1);
	int black = BG_COLOR, yellow = YELLOW, green = GREEN;
	tft.fillScreen(BG_COLOR);
	tft.setCorner(34,17);
	tft.setTextColor(yellow,BG_COLOR);
	tft.setTextSize(4);
	tft.setTextColor(green,BG_COLOR);
#include "FreeSerifItalic24pt7b.h"
	tft.setTextSize(1);
	tft.setFont(&FreeSerifItalic24pt7b);
	tft.println("");
	tft.println("!Ubuntu font");
	tft.println("Arduino Simula-\ntor demo");
	tft.setTextColor(yellow,BG_COLOR);
	// Symbol return
	showImage(ILI9340_TFTHEIGHT-60, 75, (uint8_t*)Font_symbol, 24, 25, yellow, 2);
	// Left arrow
	showImage(ILI9340_TFTHEIGHT-60, 120, (uint8_t*)Font_symbol, 24, 25, yellow, 1);
	// Right arrow
	showImage(ILI9340_TFTHEIGHT-60, 145, (uint8_t*)Font_symbol, 24, 25, yellow, 0);
	tft.setCorner(0,0);
	showDigit(0,120,9,true);
	showDigit(51,120,1,true);
	showDigit(102,120,2,true);
	showDigit(153,120,4,true);
	showDigit(204,120,8,true);
}

void clearSpace(int16_t x, int16_t y, int16_t w, int16_t h) {
	tft.fillRect(x,y,w,h,BLACK);
}

void putPixel(int x, int y/*, int col*/) {
#define col WHITE
	tft.drawPixel(x,y,col);
}
