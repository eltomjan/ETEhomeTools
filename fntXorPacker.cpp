#include <iostream>
#include <fstream>
#include <string>

using namespace std;

//===========================================================================
// Params: GLCD.glc G 10x19 chars ATMout.h
//         XORed.h  X 10x19 chars ATMout.h
//===========================================================================
int fntXorPacker(int argc, char *argv[])
{
	int argPos = 2;
    if(argc >= 5) {
		bool xored = false, glcd = false;
		if(!(xored = !strcmp(argv[argPos],"X")) && !(glcd = !strcmp(argv[argPos],"G")))
			return -1;
		argPos++;
        ifstream cFile;
        cFile.open(argv[1], ios::in);
		bool rotated = false;
		if(cFile.is_open()) {
			unsigned int width, height;
			if(argv[argPos][0] == 'R') {
				argPos++;
				rotated = true;
			}
			width = atoi(argv[argPos]);
			height = atoi(strchr(argv[argPos++],'x') + 1);
			int chars = atoi(argv[argPos++]);
			unsigned int dataWidth, dataHeight;
			unsigned int charBufSize, bufSize;
			dataWidth = (width/8) + !!(width&7);
			dataHeight = (height/8) + !!(height&7);
			charBufSize = dataWidth * height;
			if(charBufSize < dataWidth * dataHeight) charBufSize = width * dataHeight;
			bufSize = (chars + 1) * charBufSize;
            string line;
			unsigned int pos = 0;
			bool data = false;
			while(getline(cFile, line)) if(line.length()) {
				pos = 0;
				while(isspace(line[pos])) pos++;
				if(line[pos] == '#') continue;
				if(line.substr(pos,2) == "//") continue;
				if(pos == line.length())continue;
				if((pos = (int)line.find('{')) >= 0) {
					data = true;
					line = line.substr(pos+1);
					break;
				}
            }
			if(data) {
				unsigned int zerosNo0 = 0, zerosNo1 = 0; // # of zeros before and after XOR
				string src, xor;
				unsigned char *fontBuf = NULL;
				unsigned char *nzData = NULL;
				fontBuf = new unsigned char[bufSize];
				memset(fontBuf, 0, bufSize);
				if(glcd) {
					nzData = new unsigned char[bufSize];
				}
				unsigned int rowBitmapSize = height * chars;
				unsigned char *rowBitmap = NULL;
				if(glcd) {
					rowBitmapSize = (rowBitmapSize / 8) + !!(rowBitmapSize & 7);
					rowBitmap = new unsigned char[rowBitmapSize];
					memset(rowBitmap, 0, rowBitmapSize);
				}
				unsigned int colBitmapSize = height * dataWidth * chars;
				unsigned char *colBitmap = NULL;
				if(glcd) {
					colBitmapSize = (colBitmapSize / 8) + !!(colBitmapSize & 7);
					colBitmap = new unsigned char[colBitmapSize];
					memset(colBitmap, 0, colBitmapSize);
				}
				unsigned char *transformBuf = new unsigned char[charBufSize*2];
				unsigned char *bufPos = fontBuf;
				unsigned int charPos = 0;
				string allowed = "0123456789ABCDEFX,";
				do {
					if(line.length()) {
						pos = 0;
						while(isspace(line[pos])) pos++;
						bool hex = false, read = false;
						unsigned char readByte = 0;
						int hexPos;
						while((line.length() >= pos) && (hexPos = (int)allowed.find((char)toupper(line[pos]))) >= 0) {
							if(readByte && hexPos < 16) {
								if(hex) readByte <<= 4; else readByte *= 10;
							}
							if(hexPos < 10 || (hex && hexPos < 16)) {
								read = true;
								readByte += hexPos;
							}
							if(hexPos == 16) hex = true;
							if(line.length() > pos) pos++; else break;
							if(hexPos == 17) {
								read = false;
								*bufPos++ = readByte;
								readByte = 0;
								charPos++;
								pos++;
							}
						}
						if(read) {
							*bufPos++ = readByte;
							charPos++;
						}
						if(glcd && charPos >= dataHeight) { // 1 char read
							unsigned char *srcBuf = bufPos - charPos;
							string oneRead = readGlcChar(srcBuf, width, height);
							bufPos += charBufSize - charPos;
							transformChar(oneRead, bufPos, width, dataWidth, charBufSize, height, zerosNo0);
							xorChar(srcBuf, dataWidth, charBufSize, height, zerosNo1);
							src += oneRead;
							charPos = 0;
							xor += readLinearChar(srcBuf, width, height);
						}
					}
				} while(getline(cFile, line));
				unsigned char rowBMmask = 128, colBMbit = 7;
				unsigned int colByte;
				unsigned char *colBitmapPtr = colBitmap;
				unsigned char *rowBitmapPtr = rowBitmap;
				unsigned char *nzDataPtr = nzData;
				if(xored) {
					nzDataPtr = bufPos;
					bufPos = fontBuf;
					colBitmapSize = ((*bufPos++) << 8);
					colBitmapSize += (*bufPos++);
					chars = *bufPos++;
					dataWidth = *bufPos++;
					height = *bufPos++;
					rowBitmapSize = height * chars; // 89
					rowBitmapSize = (rowBitmapSize / 8) + !!(rowBitmapSize & 7);
					rowBitmap = bufPos;
					rowBitmapPtr = colBitmap = bufPos+rowBitmapSize;
					colBitmapPtr = colBitmap + colBitmapSize;
					nzData = colBitmapPtr;
				} else if(glcd) {
					bufPos = fontBuf;
					for(int i=0;i<chars;i++) {
						for(unsigned int r=0;r<height;r++) { // collect row bitmap (zero or not zero)
							colByte = 0;
							for(unsigned int c=0;c<dataWidth;c++) {
								colByte <<= 1;
								if(*bufPos) {
									*nzDataPtr++ = *bufPos;
									colByte++;
								}
								bufPos++;
							}
							if(colByte) {
								unsigned char shift = colBMbit+9-dataWidth; // shift to 15-th bit
								colByte <<= shift;
								*colBitmapPtr |= (colByte >> 8);
								if(shift < 8)
									*++colBitmapPtr |= (colByte & 255);
								else if(colBMbit < dataWidth)
									colBitmapPtr++;
								colBMbit = (colBMbit - dataWidth) & 7;
								*rowBitmapPtr |= rowBMmask;
							}
							rowBMmask >>= 1;
							if(!rowBMmask) {
								rowBMmask = 128;
								rowBitmapPtr++;
							}
						}
					}
				}
//				delete[] fontBuf;
				string output = "= {"; // start data block
				output += "0x";
				output += allowed[colBitmapSize >> 12];
				output += allowed[(colBitmapSize >> 8) & 15];
				output += ",0x";
				output +=  allowed[(colBitmapSize >> 4) & 15];
				output +=  allowed[colBitmapSize & 15];

				output += ",0x";
				output +=  allowed[chars >> 4];
				output +=  allowed[chars & 15];

				output += ",0x";
				output +=  allowed[dataWidth];

				output += ",0x";
				output +=  allowed[height >> 4];
				output +=  allowed[height & 15];
				output += ",// CollBM size H,L, charsNo, byteWidth, bitHeight\n//RowBMP\n";
				if(*rowBitmapPtr && !xored) rowBitmapPtr++; // uncomplete byte - move end pos
				unsigned char *ptr = rowBitmap;
				while(ptr < rowBitmapPtr) {
					output += "0x";
					output += allowed[(*ptr) >> 4];
					output += allowed[(*ptr++) & 15];
					output += ",";
				}
				output += "\n//ColBMP\n";
				if(*colBitmapPtr && !xored) colBitmapPtr++; // uncomplete byte - move end pos
				colBitmapSize = (unsigned int)(colBitmapPtr - colBitmap);
				ptr = colBitmap;
				while(ptr < colBitmapPtr) {
					output += "0x";
					output += allowed[(*ptr) >> 4];
					output += allowed[(*ptr++) & 15];
					output += ",";
				}
				// Check unpacking from binary arrays
				string check;
				ptr = rowBitmapPtr;
				rowBMmask = 128;
				unsigned char *ptrNz = nzData;
				unsigned char *ptrCB = colBitmap;
				colBMbit = 128;
				ptr = rowBitmap;
				unsigned char charRows = height;
				size_t xoredPos = 0;
				while(ptr < rowBitmapPtr) {
					if(*ptr & rowBMmask) {
						int maxW = width;
						for(unsigned int c=0;c<dataWidth;c++) {
							if(*ptrCB & colBMbit) {
								for(unsigned char m=128;m && maxW--;m>>=1)
									check += (*ptrNz&m)?'*':' ';
								ptrNz++;
							} else {
								for(unsigned int c=0;c<8 && maxW--;c++)
									check += ' ';
							}
							colBMbit >>= 1;
							if(!colBMbit) {
								colBMbit = 128;
								ptrCB++;
							}
						}
					} else {
						for(unsigned int c=0;c<width;c++)
							check += ' ';
					}
					check += "|\n";
					if(!--charRows) {
						for(unsigned int c=0;c<width;c++)
							check += '-';
						check += "\n";
						if(xored) {
							transformChar(check.substr(xoredPos), transformBuf + charBufSize, width, dataWidth, charBufSize, height, zerosNo0);
							xorCharRev(transformBuf, dataWidth, charBufSize, height, zerosNo1);
							xoredPos = check.length();
							src += readLinearChar(transformBuf, width, height);
						}
						charRows = height;
					}
					rowBMmask >>= 1;
					if(!rowBMmask) {
						rowBMmask = 128;
						ptr++;
					}
				}
				if(xored) {
					xor = check;
				}
				// End of check
				output += "\n//Non-zero font data\n";
				ptr = nzData;
				while(ptr < nzDataPtr) {
					output += "0x";
					output += allowed[(*ptr) >> 4];
					output += allowed[(*ptr++) & 15];
					if(ptr < nzDataPtr) output += ",";
				}
				output += "\n};\n";
//				delete[] nzData;
/*				// Save GLCD as ASCII
				if(argc > argPos) {
					ofstream o(argv[argPos++]);
					o << src;
					o.close();
				} else
					cout << src;
				// Save GLCD as XORed ASCII
				if(argc > argPos) {
					ofstream o(argv[argPos++]);
					o << xor;
					o.close();
				} else
					cout << xor;*/
				// Save GLCD as tiny font data
				if(argc > argPos) {
					ofstream o(argv[argPos++]);
					int orig = height*width*chars;
					orig = (orig/8) + !!(orig&7);
					o << "// Zero's no:" << zerosNo1 << "-" << zerosNo0 << "=" << zerosNo1-zerosNo0 << 
						" tabs size 5+" << (rowBitmapPtr-rowBitmap) << "+" << colBitmapSize << "=" << 5+(rowBitmapPtr-rowBitmap)+colBitmapSize <<
						" data:" << (nzDataPtr-nzData) << " of b->B " << orig << " => " << 
						(int)(5+(rowBitmapPtr-rowBitmap)+colBitmapSize+(nzDataPtr-nzData))*100 / orig - 100 << "%" << endl;
					o << output;
					o << "/*" << endl;
					size_t startLine = 0, endLine;
					rowBMmask = 128;
					unsigned char *ptrCB = colBitmap;
					colBMbit = 128;
					ptr = rowBitmap;
					bufPos = nzData;
					int rows = height;
					while((endLine = src.find('\n', startLine+1)) != string::npos) {
						o << src.substr(startLine, endLine - startLine);
						o << xor.substr(startLine, endLine - startLine);
						if(rows > 0) {
							o << (int)!!(*ptr & rowBMmask);
							if(*ptr & rowBMmask) {
								unsigned char cols = 0;
								for(unsigned int c=0;c<dataWidth;c++) {
									cols <<= 1;
									cols |= !!(*ptrCB & colBMbit);
									o << (cols & 1);
									colBMbit >>= 1;
									if(!colBMbit) {
										colBMbit = 128;
										ptrCB++;
									}
								}
								string bin, hexVals;
								unsigned char colsMask = 1 << (dataWidth-1);
								while(cols) {
									unsigned int val = (cols & colsMask)?(*bufPos++):0;
									if(val) {
										hexVals += " 0x";
										hexVals += allowed[val >> 4];
										hexVals += allowed[val & 15];
									}
									val = val*2 + 1;
									while(val&255) {
										if((val & 511) == 256) break;
										if(val&256) bin += '*'; else bin += '.';
										val <<= 1;
									}
									bin += ' ';
									cols <<= 1;
								}
								o << bin.substr(0,width) << hexVals;
							} else {
								for(unsigned int c=0;c<dataWidth;c++)
									o << '-';
							}
						}
						o << endl;
						if(rows > 0)
							rowBMmask >>= 1;
						startLine = endLine+1;
						if(!rowBMmask) {
							rowBMmask = 128;
							ptr++;
						}
						rows--;
						if(rows < 0)
							rows = height;
					}
					o << "*/" << endl;
					o.close();
				} else
					cout << output;
				if(!xored) {
					delete[] nzData;
					delete[] rowBitmap;
					delete[] colBitmap;
				}
				delete[] fontBuf;
				delete[] transformBuf;
			}
        }
    }
    return 0;
}
//===========================================================================
// Convert GLCD bitmap data to ASCII
//===========================================================================
string readGlcChar(unsigned char *srcBuf, unsigned int dataWidth, unsigned int height) {
	string picture;
	unsigned char mask = 1;
	unsigned int bytesRead = 0;
	for(int y=0;height;height--) {
		if(!mask) {
			mask = 1;
			y++;
		}
		for(unsigned int x=0;x<dataWidth;x++) {
//			if(*(srcBuf+x+y*dataWidth) & mask)
			if(*srcBuf++ & mask) picture += '*'; else picture += ' ';
		}
		picture += "|\n";
		mask <<= 1;
		if(mask) srcBuf -= dataWidth;
		else bytesRead += dataWidth;
	}
	for(unsigned int x=0;x<dataWidth;x++)
		picture += "-";
	picture += "\n";
	return picture;
}

