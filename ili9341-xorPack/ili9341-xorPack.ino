/*
  Simple "Hello World" for ILI9341 LCD

  https://wokwi.com/arduino/projects/308024602434470466
*/

#include "SPI.h"
#include "GFX/Adafruit_GFX.hpp"
#include "GFX/Adafruit_ILI9341.hpp"
#include "FreeSerifBoldItalic9pt7b.hpp"

#define TFT_DC 9
#define TFT_CS 10
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

void setup() {
  tft.begin();

  tft.setFont(&FreeSerifBoldItalic9pt7b);
  tft.setTextColor(ILI9341_RED);
  tft.setTextSize(2);
  for(int i=0;i<='u';i++) tft.write(i);

  // Meme reference: https://english.stackexchange.com/questions/20356/origin-of-i-can-haz
}

void loop() { }
