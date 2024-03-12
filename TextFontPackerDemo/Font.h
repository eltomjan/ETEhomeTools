#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

typedef struct charBox_ {
	unsigned char width;
	unsigned char height;
	unsigned char blockSize;
	char bitData[2048]; // string viewer
} charBox;

typedef struct {
	const char *filePath;
	char *buffer_;
	long length;
	unsigned char charactersNo;
	charBox *characters;
} fontData;
