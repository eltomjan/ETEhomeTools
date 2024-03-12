#pragma once
#include "CharacterOperations.h"
#include <string.h>

bool convertChars(fontData *font)
{
	const char *separator = "\n";
	charBox *header = (charBox *)font->buffer_; // in-situ conversion
	font->characters = header;
	char *line = strtok(font->buffer_, separator);
	char *destPos = NULL;
	font->charactersNo = 0;
	do {
		char *delimiterPos = NULL; // WWxHH\r\nRowNbits\r\n...
		unsigned char width = (unsigned char)strtoul(line, &delimiterPos, 16);
		line = ++delimiterPos;
		unsigned char height = (unsigned char)strtoul(line, &delimiterPos, 16);
		header->width = width;
		header->height = height;
		header->blockSize = 1;

		for (int row = 0; row < height; row++)
		{
			line = strtok(NULL, separator);
			destPos = &header->bitData[row * header->width];
			memcpy_s(destPos, header->width, line, header->width);
		}
		delimiterPos = &header->bitData[header->height * header->width];
		*delimiterPos++ = 0; // end string
		header = (charBox *)delimiterPos;
		destPos = line;
		line = strtok(NULL, separator);
		font->charactersNo++;
	} while (line);
	memset(header, 0, (size_t)&((charBox*)0)->bitData[0]);

	return true;
}

charBox *GetNthChar(unsigned char pos, fontData *font)
{
	if (!font || pos > font->charactersNo - 1)
		return NULL;
	charBox *res = font->characters;
	for (unsigned char i = 0; i < pos; i++) {
		intptr_t shift = (intptr_t)&res->bitData[0];
		shift += (res->width * res->height) + 1;
		res = (charBox*)shift;
		if ((res->width == 0 || res->height == 0) && res->blockSize == 0)
			return NULL;
	}
	return res;
}

void XorZeros(charBox *character)
{
	unsigned char y = character->height;
	while (--y)
	{
		for (unsigned char x = 0; x < character->width; x++)
		{
			char *bottom = &character->bitData[(y * character->width) + x];
			char *top = bottom - character->width;
			*bottom = '0' + (*bottom != *top);
		}
	}
}

static unsigned blockSize_(const char *chr, unsigned currentBlock, size_t pos, unsigned bits)
{
	unsigned maxSize = bits - pos;
	if (maxSize > currentBlock) {
		maxSize = currentBlock;
	}
	for (size_t i = 0; i < maxSize; i++)
	{
		if (*(chr + i) != '0') return currentBlock + 1; // 1,block
	}

	return 1; // 0 (block of zeros == single bit)
}

blockStats FindBestBlock(charBox *character)
{
	blockStats currentBlock = { 2 };
	unsigned packedSize;
	unsigned bits = character->height * character->width;
	unsigned minSize = bits;
	while (currentBlock.blockSize < bits / 2)
	{
		char *pos = &character->bitData[0];
		packedSize = 0;
		for (size_t i = 0; i < bits; i += currentBlock.blockSize)
		{
			packedSize += blockSize_(&pos[i], currentBlock.blockSize, i, bits);
		}
		if (minSize > packedSize && packedSize)
		{
			minSize = packedSize;
			currentBlock.bestBlockSize = currentBlock.blockSize;
		}
		currentBlock.blockSize++;
	}
	currentBlock.blockSize = currentBlock.bestBlockSize;
	currentBlock.bestBlockSize = minSize;
	return currentBlock;
}

typedef struct {
	char *xoredData;
	unsigned sourcePos;
	unsigned blockSize_;
	unsigned blockPos;
	char *buffer;
	bool evenOdd;
} blockReader;

static void BlockReader(blockReader *br, char *src, unsigned blockSize)
{
	br->xoredData = src;
	br->sourcePos = 0;
	br->blockSize_ = blockSize;
	br->buffer = malloc(blockSize + 1);
	br->blockPos = blockSize;
	br->evenOdd = false;
}

static char *GetBlock(blockReader *br)
{
	br->evenOdd = !br->evenOdd;
	br->blockPos = 0;
	for (unsigned i = 0; i < br->blockSize_; i++)
	{
		if (br->xoredData[br->sourcePos + i] != '0')
		{
			unsigned pos = br->sourcePos;
			br->sourcePos += br->blockSize_;
			return &br->xoredData[pos];
		}
	}
	br->sourcePos += br->blockSize_;
	memset(br->buffer, '-', br->blockSize_);
	br->buffer[br->blockSize_] = 0;
	return br->buffer;
}

static const char *GetBlockBit(blockReader *br, const char *buf)
{
	if (br->blockPos < br->blockSize_)
	{
		char bit = buf[br->blockPos++];
		switch (bit)
		{
		case '-':
			if (br->blockPos == 1)
			{
				return "<div id=\"000\"></div>";
			}
			if (br->evenOdd)
			{
				return "<div id=\"0\"></div>";
			}
			return "<div id=\"00e\"></div>";
		case '0':
			if (br->evenOdd)
			{
				return "<div id=\"0\"></div>";
			}
			return "<div id=\"00\"></div>";
		case '1':
			return "<div id=\"1\"></div>";
		}
	}
	return NULL;
}

static bool LastOfBlock(blockReader *br)
{
	return br->blockPos > br->blockSize_ - 1;
}

void HtmlLog(charBox *character, FILE *dest)
{
	blockReader blocks;
	BlockReader(&blocks, &character->bitData[0], character->blockSize );
	const char *blockView = NULL;

	for (unsigned char y = 0; y < character->height; y++)
	{
		for (unsigned char x = 0; x < character->width; x++)
		{
			unsigned relativePos = (y * character->height) + x;
			if (LastOfBlock(&blocks)) {
				blockView = GetBlock(&blocks);
			}
			char *bit = GetBlockBit(&blocks, blockView);
			fprintf(dest, "%s", bit);
		}
		fprintf(dest, "<br>\n");
	}
}

static void testPrint(charBox *character)
{
	for (unsigned char y = 0; y < character->height; y++)
	{
		for (unsigned char x = 0; x < character->width; x++)
		{
			char *bottom = &character->bitData[(y * character->width) + x];
			putchar(*bottom);
		}
		putchar('\n');
	}

}
