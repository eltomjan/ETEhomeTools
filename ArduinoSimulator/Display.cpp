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

Adafruit_ILI9340 tft = Adafruit_ILI9340(_cs, _dc, _rst);

#define BG_COLOR DARK_GREEN

void showImage(int x, int y, uint8_t *image, int w, int h, int color, int no) {
	while(no--) {
		image += *image;
		image++;
	}
	char real = *image++ / 3;
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
	tft.setRotation(1);
	int black = BG_COLOR, yellow = YELLOW, green = GREEN;
	tft.fillScreen(BG_COLOR);
	tft.setTextColor(green,BG_COLOR);
	tft.setTextSize(1);
#include "Arial10ptP.h"
	tft.setFont(&Arial10pt7b, sizeof(Arial10pt7bBitmaps));
	for (uint16_t i = ' '; i < 128; i++)
	{
		tft.print((char*)&i);
		if ((i-' ') % 20 == 0) tft.println();
	}
	return;
}

void clearSpace(int16_t x, int16_t y, int16_t w, int16_t h) {
	tft.fillRect(x,y,w,h,BLACK);
}

void putPixel(int x, int y/*, int col*/) {
#define col WHITE
	tft.drawPixel(x,y,col);
}
