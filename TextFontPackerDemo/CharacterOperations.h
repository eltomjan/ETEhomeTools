#pragma once
#include "Font.h"
#include <stdbool.h>

bool convertChars(fontData *font);
charBox *GetNthChar(unsigned char pos, fontData *font);
void XorZeros(charBox *character);

typedef struct {
	unsigned blockSize;
	unsigned bestBlockSize;
} blockStats;
blockStats FindBestBlock(charBox *character);

void HtmlLog(charBox *character, FILE *dest);
