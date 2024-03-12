#pragma once
#include "Font.h"
#include <stdbool.h>

bool readFontFile(fontData *font);
void cleanUpFont(fontData *font);