//===========================================================================
// Convert ASCII string to common BMP font data
//===========================================================================
void transformChar(string src, unsigned char *destBuf, unsigned int width, unsigned int dataWidth, unsigned int size, unsigned int height, unsigned int& zerosNo0) {
	unsigned char mask = 128;
	unsigned char byte = 0;
	src = src.substr(0, src.length()-2);
	src = src.substr(0, src.find(src[src.length()-1]));
	const char *pos = src.c_str();
	unsigned char *bufPos=destBuf;
	unsigned int bufWritten = 0;
	bool cycle;
#ifdef Testing
	string check;
#endif
	while(*pos) {
		byte = 0;
		while(mask) {
			cycle = false;
			if(*pos == '|') break;
			else if(*pos == '\n') break;
			else if(*pos++ != ' ') byte |= mask;
			cycle = true;
			mask >>= 1;
		}
		if(cycle || ((*pos == '|') && (dataWidth*8 > width))) {
			*bufPos++ = byte;
#ifdef Testing
			mask = 128;
			while(mask) {
				check += mask&byte?'*':' ';
				mask >>= 1;
			}
#endif
			bufWritten++;
		}
		if(!cycle) {
			mask = 128;
			while(*pos++ != '\n');
#ifdef Testing
			check += "skip2LF\n";
#endif
		}
		if(!mask) {
			mask = 128;
		}
	}
#ifdef Testing
	cout << check;
#endif
	memcpy(destBuf - size, destBuf, size);
	bufPos = destBuf - 1;
	unsigned char *srcBuf = destBuf - size + dataWidth - 1;
	for(unsigned int r=0;r<height;r++)
		for(unsigned int c=0;c<dataWidth;c++)
			if(!*(srcBuf+c+(r*dataWidth))) zerosNo0++;
}

