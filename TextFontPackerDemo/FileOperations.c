#include "Font.h"
#include <stdbool.h>

bool readFontFile(fontData *font)
{
	FILE *f = fopen(font->filePath, "rt");

	if (f)
	{
		fseek(f, 0, SEEK_END);
		font->length = ftell(f);
		fseek(f, 0, SEEK_SET);
		font->buffer_ = malloc(font->length + 1);
		if (font->buffer_)
		{
			fread(font->buffer_, 1, font->length, f);
			font->buffer_[font->length] = 0;
			return true;
		}
		fclose(f);
	}
	return false;
}

void cleanUpFont(fontData *font)
{
	if (font->buffer_) {
		free(font->buffer_);
		font->buffer_ = NULL;
	}
}
