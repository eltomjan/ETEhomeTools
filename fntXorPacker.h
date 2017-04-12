#include <iostream>
#include <fstream>
#include <string>

using namespace std;
//===========================================================================
// Convert GLCD bitmap data to ASCII
//===========================================================================
string readGlcChar(cArrHolder &srcBuf, unsigned int dataWidth, unsigned int height);
//===========================================================================
// Convert ASCII string to common BMP font data
//===========================================================================
void transformChar(string src, cArrHolder& destBuf, unsigned int width, unsigned int dataWidth, unsigned int size, unsigned int height, unsigned int& zerosNo0);
//===========================================================================
// XOR data
//===========================================================================
void xorChar(cArrHolder& destBuf, unsigned int dataWidth, unsigned int size, unsigned int height, unsigned int& zerosNo1);
//===========================================================================
// unXOR data
//===========================================================================
void xorCharRev(unsigned char *destBuf, unsigned int dataWidth, unsigned int size, unsigned int height, unsigned int& zerosNo1);
//===========================================================================
// Read common font data 2 string
//===========================================================================
string readLinearChar(cArrHolder& srcBuf, unsigned int width, unsigned int height);
//===========================================================================
// Params: GLCD.glc G 10x19 chars ATMout.h
//         XORed.h  X 10x19 chars ATMout.h
//===========================================================================
int fntXorPacker(int argc, char *argv[]);