//===========================================================================
// XOR data
//===========================================================================
void xorChar(unsigned char *destBuf, unsigned int dataWidth, unsigned int size, unsigned int height, unsigned int& zerosNo1) {
	// XOR from bottom to 2-nd line
	unsigned char *bufPos = destBuf + size - 1;
	unsigned char *srcBuf = destBuf + dataWidth;
	if(dataWidth == 1) {
		srcBuf--;
		bufPos++;
	}
#ifdef Testing
	string check;
	unsigned char mask = 128;
	for(int r=0;r<height;r++) {
		for(int c=0;c<dataWidth;c++) {
			mask = 128;
			while(mask) {
				check += mask&*(destBuf+r*dataWidth+c)?'*':' ';
				mask >>= 1;
			}
		}
		check += '\n';
	}
	cout << check;
#endif
	while(--bufPos > srcBuf) {
		*bufPos ^= *(bufPos-dataWidth);
	}
#ifdef Testing
	for(int r=0;r<height;r++) {
		for(int c=0;c<dataWidth;c++) {
			mask = 128;
			while(mask) {
				check += mask&*(destBuf+r*dataWidth+c)?'*':' ';
				mask >>= 1;
			}
		}
		check += '\n';
	}
	cout << check;
#endif
	for(unsigned int r=0;r<height;r++)
		for(unsigned int c=0;c<dataWidth;c++)
			if(!*(srcBuf+c+(r*dataWidth))) zerosNo1++;
}

