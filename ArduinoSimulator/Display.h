#define    WHITE            tft.Color565(255,255,255)
#define    BLACK            tft.Color565(0,0,0)
#define    RED              tft.Color565(200,0,0)
#define    DARK_RED         tft.Color565(100,0,0)
#define    ORANGE           tft.Color565(255,68,0)
#define    GREEN            tft.Color565(0,255,0)
#define    DARK_GREEN       tft.Color565(0,50,0)
#define    BLUE             tft.Color565(13,143,209)
#define    YELLOW           tft.Color565(209,193,13)
#define    DARK_GRAY        tft.Color565(40,40,40)
#define    GRAY             tft.Color565(80,80,80)
#define    LIGHT_GRAY       tft.Color565(150,150,150)
#define    LIGHTER_GRAY     tft.Color565(200,200,200)

#ifndef _FP_DISPLAY_H_
#define _FP_DISPLAY_H_

	#include "SPI.h"
	#include "_GFX.h"
	#include "_ILI9340.h"
	
void showScreen(char round);
void to_string(char *output, int i, char size);
void putPixel(int x, int y/*, int col*/);
void clearSpace(int16_t x, int16_t y, int16_t w, int16_t h);

#endif 