//===========================================================================
// unXOR data
//===========================================================================
void xorCharRev(unsigned char *destBuf, unsigned int dataWidth, unsigned int size, unsigned int height, unsigned int& zerosNo1) {
	// XOR from top to bottom-1
	unsigned char *bufPos = destBuf;
	unsigned char *srcBuf = bufPos + size - 2*dataWidth;
	while(bufPos++ < srcBuf) {
		*(bufPos+dataWidth) ^= *bufPos;
	}
	for(unsigned int r=0;r<height;r++)
		for(unsigned int c=0;c<dataWidth;c++)
			if(!*(srcBuf+c+(r*dataWidth))) zerosNo1++;
}

//===========================================================================
// Read common font data 2 string
//===========================================================================
string readLinearChar(unsigned char *srcBuf, unsigned int width, unsigned int height) {
	string picture;
	unsigned char mask = 128;
	unsigned int bytesRead = 0;
	for(unsigned int y=0;y<height;y++) {
		for(unsigned int x=0;x<width;x++) {
			if(*srcBuf & mask) picture += '*'; else picture += ' ';
			mask >>= 1;
			if(!mask) {
				mask = 128;
				srcBuf++;
				bytesRead++;
			}
		}
		if(mask && ((width < 8) || (width & 7)))
			srcBuf++;
		mask = 128;
		bytesRead++;
		picture += "|\n";
	}
	for(unsigned int x=0;x<width;x++)
		picture += "-";
	picture += "\n";
	return picture;
}
